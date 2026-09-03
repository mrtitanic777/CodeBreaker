; nwredir_pcode_05_01.asm -- NWREDIR object 5 segment _PTEXT  (part 1/3)
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        db  080h, 03Dh, 06Ch, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_5_001E
        db  0F6h, 005h, 099h, 003h, 000h, 000h, 080h
        jne    SHORT loc_5_001E
        sub    al, al
        db  086h, 005h, 096h, 003h, 000h, 000h
        or     al, al
        je     SHORT loc_5_0086
loc_5_001E:
        mov    eax, 493e0h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
sub_5_002F:
        pushad
        db  068h, 098h, 003h, 000h, 000h
        db  0E8h, 0FAh, 001h, 0FBh, 0FFh
        add    esp, 4
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_5_0077
        db  0B9h, 0D0h, 056h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        add    ecx, 0fffh
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        db  0C6h, 005h, 095h, 003h, 000h, 000h, 001h
        call   0
loc_5_0077:
        db  068h, 098h, 003h, 000h, 000h
        db  0E8h, 0CEh, 001h, 0FBh, 0FFh
        add    esp, 4
        popad
        ret
loc_5_0086:
        db  068h, 098h, 003h, 000h, 000h
        db  0E8h, 0A4h, 001h, 0FBh, 0FFh
        db  0B9h, 0D0h, 056h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        add    ecx, 0fffh
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        db  0C6h, 005h, 095h, 003h, 000h, 000h, 000h
        db  0E8h, 08Eh, 001h, 0FBh, 0FFh
        add    esp, 4
        ret
sub_5_00C5:
        db  0CDh, 020h, 069h, 080h, 040h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_5_00D0:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 60h]
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 4
        ja     SHORT loc_5_0110
        je     NEAR PTR loc_5_0260
        or     eax, eax
        je     SHORT loc_5_0141
        cmp    eax, 1
        je     SHORT loc_5_014B
        cmp    eax, 2
        je     SHORT loc_5_0169
        cmp    eax, 3
        je     NEAR PTR loc_5_0200
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 049h, 000h
loc_5_0110:
        cmp    eax, 5
        je     NEAR PTR loc_5_0320
        cmp    eax, 6
        je     NEAR PTR loc_5_03B2
        cmp    eax, 7
        je     NEAR PTR loc_5_03D0
        cmp    eax, -1
        je     NEAR PTR loc_5_0492
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_0141:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_014B:
        mov    ecx, dword ptr [esi + 18h]
        or     ecx, ecx
        je     SHORT loc_5_015F
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_5_015F
        db  0A1h, 0A0h, 005h, 000h, 000h
        mov    dword ptr [ecx], eax
loc_5_015F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_0169:
        mov    edi, dword ptr [esi + 18h]
        lea    eax, [esp + 16h]
        push   eax
        db  0E8h, 022h, 00Ah, 0FBh, 0FFh
        add    esp, 4
        or     al, al
        jne    SHORT loc_5_01E7
        mov    word ptr [esp + 20h], 0
        mov    byte ptr [esp + 15h], 0
        mov    dword ptr [esp + 34h], 0
        mov    word ptr [esp + 3ah], 0
        lea    eax, [esp + 1ah]
        lea    edx, [esp + 4ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [esp + 10h]
        mov    word ptr [esp + 52h], 1
        mov    dword ptr [esp + 54h], eax
        mov    word ptr [esp + 58h], 1eh
        lea    eax, [esp + 30h]
        push   eax
        db  0E8h, 060h, 00Ah, 0FBh, 0FFh
        add    esp, 4
        cmp    byte ptr [esp + 38h], 0
        je     SHORT loc_5_01E3
        sub    ebx, ebx
loc_5_01D8:
        db  0E8h, 0C2h, 00Ah, 0FBh, 0FFh
        cmp    byte ptr [esp + 38h], bl
        jne    SHORT loc_5_01D8
loc_5_01E3:
        mov    al, byte ptr [esp + 39h]
loc_5_01E7:
        or     edi, edi
        je     SHORT loc_5_01F3
        cmp    dword ptr [esi + 1ch], 1
        jb     SHORT loc_5_01F3
        mov    byte ptr [edi], al
loc_5_01F3:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 049h, 000h
loc_5_0200:
        mov    edx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 18h]
        or     edx, edx
        je     SHORT loc_5_0254
        cmp    dword ptr [esi + 14h], 8
        jb     SHORT loc_5_0254
        or     ecx, ecx
        je     SHORT loc_5_0254
        cmp    dword ptr [esi + 1ch], 7
        jb     SHORT loc_5_0254
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [edx + 4]
        cmp    dword ptr [eax], 534552h
        jne    SHORT loc_5_0254
        cmp    dword ptr [edi], 454c4946h
        jne    SHORT loc_5_0254
        mov    eax, dword ptr [eax + 10h]
        db  02Bh, 005h, 0A0h, 003h, 000h, 000h
        mov    esi, 0a0h
        cdq
        idiv   esi
        sub    al, 7fh
        mov    byte ptr [ecx], al
        add    edi, 4
        inc    ecx
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [ecx + 4], ax
loc_5_0254:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Bh, 0FFh
loc_5_0260:
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 18h]
        or     eax, eax
        je     NEAR PTR loc_5_0303
        cmp    dword ptr [esi + 14h], 1
        jb     NEAR PTR loc_5_0303
        or     ecx, ecx
        je     NEAR PTR loc_5_0303
        mov    dl, byte ptr [eax]
        cmp    dl, 81h
        jb     SHORT loc_5_02B3
        xor    eax, eax
        mov    al, dl
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 014h, 004h, 000h, 000h
        lea    edi, [eax - 81h]
        cmp    edi, ebx
        jge    SHORT loc_5_02B3
        lea    eax, [eax + eax*4]
        shl    eax, 5
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        lea    edi, [eax + edx - 50a0h]
        jmp    SHORT loc_5_02CB
loc_5_02B3:
        or     dl, dl
        je     SHORT loc_5_0310
        cmp    dl, 8
        ja     SHORT loc_5_0310
        db  0A1h, 0ACh, 003h, 000h, 000h
        and    edx, 0ffh
        mov    edi, dword ptr [eax + edx*4 - 4]
loc_5_02CB:
        cmp    dword ptr [edi + 84h], 4e4e4f43h
        jne    SHORT loc_5_0303
        cmp    byte ptr [edi + 34h], 0
        je     SHORT loc_5_0303
        mov    edx, dword ptr [esi + 1ch]
        cmp    edx, 4
        jb     SHORT loc_5_02ED
        xor    eax, eax
        mov    ax, word ptr [edi + 52h]
        mov    dword ptr [ecx], eax
loc_5_02ED:
        cmp    edx, 8
        jb     SHORT loc_5_02F8
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [ecx + 4], eax
loc_5_02F8:
        cmp    edx, 0ch
        jb     SHORT loc_5_0303
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [ecx + 8], eax
loc_5_0303:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 049h, 000h
loc_5_0310:
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 049h, 000h
loc_5_0320:
        mov    eax, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_5_03A8
        cmp    dword ptr [esi + 14h], 1
        jb     SHORT loc_5_03A8
        or     edi, edi
        je     SHORT loc_5_03A8
        mov    bl, byte ptr [eax]
        xor    ecx, ecx
        db  0A1h, 0DCh, 003h, 000h, 000h
        mov    cl, bl
        mov    eax, dword ptr [eax + ecx*4]
        cmp    bl, 20h
        jae    SHORT loc_5_03A8
        or     eax, eax
        je     SHORT loc_5_03A8
        mov    esi, dword ptr [edi]
        dec    esi
        mov    ebp, dword ptr [eax + 0ch]
        add    ebp, 4
        add    edi, 4
        cmp    word ptr [ebp], 0
        je     SHORT loc_5_0397
        mov    edi, edi
loc_5_0360:
        cmp    esi, 1
        jle    SHORT loc_5_0397
        mov    byte ptr [edi], 5ch
        inc    edi
        dec    esi
        push   1
        push   esi
        xor    eax, eax
        mov    ax, word ptr [ebp]
        sub    eax, 2
        push   eax
        lea    eax, [ebp + 2]
        push   eax
        push   edi
        db  0E8h, 039h, 007h, 0FBh, 0FFh
        add    esp, 14h
        add    edi, eax
        sub    esi, eax
        xor    eax, eax
        mov    ax, word ptr [ebp]
        add    ebp, eax
        cmp    word ptr [ebp], 0
        jne    SHORT loc_5_0360
loc_5_0397:
        mov    byte ptr [edi], 0
        inc    edi
        push   edi
        mov    byte ptr [edi], 0
        push   ebx
        db  0E8h, 090h, 007h, 0FBh, 0FFh
        add    esp, 8
loc_5_03A8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_03B2:
        mov    edx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   edx
        lea    ecx, [edx + 8]
        mov    edx, dword ptr [edx + 0ch]
        db  0E8h, 0ABh, 017h, 0FBh, 0FFh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  090h
loc_5_03D0:
        mov    ebx, dword ptr [esi + 18h]
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_5_0485
        db  08Bh, 015h, 0C0h, 003h, 000h, 000h
        mov    ecx, 0ffffffffh
        lea    ebp, [edx + 4]
        mov    edi, ebp
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
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        lea    ebp, [edx + 35h]
        mov    edi, ebp
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
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        add    edx, 66h
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
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_0485:
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_0492:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_04A0:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        db  0A3h, 054h, 01Bh, 000h, 000h
        mov    byte ptr [esp + 4ch], 0
        mov    byte ptr [esp + 4dh], 0ffh
        db  0A0h, 05Fh, 004h, 000h, 000h
        sub    edi, edi
        db  0A2h, 05Eh, 004h, 000h, 000h
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    NEAR PTR loc_5_055D
        sub    esi, esi
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        add    eax, 0
loc_5_04E0:
        lea    ebx, [esi + ecx]
        cmp    byte ptr [ebx + 34h], 0
        je     SHORT loc_5_054A
        db  0A0h, 05Fh, 004h, 000h, 000h
        mov    byte ptr [ebx + 54h], al
        cmp    byte ptr [ebx + 5ch], 37h
        jne    SHORT loc_5_04FB
        mov    byte ptr [ebx + 5ch], 33h
loc_5_04FB:
        cmp    byte ptr [ebx + 56h], 8
        jae    SHORT loc_5_050A
        push   ebx
        call   sub_5_1EA0
        add    esp, 4
loc_5_050A:
        mov    word ptr [ebx + 50h], 0
        mov    ecx, ebx
        call   sub_5_7230
        or     byte ptr [ebx + 58h], 10h
        push   ebx
        call   sub_5_A6B0
        add    esp, 4
        push   ebx
        db  0E8h, 026h, 008h, 0FBh, 0FFh
        lea    eax, [esp + 10h]
        add    esp, 4
        push   eax
        push   ebx
        call   sub_5_15B0
        add    esp, 8
        push   ebx
        db  0E8h, 04Fh, 008h, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
loc_5_054A:
        add    esi, 0a0h
        inc    edi
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        ja     SHORT loc_5_04E0
loc_5_055D:
        db  0A1h, 0B0h, 005h, 000h, 000h
        push   eax
        db  0E8h, 01Fh, 007h, 0FBh, 0FFh
        add    esp, 4
        db  0A1h, 0ACh, 005h, 000h, 000h
        push   eax
        db  0E8h, 011h, 007h, 0FBh, 0FFh
        add    esp, 4
        db  066h, 0A1h, 0D4h, 005h, 000h, 000h
        push   eax
        db  0E8h, 094h, 006h, 0FBh, 0FFh
        add    esp, 4
        db  066h, 0A1h, 0D2h, 005h, 000h, 000h
        push   eax
        db  0E8h, 085h, 006h, 0FBh, 0FFh
        add    esp, 4
        db  066h, 0A1h, 0CCh, 005h, 000h, 000h
        push   eax
        db  0E8h, 076h, 006h, 0FBh, 0FFh
        add    esp, 4
        push   5204h
        db  0E8h, 069h, 006h, 0FBh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_05C0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_05D0:
        push   edi
        mov    ecx, dword ptr [esp + 8]
        db  08Bh, 015h, 004h, 004h, 000h, 000h
        add    edx, ecx
        mov    ecx, dword ptr [ecx + 4]
        db  0A1h, 0D0h, 003h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [edx], eax
        db  0A1h, 0D4h, 003h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [edx + 4], eax
        mov    byte ptr [edx + 48h], 0
        mov    byte ptr [edx + 49h], 1
        db  0A1h, 0E8h, 003h, 000h, 000h
        mov    byte ptr [ecx + eax], 2
        lea    eax, [edx + 73h]
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [eax + 8], 0ffffffffh
        mov    dword ptr [eax + 0ch], 0ffffffffh
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        jne    SHORT loc_5_0639
        mov    edi, dword ptr [edx]
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    byte ptr [edx + 4ah], al
loc_5_0639:
        mov    edi, dword ptr [edx + 4]
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        call   sub_5_1900
        pop    edi
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_0650:
        sub    esp, 94h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 027h, 003h, 0FBh, 0FFh
        mov    edx, dword ptr [esp + 0a8h]
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        lea    esi, [edx + ecx]
        mov    dword ptr [esp + 14h], esi
        add    ecx, eax
        mov    dword ptr [esp + 18h], ecx
        mov    byte ptr [esp + 9ch], 0
        mov    byte ptr [esp + 9dh], 0ffh
        mov    esi, dword ptr [edx + 4]
        lea    edi, [esp + 3ch]
        db  08Bh, 00Dh, 0E8h, 003h, 000h, 000h
        mov    byte ptr [esi + ecx], 2
        mov    esi, dword ptr [eax + 4]
        db  08Bh, 01Dh, 0D0h, 003h, 000h, 000h
        add    esi, ebx
        mov    ecx, 8
        rep movsd
        mov    esi, dword ptr [eax + 4]
        lea    edi, [esp + 1ch]
        db  0A1h, 0D4h, 003h, 000h, 000h
        add    esi, eax
        mov    ecx, 8
        rep movsd
        mov    ecx, dword ptr [edx + 4]
        lea    ebp, [ebx + ecx]
        lea    edi, [ecx + eax]
        sub    bl, bl
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
loc_5_06D2:
        mov    esi, ebx
        and    esi, 0ffh
        mov    edx, dword ptr [ecx + esi*4]
        or     edx, edx
        je     SHORT loc_5_072D
        lea    eax, [esp + 13h]
        push   eax
        db  068h, 084h, 012h, 000h, 000h
        lea    eax, [esi + 41h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + esi + 48h]
        push   eax
        push   12h
        push   16h
        lea    eax, [esp + 74h]
        db  068h, 088h, 012h, 000h, 000h
        push   eax
        mov    eax, dword ptr [edx + 10h]
        push   eax
        db  0E8h, 072h, 0FBh, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        je     SHORT loc_5_071F
        mov    byte ptr [esp + 13h], 0
loc_5_071F:
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [esi + ebp], al
        mov    al, byte ptr [esp + esi + 1ch]
        mov    byte ptr [edi + esi], al
loc_5_072D:
        inc    bl
        cmp    bl, 20h
        jb     SHORT loc_5_06D2
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ebp
        mov    dword ptr [esi + 4], edi
        add    ecx, 73h
        mov    byte ptr [esi + 48h], 0
        mov    byte ptr [esi + 49h], 1
        mov    edx, esi
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 73h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 77h], eax
        add    edx, 73h
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    byte ptr [esi + 4ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 94h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0780:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 5ch]
        sub    bl, bl
        db  08Bh, 03Dh, 004h, 004h, 000h, 000h
        add    edi, esi
        mov    byte ptr [esp + 50h], 0
        mov    byte ptr [esp + 51h], 0ffh
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
loc_5_07A5:
        xor    eax, eax
        mov    al, bl
        mov    ecx, dword ptr [edx + eax*4]
        or     ecx, ecx
        je     SHORT loc_5_07DF
        mov    ebp, dword ptr [edi]
        mov    al, byte ptr [eax + ebp]
        or     al, al
        je     SHORT loc_5_07DF
        and    eax, 0ffh
        push   eax
        push   14h
        push   16h
        lea    eax, [esp + 1ch]
        db  068h, 090h, 012h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        db  0E8h, 0A9h, 0FAh, 0FAh, 0FFh
        add    esp, 18h
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
loc_5_07DF:
        inc    bl
        cmp    bl, 20h
        jb     SHORT loc_5_07A5
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     SHORT loc_5_081F
        db  0BFh, 0B8h, 014h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 014h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_5_081F
        mov    edi, edi
loc_5_0800:
        cmp    dword ptr [ecx + 3ch], esi
        jne    SHORT loc_5_0816
        cmp    dword ptr [ecx], 534552h
        je     SHORT loc_5_0816
        mov    edx, edi
        call   sub_5_2FB0
        jmp    SHORT loc_5_0819
loc_5_0816:
        lea    edi, [ecx + 4]
loc_5_0819:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    SHORT loc_5_0800
loc_5_081F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0830:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    ecx, 8
        db  003h, 01Dh, 004h, 004h, 000h, 000h
        lea    edx, [ebx + 8]
        mov    edi, edx
        mov    esi, dword ptr [ebx]
        rep movsd
        lea    eax, [ebx + 28h]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 4]
        mov    ecx, 8
        rep movsd
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0870:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 015h, 004h, 004h, 000h, 000h
        add    edx, dword ptr [esp + 5ch]
        mov    eax, edx
        mov    ecx, dword ptr [esp + 5ch]
        db  08Bh, 02Dh, 0D0h, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx + 4]
        add    ebp, ecx
        db  08Bh, 01Dh, 0D4h, 003h, 000h, 000h
        add    ebx, ecx
        sub    eax, dword ptr [edx]
        cmp    eax, -8
        jne    NEAR PTR loc_5_093E
        lea    esi, [edx + 8]
        mov    edi, ebp
        mov    ecx, 8
        rep movsd
        lea    esi, [edx + 28h]
        mov    edi, ebx
        mov    ecx, 8
        rep movsd
        mov    dword ptr [edx], ebp
        mov    dword ptr [edx + 4], ebx
        sub    bl, bl
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        db  08Bh, 03Dh, 00Ch, 004h, 000h, 000h
loc_5_08D0:
        mov    esi, ebx
        and    esi, 0ffh
        mov    eax, dword ptr [ecx + esi*4]
        or     eax, eax
        je     SHORT loc_5_0937
        cmp    byte ptr [esi + ebp], 0
        jne    SHORT loc_5_0937
        mov    ecx, dword ptr [esp + 5ch]
        mov    edx, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ecx + 4]
        add    ecx, edi
        mov    byte ptr [esp + 50h], 0
        mov    byte ptr [esp + 51h], 0ffh
        lea    edi, [esi + ebp]
        push   edi
        db  00Fh, 0B6h, 03Dh, 010h, 004h, 000h, 000h
        imul   edi, esi
        add    edi, ecx
        add    edi, 3
        push   edi
        add    esi, 41h
        push   esi
        push   eax
        push   12h
        push   16h
        lea    eax, [esp + 28h]
        db  068h, 094h, 012h, 000h, 000h
        push   eax
        push   edx
        db  0E8h, 057h, 0F9h, 0FAh, 0FFh
        add    esp, 24h
        db  08Bh, 03Dh, 00Ch, 004h, 000h, 000h
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
loc_5_0937:
        inc    bl
        cmp    bl, 20h
        jb     SHORT loc_5_08D0
loc_5_093E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0950:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        db  066h, 039h, 02Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_09B4
        db  0A1h, 0A0h, 003h, 000h, 000h
        mov    edx, dword ptr [esp + 18h]
        add    eax, 0
loc_5_0970:
        mov    ebx, eax
        cmp    byte ptr [eax + 34h], 0
        je     SHORT loc_5_099F
        lea    esi, [edx + 3eh]
        mov    edi, dword ptr [eax + 40h]
        mov    ecx, 2
        lea    edi, [edi + 0aeh]
        repe cmpsd
        jne    SHORT loc_5_099F
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    SHORT loc_5_099F
        mov    cx, word ptr [eax + 62h]
        cmp    word ptr [edx + 3ch], cx
        je     SHORT loc_5_09BC
loc_5_099F:
        add    eax, 0a0h
        inc    ebp
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, ebp
        ja     SHORT loc_5_0970
        jmp    SHORT loc_5_09BC
loc_5_09B4:
        mov    edx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 10h]
loc_5_09BC:
        cmp    byte ptr [edx + 9], 0
        jne    SHORT loc_5_0A20
        cmp    byte ptr [edx + 4bh], 3fh
        jne    SHORT loc_5_0A20
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, ebp
        jne    SHORT loc_5_09DA
        mov    byte ptr [edx + 4bh], 4eh
        jmp    SHORT loc_5_09EB
loc_5_09DA:
        mov    al, byte ptr [edx + 4ah]
        sub    al, byte ptr [ebx + 73h]
        cmp    al, 1
        sbb    al, al
        and    al, 0bh
        add    al, 4eh
        mov    byte ptr [edx + 4bh], al
loc_5_09EB:
        lea    esi, [edx + 32h]
        mov    eax, dword ptr [edx + 3eh]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 42h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 46h]
        lea    ecx, [edx + 3eh]
        mov    dword ptr [esi + 8], eax
        db  0C7h, 042h, 004h, 040h, 00Ah, 000h, 000h
        cmp    byte ptr [edx + 2ah], 0
        jne    SHORT loc_5_0A2F
        push   edx
        db  0E8h, 017h, 002h, 0FBh, 0FFh
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_0A20:
        cmp    byte ptr [edx + 2ah], 0
        jne    SHORT loc_5_0A2F
        push   edx
        db  0E8h, 044h, 002h, 0FBh, 0FFh
        add    esp, 4
loc_5_0A2F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0A40:
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 040h, 004h, 050h, 009h, 000h, 000h
        cmp    byte ptr [eax + 2ah], 0
        jne    SHORT loc_5_0A5A
        push   eax
        db  0E8h, 019h, 002h, 0FBh, 0FFh
        add    esp, 4
loc_5_0A5A:
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_0A60:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        db  066h, 039h, 02Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_0AC0
        db  0A1h, 0A0h, 003h, 000h, 000h
        mov    edx, dword ptr [esp + 18h]
        add    eax, 0
loc_5_0A80:
        mov    ebx, eax
        cmp    byte ptr [eax + 34h], 0
        je     SHORT loc_5_0AA5
        lea    esi, [edx + 3eh]
        mov    edi, dword ptr [eax + 40h]
        mov    ecx, 2
        lea    edi, [edi + 0aeh]
        repe cmpsd
        jne    SHORT loc_5_0AA3
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
loc_5_0AA3:
        je     SHORT loc_5_0AC8
loc_5_0AA5:
        add    eax, 0a0h
        inc    ebp
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, ebp
        ja     SHORT loc_5_0A80
        jmp    SHORT loc_5_0AC8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_0AC0:
        mov    ebx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 18h]
loc_5_0AC8:
        cmp    byte ptr [edx + 2ah], 0
        jne    SHORT loc_5_0AD7
        push   edx
        db  0E8h, 09Ch, 001h, 0FBh, 0FFh
        add    esp, 4
loc_5_0AD7:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, ebp
        jbe    SHORT loc_5_0AF6
        mov    byte ptr [ebx + 55h], 1
        cmp    byte ptr [ebx + 35h], 0
        jne    SHORT loc_5_0AF6
        push   ebx
        call   sub_5_0B00
        add    esp, 4
loc_5_0AF6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_5_0B00:
        sub    esp, 48h
        push   esi
        mov    esi, dword ptr [esp + 50h]
        cmp    byte ptr [esi + 55h], 0
        je     SHORT loc_5_0B43
        push   esi
        db  0E8h, 03Ch, 002h, 0FBh, 0FFh
        mov    byte ptr [esp + 48h], 0
        mov    byte ptr [esp + 49h], 0ffh
        add    esp, 4
        push   3eh
        mov    eax, dword ptr [esi + 44h]
        push   eax
        db  068h, 050h, 00Bh, 000h, 000h
        push   1
        push   15h
        lea    eax, [esp + 18h]
        db  068h, 09Ch, 012h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0D0h, 002h, 0FBh, 0FFh
        add    esp, 20h
loc_5_0B43:
        pop    esi
        add    esp, 48h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_5_0B50:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, ecx
        mov    ebx, dword ptr [ebp + 90h]
        mov    cl, byte ptr [ebp + 0f6h]
        lea    eax, [ebp + 0f7h]
        mov    dword ptr [esp + 10h], eax
        cmp    byte ptr [ebp + 96h], 0
        jne    NEAR PTR loc_5_0C6A
        or     cl, cl
        je     NEAR PTR loc_5_0C6A
        mov    byte ptr [esp + 54h], 0
        mov    byte ptr [esp + 55h], 0ffh
        cmp    cl, 3ch
        jbe    SHORT loc_5_0B95
        mov    cl, 3ch
loc_5_0B95:
        and    ecx, 0ffh
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [ecx + eax], 0
        db  0A0h, 0EDh, 003h, 000h, 000h
        or     al, al
        je     SHORT loc_5_0BB4
        cmp    al, 2
        jne    NEAR PTR loc_5_0C43
loc_5_0BB4:
        mov    edi, dword ptr [ebx + 3ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  0BFh, 070h, 01Eh, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 070h, 01Eh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  08Dh, 091h, 070h, 01Eh, 000h, 000h
        db  0A1h, 0A4h, 012h, 000h, 000h
        mov    dword ptr [edx], eax
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        mov    edi, dword ptr [esp + 10h]
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
        push   0
        push   0
        push   96h
        db  068h, 070h, 01Eh, 000h, 000h
        db  068h, 0A0h, 010h, 000h, 000h
        db  0E8h, 0B0h, 0FEh, 0FAh, 0FFh
        add    esp, 14h
loc_5_0C43:
        push   3eh
        push   ebp
        db  068h, 050h, 00Bh, 000h, 000h
        push   1
        push   15h
        lea    eax, [esp + 28h]
        db  068h, 0A8h, 012h, 000h, 000h
        push   eax
        push   ebx
        db  0E8h, 0B1h, 001h, 0FBh, 0FFh
        add    esp, 20h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_5_0C6A:
        mov    byte ptr [ebx + 55h], 0
        push   ebx
        db  0E8h, 01Ch, 001h, 0FBh, 0FFh
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  090h
sub_5_0C80:
        push   esi
        mov    esi, ecx
        cmp    byte ptr [esi + 34h], 0
        jne    NEAR PTR loc_5_0D29
        push   1
        push   5ach
        db  0E8h, 025h, 0FDh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_0D29
        mov    byte ptr [esi + 34h], 0ffh
        db  08Ah, 00Dh, 05Eh, 004h, 000h, 000h
        mov    byte ptr [esi + 54h], cl
        mov    dword ptr [esi + 40h], eax
        lea    edx, [eax + 328h]
        mov    dword ptr [esi + 44h], edx
        lea    ecx, [eax + 57ch]
        mov    dword ptr [esi + 3ch], ecx
        mov    byte ptr [esi + 5bh], 0
        mov    dword ptr [esi + 64h], 0
        mov    dword ptr [eax + 0a4h], 444e4553h
        mov    dword ptr [edx + 0cch], 56434552h
        mov    dword ptr [eax + 9ch], 0
        mov    dword ptr [esi + 88h], 0
        mov    byte ptr [ecx], 0
        mov    dword ptr [esi + 30h], 0
        mov    byte ptr [esi + 58h], 10h
        mov    byte ptr [esi + 56h], 0ffh
        mov    byte ptr [esi + 5ch], 0
        mov    dword ptr [esi + 38h], 0
        mov    byte ptr [esi + 5ah], 0
        mov    word ptr [esi + 50h], 0
        mov    word ptr [esi + 92h], 0
loc_5_0D29:
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_0D30:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        db  0E8h, 0B9h, 0FCh, 0FAh, 0FFh
        mov    edi, eax
        cmp    byte ptr [esi + 34h], 0
        je     NEAR PTR loc_5_0E0A
        sub    ebx, ebx
loc_5_0D48:
        mov    eax, dword ptr [esi + 40h]
        cmp    byte ptr [eax + 8], bl
        jne    SHORT loc_5_0D69
        mov    eax, dword ptr [esi + 44h]
        cmp    byte ptr [eax + 8], bl
        jne    SHORT loc_5_0D69
        cmp    dword ptr [esi + 30h], ebx
        jne    SHORT loc_5_0D69
        mov    eax, dword ptr [esi + 64h]
        cmp    eax, ebx
        je     SHORT loc_5_0D9D
        cmp    byte ptr [eax + 8], bl
        je     SHORT loc_5_0D9D
loc_5_0D69:
        db  0E8h, 085h, 0FCh, 0FAh, 0FFh
        lea    ecx, [edi + 7d0h]
        cmp    eax, ecx
        jbe    SHORT loc_5_0D85
        mov    eax, dword ptr [esi + 44h]
        cmp    byte ptr [eax + 8], bl
        jne    SHORT loc_5_0D85
        cmp    dword ptr [esi + 30h], ebx
        je     SHORT loc_5_0D90
loc_5_0D85:
        db  0E8h, 015h, 0FFh, 0FAh, 0FFh
        jmp    SHORT loc_5_0D48
        db  08Dh, 064h, 024h, 000h
loc_5_0D90:
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [eax + 0a4h], 444e6573h
loc_5_0D9D:
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [eax + 0cch], 76636572h
        mov    eax, dword ptr [esi + 40h]
        mov    byte ptr [esi + 34h], 0
        cmp    dword ptr [eax + 0a4h], 444e6573h
        je     SHORT loc_5_0DD2
        mov    dword ptr [eax + 0a4h], 646e6573h
        push   0
        push   eax
        db  0E8h, 002h, 0FCh, 0FAh, 0FFh
        add    esp, 8
loc_5_0DD2:
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_5_0DE7
        push   0
        push   eax
        db  0E8h, 0EDh, 0FBh, 0FAh, 0FFh
        add    esp, 8
loc_5_0DE7:
        mov    dword ptr [esi + 88h], 0
        mov    eax, dword ptr [esi + 64h]
        or     eax, eax
        je     SHORT loc_5_0E03
        push   0
        push   eax
        db  0E8h, 0D1h, 0FBh, 0FAh, 0FFh
        add    esp, 8
loc_5_0E03:
        mov    dword ptr [esi + 64h], 0
loc_5_0E0A:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_5_0E10:
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 50h]
        lea    ebx, [edi + 76h]
        lea    esi, [edi + 75h]
        push   ebx
        push   esi
        push   30h
        mov    eax, dword ptr [edi + 3ch]
        push   eax
        push   11h
        push   17h
        mov    eax, dword ptr [esp + 6ch]
        db  068h, 0B0h, 012h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0D3h, 0FFh, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        jne    NEAR PTR loc_5_13C8
        mov    al, byte ptr [esi]
        xor    edx, edx
        mov    dl, al
        shl    edx, 2
        xor    ecx, ecx
        lea    edx, [edx + edx*4]
        mov    cl, byte ptr [ebx]
        lea    edx, [edx + edx*4]
        add    edx, ecx
        cmp    edx, 0d7h
        jge    SHORT loc_5_0E70
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_5_0E70:
        sub    al, 2
        mov    byte ptr [esi], al
        db  0E8h, 033h, 0FDh, 0FAh, 0FFh
        db  066h, 039h, 005h, 060h, 004h, 000h, 000h
        mov    word ptr [esp + 12h], ax
        jae    SHORT loc_5_0E92
        db  066h, 0A1h, 060h, 004h, 000h, 000h
        mov    word ptr [esp + 12h], ax
loc_5_0E92:
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_5_0EA4
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_5_0EA4:
        sub    word ptr [esp + 12h], 2
        and    word ptr [esp + 12h], 0fffeh
        cmp    byte ptr [edi + 5bh], 0
        je     SHORT loc_5_0EBB
        mov    al, 0ffh
        jmp    SHORT loc_5_0EE2
loc_5_0EBB:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esp + 1ah]
        push   eax
        push   61h
        mov    eax, dword ptr [esp + 64h]
        db  068h, 0B8h, 012h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 031h, 0FFh, 0FAh, 0FFh
        add    esp, 1ch
loc_5_0EE2:
        sub    word ptr [esp + 12h], 1eh
        mov    dword ptr [edi + 24h], 0
        mov    dword ptr [edi + 28h], 0
        mov    cx, word ptr [edi + 6ch]
        mov    bx, 222h
        mov    word ptr [esp + 1ah], cx
        imul   cx, cx, 3
        mov    word ptr [edi + 6ch], cx
        or     al, al
        jne    NEAR PTR loc_5_10F0
        cmp    word ptr [esp + 12h], bx
        jbe    NEAR PTR loc_5_10F0
        xor    eax, eax
        mov    ax, word ptr [esp + 12h]
        push   1
        add    eax, eax
        push   eax
        db  0E8h, 08Fh, 0FAh, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        xor    eax, eax
        mov    ax, word ptr [esp + 12h]
        lea    ecx, [esi + eax]
        mov    dword ptr [esp + 28h], ecx
        or     esi, esi
        je     NEAR PTR loc_5_110F
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [esp + 10h], ax
        mov    dword ptr [esp + 20h], 0
loc_5_0F5C:
        sub    eax, eax
        cmp    word ptr [esp + 10h], ax
        jbe    SHORT loc_5_0F85
loc_5_0F65:
        mov    ecx, eax
        shr    ecx, 8
        xor    cl, byte ptr [esp + 10h]
        xor    cl, al
        xor    cl, 55h
        mov    byte ptr [eax + esi], cl
        inc    eax
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 0ffffh
        cmp    ecx, eax
        ja     SHORT loc_5_0F65
loc_5_0F85:
        db  0E8h, 069h, 0FAh, 0FAh, 0FFh
        mov    ebp, eax
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        push   eax
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        push   eax
        push   esi
        mov    eax, dword ptr [esp + 24h]
        and    eax, 0ffffh
        push   eax
        db  068h, 0C0h, 012h, 000h, 000h
        mov    eax, dword ptr [esp + 6ch]
        push   eax
        push   edi
        db  0E8h, 043h, 0FEh, 0FAh, 0FFh
        add    esp, 20h
        or     al, al
        jne    SHORT loc_5_0FF1
        db  0E8h, 01Ah, 0FAh, 0FAh, 0FFh
        sub    eax, ebp
        mov    bx, word ptr [esp + 10h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, ebx
        and    eax, 0ffffh
        mov    dword ptr [esp + 24h], eax
        jmp    SHORT loc_5_0FFB
loc_5_0FF1:
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esp + 12h], ax
loc_5_0FFB:
        xor    ebp, ebp
        mov    bp, word ptr [esp + 12h]
        mov    ecx, ebx
        and    ecx, 0ffffh
        lea    eax, [ebp + ecx]
        cdq
        sub    eax, edx
        sar    eax, 1
        mov    word ptr [esp + 10h], ax
        add    ecx, 8
        cmp    ecx, ebp
        jl     NEAR PTR loc_5_0F5C
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_5_10E3
        db  0E8h, 0C0h, 0F9h, 0FAh, 0FFh
        mov    ebp, eax
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [esp + 1eh], ax
        mov    ax, word ptr [esp + 1eh]
        xchg   al, ah
        mov    word ptr [esp + 1ch], ax
        push   0ah
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        push   0ah
        push   esi
        mov    eax, dword ptr [esp + 2ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 0C8h, 012h, 000h, 000h
        mov    eax, dword ptr [esp + 6ch]
        push   eax
        push   edi
        db  0E8h, 0A1h, 0FDh, 0FAh, 0FFh
        add    esp, 20h
        db  0E8h, 07Ch, 0F9h, 0FAh, 0FFh
        sub    eax, ebp
        cmp    dword ptr [esp + 24h], 0ah
        jbe    SHORT loc_5_10E3
        cmp    eax, dword ptr [esp + 20h]
        jae    SHORT loc_5_10E3
        mov    edx, dword ptr [esp + 20h]
        sub    edx, eax
        mov    eax, edx
        shl    edx, 5
        sub    edx, eax
        lea    edx, [eax + edx*4]
        mov    ecx, dword ptr [esp + 24h]
        lea    edx, [edx + edx*4]
        sub    ecx, 0ah
        lea    edx, [edx + edx*4]
        shl    edx, 4
        mov    eax, edx
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 20h]
        lea    eax, [eax + eax*4]
        lea    ecx, [eax*2]
        shl    edx, 6
        add    edx, dword ptr [esp + 20h]
        mov    dword ptr [edi + 24h], ecx
        mov    eax, dword ptr [esp + 20h]
        lea    edx, [edx + edx*2]
        lea    edx, [eax + edx*4]
        lea    edx, [eax + edx*4]
        imul   ecx, dword ptr [esp + 24h]
        lea    edx, [edx + edx*4]
        shl    edx, 6
        sub    edx, ecx
        shr    edx, 1
        mov    dword ptr [edi + 28h], edx
loc_5_10E3:
        push   0
        push   esi
        db  0E8h, 0E6h, 0F8h, 0FAh, 0FFh
        add    esp, 8
        jmp    SHORT loc_5_110F
loc_5_10F0:
        lea    eax, [esp + 40h]
        push   eax
        db  0E8h, 09Eh, 0FAh, 0FAh, 0FFh
        add    esp, 4
        or     al, al
        jne    SHORT loc_5_110F
        mov    eax, dword ptr [edi + 78h]
        cmp    eax, dword ptr [esp + 40h]
        jne    SHORT loc_5_110F
        mov    bx, word ptr [esp + 12h]
loc_5_110F:
        mov    ax, word ptr [esp + 1ah]
        mov    word ptr [edi + 6ch], ax
        and    bx, 0fffeh
        mov    word ptr [edi + 4ch], bx
        sub    bx, 14h
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        push   21h
        mov    eax, dword ptr [esp + 60h]
        db  068h, 0D0h, 012h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0CCh, 0FCh, 0FAh, 0FFh
        add    esp, 18h
        or     al, al
        jne    NEAR PTR loc_5_13C8
        mov    ax, word ptr [esp + 10h]
        cmp    ax, bx
        jb     SHORT loc_5_115C
        mov    ax, bx
loc_5_115C:
        mov    word ptr [edi + 4eh], ax
        mov    byte ptr [edi + 55h], 0
        mov    byte ptr [edi + 57h], 1
        mov    dword ptr [edi + 48h], 0
        lea    esi, [edi + 94h]
        mov    byte ptr [edi + 59h], 1
        db  0E8h, 054h, 0F9h, 0FAh, 0FFh
        mov    dword ptr [esi], eax
        lea    eax, [esp + 34h]
        push   0ch
        push   eax
        db  068h, 0D8h, 012h, 000h, 000h
        push   1
        db  068h, 0E8h, 012h, 000h, 000h
        push   3b06h
        push   3dh
        push   17h
        mov    eax, dword ptr [esp + 74h]
        db  068h, 0FCh, 012h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 064h, 0FCh, 0FAh, 0FFh
        add    esp, 2ch
        or     al, al
        jne    SHORT loc_5_1227
        cmp    dword ptr [esp + 34h], 0
        jbe    SHORT loc_5_1227
        cmp    dword ptr [esp + 38h], 1
        ja     SHORT loc_5_1227
        mov    dword ptr [esp + 2ch], 1
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [edi + 98h], eax
        mov    ax, word ptr [esp + 3ch]
        mov    word ptr [edi + 9ch], ax
        mov    al, byte ptr [esp + 3eh]
        mov    byte ptr [edi + 9eh], al
        mov    al, byte ptr [esp + 3fh]
        mov    byte ptr [edi + 9fh], al
        lea    eax, [esp + 2ch]
        push   esi
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        push   0
        push   1
        mov    eax, dword ptr [esp + 68h]
        db  068h, 004h, 013h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0FEh, 0FBh, 0FAh, 0FFh
        add    esp, 20h
        or     al, al
        jne    SHORT loc_5_1227
        cmp    dword ptr [esp + 30h], 8
        jb     SHORT loc_5_1227
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_5_123A
loc_5_1227:
        mov    dword ptr [edi + 98h], 0
        mov    word ptr [edi + 9ch], 0
loc_5_123A:
        db  080h, 03Dh, 069h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_5_13C6
        cmp    byte ptr [edi + 5bh], 0
        ja     NEAR PTR loc_5_13C6
        push   1
        push   12ah
        db  0E8h, 061h, 0F7h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_1270
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_5_1270:
        mov    word ptr [esi + 54h], 0
        mov    word ptr [esi + 2ch], 10h
        mov    word ptr [esi + 2eh], 4
        mov    word ptr [esi + 30h], 2
        lea    eax, [esi + 54h]
        mov    word ptr [esi + 32h], 0
        push   edi
        db  068h, 09Bh, 004h, 000h, 000h
        push   eax
        db  0E8h, 056h, 0F9h, 0FAh, 0FFh
        add    esp, 0ch
        or     al, al
        je     SHORT loc_5_12B9
        push   0
        push   esi
        db  0E8h, 025h, 0F7h, 0FAh, 0FFh
        add    esp, 8
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_5_12B9:
        db  0E8h, 035h, 0F7h, 0FAh, 0FFh
        mov    dword ptr [esi], eax
        mov    bx, word ptr [edi + 4ch]
        sub    bx, 24h
        and    ebx, 0fffch
        mov    eax, 0ffffh
        cdq
        idiv   ebx
        lea    ebp, [esi + 18h]
        mov    edx, eax
        cmp    edx, 0fh
        jb     SHORT loc_5_12E5
        mov    edx, 0fh
loc_5_12E5:
        imul   edx, ebx
        mov    dword ptr [ebp], edx
        cmp    eax, 0ah
        jb     SHORT loc_5_12F5
        mov    eax, 0ah
loc_5_12F5:
        imul   eax, ebx
        mov    dword ptr [esi + 14h], eax
        add    bx, 42h
        lea    ecx, [esi + 14h]
        push   ecx
        lea    ecx, [esp + 14h]
        push   ebp
        push   ecx
        lea    ecx, [esi + 4]
        push   ecx
        push   edx
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 54h]
        push   eax
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        push   65h
        mov    eax, dword ptr [esp + 7ch]
        db  068h, 00Ch, 013h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0DDh, 0FAh, 0FAh, 0FFh
        add    esp, 34h
        or     al, al
        je     SHORT loc_5_1360
        mov    ax, word ptr [esi + 54h]
        push   eax
        db  0E8h, 0D5h, 0F8h, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        db  0E8h, 082h, 0F6h, 0FAh, 0FFh
        add    esp, 8
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_1360:
        mov    ax, word ptr [esp + 10h]
        cmp    ax, bx
        jb     SHORT loc_5_136D
        mov    ax, bx
loc_5_136D:
        sub    ax, 42h
        sub    dword ptr [esi + 14h], 18h
        mov    word ptr [esi + 46h], ax
        sub    dword ptr [ebp], 8
        mov    dword ptr [esi + 8], 0
        xor    eax, eax
        mov    word ptr [esi + 48h], 0
        mov    word ptr [esi + 4ah], 0
        mov    word ptr [esi + 5ah], 0
        mov    word ptr [esi + 5ch], 0
        mov    ecx, 3e8h
        mov    ax, word ptr [edi + 4ch]
        imul   eax, dword ptr [edi + 24h]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [edi + 48h], esi
        mov    byte ptr [edi + 59h], 11h
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_5_13C6:
        sub    al, al
loc_5_13C8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
sub_5_13D0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, esi
        call   sub_5_0C80
        cmp    byte ptr [esi + 34h], 0
        jne    SHORT loc_5_13F2
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_13F2:
        mov    al, byte ptr [esp + 28h]
        mov    byte ptr [esi + 5bh], al
        or     al, al
        je     NEAR PTR loc_5_1500
        mov    word ptr [esi + 60h], 0
        push   0
        db  068h, 062h, 004h, 000h, 000h
        lea    eax, [esi + 60h]
        push   eax
        db  0E8h, 0DCh, 0F7h, 0FAh, 0FFh
        add    esp, 0ch
        or     al, al
        je     SHORT loc_5_1430
        mov    ecx, esi
        call   sub_5_0D30
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_5_1430:
        mov    ax, word ptr [esi + 60h]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        inc    ax
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        lea    edi, [esi + 62h]
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [edi], ax
        push   edi
        db  0E8h, 064h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        or     al, al
        je     SHORT loc_5_1493
        mov    ecx, esi
        call   sub_5_0D30
        mov    ax, word ptr [esi + 60h]
        push   eax
        db  0E8h, 093h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_1493:
        push   1
        push   4ch
        db  0E8h, 022h, 0F5h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_14D0
        mov    ecx, esi
        call   sub_5_0D30
        mov    ax, word ptr [esi + 60h]
        push   eax
        db  0E8h, 065h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, word ptr [edi]
        push   eax
        db  0E8h, 059h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_5_14D0:
        mov    byte ptr [eax + 2ah], 0
        db  0C7h, 040h, 004h, 050h, 009h, 000h, 000h
        mov    cx, word ptr [edi]
        mov    word ptr [eax + 0ah], cx
        lea    ecx, [eax + 2ch]
        mov    word ptr [eax + 22h], 1
        mov    dword ptr [eax + 24h], ecx
        mov    word ptr [eax + 28h], 20h
        mov    dword ptr [esi + 64h], eax
        jmp    SHORT loc_5_1519
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1500:
        lea    eax, [esi + 60h]
        db  066h, 0A1h, 0CCh, 005h, 000h, 000h
        mov    word ptr [esi + 60h], ax
        lea    edi, [esi + 62h]
        db  066h, 0A1h, 0D2h, 005h, 000h, 000h
        mov    word ptr [edi], ax
loc_5_1519:
        mov    ebp, dword ptr [esp + 24h]
        db  068h, 01Ch, 013h, 000h, 000h
        push   ebp
        push   esi
        db  0E8h, 0E7h, 0F8h, 0FAh, 0FFh
        add    esp, 0ch
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_1576
        push   ebp
        push   esi
        call   sub_5_0E10
        add    esp, 8
        mov    bl, al
        or     bl, bl
        je     SHORT loc_5_1555
        db  068h, 020h, 013h, 000h, 000h
        push   ebp
        push   esi
        db  0E8h, 0C2h, 0F8h, 0FAh, 0FFh
        add    esp, 0ch
        or     bl, bl
        jne    SHORT loc_5_1576
loc_5_1555:
        and    byte ptr [esi + 58h], 0efh
        cmp    byte ptr [esp + 28h], 0
        je     SHORT loc_5_159D
        mov    eax, dword ptr [esi + 64h]
        push   eax
        db  0E8h, 007h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_1576:
        cmp    byte ptr [esp + 28h], 0
        je     SHORT loc_5_1596
        mov    ax, word ptr [esi + 60h]
        push   eax
        db  0E8h, 092h, 0F6h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, word ptr [edi]
        push   eax
        db  0E8h, 086h, 0F6h, 0FAh, 0FFh
        add    esp, 4
loc_5_1596:
        mov    ecx, esi
        call   sub_5_0D30
loc_5_159D:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_15B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 48h]
        db  0E8h, 0C7h, 0F3h, 0FAh, 0FFh
        mov    ebx, eax
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
loc_5_15C7:
        lea    edx, [ecx + eax]
        mov    cl, byte ptr [edx + 4ah]
        cmp    cl, 81h
        jb     SHORT loc_5_15F0
        and    ecx, 0ffh
        lea    ecx, [ecx + ecx*4 - 285h]
        shl    ecx, 5
        db  003h, 00Dh, 0A0h, 003h, 000h, 000h
        cmp    ecx, esi
        jne    SHORT loc_5_15F0
        mov    byte ptr [edx + 4ah], 0
loc_5_15F0:
        push   eax
        db  0E8h, 09Bh, 0F3h, 0FAh, 0FFh
        add    esp, 4
        cmp    ebx, eax
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        jne    SHORT loc_5_15C7
        db  0A1h, 0A8h, 005h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_1620
        mov    ecx, 3
loc_5_1611:
        cmp    dword ptr [eax + 10h], esi
        jne    SHORT loc_5_1619
        mov    byte ptr [eax + 1dh], cl
loc_5_1619:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_5_1611
loc_5_1620:
        or     edi, edi
        je     SHORT loc_5_167B
        mov    ax, word ptr [edi + 54h]
        push   eax
        db  0E8h, 0EBh, 0F5h, 0FAh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 0B0h, 003h, 000h, 000h
loc_5_1637:
        sub    ebx, ebx
        lea    eax, [ecx + 8]
        lea    esp, [esp]
loc_5_1640:
        cmp    byte ptr [eax], 0
        je     SHORT loc_5_164D
        cmp    dword ptr [eax + 88h], esi
        je     SHORT loc_5_1660
loc_5_164D:
        add    eax, 9ah
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_5_1640
        jmp    SHORT loc_5_166B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_1660:
        db  0E8h, 03Ah, 0F6h, 0FAh, 0FFh
        db  08Bh, 00Dh, 0B0h, 003h, 000h, 000h
loc_5_166B:
        cmp    ebx, 8
        jb     SHORT loc_5_1637
        push   0
        push   edi
        db  0E8h, 059h, 0F3h, 0FAh, 0FFh
        add    esp, 8
loc_5_167B:
        mov    eax, dword ptr [esi + 64h]
        or     eax, eax
        je     SHORT loc_5_168F
        mov    byte ptr [eax + 2ah], 1
        push   eax
        db  0E8h, 0FBh, 0F5h, 0FAh, 0FFh
        add    esp, 4
loc_5_168F:
        mov    eax, dword ptr [esp + 14h]
        db  068h, 024h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 071h, 0F7h, 0FAh, 0FFh
        add    esp, 0ch
        cmp    byte ptr [esi + 5bh], 0
        je     SHORT loc_5_16C2
        mov    ax, word ptr [esi + 60h]
        push   eax
        db  0E8h, 067h, 0F5h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, word ptr [esi + 62h]
        push   eax
        db  0E8h, 05Ah, 0F5h, 0FAh, 0FFh
        add    esp, 4
loc_5_16C2:
        mov    ecx, esi
        call   sub_5_0D30
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_5_16D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    bl, byte ptr [esi + 56h]
        cmp    byte ptr [esi + 5ch], 37h
        jne    SHORT loc_5_16E5
        mov    byte ptr [esi + 5ch], 33h
loc_5_16E5:
        cmp    bl, 8
        jae    SHORT loc_5_16F3
        push   esi
        call   sub_5_1EA0
        add    esp, 4
loc_5_16F3:
        mov    ecx, esi
        call   sub_5_7230
        or     byte ptr [esi + 58h], 10h
        push   esi
        call   sub_5_A6B0
        add    esp, 4
        push   esi
        db  0E8h, 043h, 0F6h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        push   eax
        push   esi
        call   sub_5_15B0
        add    esp, 8
        push   esi
        db  0E8h, 06Ch, 0F6h, 0FAh, 0FFh
        add    esp, 4
        cmp    bl, 8
        jae    SHORT loc_5_1786
        sub    edi, edi
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_176F
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        sub    eax, eax
        mov    ecx, 0ffh
loc_5_1744:
        mov    esi, edx
        cmp    byte ptr [edx + 34h], al
        je     SHORT loc_5_175B
        cmp    byte ptr [edx + 56h], cl
        jne    SHORT loc_5_175B
        cmp    byte ptr [edx + 5bh], al
        jne    SHORT loc_5_175B
        test   byte ptr [edx + 58h], 10h
        je     SHORT loc_5_176F
loc_5_175B:
        add    edx, 0a0h
        inc    edi
        xor    ebp, ebp
        db  066h, 08Bh, 02Dh, 014h, 004h, 000h, 000h
        cmp    ebp, edi
        ja     SHORT loc_5_1744
loc_5_176F:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        jbe    SHORT loc_5_1786
        xor    edx, edx
        mov    dl, bl
        mov    ecx, esi
        call   sub_5_1D00
loc_5_1786:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_1790:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        db  083h, 03Dh, 0C0h, 005h, 000h, 000h, 000h
        jne    SHORT loc_5_17CF
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 493e0h
        db  0C7h, 045h, 0F8h, 0E0h, 017h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 0C0h, 005h, 000h, 000h
loc_5_17CF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 0C0h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    edi, edi
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    NEAR PTR loc_5_1887
        sub    esi, esi
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        sub    ebx, ebx
        lea    esp, [esp]
loc_5_1810:
        lea    eax, [esi + ecx]
        cmp    byte ptr [eax + 34h], bl
        je     SHORT loc_5_183D
        cmp    byte ptr [eax + 56h], 0ffh
        jne    SHORT loc_5_183D
        cmp    byte ptr [eax + 5bh], bl
        jne    SHORT loc_5_183D
        cmp    byte ptr [eax + 77h], bl
        je     SHORT loc_5_182E
        cmp    word ptr [eax + 50h], bx
        je     SHORT loc_5_1857
loc_5_182E:
        mov    byte ptr [eax + 77h], 1
        call   sub_5_1790
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
loc_5_183D:
        add    esi, 0a0h
        inc    edi
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        ja     SHORT loc_5_1810
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_1857:
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 0
        db  0C7h, 045h, 0F8h, 090h, 018h, 000h, 000h
        push   dword ptr [ebp]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 10h
loc_5_1887:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_5_1890:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_18F8
        sub    esi, esi
        sub    ebx, ebx
loc_5_18A5:
        db  0A1h, 0A0h, 003h, 000h, 000h
        add    eax, esi
        cmp    byte ptr [eax + 34h], bl
        je     SHORT loc_5_18E5
        cmp    byte ptr [eax + 56h], 0ffh
        jne    SHORT loc_5_18E5
        cmp    byte ptr [eax + 5bh], bl
        jne    SHORT loc_5_18E5
        cmp    byte ptr [eax + 77h], bl
        je     SHORT loc_5_18E0
        cmp    word ptr [eax + 50h], bx
        jne    SHORT loc_5_18E0
        mov    byte ptr [esp + 4ch], bl
        mov    byte ptr [esp + 4dh], 0ffh
        lea    ecx, [esp + 0ch]
        push   ecx
        push   eax
        call   sub_5_16D0
        add    esp, 8
        jmp    SHORT loc_5_18E5
loc_5_18E0:
        call   sub_5_1790
loc_5_18E5:
        add    esi, 0a0h
        inc    edi
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        ja     SHORT loc_5_18A5
loc_5_18F8:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  090h
sub_5_1900:
        sub    esp, 4
        db  08Bh, 00Dh, 0B8h, 005h, 000h, 000h
        lea    eax, [ecx + 0ah]
        cmp    word ptr [eax], 0
        jne    SHORT loc_5_1948
        push   0
        db  068h, 0C3h, 004h, 000h, 000h
        push   eax
        db  0E8h, 0D4h, 0F2h, 0FAh, 0FFh
        add    esp, 0ch
        or     al, al
        db  08Bh, 00Dh, 0B8h, 005h, 000h, 000h
        jne    SHORT loc_5_1957
        mov    word ptr [esp + 2], 3
        mov    ax, word ptr [esp + 2]
        xchg   al, ah
        mov    word ptr [esp], ax
        mov    ax, word ptr [esp]
        mov    word ptr [ecx + 48h], ax
loc_5_1948:
        cmp    byte ptr [ecx + 8], 0
        jne    SHORT loc_5_1957
        push   ecx
        db  0E8h, 0D8h, 0F2h, 0FAh, 0FFh
        add    esp, 4
loc_5_1957:
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_1960:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   esi
        mov    word ptr [ebp - 0ch], 3
        mov    ax, word ptr [ebp - 0ch]
        xchg   al, ah
        mov    word ptr [ebp - 0ah], ax
        db  08Bh, 00Dh, 0B8h, 005h, 000h, 000h
        mov    ax, word ptr [ebp - 0ah]
        cmp    word ptr [ecx + 48h], ax
        jne    SHORT loc_5_19A3
        cmp    byte ptr [ecx + 9], 4
        je     SHORT loc_5_19A3
        mov    word ptr [ebp - 8], 1
        mov    ax, word ptr [ebp - 8]
        xchg   al, ah
        mov    word ptr [ebp - 6], ax
        mov    ax, word ptr [ebp - 6]
        jmp    SHORT loc_5_19B7
loc_5_19A3:
        mov    word ptr [ebp - 4], 3
        mov    ax, word ptr [ebp - 4]
        xchg   al, ah
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebp - 2]
loc_5_19B7:
        mov    word ptr [ecx + 48h], ax
        db  083h, 03Dh, 0BCh, 005h, 000h, 000h, 000h
        jne    SHORT loc_5_19F3
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 1ch], 0ea60h
        db  0C7h, 045h, 0ECh, 000h, 01Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebp - 14h]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebp - 10h], esi
        mov    eax, dword ptr [ebp - 10h]
        db  0A3h, 0BCh, 005h, 000h, 000h
loc_5_19F3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_5_1A00:
        db  0A1h, 0B8h, 005h, 000h, 000h
        db  0C7h, 005h, 0BCh, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    word ptr [eax + 0ah], 0
        je     SHORT loc_5_1A2F
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_5_1A2F
        db  066h, 083h, 03Dh, 050h, 004h, 000h, 000h, 000h
        jne    SHORT loc_5_1A2F
        push   eax
        db  0E8h, 000h, 0F2h, 0FAh, 0FFh
        add    esp, 4
loc_5_1A2F:
        ret
sub_5_1A30:
        db  0A1h, 0B8h, 005h, 000h, 000h
        mov    ax, word ptr [eax + 0ah]
        or     ax, ax
        je     SHORT loc_5_1A64
        push   eax
        db  0E8h, 0D5h, 0F1h, 0FAh, 0FFh
        db  0A1h, 0B8h, 005h, 000h, 000h
        add    esp, 4
        mov    word ptr [eax + 0ah], 0
        db  066h, 0C7h, 005h, 050h, 004h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 05Ch, 004h, 000h, 000h, 000h, 000h
loc_5_1A64:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A70:
        sub    esp, 4
        push   ebx
        push   esi
        sub    esi, esi
        db  066h, 039h, 035h, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_1AA5
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        sub    ebx, ebx
loc_5_1A88:
        mov    eax, ecx
        cmp    byte ptr [ecx + 34h], bl
        je     SHORT loc_5_1AA9
        add    ecx, 0a0h
        inc    esi
        xor    edx, edx
        db  066h, 08Bh, 015h, 014h, 004h, 000h, 000h
        cmp    edx, esi
        ja     SHORT loc_5_1A88
        jmp    SHORT loc_5_1AA9
loc_5_1AA5:
        mov    eax, dword ptr [esp + 8]
loc_5_1AA9:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, esi
        jne    SHORT loc_5_1AB8
        sub    eax, eax
loc_5_1AB8:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_5_1AC0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], dx
        mov    esi, ecx
        db  0E8h, 0A8h, 0EEh, 0FAh, 0FFh
        call   sub_5_1A70
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_1AE8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_1AE8:
        db  066h, 083h, 03Dh, 050h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_1B02
        db  0A1h, 0B8h, 005h, 000h, 000h
        mov    cx, word ptr [esp + 12h]
        cmp    word ptr [eax + 4ah], cx
        je     SHORT loc_5_1B71
loc_5_1B02:
        db  0E8h, 0ECh, 0EEh, 0FAh, 0FFh
        mov    ebx, eax
        db  0E8h, 0B0h, 0F0h, 0FAh, 0FFh
        mov    ebp, eax
        db  0A1h, 0B8h, 005h, 000h, 000h
        mov    cx, word ptr [esp + 12h]
        mov    word ptr [eax + 4ah], cx
        db  066h, 0C7h, 005h, 050h, 004h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 05Ch, 004h, 000h, 000h, 000h, 000h
        call   sub_5_1900
        or     ebp, ebp
        je     SHORT loc_5_1B71
        mov    eax, 0ea60h
        sub    edx, edx
        div    ebp
        lea    ebx, [ebx + eax + 0ah]
        db  0E8h, 0A8h, 0EEh, 0FAh, 0FFh
        cmp    eax, ebx
        jae    SHORT loc_5_1B71
        sub    ebp, ebp
loc_5_1B51:
        db  066h, 039h, 02Dh, 050h, 004h, 000h, 000h
        jne    SHORT loc_5_1B71
        db  066h, 039h, 02Dh, 05Ch, 004h, 000h, 000h
        jne    SHORT loc_5_1B71
        db  0E8h, 037h, 0F1h, 0FAh, 0FFh
        db  0E8h, 086h, 0EEh, 0FAh, 0FFh
        cmp    eax, ebx
        jb     SHORT loc_5_1B51
loc_5_1B71:
        push   edi
        db  0E8h, 0D9h, 0F1h, 0FAh, 0FFh
        add    esp, 4
        db  066h, 083h, 03Dh, 050h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_1BAA
        db  0B9h, 046h, 004h, 000h, 000h
        lea    edx, [edi + 78h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        push   0
        push   esi
        push   edi
        call   sub_5_13D0
        add    esp, 0ch
        jmp    SHORT loc_5_1BAC
loc_5_1BAA:
        mov    al, 0ffh
loc_5_1BAC:
        or     al, al
        je     SHORT loc_5_1C07
        db  066h, 083h, 03Dh, 05Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_5_1BDE
        db  0B9h, 052h, 004h, 000h, 000h
        lea    edx, [edi + 78h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        push   0
        push   esi
        push   edi
        call   sub_5_13D0
        add    esp, 0ch
loc_5_1BDE:
        or     al, al
        je     SHORT loc_5_1C07
        db  066h, 0C7h, 005h, 050h, 004h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 05Ch, 004h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 096h, 0F1h, 0FAh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_1C07:
        call   sub_5_1A30
        cmp    word ptr [esp + 12h], 400h
        jne    NEAR PTR loc_5_1CE5
        db  080h, 03Dh, 016h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_5_1CE5
        mov    eax, dword ptr [edi + 3ch]
        db  0BAh, 016h, 004h, 000h, 000h
loc_5_1C2E:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_5_1C50
        or     cl, cl
        je     SHORT loc_5_1C4A
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_5_1C50
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_5_1C2E
loc_5_1C4A:
        sub    eax, eax
        jmp    SHORT loc_5_1C55
        db  08Bh, 0FFh
loc_5_1C50:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1C55:
        or     eax, eax
        je     NEAR PTR loc_5_1CE5
        lea    eax, [esp + 14h]
        push   0ch
        push   eax
        db  068h, 028h, 013h, 000h, 000h
        push   1
        db  068h, 016h, 004h, 000h, 000h
        push   4
        push   3dh
        push   17h
        db  068h, 034h, 013h, 000h, 000h
        push   esi
        push   edi
        db  0E8h, 08Eh, 0F1h, 0FAh, 0FFh
        add    esp, 2ch
        or     al, al
        jne    SHORT loc_5_1CE5
        call   sub_5_1A70
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_1CE5
        push   ebx
        db  0E8h, 0B6h, 0F0h, 0FAh, 0FFh
        lea    edx, [esp + 18h]
        add    esp, 4
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx + 78h], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 7ch], eax
        lea    ecx, [ebx + 78h]
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        push   0
        push   esi
        push   ebx
        call   sub_5_13D0
        add    esp, 0ch
        or     al, al
        je     SHORT loc_5_1CD0
        push   ebx
        db  0E8h, 0C5h, 0F0h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_1CE5
loc_5_1CD0:
        push   esi
        push   edi
        call   sub_5_15B0
        add    esp, 8
        push   edi
        db  0E8h, 0B0h, 0F0h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, ebx
loc_5_1CE5:
        push   edi
        db  0E8h, 0A5h, 0F0h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_1D00:
        sub    esp, 90h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    ebx, ecx
        mov    ecx, edx
        shl    ecx, 5
        db  08Bh, 02Dh, 0A4h, 003h, 000h, 000h
        add    ebp, ecx
        mov    ecx, edx
        shl    ecx, 4
        db  0A1h, 0A8h, 003h, 000h, 000h
        lea    ecx, [ecx + ecx*2]
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
        mov    byte ptr [ebp], 0ffh
        mov    byte ptr [ebp + 18h], 2
        mov    al, byte ptr [ebx + 73h]
        mov    byte ptr [ebp + 17h], al
        mov    byte ptr [ebp + 1bh], al
        mov    al, byte ptr [ebx + 74h]
        mov    byte ptr [ebp + 1ch], al
        mov    ecx, 37h
        mov    al, byte ptr [ebx + 75h]
        mov    byte ptr [ebp + 1dh], al
        mov    al, byte ptr [ebx + 76h]
        mov    byte ptr [ebp + 1fh], al
        xor    eax, eax
        mov    ax, word ptr [ebx + 6ch]
        add    eax, 1bh
        cdq
        idiv   ecx
        mov    word ptr [ebp + 0eh], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 6eh]
        add    eax, 1bh
        cdq
        idiv   ecx
        mov    word ptr [ebp + 19h], ax
        lea    edx, [ebp + 2]
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebx + 7ch]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebx + 80h]
        lea    ecx, [ebx + 78h]
        mov    dword ptr [edx + 8], eax
        mov    edi, dword ptr [ebx + 3ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebx + 38h], ebp
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [ebx + 56h], al
        db  08Bh, 00Dh, 0ACh, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + eax*4], ebx
        db  0A0h, 0ECh, 003h, 000h, 000h
        or     al, al
        jne    SHORT loc_5_1DE7
        mov    al, byte ptr [esp + 10h]
        inc    al
        db  0A2h, 0ECh, 003h, 000h, 000h
        mov    byte ptr [ebp + 1], 1
        jmp    SHORT loc_5_1E55
loc_5_1DE7:
        and    eax, 0ffh
        mov    esi, dword ptr [ecx + eax*4 - 4]
        cmp    dword ptr [esi + 88h], 0
        jne    SHORT loc_5_1E55
        mov    byte ptr [esp + 98h], 0
        mov    byte ptr [esp + 99h], 0ffh
        lea    eax, [esp + 18h]
        push   40h
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 73h]
        push   eax
        push   16h
        push   17h
        lea    eax, [esp + 6ch]
        db  068h, 03Ch, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0E6h, 0EFh, 0FAh, 0FFh
        add    esp, 20h
        or     al, al
        jne    SHORT loc_5_1E38
        cmp    byte ptr [esp + 1eh], 0
        jne    SHORT loc_5_1E55
loc_5_1E38:
        mov    al, byte ptr [esp + 10h]
        inc    al
        db  0A2h, 0ECh, 003h, 000h, 000h
        mov    byte ptr [ebp + 1], 1
        lea    eax, [esp + 58h]
        push   eax
        push   esi
        call   sub_5_16D0
        add    esp, 8
loc_5_1E55:
        sub    eax, eax
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        db  08Bh, 035h, 0CCh, 003h, 000h, 000h
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
loc_5_1E69:
        mov    edx, dword ptr [edi + eax*4]
        or     edx, edx
        je     SHORT loc_5_1E82
        cmp    dword ptr [edx + 10h], ebx
        jne    SHORT loc_5_1E82
        mov    byte ptr [ecx + eax], 1
        mov    dl, byte ptr [esp + 10h]
        inc    dl
        mov    byte ptr [esi + eax], dl
loc_5_1E82:
        inc    eax
        cmp    eax, 20h
        jb     SHORT loc_5_1E69
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 90h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1EA0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 56h]
        db  08Bh, 015h, 0A4h, 003h, 000h, 000h
        mov    ecx, ebx
        shl    ecx, 5
        add    edx, ecx
        mov    ecx, ebx
        shl    ecx, 4
        db  08Bh, 03Dh, 0A8h, 003h, 000h, 000h
        lea    ecx, [ecx + ecx*2]
        add    edi, ecx
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 18h], 0
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        mov    dword ptr [esi + 38h], eax
        mov    byte ptr [esi + 56h], 0ffh
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    dword ptr [eax + ebx*4], 0
        xor    eax, eax
        db  0A0h, 0ECh, 003h, 000h, 000h
        sub    eax, ebx
        cmp    eax, 1
        jne    SHORT loc_5_1F01
        db  0C6h, 005h, 0ECh, 003h, 000h, 000h, 000h
loc_5_1F01:
        db  0E8h, 080h, 0EAh, 0FAh, 0FFh
        mov    edi, eax
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        mov    edi, edi
loc_5_1F10:
        add    ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4ah]
        sub    edx, ebx
        cmp    edx, 1
        jne    SHORT loc_5_1F22
        mov    byte ptr [ecx + 4ah], 0
loc_5_1F22:
        push   eax
        db  0E8h, 069h, 0EAh, 0FAh, 0FFh
        add    esp, 4
        cmp    edi, eax
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        jne    SHORT loc_5_1F10
        sub    eax, eax
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        db  08Bh, 015h, 0CCh, 003h, 000h, 000h
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
loc_5_1F49:
        mov    ebx, dword ptr [edi + eax*4]
        or     ebx, ebx
        je     SHORT loc_5_1F5D
        cmp    dword ptr [ebx + 10h], esi
        jne    SHORT loc_5_1F5D
        mov    byte ptr [ecx + eax], 80h
        mov    byte ptr [edx + eax], 0
loc_5_1F5D:
        inc    eax
        cmp    eax, 20h
        jb     SHORT loc_5_1F49
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1F70:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [esp + 0fh], 8
        sub    edx, edx
        db  08Bh, 035h, 0ACh, 003h, 000h, 000h
loc_5_1F83:
        mov    eax, dword ptr [esi + edx*4]
        or     eax, eax
        je     SHORT loc_5_1F9D
        db  08Ah, 01Dh, 05Eh, 004h, 000h, 000h
        mov    byte ptr [esp + 10h], bl
        cmp    byte ptr [eax + 54h], bl
        je     SHORT loc_5_2012
        or     eax, eax
        jne    SHORT loc_5_1FAB
loc_5_1F9D:
        cmp    byte ptr [esp + 0fh], 8
        jne    SHORT loc_5_1FAB
        lea    eax, [edx + 1]
        mov    byte ptr [esp + 0fh], al
loc_5_1FAB:
        inc    edx
        cmp    edx, 8
        jb     SHORT loc_5_1F83
        sub    edi, edi
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_1FEB
        db  08Bh, 035h, 0A0h, 003h, 000h, 000h
        sub    edx, edx
loc_5_1FC4:
        cmp    byte ptr [esi + 34h], dl
        je     SHORT loc_5_1FD8
        cmp    byte ptr [esi + 5bh], dl
        jne    SHORT loc_5_1FD8
        db  0A0h, 05Eh, 004h, 000h, 000h
        cmp    byte ptr [esi + 54h], al
        je     SHORT loc_5_2021
loc_5_1FD8:
        add    esi, 0a0h
        inc    edi
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        ja     SHORT loc_5_1FC4
loc_5_1FEB:
        mov    edx, 400h
        call   sub_5_1AC0
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_5_2009
        xor    edx, edx
        mov    dl, byte ptr [esp + 0fh]
        dec    edx
        mov    ecx, esi
        call   sub_5_1D00
loc_5_2009:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_2012:
        inc    dl
        db  088h, 015h, 0ECh, 003h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_2021:
        xor    edx, edx
        mov    dl, byte ptr [esp + 0fh]
        dec    edx
        mov    ecx, esi
        call   sub_5_1D00
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_2040:
        push   esi
        mov    esi, ecx
        test   byte ptr [esi + 42h], 4
        je     NEAR PTR loc_5_20D7
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + 1ch]
        shr    eax, 10h
        cmp    al, 81h
        jb     SHORT loc_5_208A
        and    eax, 0ffh
        xor    edx, edx
        db  066h, 08Bh, 015h, 014h, 004h, 000h, 000h
        lea    ecx, [eax - 81h]
        cmp    ecx, edx
        jge    SHORT loc_5_2086
        lea    ecx, [eax + eax*4]
        shl    ecx, 5
        db  0A1h, 0A0h, 003h, 000h, 000h
        lea    eax, [ecx + eax - 50a0h]
        pop    esi
        ret
loc_5_2086:
        sub    eax, eax
        pop    esi
        ret
loc_5_208A:
        or     al, al
        je     SHORT loc_5_20A1
        cmp    al, 8
        ja     SHORT loc_5_20A1
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0ACh, 003h, 000h, 000h
        pop    esi
        mov    eax, dword ptr [eax + ecx*4 - 4]
        ret
loc_5_20A1:
        push   esi
        db  0E8h, 088h, 0EAh, 0FAh, 0FFh
        add    esp, 4
        xor    edx, edx
        mov    dl, al
        dec    edx
        xor    eax, eax
        db  0A0h, 011h, 004h, 000h, 000h
        cmp    eax, edx
        jbe    SHORT loc_5_2114
        db  0A1h, 0DCh, 003h, 000h, 000h
        mov    eax, dword ptr [eax + edx*4]
        or     eax, eax
        je     SHORT loc_5_2114
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        test   byte ptr [edx + ecx], 7fh
        je     SHORT loc_5_2114
        mov    eax, dword ptr [eax + 10h]
        pop    esi
        ret
loc_5_20D7:
        db  0E8h, 09Fh, 0E8h, 0FAh, 0FFh
        db  003h, 005h, 004h, 004h, 000h, 000h
        mov    al, byte ptr [eax + 4ah]
        cmp    al, 81h
        jb     SHORT loc_5_2101
        xor    ecx, ecx
        mov    cl, al
        pop    esi
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 5
        db  0A1h, 0A0h, 003h, 000h, 000h
        lea    eax, [ecx + eax - 50a0h]
        ret
loc_5_2101:
        or     al, al
        je     SHORT loc_5_20A1
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0ACh, 003h, 000h, 000h
        pop    esi
        mov    eax, dword ptr [eax + ecx*4 - 4]
        ret
loc_5_2114:
        db  0E8h, 0B7h, 0EBh, 0FAh, 0FFh
        db  0A0h, 0ECh, 003h, 000h, 000h
        or     al, al
        je     SHORT loc_5_2138
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    esi, dword ptr [eax + ecx*4 - 4]
        db  0E8h, 0FCh, 0EBh, 0FAh, 0FFh
        mov    eax, esi
        pop    esi
        ret
loc_5_2138:
        mov    ecx, esi
        call   sub_5_1F70
        mov    esi, eax
        db  0E8h, 0EAh, 0EBh, 0FAh, 0FFh
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_2150:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_5_215E
loc_5_2155:
        xor    byte ptr [ecx], 0a5h
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_5_2155
loc_5_215E:
        ret
        db  090h
sub_5_2160:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        mov    word ptr [esp + 12h], dx
        mov    eax, dword ptr [ebx + 88h]
        or     eax, eax
        je     SHORT loc_5_2183
        push   0
        push   eax
        db  0E8h, 051h, 0E8h, 0FAh, 0FFh
        add    esp, 8
loc_5_2183:
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebp, dword ptr [esp + 1ch]
        lea    edx, [ecx - 1]
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, 2
        push   edx
        db  0E8h, 009h, 0E8h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 88h], eax
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_5_21D0
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_5_21D0:
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx - 1]
        inc    eax
        mov    dword ptr [ebx + 8ch], eax
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [ebx + 90h], ax
        push   ebp
        db  0E8h, 0CCh, 0E5h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        push   eax
        db  0E8h, 0BFh, 0E5h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebx + 88h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebx + 8ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebx + 88h]
        call   sub_5_2150
        mov    ecx, dword ptr [ebx + 8ch]
        call   sub_5_2150
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    edx, dword ptr [esp + 16h]
        push   ebp
        mov    ecx, ebx
        call   sub_5_2290
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2290:
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    di, dx
        mov    byte ptr [esp + 60h], 0
        mov    byte ptr [esp + 61h], 0ffh
        lea    eax, [esp + 18h]
        push   8
        mov    esi, ecx
        push   eax
        push   17h
        push   17h
        lea    eax, [esp + 30h]
        db  068h, 044h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 050h, 0EBh, 0FAh, 0FFh
        add    esp, 1ch
        or     al, al
        jne    SHORT loc_5_2335
        lea    eax, [esp + 0ch]
        push   4
        mov    ebx, dword ptr [esp + 70h]
        push   eax
        and    edi, 0ffffh
        push   ebx
        push   edi
        push   35h
        push   17h
        lea    eax, [esp + 38h]
        db  068h, 048h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 022h, 0EBh, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        jne    SHORT loc_5_2356
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 1ch]
        lea    ecx, [esp + 10h]
        mov    edx, dword ptr [esp + 74h]
        push   eax
        db  0E8h, 064h, 0F8h, 0FAh, 0FFh
        push   ebx
        push   edi
        lea    eax, [esp + 18h]
        push   8
        push   eax
        push   18h
        push   17h
        lea    eax, [esp + 38h]
        db  068h, 050h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0E7h, 0EAh, 0FAh, 0FFh
        add    esp, 24h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret    8
loc_5_2335:
        mov    eax, dword ptr [esp + 70h]
        mov    ebx, dword ptr [esp + 6ch]
        push   eax
        push   ebx
        push   0
        push   17h
        lea    eax, [esp + 30h]
        db  068h, 058h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0BDh, 0EAh, 0FAh, 0FFh
        add    esp, 1ch
loc_5_2356:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 5ch
        ret    8
        db  090h
sub_5_2360:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        db  068h, 05Ch, 013h, 000h, 000h
        mov    byte ptr [esi + 5ch], 1
        mov    edi, dword ptr [esp + 20h]
        push   edi
        push   esi
        db  0E8h, 091h, 0EAh, 0FAh, 0FFh
        add    esp, 0ch
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_23AB
        push   edi
        push   esi
        call   sub_5_0E10
        add    esp, 8
        mov    bl, al
        or     bl, bl
        je     SHORT loc_5_23B9
        db  068h, 060h, 013h, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 06Ch, 0EAh, 0FAh, 0FFh
        add    esp, 0ch
        or     bl, bl
        je     SHORT loc_5_23B9
loc_5_23AB:
        mov    byte ptr [esi + 5ch], 37h
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_23B9:
        mov    byte ptr [esi + 5ch], 0
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        je     SHORT loc_5_23D3
        mov    cl, byte ptr [esi + 73h]
        mov    byte ptr [eax + 17h], cl
        mov    byte ptr [eax + 1bh], cl
        mov    cl, byte ptr [esi + 74h]
        mov    byte ptr [eax + 1ch], cl
loc_5_23D3:
        mov    ebx, dword ptr [esi + 88h]
        mov    ebp, dword ptr [esi + 8ch]
        mov    ax, word ptr [esi + 90h]
        mov    word ptr [esp + 12h], ax
        or     ebx, ebx
        je     SHORT loc_5_2445
        cmp    byte ptr [ebx], 0
        je     SHORT loc_5_2445
        mov    ecx, ebx
        call   sub_5_2150
        mov    ecx, ebp
        call   sub_5_2150
        push   ebp
        mov    edx, dword ptr [esp + 16h]
        push   ebx
        mov    ecx, esi
        call   sub_5_2290
        mov    byte ptr [esp + 11h], al
        mov    ecx, ebx
        call   sub_5_2150
        mov    ecx, ebp
        call   sub_5_2150
        cmp    byte ptr [esp + 11h], 0
        je     SHORT loc_5_2432
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_2432:
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_2450
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_2445:
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_5_2450:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 20h], edx
        mov    esi, ecx
        db  08Bh, 03Dh, 0A8h, 005h, 000h, 000h
        or     edi, edi
        je     NEAR PTR loc_5_2583
        add    eax, 0
loc_5_2470:
        cmp    dword ptr [edi + 10h], esi
        jne    NEAR PTR loc_5_2578
        mov    ecx, dword ptr [edi + 0ch]
        lea    ebp, [ecx + 4]
        xor    eax, eax
        mov    ax, word ptr [ebp]
        lea    ebx, [ebp + eax]
        xor    eax, eax
        mov    ax, word ptr [ecx + 2]
        xor    edx, edx
        mov    dx, word ptr [eax + ecx]
        add    edx, ecx
        lea    ecx, [eax + edx]
        mov    dword ptr [esp + 24h], ecx
        cmp    byte ptr [edi + 21h], 0ffh
        jne    SHORT loc_5_2505
        lea    eax, [esp + 12h]
        push   eax
        push   1
        push   6
        push   3ah
        push   ebx
        push   ebp
        push   0
        push   0
        push   12h
        push   16h
        mov    eax, dword ptr [esp + 48h]
        db  068h, 064h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 047h, 0E9h, 0FAh, 0FFh
        add    esp, 34h
        or     al, al
        jne    NEAR PTR loc_5_2570
        cmp    word ptr [ebx], 0
        je     NEAR PTR loc_5_255F
        xor    eax, eax
        mov    al, byte ptr [esp + 12h]
        mov    ecx, dword ptr [esp + 24h]
        push   ecx
        push   ebx
        push   eax
        push   eax
        push   0
        push   16h
        mov    eax, dword ptr [esp + 38h]
        db  068h, 070h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 010h, 0E9h, 0FAh, 0FFh
        add    esp, 24h
        jmp    SHORT loc_5_255F
loc_5_2505:
        lea    eax, [esp + 13h]
        push   eax
        push   ebx
        push   ebp
        push   5
        push   16h
        mov    eax, dword ptr [esp + 34h]
        db  068h, 078h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0F0h, 0E8h, 0FAh, 0FFh
        add    esp, 20h
        or     al, al
        jne    SHORT loc_5_2570
        lea    eax, [esp + 12h]
        push   eax
        mov    ecx, dword ptr [esp + 28h]
        push   0
        push   ecx
        push   ebp
        push   0ffh
        push   0
        push   0
        xor    eax, eax
        mov    al, byte ptr [esp + 2fh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 21h]
        push   eax
        push   0ch
        mov    eax, dword ptr [esp + 48h]
        db  068h, 080h, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0B4h, 0E8h, 0FAh, 0FFh
        add    esp, 34h
loc_5_255F:
        or     al, al
        jne    SHORT loc_5_2570
        mov    byte ptr [edi + 1dh], 0
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [edi + 1eh], al
        jmp    SHORT loc_5_2578
loc_5_2570:
        mov    byte ptr [edi + 1dh], 3
        mov    byte ptr [edi + 1eh], 0
loc_5_2578:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    NEAR PTR loc_5_2470
loc_5_2583:
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_5_265A
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 0F8h, 0DBh, 0FAh, 0FFh
        add    esp, 4
        db  0E8h, 0E4h, 0E3h, 0FAh, 0FFh
        mov    dword ptr [esp + 28h], eax
        mov    edi, eax
        db  0A1h, 004h, 004h, 000h, 000h
loc_5_25AD:
        add    eax, edi
        mov    dword ptr [esp + 14h], eax
        sub    bl, bl
        db  038h, 01Dh, 011h, 004h, 000h, 000h
        je     SHORT loc_5_2633
loc_5_25C0:
        xor    ecx, ecx
        db  0A1h, 0DCh, 003h, 000h, 000h
        mov    cl, bl
        mov    edx, dword ptr [eax + ecx*4]
        or     edx, edx
        je     SHORT loc_5_2629
        cmp    dword ptr [edx + 10h], esi
        jne    SHORT loc_5_2629
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        mov    byte ptr [ecx + eax], 0
        test   byte ptr [edi], 18h
        jne    SHORT loc_5_2629
        mov    ebp, dword ptr [edi + 4]
        db  0A1h, 00Ch, 004h, 000h, 000h
        add    eax, ebp
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        add    eax, ecx
        push   eax
        xor    eax, eax
        db  0A0h, 010h, 004h, 000h, 000h
        imul   eax, ecx
        add    eax, dword ptr [esp + 1ch]
        add    eax, 3
        push   eax
        add    ecx, 41h
        push   ecx
        push   edx
        push   12h
        push   16h
        mov    eax, dword ptr [esp + 38h]
        db  068h, 08Ch, 013h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0EAh, 0E7h, 0FAh, 0FFh
        add    esp, 24h
loc_5_2629:
        inc    bl
        db  038h, 01Dh, 011h, 004h, 000h, 000h
        ja     SHORT loc_5_25C0
loc_5_2633:
        push   edi
        db  0E8h, 058h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        cmp    edi, dword ptr [esp + 28h]
        db  0A1h, 004h, 004h, 000h, 000h
        jne    NEAR PTR loc_5_25AD
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 052h, 0DBh, 0FAh, 0FFh
        add    esp, 4
loc_5_265A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 083h, 0ECh
        db  050h, 053h, 056h, 057h, 055h, 033h, 0C9h, 0A1h, 0ACh, 003h, 000h, 000h, 08Ah, 04Ch, 024h, 064h
        db  08Bh, 004h, 088h, 089h, 044h, 024h, 014h, 0C6h, 044h, 024h, 058h, 000h, 0C6h, 044h, 024h, 059h
        db  0FFh, 08Bh, 07Ch, 024h, 070h, 081h, 0E7h, 0FFh, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 06Ch, 057h
        db  050h, 0E8h, 0A8h, 0E3h, 0FAh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0D8h, 00Bh, 0DBh, 074h, 05Ch, 08Bh
        db  074h, 024h, 078h, 081h, 0E6h, 0FFh, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 074h, 056h, 050h, 0E8h
        db  08Ah, 0E3h, 0FAh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0E8h, 00Bh, 0EDh, 074h, 034h, 056h, 055h, 057h
        db  053h, 033h, 0C0h, 08Ah, 044h, 024h, 078h, 050h, 08Dh, 044h, 024h, 02Ch, 068h, 094h, 013h, 000h
        db  000h, 050h, 08Bh, 044h, 024h, 030h, 050h, 0E8h, 091h, 0DBh, 0FAh, 0FFh, 088h, 044h, 024h, 033h
        db  083h, 0C4h, 020h, 057h, 053h, 0E8h, 091h, 0E3h, 0FAh, 0FFh, 083h, 0C4h, 008h, 056h, 055h, 0EBh
        db  002h, 057h, 053h, 0E8h, 083h, 0E3h, 0FAh, 0FFh, 083h, 0C4h, 008h, 08Ah, 044h, 024h, 013h, 05Dh
        db  05Fh, 05Eh, 05Bh, 083h, 0C4h, 050h, 0C3h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2720:
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  068h, 0B8h, 010h, 000h, 000h
        db  0E8h, 003h, 0DBh, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 88h]
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ecx + 1ch]
        cmp    eax, 3
        jl     NEAR PTR loc_5_2CD0
        cmp    eax, 4
        jg     NEAR PTR loc_5_2CD0
        mov    al, byte ptr [ecx + 14h]
        mov    byte ptr [esp + 15h], al
        mov    cl, byte ptr [ecx + 15h]
        mov    byte ptr [esp + 13h], cl
        mov    ecx, dword ptr [esp + 84h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    ecx, dword ptr [esp + 80h]
        call   sub_5_2040
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        jne    SHORT loc_5_2796
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 33h
        jmp    NEAR PTR loc_5_2CB5
loc_5_2796:
        mov    ecx, dword ptr [esp + 84h]
        mov    al, byte ptr [ecx + 1ch]
        cmp    al, 3
        jne    SHORT loc_5_27AE
        cmp    byte ptr [ecx + 10h], 0
        jne    NEAR PTR loc_5_2CD0
loc_5_27AE:
        mov    ecx, dword ptr [esp + 80h]
        cmp    al, 4
        mov    word ptr [ecx + 1ah], 0
        jne    SHORT loc_5_27C6
        mov    edx, 3c04h
        jmp    SHORT loc_5_27D2
loc_5_27C6:
        mov    edx, 3800h
        mov    ecx, dword ptr [esp + 80h]
loc_5_27D2:
        call   sub_5_6D80
        mov    ebx, eax
        push   ebx
        db  0E8h, 0E7h, 0DFh, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [ebx], 5ch
        jne    SHORT loc_5_27EE
        inc    ebx
        cmp    byte ptr [ebx], 5ch
        jne    SHORT loc_5_27EE
        inc    ebx
loc_5_27EE:
        sub    ebp, ebp
        cmp    byte ptr [ebx], 0
        je     SHORT loc_5_2802
loc_5_27F5:
        cmp    byte ptr [ebx + ebp], 5ch
        je     SHORT loc_5_2808
        inc    ebp
        cmp    byte ptr [ebx + ebp], 0
        jne    SHORT loc_5_27F5
loc_5_2802:
        cmp    byte ptr [ebx + ebp], 5ch
        jne    SHORT loc_5_2830
loc_5_2808:
        mov    ecx, dword ptr [esp + 84h]
        cmp    byte ptr [ecx + 1ch], 4
        je     NEAR PTR loc_5_2CD0
        mov    ecx, dword ptr [esp + 80h]
        mov    byte ptr [esp + 15h], 1
        test   byte ptr [ecx + 42h], 4
        jne    SHORT loc_5_2830
        mov    byte ptr [esp + 13h], 0
loc_5_2830:
        sub    edx, edx
        db  066h, 039h, 015h, 014h, 004h, 000h, 000h
        jbe    SHORT loc_5_2887
        db  0A1h, 0A0h, 003h, 000h, 000h
loc_5_2840:
        mov    dword ptr [esp + 1ch], eax
        cmp    byte ptr [eax + 34h], 0
        je     SHORT loc_5_2874
        mov    ecx, ebp
        shr    ecx, 2
        mov    esi, ebx
        mov    edi, dword ptr [eax + 3ch]
        repe cmpsd
        jne    SHORT loc_5_2874
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_5_2874
        mov    ecx, dword ptr [eax + 3ch]
        cmp    byte ptr [ebp + ecx], 0
        jne    SHORT loc_5_2874
        mov    cl, byte ptr [esp + 13h]
        cmp    byte ptr [eax + 5bh], cl
        je     SHORT loc_5_2887
loc_5_2874:
        add    eax, 0a0h
        inc    edx
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, edx
        ja     SHORT loc_5_2840
loc_5_2887:
        mov    ecx, dword ptr [esp + 84h]
        mov    al, byte ptr [ecx + 1ch]
        cmp    al, 4
        je     NEAR PTR loc_5_2C12
        cmp    byte ptr [ebx + ebp], 0
        jne    SHORT loc_5_2912
        cmp    byte ptr [esp + 15h], 3
        je     NEAR PTR loc_5_2C12
        cmp    byte ptr [ebx + ebp], 0
        jne    SHORT loc_5_2912
        cmp    byte ptr [esp + 15h], 0
        je     SHORT loc_5_28C3
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edx
        jbe    SHORT loc_5_2912
loc_5_28C3:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edx
        jbe    SHORT loc_5_2900
        mov    ecx, dword ptr [esp + 1ch]
        mov    al, byte ptr [ecx + 56h]
        cmp    al, 0ffh
        jne    SHORT loc_5_28E0
        lea    eax, [edx - 7fh]
        jmp    SHORT loc_5_28E2
        db  090h
loc_5_28E0:
        inc    al
loc_5_28E2:
        xor    edx, edx
        mov    dl, al
        mov    ecx, dword ptr [esp + 84h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1ch]
        shl    edx, 10h
        add    edx, eax
        mov    dword ptr [ecx + 1ch], edx
        jmp    NEAR PTR loc_5_2CB5
loc_5_2900:
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0ffh
        jmp    NEAR PTR loc_5_2CB5
loc_5_2912:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edx
        jne    NEAR PTR loc_5_2B4F
        cmp    byte ptr [esp + 15h], 2
        jne    SHORT loc_5_2947
        mov    word ptr [esp + 1ah], 7b06h
        mov    ax, word ptr [esp + 1ah]
        xchg   al, ah
        mov    word ptr [esp + 18h], ax
        mov    eax, dword ptr [esp + 18h]
        and    eax, 0ffffh
        jmp    SHORT loc_5_294C
loc_5_2947:
        mov    eax, 4
loc_5_294C:
        lea    ecx, [esp + 5ah]
        push   0ch
        push   ecx
        db  068h, 0A0h, 013h, 000h, 000h
        push   1
        push   ebp
        push   ebx
        push   ebp
        push   eax
        push   3dh
        push   17h
        mov    ecx, dword ptr [esp + 0a8h]
        db  068h, 0ACh, 013h, 000h, 000h
        mov    eax, dword ptr [esp + 50h]
        push   ecx
        push   eax
        db  0E8h, 006h, 0D9h, 0FAh, 0FFh
        mov    byte ptr [esp + 48h], al
        add    esp, 34h
        or     al, al
        je     SHORT loc_5_29BF
        cmp    byte ptr [esp + 15h], 1
        jne    SHORT loc_5_29BF
        db  0E8h, 0B7h, 0E1h, 0FAh, 0FFh
        or     eax, eax
        je     SHORT loc_5_29BF
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 17h], al
        mov    byte ptr [ebx + ebp], 0
        lea    eax, [esp + 28h]
        push   eax
        push   ebx
        push   7d06h
        push   0
        db  0E8h, 0A5h, 0E1h, 0FAh, 0FFh
        mov    byte ptr [esp + 24h], al
        mov    al, byte ptr [esp + 27h]
        add    esp, 10h
        mov    byte ptr [ebx + ebp], al
loc_5_29BF:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_5_29D8
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0efh
        jmp    NEAR PTR loc_5_2CB5
loc_5_29D8:
        mov    dword ptr [esp + 20h], 0
        db  066h, 083h, 03Dh, 014h, 004h, 000h, 000h, 000h
        jbe    SHORT loc_5_2A33
        db  0A1h, 0A0h, 003h, 000h, 000h
        sub    edx, edx
loc_5_29F1:
        mov    dword ptr [esp + 1ch], eax
        cmp    byte ptr [eax + 34h], dl
        je     SHORT loc_5_2A1B
        lea    esi, [eax + 78h]
        lea    edi, [esp + 5ah]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_2A1B
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    SHORT loc_5_2A1B
        mov    cl, byte ptr [esp + 13h]
        cmp    byte ptr [eax + 5bh], cl
        je     SHORT loc_5_2A33
loc_5_2A1B:
        add    eax, 0a0h
        xor    ecx, ecx
        inc    dword ptr [esp + 20h]
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, dword ptr [esp + 20h]
        ja     SHORT loc_5_29F1
loc_5_2A33:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, dword ptr [esp + 20h]
        jne    NEAR PTR loc_5_2AF0
        call   sub_5_1A70
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_5_2A64
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0f9h
        jmp    NEAR PTR loc_5_2CB5
loc_5_2A64:
        mov    eax, dword ptr [esp + 1ch]
        db  02Bh, 005h, 0A0h, 003h, 000h, 000h
        mov    ecx, 0a0h
        cdq
        idiv   ecx
        mov    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        db  0E8h, 0CCh, 0E2h, 0FAh, 0FFh
        lea    ecx, [esp + 5eh]
        mov    edx, dword ptr [esp + 20h]
        add    esp, 4
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 78h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 7ch], eax
        add    edx, 78h
        mov    eax, dword ptr [ecx + 8]
        mov    cl, byte ptr [esp + 13h]
        push   ecx
        mov    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [esp + 84h]
        push   ecx
        mov    ecx, dword ptr [esp + 24h]
        push   ecx
        call   sub_5_13D0
        mov    byte ptr [esp + 20h], al
        mov    ecx, dword ptr [esp + 28h]
        add    esp, 0ch
        push   ecx
        db  0E8h, 0C5h, 0E2h, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_5_2AF0
        mov    al, byte ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 80h]
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 1ah], ax
        jmp    NEAR PTR loc_5_2CB5
        db  08Bh, 0FFh
loc_5_2AF0:
        mov    eax, dword ptr [esp + 20h]
        sub    al, 7fh
        mov    byte ptr [esp + 16h], al
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_5_2B34
        sub    esi, esi
        db  0A1h, 0ACh, 003h, 000h, 000h
loc_5_2B08:
        cmp    dword ptr [eax], 0
        je     SHORT loc_5_2B16
        add    eax, 4
        inc    esi
        cmp    esi, 8
        jb     SHORT loc_5_2B08
loc_5_2B16:
        cmp    esi, 8
        jne    SHORT loc_5_2B22
        call   sub_5_1790
        jmp    SHORT loc_5_2B34
loc_5_2B22:
        mov    edx, esi
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_5_1D00
        lea    eax, [esi + 1]
        mov    byte ptr [esp + 16h], al
loc_5_2B34:
        xor    edx, edx
        mov    dl, byte ptr [esp + 16h]
        mov    ecx, dword ptr [esp + 84h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 1ch]
        shl    edx, 10h
        add    edx, eax
        mov    dword ptr [ecx + 1ch], edx
loc_5_2B4F:
        cmp    byte ptr [ebx + ebp], 0
        je     NEAR PTR loc_5_2CB5
        mov    ecx, dword ptr [esp + 1ch]
        cmp    word ptr [ecx + 50h], 0
        jbe    SHORT loc_5_2B76
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0ffh
        jmp    NEAR PTR loc_5_2CB5
loc_5_2B76:
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_5_7230
        cmp    byte ptr [ebx + ebp + 1], 0
        jne    SHORT loc_5_2BA7
        push   19h
        mov    ecx, dword ptr [esp + 84h]
        db  068h, 0B8h, 013h, 000h, 000h
        push   ecx
        mov    ecx, dword ptr [esp + 28h]
        push   ecx
        db  0E8h, 0E0h, 0D6h, 0FAh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_5_2CB5
loc_5_2BA7:
        lea    ebx, [ebx + ebp + 1]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ebx + ecx]
        mov    ecx, dword ptr [esp + 84h]
        mov    di, word ptr [ecx + 18h]
        mov    ecx, dword ptr [esp + 80h]
        test   byte ptr [ecx + 42h], 4
        jne    SHORT loc_5_2BD7
        mov    di, 1
loc_5_2BD7:
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        db  0E8h, 06Fh, 0E1h, 0FAh, 0FFh
        add    esp, 4
        push   esi
        push   ebx
        mov    edx, edi
        mov    ecx, dword ptr [esp + 24h]
        call   sub_5_2160
        mov    ecx, dword ptr [esp + 80h]
        and    ax, 0ffh
        mov    word ptr [ecx + 1ah], ax
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        db  0E8h, 086h, 0E1h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_2CB5
loc_5_2C12:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, edx
        jne    SHORT loc_5_2C31
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0ffh
        jmp    NEAR PTR loc_5_2CB5
loc_5_2C31:
        mov    ecx, dword ptr [esp + 1ch]
        cmp    word ptr [ecx + 50h], 0
        jbe    SHORT loc_5_2C4B
        mov    ecx, dword ptr [esp + 80h]
        mov    word ptr [ecx + 1ah], 0ffh
        jmp    SHORT loc_5_2CB5
loc_5_2C4B:
        cmp    al, 4
        jne    SHORT loc_5_2CA0
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_5_7230
        push   19h
        mov    ecx, dword ptr [esp + 84h]
        db  068h, 09Ch, 013h, 000h, 000h
        push   ecx
        mov    ecx, dword ptr [esp + 28h]
        push   ecx
        db  0E8h, 00Eh, 0D6h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 2ch]
        add    esp, 10h
        mov    eax, dword ptr [ecx + 88h]
        or     eax, eax
        je     SHORT loc_5_2C8D
        push   0
        push   eax
        db  0E8h, 047h, 0DDh, 0FAh, 0FFh
        add    esp, 8
loc_5_2C8D:
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 88h], 0
        jmp    SHORT loc_5_2CB5
        db  08Dh, 049h, 000h
loc_5_2CA0:
        mov    ecx, dword ptr [esp + 80h]
        push   ecx
        mov    ecx, dword ptr [esp + 20h]
        push   ecx
        call   sub_5_16D0
        add    esp, 8
loc_5_2CB5:
        db  068h, 0B8h, 010h, 000h, 000h
        db  0E8h, 090h, 0D5h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  090h
loc_5_2CD0:
        db  068h, 0B8h, 010h, 000h, 000h
        db  0E8h, 075h, 0D5h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_2CF0:
        sub    esp, 4
        mov    dword ptr [esp], 0
        sub    eax, eax
        mov    ecx, 0ffh
loc_5_2D02:
        mov    edx, dword ptr [esp]
        db  039h, 004h, 095h, 0D8h, 005h, 000h, 000h
        je     SHORT loc_5_2D19
        inc    dword ptr [esp]
        cmp    dword ptr [esp], ecx
        jl     SHORT loc_5_2D02
loc_5_2D19:
        mov    ecx, dword ptr [esp]
        mov    eax, dword ptr [esp + 8]
        db  089h, 004h, 08Dh, 0D8h, 005h, 000h, 000h
        push   10h
        db  0E8h, 0A9h, 0DDh, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 4]
        add    esp, 4
        db  089h, 004h, 08Dh, 0D4h, 009h, 000h, 000h
        lea    ecx, [eax - 24h]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    cl, byte ptr [esp]
        inc    cl
        mov    al, cl
        add    esp, 4
        ret
        db  090h
sub_5_2D60:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [edx + 0ch]
        lea    ebx, [edx - 10h]
        sub    eax, eax
        cmp    ebx, edi
        je     SHORT loc_5_2D83
        mov    esi, dword ptr [esp + 10h]
loc_5_2D73:
        or     eax, eax
        jne    SHORT loc_5_2D83
        mov    edi, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [edi + 20h]
        call   esi
        cmp    ebx, edi
        jne    SHORT loc_5_2D73
loc_5_2D83:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2D90:
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        sub    eax, eax
        mov    esi, dword ptr [edi + 1ch]
        cmp    edi, esi
        je     SHORT loc_5_2DB2
        mov    ebx, dword ptr [esp + 10h]
loc_5_2DA2:
        or     eax, eax
        jne    SHORT loc_5_2DB2
        mov    esi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 20h]
        call   ebx
        cmp    edi, esi
        jne    SHORT loc_5_2DA2
loc_5_2DB2:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_2DC0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, ecx
        mov    ecx, edx
        mov    dword ptr [esp + 14h], edx
        inc    word ptr [ecx + 26h]
        mov    eax, edx
        add    eax, 14h
        mov    dword ptr [esp + 10h], eax
        sub    esi, esi
        mov    ebp, dword ptr [eax]
        cmp    ebp, eax
        je     SHORT loc_5_2E27
        mov    ebx, dword ptr [esp + 1ch]
loc_5_2DE8:
        or     esi, esi
        jne    SHORT loc_5_2E27
        cmp    edi, 3
        je     SHORT loc_5_2E00
        cmp    edi, 4
        je     SHORT loc_5_2E06
        cmp    edi, 5
        je     SHORT loc_5_2E12
        jmp    SHORT loc_5_2E1E
        db  08Dh, 049h, 000h
loc_5_2E00:
        mov    ecx, ebp
        call   ebx
        jmp    SHORT loc_5_2E1C
loc_5_2E06:
        push   ebx
        mov    edx, ebp
        mov    ecx, edi
        call   sub_5_2D90
        jmp    SHORT loc_5_2E1C
loc_5_2E12:
        push   ebx
        mov    edx, ebp
        mov    ecx, edi
        call   sub_5_2D60
loc_5_2E1C:
        mov    esi, eax
loc_5_2E1E:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, dword ptr [esp + 10h]
        jne    SHORT loc_5_2DE8
loc_5_2E27:
        mov    ecx, dword ptr [esp + 14h]
        mov    ax, word ptr [ecx + 26h]
        dec    ax
        mov    word ptr [ecx + 26h], ax
        jne    SHORT loc_5_2E4A
        test   byte ptr [ecx + 24h], 1
        je     SHORT loc_5_2E4A
        mov    eax, dword ptr [esp + 10h]
        push   eax
        db  0E8h, 051h, 027h, 0FBh, 0FFh
        add    esp, 4
loc_5_2E4A:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2E60:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    esi, ecx
        db  08Bh, 02Dh, 0A8h, 005h, 000h, 000h
        sub    eax, eax
        cmp    ebp, eax
        je     SHORT loc_5_2EA9
        mov    ebx, dword ptr [esp + 14h]
loc_5_2E78:
        or     eax, eax
        jne    SHORT loc_5_2EA9
        cmp    dword ptr [ebp + 10h], edi
        jne    SHORT loc_5_2EA2
        cmp    esi, 2
        je     SHORT loc_5_2E92
        cmp    esi, 3
        jl     SHORT loc_5_2EA2
        cmp    esi, 5
        jle    SHORT loc_5_2E98
        jmp    SHORT loc_5_2EA2
loc_5_2E92:
        mov    ecx, ebp
        call   ebx
        jmp    SHORT loc_5_2EA2
loc_5_2E98:
        push   ebx
        mov    edx, ebp
        mov    ecx, esi
        call   sub_5_2DC0
loc_5_2EA2:
        mov    ebp, dword ptr [ebp + 4]
        or     ebp, ebp
        jne    SHORT loc_5_2E78
loc_5_2EA9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_2EB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    esi, ecx
        sub    ebx, ebx
        sub    eax, eax
        db  066h, 039h, 005h, 014h, 004h, 000h, 000h
        je     SHORT loc_5_2F0C
        sub    ebp, ebp
loc_5_2EC7:
        or     eax, eax
        jne    SHORT loc_5_2F0C
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        add    edx, ebp
        cmp    byte ptr [edx + 34h], 0
        je     SHORT loc_5_2EF8
        cmp    esi, 1
        je     SHORT loc_5_2EEA
        cmp    esi, 2
        jl     SHORT loc_5_2EF8
        cmp    esi, 5
        jle    SHORT loc_5_2EF0
        jmp    SHORT loc_5_2EF8
loc_5_2EEA:
        mov    ecx, edx
        call   edi
        jmp    SHORT loc_5_2EF8
loc_5_2EF0:
        push   edi
        mov    ecx, esi
        call   sub_5_2E60
loc_5_2EF8:
        add    ebp, 0a0h
        inc    ebx
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, ebx
        ja     SHORT loc_5_2EC7
loc_5_2F0C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2F20:
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        db  0A1h, 0B8h, 014h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_2F41
loc_5_2F30:
        cmp    word ptr [eax + 40h], di
        je     SHORT loc_5_2F3D
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_5_2F30
loc_5_2F3D:
        or     eax, eax
        jne    SHORT loc_5_2F75
loc_5_2F41:
        push   1
        push   44h
        db  0E8h, 074h, 0DAh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_2FA5
        db  08Bh, 00Dh, 0B8h, 014h, 000h, 000h
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 0B8h, 014h, 000h, 000h
        mov    dword ptr [eax], 736572h
        lea    ecx, [eax + 14h]
        db  0C7h, 040h, 00Ch, 064h, 014h, 000h, 000h
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 14h], ecx
loc_5_2F75:
        lea    ecx, [esp + 14h]
        lea    ebx, [eax + 34h]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebx], edx
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [ebx + 4], cx
        mov    cx, word ptr [esp + 1ch]
        mov    word ptr [eax + 3ah], cx
        mov    dword ptr [eax + 3ch], esi
        mov    dword ptr [eax + 40h], edi
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 28h], ecx
loc_5_2FA5:
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
sub_5_2FB0:
        push   esi
        push   edi
        mov    edi, edx
        cmp    dword ptr [edi], 0
        je     SHORT loc_5_2FC7
loc_5_2FB9:
        mov    eax, dword ptr [edi]
        cmp    eax, ecx
        je     SHORT loc_5_2FC7
        lea    edi, [eax + 4]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_5_2FB9
loc_5_2FC7:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_5_2FDD
        mov    esi, dword ptr [eax + 4]
        push   0
        push   eax
        db  0E8h, 0F9h, 0D9h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi], esi
loc_5_2FDD:
        pop    edi
        pop    esi
        ret
loc_5_2FE0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    edi, dword ptr [ebx + 0ch]
        or     edi, edi
        je     SHORT loc_5_3010
        xor    edx, edx
        db  066h, 08Bh, 015h, 064h, 014h, 000h, 000h
        add    edx, 2
        db  0BEh, 064h, 014h, 000h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_5_3010:
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 20h], eax
        mov    word ptr [ebx + 18h], 0
        mov    byte ptr [ebx + 4], 2
        sub    eax, eax
        mov    word ptr [ebx + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_3030:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0BAh, 0B8h, 014h, 000h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        call   sub_5_2FB0
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_3050:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0E4h, 003h, 000h, 000h
        mov    ebx, dword ptr [eax + edx*8]
        or     ebx, ebx
        je     NEAR PTR loc_5_31AE
        mov    esi, dword ptr [ebx + 10h]
        shl    edx, 6
        db  0A1h, 0E0h, 003h, 000h, 000h
        add    eax, edx
        sub    edx, edx
        cmp    byte ptr [eax + 37h], dl
        je     SHORT loc_5_3094
        mov    eax, dword ptr [eax + 39h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 0ch], eax
        mov    edx, dword ptr [esp + 0ch]
loc_5_3094:
        cmp    dword ptr [ebx + 28h], edx
        je     NEAR PTR loc_5_31AE
        mov    dword ptr [ebx + 28h], edx
        or     edx, edx
        jne    SHORT loc_5_3100
        push   1
        xor    eax, eax
        db  066h, 0A1h, 05Ch, 014h, 000h, 000h
        add    eax, 2
        push   eax
        db  0E8h, 007h, 0D9h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_5_30D1
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 006h, 0D9h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 0ch], esi
loc_5_30D1:
        xor    eax, eax
        db  066h, 0A1h, 05Ch, 014h, 000h, 000h
        add    eax, 2
        db  0BEh, 05Ch, 014h, 000h, 000h
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_3100:
        mov    byte ptr [esp + 43h], 0
        lea    eax, [esp + 14h]
        push   2fh
        push   eax
        push   4
        mov    eax, dword ptr [ebx + 28h]
        push   eax
        push   36h
        push   17h
        db  068h, 06Ch, 015h, 000h, 000h
        push   ecx
        push   esi
        db  0E8h, 05Dh, 0D1h, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        je     SHORT loc_5_3137
        mov    dword ptr [ebx + 28h], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
loc_5_3137:
        lea    edi, [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    edi, ecx
        and    edi, 0ffffh
        push   1
        lea    eax, [edi*2 + 8]
        push   eax
        db  0E8h, 060h, 0D8h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_3175
        mov    dword ptr [ebx + 28h], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
loc_5_3175:
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 051h, 0D8h, 0FAh, 0FFh
        add    esp, 8
        push   1
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        lea    eax, [esi + 6]
        push   eax
        db  0E8h, 02Ch, 0D9h, 0FAh, 0FFh
        add    esp, 10h
        lea    ecx, [eax + 6]
        mov    word ptr [esi], cx
        mov    word ptr [esi + 2], 4
        add    ax, 2
        mov    word ptr [esi + 4], ax
        mov    dword ptr [ebx + 0ch], esi
loc_5_31AE:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_31C0:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 38h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    al, byte ptr [eax + 1fh]
        mov    byte ptr [esp + 13h], al
        db  0E8h, 092h, 0D7h, 0FAh, 0FFh
        mov    edi, eax
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        add    ecx, edi
        movzx  esi, byte ptr [esp + 13h]
        mov    edx, esi
        shl    edx, 6
        db  0A1h, 0E0h, 003h, 000h, 000h
        add    eax, edx
        mov    dword ptr [esp + 28h], eax
        lea    eax, [esi + esi*2]
        lea    eax, [esi + eax*4]
        lea    ecx, [ecx + eax + 4ch]
        mov    dword ptr [esp + 20h], ecx
        push   1
        push   30h
        db  0E8h, 09Fh, 0D7h, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        mov    edx, 2
        mov    ecx, ebx
        call   sub_5_A4A0
        mov    dword ptr [esp + 24h], eax
        or     ebp, ebp
        je     NEAR PTR loc_5_358F
        or     eax, eax
        je     NEAR PTR loc_5_3580
        mov    dword ptr [ebp], 454c4946h
        or     dword ptr [eax + 54h], 4
        mov    word ptr [eax + 6ch], 0
        mov    word ptr [eax + 6eh], 0
        mov    word ptr [eax + 70h], 0
        mov    dword ptr [eax + 5ch], 0
        mov    byte ptr [ebp + 0eh], 2
        mov    byte ptr [ebp + 0fh], 1
        mov    dword ptr [ebp + 14h], edi
        mov    eax, dword ptr [ebx + 8]
        push   eax
        db  0E8h, 01Fh, 0D6h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp + 10h], al
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 18h], eax
        cmp    byte ptr [ecx + 2], 0
        je     SHORT loc_5_32D0
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_3050
        mov    eax, dword ptr [esp + 20h]
        add    eax, 5
        lea    edx, [ebp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    ecx, dword ptr [esp + 20h]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [ecx + 3]
        mov    word ptr [ebp + 0ch], ax
        mov    al, byte ptr [ecx + 0ch]
        mov    byte ptr [ebp + 10h], al
        jmp    NEAR PTR loc_5_3506
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_32D0:
        mov    ecx, dword ptr [esp + 20h]
        cmp    byte ptr [ecx + 0bh], 0
        je     SHORT loc_5_3301
        mov    eax, ecx
        add    eax, 5
        lea    edx, [ebp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ecx, dword ptr [esp + 20h]
        mov    byte ptr [ebp + 0fh], 3
        mov    al, byte ptr [ecx + 0ch]
        mov    byte ptr [ebp + 10h], al
        jmp    NEAR PTR loc_5_3506
loc_5_3301:
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_3050
        db  0BEh, 074h, 015h, 000h, 000h
        db  0BFh, 008h, 01Fh, 000h, 000h
        mov    ecx, 5
        rep movsd
        movsb
        mov    al, byte ptr [esp + 13h]
        add    al, 31h
        db  0A2h, 00Bh, 01Fh, 000h, 000h
        db  0C6h, 005h, 075h, 01Fh, 000h, 000h, 000h
        db  0C6h, 005h, 083h, 01Fh, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 1eh], ax
        mov    ax, word ptr [esp + 1eh]
        xchg   al, ah
        mov    word ptr [esp + 1ch], ax
        mov    ax, word ptr [esp + 1ch]
        db  066h, 0A3h, 03Ah, 01Fh, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 1ah], ax
        mov    ax, word ptr [esp + 1ah]
        xchg   al, ah
        mov    word ptr [esp + 18h], ax
        mov    ax, word ptr [esp + 18h]
        db  066h, 0A3h, 03Ch, 01Fh, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        db  066h, 0A3h, 03Eh, 01Fh, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        mov    ax, word ptr [eax + 19h]
        db  066h, 0A3h, 040h, 01Fh, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        mov    ax, word ptr [eax + 1bh]
        db  066h, 0A3h, 042h, 01Fh, 000h, 000h
        db  0BFh, 0F7h, 003h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  0BFh, 05Ah, 01Fh, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 28h]
        add    edi, 1dh
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  0BFh, 044h, 01Fh, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 28h]
        add    edi, 7
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  0BFh, 067h, 01Fh, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 20h]
        add    edi, 5
        mov    esi, dword ptr [esp + 20h]
        add    esi, 3
        push   6
        push   edi
        push   12h
        push   esi
        push   16h
        push   0cbh
        db  068h, 008h, 01Fh, 000h, 000h
        push   1ah
        push   10h
        mov    eax, dword ptr [esp + 4ch]
        push   0
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        push   eax
        push   8
        push   -1
        push   -1
        push   -1
        mov    eax, dword ptr [esp + 68h]
        push   6
        mov    eax, dword ptr [eax + 28h]
        push   eax
        push   68h
        push   17h
        db  068h, 08Ch, 015h, 000h, 000h
        mov    eax, dword ptr [esp + 80h]
        push   ebx
        push   eax
        db  0E8h, 0E1h, 0CDh, 0FAh, 0FFh
        mov    byte ptr [esp + 6ah], al
        add    esp, 58h
        or     al, al
        je     SHORT loc_5_34D0
        push   0
        push   ebp
        db  0E8h, 020h, 0D5h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 1ah]
        add    esp, 8
        db  0E8h, 093h, 006h, 0FBh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_34D0:
        mov    ecx, dword ptr [esp + 20h]
        mov    byte ptr [ecx + 0bh], 0ffh
        mov    byte ptr [ecx + 2], 0ffh
        mov    eax, dword ptr [ebx + 8]
        push   eax
        db  0E8h, 0B6h, 0D3h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 4
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp + 4], eax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [ebp + 8], ax
        lea    ecx, [ebp + 4]
        mov    ax, word ptr [esi]
        mov    word ptr [ebp + 0ch], ax
loc_5_3506:
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 24h]
        mov    byte ptr [ecx], 0ffh
        mov    byte ptr [ecx + 1], 0ffh
        mov    dword ptr [ebx + 20h], ebp
        mov    word ptr [ebx + 18h], 1
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 6ch]
        mov    word ptr [ebx + 2ah], ax
        mov    eax, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 6eh]
        mov    word ptr [ebx + 28h], ax
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [esp + 24h]
        db  0C7h, 000h, 060h, 060h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        db  0C7h, 040h, 004h, 090h, 063h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        db  0C7h, 040h, 008h, 0BCh, 014h, 000h, 000h
        call   sub_5_A8D0
        mov    eax, dword ptr [esp + 30h]
        pop    ebp
        inc    word ptr [eax + 50h]
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_3580:
        or     ebp, ebp
        je     SHORT loc_5_358F
        push   0
        push   ebp
        db  0E8h, 045h, 0D4h, 0FAh, 0FFh
        add    esp, 8
loc_5_358F:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_5_359F
        mov    ecx, dword ptr [esp + 24h]
        call   sub_5_A7F0
loc_5_359F:
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_35C0:
        sub    esp, 70h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 84h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 30h], eax
        mov    edx, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 38h], edx
        mov    eax, dword ptr [esp + 34h]
        mov    word ptr [esp + 18h], 0
        mov    al, byte ptr [eax + 1ch]
        mov    byte ptr [esp + 17h], al
        mov    byte ptr [esp + 14h], 0
        mov    byte ptr [esp + 16h], 0
        mov    eax, dword ptr [ebx + 8]
        push   eax
        db  0E8h, 093h, 0D2h, 0FAh, 0FFh
        mov    byte ptr [esp + 19h], al
        add    esp, 4
        mov    al, byte ptr [ebx + 4]
        and    al, 7
        sub    al, 3
        cmp    al, 1
        mov    esi, dword ptr [ebx + 14h]
        sbb    eax, eax
        and    eax, 48h
        add    ax, 6
        cmp    byte ptr [ebx + 4], 0ffh
        mov    word ptr [esp + 1ah], ax
        mov    al, byte ptr [esp + 15h]
        mov    byte ptr [ebx + 40h], al
        mov    byte ptr [ebx + 41h], 0ffh
        jne    SHORT loc_5_363E
        mov    byte ptr [ebx + 4], 2
loc_5_363E:
        mov    cl, byte ptr [ebx + 4]
        mov    al, cl
        and    al, 70h
        shr    al, 4
        and    eax, 0ffh
        and    ecx, 3
        db  08Ah, 080h, 008h, 015h, 000h, 000h
        db  002h, 081h, 004h, 015h, 000h, 000h
        mov    cx, word ptr [ebx + 18h]
        mov    byte ptr [esp + 12h], al
        mov    eax, ecx
        and    eax, 0ffffh
        mov    dword ptr [esp + 58h], eax
        and    cx, 0ffh
        mov    word ptr [ebx + 18h], cx
        sub    ebp, ebp
        mov    ecx, ebx
        db  0E8h, 06Fh, 005h, 0FBh, 0FFh
        mov    byte ptr [esp + 13h], al
        cmp    al, 80h
        jne    SHORT loc_5_36A0
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_36A0:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_5_36B5
        db  0F6h, 005h, 06Eh, 004h, 000h, 000h, 001h
        je     SHORT loc_5_36B5
        mov    byte ptr [esp + 13h], 0ffh
loc_5_36B5:
        mov    eax, dword ptr [esp + 34h]
        cmp    byte ptr [eax + 1ch], 2
        jne    NEAR PTR loc_5_3850
        cmp    byte ptr [eax + 1fh], 0ffh
        je     SHORT loc_5_36E1
        push   ebx
        call   sub_5_31C0
        add    esp, 4
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_36E1:
        test   byte ptr [ebx + 18h], 10h
        jne    SHORT loc_5_3700
        mov    word ptr [ebx + 1ah], 2
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_3700:
        mov    byte ptr [esp + 14h], 1
        db  0BEh, 0A0h, 015h, 000h, 000h
        db  0BFh, 0D4h, 01Fh, 000h, 000h
        mov    ecx, 5
        rep movsd
        db  0C6h, 005h, 041h, 020h, 000h, 000h, 000h
        db  0C6h, 005h, 04Fh, 020h, 000h, 000h, 000h
        mov    word ptr [esp + 2eh], 8
        mov    ax, word ptr [esp + 2eh]
        xchg   al, ah
        mov    word ptr [esp + 2ch], ax
        mov    ax, word ptr [esp + 2ch]
        db  066h, 0A3h, 006h, 020h, 000h, 000h
        mov    word ptr [esp + 2ah], 1
        mov    ax, word ptr [esp + 2ah]
        xchg   al, ah
        mov    word ptr [esp + 28h], ax
        mov    ax, word ptr [esp + 28h]
        db  066h, 0A3h, 008h, 020h, 000h, 000h
        mov    word ptr [esp + 26h], 80h
        mov    ax, word ptr [esp + 26h]
        xchg   al, ah
        mov    word ptr [esp + 24h], ax
        mov    ax, word ptr [esp + 24h]
        db  066h, 0A3h, 00Ah, 020h, 000h, 000h
        mov    word ptr [esp + 22h], 42h
        mov    ax, word ptr [esp + 22h]
        xchg   al, ah
        mov    word ptr [esp + 20h], ax
        mov    ax, word ptr [esp + 20h]
        db  066h, 0A3h, 00Ch, 020h, 000h, 000h
        mov    word ptr [esp + 1eh], 84h
        mov    ax, word ptr [esp + 1eh]
        xchg   al, ah
        mov    word ptr [esp + 1ch], ax
        mov    ax, word ptr [esp + 1ch]
        db  066h, 0A3h, 00Eh, 020h, 000h, 000h
        db  0A0h, 0B4h, 015h, 000h, 000h
        db  0A2h, 026h, 020h, 000h, 000h
        db  0B8h, 0B8h, 015h, 000h, 000h
        db  0B9h, 010h, 020h, 000h, 000h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], al
        db  0A0h, 0C0h, 015h, 000h, 000h
        db  0A2h, 033h, 020h, 000h, 000h
        push   6
        lea    eax, [esp + 70h]
        push   eax
        push   12h
        lea    eax, [esp + 24h]
        push   eax
        push   16h
        push   0cbh
        db  068h, 0D4h, 01Fh, 000h, 000h
        push   1ah
        push   10h
        push   0
        push   0
        push   8
        push   -1
        push   -1
        push   -1
        mov    eax, dword ptr [esp + 70h]
        push   6
        mov    eax, dword ptr [eax + 28h]
        push   eax
        push   68h
        push   17h
        db  068h, 0C4h, 015h, 000h, 000h
        mov    eax, dword ptr [esp + 80h]
        push   ebx
        push   eax
        db  0E8h, 052h, 0CAh, 0FAh, 0FFh
        add    esp, 58h
        or     al, al
        je     NEAR PTR loc_5_3E25
        mov    ecx, eax
        db  0E8h, 011h, 003h, 0FBh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Bh, 0FFh
loc_5_3850:
        mov    eax, dword ptr [esp + 34h]
        test   byte ptr [eax + 24h], 4
        je     SHORT loc_5_38A2
        mov    edx, dword ptr [eax + 28h]
        mov    dword ptr [eax + 28h], 0
        lea    eax, [esp + 6ch]
        mov    byte ptr [esp + 16h], 1
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        add    edx, 2
        mov    dword ptr [eax + 0ch], 0
        mov    byte ptr [eax + 10h], 0
        mov    ecx, eax
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ecx + 4], ax
        jmp    NEAR PTR loc_5_3EAC
loc_5_38A2:
        db  080h, 03Dh, 068h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_38EA
        or     esi, esi
        je     SHORT loc_5_38EA
        test   byte ptr [esi], 1
        je     SHORT loc_5_38E0
        mov    eax, dword ptr [esi + 4]
        push   1
        movsx  ecx, word ptr [eax]
        sub    ecx, 2
        push   ecx
        push   ecx
        add    eax, 2
        push   eax
        lea    eax, [esp + 6ch]
        push   eax
        db  0E8h, 0EBh, 0D1h, 0FAh, 0FFh
        add    esp, 14h
        mov    byte ptr [esp + eax + 5ch], 0
        jmp    SHORT loc_5_38EA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_38E0:
        mov    byte ptr [esp + 5ch], 2dh
        mov    byte ptr [esp + 5dh], 0
loc_5_38EA:
        mov    byte ptr [esp + 14h], 0
        test   byte ptr [ebx], 58h
        je     SHORT loc_5_3907
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_3907:
        sub    edx, edx
        mov    ecx, ebx
        call   sub_5_A4A0
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_3960
        inc    dword ptr [ebp + 2ch]
        sub    esi, esi
loc_5_391B:
        mov    ecx, dword ptr [ebp + 48h]
        cmp    ecx, esi
        jne    SHORT loc_5_392F
        mov    eax, dword ptr [esp + 30h]
        cmp    dword ptr [eax + 2ch], esi
        je     SHORT loc_5_3960
        cmp    ecx, esi
        je     SHORT loc_5_393E
loc_5_392F:
        inc    dword ptr [ebp + 58h]
        push   ebp
        db  0E8h, 0ACh, 0D1h, 0FAh, 0FFh
        add    esp, 4
        dec    dword ptr [ebp + 58h]
loc_5_393E:
        mov    eax, dword ptr [esp + 30h]
        cmp    dword ptr [eax + 2ch], esi
        je     SHORT loc_5_391B
        inc    byte ptr [eax + 37h]
        push   eax
        db  0E8h, 094h, 0D1h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 34h]
        add    esp, 4
        dec    byte ptr [eax + 37h]
        jmp    SHORT loc_5_391B
        db  08Dh, 064h, 024h, 000h
loc_5_3960:
        mov    eax, dword ptr [esp + 38h]
        add    eax, 4
        mov    dword ptr [esp + 3ch], eax
        mov    edx, dword ptr [esp + 38h]
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        mov    edi, edx
        add    edi, eax
        test   byte ptr [ebx + 18h], 10h
        je     SHORT loc_5_39B7
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_5_39B7
        cmp    byte ptr [esp + 13h], 4
        je     SHORT loc_5_39B7
        mov    eax, dword ptr [esp + 34h]
        cmp    byte ptr [eax + 21h], 4
        jne    SHORT loc_5_39B7
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    eax, edi
        push   eax
        mov    eax, dword ptr [esp + 40h]
        push   eax
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 3ch]
        db  0E8h, 052h, 002h, 0FBh, 0FFh
        or     eax, eax
        jne    SHORT loc_5_39B7
        mov    byte ptr [esp + 13h], 4
loc_5_39B7:
        cmp    byte ptr [esp + 13h], 0ffh
        je     NEAR PTR loc_5_3B77
        mov    si, word ptr [ebx + 18h]
        db  0F6h, 005h, 065h, 004h, 000h, 000h, 001h
        je     SHORT loc_5_39E8
        cmp    byte ptr [esp + 13h], 4
        jne    SHORT loc_5_39E8
        mov    eax, dword ptr [ebx + 28h]
        lea    ecx, [eax - 2]
        mov    dword ptr [esp + 40h], ecx
        mov    ax, word ptr [edi]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_5_39EC
loc_5_39E8:
        mov    dword ptr [esp + 40h], edi
loc_5_39EC:
        mov    ecx, dword ptr [esp + 40h]
        xor    eax, eax
        mov    ax, word ptr [ecx]
        add    eax, ecx
        mov    dword ptr [esp + 54h], eax
        cmp    si, 10h
        jb     SHORT loc_5_3A05
        sub    si, 0dh
loc_5_3A05:
        xor    eax, eax
        mov    ax, word ptr [esp + 1ah]
        mov    dword ptr [esp + 50h], eax
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        mov    dword ptr [esp + 4ch], eax
loc_5_3A1A:
        lea    eax, [esp + 7bh]
        push   eax
        lea    eax, [esp + 7bh]
        push   4
        push   eax
        lea    eax, [esp + 85h]
        push   eax
        lea    eax, [esp + 83h]
        push   0eh
        push   eax
        lea    eax, [esp + 8ah]
        push   4
        push   eax
        lea    eax, [esp + 31h]
        push   4
        push   eax
        lea    eax, [esp + 96h]
        push   4
        push   eax
        mov    eax, dword ptr [esp + 84h]
        push   14h
        mov    ecx, dword ptr [esp + 74h]
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [esp + 7ch]
        push   edi
        push   eax
        mov    eax, dword ptr [esp + 7ch]
        push   4
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 62h]
        push   eax
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 0ach]
        push   8ch
        push   eax
        mov    eax, esi
        and    eax, 0ffffh
        db  08Ah, 080h, 010h, 015h, 000h, 000h
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [esp + 0b4h]
        push   eax
        push   1
        db  068h, 0D8h, 015h, 000h, 000h
        mov    eax, dword ptr [esp + 0a4h]
        push   ebx
        push   eax
        db  0E8h, 0C5h, 0C7h, 0FAh, 0FFh
        add    esp, 7ch
        or     al, al
        je     NEAR PTR loc_5_3B56
        cmp    word ptr [ebx + 18h], 11h
        jne    SHORT loc_5_3ADB
        cmp    si, 4
        jne    SHORT loc_5_3ADB
        mov    si, 3
        jmp    NEAR PTR loc_5_3A1A
loc_5_3ADB:
        mov    cl, byte ptr [ebx + 4]
        mov    dl, cl
        and    dl, 7
        cmp    dl, 2
        jne    SHORT loc_5_3B16
        cmp    al, 90h
        jne    SHORT loc_5_3B16
        and    cl, 0f8h
        mov    al, cl
        and    al, 70h
        shr    al, 4
        and    eax, 0ffh
        mov    byte ptr [ebx + 4], cl
        and    ecx, 3
        db  08Ah, 080h, 008h, 015h, 000h, 000h
        db  002h, 081h, 004h, 015h, 000h, 000h
        mov    byte ptr [esp + 12h], al
        jmp    NEAR PTR loc_5_3A1A
loc_5_3B16:
        or     al, al
        je     SHORT loc_5_3B56
        or     ebp, ebp
        je     SHORT loc_5_3B21
        dec    dword ptr [ebp + 2ch]
loc_5_3B21:
        cmp    al, 0ffh
        jne    SHORT loc_5_3B40
        cmp    word ptr [ebx + 18h], 10h
        jne    SHORT loc_5_3B40
        mov    word ptr [ebx + 1ah], 50h
        mov    eax, 50h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  090h
loc_5_3B40:
        mov    ecx, eax
        db  0E8h, 009h, 000h, 0FBh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_3B56:
        mov    ax, word ptr [esp + 6eh]
        inc    ax
        cmp    byte ptr [esp + 11h], 3
        mov    word ptr [esp + 6ch], ax
        jbe    NEAR PTR loc_5_3E25
        mov    byte ptr [esp + 11h], 3
        jmp    NEAR PTR loc_5_3E25
loc_5_3B77:
        mov    ax, word ptr [ebx + 18h]
        cmp    ax, 10h
        jne    NEAR PTR loc_5_3C0A
        xor    ecx, ecx
        lea    eax, [esp + 79h]
        push   eax
        mov    cx, word ptr [edi]
        lea    eax, [esp + 7bh]
        push   eax
        lea    eax, [esp + 83h]
        push   eax
        lea    eax, [esp + 7fh]
        push   eax
        lea    eax, [esp + 82h]
        push   eax
        push   0eh
        lea    eax, [esp + 84h]
        push   6
        push   eax
        lea    eax, [edi + ecx]
        push   eax
        mov    eax, dword ptr [esp + 60h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 60h]
        push   eax
        push   4dh
        db  068h, 0F4h, 015h, 000h, 000h
        mov    eax, dword ptr [esp + 68h]
        push   ebx
        push   eax
        db  0E8h, 0A7h, 0C6h, 0FAh, 0FFh
        add    esp, 40h
        or     al, al
        je     SHORT loc_5_3C00
        or     ebp, ebp
        je     SHORT loc_5_3BE6
        dec    dword ptr [ebp + 2ch]
loc_5_3BE6:
        mov    ecx, eax
        db  0E8h, 063h, 0FFh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_3C00:
        mov    byte ptr [esp + 11h], 2
        jmp    NEAR PTR loc_5_3E25
loc_5_3C0A:
        xor    ecx, ecx
        mov    cx, word ptr [edi]
        add    edi, ecx
        cmp    ax, 12h
        jne    SHORT loc_5_3C26
        mov    byte ptr [esp + 10h], 0ffh
        mov    byte ptr [esp + 11h], 3
        jmp    NEAR PTR loc_5_3D66
loc_5_3C26:
        xor    eax, eax
        mov    al, byte ptr [esp + 12h]
        mov    dword ptr [esp + 44h], eax
        lea    eax, [esp + 79h]
        push   eax
        lea    eax, [esp + 7bh]
        push   eax
        lea    eax, [esp + 83h]
        push   eax
        lea    eax, [esp + 7fh]
        push   eax
        lea    eax, [esp + 82h]
        push   eax
        push   0eh
        lea    eax, [esp + 84h]
        push   6
        push   eax
        mov    eax, dword ptr [esp + 5ch]
        push   edi
        push   eax
        mov    eax, dword ptr [esp + 6ch]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esp + 46h]
        push   eax
        mov    eax, dword ptr [esp + 64h]
        push   eax
        push   4ch
        db  068h, 004h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 6ch]
        push   ebx
        push   eax
        db  0E8h, 0FAh, 0C5h, 0FAh, 0FFh
        mov    byte ptr [esp + 54h], al
        add    esp, 44h
        cmp    al, 0ffh
        jne    NEAR PTR loc_5_3D61
        db  0A0h, 06Eh, 004h, 000h, 000h
        and    al, 3
        cmp    al, 1
        je     SHORT loc_5_3CB2
        cmp    al, 3
        jne    NEAR PTR loc_5_3D61
        cmp    byte ptr [esp + 12h], 1
        jne    NEAR PTR loc_5_3D61
loc_5_3CB2:
        db  0E8h, 0C4h, 0CCh, 0FAh, 0FFh
        db  003h, 005h, 004h, 004h, 000h, 000h
        mov    dword ptr [esp + 48h], eax
        sub    esi, esi
loc_5_3CC3:
        mov    eax, dword ptr [esp + 48h]
        mov    al, byte ptr [esi + eax + 73h]
        cmp    al, 0ffh
        je     NEAR PTR loc_5_3D57
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0DCh, 003h, 000h, 000h
        mov    edx, dword ptr [eax + ecx*4]
        or     edx, edx
        je     SHORT loc_5_3D57
        mov    eax, dword ptr [esp + 30h]
        cmp    dword ptr [edx + 10h], eax
        jne    SHORT loc_5_3D57
        lea    eax, [esp + 79h]
        push   eax
        lea    eax, [esp + 7bh]
        push   eax
        lea    eax, [esp + 83h]
        push   eax
        lea    eax, [esp + 7fh]
        push   eax
        lea    eax, [esp + 82h]
        push   eax
        push   0eh
        lea    eax, [esp + 84h]
        push   6
        push   eax
        mov    edx, dword ptr [esp + 58h]
        push   edi
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 6ch]
        push   eax
        push   6
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        and    eax, 0ffh
        push   eax
        push   4ch
        db  068h, 018h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 6ch]
        push   ebx
        push   eax
        db  0E8h, 033h, 0C5h, 0FAh, 0FFh
        mov    byte ptr [esp + 54h], al
        add    esp, 44h
        or     al, al
        je     SHORT loc_5_3D61
loc_5_3D57:
        inc    esi
        cmp    esi, 10h
        jb     NEAR PTR loc_5_3CC3
loc_5_3D61:
        mov    byte ptr [esp + 11h], 2
loc_5_3D66:
        test   byte ptr [ebx + 18h], 10h
        je     NEAR PTR loc_5_3DF8
        cmp    byte ptr [esp + 10h], 0ffh
        je     SHORT loc_5_3D80
        mov    byte ptr [esp + 11h], 1
        jmp    SHORT loc_5_3DD2
        db  08Bh, 0FFh
loc_5_3D80:
        lea    eax, [esp + 79h]
        push   eax
        lea    eax, [esp + 7bh]
        push   eax
        lea    eax, [esp + 83h]
        push   eax
        lea    eax, [esp + 7fh]
        push   eax
        lea    eax, [esp + 82h]
        push   eax
        push   0eh
        lea    eax, [esp + 84h]
        push   6
        push   eax
        mov    eax, dword ptr [esp + 5ch]
        push   edi
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 60h]
        push   eax
        push   43h
        db  068h, 02Ch, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 68h]
        push   ebx
        push   eax
        db  0E8h, 0B4h, 0C4h, 0FAh, 0FFh
        mov    byte ptr [esp + 50h], al
        add    esp, 40h
loc_5_3DD2:
        cmp    byte ptr [esp + 10h], 0
        je     SHORT loc_5_3E25
        or     ebp, ebp
        je     SHORT loc_5_3DE0
        dec    dword ptr [ebp + 2ch]
loc_5_3DE0:
        mov    ecx, dword ptr [esp + 10h]
        db  0E8h, 067h, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_3DF8:
        cmp    byte ptr [esp + 10h], 0
        je     SHORT loc_5_3E20
        or     ebp, ebp
        je     SHORT loc_5_3E06
        dec    dword ptr [ebp + 2ch]
loc_5_3E06:
        mov    ecx, dword ptr [esp + 10h]
        db  0E8h, 041h, 0FDh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Bh, 0FFh
loc_5_3E20:
        mov    byte ptr [esp + 11h], 1
loc_5_3E25:
        cmp    byte ptr [esp + 11h], 1
        jbe    SHORT loc_5_3E35
        cmp    dword ptr [ebx + 24h], 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_5_3E3C
loc_5_3E35:
        mov    ax, word ptr [ebx + 18h]
        and    eax, 2
loc_5_3E3C:
        or     eax, eax
        je     SHORT loc_5_3EAC
        push   0
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        lea    eax, [esp + 74h]
        push   6
        push   eax
        push   49h
        db  068h, 03Ch, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ebx
        push   eax
        db  0E8h, 020h, 0C4h, 0FAh, 0FFh
        mov    byte ptr [esp + 30h], al
        add    esp, 20h
        or     al, al
        je     SHORT loc_5_3EA5
        lea    eax, [esp + 6ch]
        push   6
        push   eax
        push   42h
        db  068h, 044h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 40h]
        push   ebx
        push   eax
        db  0E8h, 0FCh, 0C3h, 0FAh, 0FFh
        add    esp, 18h
        or     ebp, ebp
        je     SHORT loc_5_3E8D
        dec    dword ptr [ebp + 2ch]
loc_5_3E8D:
        mov    ecx, dword ptr [esp + 10h]
        db  0E8h, 0BAh, 0FCh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_3EA5:
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esp + 73h], eax
loc_5_3EAC:
        push   1
        push   30h
        db  0E8h, 009h, 0CBh, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     ebp, ebp
        jne    SHORT loc_5_3ED3
        mov    edx, 1
        mov    ecx, ebx
        call   sub_5_A4A0
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_3ED3
        inc    dword ptr [ebp + 2ch]
loc_5_3ED3:
        or     esi, esi
        je     NEAR PTR loc_5_4081
        or     ebp, ebp
        je     NEAR PTR loc_5_4081
        test   byte ptr [esp + 72h], 1
        je     SHORT loc_5_3F06
        cmp    byte ptr [esp + 11h], 2
        jae    SHORT loc_5_3F06
        mov    al, byte ptr [ebx + 4]
        and    al, 7
        cmp    al, 1
        je     NEAR PTR loc_5_4081
        cmp    al, 2
        je     NEAR PTR loc_5_4081
loc_5_3F06:
        mov    dword ptr [esi], 454c4946h
        lea    ecx, [esp + 6ch]
        lea    edx, [esi + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [ebp - 10h]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [eax + 20h], esi
        mov    dword ptr [ebp + 0ch], esi
        test   byte ptr [esp + 59h], 1
        jne    SHORT loc_5_3F3F
        cmp    byte ptr [esp + 16h], 0
        je     SHORT loc_5_3F46
loc_5_3F3F:
        or     dword ptr [ebp + 54h], 1000h
loc_5_3F46:
        dec    dword ptr [ebp + 2ch]
        mov    dword ptr [esi + 18h], ebp
        mov    ecx, dword ptr [ebp + 5ch]
        cmp    ecx, -1
        je     SHORT loc_5_3F92
        mov    ax, word ptr [esp + 77h]
        cmp    word ptr [ebp + 6ch], ax
        jne    SHORT loc_5_3F7F
        mov    ax, word ptr [esp + 79h]
        cmp    word ptr [ebp + 6eh], ax
        jne    SHORT loc_5_3F7F
        cmp    dword ptr [esp + 73h], ecx
        jne    SHORT loc_5_3F7F
        mov    al, byte ptr [esp + 72h]
        and    eax, 0ffff00ffh
        cmp    word ptr [ebp + 68h], ax
        je     SHORT loc_5_3FC1
loc_5_3F7F:
        cmp    ecx, -1
        je     SHORT loc_5_3F92
        mov    ecx, ebp
        call   sub_5_A7B0
        mov    ecx, ebp
        call   sub_5_A710
loc_5_3F92:
        mov    ax, word ptr [esp + 77h]
        mov    word ptr [ebp + 6ch], ax
        mov    ax, word ptr [esp + 79h]
        mov    word ptr [ebp + 6eh], ax
        mov    ax, word ptr [esp + 7bh]
        mov    word ptr [ebp + 70h], ax
        mov    eax, dword ptr [esp + 73h]
        mov    dword ptr [ebp + 5ch], eax
        mov    al, byte ptr [esp + 72h]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp + 68h], ax
loc_5_3FC1:
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [esi + 0ch], ax
        mov    al, byte ptr [esp + 17h]
        mov    byte ptr [esi + 0eh], al
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [esi + 0fh], al
        db  0E8h, 09Eh, 0C9h, 0FAh, 0FFh
        mov    dword ptr [esi + 14h], eax
        mov    al, byte ptr [esp + 15h]
        mov    byte ptr [esi + 10h], al
        mov    dword ptr [esi + 2ch], 0
        mov    dword ptr [ebx + 20h], esi
        mov    al, byte ptr [esp + 11h]
        and    eax, 0ffff00ffh
        mov    word ptr [ebx + 18h], ax
        xor    eax, eax
        mov    al, byte ptr [esp + 72h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 73h]
        mov    dword ptr [ebx + 24h], eax
        mov    ax, word ptr [esp + 77h]
        lea    esi, [ebx + 10h]
        mov    word ptr [ebx + 2ah], ax
        mov    ax, word ptr [esp + 79h]
        mov    word ptr [ebx + 28h], ax
        mov    eax, dword ptr [esp + 34h]
        mov    al, byte ptr [eax + 1ch]
        cmp    al, 1
        jne    SHORT loc_5_4036
        mov    edx, dword ptr [esi]
        lea    ecx, [ebp + 30h]
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_403E
loc_5_4036:
        mov    ecx, dword ptr [esi]
        db  0C7h, 001h, 060h, 060h, 000h, 000h
loc_5_403E:
        cmp    al, 1
        jne    SHORT loc_5_4050
        mov    ecx, dword ptr [esi]
        lea    eax, [ebp + 38h]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_5_4059
        db  08Dh, 064h, 024h, 000h
loc_5_4050:
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 090h, 063h, 000h, 000h
loc_5_4059:
        mov    ecx, ebp
        call   sub_5_A8D0
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 008h, 0BCh, 014h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        pop    ebp
        inc    word ptr [eax + 50h]
        sub    eax, eax
        mov    word ptr [ebx + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
loc_5_4081:
        mov    eax, dword ptr [esp + 34h]
        cmp    byte ptr [eax + 1ch], 1
        jne    SHORT loc_5_40B0
        lea    eax, [esp + 6ch]
        push   6
        push   eax
        push   42h
        db  068h, 048h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 40h]
        push   ebx
        push   eax
        db  0E8h, 0DBh, 0C1h, 0FAh, 0FFh
        add    esp, 18h
        jmp    SHORT loc_5_40D9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_40B0:
        mov    eax, dword ptr [esp + 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [esp + 38h]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        push   6ah
        push   17h
        db  068h, 04Ch, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 44h]
        push   ebx
        push   eax
        db  0E8h, 0A9h, 0C1h, 0FAh, 0FFh
        add    esp, 1ch
loc_5_40D9:
        or     esi, esi
        je     SHORT loc_5_40E8
        push   0
        push   esi
        db  0E8h, 0ECh, 0C8h, 0FAh, 0FFh
        add    esp, 8
loc_5_40E8:
        or     ebp, ebp
        je     SHORT loc_5_40F6
        dec    dword ptr [ebp + 2ch]
        mov    ecx, ebp
        call   sub_5_A7F0
loc_5_40F6:
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 70h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_4110:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    ebp, dword ptr [edi + 1ch]
        mov    eax, dword ptr [ebp + 10h]
        lea    esi, [ebp + 34h]
        mov    dword ptr [esp + 14h], eax
        db  0E8h, 04Eh, 0C8h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        db  0B9h, 054h, 014h, 000h, 000h
        mov    eax, dword ptr [esi]
        cmp    eax, dword ptr [ecx]
        jne    SHORT loc_5_4159
        mov    ax, word ptr [esi + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    SHORT loc_5_4159
        mov    word ptr [edi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_4159:
        push   1
        push   30h
        db  0E8h, 05Ch, 0C8h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        mov    edx, 2
        mov    ecx, edi
        call   sub_5_A4A0
        mov    dword ptr [esp + 10h], eax
        or     ebx, ebx
        je     NEAR PTR loc_5_4232
        or     eax, eax
        je     NEAR PTR loc_5_4232
        mov    dword ptr [ebx], 454c4946h
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx + 4], eax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [ebx + 8], ax
        lea    ecx, [ebx + 4]
        mov    ax, word ptr [ebp + 3ah]
        mov    word ptr [ebx + 0ch], ax
        mov    byte ptr [ebx + 0eh], 2
        mov    byte ptr [ebx + 0fh], 0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [edi + 8]
        push   eax
        db  0E8h, 0DEh, 0C6h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [ebx + 10h], al
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [edi + 20h], ebx
        mov    word ptr [edi + 18h], 1
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 24h], 0
        mov    word ptr [edi + 2ah], 0
        mov    eax, dword ptr [edi + 10h]
        mov    word ptr [edi + 28h], 0
        db  0C7h, 000h, 060h, 060h, 000h, 000h
        mov    eax, dword ptr [edi + 10h]
        db  0C7h, 040h, 004h, 090h, 063h, 000h, 000h
        mov    eax, dword ptr [edi + 10h]
        db  0C7h, 040h, 008h, 0BCh, 014h, 000h, 000h
        db  0B8h, 054h, 014h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        inc    word ptr [eax + 50h]
        sub    eax, eax
        mov    word ptr [edi + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_4232:
        push   6
        push   esi
        push   42h
        db  068h, 050h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        push   edi
        push   eax
        db  0E8h, 038h, 0C0h, 0FAh, 0FFh
        db  0B8h, 054h, 014h, 000h, 000h
        add    esp, 18h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esi + 4], ax
        or     ebx, ebx
        je     SHORT loc_5_426A
        push   0
        push   ebx
        db  0E8h, 06Ah, 0C7h, 0FAh, 0FFh
        add    esp, 8
loc_5_426A:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_427A
        mov    ecx, dword ptr [esp + 10h]
        call   sub_5_A7F0
loc_5_427A:
        mov    word ptr [edi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_5_4290:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, esi
        mov    ebx, dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esp + 14h], eax
        db  0E8h, 041h, 0F9h, 0FAh, 0FFh
        mov    byte ptr [esp + 13h], al
        cmp    al, 80h
        jne    SHORT loc_5_42CA
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_42CA:
        test   byte ptr [esi], 58h
        je     SHORT loc_5_42E2
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_42E2:
        lea    eax, [edi + 4]
        mov    dword ptr [esp + 18h], eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        lea    ebp, [edi + eax]
        sub    edx, edx
        mov    ecx, esi
        call   sub_5_A4A0
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_5_431E
        inc    dword ptr [edi + 58h]
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_5_4318
loc_5_4309:
        push   edi
        db  0E8h, 0D5h, 0C7h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [edi + 4ch], 0
        jne    SHORT loc_5_4309
loc_5_4318:
        dec    dword ptr [edi + 58h]
        inc    dword ptr [edi + 2ch]
loc_5_431E:
        cmp    byte ptr [esp + 13h], 0ffh
        je     SHORT loc_5_435A
        push   0
        mov    eax, dword ptr [esp + 1ch]
        push   ebp
        push   eax
        push   4
        push   ebx
        mov    eax, dword ptr [esi]
        and    eax, 6
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 20h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 2fh]
        push   eax
        push   8
        db  068h, 054h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   esi
        push   eax
        db  0E8h, 02Ah, 0BFh, 0FAh, 0FFh
        add    esp, 30h
        jmp    SHORT loc_5_437C
loc_5_435A:
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        push   eax
        mov    eax, dword ptr [esi]
        and    eax, 6
        push   eax
        push   ebx
        push   44h
        db  068h, 060h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 2ch]
        push   esi
        push   eax
        db  0E8h, 006h, 0BFh, 0FAh, 0FFh
        add    esp, 20h
loc_5_437C:
        mov    bl, al
        or     edi, edi
        je     SHORT loc_5_4390
        dec    dword ptr [edi + 2ch]
        or     bl, bl
        jne    SHORT loc_5_4390
        mov    ecx, edi
        call   sub_5_A7F0
loc_5_4390:
        mov    ecx, ebx
        db  0E8h, 0B9h, 0F7h, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_43B0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 28h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, ebx
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ebx + 0ch]
        mov    al, byte ptr [eax + 1fh]
        mov    byte ptr [esp + 15h], al
        mov    edi, dword ptr [ebx + 10h]
        db  0E8h, 00Fh, 0F8h, 0FAh, 0FFh
        mov    byte ptr [esp + 14h], al
        cmp    al, 80h
        jne    SHORT loc_5_4400
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_4400:
        lea    ecx, [esi + 4]
        xor    eax, eax
        mov    al, byte ptr [ebx + 4]
        mov    dword ptr [esp + 18h], ecx
        or     eax, eax
        je     SHORT loc_5_4443
        cmp    eax, 1
        je     NEAR PTR loc_5_4572
        cmp    eax, 2
        je     NEAR PTR loc_5_462A
        cmp    eax, 3
        jl     SHORT loc_5_4430
        cmp    eax, 4
        jle    NEAR PTR loc_5_4766
loc_5_4430:
        mov    word ptr [ebx + 1ah], 1
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_4443:
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_5_447B
        cmp    byte ptr [esp + 14h], 4
        je     SHORT loc_5_447B
        mov    eax, dword ptr [esp + 1ch]
        cmp    byte ptr [eax + 21h], 4
        jne    SHORT loc_5_447B
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, esi
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 24h]
        db  0E8h, 08Eh, 0F7h, 0FAh, 0FFh
        or     eax, eax
        jne    SHORT loc_5_447B
        mov    byte ptr [esp + 14h], 4
loc_5_447B:
        cmp    byte ptr [esp + 14h], 0ffh
        je     SHORT loc_5_44F2
        xor    eax, eax
        db  0F6h, 005h, 065h, 004h, 000h, 000h, 001h
        mov    ax, word ptr [esi + 2]
        lea    edx, [esi + eax]
        je     SHORT loc_5_44A9
        cmp    byte ptr [esp + 14h], 4
        jne    SHORT loc_5_44A9
        mov    ecx, dword ptr [ebx + 28h]
        sub    ecx, 2
        mov    ax, word ptr [edx]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_5_44AB
loc_5_44A9:
        mov    ecx, edx
loc_5_44AB:
        xor    eax, eax
        mov    ax, word ptr [ecx]
        add    eax, ecx
        push   14h
        push   eax
        push   ecx
        push   0
        mov    ecx, dword ptr [esp + 28h]
        push   edx
        push   ecx
        mov    eax, dword ptr [esp + 34h]
        push   4
        push   eax
        push   0ffh
        push   0
        push   10h
        push   0
        push   0
        push   8
        xor    eax, eax
        mov    al, byte ptr [esp + 4ch]
        push   eax
        push   1
        db  068h, 088h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 64h]
        push   ebx
        push   eax
        db  0E8h, 092h, 0BDh, 0FAh, 0FFh
        add    esp, 4ch
        jmp    SHORT loc_5_4520
loc_5_44F2:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, esi
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 24h]
        push   0ffh
        push   eax
        push   0ah
        push   16h
        db  068h, 098h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   ebx
        push   eax
        db  0E8h, 062h, 0BDh, 0FAh, 0FFh
        add    esp, 24h
loc_5_4520:
        mov    byte ptr [esp + 12h], al
        cmp    byte ptr [esp + 12h], 0ffh
        jne    SHORT loc_5_4540
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_5_4540:
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_455A
        mov    edx, dword ptr [esp + 20h]
        db  068h, 090h, 0A7h, 000h, 000h
        mov    ecx, 3
        call   sub_5_2E60
loc_5_455A:
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 0EDh, 0F5h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_4572:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, esi
        cmp    byte ptr [esp + 14h], 0ffh
        je     SHORT loc_5_45B8
        xor    edx, edx
        mov    dl, byte ptr [esp + 14h]
        push   0
        mov    ecx, dword ptr [esp + 1ch]
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 28h]
        push   4
        push   eax
        push   16h
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        push   edx
        push   8
        db  068h, 0A0h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ebx
        push   eax
        db  0E8h, 0CCh, 0BCh, 0FAh, 0FFh
        add    esp, 30h
        jmp    SHORT loc_5_45DA
loc_5_45B8:
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   ecx
        push   eax
        push   0bh
        push   16h
        db  068h, 0ACh, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 38h]
        push   ebx
        push   eax
        db  0E8h, 0A8h, 0BCh, 0FAh, 0FFh
        add    esp, 20h
loc_5_45DA:
        mov    byte ptr [esp + 12h], al
        cmp    byte ptr [esp + 12h], 0ffh
        jne    SHORT loc_5_45F8
        mov    word ptr [ebx + 1ah], 3
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_45F8:
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_4612
        mov    edx, dword ptr [esp + 20h]
        db  068h, 070h, 0A7h, 000h, 000h
        mov    ecx, 3
        call   sub_5_2E60
loc_5_4612:
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 035h, 0F5h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_462A:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, eax
        cmp    esi, dword ptr [esp + 18h]
        jbe    SHORT loc_5_4690
        cmp    byte ptr [esp + 14h], 4
        jne    SHORT loc_5_4690
        xor    edx, edx
        mov    dl, byte ptr [esp + 14h]
        lea    eax, [esp + 16h]
        push   eax
        push   4
        push   0
        mov    ecx, dword ptr [esp + 24h]
        push   esi
        push   ecx
        mov    eax, dword ptr [esp + 30h]
        push   4
        push   eax
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        push   4
        push   16h
        push   edx
        push   edx
        push   6
        db  068h, 068h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 58h]
        push   ebx
        push   eax
        db  0E8h, 004h, 0BCh, 0FAh, 0FFh
        add    esp, 40h
        test   byte ptr [esp + 16h], 10h
        jne    NEAR PTR loc_5_4734
        mov    al, 0ffh
        jmp    NEAR PTR loc_5_4734
loc_5_4690:
        cmp    byte ptr [esp + 15h], 20h
        jae    SHORT loc_5_4704
        movzx  edi, byte ptr [esp + 15h]
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    dl, byte ptr [edi + eax]
        or     dl, dl
        je     SHORT loc_5_4704
        mov    ecx, dword ptr [esp + 18h]
        push   esi
        mov    eax, dword ptr [esp + 20h]
        push   ecx
        push   eax
        xor    eax, eax
        mov    al, dl
        push   eax
        push   0
        push   16h
        db  068h, 080h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   ebx
        push   eax
        db  0E8h, 0B3h, 0BBh, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        jne    SHORT loc_5_4734
        db  08Bh, 015h, 0D4h, 003h, 000h, 000h
        add    edx, edi
        cmp    byte ptr [edx], 0ffh
        je     SHORT loc_5_4734
        mov    byte ptr [edx], 0
        cmp    esi, dword ptr [esp + 18h]
        jbe    SHORT loc_5_4734
loc_5_46E9:
        mov    ecx, dword ptr [esp + 18h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        add    dword ptr [esp + 18h], ecx
        inc    byte ptr [edx]
        cmp    esi, dword ptr [esp + 18h]
        ja     SHORT loc_5_46E9
        jmp    SHORT loc_5_4734
loc_5_4704:
        cmp    esi, dword ptr [esp + 18h]
        ja     SHORT loc_5_4710
        sub    al, al
        jmp    SHORT loc_5_4734
        db  08Bh, 0FFh
loc_5_4710:
        mov    ecx, dword ptr [esp + 18h]
        push   esi
        push   ecx
        mov    eax, dword ptr [esp + 24h]
        push   16h
        push   eax
        push   -1
        push   40h
        db  068h, 078h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   ebx
        push   eax
        db  0E8h, 04Eh, 0BBh, 0FAh, 0FFh
        add    esp, 24h
loc_5_4734:
        cmp    al, 0ffh
        jne    SHORT loc_5_4750
        mov    word ptr [ebx + 1ah], 3
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_4750:
        mov    ecx, eax
        db  0E8h, 0F9h, 0F3h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_4766:
        mov    eax, dword ptr [esp + 1ch]
        lea    ebp, [edi + 4]
        cmp    byte ptr [eax + 21h], 4
        jne    NEAR PTR loc_5_48C0
        mov    ecx, dword ptr [esp + 18h]
        cmp    word ptr [ecx], 0
        je     NEAR PTR loc_5_48C0
        mov    byte ptr [esp + 13h], 0
        cmp    byte ptr [ebx + 4], 4
        jne    SHORT loc_5_47D2
        lea    eax, [esp + 13h]
        push   eax
        db  068h, 0B4h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        push   0
        push   eax
        push   13h
        push   16h
        db  068h, 0B8h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   ebx
        push   eax
        db  0E8h, 0CAh, 0BAh, 0FAh, 0FFh
        add    esp, 24h
        or     al, al
        je     SHORT loc_5_47D2
        mov    ecx, eax
        db  0E8h, 08Dh, 0F3h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_47D2:
        mov    esi, dword ptr [esp + 18h]
loc_5_47D6:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, eax
        mov    eax, ebp
        sub    eax, edi
        mov    word ptr [edi + 2], ax
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_5_47F4
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        jmp    SHORT loc_5_4800
loc_5_47F4:
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 1eh]
        and    eax, 0ffh
loc_5_4800:
        mov    dl, byte ptr [ebx + 4]
        lea    ecx, [ebp + 2]
        push   ecx
        push   4ch
        push   0
        mov    ecx, dword ptr [esp + 24h]
        push   esi
        push   ecx
        push   4
        push   eax
        mov    eax, dword ptr [esp + 38h]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        push   1
        push   16h
        lea    eax, [edx - 3]
        cmp    al, 1
        cmc
        sbb    eax, eax
        and    eax, 4
        push   eax
        sub    dl, 3
        cmp    dl, 1
        sbb    eax, eax
        and    eax, 4
        push   eax
        push   6
        db  068h, 0C0h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 58h]
        push   ebx
        push   eax
        db  0E8h, 030h, 0BAh, 0FAh, 0FFh
        mov    byte ptr [esp + 52h], al
        add    esp, 40h
        or     al, al
        jne    SHORT loc_5_4892
        sub    cx, cx
        cmp    word ptr [ebp + 2], cx
        je     SHORT loc_5_4876
        sub    eax, eax
loc_5_4865:
        inc    cx
        mov    edx, ecx
        and    edx, 0ffffh
        cmp    word ptr [ebp + edx*2 + 2], ax
        jne    SHORT loc_5_4865
loc_5_4876:
        lea    eax, [ecx*2 + 2]
        mov    word ptr [ebp], ax
        and    eax, 0ffffh
        add    ebp, eax
        cmp    word ptr [esi], 0
        jne    NEAR PTR loc_5_47D6
loc_5_4892:
        sub    ebp, edi
        mov    word ptr [edi], bp
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_5_48DB
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        push   eax
        push   14h
        push   16h
        db  068h, 0D0h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        push   ebx
        push   eax
        db  0E8h, 0C6h, 0B9h, 0FAh, 0FFh
        add    esp, 18h
        jmp    SHORT loc_5_48DB
        db  08Bh, 0FFh
loc_5_48C0:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [esp + 12h], 0
loc_5_48DB:
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 06Ch, 0F2h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_4900:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    bp, 6
        mov    edi, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, esi
        db  0E8h, 0CDh, 0F2h, 0FAh, 0FFh
        cmp    al, 80h
        jne    SHORT loc_5_493A
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_493A:
        xor    ecx, ecx
        mov    cx, word ptr [ebx]
        add    ecx, ebx
        mov    dword ptr [esp + 14h], ecx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 4]
        or     ecx, ecx
        je     SHORT loc_5_4970
        cmp    ecx, 1
        je     NEAR PTR loc_5_4A3B
        mov    word ptr [esi + 1ah], 1
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_4970:
        cmp    word ptr [ebx + 4], 0
        jne    SHORT loc_5_4984
        mov    dword ptr [esi], 10h
        sub    al, al
        jmp    NEAR PTR loc_5_4A25
loc_5_4984:
        cmp    al, 0ffh
        mov    dword ptr [esi], 0
        je     SHORT loc_5_49E4
        and    eax, 0ffh
        mov    dword ptr [esp + 1ch], eax
loc_5_4997:
        push   esi
        push   4
        mov    ecx, dword ptr [esp + 1ch]
        push   0
        lea    eax, [ebx + 4]
        push   ecx
        push   eax
        push   4
        push   edi
        xor    eax, eax
        mov    al, byte ptr [edi + 20h]
        push   eax
        push   4
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   eax
        push   eax
        push   6
        db  068h, 0D4h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   esi
        push   eax
        db  0E8h, 0AFh, 0B8h, 0FAh, 0FFh
        add    esp, 40h
        test   bp, 10h
        jne    SHORT loc_5_4A25
        or     bp, 10h
        or     al, al
        jne    SHORT loc_5_4997
        jmp    SHORT loc_5_4A25
loc_5_49E4:
        push   esi
        mov    ecx, dword ptr [esp + 18h]
        push   0eh
        push   ecx
        lea    eax, [ebx + 4]
        push   eax
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        push   edi
        push   -1
        push   40h
        db  068h, 0E4h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 34h]
        push   esi
        push   eax
        db  0E8h, 072h, 0B8h, 0FAh, 0FFh
        add    esp, 2ch
        test   bp, 10h
        jne    SHORT loc_5_4A1F
        or     bp, 10h
        or     al, al
        jne    SHORT loc_5_49E4
loc_5_4A1F:
        and    dword ptr [esi], 0bfh
loc_5_4A25:
        mov    ecx, eax
        db  0E8h, 024h, 0F1h, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_4A3B:
        cmp    al, 0ffh
        je     SHORT loc_5_4A97
        and    eax, 0ffh
        mov    dword ptr [esp + 18h], eax
loc_5_4A48:
        mov    ecx, dword ptr [esp + 14h]
        push   0
        push   ecx
        lea    eax, [ebx + 4]
        push   eax
        push   4
        push   edi
        xor    eax, eax
        mov    al, byte ptr [edi + 20h]
        push   eax
        push   24h
        mov    eax, dword ptr [esi]
        push   eax
        mov    eax, ebp
        push   2
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [esp + 40h]
        push   eax
        push   eax
        push   7
        db  068h, 0F0h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   esi
        push   eax
        db  0E8h, 0FCh, 0B7h, 0FAh, 0FFh
        add    esp, 40h
        test   bp, 10h
        jne    SHORT loc_5_4AD0
        or     bp, 10h
        or     al, al
        jne    SHORT loc_5_4A48
        jmp    SHORT loc_5_4AD0
loc_5_4A97:
        mov    ecx, dword ptr [esp + 14h]
        push   ecx
        lea    eax, [ebx + 4]
        push   eax
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        push   edi
        mov    eax, dword ptr [esi]
        push   eax
        push   46h
        db  068h, 0FCh, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 2ch]
        push   esi
        push   eax
        db  0E8h, 0C1h, 0B7h, 0FAh, 0FFh
        add    esp, 24h
        test   bp, 10h
        jne    SHORT loc_5_4AD0
        or     bp, 10h
        or     al, al
        jne    SHORT loc_5_4A97
loc_5_4AD0:
        mov    ecx, eax
        db  0E8h, 079h, 0F0h, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_4AF0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  068h, 0B0h, 0A7h, 000h, 000h
        mov    edx, dword ptr [esi + 1ch]
        mov    ecx, 3
        call   sub_5_2DC0
        cmp    word ptr [esi + 18h], 1
        jne    SHORT loc_5_4B20
        db  068h, 010h, 0A7h, 000h, 000h
        mov    edx, dword ptr [esi + 1ch]
        mov    ecx, 3
        call   sub_5_2DC0
loc_5_4B20:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_PTEXT ends
        end
