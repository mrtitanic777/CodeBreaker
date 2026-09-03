; vdef_lcode_01.asm -- VDEF object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cmp    byte ptr [ebp + 4], 4
        je     SHORT loc_1_0014
        cmp    byte ptr [ebp + 4], 0
        jne    SHORT loc_1_0026
loc_1_0014:
        call   sub_1_005B
        mov    word ptr [ebp + 1ah], ax
        db  0C7h, 045h, 010h, 0D4h, 001h, 000h, 000h
        jmp    SHORT loc_1_002C
loc_1_0026:
        xor    eax, eax
        mov    word ptr [ebp + 1ah], ax
loc_1_002C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0031:
        push   ebp
        push   edi
        push   ebx
        mov    ebp, dword ptr [esp + 10h]
        mov    edi, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        mov    word ptr [ebp + 1ah], ax
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_1_005B:
        movzx  edi, word ptr [ebp + 18h]
        mov    ebx, dword ptr [ebp + 28h]
        db  0BEh, 05Ch, 004h, 000h, 000h
loc_1_0067:
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 05Ch, 004h, 000h, 000h
        je     SHORT loc_1_008F
        cmp    bl, byte ptr [esi + 10h]
        jne    SHORT loc_1_0067
        mov    eax, edi
        mov    byte ptr [esi + 11h], al
        mov    ax, word ptr [ebp + 1ah]
        mov    word ptr [esi + 12h], ax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 1ch], eax
        mov    eax, 11ch
        ret
loc_1_008F:
        push   0
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_00EC
        mov    ecx, edi
        mov    byte ptr [eax + 11h], cl
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [edi + 0ch]
        db  089h, 00Dh, 064h, 004h, 000h, 000h
        db  0C7h, 047h, 018h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [eax + 8], edi
        mov    byte ptr [eax + 10h], bl
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [eax + 0ch], ebx
        mov    bx, word ptr [ebp + 1ah]
        mov    word ptr [eax + 12h], bx
        mov    dword ptr [ebp + 1ch], eax
        pushfd
        cli
        db  0BBh, 05Ch, 004h, 000h, 000h
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], eax
        popfd
        sub    eax, eax
        ret
loc_1_00EC:
        mov    eax, 5
        ret
        db  000h, 000h, 01Ch, 001h, 000h, 000h, 015h, 001h, 000h, 000h, 015h, 001h, 000h, 000h, 01Dh, 001h
        db  000h, 000h
loc_1_0104:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        ret
        db  0E8h, 001h, 000h, 000h, 000h, 0C3h
loc_1_0115:
        mov    word ptr [ebx + 2], 0ffffh
        ret
loc_1_011C:
        ret
loc_1_011D:
        ret
        db  000h, 000h
sub_1_0120:
        pushfd
        mov    esi, dword ptr [edi + 8]
        mov    bx, word ptr [esi + 0ah]
        mov    ax, word ptr [esi + 8]
        shl    ax, 3
        add    bx, ax
        mov    eax, dword ptr [esi + 0ch]
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi + 4], bx
        mov    dword ptr [esi + 6], eax
        mov    byte ptr [esi + 0eh], 2
        mov    word ptr [esi], 2
        push   esi
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        mov    ebx, dword ptr [esi + 0ah]
        add    esp, 14h
        add    ebx, eax
        mov    dword ptr [ebx + 20h], eax
        mov    eax, ebx
        add    eax, 58h
        mov    dword ptr [ebx + 48h], eax
        sub    eax, eax
        mov    word ptr [ebx + 4], ax
        mov    dword ptr [ebx + 14h], eax
        mov    dword ptr [ebx + 18h], eax
        mov    byte ptr [ebx + 4ch], al
        mov    dword ptr [ebx + 8], 409h
        mov    eax, dword ptr [edi + 8]
        popfd
        clc
        ret
sub_1_0181:
        pushfd
        sub    ebx, dword ptr [ebx + 20h]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ebx
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        add    esp, 0ch
        popfd
        ret
sub_1_019E:
        push   ebp
        mov    esi, eax
        mov    al, byte ptr [edi + 10h]
        mov    byte ptr [esi + 4dh], al
        mov    edi, dword ptr [edi + 8]
        or     byte ptr [esi + 0ah], 8
        test   byte ptr [edi + 1], 40h
        je     SHORT loc_1_01BE
        and    byte ptr [esi + 0ah], 0f7h
        mov    eax, dword ptr [edi + 20h]
        add    dword ptr [esi + 10h], eax
loc_1_01BE:
        mov    dword ptr [esi + 44h], edi
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [edi + 1ch]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        cld
        pop    ebp
        ret
        db  000h
loc_1_01D4:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        inc    edx
        add    al, byte ptr [eax]
        db  000h, 014h, 002h
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        add    byte ptr [edx], 0
        db  000h, 014h, 002h
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        inc    eax
        add    eax, dword ptr [eax]
        db  000h
loc_1_0214:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0ch]
        mov    edi, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [edi + 12h]
        cmp    eax, 15h
        je     SHORT loc_1_023B
        cmp    eax, 28h
        je     SHORT loc_1_023B
        mov    eax, 20h
        cmp    byte ptr [edi + 11h], 1
        je     SHORT loc_1_023B
        mov    eax, 1fh
loc_1_023B:
        mov    word ptr [ebp + 1ah], ax
        pop    edi
        pop    ebp
        ret
loc_1_0242:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    eax, eax
        mov    word ptr [ebp + 1ah], ax
        pop    ebp
        ret
        db  000h
VDEF_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0A7h, 0FDh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0A0h, 0FDh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0C0h, 0FDh, 0FEh, 0FFh
        cmp    eax, 4
        je     SHORT loc_1_0279
        cmp    eax, 14h
        je     SHORT loc_1_0277
        clc
        ret
loc_1_0277:
        clc
        ret
loc_1_0279:
        clc
        ret
        db  0F8h, 0C3h, 000h, 000h, 000h
loc_1_0280:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   ebp
        mov    edi, dword ptr [ebp + 1ch]
        test   byte ptr [edi + 11h], 1
        jne    NEAR PTR loc_1_0325
loc_1_0298:
        call   sub_1_0120
        mov    word ptr [ebx + 4], 0dh
        or     dword ptr [ebx + 8], 110h
        mov    esi, edi
        mov    edi, ebx
        mov    ebx, dword ptr [ebp + 28h]
        push   ebx
        mov    dword ptr [edi + 38h], ebx
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [edi + 2ch], ax
        dec    al
        mov    byte ptr [edi + 4dh], al
        movzx  eax, word ptr [ebx + 18h]
        mov    dword ptr [edi + 30h], eax
        test   byte ptr [ebp + 18h], 1
        je     SHORT loc_1_02DB
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 34h], eax
        and    byte ptr [edi + 8], 0efh
        jmp    SHORT loc_1_02E8
loc_1_02DB:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [edi + 34h], eax
loc_1_02E8:
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edi + 2eh], ax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 44h], eax
        mov    eax, edi
        push   edi
        mov    edi, esi
        call   sub_1_019E
        pop    edi
        pop    ebx
        pop    ebp
        mov    eax, dword ptr [edi + 3ch]
        mov    word ptr [ebx + 1ch], ax
        cmp    word ptr [edi + 6], 10h
        jae    SHORT loc_1_0313
        xor    eax, eax
loc_1_0313:
        mov    word ptr [ebp + 1ah], ax
        mov    ebx, edi
        call   sub_1_0181
loc_1_031E:
        pop    ebp
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_0325:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    ebx, dword ptr [edi + 0ch]
        pop    ebx
        je     NEAR PTR loc_1_0298
        pop    ebp
        mov    word ptr [ebp + 1ah], 5
        jmp    SHORT loc_1_031E
        db  000h
loc_1_0340:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        push   ebx
        push   esi
        push   edi
        xor    ax, ax
        mov    word ptr [ebp + 1ah], ax
        cmp    byte ptr [ebp + 4], 2
        je     SHORT loc_1_03AA
        mov    edi, dword ptr [ebp + 1ch]
        call   sub_1_03BD
        call   sub_1_0120
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 1ch], eax
        call   sub_1_043A
        or     byte ptr [ebx + 9], 1
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_0389
        mov    word ptr [ebx + 4], 1
loc_1_0389:
        mov    eax, ebx
        call   sub_1_019E
        xor    ax, ax
        movzx  ecx, word ptr [ebx + 6]
        cmp    ecx, 10h
        ja     SHORT loc_1_03AF
loc_1_039C:
        mov    word ptr [ebp + 1ah], ax
        call   sub_1_0181
        call   sub_1_0419
loc_1_03AA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_03AF:
        mov    eax, ecx
        movzx  ecx, word ptr [ebx + 4]
        db  0CDh, 020h, 00Eh, 000h, 010h, 000h
        jmp    SHORT loc_1_039C
sub_1_03BD:
        mov    eax, dword ptr [ebp]
        mov    esi, dword ptr [edi + 8]
        mul    dword ptr [esi + 10h]
        mov    edx, 80000000h
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_03D3
        xor    edx, edx
loc_1_03D3:
        or     edx, 40000000h
        mov    esi, dword ptr [ebp + 14h]
        add    eax, esi
        dec    eax
        shr    esi, 0ch
        shr    eax, 0ch
        sub    eax, esi
        inc    eax
        mov    dword ptr [ebp + 38h], eax
        push   edx
        push   eax
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        and    eax, eax
        je     SHORT loc_1_0414
        mov    edx, dword ptr [ebp + 14h]
        and    edx, 0fffh
        or     eax, edx
        mov    dword ptr [ebp + 14h], eax
        shr    eax, 0ch
        mov    dword ptr [ebp + 34h], eax
        or     byte ptr [ebp + 3ch], 1
        ret
loc_1_0414:
        or     byte ptr [ebp + 3ch], 2
        ret
sub_1_0419:
        test   byte ptr [ebp + 3ch], 1
        je     SHORT loc_1_0439
        mov    esi, dword ptr [ebp + 34h]
        mov    eax, dword ptr [ebp + 38h]
        push   40000000h
        push   eax
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        xor    dword ptr [ebp + 3ch], 1
loc_1_0439:
        ret
sub_1_043A:
        test   byte ptr [ebp + 3ch], 2
        jne    SHORT loc_1_0457
        push   edi
        push   ebx
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 44h], eax
        push   ebx
        db  0FFh, 015h, 078h, 004h, 000h, 000h
        or     eax, eax
        pop    eax
        pop    ebx
        pop    edi
        jne    SHORT loc_1_0457
        ret
loc_1_0457:
        or     byte ptr [ebx + 8], 40h
        ret
loc_1_045C:
        pop    esp
        add    al, 0
        db  000h, 05Ch, 004h, 000h
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
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 000h, 000h, 000h, 056h, 044h, 045h, 046h, 020h
        db  020h, 020h, 020h, 000h, 001h, 001h, 0A0h, 050h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h
        db  000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
