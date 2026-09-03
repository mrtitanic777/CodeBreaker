; nwredir_pcode_05_03.asm -- NWREDIR object 5 segment _PTEXT  (part 3/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_9820:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    esi, ecx
        mov    dword ptr [esp + 10h], 0
        push   edi
        db  0E8h, 0EAh, 072h, 0FAh, 0FFh
        add    esp, 4
        mov    ebp, dword ptr [edi + 0ch]
        add    ebp, 4
        and    eax, 0fh
        je     SHORT loc_5_9860
        cmp    eax, 2
        je     SHORT loc_5_98B0
        cmp    eax, 4
        je     SHORT loc_5_98B0
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_9860:
        push   edi
        db  0E8h, 0C9h, 072h, 0FAh, 0FFh
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0DCh, 003h, 000h, 000h
        add    esp, 4
        mov    eax, dword ptr [eax + ecx*4 - 4]
        or     eax, eax
        jne    SHORT loc_5_9890
        mov    dword ptr [esi + 4], 0
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_9890:
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esi + 4], ecx
        mov    al, byte ptr [eax + 1eh]
        mov    byte ptr [esi + 8], al
        sub    al, al
        mov    dword ptr [esi], ebp
        mov    byte ptr [esi + 9], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_98B0:
        cmp    word ptr [ebp + 2], 2ah
        jne    SHORT loc_5_98D0
        mov    ecx, edi
        call   sub_5_2040
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_5_98D0
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_5_98D0:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_5_998C
        push   1
        push   2fh
        xor    eax, eax
        mov    ax, word ptr [ebp]
        sub    eax, 4
        push   eax
        lea    eax, [ebp + 4]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        db  0E8h, 0C3h, 071h, 0FAh, 0FFh
        add    esp, 14h
        mov    byte ptr [esp + eax + 14h], 0
        sub    cl, cl
        db  066h, 083h, 03Dh, 014h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_9985
loc_5_990B:
        xor    eax, eax
        mov    al, cl
        mov    ebx, eax
        shl    ebx, 4
        lea    edi, [esp + 14h]
        lea    ebx, [ebx + ebx*2]
        db  003h, 01Dh, 0A8h, 003h, 000h, 000h
loc_5_9921:
        mov    dl, byte ptr [edi]
        cmp    dl, byte ptr [ebx]
        jne    SHORT loc_5_9941
        or     dl, dl
        je     SHORT loc_5_993D
        mov    dl, byte ptr [edi + 1]
        cmp    dl, byte ptr [ebx + 1]
        jne    SHORT loc_5_9941
        add    edi, 2
        add    ebx, 2
        or     dl, dl
        jne    SHORT loc_5_9921
loc_5_993D:
        sub    edx, edx
        jmp    SHORT loc_5_9946
loc_5_9941:
        sbb    edx, edx
        sbb    edx, -1
loc_5_9946:
        or     edx, edx
        jne    SHORT loc_5_995D
        lea    eax, [eax + eax*4]
        shl    eax, 5
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        cmp    byte ptr [eax + edx + 34h], 0
        jne    SHORT loc_5_9971
loc_5_995D:
        inc    cl
        xor    edx, edx
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        mov    dl, cl
        cmp    edx, eax
        jb     SHORT loc_5_990B
        jmp    SHORT loc_5_9985
loc_5_9971:
        xor    eax, eax
        mov    al, cl
        lea    eax, [eax + eax*4]
        shl    eax, 5
        db  003h, 005h, 0A0h, 003h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
loc_5_9985:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_99C0
loc_5_998C:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4], eax
        xor    eax, eax
        mov    ax, word ptr [ebp]
        add    eax, ebp
        mov    dword ptr [esi], eax
        mov    byte ptr [esi + 8], 0
        mov    byte ptr [esi + 9], 1
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        cmp    word ptr [eax + ecx], 1
        sbb    al, al
        neg    al
        mov    byte ptr [esi + 0ah], al
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_5_99C0:
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_99D0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    bl, bl
        sub    bp, bp
        db  0E8h, 09Ah, 06Fh, 0FAh, 0FFh
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        cmp    byte ptr [eax + ecx + 48h], bl
        mov    esi, dword ptr [esp + 28h]
        je     SHORT loc_5_99F8
        mov    bl, byte ptr [esi + 1ch]
        mov    bp, word ptr [esi + 8]
loc_5_99F8:
        cmp    bl, 1
        jbe    SHORT loc_5_9A10
        mov    byte ptr [esi + 1ch], 0fbh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_9A10:
        lea    ecx, [esp + 10h]
        mov    edi, dword ptr [esp + 24h]
        mov    edx, edi
        call   sub_5_9820
        or     al, al
        je     SHORT loc_5_9A31
        mov    byte ptr [esi + 1ch], 3
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_9A31:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_5_9A46
        mov    byte ptr [esi + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_9A46:
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_A9D0
        mov    eax, dword ptr [edi + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    edx, dword ptr [esp + 10h]
        push   0deh
        mov    dx, word ptr [edx]
        shr    dx, 1
        and    edx, 0ffffh
        add    edx, 5
        push   edx
        xor    edx, edx
        mov    dx, word ptr [eax + ecx]
        add    edx, ecx
        add    edx, eax
        mov    eax, dword ptr [esp + 18h]
        push   edx
        push   eax
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    al, bl
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 30h]
        push   eax
        push   3
        db  0B8h, 0FCh, 019h, 000h, 000h
        cmp    byte ptr [esp + 39h], 0
        jne    SHORT loc_5_9AA8
        db  0B8h, 004h, 01Ah, 000h, 000h
loc_5_9AA8:
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   edi
        push   eax
        db  0E8h, 0CBh, 067h, 0FAh, 0FFh
        add    esp, 2ch
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9AD0:
        sub    esp, 0ch
        push   esi
        push   edi
        lea    ecx, [esp + 8]
        mov    esi, dword ptr [esp + 1ch]
        mov    edx, esi
        call   sub_5_9820
        or     al, al
        je     SHORT loc_5_9AF8
        mov    edi, dword ptr [esp + 20h]
        sub    eax, eax
        mov    byte ptr [edi + 1ch], 3
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
loc_5_9AF8:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_5_9B10
        mov    edi, dword ptr [esp + 20h]
        sub    eax, eax
        mov    byte ptr [edi + 1ch], 0
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  090h
loc_5_9B10:
        mov    edx, dword ptr [esi + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 2]
        mov    eax, dword ptr [esp + 8]
        push   0deh
        mov    ax, word ptr [eax]
        shr    ax, 1
        and    eax, 0ffffh
        add    eax, 2
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx]
        add    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 20h]
        mov    edi, dword ptr [esp + 30h]
        push   eax
        mov    al, byte ptr [edi + 1dh]
        sub    al, 0ech
        cmp    al, 1
        sbb    al, al
        and    al, 0feh
        add    al, 7
        and    eax, 0ffh
        push   eax
        db  0B8h, 00Ch, 01Ah, 000h, 000h
        cmp    byte ptr [esp + 29h], 0
        jne    SHORT loc_5_9B70
        db  0B8h, 014h, 01Ah, 000h, 000h
loc_5_9B70:
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   esi
        push   eax
        db  0E8h, 003h, 067h, 0FAh, 0FFh
        add    esp, 24h
        mov    byte ptr [edi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_9B90:
        sub    esp, 0ch
        lea    eax, [esp]
        push   eax
        db  0E8h, 0FBh, 06Fh, 0FAh, 0FFh
        mov    al, byte ptr [esp + 8]
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        mov    byte ptr [ecx + 19h], al
        mov    al, byte ptr [esp + 5]
        mov    byte ptr [ecx + 18h], al
        mov    al, byte ptr [esp + 6]
        mov    byte ptr [ecx + 11h], al
        mov    al, byte ptr [esp + 7]
        mov    byte ptr [ecx + 10h], al
        mov    al, byte ptr [esp + 8]
        mov    byte ptr [ecx + 1dh], al
        mov    al, byte ptr [esp + 9]
        mov    byte ptr [ecx + 1ch], al
        sub    eax, eax
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9BE0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        mov    ecx, ebx
        call   sub_5_2040
        mov    esi, dword ptr [esp + 18h]
        xor    eax, eax
        mov    al, byte ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_5_9C20
        cmp    eax, 1
        je     SHORT loc_5_9C28
        cmp    eax, 2
        je     SHORT loc_5_9C30
        cmp    eax, 3
        je     SHORT loc_5_9C38
        cmp    eax, 4
        je     SHORT loc_5_9C40
        mov    byte ptr [esi + 1ch], 0ffh
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_9C20:
        db  08Bh, 03Dh, 0D0h, 003h, 000h, 000h
        jmp    SHORT loc_5_9C46
loc_5_9C28:
        db  08Bh, 03Dh, 0C8h, 003h, 000h, 000h
        jmp    SHORT loc_5_9C46
loc_5_9C30:
        db  08Bh, 03Dh, 0CCh, 003h, 000h, 000h
        jmp    SHORT loc_5_9C46
loc_5_9C38:
        db  08Bh, 03Dh, 0A4h, 003h, 000h, 000h
        jmp    SHORT loc_5_9C46
loc_5_9C40:
        db  08Bh, 03Dh, 0A8h, 003h, 000h, 000h
loc_5_9C46:
        mov    word ptr [esi + 1ch], 0
        test   byte ptr [ebx + 42h], 4
        je     SHORT loc_5_9C65
        db  0E8h, 024h, 06Dh, 0FAh, 0FFh
        mov    eax, dword ptr [eax + 4]
        add    eax, edi
        mov    dword ptr [esi + 4], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_9C65:
        lea    eax, [esi + 4]
        push   eax
        add    esi, 38h
        push   esi
        push   1000h
        push   edi
        db  0E8h, 028h, 06Dh, 0FAh, 0FFh
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9C90:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 0E2h, 06Ch, 0FAh, 0FFh
        db  08Bh, 03Dh, 004h, 004h, 000h, 000h
        add    edi, eax
        mov    ebp, dword ptr [esp + 18h]
        mov    ecx, ebp
        call   sub_5_2040
        or     eax, eax
        mov    esi, dword ptr [esp + 1ch]
        jne    SHORT loc_5_9CC0
        mov    byte ptr [esi + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_9CC0:
        mov    bl, byte ptr [esi + 14h]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 1ch]
        cmp    ecx, 5
        ja     SHORT loc_5_9CD4
        db  0FFh, 024h, 08Dh, 0FCh, 09Dh, 000h, 000h
loc_5_9CD4:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_5_9CE0:
        or     bl, bl
        je     SHORT loc_5_9D37
        cmp    bl, 8
        ja     SHORT loc_5_9CFA
        xor    eax, eax
        db  08Bh, 00Dh, 0ACh, 003h, 000h, 000h
        mov    al, bl
        cmp    dword ptr [ecx + eax*4 - 4], 0
        jne    SHORT loc_5_9D37
loc_5_9CFA:
        cmp    bl, 81h
        jb     NEAR PTR loc_5_9DF1
        xor    eax, eax
        mov    al, bl
        xor    edx, edx
        db  066h, 08Bh, 015h, 014h, 004h, 000h, 000h
        lea    ecx, [eax - 81h]
        cmp    ecx, edx
        jge    NEAR PTR loc_5_9DF1
        lea    ecx, [eax + eax*4]
        shl    ecx, 5
        db  0A1h, 0A0h, 003h, 000h, 000h
        cmp    byte ptr [ecx + eax - 506ch], 0
        je     NEAR PTR loc_5_9DF1
loc_5_9D37:
        mov    byte ptr [edi + 4ah], bl
        sub    bl, bl
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_9D46:
        mov    bl, byte ptr [edi + 4ah]
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_9D53:
        mov    bl, byte ptr [eax + 56h]
        inc    bl
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_9D62:
        xor    edx, edx
        db  08Bh, 00Dh, 0E4h, 003h, 000h, 000h
        db  08Ah, 015h, 0EEh, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx + edx*8]
        or     ecx, ecx
        je     SHORT loc_5_9D90
        mov    byte ptr [esi + 1dh], 0ffh
        mov    eax, dword ptr [ecx + 10h]
        pop    ebp
        mov    bl, byte ptr [eax + 56h]
        inc    bl
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_5_9D90:
        mov    bl, byte ptr [eax + 56h]
        inc    bl
        mov    byte ptr [esi + 1dh], 0
        db  088h
loc_5_9D9A:
        pop    esi
        sbb    al, 2bh
        rcr    byte ptr [ebp + 5fh], 5eh
        pop    ebx
        ret
loc_5_9DA3:
        or     bl, bl
        je     SHORT loc_5_9DBD
        cmp    bl, 8
        ja     SHORT loc_5_9DF1
        xor    eax, eax
        db  08Bh, 00Dh, 0ACh, 003h, 000h, 000h
        mov    al, bl
        cmp    dword ptr [ecx + eax*4 - 4], 0
        je     SHORT loc_5_9DF1
loc_5_9DBD:
        db  088h, 01Dh, 0ECh, 003h, 000h, 000h
        sub    bl, bl
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_9DD0:
        db  0E8h, 0FBh, 06Eh, 0FAh, 0FFh
        db  08Ah, 01Dh, 0ECh, 003h, 000h, 000h
        or     bl, bl
        jne    SHORT loc_5_9DEC
        mov    ecx, ebp
        call   sub_5_1F70
        db  08Ah, 01Dh, 0ECh, 003h, 000h, 000h
loc_5_9DEC:
        db  0E8h, 03Fh, 06Fh, 0FAh, 0FFh
loc_5_9DF1:
        mov    byte ptr [esi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_9DFC:
        loopne loc_5_9D9A
        add    byte ptr [eax], al
        inc    esi
        popfd
        add    byte ptr [eax], al
        push   ebx
        popfd
        add    byte ptr [eax], al
        db  062h, 09Dh, 000h, 000h, 0A3h, 09Dh
        add    byte ptr [eax], al
        rcr    byte ptr [ebp + 50000h], 1
        add    byte ptr [eax], al
        add    byte ptr [ebp + 24a4h], cl
        add    byte ptr [eax], al
loc_5_9E20:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        mov    al, byte ptr [eax + 14h]
        dec    al
        cmp    al, 8
        jb     SHORT loc_5_9E46
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        mov    byte ptr [eax + 1ch], 0f7h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_9E46:
        and    eax, 0ffh
        mov    dword ptr [esp + 14h], eax
        mov    ecx, eax
        db  0A1h, 0ACh, 003h, 000h, 000h
        db  08Bh, 01Dh, 0A4h, 003h, 000h, 000h
        mov    esi, dword ptr [eax + ecx*4]
        shl    ecx, 5
        mov    eax, dword ptr [esp + 24h]
        add    ebx, ecx
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        or     eax, eax
        je     SHORT loc_5_9E93
        cmp    eax, 1
        je     NEAR PTR loc_5_9F79
        cmp    eax, 2
        je     NEAR PTR loc_5_9FAA
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_9E93:
        or     esi, esi
        je     SHORT loc_5_9EA1
        mov    byte ptr [esp + 13h], 0f5h
        jmp    NEAR PTR loc_5_A002
loc_5_9EA1:
        sub    ebp, ebp
        db  066h, 039h, 02Dh, 014h, 004h, 000h, 000h
        je     SHORT loc_5_9EF9
        db  0A1h, 0A0h, 003h, 000h, 000h
        sub    edx, edx
loc_5_9EB3:
        cmp    byte ptr [eax + 34h], dl
        je     SHORT loc_5_9ED4
        cmp    byte ptr [eax + 5bh], dl
        jne    SHORT loc_5_9ED4
        lea    esi, [eax + 78h]
        lea    edi, [ebx + 2]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_9ED2
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
loc_5_9ED2:
        je     SHORT loc_5_9EE9
loc_5_9ED4:
        add    eax, 0a0h
        inc    ebp
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, ebp
        ja     SHORT loc_5_9EB3
        jmp    SHORT loc_5_9EF9
loc_5_9EE9:
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, eax
        call   sub_5_1D00
        mov    byte ptr [esp + 13h], 0
loc_5_9EF9:
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, ebp
        jne    NEAR PTR loc_5_A002
        call   sub_5_1A70
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_9F20
        mov    byte ptr [esp + 13h], 0f4h
        jmp    NEAR PTR loc_5_A002
        db  08Bh, 0FFh
loc_5_9F20:
        push   esi
        db  0E8h, 02Ah, 06Eh, 0FAh, 0FFh
        add    esp, 4
        lea    edx, [esi + 78h]
        mov    eax, dword ptr [ebx + 2]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebx + 6]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebx + 0ah]
        lea    ecx, [ebx + 2]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   eax
        push   esi
        call   sub_5_13D0
        mov    byte ptr [esp + 1fh], al
        add    esp, 0ch
        or     al, al
        je     SHORT loc_5_9F60
        mov    byte ptr [ebx], 0
        jmp    SHORT loc_5_9F6B
        db  08Dh, 049h, 000h
loc_5_9F60:
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_5_1D00
loc_5_9F6B:
        push   esi
        db  0E8h, 01Fh, 06Eh, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_A002
loc_5_9F79:
        or     esi, esi
        jne    SHORT loc_5_9F87
        mov    byte ptr [esp + 13h], 0ffh
        jmp    NEAR PTR loc_5_A002
loc_5_9F87:
        cmp    word ptr [esi + 50h], 0
        jbe    SHORT loc_5_9F95
        mov    byte ptr [esp + 13h], 1
        jmp    SHORT loc_5_A002
loc_5_9F95:
        mov    eax, dword ptr [esp + 20h]
        push   eax
        push   esi
        call   sub_5_16D0
        mov    byte ptr [esp + 1bh], 0
        add    esp, 8
        jmp    SHORT loc_5_A002
loc_5_9FAA:
        or     esi, esi
        jne    SHORT loc_5_9FB5
        mov    byte ptr [esp + 13h], 0ffh
        jmp    SHORT loc_5_A002
loc_5_9FB5:
        cmp    word ptr [esi + 50h], 0
        jbe    SHORT loc_5_9FC3
        mov    byte ptr [esp + 13h], 1
        jmp    SHORT loc_5_A002
loc_5_9FC3:
        mov    ecx, esi
        call   sub_5_7230
        push   19h
        mov    eax, dword ptr [esp + 24h]
        db  068h, 018h, 01Ah, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 0A3h, 062h, 0FAh, 0FFh
        mov    byte ptr [esp + 23h], al
        add    esp, 10h
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_5_9FF8
        push   0
        push   eax
        db  0E8h, 0DCh, 069h, 0FAh, 0FFh
        add    esp, 8
loc_5_9FF8:
        mov    dword ptr [esi + 88h], 0
loc_5_A002:
        mov    eax, dword ptr [esp + 24h]
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_A020:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 3ch]
        call   sub_5_2040
        mov    esi, eax
        or     esi, esi
        mov    eax, dword ptr [esp + 40h]
        jne    SHORT loc_5_A048
        mov    byte ptr [eax + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_A048:
        mov    al, byte ptr [eax + 1ch]
        mov    byte ptr [esp + 13h], al
        mov    eax, dword ptr [esp + 40h]
        mov    edx, 3c04h
        mov    di, word ptr [eax + 18h]
        mov    bx, word ptr [eax + 14h]
        mov    ecx, dword ptr [esp + 3ch]
        call   sub_5_6D80
        mov    dword ptr [esp + 20h], eax
        or     bx, bx
        je     SHORT loc_5_A088
        mov    edx, 3800h
        mov    ecx, dword ptr [esp + 3ch]
        call   sub_5_6D80
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 18h], eax
loc_5_A088:
        mov    byte ptr [esp + 12h], 0ffh
        and    edi, 0ffffh
        mov    eax, dword ptr [esp + 20h]
        push   edi
        push   eax
        db  0E8h, 0B2h, 069h, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_A126
        or     bx, bx
        je     SHORT loc_5_A0FC
        and    ebx, 0ffffh
        mov    eax, dword ptr [esp + 18h]
        push   ebx
        push   eax
        db  0E8h, 093h, 069h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_A11C
        push   ebx
        push   eax
        push   edi
        push   ebp
        xor    eax, eax
        mov    al, byte ptr [esp + 23h]
        push   eax
        mov    eax, dword ptr [esp + 50h]
        db  068h, 01Ch, 01Ah, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 09Ch, 061h, 0FAh, 0FFh
        mov    byte ptr [esp + 32h], al
        add    esp, 20h
        push   edi
        push   ebp
        db  0E8h, 09Ch, 069h, 0FAh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [esp + 18h]
        push   ebx
        push   eax
        jmp    SHORT loc_5_A11E
loc_5_A0FC:
        push   edi
        push   ebp
        xor    eax, eax
        mov    al, byte ptr [esp + 1bh]
        push   eax
        mov    eax, dword ptr [esp + 48h]
        db  068h, 024h, 01Ah, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 06Ah, 061h, 0FAh, 0FFh
        mov    byte ptr [esp + 2ah], al
        add    esp, 18h
loc_5_A11C:
        push   edi
        push   ebp
loc_5_A11E:
        db  0E8h, 06Ah, 069h, 0FAh, 0FFh
        add    esp, 8
loc_5_A126:
        mov    eax, dword ptr [esp + 20h]
        cmp    byte ptr [eax + 2], 79h
        jne    NEAR PTR loc_5_A1CE
        cmp    byte ptr [esp + 13h], 17h
        jne    NEAR PTR loc_5_A1CE
        cmp    byte ptr [esp + 12h], 0
        jne    NEAR PTR loc_5_A1CE
        mov    word ptr [esp + 30h], 0
        mov    eax, dword ptr [esp + 1ch]
        mov    ax, word ptr [eax + 3eh]
        mov    word ptr [esp + 2eh], ax
        mov    eax, dword ptr [esp + 1ch]
        mov    ax, word ptr [eax + 3eh]
        mov    word ptr [esp + 2ch], ax
        inc    byte ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [esp + 28h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 24h], eax
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 1ch]
        mov    ax, word ptr [eax + 26h]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        push   ecx
        mov    ax, word ptr [esp + 18h]
        push   eax
        mov    ax, word ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        push   esi
        mov    eax, dword ptr [esp + 50h]
        mov    esi, dword ptr [eax + 8]
        db  0E8h, 0B6h, 067h, 0FAh, 0FFh
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_2F20
loc_5_A1CE:
        mov    eax, dword ptr [esp + 40h]
        mov    cl, byte ptr [esp + 12h]
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_A1F0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 14h]
        or     ax, ax
        jne    SHORT loc_5_A213
        mov    ecx, dword ptr [esp + 20h]
        call   sub_5_2040
        mov    dword ptr [esp + 14h], eax
        jmp    SHORT loc_5_A23B
loc_5_A213:
        db  066h, 03Bh, 005h, 014h, 004h, 000h, 000h
        ja     SHORT loc_5_A233
        mov    ecx, eax
        db  0A1h, 0ACh, 003h, 000h, 000h
        and    ecx, 0ffffh
        mov    eax, dword ptr [eax + ecx*4 - 4]
        mov    dword ptr [esp + 14h], eax
        jmp    SHORT loc_5_A23B
loc_5_A233:
        mov    dword ptr [esp + 14h], 0
loc_5_A23B:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_5_A256
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        mov    word ptr [eax + 1ch], 0ffh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_A256:
        mov    edx, 3c04h
        mov    ecx, dword ptr [esp + 20h]
        call   sub_5_6D80
        mov    esi, eax
        mov    edx, 3800h
        mov    ecx, dword ptr [esp + 20h]
        call   sub_5_6D80
        mov    edi, eax
        mov    cx, word ptr [esi]
        mov    ax, word ptr [esi + 4]
        mov    word ptr [esp + 12h], ax
        mov    eax, dword ptr [esp + 24h]
        cmp    cx, 6
        mov    bx, word ptr [eax + 18h]
        jb     NEAR PTR loc_5_A330
        cmp    bx, 8
        jb     NEAR PTR loc_5_A330
        sub    cx, 6
        mov    byte ptr [esp + 11h], 0ffh
        mov    ebp, ecx
        and    ebp, 0ffffh
        push   ebp
        add    esi, 6
        push   esi
        db  0E8h, 097h, 067h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_5_A317
        and    ebx, 0ffffh
        push   ebx
        push   edi
        db  0E8h, 081h, 067h, 0FAh, 0FFh
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_5_A30D
        push   ebx
        push   edi
        push   ebp
        push   esi
        xor    eax, eax
        mov    ax, word ptr [esp + 22h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        db  068h, 028h, 01Ah, 000h, 000h
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        db  0E8h, 087h, 05Fh, 0FAh, 0FFh
        mov    byte ptr [esp + 31h], al
        add    esp, 20h
        push   ebp
        push   esi
        db  0E8h, 087h, 067h, 0FAh, 0FFh
        add    esp, 8
        push   ebx
        push   edi
        jmp    SHORT loc_5_A30F
loc_5_A30D:
        push   ebp
        push   esi
loc_5_A30F:
        db  0E8h, 079h, 067h, 0FAh, 0FFh
        add    esp, 8
loc_5_A317:
        mov    eax, dword ptr [esp + 24h]
        mov    cl, byte ptr [esp + 11h]
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_A330:
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        mov    word ptr [eax + 1ch], 0fbh
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_A350:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, 3800h
        mov    esi, dword ptr [esp + 30h]
        mov    ecx, esi
        call   sub_5_6D80
        mov    edi, eax
        mov    ax, word ptr [edi]
        push   eax
        push   esi
        db  0E8h, 0B6h, 067h, 0FAh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_A390
        mov    ecx, dword ptr [esp + 34h]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_5_A390:
        mov    eax, dword ptr [esi + 20h]
        add    eax, 4
        lea    edx, [esp + 18h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esi + 20h]
        mov    ebp, dword ptr [esi + 1ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [esp + 14h], eax
        mov    ax, word ptr [edi + 2]
        push   eax
        push   esi
        db  0E8h, 06Bh, 067h, 0FAh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_A3D8
        mov    ecx, dword ptr [esp + 34h]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_A3D8:
        mov    eax, dword ptr [esi + 20h]
        add    eax, 4
        lea    edx, [esp + 20h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [eax + 18h]
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ecx + 10h], eax
        je     SHORT loc_5_A411
        mov    ecx, dword ptr [esp + 34h]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_A411:
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_A7B0
        mov    ecx, ebx
        call   sub_5_A710
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 8]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [esp + 30h]
        push   6
        push   eax
        lea    eax, [esp + 30h]
        push   6
        push   eax
        push   4ah
        db  068h, 030h, 01Ah, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 02Bh, 05Eh, 0FAh, 0FFh
        add    esp, 30h
        or     al, al
        jne    SHORT loc_5_A46E
        mov    edx, dword ptr [edi + 8]
        add    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ebx + 5ch]
        cmp    edx, ecx
        ja     SHORT loc_5_A46B
        mov    edx, ecx
loc_5_A46B:
        mov    dword ptr [ebx + 5ch], edx
loc_5_A46E:
        mov    ecx, dword ptr [esp + 34h]
        mov    byte ptr [ecx + 1ch], al
        mov    eax, dword ptr [esp + 10h]
        shr    eax, 10h
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [ecx + 18h], ax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A4A0:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, ecx
        mov    dword ptr [esp + 20h], ecx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 14h], eax
        mov    eax, ecx
        mov    dword ptr [esp + 24h], edx
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [esp + 10h], ecx
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_AA10
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        xor    ebp, ebp
        mov    bp, word ptr [eax]
        add    ebp, 2
        mov    eax, dword ptr [esp + 24h]
        and    eax, 3
        mov    dword ptr [esp + 28h], eax
        cmp    eax, 2
        je     NEAR PTR loc_5_A5B8
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, ecx
        inc    word ptr [ecx + 26h]
        add    eax, 14h
        mov    dword ptr [esp + 18h], eax
        mov    ebx, eax
        mov    edx, 2
loc_5_A506:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, dword ptr [esp + 18h]
        je     NEAR PTR loc_5_A595
        xor    eax, eax
        mov    ax, word ptr [ebx + 6ah]
        cmp    eax, dword ptr [esp + 1ch]
        jne    SHORT loc_5_A506
        test   dword ptr [ebx + 54h], edx
        jne    SHORT loc_5_A506
        mov    ecx, ebp
        shr    ecx, 2
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [ebx + 78h]
        repe cmpsd
        jne    SHORT loc_5_A506
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_5_A506
        test   byte ptr [esp + 24h], 10h
        jne    SHORT loc_5_A55E
        cmp    dword ptr [ebx + 28h], 0
        je     SHORT loc_5_A55E
loc_5_A549:
        inc    dword ptr [ebx + 58h]
        push   ebx
        db  0E8h, 092h, 065h, 0FAh, 0FFh
        add    esp, 4
        dec    dword ptr [ebx + 58h]
        cmp    dword ptr [ebx + 28h], 0
        jne    SHORT loc_5_A549
loc_5_A55E:
        test   byte ptr [esp + 24h], 20h
        je     SHORT loc_5_A568
        inc    dword ptr [ebx + 2ch]
loc_5_A568:
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [ecx + 26h]
        dec    ax
        mov    word ptr [ecx + 26h], ax
        jne    SHORT loc_5_A58B
        test   byte ptr [ecx + 24h], 1
        je     SHORT loc_5_A58B
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0E8h, 010h, 0B0h, 0FAh, 0FFh
        add    esp, 4
loc_5_A58B:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_A595:
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [ecx + 26h]
        dec    ax
        mov    word ptr [ecx + 26h], ax
        jne    SHORT loc_5_A5B8
        test   byte ptr [ecx + 24h], 1
        je     SHORT loc_5_A5B8
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0E8h, 0E3h, 0AFh, 0FAh, 0FFh
        add    esp, 4
loc_5_A5B8:
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_5_A5C9
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_A5C9:
        push   7ch
        db  0E8h, 008h, 065h, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_5_A690
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 1fh
        rep stosd
        push   ebp
        db  0E8h, 0EAh, 064h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 78h], eax
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_5_A671
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 8], ecx
        lea    eax, [ebx - 10h]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 20h], ebx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 1ch], ebx
        test   byte ptr [eax + 3], 24h
        je     SHORT loc_5_A625
        or     dword ptr [ebx + 54h], 20h
loc_5_A625:
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [ebx + 6ah], ax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 14h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx + 18h], ebx
        mov    ecx, ebx
        mov    byte ptr [ebx + 38h], 0e9h
        mov    byte ptr [ebx + 30h], 0e9h
        call   sub_5_A8D0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_A671:
        push   ebx
        db  0E8h, 067h, 064h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 4
        mov    word ptr [eax + 1ah], 24h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_5_A690:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    word ptr [eax + 1ah], 24h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A6B0:
        mov    edx, dword ptr [esp + 4]
        or     edx, edx
        je     SHORT loc_5_A6C8
        db  068h, 0B0h, 0A7h, 000h, 000h
        mov    ecx, 3
        call   sub_5_2E60
        ret
loc_5_A6C8:
        db  0BAh, 0B0h, 0A7h, 000h, 000h
        mov    ecx, 3
        jmp    NEAR PTR sub_5_2EB0
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A6E0:
        mov    edx, dword ptr [esp + 4]
        or     edx, edx
        je     SHORT loc_5_A6F8
        db  068h, 010h, 0A7h, 000h, 000h
        mov    ecx, 3
        call   sub_5_2E60
        ret
loc_5_A6F8:
        db  0BAh, 010h, 0A7h, 000h, 000h
        mov    ecx, 3
        jmp    NEAR PTR sub_5_2EB0
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A710:
        push   esi
        push   edi
        mov    esi, ecx
        sub    edi, edi
loc_5_A716:
        cmp    dword ptr [esi + 40h], edi
        jne    SHORT loc_5_A720
        cmp    dword ptr [esi + 44h], edi
        je     SHORT loc_5_A750
loc_5_A720:
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 58h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 041h, 09Fh, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 0ACh, 063h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 051h, 09Fh, 0FAh, 0FFh
        add    esp, 4
        dec    dword ptr [esi + 58h]
        jmp    SHORT loc_5_A716
        db  090h
loc_5_A750:
        inc    dword ptr [esi + 50h]
        db  068h, 0C3h, 047h, 000h, 000h
        mov    edx, esi
        mov    ecx, 4
        call   sub_5_2D90
        dec    dword ptr [esi + 50h]
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_A770:
        test   byte ptr [ecx + 68h], 1
        jne    SHORT loc_5_A781
        test   byte ptr [ecx + 55h], 80h
        jne    SHORT loc_5_A781
        jmp    NEAR PTR sub_5_A7B0
loc_5_A781:
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_A790:
        test   byte ptr [ecx + 68h], 1
        jne    SHORT loc_5_A7A1
        test   byte ptr [ecx + 55h], 80h
        jne    SHORT loc_5_A7A1
        jmp    NEAR PTR sub_5_A710
loc_5_A7A1:
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A7B0:
        push   esi
        mov    esi, ecx
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_5_A7EC
loc_5_A7B9:
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 58h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 0A8h, 09Eh, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 013h, 063h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 0B8h, 09Eh, 0FAh, 0FFh
        add    esp, 4
        dec    dword ptr [esi + 58h]
        cmp    dword ptr [esi + 44h], 0
        jne    SHORT loc_5_A7B9
loc_5_A7EC:
        sub    eax, eax
        pop    esi
        ret
sub_5_A7F0:
        push   esi
        mov    esi, ecx
        mov    ecx, esi
        inc    dword ptr [esi + 2ch]
        call   sub_5_A7B0
        mov    ecx, esi
        call   sub_5_A710
        mov    eax, dword ptr [esi + 2ch]
        dec    eax
        mov    dword ptr [esi + 2ch], eax
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_5_A857
        or     eax, eax
        jne    SHORT loc_5_A857
        mov    eax, dword ptr [esi + 54h]
        test   al, 2
        jne    SHORT loc_5_A829
        or     eax, 2
        mov    dword ptr [esi + 54h], eax
        mov    ecx, esi
        call   sub_5_A8D0
loc_5_A829:
        mov    eax, esi
        sub    eax, dword ptr [esi + 0ch]
        cmp    eax, 10h
        jne    SHORT loc_5_A857
        mov    eax, dword ptr [esi + 8]
        or     dword ptr [esi + 54h], 80h
        cmp    word ptr [eax + 26h], 0
        je     SHORT loc_5_A850
        or     word ptr [eax + 24h], 1
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_A850:
        mov    ecx, esi
        db  0E8h, 069h, 05Ah, 0FAh, 0FFh
loc_5_A857:
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A860:
        mov    eax, dword ptr [ecx + 8]
        db  080h, 03Dh, 06Bh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        je     SHORT loc_5_A89B
        mov    edx, dword ptr [ecx + 54h]
        test   edx, 1004h
        jne    SHORT loc_5_A89B
        mov    al, byte ptr [eax + 58h]
        test   al, 0e0h
        jne    SHORT loc_5_A89B
        test   byte ptr [ecx + 68h], 1
        jne    SHORT loc_5_A895
        test   dh, 80h
        jne    SHORT loc_5_A895
        test   al, 2
        jne    SHORT loc_5_A89B
        test   dh, 1
        jne    SHORT loc_5_A89B
loc_5_A895:
        mov    eax, 1
        ret
loc_5_A89B:
        sub    eax, eax
        ret
        db  08Bh, 0FFh
sub_5_A8A0:
        push   esi
        mov    esi, ecx
        call   sub_5_A860
        or     eax, eax
        je     SHORT loc_5_A8CA
        db  0E8h, 077h, 059h, 0FAh, 0FFh
        shl    eax, 0ch
        cmp    eax, dword ptr [esi + 5ch]
        jae    SHORT loc_5_A8C3
        test   byte ptr [esi + 55h], 20h
        je     SHORT loc_5_A8C3
        sub    eax, eax
        pop    esi
        ret
loc_5_A8C3:
        mov    eax, 1
        pop    esi
        ret
loc_5_A8CA:
        sub    eax, eax
        pop    esi
        ret
        db  08Bh, 0FFh
sub_5_A8D0:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        sub    edi, edi
        call   sub_5_A860
        cmp    eax, edi
        jne    SHORT loc_5_A8F9
        mov    eax, dword ptr [esi + 54h]
        lea    ebx, [esi + 54h]
        test   ah, 8
        je     SHORT loc_5_A8F0
        mov    edi, 1
loc_5_A8F0:
        and    eax, 0fffff3ffh
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_5_A916
loc_5_A8F9:
        mov    ecx, esi
        call   sub_5_A8A0
        or     eax, eax
        lea    ebx, [esi + 54h]
        jne    SHORT loc_5_A910
        and    dword ptr [ebx], 0fffffbffh
        jmp    SHORT loc_5_A916
        db  090h
loc_5_A910:
        or     dword ptr [ebx], 0c00h
loc_5_A916:
        mov    eax, dword ptr [ebx]
        test   ah, 4
        je     SHORT loc_5_A940
        db  0B9h, 0A0h, 05Fh, 000h, 000h
        test   ah, 2
        db  0BAh, 000h, 060h, 000h, 000h
        jne    SHORT loc_5_A931
        db  0BAh, 090h, 063h, 000h, 000h
loc_5_A931:
        or     eax, 400h
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_5_A94A
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_A940:
        db  0B9h, 060h, 060h, 000h, 000h
        db  0BAh, 090h, 063h, 000h, 000h
loc_5_A94A:
        mov    eax, 0ffffffd0h
        sub    eax, esi
        add    eax, ecx
        sub    eax, 5
        mov    dword ptr [esi + 31h], eax
        mov    eax, 0ffffffc8h
        sub    eax, esi
        add    eax, edx
        sub    eax, 5
        mov    dword ptr [esi + 39h], eax
        cmp    edi, 1
        jne    SHORT loc_5_A987
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 10h]
        test   byte ptr [eax + 58h], 80h
        jne    SHORT loc_5_A987
        mov    ecx, esi
        call   sub_5_A7B0
        mov    ecx, esi
        call   sub_5_A710
loc_5_A987:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_5_A990:
        db  0C6h, 005h, 06Bh, 004h, 000h, 000h, 000h
        db  0BAh, 0D0h, 0A8h, 000h, 000h
        mov    ecx, 3
        jmp    NEAR PTR sub_5_2EB0
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A9B0:
        db  0C6h, 005h, 06Bh, 004h, 000h, 000h, 001h
        db  0BAh, 0D0h, 0A8h, 000h, 000h
        mov    ecx, 3
        jmp    NEAR PTR sub_5_2EB0
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_A9D0:
        mov    al, byte ptr [ecx + 58h]
        test   al, 2
        jne    SHORT loc_5_A9ED
        or     al, 2
        mov    byte ptr [ecx + 58h], al
        db  068h, 0D0h, 0A8h, 000h, 000h
        mov    edx, ecx
        mov    ecx, 3
        call   sub_5_2E60
loc_5_A9ED:
        ret
        db  08Bh, 0FFh
sub_5_A9F0:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [eax + 10h]
        jmp    NEAR PTR sub_5_A9D0
        db  005h, 000h, 000h, 000h, 000h
sub_5_AA00:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_AA10:
        sub    eax, eax
        cmp    word ptr [ecx], ax
        je     SHORT loc_5_AA27
loc_5_AA17:
        xor    edx, edx
        mov    dx, word ptr [ecx]
        add    ecx, 2
        add    eax, edx
        cmp    word ptr [ecx], 0
        jne    SHORT loc_5_AA17
loc_5_AA27:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_AA30:
        push   esi
        mov    esi, ecx
        call   sub_5_AF00
        mov    byte ptr [eax + 14h], 4
        mov    dword ptr [eax + 2ch], esi
        mov    ecx, dword ptr [esi + 8]
        sub    edx, edx
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 8], 0
        and    dword ptr [esi + 54h], 0ffffffbfh
        mov    ecx, eax
        inc    dword ptr [esi + 28h]
        call   sub_5_BCF0
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_AA70:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esp + 14h], ecx
        cmp    byte ptr [eax + 14h], 1
        jne    NEAR PTR loc_5_AB92
        mov    ebx, dword ptr [ecx]
        cmp    ebx, ecx
        je     NEAR PTR loc_5_AB92
loc_5_AA92:
        lea    edx, [ebx + 0ch]
        mov    dword ptr [esp + 1ch], edx
        mov    edx, dword ptr [edx]
        lea    esi, [ebx + 10h]
        mov    dword ptr [esp + 18h], esi
        mov    eax, dword ptr [esi]
        add    eax, edx
        lea    ecx, [eax - 1]
        xor    ecx, edx
        test   ecx, 0fffffe00h
        je     NEAR PTR loc_5_AB86
        test   edx, 1ffh
        je     SHORT loc_5_AB22
        db  0E8h, 06Dh, 09Dh, 0FAh, 0FFh
        mov    ebp, eax
        mov    edi, ebp
        mov    esi, ebx
        mov    ecx, 16h
        rep movsd
        mov    eax, 200h
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 1ffh
        sub    eax, ecx
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [ebp + 10h], eax
        mov    edx, dword ptr [esp + 1ch]
        sub    dword ptr [esi], eax
        add    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebp], ebx
        mov    dword ptr [eax], ebp
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 4], ebp
        inc    byte ptr [ecx + 2dh]
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [ebp + 28h], ebx
        mov    dword ptr [eax + 28h], ebp
        mov    dword ptr [ebx + 2ch], ebp
        mov    ebx, ebp
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        inc    word ptr [eax + 19h]
        db  0E8h, 03Fh, 0AAh, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_AB86
loc_5_AB22:
        test   eax, 1ffh
        je     SHORT loc_5_AB86
        db  0E8h, 003h, 09Dh, 0FAh, 0FFh
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, 16h
        rep movsd
        mov    esi, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi]
        mov    ecx, edi
        and    ecx, 1ffh
        sub    edi, ecx
        mov    dword ptr [esi], edi
        mov    edx, dword ptr [edx]
        add    edx, edi
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 4], eax
        inc    byte ptr [ecx + 2dh]
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [eax + 28h], ebx
        mov    dword ptr [ecx + 28h], eax
        mov    dword ptr [ebx + 2ch], eax
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        inc    word ptr [eax + 19h]
        db  0E8h, 0D9h, 0A9h, 0FAh, 0FFh
        add    esp, 4
loc_5_AB86:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, dword ptr [esp + 14h]
        jne    NEAR PTR loc_5_AA92
loc_5_AB92:
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [ecx]
        cmp    edi, ecx
        je     NEAR PTR loc_5_ADFB
loc_5_ABA0:
        mov    esi, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 24h]
        test   al, 4
        je     NEAR PTR loc_5_ACFA
        mov    cx, word ptr [esi + 19h]
        dec    cx
        mov    word ptr [esi + 19h], cx
        jne    SHORT loc_5_ABBE
        mov    byte ptr [esi + 32h], 0
loc_5_ABBE:
        mov    ebx, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [edi + 10h]
        or     ecx, ebx
        test   ecx, 1ffh
        je     NEAR PTR loc_5_ADA2
        mov    dl, 1
        mov    ecx, ebx
        shr    ecx, 9
        and    cl, 7
        shl    dl, cl
        mov    byte ptr [esp + 13h], dl
        mov    cl, dl
        test   byte ptr [esi + 14h], cl
        jne    NEAR PTR loc_5_ADA2
        mov    edx, dword ptr [esi + 28h]
        mov    ebp, edx
        or     eax, 8
        mov    dword ptr [edi + 24h], eax
        cmp    esi, ebp
        je     NEAR PTR loc_5_ADA2
loc_5_AC00:
        cmp    ebp, edi
        je     SHORT loc_5_AC2B
        test   byte ptr [ebp + 24h], 30h
        jne    SHORT loc_5_AC2B
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, ebx
        jb     SHORT loc_5_AC2B
        mov    ecx, dword ptr [edi + 10h]
        add    ecx, ebx
        cmp    ecx, dword ptr [ebp + 0ch]
        jb     SHORT loc_5_AC2B
        mov    ecx, ebx
        xor    ecx, dword ptr [ebp + 0ch]
        test   ecx, 0fffffe00h
        je     SHORT loc_5_AC37
loc_5_AC2B:
        mov    ebp, dword ptr [ebp + 28h]
        cmp    esi, ebp
        jne    SHORT loc_5_AC00
        jmp    NEAR PTR loc_5_ADA2
loc_5_AC37:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, ebx
        jb     SHORT loc_5_AC40
        mov    ecx, ebx
loc_5_AC40:
        mov    dword ptr [esp + 20h], ecx
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, dword ptr [ebp + 0ch]
        add    ebx, dword ptr [edi + 10h]
        cmp    ecx, ebx
        ja     SHORT loc_5_AC53
        mov    ecx, ebx
loc_5_AC53:
        mov    ebx, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 0ch], ebx
        sub    ecx, ebx
        mov    dword ptr [ebp + 10h], ecx
        and    eax, 0fffffff7h
        mov    dword ptr [edi + 24h], eax
        cmp    ecx, 200h
        jne    NEAR PTR loc_5_ADA2
        mov    cl, byte ptr [esp + 13h]
        test   byte ptr [esi + 16h], cl
        jne    NEAR PTR loc_5_ADA2
        cmp    esi, edx
        je     SHORT loc_5_ACE5
        mov    ebp, 30h
loc_5_AC87:
        mov    ebx, dword ptr [edx + 28h]
        cmp    edx, edi
        je     SHORT loc_5_ACDF
        mov    eax, dword ptr [edx + 0ch]
        xor    eax, dword ptr [edi + 0ch]
        test   eax, 0fffffe00h
        jne    SHORT loc_5_ACDF
        test   dword ptr [edx + 24h], ebp
        jne    SHORT loc_5_ACDF
        mov    ecx, dword ptr [edx + 2ch]
        mov    eax, ebx
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, dword ptr [edx + 28h]
        mov    eax, dword ptr [edx + 2ch]
        mov    dword ptr [ecx + 2ch], eax
        mov    ecx, dword ptr [esp + 14h]
        cmp    dword ptr [edx + 20h], ecx
        jne    SHORT loc_5_ACD0
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 20h], 0
loc_5_ACD0:
        cmp    dword ptr [edx + 20h], 0
        jne    SHORT loc_5_ACDF
        push   edx
        db  0E8h, 016h, 055h, 0FAh, 0FFh
        add    esp, 4
loc_5_ACDF:
        mov    edx, ebx
        cmp    esi, ebx
        jne    SHORT loc_5_AC87
loc_5_ACE5:
        mov    al, 1
        mov    ecx, dword ptr [edi + 0ch]
        shr    ecx, 9
        and    cl, 7
        shl    al, cl
        or     byte ptr [esi + 14h], al
        jmp    NEAR PTR loc_5_ADA2
loc_5_ACFA:
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, eax
        and    ebx, 0fffffe00h
        mov    ecx, dword ptr [edi + 10h]
        sub    ecx, ebx
        add    ecx, eax
        add    ecx, 1ffh
        and    ecx, 0fffffe00h
        mov    dl, 1
        shr    ecx, 9
        shl    dl, cl
        dec    dl
        mov    ecx, ebx
        shr    ecx, 9
        and    cl, 7
        shl    dl, cl
        mov    cl, dl
        not    cl
        and    byte ptr [esi + 16h], cl
        mov    al, byte ptr [esi + 17h]
        test   al, dl
        je     SHORT loc_5_AD40
        and    al, cl
        mov    byte ptr [esi + 17h], al
        jmp    SHORT loc_5_AD46
loc_5_AD40:
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_5_AD4F
loc_5_AD46:
        push   esi
        db  0E8h, 09Eh, 05Dh, 0FAh, 0FFh
        add    esp, 4
loc_5_AD4F:
        mov    edx, dword ptr [esi + 28h]
        cmp    esi, edx
        je     SHORT loc_5_ADA2
loc_5_AD56:
        mov    ecx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edx + 28h]
        mov    ebp, dword ptr [ecx + 14h]
        mov    eax, dword ptr [edx + 0ch]
        cmp    ebp, eax
        ja     SHORT loc_5_AD9C
        mov    cx, word ptr [ecx + 28h]
        and    ecx, 0fe00h
        add    ecx, ebp
        mov    ebp, dword ptr [edx + 10h]
        add    ebp, eax
        cmp    ecx, ebp
        jb     SHORT loc_5_AD9C
        test   byte ptr [edx + 24h], 10h
        jne    SHORT loc_5_AD9C
        mov    ecx, dword ptr [edx + 2ch]
        mov    eax, ebx
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, dword ptr [edx + 28h]
        mov    eax, dword ptr [edx + 2ch]
        mov    dword ptr [ecx + 2ch], eax
        push   edx
        db  0E8h, 059h, 054h, 0FAh, 0FFh
        add    esp, 4
loc_5_AD9C:
        mov    edx, ebx
        cmp    esi, ebx
        jne    SHORT loc_5_AD56
loc_5_ADA2:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   esi
        db  0E8h, 0D3h, 0A7h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [edi + 24h]
        test   al, 8
        jne    SHORT loc_5_ADE0
        mov    ecx, dword ptr [edi + 2ch]
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, dword ptr [edi + 28h]
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ecx + 2ch], eax
        push   edi
        db  0E8h, 019h, 054h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_ADED
        db  08Bh, 0FFh
loc_5_ADE0:
        mov    dword ptr [edi + 20h], 0
        and    eax, 0ffffffefh
        mov    dword ptr [edi + 24h], eax
loc_5_ADED:
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [ecx]
        cmp    edi, ecx
        jne    NEAR PTR loc_5_ABA0
loc_5_ADFB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_AE10:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    esi, dword ptr [ebx + 18h]
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 10h]
        mov    eax, dword ptr [esi + 48h]
        dec    eax
        mov    dword ptr [esi + 48h], eax
        jne    SHORT loc_5_AE3C
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_5_AE3C
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_5_AE3C
        push   esi
        db  0E8h, 0B1h, 05Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_AE3C:
        mov    ecx, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebx + 20h]
        mov    dword ptr [eax + 20h], ecx
        mov    ax, word ptr [edi + 50h]
        dec    ax
        mov    word ptr [edi + 50h], ax
        jne    SHORT loc_5_AE77
        mov    al, byte ptr [edi + 58h]
        test   al, 2
        je     SHORT loc_5_AE77
        and    al, 0fdh
        mov    byte ptr [edi + 58h], al
        db  068h, 0D0h, 0A8h, 000h, 000h
        mov    edx, edi
        mov    ecx, 3
        call   sub_5_2E60
loc_5_AE77:
        test   byte ptr [esi + 54h], 40h
        je     SHORT loc_5_AE84
        mov    ecx, esi
        call   sub_5_AA30
loc_5_AE84:
        push   esi
        db  0E8h, 0E6h, 053h, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_AEA0:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    al, byte ptr [esi + 14h]
        or     al, al
        je     SHORT loc_5_AEB0
        cmp    al, 1
        jne    SHORT loc_5_AEEF
loc_5_AEB0:
        mov    edi, dword ptr [esi + 0ch]
        cmp    edi, esi
        je     SHORT loc_5_AED0
loc_5_AEB7:
        mov    ebx, dword ptr [edi + 0ch]
        mov    ecx, edi
        call   sub_5_AA70
        push   edi
        db  0E8h, 02Bh, 053h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, ebx
        cmp    ebx, esi
        jne    SHORT loc_5_AEB7
loc_5_AED0:
        cmp    byte ptr [esi + 14h], 1
        jne    SHORT loc_5_AEEF
        mov    eax, dword ptr [esi + 2ch]
        mov    edx, dword ptr [esi + 4ch]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 2ch]
        mov    edi, eax
        cmp    edx, edi
        jb     SHORT loc_5_AEEA
        mov    edx, edi
loc_5_AEEA:
        sub    eax, edx
        mov    dword ptr [ecx + 2ch], eax
loc_5_AEEF:
        push   esi
        db  0E8h, 0FDh, 052h, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_AF00:
        db  0E8h, 02Ch, 099h, 0FAh, 0FFh
        or     eax, eax
        je     SHORT loc_5_AF53
        lea    ecx, [eax + 34h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    dword ptr [ecx + 0ch], 0
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [eax + 24h], 0
        mov    dword ptr [eax + 30h], 0
        mov    byte ptr [eax + 17h], 0
        mov    dword ptr [eax + 1ch], 0
        mov    dword ptr [eax + 2ch], 0
        mov    byte ptr [eax + 15h], 8
        ret
loc_5_AF53:
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_AF60:
        push   esi
        push   edi
        mov    esi, 40h
loc_5_AF67:
        lea    eax, [esi + esi*4]
        lea    eax, [esi + eax*2]
        shl    eax, 3
        add    eax, 8
        push   eax
        db  0E8h, 05Fh, 05Bh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_AF90
        sar    esi, 1
        cmp    esi, 2
        jg     SHORT loc_5_AF67
        or     eax, eax
        jne    SHORT loc_5_AF90
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_5_AF90:
        mov    dword ptr [eax + 4], esi
        db  08Bh, 00Dh, 074h, 01Ah, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 074h, 01Ah, 000h, 000h
        lea    edi, [eax + 8]
        db  08Bh, 00Dh, 054h, 01Ah, 000h, 000h
        add    ecx, esi
        db  089h, 00Dh, 054h, 01Ah, 000h, 000h
        db  08Bh, 00Dh, 058h, 01Ah, 000h, 000h
        add    ecx, esi
        db  089h, 00Dh, 058h, 01Ah, 000h, 000h
        or     esi, esi
        jle    SHORT loc_5_AFD4
loc_5_AFC3:
        mov    eax, edi
        add    edi, 58h
        push   eax
        db  0E8h, 024h, 052h, 0FAh, 0FFh
        add    esp, 4
        dec    esi
        jne    SHORT loc_5_AFC3
loc_5_AFD4:
        db  0A1h, 070h, 01Ah, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_AFE0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [ebx + 8]
        mov    esi, dword ptr [eax + 10h]
        mov    ebp, dword ptr [esi]
        cmp    ebp, esi
        je     SHORT loc_5_B039
loc_5_AFF9:
        mov    edi, ebp
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_5_B035
        cmp    byte ptr [edi + 14h], 1
        jne    SHORT loc_5_B035
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 48h], eax
        jbe    SHORT loc_5_B035
        mov    eax, dword ptr [edi + 4]
        mov    ecx, edi
        mov    dword ptr [eax], ebp
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [esi + 44h]
        mov    byte ptr [eax + 96h], 0
        call   sub_5_BDC0
        mov    ecx, edi
        call   sub_5_AEA0
loc_5_B035:
        cmp    ebp, esi
        jne    SHORT loc_5_AFF9
loc_5_B039:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B050:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_AFE0
        mov    ecx, esi
        call   sub_5_A7B0
        mov    ecx, esi
        call   sub_5_A710
        mov    dword ptr [esi + 5ch], edi
        pop    edi
        pop    esi
        ret
        db  090h
loc_5_B070:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, ecx
        test   byte ptr [edi + 15h], 1
        jne    SHORT loc_5_B0CE
        mov    esi, dword ptr [edi + 28h]
        cmp    esi, edi
        je     SHORT loc_5_B0CE
        mov    ebx, 10h
        sub    ebp, ebp
        lea    ebx, [ebx]
loc_5_B090:
        db  0A1h, 054h, 01Ah, 000h, 000h
        db  039h, 005h, 05Ch, 01Ah, 000h, 000h
        ja     SHORT loc_5_B0D5
        test   dword ptr [esi + 24h], ebx
        jne    SHORT loc_5_B0C7
        cmp    dword ptr [esi + 20h], ebp
        jne    SHORT loc_5_B0C7
        mov    ecx, esi
        mov    esi, dword ptr [esi + 2ch]
        mov    eax, dword ptr [ecx + 2ch]
        mov    edx, dword ptr [ecx + 28h]
        mov    dword ptr [eax + 28h], edx
        mov    edx, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [edx + 2ch], eax
        push   ecx
        db  0E8h, 02Eh, 051h, 0FAh, 0FFh
        add    esp, 4
loc_5_B0C7:
        mov    esi, dword ptr [esi + 28h]
        cmp    esi, edi
        jne    SHORT loc_5_B090
loc_5_B0CE:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_B0D5:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_B0E0:
        push   ecx
        db  0E8h, 08Ah, 095h, 0FAh, 0FFh
        add    esp, 4
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_B0F0:
        push   ecx
        db  0E8h, 0A2h, 095h, 0FAh, 0FFh
        add    esp, 4
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_B100:
        db  0A1h, 054h, 01Ah, 000h, 000h
        db  039h, 005h, 05Ch, 01Ah, 000h, 000h
        jae    SHORT loc_5_B15E
        db  0BAh, 0E0h, 0B0h, 000h, 000h
        mov    ecx, 1
        call   sub_5_2EB0
        db  0A1h, 054h, 01Ah, 000h, 000h
        db  039h, 005h, 05Ch, 01Ah, 000h, 000h
        jae    SHORT loc_5_B14F
loc_5_B129:
        push   1
        db  0E8h, 0CFh, 058h, 0FAh, 0FFh
        add    esp, 4
        db  0BAh, 070h, 0B0h, 000h, 000h
        mov    ecx, 4
        call   sub_5_2EB0
        db  0A1h, 054h, 01Ah, 000h, 000h
        db  039h, 005h, 05Ch, 01Ah, 000h, 000h
        jb     SHORT loc_5_B129
loc_5_B14F:
        db  0BAh, 0F0h, 0B0h, 000h, 000h
        mov    ecx, 1
        call   sub_5_2EB0
loc_5_B15E:
        ret
        db  090h
sub_5_B160:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 10h]
        sub    eax, eax
        mov    edx, dword ptr [esi]
        cmp    esi, edx
        je     SHORT loc_5_B189
        mov    ebx, 1
loc_5_B173:
        cmp    byte ptr [edx + 14h], bl
        jne    SHORT loc_5_B183
        mov    edi, dword ptr [edx + 2ch]
        cmp    dword ptr [edi + 8], ecx
        jne    SHORT loc_5_B183
        add    eax, dword ptr [edx + 4ch]
loc_5_B183:
        mov    edx, dword ptr [edx]
        cmp    esi, edx
        jne    SHORT loc_5_B173
loc_5_B189:
        mov    edx, dword ptr [esi + 8]
        add    esi, 8
        cmp    esi, edx
        je     SHORT loc_5_B1AE
        mov    ebx, 1
loc_5_B198:
        cmp    byte ptr [edx + 14h], bl
        jne    SHORT loc_5_B1A8
        mov    edi, dword ptr [edx + 2ch]
        cmp    dword ptr [edi + 8], ecx
        jne    SHORT loc_5_B1A8
        add    eax, dword ptr [edx + 4ch]
loc_5_B1A8:
        mov    edx, dword ptr [edx]
        cmp    esi, edx
        jne    SHORT loc_5_B198
loc_5_B1AE:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B1C0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_5_B1DE
        call   sub_5_002F
loc_5_B1DE:
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [esp + 14h], ecx
        mov    edi, dword ptr [ecx + 8]
        mov    ebx, 2710h
        cmp    dword ptr [edi + 2ch], 0a00000h
        ja     SHORT loc_5_B1FE
        mov    ebx, 3e8h
loc_5_B1FE:
        add    ebx, dword ptr [edi + 30h]
        db  0E8h, 0E7h, 057h, 0FAh, 0FFh
        cmp    ebx, eax
        jae    SHORT loc_5_B259
        lea    eax, [esp + 12h]
        push   eax
        mov    ecx, esi
        lea    eax, [esp + 20h]
        lea    edx, [esp + 24h]
        push   eax
        call   sub_5_4B30
        or     eax, eax
        jne    SHORT loc_5_B252
        movzx  eax, word ptr [esp + 12h]
        imul   eax, eax, 200h
        imul   dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        or     edx, edx
        jne    SHORT loc_5_B23D
        mov    ecx, eax
loc_5_B23D:
        mov    dword ptr [esp + 18h], ecx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 2ch], eax
        db  0E8h, 0A0h, 057h, 0FAh, 0FFh
        mov    dword ptr [edi + 30h], eax
        jmp    SHORT loc_5_B259
loc_5_B252:
        mov    dword ptr [edi + 2ch], 0
loc_5_B259:
        mov    ecx, edi
        call   sub_5_B160
        mov    ecx, dword ptr [esi]
        add    ecx, eax
        add    ecx, 100000h
        cmp    ecx, dword ptr [edi + 2ch]
        jbe    SHORT loc_5_B280
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_B280:
        mov    ebx, dword ptr [edi + 10h]
        call   sub_5_B100
        call   sub_5_AF00
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_B2A0
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_B2A0:
        mov    dword ptr [edi + 28h], esi
        mov    byte ptr [edi + 14h], 1
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 48h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4ch], eax
        mov    dword ptr [edi + 18h], ebx
        sub    ebx, ebx
        mov    dword ptr [edi + 10h], edi
        mov    dword ptr [edi + 0ch], edi
loc_5_B2C6:
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   edi
        db  0E8h, 02Ch, 09Dh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_B304
        db  0E8h, 011h, 057h, 0FAh, 0FFh
        lea    ebp, [eax + 1f4h]
        db  0E8h, 006h, 057h, 0FAh, 0FFh
        cmp    eax, ebp
        jae    SHORT loc_5_B2FE
        add    eax, 0
loc_5_B2F0:
        call   sub_5_00C5
        db  0E8h, 0F3h, 056h, 0FAh, 0FFh
        cmp    eax, ebp
        jb     SHORT loc_5_B2F0
loc_5_B2FE:
        inc    ebx
        cmp    ebx, 5
        jl     SHORT loc_5_B2C6
loc_5_B304:
        cmp    ebx, 5
        jne    SHORT loc_5_B331
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_A7B0
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_A710
        push   edi
        db  0E8h, 0D1h, 04Eh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_B331:
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edi + 28h], 0
        mov    ecx, dword ptr [ecx + 18h]
        mov    eax, dword ptr [esi + 20h]
        cmp    ecx, eax
        je     SHORT loc_5_B35F
        or     dword ptr [eax + 2ch], 2
        or     ecx, ecx
        jne    SHORT loc_5_B35F
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esp + 14h]
        or     dword ptr [eax + 2ch], 8
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 18h], eax
loc_5_B35F:
        mov    ecx, dword ptr [esp + 14h]
        or     dword ptr [ecx + 54h], 40h
        cmp    dword ptr [edi + 0ch], edi
        je     SHORT loc_5_B390
        db  0E8h, 07Ch, 056h, 0FAh, 0FFh
        add    eax, 0c8h
        mov    dword ptr [edi + 30h], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 54h], eax
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_BCF0
        jmp    SHORT loc_5_B397
        db  08Dh, 049h, 000h
loc_5_B390:
        mov    ecx, edi
        call   sub_5_AEA0
loc_5_B397:
        mov    eax, dword ptr [esi]
        add    eax, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 24h], eax
        cmp    dword ptr [ecx + 5ch], eax
        jae    SHORT loc_5_B3AB
        mov    dword ptr [ecx + 5ch], eax
loc_5_B3AB:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B3C0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [eax + 18h]
        mov    eax, dword ptr [esi + 24h]
        mov    ebp, dword ptr [edi + 60h]
        mov    edx, dword ptr [edi + 64h]
        lea    ecx, [ebp + edx]
        cmp    ecx, eax
        ja     SHORT loc_5_B3F2
        add    ecx, 1000h
        cmp    ecx, eax
        jb     SHORT loc_5_B3F2
        lea    ebx, [edi + 54h]
        and    dword ptr [ebx], 0ffff9fffh
        jmp    SHORT loc_5_B40D
loc_5_B3F2:
        mov    ecx, dword ptr [edi + 54h]
        lea    ebx, [edi + 54h]
        test   ch, 40h
        je     SHORT loc_5_B405
        or     ecx, 2000h
        jmp    SHORT loc_5_B40B
loc_5_B405:
        or     ecx, 4000h
loc_5_B40B:
        mov    dword ptr [ebx], ecx
loc_5_B40D:
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_5_B461
        cmp    dword ptr [esi], edx
        jne    SHORT loc_5_B450
        cmp    eax, ebp
        jne    SHORT loc_5_B450
        mov    al, byte ptr [edi + 76h]
        inc    al
        mov    byte ptr [edi + 76h], al
        cmp    al, 4
        ja     SHORT loc_5_B431
        db  0E8h, 0C1h, 055h, 0FAh, 0FFh
        cmp    eax, dword ptr [edi + 72h]
        jbe    SHORT loc_5_B461
loc_5_B431:
        or     dword ptr [ebx], 100h
        mov    ecx, edi
        call   sub_5_A8D0
        mov    ecx, edi
        call   sub_5_A7B0
        mov    ecx, edi
        call   sub_5_A710
        jmp    SHORT loc_5_B461
        db  08Bh, 0FFh
loc_5_B450:
        mov    byte ptr [edi + 76h], 0
        db  0E8h, 094h, 055h, 0FAh, 0FFh
        add    eax, 7d0h
        mov    dword ptr [edi + 72h], eax
loc_5_B461:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 64h], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 60h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B480:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        call   sub_5_B3C0
        push   esi
        db  0E8h, 097h, 092h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_B4A5
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B4A5:
        call   sub_5_AF00
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_B4C0
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_5_B4C0:
        mov    edx, dword ptr [esi + 20h]
        mov    ebx, dword ptr [edx + 18h]
        mov    dword ptr [edi + 2ch], ebx
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [edi + 28h], esi
        mov    byte ptr [edi + 14h], 0
        mov    dword ptr [edi + 18h], ecx
        mov    edx, dword ptr [esi + 24h]
        mov    eax, edx
        and    eax, 1ffh
        sub    edx, eax
        mov    dword ptr [edi + 48h], edx
        mov    dword ptr [edi + 20h], edx
        mov    edx, dword ptr [esi]
        add    edx, eax
        add    edx, 1ffh
        and    edx, 0fffffe00h
        mov    dword ptr [edi + 4ch], edx
        mov    eax, dword ptr [edi + 20h]
        add    eax, edx
        dec    eax
        mov    dword ptr [edi + 50h], eax
        cmp    dword ptr [ecx + 24h], 0ee6bh
        jae    SHORT loc_5_B570
        test   byte ptr [ebx + 55h], 20h
        jne    SHORT loc_5_B570
        mov    eax, dword ptr [ecx + 48h]
        or     eax, eax
        je     SHORT loc_5_B528
        cmp    byte ptr [ecx + 59h], 1
        jbe    SHORT loc_5_B528
        mov    ebp, dword ptr [eax + 18h]
        jmp    SHORT loc_5_B52E
loc_5_B528:
        xor    ebp, ebp
        mov    bp, word ptr [ecx + 4eh]
loc_5_B52E:
        or     eax, eax
        je     SHORT loc_5_B540
        cmp    byte ptr [ecx + 59h], 1
        jbe    SHORT loc_5_B540
        mov    eax, dword ptr [eax + 18h]
        jmp    SHORT loc_5_B546
        db  08Dh, 049h, 000h
loc_5_B540:
        xor    eax, eax
        mov    ax, word ptr [ecx + 4eh]
loc_5_B546:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [edi + 4ch]
        sub    edx, edx
        div    dword ptr [esp + 10h]
        sub    ebp, edx
        and    ebp, 0fffffe00h
        cmp    ebp, 4000h
        jb     SHORT loc_5_B568
        mov    ebp, 4000h
loc_5_B568:
        mov    eax, dword ptr [edi + 4ch]
        add    eax, ebp
        mov    dword ptr [edi + 4ch], eax
loc_5_B570:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, dword ptr [esi + 20h]
        je     SHORT loc_5_B590
        mov    edx, dword ptr [esi + 20h]
        or     dword ptr [edx + 2ch], 1
        or     eax, eax
        jne    SHORT loc_5_B590
        mov    eax, dword ptr [esi + 20h]
        or     dword ptr [eax + 2ch], 4
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebx + 14h], eax
loc_5_B590:
        mov    word ptr [esi + 34h], 0
        mov    dword ptr [edi + 10h], edi
        mov    dword ptr [edi + 0ch], edi
        mov    eax, dword ptr [ecx + 48h]
        or     eax, eax
        je     SHORT loc_5_B5B0
        cmp    byte ptr [ecx + 59h], 1
        jbe    SHORT loc_5_B5B0
        mov    eax, dword ptr [eax + 18h]
        jmp    SHORT loc_5_B5B6
        db  08Bh, 0FFh
loc_5_B5B0:
        xor    eax, eax
        mov    ax, word ptr [ecx + 4eh]
loc_5_B5B6:
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 59h]
        push   eax
        push   edi
        db  0E8h, 032h, 095h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_B5E0
        push   edi
        db  0E8h, 022h, 04Ch, 0FAh, 0FFh
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B5E0:
        cmp    dword ptr [edi + 0ch], edi
        jne    SHORT loc_5_B5F8
        push   edi
        db  0E8h, 007h, 04Ch, 0FAh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B5F8:
        inc    dword ptr [edi + 1ch]
        sub    ebx, ebx
        test   byte ptr [edi + 24h], 1
        je     SHORT loc_5_B651
        lea    eax, [esi + 34h]
        push   eax
        db  0E8h, 0C8h, 092h, 0FAh, 0FFh
        add    esp, 4
        mov    ebp, dword ptr [esi]
        mov    ebx, dword ptr [esi + 14h]
        push   ebp
        push   ebx
        db  0E8h, 035h, 054h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
        or     eax, eax
        jne    SHORT loc_5_B640
        mov    dword ptr [esi + 14h], ebx
        dec    dword ptr [edi + 1ch]
        mov    ecx, edi
        call   sub_5_AEA0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_5_B640:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 54h], eax
        sub    edx, edx
        mov    ecx, edi
        call   sub_5_BCF0
        jmp    SHORT loc_5_B655
loc_5_B651:
        mov    ebp, dword ptr [esp + 14h]
loc_5_B655:
        test   byte ptr [edi + 24h], 2
        je     SHORT loc_5_B66F
        push   edi
        db  0E8h, 038h, 09Eh, 0FAh, 0FFh
        add    esp, 4
        and    dword ptr [edi + 24h], 0fffffffdh
        mov    ecx, edi
        call   sub_5_B6B0
loc_5_B66F:
        mov    eax, dword ptr [edi + 1ch]
        dec    eax
        mov    dword ptr [edi + 1ch], eax
        jne    SHORT loc_5_B67F
        mov    ecx, edi
        call   sub_5_AEA0
loc_5_B67F:
        lea    eax, [esi + 34h]
        push   eax
        db  0E8h, 0F9h, 091h, 0FAh, 0FFh
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_5_B69F
        push   ebp
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 0F4h, 053h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 14h], ebx
loc_5_B69F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B6B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, dword ptr [esi + 28h]
        or     ecx, ecx
        je     NEAR PTR loc_5_B75E
        mov    eax, dword ptr [esi + 24h]
        test   al, 2
        jne    NEAR PTR loc_5_B75E
        sub    edi, edi
        test   al, 4
        je     SHORT loc_5_B6D3
        jmp    SHORT loc_5_B6F9
loc_5_B6D3:
        mov    edx, esi
        mov    eax, 1
loc_5_B6DA:
        mov    edx, dword ptr [edx + 0ch]
        cmp    edx, esi
        je     SHORT loc_5_B6F9
        test   dword ptr [edx + 18h], eax
        jne    SHORT loc_5_B6DA
        cmp    edx, esi
        je     SHORT loc_5_B6F9
        mov    eax, dword ptr [ecx]
        add    eax, dword ptr [ecx + 24h]
        cmp    dword ptr [edx + 14h], eax
        jae    SHORT loc_5_B6F9
        cmp    dword ptr [edx + 20h], eax
        jb     SHORT loc_5_B6FE
loc_5_B6F9:
        mov    edi, 1
loc_5_B6FE:
        or     edi, edi
        je     SHORT loc_5_B75E
        mov    edi, dword ptr [ecx + 24h]
        mov    edx, dword ptr [esi + 20h]
        sub    edx, edi
        mov    eax, dword ptr [ecx]
        cmp    edx, eax
        jb     SHORT loc_5_B712
        mov    edx, eax
loc_5_B712:
        mov    eax, dword ptr [esi + 2ch]
        mov    ebx, dword ptr [eax + 5ch]
        lea    eax, [edi + edx]
        cmp    eax, ebx
        jbe    SHORT loc_5_B72C
        cmp    ebx, edi
        mov    edx, 0
        jbe    SHORT loc_5_B72C
        mov    edx, ebx
        sub    edx, edi
loc_5_B72C:
        mov    al, byte ptr [esi + 17h]
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 1ah], ax
        or     ax, ax
        jne    SHORT loc_5_B74B
        mov    dword ptr [ecx], edx
        or     edx, edx
        je     SHORT loc_5_B748
        add    dword ptr [ecx + 24h], edx
        jmp    SHORT loc_5_B74B
loc_5_B748:
        mov    dword ptr [ecx + 24h], ebx
loc_5_B74B:
        add    ecx, 34h
        push   ecx
        db  0E8h, 042h, 091h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 28h], 0
loc_5_B75E:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B770:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_5_B7CF
        mov    ebx, dword ptr [edi + 2ch]
        lea    ebp, [edi + 2ch]
        test   bl, 10h
        je     SHORT loc_5_B7CF
        cmp    dword ptr [esi + 40h], 0
        je     SHORT loc_5_B7C2
        lea    ecx, [esi - 10h]
        mov    eax, ecx
loc_5_B796:
        mov    eax, dword ptr [eax + 1ch]
        cmp    ecx, eax
        je     SHORT loc_5_B7BE
        cmp    eax, edi
        je     SHORT loc_5_B796
        test   byte ptr [eax + 2ch], 1
        je     SHORT loc_5_B796
        test   byte ptr [eax + 2ch], 10h
        jne    SHORT loc_5_B796
        and    ebx, 0fffffffbh
        mov    dword ptr [ebp], ebx
        or     dword ptr [eax + 2ch], 4
        mov    dword ptr [esi + 14h], eax
        cmp    ecx, eax
        jne    SHORT loc_5_B7CF
loc_5_B7BE:
        sub    edi, edi
        jmp    SHORT loc_5_B7CF
loc_5_B7C2:
        and    ebx, 0fffffffbh
        mov    dword ptr [ebp], ebx
        mov    dword ptr [esi + 14h], 0
loc_5_B7CF:
        mov    ecx, dword ptr [esi + 18h]
        or     ecx, ecx
        je     SHORT loc_5_B837
        lea    ebp, [ecx + 2ch]
        mov    dword ptr [esp + 14h], ebp
        mov    ebx, dword ptr [ebp]
        test   bl, 10h
        je     SHORT loc_5_B837
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_5_B826
        lea    eax, [esi - 10h]
        mov    dword ptr [esp + 10h], eax
loc_5_B7F2:
        mov    eax, dword ptr [eax + 1ch]
        cmp    dword ptr [esp + 10h], eax
        je     SHORT loc_5_B822
        cmp    eax, ecx
        je     SHORT loc_5_B7F2
        test   byte ptr [eax + 2ch], 2
        je     SHORT loc_5_B7F2
        test   byte ptr [eax + 2ch], 10h
        jne    SHORT loc_5_B7F2
        mov    ebp, dword ptr [esp + 14h]
        and    ebx, 0fffffff7h
        mov    dword ptr [ebp], ebx
        or     dword ptr [eax + 2ch], 8
        mov    dword ptr [esi + 18h], eax
        cmp    dword ptr [esp + 10h], eax
        jne    SHORT loc_5_B837
loc_5_B822:
        sub    ecx, ecx
        jmp    SHORT loc_5_B837
loc_5_B826:
        mov    ebp, dword ptr [esp + 14h]
        and    ebx, 0fffffff7h
        mov    dword ptr [ebp], ebx
        mov    dword ptr [esi + 18h], 0
loc_5_B837:
        or     edi, edi
        je     SHORT loc_5_B850
        test   byte ptr [edi + 2ch], 0ch
        jne    SHORT loc_5_B850
        mov    ecx, edi
        call   sub_5_6730
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B850:
        or     ecx, ecx
        je     SHORT loc_5_B86B
        cmp    ecx, edi
        je     SHORT loc_5_B86B
        test   byte ptr [ecx + 2ch], 0ch
        jne    SHORT loc_5_B86B
        call   sub_5_6730
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B86B:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_B880:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], ecx
loc_5_B88B:
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + 10h]
        mov    edx, dword ptr [edi + 24h]
        lea    esi, [edi + 24h]
        test   dl, 40h
        jne    NEAR PTR loc_5_BB30
        or     edx, 40h
        mov    dword ptr [esi], edx
        lea    eax, [edi + 54h]
        mov    dword ptr [esp + 20h], eax
        mov    ebx, dword ptr [eax]
        mov    ecx, dword ptr [ebx]
        mov    eax, ecx
        or     eax, eax
        je     SHORT loc_5_B8C1
loc_5_B8B7:
        and    dword ptr [eax + 24h], 0ffffffdfh
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_B8B7
loc_5_B8C1:
        and    dword ptr [ebx + 18h], 0ffffffefh
        mov    al, byte ptr [ebx + 2ch]
        mov    byte ptr [esp + 1ch], al
        and    eax, 0ffh
        or     eax, eax
        jne    SHORT loc_5_B8E7
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 44h]
        mov    al, byte ptr [eax + 96h]
        and    eax, 0ffh
loc_5_B8E7:
        mov    ebp, dword ptr [ebx + 4]
        or     eax, eax
        mov    dword ptr [ebp], ebx
        je     SHORT loc_5_B946
        test   dl, 10h
        jne    SHORT loc_5_B946
        cmp    eax, 3fh
        je     SHORT loc_5_B946
        mov    byte ptr [edi + 17h], al
        or     dword ptr [esi], 4
        mov    ecx, edi
        call   sub_5_BDC0
        cmp    byte ptr [edi + 14h], 0
        jne    SHORT loc_5_B91D
        cmp    dword ptr [edi + 1ch], 0
        jne    SHORT loc_5_B91D
        mov    ecx, edi
        call   sub_5_AEA0
        sub    edi, edi
loc_5_B91D:
        or     edi, edi
        je     NEAR PTR loc_5_BB6F
        mov    eax, dword ptr [edi + 24h]
        and    eax, 0ffffffbfh
        mov    dword ptr [edi + 24h], eax
        test   al, 80h
        je     NEAR PTR loc_5_BB6F
        and    eax, 0ffffff7fh
        mov    dword ptr [edi + 24h], eax
        dec    dword ptr [edi + 1ch]
        jmp    NEAR PTR loc_5_B88B
loc_5_B946:
        mov    byte ptr [edi + 15h], 8
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax], edx
        xor    eax, eax
        mov    al, byte ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_5_B970
        cmp    eax, 1
        je     NEAR PTR loc_5_BAE1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_5_B970:
        lea    ecx, [edi + 1ch]
        mov    dword ptr [esp + 14h], ecx
        inc    dword ptr [ecx]
        mov    edx, dword ptr [edi + 28h]
        mov    dword ptr [esp + 18h], edx
        mov    eax, dword ptr [ebx + 48h]
        or     eax, eax
        je     SHORT loc_5_B98D
        push   eax
        db  0E8h, 09Fh, 049h, 0FAh, 0FFh
loc_5_B98D:
        mov    bp, word ptr [ebx + 28h]
        mov    edx, dword ptr [ebx + 1ch]
        mov    eax, ebp
        and    eax, 0ffffh
        cmp    eax, edx
        jae    SHORT loc_5_B9C3
        mov    ecx, dword ptr [edi + 2ch]
        add    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ecx + 5ch]
        cmp    eax, ecx
        jae    SHORT loc_5_B9B9
        sub    ecx, dword ptr [ebx + 14h]
        cmp    ecx, edx
        jb     SHORT loc_5_B9B5
        mov    ecx, edx
loc_5_B9B5:
        mov    word ptr [ebx + 28h], cx
loc_5_B9B9:
        cmp    word ptr [ebx + 28h], bp
        jbe    SHORT loc_5_B9C3
        or     dword ptr [ebx + 18h], 4
loc_5_B9C3:
        xor    eax, eax
        or     dword ptr [ebx + 18h], 1
        mov    ax, word ptr [ebx + 28h]
        cmp    eax, edx
        jb     SHORT loc_5_B9F0
        cmp    dword ptr [ebx + 0ch], edi
        je     SHORT loc_5_B9F0
        test   byte ptr [esi], 10h
        jne    SHORT loc_5_B9F0
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_5_C110
        add    esp, 4
        jmp    SHORT loc_5_BA00
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_B9F0:
        mov    ecx, edi
        call   sub_5_BDC0
        xor    eax, eax
        mov    ax, word ptr [ebx + 28h]
        mov    dword ptr [ebx + 1ch], eax
loc_5_BA00:
        test   byte ptr [ebx + 18h], 4
        je     SHORT loc_5_BA80
        mov    ax, word ptr [ebx + 28h]
        or     ax, ax
        je     SHORT loc_5_BA80
        mov    ecx, dword ptr [ebx + 20h]
        mov    edx, ecx
        and    edx, 1ffh
        xor    ebp, ebp
        mov    bp, word ptr [ebx + 2ah]
        and    eax, 0ffffh
        sub    eax, ebp
        sub    eax, ecx
        add    eax, dword ptr [ebx + 14h]
        add    eax, edx
        and    eax, 0fffffe00h
        push   eax
        sub    ecx, edx
        push   ecx
        push   ebx
        db  0E8h, 09Fh, 09Ah, 0FAh, 0FFh
        add    esp, 0ch
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_BA8F
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ebx + 14h]
        mov    eax, dword ptr [edx + 24h]
        xor    ebp, ebp
        mov    bp, word ptr [ebx + 28h]
        add    ebp, ecx
        mov    edx, dword ptr [edx]
        add    edx, eax
        cmp    ebp, edx
        jb     SHORT loc_5_BA63
        mov    ebp, edx
loc_5_BA63:
        push   ebp
        cmp    ecx, eax
        ja     SHORT loc_5_BA6A
        mov    ecx, eax
loc_5_BA6A:
        mov    edx, dword ptr [esp + 1ch]
        push   ecx
        push   edx
        push   ebx
        db  0E8h, 00Ah, 099h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_5_BA8F
        db  005h, 000h, 000h, 000h, 000h
loc_5_BA80:
        cmp    dword ptr [ebx + 48h], 0
        je     SHORT loc_5_BA8F
        push   ebx
        call   sub_5_BB80
        add    esp, 4
loc_5_BA8F:
        mov    ax, word ptr [ebx + 28h]
        or     ax, ax
        je     SHORT loc_5_BAAE
        mov    ecx, dword ptr [ebx + 14h]
        and    eax, 0ffffh
        add    ecx, eax
        mov    eax, dword ptr [edi + 20h]
        cmp    ecx, eax
        ja     SHORT loc_5_BAAB
        mov    ecx, eax
loc_5_BAAB:
        mov    dword ptr [edi + 20h], ecx
loc_5_BAAE:
        mov    ecx, edi
        call   sub_5_B6B0
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        je     NEAR PTR loc_5_BB42
        mov    eax, dword ptr [esi]
        and    eax, 0ffffffbfh
        mov    dword ptr [esi], eax
        test   al, 80h
        je     NEAR PTR loc_5_BB6F
        and    eax, 0ffffff7fh
        mov    dword ptr [esi], eax
        dec    dword ptr [ecx]
        jmp    NEAR PTR loc_5_B88B
loc_5_BAE1:
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [ebx + 1ch]
        add    dword ptr [eax + 20h], ebp
        cmp    ebx, ecx
        je     SHORT loc_5_BAFA
        nop
loc_5_BAF0:
        or     dword ptr [ecx + 24h], 40h
        mov    ecx, dword ptr [ecx]
        cmp    ebx, ecx
        jne    SHORT loc_5_BAF0
loc_5_BAFA:
        cmp    edi, edx
        je     SHORT loc_5_BB51
        cmp    byte ptr [esp + 1ch], 0
        jne    SHORT loc_5_BB51
        test   byte ptr [esi], 10h
        jne    SHORT loc_5_BB51
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_5_C110
        add    esp, 4
        mov    eax, dword ptr [esi]
        and    eax, 0ffffffbfh
        mov    dword ptr [esi], eax
        test   al, 80h
        je     SHORT loc_5_BB6F
        and    eax, 0ffffff7fh
        mov    dword ptr [esi], eax
        jmp    NEAR PTR loc_5_B88B
        db  08Bh, 0FFh
loc_5_BB30:
        or     dword ptr [edi + 24h], 80h
        inc    dword ptr [edi + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_BB42:
        mov    ecx, edi
        call   sub_5_AEA0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_BB51:
        mov    al, byte ptr [ebx + 2ch]
        or     al, al
        je     SHORT loc_5_BB61
        cmp    byte ptr [edi + 17h], 0
        jne    SHORT loc_5_BB61
        mov    byte ptr [edi + 17h], al
loc_5_BB61:
        mov    ecx, edi
        call   sub_5_BDC0
        mov    ecx, edi
        call   sub_5_AEA0
loc_5_BB6F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_BB80:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cli
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 24h]
        mov    eax, dword ptr [esi + 30h]
        mov    ecx, eax
        and    ecx, 1ffh
        xor    edx, edx
        mov    dx, word ptr [esi + 2eh]
        add    edx, ecx
        and    edx, 0fffffe00h
        push   edx
        sub    eax, ecx
        push   eax
        push   esi
        db  0E8h, 02Dh, 099h, 0FAh, 0FFh
        add    esp, 0ch
        mov    edx, dword ptr [edi + 28h]
        or     edx, edx
        je     SHORT loc_5_BBFF
        mov    ecx, dword ptr [esi + 30h]
        xor    ebx, ebx
        mov    eax, dword ptr [edx + 24h]
        mov    bx, word ptr [esi + 2eh]
        add    ebx, ecx
        mov    ebp, dword ptr [edx]
        add    ebp, eax
        cmp    ebx, ebp
        jb     SHORT loc_5_BBD1
        mov    ebx, ebp
loc_5_BBD1:
        push   ebx
        cmp    eax, ecx
        ja     SHORT loc_5_BBD8
        mov    eax, ecx
loc_5_BBD8:
        push   eax
        push   edx
        push   esi
        db  0E8h, 0A0h, 097h, 0FAh, 0FFh
        add    esp, 10h
        xor    eax, eax
        mov    ax, word ptr [esi + 2eh]
        add    eax, dword ptr [esi + 30h]
        mov    ecx, dword ptr [edi + 20h]
        cmp    eax, ecx
        ja     SHORT loc_5_BBF5
        mov    eax, ecx
loc_5_BBF5:
        mov    dword ptr [edi + 20h], eax
        mov    ecx, edi
        call   sub_5_B6B0
loc_5_BBFF:
        mov    dword ptr [esi + 48h], 0
        sti
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_BC10:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, edx
        mov    edi, edx
        mov    ecx, ecx
        mov    esi, ecx
        cli
        mov    edx, dword ptr [esi + 18h]
        test   dl, 4
        jne    NEAR PTR loc_5_BCDA
        mov    cx, word ptr [esp + 14h]
        mov    eax, ecx
        and    eax, 0ffffh
        add    eax, edi
        cmp    eax, 8
        ja     SHORT loc_5_BC44
        sti
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_5_BC44:
        cmp    edi, 8
        lea    ebx, [esi + 14h]
        jae    SHORT loc_5_BC60
        mov    eax, edi
        mov    ebp, dword ptr [ebx]
        and    bp, 3
        sub    ax, bp
        sub    ax, 8
        add    cx, ax
        jmp    SHORT loc_5_BC6A
loc_5_BC60:
        mov    eax, dword ptr [ebx]
        and    eax, 3
        add    eax, 8
        sub    edi, eax
loc_5_BC6A:
        add    edi, dword ptr [ebx]
        mov    ebx, dword ptr [esi + 20h]
        cmp    edi, ebx
        jne    SHORT loc_5_BCC0
        or     cx, cx
        je     SHORT loc_5_BCDA
        mov    eax, ecx
        and    eax, 0ffffh
        add    eax, ebx
        mov    dword ptr [esi + 20h], eax
        cmp    dword ptr [esi + 48h], 0
        jne    SHORT loc_5_BCB4
        mov    dword ptr [esi + 30h], edi
        mov    word ptr [esi + 2eh], cx
        push   esi
        db  068h, 072h, 004h, 000h, 000h
        db  0E8h, 054h, 046h, 0FAh, 0FFh
        mov    dword ptr [esi + 48h], eax
        or     eax, eax
        jne    SHORT loc_5_BCDA
        push   esi
        call   sub_5_BB80
        add    esp, 4
        sti
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_5_BCB4:
        add    word ptr [esi + 2eh], cx
        sti
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_5_BCC0:
        or     edx, 4
        mov    dword ptr [esi + 18h], edx
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_5_BCDA
        push   eax
        db  0E8h, 059h, 046h, 0FAh, 0FFh
        mov    dword ptr [esi + 48h], 0
loc_5_BCDA:
        sti
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_BCF0:
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_5_BD0C
        call   sub_5_002F
loc_5_BD0C:
        or     edi, edi
        mov    ecx, dword ptr [esi + 18h]
        je     SHORT loc_5_BD15
        jmp    SHORT loc_5_BD18
loc_5_BD15:
        add    ecx, 8
loc_5_BD18:
        mov    eax, dword ptr [esi + 18h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_5_BD28
        inc    dword ptr [eax + 4ch]
loc_5_BD28:
        db  0A1h, 060h, 010h, 000h, 000h
        inc    eax
        db  0A3h, 060h, 010h, 000h, 000h
        cmp    edi, 2
        jne    SHORT loc_5_BD46
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx], esi
        jmp    SHORT loc_5_BD53
loc_5_BD46:
        mov    dword ptr [esi], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
loc_5_BD53:
        mov    eax, dword ptr [esi + 18h]
        test   byte ptr [eax + 58h], 20h
        je     SHORT loc_5_BD75
        mov    ebx, 20h
loc_5_BD61:
        mov    eax, dword ptr [esi + 18h]
        push   eax
        db  0E8h, 07Ah, 04Dh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        test   byte ptr [eax + 58h], bl
        jne    SHORT loc_5_BD61
loc_5_BD75:
        xor    eax, eax
        mov    al, byte ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_5_BD8A
        cmp    eax, 1
        je     SHORT loc_5_BD95
        cmp    eax, 9
        je     SHORT loc_5_BDA0
        jmp    SHORT loc_5_BDA6
loc_5_BD8A:
        mov    eax, dword ptr [esi + 2ch]
        inc    dword ptr [eax + 40h]
        inc    dword ptr [esi + 1ch]
        jmp    SHORT loc_5_BDA6
loc_5_BD95:
        mov    eax, dword ptr [esi + 2ch]
        inc    dword ptr [eax + 44h]
        jmp    SHORT loc_5_BDA6
        db  08Dh, 049h, 000h
loc_5_BDA0:
        mov    eax, dword ptr [esi + 2ch]
        inc    dword ptr [eax + 40h]
loc_5_BDA6:
        cmp    edi, 2
        je     SHORT loc_5_BDB7
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   sub_5_C3C0
        add    esp, 4
loc_5_BDB7:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_BDC0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        mov    esi, dword ptr [ebx + 18h]
        cmp    byte ptr [ebx + 17h], 0
        jne    SHORT loc_5_BDDB
        mov    eax, dword ptr [esi + 44h]
        mov    al, byte ptr [eax + 96h]
        mov    byte ptr [ebx + 17h], al
loc_5_BDDB:
        mov    cl, byte ptr [ebx + 14h]
        cmp    cl, 1
        je     SHORT loc_5_BDEA
        mov    dword ptr [esi + 20h], 0
loc_5_BDEA:
        mov    al, byte ptr [ebx + 17h]
        or     al, al
        je     SHORT loc_5_BE20
        cmp    dword ptr [ebx + 28h], 0
        jne    SHORT loc_5_BE20
        xor    edx, edx
        mov    dl, cl
        db  0F6h, 082h, 041h, 01Ah, 000h, 000h, 001h
        jne    SHORT loc_5_BE20
        test   byte ptr [ebx + 24h], 10h
        jne    SHORT loc_5_BE20
        cmp    al, 3fh
        je     SHORT loc_5_BE20
        or     byte ptr [esi + 58h], 20h
        mov    ecx, ebx
        call   sub_5_C830
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_5_BE20:
        or     cl, cl
        jne    SHORT loc_5_BE27
        dec    dword ptr [ebx + 1ch]
loc_5_BE27:
        dec    dword ptr [esi + 14h]
        db  0A1h, 060h, 010h, 000h, 000h
        dec    eax
        db  0A3h, 060h, 010h, 000h, 000h
        or     dword ptr [ebx + 24h], 4
        mov    edi, dword ptr [ebx + 2ch]
        or     edi, edi
        je     SHORT loc_5_BE43
        dec    dword ptr [edi + 4ch]
loc_5_BE43:
        mov    dword ptr [esi + 10h], 0
        xor    eax, eax
        mov    al, byte ptr [ebx + 14h]
        cmp    eax, 0bh
        ja     NEAR PTR loc_5_C0AD
        db  0FFh, 024h, 085h, 0D4h, 0C0h, 000h, 000h
        db  090h
loc_5_BE60:
        mov    eax, dword ptr [edi + 40h]
        dec    eax
        mov    dword ptr [edi + 40h], eax
        jne    SHORT loc_5_BE7E
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_5_BE7E
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_5_BE7E
        push   edi
        db  0E8h, 06Fh, 04Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_BE7E:
        cmp    byte ptr [ebx + 17h], 0
        je     SHORT loc_5_BE8B
        mov    ecx, ebx
        call   sub_5_B6B0
loc_5_BE8B:
        sub    edx, edx
        mov    ecx, dword ptr [ebx + 2ch]
        call   sub_5_B770
        jmp    NEAR PTR loc_5_C0AD
loc_5_BE9A:
        mov    eax, dword ptr [edi + 44h]
        dec    eax
        mov    dword ptr [edi + 44h], eax
        jne    SHORT loc_5_BEB8
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_5_BEB8
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_5_BEB8
        push   edi
        db  0E8h, 035h, 04Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_BEB8:
        sub    edx, edx
        mov    ecx, dword ptr [ebx + 2ch]
        call   sub_5_B770
        cmp    dword ptr [esi + 20h], 8000h
        jb     NEAR PTR loc_5_C0AD
        mov    eax, dword ptr [ebx + 2ch]
        mov    ecx, dword ptr [eax + 8]
        call   sub_5_C7D0
        jmp    NEAR PTR loc_5_C0AD
        db  090h
loc_5_BEE0:
        mov    ebp, dword ptr [ebx + 48h]
        push   esi
        db  0E8h, 0AFh, 087h, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, ebp
        call   sub_5_AE10
        cmp    dword ptr [ebx + 28h], 0
        jne    SHORT loc_5_BF19
        mov    dword ptr [ebp], 656c6966h
        push   0
        push   ebp
        db  0E8h, 0C9h, 04Ah, 0FAh, 0FFh
        add    esp, 8
        push   ebx
        db  0E8h, 0E1h, 042h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_C0AD
loc_5_BF19:
        or     dword ptr [ebp + 2ch], 20h
        push   ebp
        db  0E8h, 0C7h, 04Bh, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_C0AD
        db  005h, 000h, 000h, 000h, 000h
loc_5_BF30:
        mov    byte ptr [esi + 57h], 0
        mov    eax, dword ptr [ebx + 8]
        db  08Bh, 00Ch, 085h, 0D0h, 009h, 000h, 000h
        mov    edx, dword ptr [ecx + 8]
        add    ecx, 8
        dec    edx
        mov    dword ptr [ecx], edx
        jne    SHORT loc_5_BF52
        push   eax
        db  0E8h, 075h, 049h, 0FAh, 0FFh
        add    esp, 4
loc_5_BF52:
        push   ebx
        db  0E8h, 09Ah, 042h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 2ch]
        dec    eax
        mov    dword ptr [esi + 2ch], eax
        jne    NEAR PTR loc_5_C0AD
        cmp    byte ptr [esi + 37h], 0
        je     NEAR PTR loc_5_C0AD
        push   esi
        db  0E8h, 072h, 04Bh, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_C0AD
loc_5_BF80:
        mov    ax, word ptr [ebx + 4dh]
        mov    word ptr [edi + 6ch], ax
        mov    ax, word ptr [ebx + 4fh]
        mov    word ptr [edi + 6eh], ax
        mov    al, byte ptr [ebx + 48h]
        and    eax, 0ffff00ffh
        mov    word ptr [edi + 68h], ax
        mov    eax, dword ptr [edi + 28h]
        dec    eax
        mov    dword ptr [edi + 28h], eax
        jne    SHORT loc_5_BFB4
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_5_BFB4
        push   edi
        db  0E8h, 039h, 04Bh, 0FAh, 0FFh
        add    esp, 4
loc_5_BFB4:
        cmp    byte ptr [ebx + 17h], 0
        je     NEAR PTR loc_5_C0A6
        mov    ecx, edi
        call   sub_5_A710
        jmp    NEAR PTR loc_5_C0A6
loc_5_BFCA:
        push   esi
        db  0E8h, 0C8h, 086h, 0FAh, 0FFh
        add    esp, 4
        mov    ebp, dword ptr [ebx + 48h]
        inc    word ptr [ebp + 26h]
        mov    edi, dword ptr [ebp + 14h]
        lea    eax, [ebp + 14h]
        cmp    edi, eax
        je     SHORT loc_5_BFF4
loc_5_BFE4:
        mov    ecx, edi
        call   sub_5_A7F0
        mov    edi, dword ptr [edi]
        lea    eax, [ebp + 14h]
        cmp    edi, eax
        jne    SHORT loc_5_BFE4
loc_5_BFF4:
        mov    ax, word ptr [ebp + 26h]
        dec    ax
        mov    word ptr [ebp + 26h], ax
        jne    SHORT loc_5_C012
        test   byte ptr [ebp + 24h], 1
        je     SHORT loc_5_C012
        lea    eax, [ebp + 14h]
        push   eax
        db  0E8h, 089h, 095h, 0FAh, 0FFh
        add    esp, 4
loc_5_C012:
        sub    edi, edi
        mov    dword ptr [ebp], 736572h
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0ACh, 049h, 0FAh, 0FFh
        add    esp, 8
        push   edi
        push   ebp
        db  0E8h, 0A2h, 049h, 0FAh, 0FFh
        add    esp, 8
        push   ebx
        db  0E8h, 0BAh, 041h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, word ptr [esi + 92h]
        dec    ax
        mov    word ptr [esi + 92h], ax
        jne    SHORT loc_5_C0AD
        cmp    byte ptr [esi + 37h], 0
        je     SHORT loc_5_C0AD
        push   esi
        db  0E8h, 091h, 04Ah, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_C0AD
        db  08Bh, 0FFh
loc_5_C060:
        mov    eax, dword ptr [edi + 40h]
        dec    eax
        mov    dword ptr [edi + 40h], eax
        jne    SHORT loc_5_C07E
        cmp    dword ptr [edi + 4ch], 0
        je     SHORT loc_5_C07E
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_5_C07E
        push   edi
        db  0E8h, 06Fh, 04Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_C07E:
        mov    ax, word ptr [ebx + 4dh]
        mov    word ptr [edi + 6ch], ax
        mov    ax, word ptr [ebx + 4fh]
        mov    word ptr [edi + 6eh], ax
        sub    edx, edx
        mov    ecx, dword ptr [ebx + 2ch]
        call   sub_5_B770
        jmp    SHORT loc_5_C0A6
loc_5_C09A:
        mov    eax, dword ptr [ebx + 49h]
        push   eax
        db  0E8h, 03Bh, 04Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_C0A6:
        mov    ecx, ebx
        call   sub_5_AEA0
loc_5_C0AD:
        or     edi, edi
        je     SHORT loc_5_C0C6
        cmp    dword ptr [edi + 4ch], 0
        jne    SHORT loc_5_C0C6
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_5_C0C6
        push   edi
        db  0E8h, 027h, 04Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_C0C6:
        push   esi
        call   sub_5_C3C0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  060h, 0BEh, 000h, 000h, 09Ah, 0BEh, 000h, 000h, 0E0h, 0BEh, 000h, 000h, 030h, 0BFh, 000h, 000h
        db  080h, 0BFh, 000h, 000h, 0ADh, 0C0h, 000h, 000h, 0ADh, 0C0h, 000h, 000h, 0ADh, 0C0h, 000h, 000h
        db  0CAh, 0BFh, 000h, 000h, 060h, 0C0h, 000h, 000h, 0A6h, 0C0h, 000h, 000h, 09Ah, 0C0h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_C110:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ebx + 10h]
        mov    edi, dword ptr [esi + 54h]
        cmp    edi, esi
        je     SHORT loc_5_C19E
        sub    eax, eax
loc_5_C127:
        mov    dword ptr [esi + 54h], edi
        mov    edx, dword ptr [edi + 18h]
        test   dl, 1
        jne    SHORT loc_5_C197
        mov    ecx, dword ptr [esi + 28h]
        cmp    ecx, eax
        je     SHORT loc_5_C144
        mov    ebp, dword ptr [ecx + 8]
        mov    dword ptr [edi + 8], ebp
        mov    ecx, dword ptr [ecx + 20h]
        jmp    SHORT loc_5_C146
loc_5_C144:
        mov    ecx, eax
loc_5_C146:
        mov    ebp, dword ptr [edi + 4]
        test   dl, 10h
        mov    dword ptr [ebp], eax
        mov    byte ptr [edi + 2ch], al
        jne    SHORT loc_5_C171
        mov    ebp, dword ptr [edi]
        cmp    ebp, eax
        je     SHORT loc_5_C16B
        lea    ebx, [ebx]
loc_5_C160:
        or     dword ptr [ebp + 24h], 20h
        mov    ebp, dword ptr [ebp]
        cmp    ebp, eax
        jne    SHORT loc_5_C160
loc_5_C16B:
        or     edx, 10h
        mov    dword ptr [edi + 18h], edx
loc_5_C171:
        mov    dword ptr [edi + 34h], eax
        mov    dword ptr [edi + 38h], eax
        mov    dword ptr [edi + 3ch], eax
        mov    dword ptr [edi + 40h], eax
        mov    dword ptr [edi + 44h], eax
        lea    edx, [edi + 34h]
        xor    edx, edx
        mov    dl, byte ptr [esi + 14h]
        cmp    edx, eax
        je     SHORT loc_5_C1B0
        cmp    edx, 1
        je     NEAR PTR loc_5_C2CB
        jmp    SHORT loc_5_C19A
loc_5_C197:
        mov    edi, dword ptr [edi + 0ch]
loc_5_C19A:
        cmp    edi, esi
        jne    SHORT loc_5_C127
loc_5_C19E:
        mov    ecx, esi
        call   sub_5_BDC0
loc_5_C1A5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_5_C1B0:
        or     ecx, ecx
        jne    SHORT loc_5_C1C2
        mov    eax, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [eax + 14h]
        xor    eax, eax
        mov    al, byte ptr [ecx + 10h]
        mov    dword ptr [edi + 8], eax
loc_5_C1C2:
        mov    dword ptr [edi + 48h], 0
        cmp    byte ptr [edi + 2dh], 1
        ja     SHORT loc_5_C232
        mov    edx, dword ptr [edi + 1ch]
        xor    eax, eax
        mov    ax, word ptr [ebx + 4eh]
        cmp    eax, edx
        jb     SHORT loc_5_C232
        mov    eax, dword ptr [edi + 14h]
        mov    ebp, eax
        and    ebp, 0fffh
        add    ebp, edx
        cmp    ebp, 1000h
        ja     SHORT loc_5_C232
        mov    ebp, dword ptr [edi]
        or     dword ptr [edi + 18h], 4
        mov    ebp, dword ptr [ebp + 10h]
        push   ebp
        mov    ebp, dword ptr [edi]
        mov    ebp, dword ptr [ebp + 14h]
        push   ebp
        mov    ebp, eax
        and    ebp, 1
        push   ebp
        lea    ebp, [edi + 28h]
        push   ebp
        push   ebx
        db  068h, 080h, 0B8h, 000h, 000h
        and    edx, 0ffffh
        push   edx
        push   eax
        push   6
        add    ecx, 4
        push   ecx
        push   48h
        db  068h, 078h, 01Ah, 000h, 000h
        push   edi
        push   ebx
        db  0E8h, 0E3h, 04Bh, 0FAh, 0FFh
        add    esp, 38h
        jmp    SHORT loc_5_C276
loc_5_C232:
        or     dword ptr [edi + 18h], 20h
        mov    eax, dword ptr [edi + 14h]
        mov    edx, dword ptr [edi + 1ch]
        mov    ebp, dword ptr [edi]
        push   ebp
        mov    ebp, eax
        and    ebp, 3
        push   ebp
        lea    ebp, [edi + 28h]
        push   ebp
        push   5
        lea    ebp, [edi + 2ch]
        push   ebp
        push   ebx
        db  068h, 080h, 0B8h, 000h, 000h
        push   edx
        add    ecx, 6
        push   eax
        push   edx
        push   eax
        push   4
        push   ecx
        push   1
        push   edi
        db  068h, 010h, 0BCh, 000h, 000h
        db  068h, 084h, 01Ah, 000h, 000h
        push   edi
        push   ebx
        db  0E8h, 09Dh, 04Bh, 0FAh, 0FFh
        add    esp, 4ch
loc_5_C276:
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_5_C1A5
        cmp    eax, 39h
        jne    SHORT loc_5_C2B9
        mov    cl, byte ptr [esi + 15h]
        dec    cl
        mov    byte ptr [esi + 15h], cl
        je     SHORT loc_5_C2B9
        xor    eax, eax
        mov    al, cl
        mov    edx, 0ah
        sub    edx, eax
        push   ebx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        add    edx, edx
        push   edx
        db  068h, 0C0h, 048h, 000h, 000h
        db  0E8h, 057h, 040h, 0FAh, 0FFh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_C2B9:
        mov    byte ptr [edi + 2ch], al
        mov    ecx, ebx
        call   sub_5_B880
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_C2CB:
        or     ecx, ecx
        jne    SHORT loc_5_C2DD
        mov    eax, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [eax + 18h]
        xor    eax, eax
        mov    al, byte ptr [ecx + 10h]
        mov    dword ptr [edi + 8], eax
loc_5_C2DD:
        cmp    byte ptr [edi + 2dh], 1
        ja     SHORT loc_5_C330
        mov    edx, dword ptr [edi + 1ch]
        xor    eax, eax
        mov    ax, word ptr [ebx + 4eh]
        cmp    eax, edx
        jb     SHORT loc_5_C330
        mov    eax, dword ptr [edi + 14h]
        and    eax, 0fffh
        add    eax, edx
        cmp    eax, 1000h
        ja     SHORT loc_5_C330
        mov    eax, dword ptr [edi]
        push   ebx
        db  068h, 080h, 0B8h, 000h, 000h
        mov    ebp, dword ptr [eax + 10h]
        push   ebp
        add    ecx, 4
        mov    eax, dword ptr [eax + 14h]
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   6
        push   ecx
        push   49h
        db  068h, 094h, 01Ah, 000h, 000h
        push   edi
        push   ebx
        db  0E8h, 0E6h, 04Ah, 0FAh, 0FFh
        add    esp, 30h
        jmp    SHORT loc_5_C366
        db  090h
loc_5_C330:
        or     dword ptr [edi + 18h], 20h
        mov    eax, dword ptr [edi + 1ch]
        mov    edx, dword ptr [edi + 14h]
        push   3
        lea    ebp, [edi + 2ch]
        push   ebp
        push   ebx
        db  068h, 080h, 0B8h, 000h, 000h
        mov    ebp, dword ptr [edi]
        add    ecx, 6
        push   ebp
        push   eax
        push   edx
        push   eax
        push   edx
        push   4
        push   ecx
        push   2
        push   0
        db  068h, 0A0h, 01Ah, 000h, 000h
        push   edi
        push   ebx
        db  0E8h, 0ADh, 04Ah, 0FAh, 0FFh
        add    esp, 40h
loc_5_C366:
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_5_C1A5
        cmp    eax, 39h
        jne    SHORT loc_5_C3A9
        mov    cl, byte ptr [esi + 15h]
        dec    cl
        mov    byte ptr [esi + 15h], cl
        je     SHORT loc_5_C3A9
        xor    eax, eax
        mov    al, cl
        mov    edx, 0ah
        sub    edx, eax
        push   ebx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        add    edx, edx
        push   edx
        db  068h, 0C0h, 048h, 000h, 000h
        db  0E8h, 067h, 03Fh, 0FAh, 0FFh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_C3A9:
        mov    byte ptr [edi + 2ch], al
        mov    ecx, ebx
        call   sub_5_B880
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_C3C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    cl, byte ptr [esi + 58h]
        test   cl, 8
        je     SHORT loc_5_C3E1
        and    cl, 0f7h
        mov    byte ptr [esi + 58h], cl
        mov    edi, dword ptr [esi + 10h]
        jmp    NEAR PTR loc_5_C549
loc_5_C3E1:
        sub    edi, edi
        cmp    dword ptr [esi + 10h], edi
        jne    NEAR PTR loc_5_C75E
        lea    eax, [esi + 8]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_5_C450
        mov    eax, dword ptr [esi]
        cmp    esi, eax
        je     SHORT loc_5_C445
        cmp    byte ptr [esi + 36h], 0
        je     SHORT loc_5_C412
        cmp    dword ptr [esi + 18h], edi
        jne    SHORT loc_5_C412
        test   cl, 90h
        jne    SHORT loc_5_C412
        or     cl, 4
        mov    byte ptr [esi + 58h], cl
        jmp    SHORT loc_5_C452
loc_5_C412:
        mov    edi, eax
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_5_C452
        db  0E8h, 0CDh, 045h, 0FAh, 0FFh
        sub    ebx, eax
        or     ebx, ebx
        jle    SHORT loc_5_C452
        cmp    dword ptr [esi + 18h], 0
        jne    SHORT loc_5_C452
        cmp    dword ptr [esi + 1ch], 0
        jne    SHORT loc_5_C441
        push   esi
        push   ebx
        db  068h, 0AFh, 048h, 000h, 000h
        db  0E8h, 0CAh, 03Eh, 0FAh, 0FFh
        mov    dword ptr [esi + 1ch], eax
loc_5_C441:
        sub    edi, edi
        jmp    SHORT loc_5_C452
loc_5_C445:
        and    cl, 0bfh
        mov    byte ptr [esi + 58h], cl
        jmp    SHORT loc_5_C452
        db  08Dh, 049h, 000h
loc_5_C450:
        mov    edi, dword ptr [eax]
loc_5_C452:
        or     edi, edi
        jne    SHORT loc_5_C475
        test   byte ptr [esi + 58h], 1
        je     NEAR PTR loc_5_C75E
        push   esi
        db  0E8h, 02Ah, 049h, 0FAh, 0FFh
        add    esp, 4
        and    byte ptr [esi + 58h], 0feh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C475:
        mov    al, byte ptr [esi + 58h]
        test   al, 1
        jne    SHORT loc_5_C49E
        cmp    byte ptr [esi + 35h], 0
        je     SHORT loc_5_C490
        or     al, 4
        mov    byte ptr [esi + 58h], al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_5_C490:
        or     al, 1
        mov    byte ptr [esi + 58h], al
        push   esi
        db  0E8h, 0B5h, 048h, 0FAh, 0FFh
        add    esp, 4
loc_5_C49E:
        test   byte ptr [edi + 24h], 20h
        je     NEAR PTR loc_5_C537
        mov    ebx, edi
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_5_C4C0
        cmp    byte ptr [esi + 59h], 1
        jbe    SHORT loc_5_C4C0
        mov    eax, dword ptr [eax + 14h]
        jmp    SHORT loc_5_C4C6
        db  08Dh, 064h, 024h, 000h
loc_5_C4C0:
        xor    eax, eax
        mov    ax, word ptr [esi + 4eh]
loc_5_C4C6:
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 59h]
        push   eax
        push   edi
        db  0E8h, 0E1h, 084h, 0FAh, 0FFh
        mov    edi, eax
        add    esp, 0ch
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 54h], eax
        mov    edx, 2
        mov    ecx, edi
        call   sub_5_BCF0
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [eax], eax
        jne    SHORT loc_5_C530
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 18h]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebx + 2ch]
        dec    dword ptr [eax + 4ch]
        mov    eax, dword ptr [ebx + 2ch]
        dec    dword ptr [eax + 44h]
        db  0FFh, 00Dh, 060h, 010h, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 0D1h, 03Ch, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        db  0E8h, 0C8h, 03Ch, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_C537
        db  090h
loc_5_C530:
        mov    dword ptr [ebx + 30h], 0
loc_5_C537:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 10h], edi
loc_5_C549:
        xor    eax, eax
        mov    al, byte ptr [edi + 14h]
        cmp    eax, 0bh
        ja     NEAR PTR loc_5_C719
        db  0FFh, 024h, 085h, 068h, 0C7h, 000h, 000h
        db  08Bh, 0FFh
loc_5_C560:
        push   esi
        call   sub_5_C110
        add    esp, 4
        sub    bl, bl
        jmp    NEAR PTR loc_5_C71D
loc_5_C570:
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        push   6
        mov    eax, dword ptr [edi + 48h]
        add    eax, 4
        push   eax
        push   42h
        db  068h, 004h, 01Bh, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 083h, 048h, 0FAh, 0FFh
        add    esp, 20h
        mov    bl, al
        jmp    NEAR PTR loc_5_C71D
loc_5_C597:
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        push   18h
        db  068h, 0B8h, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 065h, 048h, 0FAh, 0FFh
        add    esp, 18h
        mov    bl, al
        jmp    NEAR PTR loc_5_C71D
loc_5_C5B5:
        mov    ebp, dword ptr [edi + 2ch]
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 78h]
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    bx, word ptr [eax]
        add    ebx, eax
        test   byte ptr [ebp + 54h], 20h
        je     SHORT loc_5_C61C
        movzx  ebp, byte ptr [ecx + 21h]
        lea    eax, [edi + 4dh]
        push   eax
        lea    eax, [edi + 4fh]
        push   eax
        push   0eh
        lea    eax, [edi + 49h]
        push   eax
        push   4
        lea    eax, [edi + 48h]
        push   eax
        push   4
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        xor    eax, eax
        push   0
        push   ebx
        push   edx
        push   4
        push   ecx
        mov    al, byte ptr [ecx + 20h]
        push   eax
        push   8ch
        push   6
        push   ebp
        push   ebp
        push   6
        db  068h, 0DCh, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 0FEh, 047h, 0FAh, 0FFh
        add    esp, 5ch
        mov    bl, al
        jmp    NEAR PTR loc_5_C71D
loc_5_C61C:
        lea    eax, [edi + 4fh]
        push   eax
        lea    eax, [edi + 4dh]
        push   eax
        push   4
        lea    eax, [edi + 49h]
        push   eax
        lea    eax, [edi + 48h]
        push   eax
        push   0eh
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        push   ebx
        push   edx
        push   6
        push   ecx
        push   -1
        push   40h
        db  068h, 0F0h, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 0C5h, 047h, 0FAh, 0FFh
        add    esp, 44h
        mov    bl, al
        jmp    NEAR PTR loc_5_C71D
loc_5_C655:
        mov    eax, dword ptr [edi + 48h]
        cmp    byte ptr [eax + 1dh], 0
        jne    SHORT loc_5_C680
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        push   eax
        push   14h
        push   16h
        db  068h, 0B0h, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 09Bh, 047h, 0FAh, 0FFh
        add    esp, 20h
        mov    bl, al
        jmp    NEAR PTR loc_5_C71D
        db  090h
loc_5_C680:
        sub    bl, bl
        mov    ecx, edi
        call   sub_5_BDC0
        jmp    NEAR PTR loc_5_C71D
        db  08Bh, 0FFh
loc_5_C690:
        mov    ecx, dword ptr [edi + 2ch]
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [edi + 4dh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 4fh]
        push   eax
        push   6
        mov    eax, dword ptr [ecx + 14h]
        add    eax, 4
        push   eax
        push   4bh
        db  068h, 0C0h, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 052h, 047h, 0FAh, 0FFh
        add    esp, 28h
        mov    bl, al
        jmp    SHORT loc_5_C71D
loc_5_C6C5:
        lea    eax, [edi + 50h]
        push   eax
        lea    eax, [edi + 4eh]
        push   eax
        lea    eax, [edi + 4ch]
        push   eax
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        mov    eax, dword ptr [edi + 48h]
        push   eax
        push   15h
        push   16h
        db  068h, 0CCh, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 025h, 047h, 0FAh, 0FFh
        add    esp, 2ch
        mov    bl, al
        jmp    SHORT loc_5_C71D
loc_5_C6F2:
        mov    eax, dword ptr [edi + 49h]
        push   eax
        push   edi
        db  068h, 0C0h, 0BDh, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [edi + 48h]
        push   eax
        push   1
        push   16h
        db  068h, 0D4h, 01Ah, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 0FEh, 046h, 0FAh, 0FFh
        add    esp, 24h
        mov    bl, al
        jmp    SHORT loc_5_C71D
loc_5_C719:
        mov    bl, byte ptr [esp + 13h]
loc_5_C71D:
        or     bl, bl
        je     SHORT loc_5_C75E
        cmp    bl, 39h
        jne    SHORT loc_5_C754
        mov    al, byte ptr [edi + 15h]
        dec    al
        mov    byte ptr [edi + 15h], al
        je     SHORT loc_5_C754
        or     byte ptr [esi + 58h], 8
        push   esi
        mov    ecx, 1f4h
        and    eax, 0ffh
        sub    ecx, eax
        push   ecx
        db  068h, 0AFh, 048h, 000h, 000h
        db  0E8h, 0BCh, 03Bh, 0FAh, 0FFh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C754:
        mov    byte ptr [edi + 17h], bl
        mov    ecx, edi
        call   sub_5_BDC0
loc_5_C75E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 060h, 0C5h, 000h, 000h, 060h, 0C5h, 000h, 000h, 070h, 0C5h, 000h, 000h, 097h, 0C5h
        db  000h, 000h, 0B5h, 0C5h, 000h, 000h, 019h, 0C7h, 000h, 000h, 019h, 0C7h, 000h, 000h, 019h, 0C7h
        db  000h, 000h, 055h, 0C6h, 000h, 000h, 090h, 0C6h, 000h, 000h, 0C5h, 0C6h, 000h, 000h, 0F2h, 0C6h
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 056h, 08Bh, 0F1h, 0E8h, 058h, 0E7h
        db  0FFh, 0FFh, 0C6h, 040h, 014h, 00Ah, 089h, 070h, 048h, 08Bh, 04Eh, 010h, 089h, 048h, 018h, 02Bh
        db  0D2h, 0C7h, 040h, 008h, 000h, 000h, 000h, 000h, 08Bh, 0C8h, 0E8h, 02Bh, 0F5h, 0FFh, 0FFh, 05Eh
        db  0C3h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_C7D0:
        push   esi
        push   edi
        mov    esi, ecx
        push   0ffh
        db  0E8h, 0FAh, 042h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_5_C80F
        call   sub_5_AF00
        mov    byte ptr [eax + 14h], 0bh
        mov    cl, byte ptr [esi + 1eh]
        mov    byte ptr [eax + 48h], cl
        mov    dword ptr [eax + 49h], edi
        sub    edx, edx
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 8], 0
        mov    ecx, eax
        call   sub_5_BCF0
loc_5_C80F:
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_C820:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0E8h, 024h, 08Eh, 0FAh, 0FFh
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_5_C830:
        push   esi
        mov    esi, ecx
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [eax + 58h]
        add    eax, 58h
        test   cl, 80h
        jne    SHORT loc_5_C858
        or     cl, 80h
        mov    byte ptr [eax], cl
        db  068h, 0D0h, 0A8h, 000h, 000h
        mov    edx, dword ptr [esi + 18h]
        mov    ecx, 3
        call   sub_5_2E60
loc_5_C858:
        push   esi
        call   sub_5_C820
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_C870:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 24h]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        mov    edi, dword ptr [esp + 20h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx + 1fh]
        push   eax
        db  0E8h, 0FEh, 041h, 0FAh, 0FFh
        mov    ebp, eax
        mov    esi, dword ptr [esp + 18h]
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 3ch]
        push   eax
        push   ebx
        mov    edi, ebp
        push   ebp
        db  0E8h, 031h, 03Ah, 0FAh, 0FFh
        add    esp, 0ch
        db  0BAh, 00Ch, 01Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ax, word ptr [edx]
        mov    word ptr [edi], ax
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [edi + 2], al
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ebp + ecx - 1]
        mov    al, byte ptr [esi + 14h]
        push   eax
        db  0E8h, 0BAh, 08Ch, 0FAh, 0FFh
        add    esp, 4
        push   eax
        mov    edi, ebx
        mov    eax, dword ptr [esp + 20h]
        push   eax
        push   ebx
        db  0E8h, 0E8h, 039h, 0FAh, 0FFh
        add    esp, 0ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 010h, 01Bh, 000h, 000h
        mov    word ptr [edi - 1], ax
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ebx + ecx - 1]
        movzx  edi, byte ptr [esi + 17h]
        push   edi
        push   0
        db  0E8h, 097h, 08Ch, 0FAh, 0FFh
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   edi
        push   eax
        mov    edi, ebx
        push   ebx
        db  0E8h, 0A3h, 039h, 0FAh, 0FFh
        add    esp, 10h
        db  0BAh, 014h, 01Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    ax, word ptr [edx]
        mov    word ptr [edi - 1], ax
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [edi + 1], al
        dec    edi
        db  0C6h, 005h, 06Ch, 01Ah, 000h, 000h, 001h
        push   ebp
        db  0E8h, 0F7h, 08Ch, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        db  0C6h, 005h, 06Ch, 01Ah, 000h, 000h, 000h
        push   ebp
        db  0E8h, 01Fh, 041h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        and    byte ptr [eax + 58h], 0dfh
        mov    eax, dword ptr [esi + 18h]
        push   eax
        db  0E8h, 018h, 041h, 0FAh, 0FFh
        add    esp, 4
        cmp    edi, 1
        je     SHORT loc_5_C9F0
        cmp    edi, 2
        je     SHORT loc_5_C9E9
        cmp    edi, 4
        je     SHORT loc_5_C9F0
        cmp    edi, 5
        jne    SHORT loc_5_CA41
loc_5_C9E9:
        mov    eax, dword ptr [esi + 18h]
        or     byte ptr [eax + 58h], 40h
loc_5_C9F0:
        mov    eax, dword ptr [esi + 18h]
        or     byte ptr [eax + 58h], 8
        mov    al, byte ptr [esi + 14h]
        or     al, al
        je     SHORT loc_5_CA02
        cmp    al, 1
        jne    SHORT loc_5_CA2D
loc_5_CA02:
        mov    eax, dword ptr [esi + 54h]
        test   byte ptr [eax + 18h], 20h
        je     SHORT loc_5_CA2D
        mov    ecx, dword ptr [esi + 18h]
        mov    ecx, dword ptr [ecx + 48h]
        mov    byte ptr [eax + 2ch], 0
        mov    ax, word ptr [ecx + 4eh]
        mov    word ptr [ecx + 48h], ax
        mov    ax, word ptr [ecx + 50h]
        mov    word ptr [ecx + 4ah], ax
        mov    ax, word ptr [ecx + 52h]
        mov    word ptr [ecx + 4ch], ax
loc_5_CA2D:
        mov    byte ptr [esi + 17h], 0
        mov    byte ptr [esi + 15h], 8
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   sub_5_C3C0
        add    esp, 4
loc_5_CA41:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_CA50:
        sub    esp, 98h
        push   0
        lea    eax, [esp + 4]
        push   96h
        push   eax
        push   edx
        db  0E8h, 0A0h, 035h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_CA78
        db  0A1h, 018h, 01Bh, 000h, 000h
        mov    dword ptr [esp], eax
loc_5_CA78:
        add    esp, 98h
        ret
        db  090h
loc_5_CA80:
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 64h], 0ch
        jne    NEAR PTR loc_5_CB76
        db  0B9h, 01Ch, 01Bh, 000h, 000h
        mov    esi, dword ptr [esp + 6ch]
        mov    edx, esi
        call   sub_5_CA50
        push   0
        lea    eax, [esp + 14h]
        push   esi
        push   eax
        db  0E8h, 051h, 035h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_CB76
        db  0B9h, 024h, 01Bh, 000h, 000h
        mov    edx, dword ptr [esp + 10h]
        call   sub_5_CA50
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        db  0E8h, 027h, 035h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_CAEE
        db  0B9h, 02Ch, 01Bh, 000h, 000h
        mov    edx, dword ptr [esp + 14h]
        call   sub_5_CA50
loc_5_CAEE:
        sub    ebp, ebp
        db  066h, 039h, 02Dh, 014h, 004h, 000h, 000h
        je     SHORT loc_5_CB76
        sub    edi, edi
        add    eax, 0
loc_5_CB00:
        db  08Bh, 035h, 0A0h, 003h, 000h, 000h
        add    esi, edi
        cmp    byte ptr [esi + 34h], 0
        je     SHORT loc_5_CB63
        mov    eax, dword ptr [esi + 78h]
        push   eax
        push   1
        db  0E8h, 094h, 041h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_CB63
        cmp    dword ptr [esp + 10h], eax
        jne    SHORT loc_5_CB63
        db  08Ah, 01Dh, 05Eh, 004h, 000h, 000h
        db  0B9h, 038h, 01Bh, 000h, 000h
        mov    edx, eax
        call   sub_5_CA50
        db  0A0h, 05Fh, 004h, 000h, 000h
        db  0A2h, 05Eh, 004h, 000h, 000h
        mov    byte ptr [esp + 58h], 0
        mov    byte ptr [esp + 59h], 0ffh
        mov    byte ptr [esi + 54h], al
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_5_16D0
        add    esp, 8
        db  088h, 01Dh, 05Eh, 004h, 000h, 000h
loc_5_CB63:
        add    edi, 0a0h
        inc    ebp
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, ebp
        ja     SHORT loc_5_CB00
loc_5_CB76:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 50h
        ret
sub_5_CB80:
        push   0
        push   0
        mov    eax, dword ptr [esp + 0ch]
        db  068h, 080h, 0CAh, 000h, 000h
        push   eax
        db  0E8h, 07Dh, 034h, 0FAh, 0FFh
        add    esp, 10h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_CBA0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebx], 0ffh
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        cmp    eax, 6
        ja     SHORT loc_5_CBC3
        db  0FFh, 024h, 085h, 0B0h, 0CCh, 000h, 000h
loc_5_CBC3:
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_5_CBD0:
        lea    edx, [ebx + 5]
        sub    ebp, ebp
        mov    byte ptr [edx], 0
        inc    edx
        mov    esi, 3
        mov    edi, edi
loc_5_CBE0:
        db  08Dh, 0BDh, 070h, 004h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebp, ecx
        dec    esi
        jne    SHORT loc_5_CBE0
        db  0BEh, 070h, 004h, 000h, 000h
        mov    edi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    ebp, 6
        jmp    NEAR PTR loc_5_CC9D
loc_5_CC13:
        lea    eax, [ebx + 5]
        mov    ebp, 11h
        push   eax
        db  0E8h, 077h, 03Fh, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_CC9D
loc_5_CC26:
        lea    edi, [ebx + 5]
        mov    ebp, 2bh
        db  0BEh, 0A0h, 020h, 000h, 000h
        mov    ecx, 9
        rep movsd
        movsw
        jmp    SHORT loc_5_CC9D
        db  08Bh, 0FFh
loc_5_CC40:
        lea    edi, [ebx + 5]
        mov    ebp, 105h
        db  08Bh, 035h, 0A4h, 003h, 000h, 000h
        mov    ecx, 40h
        rep movsd
        jmp    SHORT loc_5_CC9D
loc_5_CC57:
        lea    edi, [ebx + 5]
        mov    ebp, 185h
        db  08Bh, 035h, 0A8h, 003h, 000h, 000h
        mov    ecx, 60h
        rep movsd
        jmp    SHORT loc_5_CC9D
        db  08Bh, 0FFh
loc_5_CC70:
        mov    ebp, 6
        db  0A0h, 0ECh, 003h, 000h, 000h
        mov    byte ptr [ebx + 5], al
        jmp    SHORT loc_5_CC9D
        db  090h
loc_5_CC80:
        lea    eax, [ebx + 5]
        db  08Ah, 00Dh, 0C0h, 018h, 000h, 000h
        mov    byte ptr [eax + 1], cl
        db  08Ah, 00Dh, 0C4h, 018h, 000h, 000h
loc_5_CC92:
        mov    byte ptr [eax], cl
        mov    ebp, 8
        mov    byte ptr [eax + 2], 0
loc_5_CC9D:
        mov    byte ptr [ebx], 0
        mov    dword ptr [ebx + 1], 0
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_5_CCB0:
        ror    bl, 1
        add    byte ptr [eax], al
        adc    ecx, esp
        add    byte ptr [eax], al
        int3
        add    byte ptr [eax], al
        inc    eax
        int3
        add    byte ptr [eax], al
        push   edi
        int3
        add    byte ptr [eax], al
        jo     SHORT loc_5_CC92
        add    byte ptr [eax], al
        or     ah, 0
        db  000h, 08Dh, 064h, 024h, 000h
sub_5_CCD0:
        db  083h, 03Dh, 054h, 01Bh, 000h, 000h, 000h
        je     SHORT loc_5_CCE9
        db  0C7h, 005h, 03Ch, 01Bh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
loc_5_CCE9:
        push   0
        db  0E8h, 04Ah, 03Dh, 0FAh, 0FFh
        add    esp, 4
        db  0A3h, 03Ch, 01Bh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_CD02
        mov    eax, 1
        ret
loc_5_CD02:
        db  0A3h, 03Ch, 01Bh, 000h, 000h
        xor    ecx, ecx
        mov    cx, word ptr [eax - 0dh]
        shl    ecx, 4
        db  089h, 00Dh, 050h, 01Bh, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_5_CD30
        db  0C7h, 005h, 03Ch, 01Bh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_CD30:
        db  0A3h, 03Ch, 01Bh, 000h, 000h
        db  089h, 00Dh, 050h, 01Bh, 000h, 000h
        add    eax, ecx
        db  0A3h, 040h, 01Bh, 000h, 000h
        db  0E8h, 03Fh, 03Ch, 0FAh, 0FFh
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        lea    eax, [eax + ecx + 73h]
        db  0A3h, 044h, 01Bh, 000h, 000h
        sub    eax, eax
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_CD60:
        db  0B9h, 058h, 01Bh, 000h, 000h
        call   sub_5_CD80
        db  0A3h, 048h, 01Bh, 000h, 000h
        db  0B9h, 060h, 01Bh, 000h, 000h
        call   sub_5_CD80
        db  0A3h, 04Ch, 01Bh, 000h, 000h
        ret
        db  090h
sub_5_CD80:
        push   esi
        push   edi
        call   sub_5_D270
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_CD92
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_5_CD92:
        push   1
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 017h, 03Ch, 0FAh, 0FFh
        add    esp, 8
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_5_CDB5
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_5_CDB5:
        mov    edi, esi
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
        mov    eax, edx
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_CDE0:
        push   esi
        mov    esi, ecx
        call   sub_5_CE00
        mov    ecx, esi
        call   sub_5_CE40
        mov    ecx, esi
        call   sub_5_CFD0
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_CE00:
        db  08Bh, 015h, 048h, 01Bh, 000h, 000h
        or     edx, edx
        je     SHORT loc_5_CE30
        cmp    byte ptr [edx + 1], 3ah
        jne    SHORT loc_5_CE1B
        xor    eax, eax
        mov    al, byte ptr [edx]
        sub    eax, ecx
        cmp    eax, 41h
        je     SHORT loc_5_CE26
loc_5_CE1B:
        xor    eax, eax
        mov    al, byte ptr [edx]
        sub    eax, ecx
        cmp    eax, 61h
        jne    SHORT loc_5_CE30
loc_5_CE26:
        db  0B9h, 058h, 01Bh, 000h, 000h
        call   sub_5_D020
loc_5_CE30:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_CE40:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        mov    dword ptr [esp + 20h], 0
        db  0B8h, 068h, 01Bh, 000h, 000h
        lea    ecx, [esp + 10h]
        mov    dx, word ptr [eax]
        mov    word ptr [ecx], dx
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ecx + 2], al
        db  0B9h, 060h, 01Bh, 000h, 000h
        call   sub_5_CD80
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        je     NEAR PTR loc_5_CFBB
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 02Bh, 03Bh, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        add    bl, 41h
        mov    byte ptr [esp + 10h], bl
        mov    eax, dword ptr [esp + 1ch]
loc_5_CEA3:
        mov    dword ptr [esp + 14h], eax
loc_5_CEA7:
        lea    edx, [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        call   sub_5_D210
        mov    ebx, eax
        or     ebx, ebx
        db  0A1h, 04Ch, 01Bh, 000h, 000h
        je     NEAR PTR loc_5_CF88
        mov    dword ptr [esp + 14h], ebx
        mov    dword ptr [esp + 18h], 1
        or     ebp, ebp
        je     SHORT loc_5_CF23
        or     eax, eax
        je     SHORT loc_5_CF23
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   3bh
        push   ebp
        db  0E8h, 00Bh, 039h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_CF0A
        mov    byte ptr [eax], 0
loc_5_CF0A:
        mov    edx, ebp
        db  08Bh, 00Dh, 04Ch, 01Bh, 000h, 000h
        call   sub_5_D210
        or     eax, eax
        je     SHORT loc_5_CF23
        mov    dword ptr [esp + 18h], 0
loc_5_CF23:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_CF80
        push   3bh
        push   ebx
        db  0E8h, 0D9h, 038h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_CF55
        cmp    ebx, dword ptr [esp + 1ch]
        jbe    SHORT loc_5_CF49
        lea    eax, [ebx - 1]
        cmp    byte ptr [eax], 3bh
        jne    SHORT loc_5_CF49
        mov    ebx, eax
loc_5_CF49:
        mov    byte ptr [ebx], 0
        inc    dword ptr [esp + 20h]
        jmp    NEAR PTR loc_5_CEA7
loc_5_CF55:
        lea    edi, [eax + 1]
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
        inc    dword ptr [esp + 20h]
        jmp    NEAR PTR loc_5_CEA7
loc_5_CF80:
        lea    eax, [ebx + 1]
        jmp    NEAR PTR loc_5_CEA3
loc_5_CF88:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_5_CF9D
        db  0B9h, 060h, 01Bh, 000h, 000h
        mov    edx, dword ptr [esp + 1ch]
        call   sub_5_D020
loc_5_CF9D:
        mov    eax, dword ptr [esp + 1ch]
        push   0
        push   eax
        db  0E8h, 028h, 03Ah, 0FAh, 0FFh
        add    esp, 8
        or     ebp, ebp
        je     SHORT loc_5_CFBB
        push   0
        push   ebp
        db  0E8h, 019h, 03Ah, 0FAh, 0FFh
        add    esp, 8
loc_5_CFBB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_CFD0:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        db  0A1h, 044h, 01Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_D013
        sub    edi, edi
        mov    ebx, 0ffh
loc_5_CFE5:
        lea    ecx, [eax + edi]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        cmp    edx, esi
        jne    SHORT loc_5_D00D
        mov    eax, 0fh
        sub    eax, edi
        push   eax
        lea    eax, [ecx + 1]
        push   eax
        push   ecx
        db  0E8h, 023h, 038h, 0FAh, 0FFh
        db  0A1h, 044h, 01Bh, 000h, 000h
        add    esp, 0ch
        mov    byte ptr [eax + 0fh], bl
loc_5_D00D:
        inc    edi
        cmp    edi, 10h
        jb     SHORT loc_5_CFE5
loc_5_D013:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_D020:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_D040
        cmp    byte ptr [edi], 0
        je     SHORT loc_5_D039
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_D0E0
loc_5_D039:
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_D040:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        db  08Bh, 035h, 03Ch, 01Bh, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_5_D0D9
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    byte ptr [esi], 0
        lea    ebp, [ecx - 1]
        je     SHORT loc_5_D0D9
loc_5_D069:
        push   ebp
        push   ebx
        push   esi
        db  0E8h, 0C8h, 037h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_D07E
        cmp    byte ptr [esi + ebp], 3dh
        je     SHORT loc_5_D097
loc_5_D07E:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_D069
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_D097:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [esi + ecx]
        db  08Bh, 00Dh, 040h, 01Bh, 000h, 000h
        sub    ecx, eax
        push   ecx
        push   eax
        push   esi
        db  0E8h, 06Eh, 037h, 0FAh, 0FFh
        add    esp, 0ch
        call   sub_5_D1E0
        mov    edi, eax
        db  08Bh, 015h, 040h, 01Bh, 000h, 000h
        sub    edx, edi
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_5_D0D9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_5_D0E0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    edi, ecx
        mov    dword ptr [esp + 14h], ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        call   sub_5_D1E0
        mov    edx, eax
        or     edx, edx
        je     NEAR PTR loc_5_D1CD
        db  08Bh, 02Dh, 040h, 01Bh, 000h, 000h
        sub    ebp, edx
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ebx + ecx + 1]
        cmp    ebp, esi
        jae    SHORT loc_5_D154
        add    ebx, 3
        cmp    ebx, ebp
        ja     NEAR PTR loc_5_D1CD
        sub    esi, ebp
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        sub    ecx, esi
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [ecx + eax], 0
loc_5_D154:
        dec    edx
        mov    edi, dword ptr [esp + 14h]
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
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 06Ch, 01Bh, 000h, 000h
        mov    word ptr [edi - 1], ax
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    byte ptr [edx + ecx], 0
loc_5_D1CD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_D1E0:
        push   edi
        db  08Bh, 015h, 03Ch, 01Bh, 000h, 000h
        or     edx, edx
        jne    SHORT loc_5_D1F0
        sub    eax, eax
        pop    edi
        ret
        db  090h
loc_5_D1F0:
        cmp    byte ptr [edx], 0
        je     SHORT loc_5_D209
loc_5_D1F5:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_5_D1F5
loc_5_D209:
        lea    eax, [edx + 1]
        pop    edi
        ret
        db  08Bh, 0FFh
sub_5_D210:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, edx
        mov    ebp, ecx
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    byte ptr [ebp], 0
        lea    edi, [ecx - 1]
        je     SHORT loc_5_D25B
        mov    edi, edi
loc_5_D230:
        mov    cl, byte ptr [ebp]
        call   sub_5_D2E0
        mov    bl, al
        mov    cl, byte ptr [esi]
        call   sub_5_D2E0
        cmp    bl, al
        jne    SHORT loc_5_D254
        push   edi
        push   esi
        push   ebp
        db  0E8h, 0ECh, 035h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_D262
loc_5_D254:
        inc    ebp
        cmp    byte ptr [ebp], 0
        jne    SHORT loc_5_D230
loc_5_D25B:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_D262:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_D270:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, ecx
        db  08Bh, 02Dh, 03Ch, 01Bh, 000h, 000h
        or     ebp, ebp
        jne    SHORT loc_5_D287
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_D287:
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    byte ptr [ebp], 0
        lea    esi, [ecx - 1]
        je     SHORT loc_5_D2C8
loc_5_D29D:
        push   esi
        push   ebx
        push   ebp
        db  0E8h, 094h, 035h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_D2B3
        cmp    byte ptr [ebp + esi], 3dh
        je     SHORT loc_5_D2D0
loc_5_D2B3:
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebp, ecx
        cmp    byte ptr [ebp], 0
        jne    SHORT loc_5_D29D
loc_5_D2C8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_D2D0:
        lea    ebp, [esi + ebp + 1]
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_D2E0:
        mov    al, cl
        cmp    al, 61h
        jb     SHORT loc_5_D2EC
        cmp    al, 7ah
        ja     SHORT loc_5_D2EC
        sub    al, 20h
loc_5_D2EC:
        ret
        db  08Dh, 049h, 000h
_PTEXT ends
        end
