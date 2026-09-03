; compaq_pcode_04.asm -- COMPAQ object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  050h, 052h, 066h, 0BAh, 0CEh, 003h, 0ECh, 0C1h, 0C8h, 008h, 066h, 0B8h, 00Fh, 005h, 066h, 0EFh
        db  0B0h, 010h, 0EEh, 042h, 0ECh, 0A8h, 008h, 075h, 006h, 00Ch, 008h, 0EEh, 0EBh, 000h, 0ECh, 04Ah
        db  0C1h, 0C0h, 008h, 0EEh, 05Ah, 058h, 0C3h
loc_4_0027:
        pushad
        db  0E8h, 07Dh, 000h, 0FCh, 0FFh
        db  080h, 00Dh, 048h, 002h, 000h, 000h, 002h
        popad
        ret
loc_4_0036:
        db  0E8h, 042h, 000h, 0FCh, 0FFh
        db  080h, 025h, 048h, 002h, 000h, 000h, 0FDh
        ret
loc_4_0043:
        db  0E8h, 062h, 000h, 0FCh, 0FFh
        db  080h, 00Dh, 048h, 002h, 000h, 000h, 001h
        ret
loc_4_0050:
        db  0E8h, 028h, 000h, 0FCh, 0FFh
        db  080h, 025h, 048h, 002h, 000h, 000h, 0FEh
        ret
loc_4_005D:
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        call   0
        mov    edx, 63cah
        in     al, dx
        and    al, 1fh
        mov    byte ptr [edi + 5], al
        mov    edx, 83c6h
        in     al, dx
        mov    byte ptr [edi + 6], al
        mov    edx, 13c8h
        in     ax, dx
        mov    word ptr [edi + 2], ax
        pop    edi
        pop    edx
        ret
loc_4_008E:
        db  080h, 03Dh, 049h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_00AF
        call   0
        push   edx
        db  003h, 00Dh, 044h, 002h, 000h, 000h
        mov    edx, 63cah
        mov    al, byte ptr [ecx + 5]
        and    al, 1fh
        out    dx, al
        pop    edx
loc_4_00AF:
        ret
loc_4_00B0:
        db  08Bh, 00Dh, 040h, 002h, 000h, 000h
        stc
        ret
loc_4_00B8:
        db  03Bh, 01Dh, 024h, 002h, 000h, 000h
        je     SHORT loc_4_00CF
        push   edi
        push   esi
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        mov    word ptr [edi], ax
        pop    esi
        pop    edi
loc_4_00CF:
        ret
loc_4_00D0:
        sbb    byte ptr [esi], al
        add    byte ptr [eax], al
        cld
        push   es
        add    byte ptr [eax], al
        xor    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    cl, byte ptr [edx]
        add    byte ptr [eax], al
        sbb    byte ptr [edx], cl
        add    byte ptr [eax], al
        sub    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        db  07Ah, 00Ah
        add    byte ptr [eax], al
        cld
        or     al, byte ptr [eax]
        db  000h, 021h
        or     eax, dword ptr [eax]
        db  000h, 03Ah
        or     eax, dword ptr [eax]
        db  000h, 0D8h
        or     eax, dword ptr [eax]
        db  000h, 0DFh
        or     eax, dword ptr [eax]
        db  000h, 0E6h
        or     eax, dword ptr [eax]
        db  000h, 0EDh
        or     eax, dword ptr [eax]
        db  000h, 0F4h
        or     eax, dword ptr [eax]
        db  000h, 0FBh
        or     eax, dword ptr [eax]
        db  000h, 002h
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 11010701h
        add    edi, edi
        db  0FFh
loc_4_013C:
        pop    esp
        add    al, byte ptr [eax]
        add    byte ptr [eax - 1ffbfffeh], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 05Ch, 002h, 000h
        add    byte ptr [eax - 1ffbfffeh], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E6h
        add    eax, dword ptr [eax]
        add    byte ptr [eax], ah
        add    eax, dword ptr [eax]
        add    al, 58h
        add    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 06Ch, 004h, 000h
        add    byte ptr [eax], al
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    ecx, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 078h, 005h
        add    byte ptr [eax], al
        add    byte ptr [4000800h], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushad
        add    eax, dword ptr [eax]
        add    byte ptr [eax - 1ff7fffeh], al
        add    dword ptr [eax], edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        out    dx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi + 1], bl
        dec    edi
        add    dl, byte ptr [eax + 3]
        add    dword ptr [ebx + edx*2], 0b069f05h
        pop    es
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0ah], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        ljmp   8013h:0df12ac11h
        adc    al, 0
        adc    eax, 170316e5h
        db  0E3h, 018h
        inc    dword ptr [edx]
        push   ecx
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        jecxz  loc_4_02F0
loc_4_02F0:
        add    byte ptr [eax], al
        add    bl, al
        add    dword ptr [edi - 7afc5efeh], ebx
        add    al, 0a6h
        add    eax, 70b061fh
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0ah], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        ljmp   8013h:0df12ac11h
        adc    al, 40h
        adc    eax, 170416e7h
        db  0E3h, 018h
        inc    dword ptr [edx]
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    eax, dword ptr [ebx]
        add    al, 4
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, 0ef00h
        add    byte ptr [eax], ah
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        pop    edi
        add    dword ptr [edi + 2], ecx
        push   eax
        add    eax, dword ptr [ecx - 60faacfch]
        push   es
        or     eax, dword ptr [edi]
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0ah], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        ljmp   13h:0df12ac11h
        adc    al, 0
        adc    eax, 170316e5h
        db  0E3h, 018h
        inc    dword ptr [edx + 10h]
        push   ecx
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi + 1], bh
        arpl   word ptr [edx], ax
        add    eax, dword ptr fs:[eax + 19056804h]
        push   es
        db  073h, 007h
        lock or byte ptr [eax], al
        or     dword ptr [eax + 0ah], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        pop    eax
        adc    dword ptr [edx + edx + 14801357h], edi
        add    byte ptr [17721658h], dl
        db  0E3h, 018h
        inc    dword ptr [edx]
        push   ecx
        and    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    dword ptr [eax], eax
        jecxz  loc_4_047C
loc_4_047C:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        pushad
        add    byte ptr [eax], al
        wait
        add    dword ptr [edi + 2], edi
        jg     SHORT loc_4_048C
        sahf
        add    al, 87h
loc_4_048C:
        add    eax, 7310617h
        cmc
        or     byte ptr [eax], al
        or     dword ptr [eax + 0ah], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        or     eax, 0ff12b411h
        adc    eax, dword ptr [eax + 14h]
        add    byte ptr [173116ffh], dl
        db  0E3h, 018h
        inc    dword ptr [edx]
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        jecxz  loc_4_0502
loc_4_0502:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        wait
        add    dword ptr [edi + 2], edi
        jg     SHORT loc_4_0512
        sahf
        add    al, 87h
loc_4_0512:
        add    eax, 7310617h
        cmc
        or     byte ptr [eax], al
        or     dword ptr [eax + 0ah], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        or     eax, 0ff12b411h
        adc    eax, dword ptr [eax - 0eaffech]
        push   ss
        xor    dword ptr [edi], edx
        db  0E3h, 018h
        inc    dword ptr [edx]
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        and    eax, dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        in     eax, dx
        add    dword ptr [edi + 10039f02h], ebx
        add    al, 0aeh
        add    eax, 72a0690h
        pop    edx
        or     byte ptr [eax], al
        or     dword ptr [eax + 0ah], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0f000e00h
        add    byte ptr [eax], dl
        add    edx, dword ptr [ecx]
        mov    bh, 12h
        call   dword ptr [ebx]
        add    byte ptr [eax + eax], dl
        adc    eax, 172a16ffh
        db  0E3h, 018h
        inc    dword ptr [edx + 1]
        push   ecx
        enter  0, 1
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 7060605h
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0ch
        or     eax, 0f0e0e0dh
        movups xmm0, xmmword ptr [ecx + 11h]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 7050600h
        invd
        jmp    dword ptr [eax + 3ch]
        db  010h, 077h, 00Ch, 00Fh, 0B6h, 0C0h, 0FFh, 014h, 085h, 0D0h, 000h, 000h, 000h, 0EBh, 006h, 066h
        db  0C7h, 045h, 01Ch, 000h, 001h, 0F9h, 061h, 0C3h
loc_4_0618:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     NEAR PTR loc_4_06F5
        cld
        mov    cl, 1
        mov    edx, dword ptr [eax]
        cmp    dl, 56h
        je     SHORT loc_4_063A
        cmp    dl, 76h
        jne    SHORT loc_4_0658
loc_4_063A:
        cmp    dh, 42h
        je     SHORT loc_4_0644
        cmp    dh, 62h
        jne    SHORT loc_4_0658
loc_4_0644:
        shr    edx, 10h
        cmp    dl, 45h
        je     SHORT loc_4_0651
        cmp    dl, 65h
        jne    SHORT loc_4_0658
loc_4_0651:
        cmp    dh, 32h
        jne    SHORT loc_4_0658
        mov    cl, 2
loc_4_0658:
        mov    dword ptr [eax], 41534556h
        mov    word ptr [eax + 4], 200h
        mov    si, word ptr [ebp + 38h]
        shl    esi, 10h
        lea    edx, [eax + 14h]
        mov    si, 14h
        cmp    cl, 1
        je     SHORT loc_4_0681
        lea    edx, [eax + 100h]
        mov    si, 100h
loc_4_0681:
        mov    dword ptr [eax + 6], esi
        mov    dword ptr [edx], 515043h
        mov    byte ptr [edx + 3], 0
        mov    dword ptr [eax + 0ah], 2
        push   ecx
        mov    si, word ptr [ebp + 38h]
        shl    esi, 10h
        lea    edi, [eax + 18h]
        mov    si, 18h
        cmp    cl, 1
        je     SHORT loc_4_06B0
        lea    edi, [eax + 22h]
        mov    si, 22h
loc_4_06B0:
        mov    dword ptr [eax + 0eh], esi
        db  0BEh, 02Ch, 001h, 000h, 000h
        mov    ecx, 10h
        rep movsb
        pop    ecx
        db  08Bh, 015h, 040h, 002h, 000h, 000h
        shr    edx, 10h
        mov    word ptr [eax + 12h], dx
        cmp    cl, 1
        je     SHORT loc_4_06ED
        mov    word ptr [eax + 14h], 200h
        mov    dword ptr [eax + 16h], 0
        mov    dword ptr [eax + 1ah], 0
        mov    dword ptr [eax + 1eh], 0
loc_4_06ED:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_06FB
loc_4_06F5:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_06FB:
        ret
loc_4_06FC:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     NEAR PTR loc_4_082B
        cld
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 40h
        rep stosd
        mov    ax, word ptr [ebp + 18h]
        and    ax, 3f7fh
        mov    ecx, 7
        db  0BFh, 02Ch, 001h, 000h, 000h
        repne scasw
        movzx  ecx, ax
        pop    eax
        jne    NEAR PTR loc_4_082B
        or     word ptr [eax], 3bh
        cmp    cl, 1
        jbe    SHORT loc_4_0752
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        jne    SHORT loc_4_0752
loc_4_074D:
        and    byte ptr [eax], 0feh
        jmp    SHORT loc_4_0774
loc_4_0752:
        cmp    cl, 4
        jbe    SHORT loc_4_0774
        db  081h, 03Dh, 040h, 002h, 000h, 000h, 000h, 000h, 010h, 000h
        jb     SHORT loc_4_074D
        cmp    cl, 7
        jne    SHORT loc_4_0774
        db  081h, 03Dh, 040h, 002h, 000h, 000h, 000h, 000h, 020h, 000h
        jb     SHORT loc_4_074D
loc_4_0774:
        mov    byte ptr [eax + 2], 7
        mov    byte ptr [eax + 3], 0
        mov    word ptr [eax + 4], 40h
        mov    word ptr [eax + 6], 40h
        mov    word ptr [eax + 8], 0a000h
        mov    word ptr [eax + 0ah], 0
        mov    dword ptr [eax + 0ch], 0
        movzx  esi, cl
        shl    esi, 4
        db  0BFh, 03Ch, 001h, 000h, 000h
        add    edi, esi
        mov    dx, word ptr [edi + 4]
        mov    word ptr [eax + 12h], dx
        mov    dx, word ptr [edi + 6]
        mov    word ptr [eax + 10h], dx
        mov    dx, word ptr [edi + 8]
        mov    word ptr [eax + 14h], dx
        mov    dl, byte ptr [edi + 0ah]
        mov    byte ptr [eax + 19h], dl
        mov    byte ptr [eax + 16h], 0
        mov    byte ptr [eax + 17h], 0
        mov    byte ptr [eax + 18h], 1
        mov    byte ptr [eax + 1bh], 4
        mov    byte ptr [eax + 1ah], 1
        mov    byte ptr [eax + 1ch], 0
        mov    byte ptr [eax + 1dh], 0
        mov    byte ptr [eax + 1eh], 1
        cmp    cl, 11h
        jne    SHORT loc_4_0803
        mov    byte ptr [eax + 1fh], 5
        mov    byte ptr [eax + 20h], 0bh
        mov    byte ptr [eax + 21h], 6
        mov    byte ptr [eax + 22h], 5
        mov    byte ptr [eax + 23h], 5
        mov    byte ptr [eax + 24h], 0
loc_4_0803:
        push   eax
        movzx  edx, word ptr [eax + 12h]
        movzx  eax, word ptr [eax + 14h]
        mul    edx
        mov    edx, eax
        pop    eax
        mov    dword ptr [eax + 2ch], edx
        db  08Bh, 00Dh, 040h, 002h, 000h, 000h
        sub    ecx, edx
        shr    ecx, 0ah
        mov    word ptr [eax + 30h], cx
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0831
loc_4_082B:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0831:
        ret
loc_4_0832:
        cld
        movzx  eax, word ptr [ebp + 10h]
        test   ah, 4
        jne    NEAR PTR loc_4_09FB
        and    ax, 3f7fh
        cmp    ax, 13h
        jbe    NEAR PTR loc_4_09E7
        mov    ecx, 7
        db  0BFh, 02Ch, 001h, 000h, 000h
        repne scasw
        jne    NEAR PTR loc_4_09FB
        cmp    al, 1
        jbe    SHORT loc_4_0872
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        je     NEAR PTR loc_4_09FB
loc_4_0872:
        cmp    al, 4
        jbe    SHORT loc_4_089A
        db  081h, 03Dh, 040h, 002h, 000h, 000h, 000h, 000h, 010h, 000h
        jb     NEAR PTR loc_4_09FB
        cmp    al, 7
        jne    SHORT loc_4_089A
        db  081h, 03Dh, 040h, 002h, 000h, 000h, 000h, 000h, 020h, 000h
        jb     NEAR PTR loc_4_09FB
loc_4_089A:
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        jne    SHORT loc_4_08AA
        db  0BEh, 0E2h, 002h, 000h, 000h
        jmp    SHORT loc_4_08B8
loc_4_08AA:
        sub    eax, 100h
        shl    eax, 4
        db  08Bh, 0B0h, 03Ch, 001h, 000h, 000h
loc_4_08B8:
        mov    edx, 3c4h
        mov    ecx, 6
        rep outsw
        mov    dl, 0ceh
        mov    ax, 50fh
        out    dx, ax
        mov    ax, 4140h
        out    dx, ax
        mov    dl, 0c4h
        mov    ax, 0ff02h
        out    dx, ax
        mov    dl, 0ceh
        mov    ax, 2810h
        out    dx, ax
        mov    dl, 0d4h
        mov    al, 11h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 7fh
        out    dx, al
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        je     SHORT loc_4_08FF
        mov    edx, 63cah
        lodsw
        out    dx, al
loc_4_08FF:
        mov    edx, 3c2h
        lodsw
        out    dx, al
        mov    edx, 83c6h
        lodsw
        out    dx, al
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        je     SHORT loc_4_0928
        mov    edx, 13c9h
        lodsw
        out    dx, al
        mov    edx, 13c8h
        lodsw
        out    dx, al
loc_4_0928:
        mov    edx, 3d4h
        mov    ecx, 19h
        rep outsw
        mov    dl, 0ceh
        outsw
        db  080h, 03Dh, 04Ah, 002h, 000h, 000h, 041h
        je     SHORT loc_4_0944
        outsw
loc_4_0944:
        mov    dl, 0dah
        in     al, dx
        mov    dl, 0c0h
        mov    cl, 14h
loc_4_094B:
        lodsw
        out    dx, al
        mov    al, ah
        out    dx, al
        dec    cl
        jne    SHORT loc_4_094B
        mov    al, 20h
        out    dx, al
        mov    dl, 0ceh
        mov    ecx, 9
        rep outsw
        cmp    byte ptr [ebp + 10h], 7
        jne    SHORT loc_4_0982
        mov    edx, 83c6h
        in     al, dx
        or     al, 80h
        out    dx, al
        mov    edx, 3c8h
        mov    al, 1
        out    dx, al
        mov    edx, 13c6h
        in     al, dx
        or     al, 8
        out    dx, al
loc_4_0982:
        mov    ax, word ptr [ebp + 10h]
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        mov    word ptr [edi], ax
        test   byte ptr [edi + 1], 80h
        jne    SHORT loc_4_09F3
        db  08Bh, 035h, 040h, 002h, 000h, 000h
        mov    edx, 3ceh
        mov    eax, 8460045h
        out    dx, ax
        ror    eax, 10h
        out    dx, ax
        ror    eax, 10h
loc_4_09B1:
        push   eax
        db  08Bh, 03Dh, 028h, 002h, 000h, 000h
        mov    ecx, 4000h
        xor    eax, eax
        rep stosd
        pop    eax
        add    eax, 10001000h
        out    dx, ax
        ror    eax, 10h
        out    dx, ax
        ror    eax, 10h
        sub    esi, 10000h
        jg     SHORT loc_4_09B1
        mov    eax, 84600045h
        out    dx, ax
        ror    eax, 10h
        out    dx, ax
        jmp    SHORT loc_4_09F3
loc_4_09E7:
        movzx  eax, byte ptr [ebp + 10h]
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_4_09F3:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0A01
loc_4_09FB:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0A01:
        ret
loc_4_0A02:
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        mov    ax, word ptr [edi]
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_0A18:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_0A24
        mov    word ptr [ebp + 10h], 0
loc_4_0A24:
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_0A2B:
        mov    edx, 3ceh
        cmp    byte ptr [ebp + 11h], 0
        je     SHORT loc_4_0A50
        xor    ah, ah
        mov    al, 45h
        out    dx, al
        inc    dl
        in     al, dx
        mov    cl, 4
        cmp    byte ptr [ebp], 7
        jne    SHORT loc_4_0A48
        mov    cl, 2
loc_4_0A48:
        shr    al, cl
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_4_0A73
loc_4_0A50:
        mov    ah, byte ptr [ebp + 14h]
        mov    cl, 4
        cmp    byte ptr [ebp], 7
        jne    SHORT loc_4_0A5D
        mov    cl, 2
loc_4_0A5D:
        shl    ah, cl
        mov    al, 45h
        out    dx, ax
        inc    al
        mov    cl, 8
        cmp    byte ptr [ebp], 7
        jne    SHORT loc_4_0A6F
        mov    cl, 2
loc_4_0A6F:
        add    ah, cl
        out    dx, ax
loc_4_0A73:
        mov    word ptr [ebp + 1ch], 4fh
        ret
loc_4_0A7A:
        movzx  ecx, byte ptr [ebp + 10h]
        cmp    cl, 4
        jae    SHORT loc_4_0AE5
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        movzx  edi, word ptr [edi]
        cmp    edi, 111h
        ja     SHORT loc_4_0AE5
        sub    edi, 100h
        jl     SHORT loc_4_0AE5
        shl    edi, 4
        db  00Fh, 0B7h, 0B7h, 040h, 001h, 000h, 000h
        db  00Fh, 0B7h, 0BFh, 042h, 001h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_0AE5
        db  0A1h, 040h, 002h, 000h, 000h
        xor    edx, edx
        div    edi
        movzx  edx, word ptr [ebp + 18h]
        mov    word ptr [ebp + 10h], di
        mov    word ptr [ebp + 18h], si
        mov    word ptr [ebp + 14h], ax
        db  0FFh, 024h, 08Dh, 0ECh, 00Ah, 000h, 000h
loc_4_0AD3:
        cmp    edx, esi
        jne    SHORT loc_4_0AE5
        jmp    SHORT loc_4_0ADD
loc_4_0AD9:
        cmp    edx, edi
        jne    SHORT loc_4_0AE5
loc_4_0ADD:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0AEB
loc_4_0AE5:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0AEB:
        ret
        db  0D3h, 00Ah, 000h, 000h, 0DDh, 00Ah, 000h, 000h, 0D9h, 00Ah, 000h, 000h, 0DDh, 00Ah, 000h, 000h
loc_4_0AFC:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_4_0B1A
        mov    byte ptr [ebp + 11h], 0
        mov    word ptr [ebp + 18h], 0
        mov    word ptr [ebp + 14h], 0
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0B20
loc_4_0B1A:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0B20:
        ret
loc_4_0B21:
        cmp    byte ptr [ebp + 10h], 1
        jne    SHORT loc_4_0B33
        mov    byte ptr [ebp + 11h], 6
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0B39
loc_4_0B33:
        mov    word ptr [ebp + 1ch], 14fh
loc_4_0B39:
        ret
loc_4_0B3A:
        mov    cl, byte ptr [ebp + 10h]
        and    cl, 7fh
        or     cl, cl
        je     SHORT loc_4_0B89
        cmp    cl, 1
        jne    NEAR PTR loc_4_0BD1
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_4_0BC9
        mov    edi, eax
        movzx  ecx, word ptr [ebp + 14h]
        movzx  esi, word ptr [ebp + 18h]
        mov    edx, 3c7h
loc_4_0B6B:
        mov    al, cl
        out    dx, al
        mov    dl, 0c9h
        mov    byte ptr [edi], cl
        in     al, dx
        mov    byte ptr [edi + 1], al
        in     al, dx
        mov    byte ptr [edi + 2], al
        in     al, dx
        mov    byte ptr [edi + 3], al
        mov    dl, 0c7h
        add    edi, 4
        inc    ecx
        dec    esi
        jne    SHORT loc_4_0B6B
        jmp    SHORT loc_4_0BC1
loc_4_0B89:
        mov    ah, 38h
        mov    al, 0
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_4_0BC9
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 14h]
        movzx  edi, word ptr [ebp + 18h]
        mov    edx, 3c8h
loc_4_0BA7:
        mov    al, cl
        out    dx, al
        inc    dl
        mov    al, byte ptr [esi + 1]
        out    dx, al
        mov    al, byte ptr [esi + 2]
        out    dx, al
        mov    al, byte ptr [esi + 3]
        out    dx, al
        dec    dl
        add    esi, 4
        inc    ecx
        dec    edi
        jne    SHORT loc_4_0BA7
loc_4_0BC1:
        mov    word ptr [ebp + 1ch], 4fh
        jmp    SHORT loc_4_0BD7
loc_4_0BC9:
        mov    word ptr [ebp + 1ch], 14fh
        jmp    SHORT loc_4_0BD7
loc_4_0BD1:
        mov    word ptr [ebp + 1ch], 24fh
loc_4_0BD7:
        ret
loc_4_0BD8:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0BDF:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0BE6:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0BED:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0BF4:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0BFB:
        mov    word ptr [ebp + 1ch], 14fh
        ret
loc_4_0C02:
        mov    al, byte ptr [ebp + 10h]
        cmp    al, 0
        je     SHORT loc_4_0C1A
        cmp    al, 1
        je     SHORT loc_4_0C28
        cmp    al, 2
        je     NEAR PTR loc_4_0CEF
        jmp    NEAR PTR loc_4_0D04
loc_4_0C1A:
        mov    eax, 710h
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_4_0D0C
loc_4_0C28:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    cx, 8
        jg     NEAR PTR loc_4_0D04
        call   0
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        mov    edx, 3ceh
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        test   al, 1
        jne    NEAR PTR loc_4_0D04
        mov    al, byte ptr [ebp + 11h]
        cmp    al, 0
        je     SHORT loc_4_0C75
        cmp    al, 1
        je     SHORT loc_4_0C96
        cmp    al, 2
        je     SHORT loc_4_0CB2
        cmp    al, 4
        je     SHORT loc_4_0CCE
        jmp    NEAR PTR loc_4_0D04
loc_4_0C75:
        db  088h, 081h, 014h, 001h, 000h, 000h
        call   sub_4_0D13
        call   sub_4_0D31
        call   sub_4_0D5A
        call   sub_4_0D22
        call   sub_4_0DB7
        jmp    SHORT loc_4_0D0C
loc_4_0C96:
        db  088h, 081h, 014h, 001h, 000h, 000h
        call   sub_4_0D13
        call   sub_4_0D43
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    SHORT loc_4_0D0C
loc_4_0CB2:
        db  088h, 081h, 014h, 001h, 000h, 000h
        call   sub_4_0D13
        call   sub_4_0D78
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    SHORT loc_4_0D0C
loc_4_0CCE:
        db  088h, 081h, 014h, 001h, 000h, 000h
        call   sub_4_0D13
        call   sub_4_0D43
        call   sub_4_0D78
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    SHORT loc_4_0D0C
loc_4_0CEF:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    cx, 8
        jg     SHORT loc_4_0D04
        db  08Ah, 081h, 014h, 001h, 000h, 000h
        mov    byte ptr [ebp + 11h], al
        jmp    SHORT loc_4_0D0C
loc_4_0D04:
        mov    word ptr [ebp + 1ch], 14fh
        jmp    SHORT loc_4_0D12
loc_4_0D0C:
        mov    word ptr [ebp + 1ch], 4fh
loc_4_0D12:
        ret
sub_4_0D13:
        mov    edx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 7fh
        out    dx, al
        ret
sub_4_0D22:
        mov    edx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 80h
        out    dx, al
        ret
sub_4_0D31:
        mov    edx, 3d4h
        mov    al, 4
        out    dx, al
        inc    dx
        db  08Ah, 081h, 01Ch, 001h, 000h, 000h
        out    dx, al
        ret
sub_4_0D43:
        mov    edx, 3d4h
        mov    al, 4
        out    dx, al
        inc    dx
        in     al, dx
        db  088h, 081h, 01Ch, 001h, 000h, 000h
        mov    ax, 0ffh
        out    dx, al
        ret
sub_4_0D5A:
        mov    edx, 3d4h
        mov    al, 10h
        out    dx, al
        inc    dx
        db  08Ah, 081h, 024h, 001h, 000h, 000h
        out    dx, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 7bh
        or     al, 4
        out    dx, al
        ret
sub_4_0D78:
        mov    edx, 3d4h
        mov    al, 10h
        out    dx, al
        inc    dx
        in     al, dx
        db  088h, 081h, 024h, 001h, 000h, 000h
        mov    ax, 0ffh
        out    dx, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 84h
        out    dx, al
        ret
        db  0BAh, 0D4h, 003h, 000h, 000h, 0B0h, 017h, 0EEh, 066h, 042h, 0ECh, 00Ch, 080h, 0EEh, 0C3h, 0BAh
        db  0D4h, 003h, 000h, 000h, 0B0h, 017h, 0EEh, 066h, 042h, 0ECh, 024h, 07Fh, 0EEh, 0C3h
sub_4_0DB7:
        mov    edx, 3dah
        in     al, dx
        mov    edx, 3c0h
        mov    al, 20h
        out    dx, al
        ret
sub_4_0DC6:
        mov    edx, 3dah
        in     al, dx
        mov    edx, 3c0h
        mov    al, 0
        out    dx, al
        ret
_PTEXT ends
        end
