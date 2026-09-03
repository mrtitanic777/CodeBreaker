; biosxlat_pcode_04.asm -- BIOSXLAT object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        movzx  ecx, word ptr [ebp + 18h]
        lea    ecx, [ecx + ecx*2]
        ret
loc_4_0008:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [ebp + 14h], 80h
        je     SHORT loc_4_001D
        or     word ptr [ebp + 2ch], 1
        ret
loc_4_001D:
        cmp    ah, 2
        jb     SHORT loc_4_003C
        je     SHORT loc_4_008D
        cmp    ah, 4
        jb     SHORT loc_4_008D
        je     SHORT loc_4_005D
        cmp    ah, 5
        je     SHORT loc_4_0047
        cmp    ah, 8
        jb     SHORT loc_4_003C
        je     SHORT loc_4_0052
        cmp    ah, 18h
        je     SHORT loc_4_0052
loc_4_003C:
        db  0BAh, 01Fh, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_4_0047:
        db  0BAh, 021h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_4_0052:
        db  0BAh, 02Ah, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_4_005D:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   dword ptr [ebp + 38h]
        push   dword ptr [ebp + 10h]
        mov    word ptr [ebp + 38h], 0f000h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    dword ptr [ebp + 10h]
        pop    dword ptr [ebp + 38h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        ret
loc_4_008D:
        sub    esp, 4
        mov    edi, esp
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 8
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_4_00EF
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp]
        add    eax, ecx
        movzx  eax, byte ptr [eax + 3]
        xor    ecx, ecx
        bts    ecx, eax
        shl    ecx, 7
        mov    word ptr [edi], cx
        movzx  eax, byte ptr [ebp + 18h]
        shl    eax, 2
        shr    al, 2
        mov    byte ptr [edi + 2], al
        mov    al, byte ptr [ebp + 15h]
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_4_0119
loc_4_00EF:
        mov    ecx, dword ptr [78h]
        movzx  eax, cx
        shr    ecx, 10h
        shl    ecx, 4
        add    ecx, eax
        movzx  eax, byte ptr [ecx + 3]
        xor    edx, edx
        bts    edx, eax
        shl    edx, 7
        mov    word ptr [edi], dx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 3], 1
loc_4_0119:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        mov    ax, word ptr [ebp + 38h]
        mov    edx, dword ptr [ebp + 10h]
        call   sub_4_029C
        jae    SHORT loc_4_01BA
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 1ch]
        mul    word ptr [edi]
        mov    ecx, eax
        mov    eax, esi
        dec    ecx
        and    eax, 0fffh
        add    ecx, eax
        shr    ecx, 0ch
        inc    ecx
        mov    eax, esi
        shr    eax, 0ch
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_0174
        add    esp, 0ch
        jmp    SHORT loc_4_01BA
loc_4_0174:
        shl    esi, 0ch
        shr    si, 0ch
        push   dword ptr [ebp + 10h]
        mov    word ptr [ebp + 10h], si
        shr    esi, 10h
        xchg   word ptr [ebp + 5ch], si
        mov    edx, 13h
        db  0E8h, 06Dh, 0FEh, 0FBh, 0FFh
        mov    word ptr [ebp + 5ch], si
        pop    dword ptr [ebp + 10h]
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        test   dword ptr [ebx], 180h
        je     NEAR PTR loc_4_023A
        mov    word ptr [ebp + 1eh], 0
        jmp    NEAR PTR loc_4_023A
loc_4_01BA:
        push   fs
        push   dword ptr [ebp + 5ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        mov    ax, 3810h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        db  0CDh, 020h, 00Ah, 000h, 006h, 000h
        mov    eax, ecx
        xor    edx, edx
        movzx  ecx, word ptr [edi]
        div    ecx
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        movzx  ecx, al
        jecxz  loc_4_0221
loc_4_01EA:
        mov    edx, ecx
        cmp    ecx, dword ptr [esp]
        jbe    SHORT loc_4_01F4
        mov    edx, dword ptr [esp]
loc_4_01F4:
        call   sub_4_023E
        jb     SHORT loc_4_0221
        sub    ecx, edx
        je     SHORT loc_4_0221
        add    byte ptr [ebp + 18h], dl
loc_4_0202:
        mov    dl, byte ptr [edi + 2]
        cmp    byte ptr [ebp + 18h], dl
        jbe    SHORT loc_4_01EA
        sub    byte ptr [ebp + 18h], dl
        inc    byte ptr [ebp + 15h]
        mov    dl, byte ptr [edi + 3]
        cmp    byte ptr [ebp + 15h], dl
        jbe    SHORT loc_4_0202
        mov    byte ptr [ebp + 15h], 0
        inc    byte ptr [ebp + 19h]
        jmp    SHORT loc_4_0202
loc_4_0221:
        add    esp, 4
        movzx  edx, al
        sub    edx, ecx
        mov    byte ptr [ebp + 1ch], dl
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 18h]
        pop    dword ptr [ebp + 10h]
        pop    dword ptr [ebp + 5ch]
        pop    fs
loc_4_023A:
        add    esp, 4
        ret
sub_4_023E:
        push   ecx
        push   edi
        push   eax
        push   edx
        movzx  eax, word ptr [edi]
        mul    edx
        mov    ecx, eax
        pop    edx
        pop    eax
        bt     eax, 8
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        mov    word ptr [ebp + 10h], di
        shr    edi, 10h
        mov    word ptr [ebp + 5ch], di
        mov    byte ptr [ebp + 1dh], ah
        mov    byte ptr [ebp + 1ch], dl
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   eax
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        bt     eax, 8
        cmc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_4_0295
        add    esi, ecx
        clc
loc_4_0295:
        movzx  edx, byte ptr [ebp + 1ch]
        pop    edi
        pop    ecx
        ret
sub_4_029C:
        push   ecx
        push   edx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        test   dword ptr [ebx], 180h
        jne    SHORT loc_4_02B7
        movzx  edx, dx
loc_4_02B7:
        add    eax, edx
        cmp    eax, 100000h
        jb     SHORT loc_4_02D0
        sub    eax, dword ptr [ebx + 4]
        jb     SHORT loc_4_02CC
        cmp    eax, 100000h
        jb     SHORT loc_4_02D0
loc_4_02CC:
        add    eax, dword ptr [ebx + 4]
        clc
loc_4_02D0:
        pop    ecx
        ret
loc_4_02D2:
        push   eax
        mov    eax, dword ptr [78h]
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    ecx, eax
        movzx  ecx, byte ptr [ecx + 4]
        shl    ecx, 2
        pop    eax
        ret
loc_4_02EC:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        cmp    byte ptr [ebp + 1dh], 0c0h
        db  00Fh, 082h, 004h, 0FDh, 0FBh, 0FFh
        je     SHORT loc_4_0313
        cmp    byte ptr [ebp + 1dh], 0c1h
        db  00Fh, 085h, 0F8h, 0FCh, 0FBh, 0FFh
        db  0BAh, 01Bh, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_4_0313:
        db  0BAh, 016h, 000h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
_PTEXT ends
        end
