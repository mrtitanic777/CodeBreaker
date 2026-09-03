; pppmac_lcode_01_03.asm -- PPPMAC object 1 segment _LTEXT  (part 3/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 4
        mov    word ptr [ebp - 46h], 2b80h
        mov    word ptr [ebp - 44h], 3781h
        mov    dword ptr [ebp - 42h], 0e4h
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        db  0C7h, 045h, 0C8h, 0F0h, 08Bh, 001h, 000h
        db  0C7h, 045h, 0CCh, 0A0h, 09Bh, 000h, 000h
        db  0C7h, 045h, 0D0h, 010h, 09Ch, 000h, 000h
        db  0C7h, 045h, 0D4h, 030h, 09Ch, 000h, 000h
        db  0C7h, 045h, 0D8h, 070h, 09Dh, 000h, 000h
        db  0C7h, 045h, 0DCh, 0B0h, 09Dh, 000h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 080h, 09Eh, 000h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 0E0h, 09Eh, 000h, 000h
        db  0C7h, 045h, 0ECh, 0A0h, 09Fh, 000h, 000h
        db  0C7h, 045h, 0F0h, 0D0h, 0AEh, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 000h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_9B30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    eax, [esi + 4]
        mov    dword ptr [esi], 0
        db  068h, 0F8h, 08Bh, 001h, 000h
        push   eax
        call   sub_1_083C
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 0eh], 2
        lea    edx, [esi + 10h]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        mov    word ptr [esi + 40h], ax
        push   2
        db  068h, 000h, 08Ch, 001h, 000h
        push   edx
        call   sub_1_083C
        mov    word ptr [esi + 40h], 2
        mov    byte ptr [esi + 42h], 0
        mov    byte ptr [esi + 43h], 0
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 45h], 0
        mov    byte ptr [esi + 46h], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9BA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        push   esi
        mov    edi, dword ptr [eax + 0ch]
        call   sub_1_9B30
        lea    eax, [esi + 90h]
        push   eax
        call   sub_1_9B30
        db  0E8h, 0A8h, 07Dh, 0FDh, 0FFh
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi + 0dch], eax
        push   2b00h
        mov    eax, dword ptr [edi + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_9BF2
        mov    eax, dword ptr [eax + 4ah]
        mov    dword ptr [esi + 0d8h], eax
loc_1_9BF2:
        mov    al, byte ptr [edi + 111h]
        db  0A2h, 0E4h, 08Bh, 001h, 000h
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9C10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12bh
        push   eax
        call   sub_1_B290
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        lea    ebx, [esi + 90h]
        mov    edi, dword ptr [eax + 0ch]
        lea    eax, [esi + 48h]
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [eax + 44h]
        or     al, al
        jne    SHORT loc_1_9C84
        cmp    byte ptr [ebx + 44h], 0
        jne    SHORT loc_1_9C70
        lea    eax, [esi + 0e0h]
        push   eax
        call   sub_1_B290
        jmp    NEAR PTR loc_1_9CF7
        db  090h
loc_1_9C70:
        or     al, al
        jne    SHORT loc_1_9C84
        push   0
        push   1
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        call   sub_1_AF30
loc_1_9C84:
        cmp    byte ptr [ebx + 44h], 0
        jne    SHORT loc_1_9CA0
        push   1
        push   0
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        call   sub_1_AF30
        cmp    byte ptr [ebx + 44h], 0
        je     SHORT loc_1_9CC0
loc_1_9CA0:
        mov    al, byte ptr [ebx + 0dh]
        and    eax, 1
        push   eax
        push   1
        push   1
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        call   sub_1_BAE0
        db  068h, 060h, 0B3h, 000h, 000h
        jmp    SHORT loc_1_9CDD
        db  08Bh, 0FFh
loc_1_9CC0:
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 44h], 0
        je     SHORT loc_1_9CF7
        push   0
        push   1
        push   0
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        call   sub_1_BAE0
        push   0
loc_1_9CDD:
        db  068h, 0D0h, 0B6h, 000h, 000h
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        push   2b00h
        push   edi
        call   sub_1_1620
        add    esp, 14h
loc_1_9CF7:
        sub    eax, eax
loc_1_9CF9:
        lea    ebx, [esi + eax]
        lea    edx, [edi + eax]
        mov    cl, byte ptr [ebx + 4ch]
        mov    byte ptr [edx + 90h], cl
        mov    cl, byte ptr [ebx + 94h]
        mov    byte ptr [edx + 96h], cl
        inc    eax
        cmp    eax, 6
        jl     SHORT loc_1_9CF9
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 42h], 0
        je     SHORT loc_1_9D39
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_9D39
        push   eax
        mov    eax, dword ptr [esi + 0d8h]
        push   eax
        db  0E8h, 0AEh, 07Ch, 0FDh, 0FFh
        add    esp, 8
loc_1_9D39:
        mov    eax, dword ptr [edi + 0feh]
        cmp    dword ptr [eax + 4], 2
        jne    SHORT loc_1_9D58
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [esi + 0d8h]
        push   eax
        db  0E8h, 038h, 07Ch, 0FDh, 0FFh
        add    esp, 8
loc_1_9D58:
        push   2b00h
        push   edi
        call   PPPMAC_Service_05
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_9D70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   2b00h
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        push   esi
        call   PPPMAC_Service_06
        add    esp, 8
        push   0
        push   0
        push   0
        push   2b00h
        push   esi
        call   sub_1_1620
        add    esp, 14h
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9DB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        lea    ebx, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebx + 4]
        push   eax
        call   sub_1_AE60
        mov    byte ptr [ebx + 43h], 1
        mov    byte ptr [ebx + 47h], 1
        db  080h, 03Dh, 0E4h, 08Bh, 001h, 000h, 000h
        je     SHORT loc_1_9DE8
        mov    byte ptr [ebx + 44h], 1
loc_1_9DE8:
        mov    word ptr [ebx + 0ah], 200h
        mov    byte ptr [ebx + 0ch], 0fh
        mov    byte ptr [ebx + 0dh], 1
        mov    byte ptr [ebx + 45h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ebx + 0eh], 0
        mov    eax, dword ptr [eax + 0feh]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebx + 0d8h]
        push   eax
        db  0E8h, 08Fh, 07Bh, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_9E35
        mov    dword ptr [ebp - 8], 0
loc_1_9E35:
        mov    byte ptr [ebx + 42h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx], eax
        lea    edi, [ebx + 48h]
        mov    esi, ebx
        mov    ecx, 12h
        rep movsd
        db  080h, 03Dh, 0E4h, 08Bh, 001h, 000h, 000h
        je     SHORT loc_1_9E71
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebx + 0e0h]
        push   eax
        call   sub_1_B180
        cmp    eax, 800h
        je     SHORT loc_1_9E71
        mov    byte ptr [ebx + 8ch], 0
loc_1_9E71:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_9E80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 93h
        sub    dx, dx
        cmp    byte ptr [eax + 42h], dl
        je     SHORT loc_1_9E97
        mov    dx, 6
loc_1_9E97:
        cmp    byte ptr [eax + 43h], 0
        je     SHORT loc_1_9EA1
        add    dx, 8
loc_1_9EA1:
        cmp    byte ptr [eax + 44h], 0
        je     SHORT loc_1_9EAB
        add    dx, 6
loc_1_9EAB:
        cmp    byte ptr [eax + 45h], 0
        je     SHORT loc_1_9EB5
        add    dx, 4
loc_1_9EB5:
        cmp    byte ptr [eax + 46h], 0
        je     SHORT loc_1_9EC6
        mov    cx, word ptr [eax + 40h]
        add    cx, 2
        add    dx, cx
loc_1_9EC6:
        cmp    byte ptr [eax + 47h], 0
        je     SHORT loc_1_9ED0
        add    dx, 2
loc_1_9ED0:
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], dx
        mov    al, 1
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_9EE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 93h
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [edi + 42h], 0
        je     SHORT loc_1_9F06
        mov    byte ptr [esi], 1
        inc    esi
        mov    byte ptr [esi], 6
        inc    esi
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        add    esi, 4
loc_1_9F06:
        cmp    byte ptr [edi + 43h], 0
        je     SHORT loc_1_9F23
        mov    byte ptr [esi], 2
        inc    esi
        push   6
        mov    byte ptr [esi], 8
        inc    esi
        lea    eax, [edi + 4]
        push   eax
        push   esi
        call   sub_1_083C
        add    esi, 6
loc_1_9F23:
        cmp    byte ptr [edi + 44h], 0
        je     SHORT loc_1_9F47
        mov    byte ptr [esi], 3
        inc    esi
        mov    byte ptr [esi], 6
        inc    esi
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [esi], ax
        add    esi, 2
        mov    al, byte ptr [edi + 0ch]
        mov    byte ptr [esi], al
        inc    esi
        mov    al, byte ptr [edi + 0dh]
        mov    byte ptr [esi], al
        inc    esi
loc_1_9F47:
        cmp    byte ptr [edi + 45h], 0
        je     SHORT loc_1_9F63
        mov    byte ptr [esi], 4
        inc    esi
        mov    byte ptr [esi], 4
        inc    esi
        mov    ax, word ptr [edi + 0eh]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        add    esi, 2
loc_1_9F63:
        cmp    byte ptr [edi + 46h], 0
        je     SHORT loc_1_9F8F
        mov    byte ptr [esi], 5
        inc    esi
        mov    ax, word ptr [edi + 40h]
        add    al, 2
        mov    byte ptr [esi], al
        inc    esi
        xor    eax, eax
        mov    ax, word ptr [edi + 40h]
        push   eax
        lea    eax, [edi + 10h]
        push   eax
        push   esi
        call   sub_1_083C
        xor    eax, eax
        mov    ax, word ptr [edi + 40h]
        add    esi, eax
loc_1_9F8F:
        cmp    byte ptr [edi + 47h], 0
        je     SHORT loc_1_9F9C
        mov    byte ptr [esi], 6
        mov    byte ptr [esi + 1], 2
loc_1_9F9C:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_9FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 10h
        lea    eax, [ebx + 4bh]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebx + 2fh]
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 2
        jl     SHORT loc_1_9FD8
        cmp    eax, 4
        jle    SHORT loc_1_9FCC
        cmp    eax, 6
        jne    SHORT loc_1_9FD8
loc_1_9FCC:
        mov    dl, byte ptr [ecx + 1]
        cmp    byte ptr [ebx + 15h], dl
        jne    NEAR PTR loc_1_A136
loc_1_9FD8:
        dec    eax
        cmp    eax, 6
        ja     SHORT loc_1_9FE5
        db  0FFh, 024h, 085h, 044h, 0A1h, 000h, 000h
loc_1_9FE5:
        mov    byte ptr [ebp - 1], 0ch
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_9FF6:
        cmp    byte ptr [ebx + 10h], 9
        jne    SHORT loc_1_A00F
        mov    esi, dword ptr [ebp - 8]
        add    esi, 90h
        lea    edi, [ebp - 50h]
        mov    ecx, 12h
        rep movsd
loc_1_A00F:
        lea    eax, [ebx + 39h]
        push   eax
        mov    eax, dword ptr [ebx + 35h]
        push   eax
        mov    ax, word ptr [ebx + 33h]
        push   eax
        mov    eax, dword ptr [ebx + 2fh]
        push   eax
        push   ebx
        call   sub_1_A160
        or     al, al
        mov    byte ptr [ebp - 1], 6
        jne    SHORT loc_1_A032
        mov    byte ptr [ebp - 1], 7
loc_1_A032:
        cmp    byte ptr [ebx + 10h], 9
        jne    NEAR PTR loc_1_A136
        cmp    byte ptr [ebp - 1], 6
        jne    NEAR PTR loc_1_A136
        mov    edi, dword ptr [ebp - 8]
        lea    esi, [ebp - 50h]
        lea    edi, [edi + 90h]
        mov    ecx, 12h
        repe cmpsd
        jne    NEAR PTR loc_1_A136
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 3bh]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 28h], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebx + 3bh]
        mov    ax, word ptr [ebx + 39h]
        push   1
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebx + 3bh]
        push   eax
        call   sub_1_4360
        mov    dword ptr [ebx + 3bh], 0
        mov    dword ptr [ebx + 35h], 0
        mov    word ptr [ebx + 39h], 0
        mov    byte ptr [ebp - 1], 10h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A0A9:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_A850
        or     al, al
        je     SHORT loc_1_A136
        mov    byte ptr [ebp - 1], 8
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A0CA:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_AA80
        or     al, al
        je     SHORT loc_1_A136
        mov    byte ptr [ebp - 1], 9
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A0EB:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_ACD0
        or     al, al
        je     SHORT loc_1_A136
        mov    byte ptr [ebp - 1], 9
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_A110:
        mov    byte ptr [ebp - 1], 0ah
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A121:
        mov    byte ptr [ebp - 1], 0bh
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A132:
        mov    byte ptr [ebp - 1], 0dh
loc_1_A136:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 0F6h, 09Fh, 000h, 000h, 0A9h, 0A0h, 000h, 000h, 0CAh, 0A0h, 000h, 000h, 0EBh, 0A0h, 000h
        db  000h, 010h, 0A1h, 000h, 000h, 021h, 0A1h, 000h, 000h, 032h, 0A1h, 000h, 000h
sub_1_A160:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 2ch], eax
        mov    ecx, eax
        add    ecx, 90h
        mov    dword ptr [ebp - 0ch], ecx
        lea    ecx, [eax + 48h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], ecx
        lea    esi, [edi + 4]
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        mov    word ptr [ebp - 6], ax
        mov    byte ptr [ebp - 1], 2
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        call   sub_1_9B30
        mov    edx, dword ptr [ebp + 14h]
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [edx + 1], al
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 1ch], eax
        mov    edi, eax
        cmp    word ptr [ebp - 6], 0
        je     NEAR PTR loc_1_A7E5
loc_1_A1C0:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [esi + 1]
        inc    esi
        mov    byte ptr [ebp - 2], al
        inc    esi
        and    ax, 0ffh
        mov    word ptr [ebp - 10h], ax
        mov    ebx, eax
        and    ebx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, ebx
        jb     NEAR PTR loc_1_A824
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        dec    eax
        cmp    eax, 5
        ja     NEAR PTR loc_1_A7B2
        db  0FFh, 024h, 085h, 038h, 0A8h, 000h, 000h
loc_1_A201:
        cmp    byte ptr [ebp - 2], 6
        jne    NEAR PTR loc_1_A7B2
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_A21A
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A21A:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], eax
        mov    ecx, dword ptr [ecx]
        cmp    eax, ecx
        jae    SHORT loc_1_A270
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_A270
        mov    edi, dword ptr [ebp - 1ch]
loc_1_A231:
        cmp    dword ptr [ebp - 18h], ecx
loc_1_A234:
        jbe    SHORT loc_1_A284
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_A261
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 08Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D6h, 079h, 0FDh, 0FFh
        add    esp, 10h
loc_1_A261:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ecx + 42h], 1
        jmp    SHORT loc_1_A2EC
        db  090h
loc_1_A270:
        cmp    dword ptr [ebp - 18h], ecx
        jb     SHORT loc_1_A234
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_1_A231
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A284:
        cmp    dword ptr [ebp - 18h], ecx
        jae    SHORT loc_1_A2C2
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_A2B4
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 090h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 083h, 079h, 0FDh, 0FFh
        add    esp, 10h
loc_1_A2B4:
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ebp - 1], 3
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_1_A2EC
loc_1_A2C2:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_A2EC
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 092h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 04Bh, 079h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_A2EC:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        inc    edi
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [edi], eax
        add    edi, 4
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_A310:
        cmp    byte ptr [ebp - 2], 8
        jne    NEAR PTR loc_1_A7B2
        mov    byte ptr [ebp - 4], 1
        sub    edx, edx
loc_1_A320:
        mov    al, byte ptr [esi + edx]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    byte ptr [edx + ecx + 4], al
        jne    SHORT loc_1_A334
        inc    edx
        cmp    edx, 6
        jl     SHORT loc_1_A320
        jmp    SHORT loc_1_A338
loc_1_A334:
        mov    byte ptr [ebp - 4], 0
loc_1_A338:
        cmp    byte ptr [ebp - 4], 0
        je     SHORT loc_1_A380
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_A350
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
        db  08Dh, 049h, 000h
loc_1_A350:
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_A359
        mov    edi, dword ptr [ebp - 1ch]
loc_1_A359:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [edi], 8
        inc    edi
        add    eax, 4
        push   eax
        push   edi
        call   sub_1_AE60
        add    edi, 6
        mov    byte ptr [ebp - 1], 3
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A380:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_A390
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
        db  090h
loc_1_A390:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        sub    edx, edx
        mov    byte ptr [edi], 8
        inc    edi
loc_1_A3A0:
        mov    al, byte ptr [esi]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [edx + ecx + 4], al
        inc    esi
        mov    byte ptr [edi], al
        inc    edi
        inc    edx
        cmp    edx, 6
        jl     SHORT loc_1_A3A0
        mov    byte ptr [ecx + 43h], 1
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A7DA
        xor    eax, eax
        mov    al, byte ptr [ecx + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 7]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 6]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 4]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 094h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 026h, 078h, 0FDh, 0FFh
        add    esp, 20h
        jmp    NEAR PTR loc_1_A7DA
loc_1_A416:
        mov    eax, dword ptr [ebp - 2ch]
        mov    dl, byte ptr [ebp - 1]
        mov    ecx, dword ptr [eax + 0e0h]
        cmp    byte ptr [ebp - 2], 4
        jb     NEAR PTR loc_1_A7B2
        or     ecx, ecx
        je     NEAR PTR loc_1_A7B2
        cmp    word ptr [esi], 200h
        jne    NEAR PTR loc_1_A564
        cmp    byte ptr [ebp - 2], 6
        jne    NEAR PTR loc_1_A7B2
        cmp    dl, 4
        jne    SHORT loc_1_A457
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A457:
        mov    byte ptr [ebp - 34h], 3
        mov    byte ptr [ebp - 33h], 6
        mov    word ptr [ebp - 32h], 200h
        lea    eax, [esi + 2]
        mov    dword ptr [ebp - 20h], eax
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 30h], al
        lea    eax, [esi + 3]
        mov    dword ptr [ebp - 24h], eax
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2fh], al
        mov    eax, dword ptr [ebp - 20h]
        cmp    byte ptr [eax], 1
        jae    SHORT loc_1_A489
        mov    byte ptr [ebp - 30h], 0fh
        mov    dl, 3
loc_1_A489:
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   eax, 0fffffffeh
        je     SHORT loc_1_A4A0
        mov    byte ptr [ebp - 2fh], 1
        mov    dl, 3
loc_1_A4A0:
        cmp    dl, 3
        jne    SHORT loc_1_A4CA
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_A4AE
        mov    edi, dword ptr [ebp - 1ch]
loc_1_A4AE:
        mov    byte ptr [ebp - 1], 3
        push   6
        lea    eax, [ebp - 34h]
        push   eax
        push   edi
        call   sub_1_083C
        add    edi, 6
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A4CA:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        inc    ax
        push   eax
        push   0
        push   ecx
        call   sub_1_AFC0
        cmp    eax, 800h
        jne    NEAR PTR loc_1_A7B2
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 20h]
        mov    byte ptr [ecx + 44h], 1
        mov    word ptr [ecx + 0ah], 200h
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp - 24h]
        push   6
        mov    al, byte ptr [eax]
        lea    esi, [ebx + esi - 2]
        mov    byte ptr [ecx + 0dh], al
        lea    eax, [ebp - 34h]
        push   eax
        push   edi
        call   sub_1_083C
        add    edi, 6
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A7DA
        mov    ecx, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    al, byte ptr [ecx + 0dh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 0ch]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 096h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D8h, 076h, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_A7DA
loc_1_A564:
        push   10h
        push   0
        push   ecx
        call   sub_1_AFC0
        cmp    eax, 800h
        jne    NEAR PTR loc_1_A7B2
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_A582
        mov    edi, dword ptr [ebp - 1ch]
loc_1_A582:
        mov    byte ptr [ebp - 1], 3
        mov    byte ptr [edi], 3
        mov    byte ptr [edi + 1], 6
        mov    word ptr [edi + 2], 200h
        lea    esi, [ebx + esi - 2]
        mov    byte ptr [edi + 4], 0fh
        mov    byte ptr [edi + 5], 1
        add    edi, 6
        jmp    NEAR PTR loc_1_A7CE
loc_1_A5A7:
        cmp    byte ptr [ebp - 2], 4
        jb     NEAR PTR loc_1_A7B2
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_A5C0
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A5C0:
        mov    ecx, dword ptr [ebp - 14h]
        mov    ax, word ptr [ecx + 0eh]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_1_A618
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_A5F0
        mov    edi, dword ptr [ebp - 1ch]
loc_1_A5F0:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [edi], 4
        inc    edi
        mov    ax, word ptr [ecx + 0eh]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edi], cx
        add    edi, 2
        mov    byte ptr [ebp - 1], 3
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A618:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        mov    cx, word ptr [esi]
        mov    ah, cl
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0ch]
        inc    edi
        mov    word ptr [ecx + 0eh], ax
        lea    eax, [ebx - 2]
        or     eax, eax
        jle    SHORT loc_1_A645
        sub    ebx, 2
loc_1_A63C:
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        inc    esi
        inc    edi
        dec    ebx
        jne    SHORT loc_1_A63C
loc_1_A645:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 6]
        mov    byte ptr [ecx + 45h], 1
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A7DA
        xor    eax, eax
        mov    ax, word ptr [ecx + 0eh]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 098h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0AEh, 075h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A7DA
        db  08Bh, 0FFh
loc_1_A690:
        cmp    byte ptr [ebp - 2], 3
        jb     NEAR PTR loc_1_A7B2
        cmp    byte ptr [ebp - 2], 31h
        ja     NEAR PTR loc_1_A7B2
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_A6B3
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_A7CE
loc_1_A6B3:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        inc    edi
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 10h
        mov    dword ptr [ebp - 28h], eax
        lea    eax, [ebx - 2]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ebx - 2]
        mov    byte ptr [ecx + 46h], 1
        push   eax
        push   esi
        push   edi
        call   sub_1_083C
        lea    edi, [ebx + edi - 2]
        lea    esi, [ebx + esi - 2]
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_A7DA
        mov    ecx, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    al, byte ptr [ecx + 17h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 16h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 15h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 14h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 13h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 12h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 11h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 09Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D9h, 074h, 0FDh, 0FFh
        add    esp, 28h
        jmp    SHORT loc_1_A7DA
loc_1_A760:
        cmp    byte ptr [ebp - 2], 2
        jne    SHORT loc_1_A7B2
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_A772
        lea    esi, [ebx + esi - 2]
        jmp    SHORT loc_1_A7CE
loc_1_A772:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        inc    edi
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_A7DA
        xor    eax, eax
        db  066h, 0A1h, 09Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 087h, 074h, 0FDh, 0FFh
        add    esp, 8
        jmp    SHORT loc_1_A7DA
loc_1_A7B2:
        cmp    byte ptr [ebp - 1], 4
        je     SHORT loc_1_A7BF
        mov    edi, dword ptr [ebp - 1ch]
        mov    byte ptr [ebp - 1], 4
loc_1_A7BF:
        sub    esi, 2
        push   ebx
        push   esi
        push   edi
        call   sub_1_083C
        add    edi, ebx
        add    esi, ebx
loc_1_A7CE:
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
loc_1_A7DA:
        cmp    word ptr [ebp - 6], 0
        jne    NEAR PTR loc_1_A1C0
loc_1_A7E5:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_A7F4
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        call   sub_1_9B30
loc_1_A7F4:
        mov    eax, dword ptr [ebp + 18h]
        sub    edi, dword ptr [ebp + 14h]
        mov    word ptr [eax], di
        mov    edx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 2], cx
        mov    al, byte ptr [ebp - 1]
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_A824:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h, 001h, 0A2h, 000h, 000h, 010h, 0A3h, 000h, 000h, 016h, 0A4h, 000h, 000h, 0A7h, 0A5h, 000h
        db  000h, 090h, 0A6h, 000h, 000h, 060h, 0A7h, 000h, 000h
sub_1_A850:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 93h
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    cx, word ptr [ebp + 10h]
        sub    cx, 4
        cmp    byte ptr [esi + 42h], 0
        je     SHORT loc_1_A8AB
        cmp    cx, 6
        jb     NEAR PTR loc_1_A91D
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 1
        jne    NEAR PTR loc_1_A91D
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 6
        jne    NEAR PTR loc_1_A91D
        mov    eax, dword ptr [edx]
        cmp    dword ptr [esi], eax
        je     SHORT loc_1_A8A4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A8A4:
        add    edx, 4
        sub    cx, 6
loc_1_A8AB:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_1_A8DF
        cmp    cx, 8
        jb     SHORT loc_1_A930
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_A930
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 8
        jne    SHORT loc_1_A930
        sub    edi, edi
loc_1_A8C9:
        mov    al, byte ptr [edx + edi]
        cmp    byte ptr [esi + edi + 4], al
        jne    SHORT loc_1_A926
        inc    edi
        cmp    edi, 6
        jl     SHORT loc_1_A8C9
        add    edx, 6
        sub    cx, 8
loc_1_A8DF:
        cmp    byte ptr [esi + 44h], 0
        je     NEAR PTR loc_1_A967
        cmp    cx, 6
        jb     NEAR PTR loc_1_A9BC
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 3
        jne    NEAR PTR loc_1_A9BC
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 6
        jne    NEAR PTR loc_1_A9BC
        mov    ax, word ptr [edx]
        cmp    word ptr [esi + 0ah], ax
        je     SHORT loc_1_A939
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A91D:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A926:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  090h
loc_1_A930:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A939:
        add    edx, 2
        mov    eax, edx
        inc    edx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + 0ch], al
        je     SHORT loc_1_A950
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  090h
loc_1_A950:
        mov    eax, edx
        inc    edx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + 0dh], al
        je     SHORT loc_1_A963
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A963:
        sub    cx, 6
loc_1_A967:
        cmp    byte ptr [esi + 45h], 0
        je     SHORT loc_1_A9CC
        cmp    cx, 4
        jb     NEAR PTR loc_1_AA4D
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 4
        jne    NEAR PTR loc_1_AA4D
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 4
        jne    NEAR PTR loc_1_AA4D
        mov    ax, word ptr [edx]
        mov    di, ax
        and    edi, 0ffh
        shl    edi, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     edi, eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0eh]
        cmp    edi, eax
        je     SHORT loc_1_A9C5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A9BC:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_A9C5:
        add    edx, 2
        sub    cx, 4
loc_1_A9CC:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_1_AA2B
        xor    edi, edi
        mov    di, word ptr [esi + 40h]
        mov    ebx, ecx
        lea    eax, [edi + 2]
        and    ebx, 0ffffh
        cmp    eax, ebx
        jg     SHORT loc_1_AA60
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 5
        jne    SHORT loc_1_AA60
        mov    eax, edx
        inc    edx
        mov    ebx, edi
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        sub    ebx, eax
        cmp    ebx, -2
        jne    SHORT loc_1_AA60
        sub    ebx, ebx
        cmp    word ptr [esi + 40h], bx
        je     SHORT loc_1_AA1E
        lea    ebx, [ebx]
loc_1_AA10:
        mov    al, byte ptr [edx + ebx]
        cmp    byte ptr [esi + ebx + 10h], al
        jne    SHORT loc_1_AA56
        inc    ebx
        cmp    ebx, edi
        jl     SHORT loc_1_AA10
loc_1_AA1E:
        add    edx, edi
        mov    ax, word ptr [esi + 40h]
        add    ax, 2
        sub    cx, ax
loc_1_AA2B:
        cmp    byte ptr [esi + 47h], 0
        je     SHORT loc_1_AA69
        cmp    cx, 2
        jb     SHORT loc_1_AA44
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_AA44
        cmp    byte ptr [edx], 2
        je     SHORT loc_1_AA69
loc_1_AA44:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_AA4D:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_AA56:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  090h
loc_1_AA60:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_AA69:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AA80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 4bh]
        mov    dword ptr [ebp - 8], ecx
        mov    ebx, ecx
        mov    eax, dword ptr [eax + 4]
        add    ebx, 48h
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        je     NEAR PTR loc_1_AC9D
loc_1_AAB4:
        xor    eax, eax
        mov    al, byte ptr [esi]
        dec    eax
        cmp    eax, 5
        ja     SHORT loc_1_AADB
        db  0FFh, 024h, 085h, 0B4h, 0ACh, 000h, 000h
loc_1_AAC5:
        cmp    byte ptr [esi + 1], 6
        jne    SHORT loc_1_AADB
        mov    eax, dword ptr [esi + 2]
        add    esi, 2
        cmp    byte ptr [ebx + 42h], 0
        je     SHORT loc_1_AAE6
        cmp    dword ptr [ebx], eax
        jbe    SHORT loc_1_AAEA
loc_1_AADB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_AAE6:
        mov    byte ptr [ebx + 42h], 1
loc_1_AAEA:
        mov    dword ptr [ebx], eax
        add    esi, 4
        sub    di, 6
        jmp    NEAR PTR loc_1_AC94
loc_1_AAF8:
        cmp    byte ptr [esi + 1], 8
        jne    SHORT loc_1_AADB
        cmp    byte ptr [ebx + 43h], 0
        jne    SHORT loc_1_AB08
        mov    byte ptr [ebx + 43h], 1
loc_1_AB08:
        add    esi, 2
        sub    ecx, ecx
        lea    ecx, [ecx]
loc_1_AB10:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + ecx + 4], al
        inc    esi
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_AB10
        sub    di, 8
        jmp    NEAR PTR loc_1_AC94
loc_1_AB26:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 0e0h]
        add    ecx, 0e0h
        mov    dword ptr [ebp - 4], eax
        cmp    word ptr [esi + 2], 200h
        je     SHORT loc_1_AB50
        mov    byte ptr [ebx + 44h], 0
        jmp    NEAR PTR loc_1_AC82
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_AB50:
        cmp    byte ptr [ebx + 44h], 0
        je     NEAR PTR loc_1_AC03
        cmp    byte ptr [esi + 1], 6
        jne    NEAR PTR loc_1_AADB
        mov    al, byte ptr [ebx + 0ch]
        cmp    byte ptr [esi + 4], al
        jne    SHORT loc_1_ABA3
        mov    al, byte ptr [ebx + 0dh]
        cmp    byte ptr [esi + 5], al
        jne    SHORT loc_1_ABA3
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_AC26
        xor    eax, eax
        db  066h, 0A1h, 09Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 099h, 070h, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_AC26
loc_1_ABA3:
        mov    al, byte ptr [esi + 5]
        cmp    byte ptr [ebx + 0dh], al
        je     SHORT loc_1_ABB3
        or     al, al
        jne    SHORT loc_1_ABB3
        mov    byte ptr [ebx + 0dh], 0
loc_1_ABB3:
        mov    al, byte ptr [esi + 4]
        cmp    byte ptr [ebx + 0ch], al
        je     SHORT loc_1_AC26
        cmp    al, 1
        jb     SHORT loc_1_AC26
        push   0
        and    ax, 0ffh
        inc    ax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AFC0
        cmp    eax, 800h
        jne    SHORT loc_1_ABE0
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx + 0ch], al
        jmp    SHORT loc_1_AC26
loc_1_ABE0:
        push   0
        mov    al, byte ptr [ebx + 0ch]
        and    eax, 0ffff00ffh
        inc    ax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AFC0
        cmp    eax, 800h
        je     SHORT loc_1_AC26
        mov    byte ptr [ebx + 44h], 0
        jmp    SHORT loc_1_AC26
loc_1_AC03:
        mov    word ptr [ebx + 0ah], 200h
        mov    byte ptr [ebx + 0ch], 0fh
        mov    byte ptr [ebx + 0dh], 1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_1_B180
        cmp    eax, 800h
        jne    SHORT loc_1_AC26
        mov    byte ptr [ebx + 44h], 1
loc_1_AC26:
        add    esi, 6
        sub    di, 6
        jmp    SHORT loc_1_AC94
        db  090h
loc_1_AC30:
        mov    ecx, dword ptr [esi + 2]
        mov    al, ch
        mov    ah, cl
        or     ax, ax
        je     SHORT loc_1_AC48
        cmp    ax, 2
        je     SHORT loc_1_AC48
        mov    byte ptr [ebx + 45h], 0
        jmp    SHORT loc_1_AC82
loc_1_AC48:
        mov    byte ptr [ebx + 45h], 1
        mov    word ptr [ebx + 0eh], ax
        jmp    SHORT loc_1_AC82
loc_1_AC52:
        mov    byte ptr [ebx + 46h], 0
        jmp    SHORT loc_1_AC82
loc_1_AC58:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_AC7E
        xor    eax, eax
        db  066h, 0A1h, 0A0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B9h, 06Fh, 0FDh, 0FFh
        add    esp, 8
loc_1_AC7E:
        mov    byte ptr [ebx + 47h], 0
loc_1_AC82:
        mov    al, byte ptr [esi + 1]
        and    eax, 0ffff00ffh
        sub    di, ax
        and    eax, 0ffffh
        add    esi, eax
loc_1_AC94:
        or     di, di
        jne    NEAR PTR loc_1_AAB4
loc_1_AC9D:
        or     di, di
        jne    NEAR PTR loc_1_AADB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 0C5h, 0AAh, 000h, 000h, 0F8h, 0AAh, 000h, 000h, 026h, 0ABh, 000h, 000h, 030h
        db  0ACh, 000h, 000h, 052h, 0ACh, 000h, 000h, 058h, 0ACh, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_ACD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 93h
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 4
        mov    dx, word ptr [ebp + 10h]
        sub    dx, 4
        cmp    byte ptr [esi + 42h], 0
        je     SHORT loc_1_AD1B
        cmp    byte ptr [ecx], 1
        jne    SHORT loc_1_AD1B
        cmp    byte ptr [ecx + 1], 6
        jne    NEAR PTR loc_1_AE50
        mov    eax, dword ptr [ecx + 2]
        add    ecx, 2
        cmp    dword ptr [esi], eax
        jne    NEAR PTR loc_1_AE50
        mov    byte ptr [esi + 42h], 0
        add    ecx, 4
        sub    dx, 6
loc_1_AD1B:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_1_AD52
        cmp    byte ptr [ecx], 2
        jne    SHORT loc_1_AD52
        cmp    byte ptr [ecx + 1], 8
        jne    NEAR PTR loc_1_AE50
        add    ecx, 2
        sub    edi, edi
loc_1_AD35:
        mov    eax, ecx
        inc    ecx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + edi + 4], al
        jne    NEAR PTR loc_1_AE50
        inc    edi
        cmp    edi, 6
        jl     SHORT loc_1_AD35
        sub    dx, 8
        mov    byte ptr [esi + 43h], 0
loc_1_AD52:
        cmp    byte ptr [esi + 44h], 0
        je     SHORT loc_1_AD98
        cmp    byte ptr [ecx], 3
        jne    SHORT loc_1_AD98
        cmp    byte ptr [ecx + 1], 6
        jne    NEAR PTR loc_1_AE50
        mov    ax, word ptr [esi + 0ah]
        cmp    word ptr [ecx + 2], ax
        jne    NEAR PTR loc_1_AE50
        mov    al, byte ptr [ecx + 4]
        cmp    byte ptr [esi + 0ch], al
        jne    NEAR PTR loc_1_AE50
        mov    al, byte ptr [esi + 0dh]
        cmp    byte ptr [ecx + 5], al
        jne    NEAR PTR loc_1_AE50
        mov    byte ptr [esi + 44h], 0
        add    ecx, 6
        sub    dx, 6
loc_1_AD98:
        cmp    byte ptr [esi + 45h], 0
        je     SHORT loc_1_ADDD
        cmp    byte ptr [ecx], 4
        jne    SHORT loc_1_ADDD
        cmp    byte ptr [ecx + 1], 4
        jne    NEAR PTR loc_1_AE50
        mov    ax, word ptr [ecx + 2]
        mov    di, ax
        and    edi, 0ffh
        shl    edi, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     edi, eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0eh]
        cmp    edi, eax
        jne    SHORT loc_1_AE50
        mov    byte ptr [esi + 45h], 0
        add    ecx, 4
        sub    dx, 4
loc_1_ADDD:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_1_AE26
        cmp    byte ptr [ecx], 5
        jne    SHORT loc_1_AE26
        xor    edi, edi
        mov    di, word ptr [esi + 40h]
        xor    ebx, ebx
        mov    bl, byte ptr [ecx + 1]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -2
        jne    SHORT loc_1_AE50
        add    ecx, 2
        sub    ebx, ebx
        cmp    word ptr [esi + 40h], bx
        je     SHORT loc_1_AE17
loc_1_AE07:
        mov    eax, ecx
        inc    ecx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + ebx + 10h], al
        jne    SHORT loc_1_AE50
        inc    ebx
        cmp    ebx, edi
        jl     SHORT loc_1_AE07
loc_1_AE17:
        mov    byte ptr [esi + 46h], 0
        mov    ax, word ptr [esi + 40h]
        add    ax, 2
        sub    dx, ax
loc_1_AE26:
        cmp    byte ptr [esi + 47h], 0
        je     SHORT loc_1_AE40
        cmp    byte ptr [ecx], 6
        jne    SHORT loc_1_AE40
        mov    byte ptr [esi + 47h], 0
        mov    al, byte ptr [ecx + 1]
        and    eax, 0ffff00ffh
        sub    dx, ax
loc_1_AE40:
        or     dx, dx
        jne    SHORT loc_1_AE50
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_AE50:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AE60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_AE90
        sub    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
loc_1_AE73:
        mov    bl, byte ptr [eax + ecx]
        mov    byte ptr [edx + ecx], bl
        inc    ecx
        cmp    ecx, 5
        jl     SHORT loc_1_AE73
        mov    al, byte ptr [eax + 3]
        inc    al
        mov    byte ptr [edx + 3], al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_AE90:
        call   sub_1_01F7
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], 61h
        mov    byte ptr [edx + 2], 6eh
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    byte ptr [edx + 3], al
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [edx + 5], al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_AED0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        add    ebx, 4bh
        cmp    dword ptr [esi + 8], 0a4h
        jae    SHORT loc_1_AEFB
        mov    dword ptr [esi + 8], 0a4h
        mov    dword ptr [esi + 0ch], 80bh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_AEFB:
        lea    edi, [esi + 14h]
        push   48h
        lea    eax, [ebx + 48h]
        push   eax
        push   edi
        call   sub_1_083C
        push   48h
        add    ebx, 90h
        push   ebx
        add    edi, 48h
        push   edi
        call   sub_1_083C
        mov    dword ptr [esi + 0ch], 800h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_AF30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_AF45
        mov    eax, 802h
        pop    esi
        pop    ebp
        ret    0ch
loc_1_AF45:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_AF7C
        mov    ax, word ptr [esi + 14h]
        or     ax, ax
        je     SHORT loc_1_AF7C
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        push   0
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 18h], 0
        mov    word ptr [esi + 14h], 0
loc_1_AF7C:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_1_AFB3
        mov    ax, word ptr [esi + 16h]
        or     ax, ax
        je     SHORT loc_1_AFB3
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        push   0
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 1ch], 0
        mov    word ptr [esi + 16h], 0
loc_1_AFB3:
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_AFC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_AFE0
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_AFE0:
        mov    bx, word ptr [ebp + 0ch]
        or     bx, bx
        je     NEAR PTR loc_1_B086
        mov    eax, ebx
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    word ptr [esi + 14h], bx
        je     SHORT loc_1_B03A
        push   0
        push   1
        push   esi
        call   sub_1_AF30
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_B040
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_B03A:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
loc_1_B040:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 18h], eax
        mov    word ptr [esi + 14h], bx
        sub    ax, ax
        cmp    bx, ax
        je     SHORT loc_1_B086
        sub    ecx, ecx
        mov    edx, 2ch
        nop
loc_1_B070:
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [edi + 4], al
        mov    edi, dword ptr [ebp - 4]
        inc    ax
        mov    byte ptr [edi + 7], cl
        add    dword ptr [ebp - 4], edx
        cmp    bx, ax
        ja     SHORT loc_1_B070
loc_1_B086:
        cmp    word ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_B16B
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 0ffffh
        lea    eax, [ebx + ebx*4]
        lea    eax, [eax + eax*8]
        sub    eax, ebx
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ebp + 10h]
        cmp    word ptr [esi + 16h], ax
        je     SHORT loc_1_B0E2
        push   1
        push   0
        push   esi
        call   sub_1_AF30
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_B0E8
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_B0E2:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 4], eax
loc_1_B0E8:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1ch], eax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [esi + 16h], ax
        sub    cx, cx
        dec    ebx
        or     ebx, ebx
        jle    SHORT loc_1_B144
loc_1_B114:
        mov    edx, ecx
        and    edx, 0ffffh
        mov    eax, edx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*8]
        sub    edx, eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [edx + eax + 4], cl
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        inc    cx
        lea    edx, [eax + 2ch]
        mov    dword ptr [eax], edx
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    eax, ebx
        jl     SHORT loc_1_B114
loc_1_B144:
        mov    edx, ecx
        and    edx, 0ffffh
        mov    eax, edx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*8]
        sub    edx, eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [edx + eax + 4], cl
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [edx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], eax
loc_1_B16B:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_B180:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_B1A0
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_B1A0:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_B1D0
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   20h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_B213
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_B1D0:
        db  0C7h, 045h, 0FCh, 004h, 08Ch, 001h, 000h
        db  083h, 03Dh, 004h, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_B1F1
loc_1_B1E0:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [esi], eax
        je     SHORT loc_1_B1F1
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_B1E0
loc_1_B1F1:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_B207
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_B207:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
loc_1_B213:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    byte ptr [eax + 11h], 1
        mov    eax, dword ptr [ebp - 4]
        push   10h
        mov    byte ptr [eax + 11h], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AFC0
        cmp    eax, 800h
        je     SHORT loc_1_B261
        push   0
        push   20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi], 0
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_B261:
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 004h, 08Ch, 001h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        db  0A3h, 004h, 08Ch, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi], eax
        mov    eax, 800h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_B290:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        or     edx, edx
        je     SHORT loc_1_B2FA
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     SHORT loc_1_B2FA
        db  0BEh, 004h, 08Ch, 001h, 000h
        db  083h, 03Dh, 004h, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_B2BD
        nop
loc_1_B2B0:
        mov    eax, dword ptr [esi]
        cmp    eax, ecx
        je     SHORT loc_1_B2BD
        mov    esi, eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_B2B0
loc_1_B2BD:
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    SHORT loc_1_B2D0
        mov    eax, 802h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_B2D0:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [edx]
        mov    dword ptr [edx], 0
        push   1
        push   1
        push   esi
        call   sub_1_AF30
        push   0
        push   20h
        push   esi
        call   sub_1_075C
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    4
loc_1_B2FA:
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_B310:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 0ffffh
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   PPPMAC_Service_02
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_B353
        mov    word ptr [edi + 28h], 2b00h
        mov    ecx, dword ptr [edi + 1ch]
        add    ecx, dword ptr [edi + 20h]
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_1_083C
        push   edi
        push   esi
        call   PPPMAC_Service_04
        add    esp, 8
loc_1_B353:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_B360:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, dword ptr [ecx + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, 6
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [ebp - 8]
        lea    edi, [ebx + 0dh]
        lea    eax, [eax + 5]
        mov    dword ptr [ebp - 10h], eax
        mov    esi, eax
        repe cmpsd
        jne    SHORT loc_1_B3DA
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_B3DA
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 11h
        jne    SHORT loc_1_B3D0
        test   byte ptr [ebx + 6], 4
        je     SHORT loc_1_B3D0
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 21h]
        cmp    byte ptr [ebx + 29h], al
        jne    SHORT loc_1_B3C7
        mov    al, byte ptr [ecx + 23h]
        cmp    byte ptr [ebx + 2bh], al
        jne    SHORT loc_1_B3C7
        mov    ax, word ptr [ebx + 26h]
        cmp    word ptr [ecx + 1eh], ax
        jne    SHORT loc_1_B3C7
        jmp    SHORT loc_1_B3D0
loc_1_B3C7:
        mov    eax, 1
        jmp    SHORT loc_1_B3D2
        db  08Bh, 0FFh
loc_1_B3D0:
        sub    eax, eax
loc_1_B3D2:
        or     eax, eax
        je     NEAR PTR loc_1_B494
loc_1_B3DA:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 8]
loc_1_B3E0:
        mov    dword ptr [ebp - 14h], ebx
        mov    ebx, dword ptr [ebx]
        mov    ecx, 6
        lea    edi, [ebx + 0dh]
        mov    esi, dword ptr [ebp - 10h]
        repe cmpsd
        jne    SHORT loc_1_B439
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_B439
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 11h
        jne    SHORT loc_1_B430
        test   byte ptr [ebx + 6], 4
        je     SHORT loc_1_B430
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 21h]
        cmp    byte ptr [ebx + 29h], al
        jne    SHORT loc_1_B427
        mov    al, byte ptr [ecx + 23h]
        cmp    byte ptr [ebx + 2bh], al
        jne    SHORT loc_1_B427
        mov    ax, word ptr [ebx + 26h]
        cmp    word ptr [ecx + 1eh], ax
        jne    SHORT loc_1_B427
        jmp    SHORT loc_1_B430
loc_1_B427:
        sub    eax, eax
        jmp    SHORT loc_1_B435
        db  005h, 000h, 000h, 000h, 000h
loc_1_B430:
        mov    eax, 1
loc_1_B435:
        or     eax, eax
        jne    SHORT loc_1_B478
loc_1_B439:
        cmp    edx, ebx
        jne    SHORT loc_1_B3E0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 8], eax
        mov    byte ptr [ebx + 6], 0
loc_1_B44A:
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 11h
        jne    NEAR PTR loc_1_B630
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 1eh]
        cmp    ax, 2222h
        je     SHORT loc_1_B46D
        cmp    ax, 3333h
        jne    NEAR PTR loc_1_B630
loc_1_B46D:
        or     byte ptr [ebx + 6], 4
        push   24h
        jmp    NEAR PTR loc_1_B632
loc_1_B478:
        cmp    edx, ebx
        jne    SHORT loc_1_B487
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_1_B494
loc_1_B487:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx], ebx
loc_1_B494:
        test   byte ptr [ebx + 6], 2
        je     SHORT loc_1_B44A
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebx + 0ch]
        cmp    byte ptr [ecx + 4], al
        jne    SHORT loc_1_B44A
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax], 11h
        jne    SHORT loc_1_B4B5
        mov    al, byte ptr [ecx + 20h]
        cmp    byte ptr [ebx + 28h], al
        jae    SHORT loc_1_B44A
loc_1_B4B5:
        lea    ecx, [ebp - 1ch]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [ecx + 11h], 2
        je     SHORT loc_1_B4E4
        mov    al, byte ptr [ebx + 4]
        lea    esi, [ebx + 4]
        cmp    byte ptr [ecx + 10h], al
        jne    SHORT loc_1_B4E4
        mov    ax, word ptr [ecx + 12h]
        cmp    ax, 5
        lea    edx, [eax + 1]
        mov    word ptr [ecx + 12h], dx
        jb     SHORT loc_1_B500
loc_1_B4E4:
        mov    byte ptr [ebp - 1], 80h
        mov    al, byte ptr [ebx + 4]
        lea    esi, [ebx + 4]
        lea    ecx, [ebp - 1bh]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1ch], al
        mov    word ptr [ecx + 12h], 1
loc_1_B500:
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx]
        cmp    ax, 0ffffh
        je     SHORT loc_1_B51A
        mov    ecx, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 1], 40h
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 0ch], 2
loc_1_B51A:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 11h], 4
        jne    SHORT loc_1_B575
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ebp - 1], 20h
        mov    ax, word ptr [ecx + 2]
        cmp    ax, 80h
        jae    SHORT loc_1_B536
        jmp    SHORT loc_1_B56D
loc_1_B536:
        cmp    ax, 4000h
        jae    SHORT loc_1_B549
        shr    ax, 8
        mov    ecx, dword ptr [ebp - 0ch]
        or     al, 80h
        mov    byte ptr [ecx], al
        jmp    SHORT loc_1_B564
loc_1_B549:
        cmp    ax, 0ffffh
        jae    SHORT loc_1_B575
        mov    ecx, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], 0c0h
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 2]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], ah
loc_1_B564:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + 2]
loc_1_B56D:
        mov    ecx, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
loc_1_B575:
        test   byte ptr [ebx + 6], 4
        je     SHORT loc_1_B5B9
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + 22h]
        cmp    byte ptr [ebx + 2ah], al
        je     SHORT loc_1_B592
        mov    ecx, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 1], 10h
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
loc_1_B592:
        test   byte ptr [ebx + 6], 4
        je     SHORT loc_1_B5B9
        push   24h
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        add    ebx, 8
        push   ebx
        call   sub_1_083C
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 24h
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dword ptr [edi], 24h
        add    dword ptr [ecx + 20h], 24h
        jmp    SHORT loc_1_B5D8
loc_1_B5B9:
        push   1eh
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        add    ebx, 8
        push   ebx
        call   sub_1_083C
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 24h
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dword ptr [edi], 1eh
        add    dword ptr [ecx + 20h], 1eh
loc_1_B5D8:
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 1ch]
        sub    ecx, eax
        or     cl, cl
        je     SHORT loc_1_B605
        xor    eax, eax
        mov    al, cl
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [edi], eax
        mov    edx, dword ptr [ecx + 20h]
        sub    edx, eax
        mov    dword ptr [ecx + 20h], edx
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, edx
        push   eax
        call   sub_1_083C
loc_1_B605:
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dword ptr [edi]
        mov    eax, dword ptr [ecx + 20h]
        dec    eax
        mov    dword ptr [ecx + 20h], eax
        mov    ecx, dword ptr [ecx + 1ch]
        mov    dl, byte ptr [ebp - 1]
        mov    byte ptr [ecx + eax], dl
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [esi]
        mov    byte ptr [ecx + 10h], al
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_B630:
        push   1eh
loc_1_B632:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    eax, [ebx + 8]
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ecx + 10h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 12h], 1
        cmp    byte ptr [eax], 11h
        jne    SHORT loc_1_B697
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 1eh]
        cmp    ax, 2222h
        je     SHORT loc_1_B669
        cmp    ax, 3333h
        jne    SHORT loc_1_B697
loc_1_B669:
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [ecx + 24h], 2
        mov    edx, dword ptr [ecx + 20h]
        sub    edx, 2
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, edx
        mov    dword ptr [ecx + 20h], edx
        mov    byte ptr [eax], 7
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [eax + 1], cl
        mov    eax, 800h
        or     byte ptr [ebx + 6], 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B697:
        mov    ecx, dword ptr [ebp + 0ch]
        add    dword ptr [ecx + 24h], 3
        mov    edx, dword ptr [ecx + 20h]
        sub    edx, 3
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, edx
        mov    dword ptr [ecx + 20h], edx
        mov    byte ptr [eax], 3
        inc    eax
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [ebx + 5]
        inc    cl
        mov    byte ptr [ebx + 5], cl
        mov    byte ptr [eax + 1], cl
        or     byte ptr [ebx + 6], 1
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B6D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 24h]
        or     ecx, ecx
        je     NEAR PTR loc_1_BAC3
        mov    esi, dword ptr [edx + 1ch]
        add    esi, dword ptr [edx + 20h]
        mov    bl, byte ptr [esi]
        inc    esi
        cmp    bl, 0ffh
        jne    SHORT loc_1_B701
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B701:
        cmp    bl, 9
        jne    SHORT loc_1_B738
        cmp    ecx, 3
        je     SHORT loc_1_B724
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 11h], 40h
        jne    NEAR PTR loc_1_BAC6
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B724:
        test   bl, 1
        je     SHORT loc_1_B72C
        cmp    bl, 1
loc_1_B72C:
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B738:
        cmp    bl, 5
        jne    NEAR PTR loc_1_B7D0
        cmp    ecx, 3
        je     SHORT loc_1_B760
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 11h], 40h
        jne    NEAR PTR loc_1_BAC6
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_B760:
        mov    cl, byte ptr [esi + 1]
        mov    al, byte ptr [esi]
        mov    edx, dword ptr [ebp + 8]
        and    eax, 0ffff00ffh
        cmp    word ptr [edx + 16h], ax
        ja     SHORT loc_1_B780
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_B780:
        mov    esi, eax
        and    esi, 0ffffh
        mov    eax, esi
        lea    esi, [esi + esi*4]
        lea    esi, [esi + esi*8]
        sub    esi, eax
        add    esi, dword ptr [edx + 1ch]
        cmp    byte ptr [esi + 5], cl
        je     SHORT loc_1_B7A6
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B7A6:
        mov    al, byte ptr [esi + 6]
        test   al, 1
        jne    SHORT loc_1_B7B9
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B7B9:
        and    al, 0feh
        mov    byte ptr [esi + 6], al
        or     al, 2
        mov    byte ptr [esi + 6], al
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_B7D0:
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx + 11h]
        test   al, 40h
        jne    SHORT loc_1_B7E6
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B7E6:
        test   bl, 1
        jne    NEAR PTR loc_1_B950
        test   bl, 80h
        je     SHORT loc_1_B819
        mov    al, byte ptr [esi]
        inc    esi
        and    ax, 0ffh
        cmp    word ptr [edx + 14h], ax
        jbe    NEAR PTR loc_1_BAC6
        and    eax, 0ffffh
        mov    edi, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        sub    eax, edi
        add    eax, dword ptr [edx + 18h]
        jmp    SHORT loc_1_B82C
loc_1_B819:
        test   al, 1
        je     SHORT loc_1_B829
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B829:
        mov    eax, dword ptr [edx + 0ch]
loc_1_B82C:
        cmp    byte ptr [eax + 7], 0ffh
        jne    NEAR PTR loc_1_BAC6
        mov    cl, byte ptr [eax + 6]
        and    cl, 4
        mov    byte ptr [ebp - 2], cl
        test   bl, 40h
        je     SHORT loc_1_B850
        mov    di, word ptr [esi]
        mov    word ptr [ebp - 6], di
        add    esi, 2
        jmp    SHORT loc_1_B856
loc_1_B850:
        mov    word ptr [ebp - 6], 0ffffh
loc_1_B856:
        sub    cx, cx
        test   bl, 20h
        je     SHORT loc_1_B8B6
        mov    cl, byte ptr [esi]
        cmp    cl, 80h
        jae    SHORT loc_1_B870
        and    cx, 0ffh
        inc    esi
        jmp    SHORT loc_1_B8AC
        db  08Dh, 049h, 000h
loc_1_B870:
        cmp    cl, 0c0h
        jae    SHORT loc_1_B891
        mov    byte ptr [ebp - 14h], cl
        and    word ptr [ebp - 14h], 7fh
        mov    di, word ptr [ebp - 14h]
        shl    di, 8
        and    cx, 0ffh
        add    cx, di
        inc    esi
        inc    esi
        jmp    SHORT loc_1_B8AC
loc_1_B891:
        cmp    cl, 0e0h
        jae    SHORT loc_1_B8A9
        mov    cl, byte ptr [esi + 1]
        inc    esi
        and    ecx, 0ffff00ffh
        imul   cx, cx, 101h
        inc    esi
        inc    esi
        jmp    SHORT loc_1_B8AC
loc_1_B8A9:
        sub    cx, cx
loc_1_B8AC:
        cmp    cx, 1eh
        jb     NEAR PTR loc_1_BAC6
loc_1_B8B6:
        test   bl, 10h
        je     SHORT loc_1_B8CB
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_BAC6
        mov    bl, byte ptr [esi]
        mov    byte ptr [eax + 2ah], bl
        inc    esi
loc_1_B8CB:
        mov    dword ptr [edx + 0ch], eax
        mov    di, word ptr [ebp - 6]
        and    byte ptr [edx + 11h], 0feh
        mov    edx, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 8], di
        mov    edi, dword ptr [edx + 20h]
        sub    esi, dword ptr [edx + 1ch]
        sub    si, di
        and    esi, 0ffffh
        add    edi, esi
        mov    dword ptr [edx + 20h], edi
        sub    dword ptr [edx + 24h], esi
        cmp    byte ptr [ebp - 2], 0
        mov    dx, 1eh
        je     SHORT loc_1_B905
        mov    dx, 24h
        inc    byte ptr [eax + 28h]
loc_1_B905:
        mov    edi, edx
        mov    edx, dword ptr [ebp + 0ch]
        and    edi, 0ffffh
        mov    esi, dword ptr [edx + 24h]
        add    esi, edi
        mov    dword ptr [edx + 24h], esi
        sub    dword ptr [edx + 20h], edi
        or     cx, cx
        jne    SHORT loc_1_B923
        mov    cx, si
loc_1_B923:
        mov    dh, cl
        mov    dl, ch
        mov    word ptr [eax + 0ah], dx
        push   edi
        add    eax, 8
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx + 1ch]
        add    eax, dword ptr [edx + 20h]
        push   eax
        call   sub_1_083C
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_B950:
        mov    edi, ebx
        and    edi, 0ffh
        cmp    edi, 1
        je     SHORT loc_1_B9B5
        cmp    edi, 3
        je     SHORT loc_1_B9CB
        cmp    edi, 7
        je     SHORT loc_1_B9CB
        mov    byte ptr [ebp - 10h], 9
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 0fh], al
        mov    byte ptr [ebp - 0eh], bl
        lea    eax, [ebp - 10h]
        push   3
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        call   sub_1_B310
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_B9A9
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 0A4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 08Eh, 062h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_B9A9:
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B9B5:
        mov    edx, dword ptr [ebp + 0ch]
        dec    ecx
        mov    dword ptr [edx + 24h], ecx
        inc    dword ptr [edx + 20h]
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_B9CB:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 1], al
        inc    esi
        and    ax, 0ffh
        cmp    word ptr [edx + 14h], ax
        jbe    NEAR PTR loc_1_BAC6
        and    eax, 0ffffh
        mov    edi, eax
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [edx + 18h]
        lea    eax, [eax + eax*8]
        sub    eax, edi
        lea    edi, [ecx + eax]
        mov    dword ptr [edx + 0ch], edi
        and    byte ptr [edx + 11h], 0feh
        mov    byte ptr [edi + 6], 0
        mov    byte ptr [edi + 7], 0ffh
        cmp    bl, 3
        jne    SHORT loc_1_BA35
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 5], al
        mov    byte ptr [ebp - 0ch], 5
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 0bh], al
        mov    al, byte ptr [edi + 5]
        inc    esi
        mov    byte ptr [ebp - 0ah], al
        push   3
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        call   sub_1_B310
        mov    word ptr [ebp - 4], 3
        jmp    SHORT loc_1_BA3B
loc_1_BA35:
        mov    word ptr [ebp - 4], 2
loc_1_BA3B:
        cmp    byte ptr [esi + 5], 11h
        jne    SHORT loc_1_BA90
        mov    ax, word ptr [esi + 1eh]
        cmp    ax, 2222h
        je     SHORT loc_1_BA51
        cmp    ax, 3333h
        jne    SHORT loc_1_BA90
loc_1_BA51:
        cmp    bl, 3
        jne    SHORT loc_1_BA82
        mov    bx, 1eh
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_BA94
        xor    eax, eax
        db  066h, 0A1h, 0A2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B7h, 061h, 0FDh, 0FFh
        add    esp, 8
        jmp    SHORT loc_1_BA94
loc_1_BA82:
        or     byte ptr [edi + 6], 4
        mov    bx, 24h
        jmp    SHORT loc_1_BA94
        db  08Dh, 064h, 024h, 000h
loc_1_BA90:
        mov    bx, 1eh
loc_1_BA94:
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        push   esi
        add    edi, 8
        push   edi
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        sub    dword ptr [edx + 24h], eax
        add    dword ptr [edx + 20h], eax
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_BAC3:
        mov    edx, dword ptr [ebp + 8]
loc_1_BAC6:
        or     byte ptr [edx + 11h], 1
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_BAE0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0ch], 0
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_BAF2
        or     byte ptr [ecx + 11h], 80h
        jmp    SHORT loc_1_BAF6
loc_1_BAF2:
        and    byte ptr [ecx + 11h], 7fh
loc_1_BAF6:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_BB02
        or     byte ptr [ecx + 11h], 40h
        jmp    SHORT loc_1_BB06
loc_1_BB02:
        and    byte ptr [ecx + 11h], 0bfh
loc_1_BB06:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_BB20
        or     byte ptr [ecx + 11h], 2
        mov    eax, 800h
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_BB20:
        and    byte ptr [ecx + 11h], 0fdh
        mov    eax, 800h
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
sub_1_BB30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 4
        mov    word ptr [ebp - 46h], 2180h
        mov    word ptr [ebp - 44h], 8
        mov    dword ptr [ebp - 42h], 89h
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        db  0C7h, 045h, 0C8h, 014h, 08Ch, 001h, 000h
        db  0C7h, 045h, 0CCh, 040h, 0BCh, 000h, 000h
        db  0C7h, 045h, 0D0h, 0B0h, 0BCh, 000h, 000h
        db  0C7h, 045h, 0D4h, 0D0h, 0BCh, 000h, 000h
        db  0C7h, 045h, 0D8h, 0F0h, 0BFh, 000h, 000h
        db  0C7h, 045h, 0DCh, 090h, 0C0h, 000h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 070h, 0C2h, 000h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 000h, 0C3h, 000h, 000h
        db  0C7h, 045h, 0ECh, 0C0h, 0C3h, 000h, 000h
        db  0C7h, 045h, 0F0h, 0D0h, 0D3h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 0FCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_BBF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [eax + 18h], 0
        mov    dword ptr [eax + 1ch], 0
        mov    byte ptr [eax + 20h], 0
        mov    byte ptr [eax + 21h], 0
        mov    byte ptr [eax + 22h], 0
        mov    byte ptr [eax + 23h], 0
        mov    byte ptr [eax + 24h], 0
        mov    byte ptr [eax + 25h], 0
        mov    byte ptr [eax + 26h], 0
        pop    ebp
        ret    4
        db  090h
loc_1_BC40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    edi, dword ptr [eax + 0ch]
        db  0E8h, 0AEh, 05Dh, 0FDh, 0FFh
        mov    dword ptr [edi + 16bh], eax
        or     eax, eax
        jne    SHORT loc_1_BC90
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_BC86
        xor    eax, eax
        db  066h, 0A1h, 0A6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B1h, 05Fh, 0FDh, 0FFh
        add    esp, 8
loc_1_BC86:
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
loc_1_BC90:
        push   esi
        call   sub_1_BBF0
        add    esi, 4eh
        push   esi
        call   sub_1_BBF0
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_BCB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0cch
        push   eax
        call   sub_1_D7D0
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_BCD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 8]
        add    esi, 4bh
        mov    eax, dword ptr [eax + 4]
        lea    ebx, [esi + 27h]
        mov    edi, dword ptr [eax + 0ch]
        lea    ecx, [esi + 4eh]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    al, byte ptr [ebx + 21h]
        or     al, al
        jne    SHORT loc_1_BD20
        cmp    byte ptr [ecx + 21h], 0
        jne    SHORT loc_1_BD20
        add    esi, 81h
        push   esi
        call   sub_1_D7D0
        jmp    NEAR PTR loc_1_BDEC
        db  08Dh, 049h, 000h
loc_1_BD20:
        mov    byte ptr [ebp - 1], 3
        or     al, al
        jne    SHORT loc_1_BD3C
        push   0
        push   1
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_D460
        mov    byte ptr [ebp - 1], 1
loc_1_BD3C:
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 21h], 0
        jne    SHORT loc_1_BD58
        push   1
        push   0
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_D460
        dec    byte ptr [ebp - 1]
loc_1_BD58:
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 21h], 0
        je     SHORT loc_1_BD80
        mov    al, byte ptr [ecx + 0bh]
        and    eax, 1
        push   eax
        push   1
        push   1
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_E830
        db  068h, 050h, 0D8h, 000h, 000h
        jmp    SHORT loc_1_BD9A
        db  090h
loc_1_BD80:
        cmp    byte ptr [ebx + 21h], 0
        je     SHORT loc_1_BDB4
        push   0
        push   1
        push   0
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_E830
        push   0
loc_1_BD9A:
        db  068h, 030h, 0DEh, 000h, 000h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        push   2100h
        push   edi
        call   sub_1_1620
        add    esp, 14h
loc_1_BDB4:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_BDEC
        cmp    byte ptr [ebp - 1], 3
        jae    SHORT loc_1_BDEC
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_BDEC
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0C8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 04Bh, 05Eh, 0FDh, 0FFh
        add    esp, 0ch
loc_1_BDEC:
        mov    dword ptr [ebp - 34h], 6
        mov    eax, dword ptr [edi + 20h]
        add    eax, 8
        lea    edx, [ebp - 30h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ecx, 1ch
        mov    dword ptr [ebp - 1ch], 0
        cmp    byte ptr [ebx + 23h], 0
        je     SHORT loc_1_BE2D
        mov    eax, dword ptr [ebx + 10h]
        or     eax, eax
        je     SHORT loc_1_BE2D
        mov    dword ptr [ebp - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        mov    ecx, 20h
loc_1_BE2D:
        cmp    byte ptr [ebx + 25h], 0
        je     SHORT loc_1_BE48
        mov    eax, dword ptr [ebx + 18h]
        or     eax, eax
        je     SHORT loc_1_BE48
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BE48:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_BE7B
        mov    dword ptr [ebp - 20h], 6
        push   ecx
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    ecx, 1ch
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 10h], 1
loc_1_BE7B:
        cmp    byte ptr [ebx + 24h], 0
        je     SHORT loc_1_BE96
        mov    eax, dword ptr [ebx + 14h]
        or     eax, eax
        je     SHORT loc_1_BE96
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BE96:
        cmp    byte ptr [ebx + 26h], 0
        je     SHORT loc_1_BEB1
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        je     SHORT loc_1_BEB1
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BEB1:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_BEE4
        mov    dword ptr [ebp - 20h], 2ch
        push   ecx
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    ecx, 1ch
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 0ch], 1
loc_1_BEE4:
        cmp    dword ptr [edi + 187h], 0
        je     NEAR PTR loc_1_BFB6
        test   byte ptr [edi + 18bh], 2
        je     NEAR PTR loc_1_BFB6
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_BF60
        mov    eax, dword ptr [edi + 193h]
        or     eax, eax
        je     SHORT loc_1_BF1C
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BF1C:
        mov    eax, dword ptr [edi + 197h]
        or     eax, eax
        je     SHORT loc_1_BF34
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BF34:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_BF60
        mov    dword ptr [ebp - 20h], 6
        push   ecx
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    ecx, 1ch
        mov    dword ptr [ebp - 1ch], 0
loc_1_BF60:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_BFB6
        mov    eax, dword ptr [edi + 19bh]
        or     eax, eax
        je     SHORT loc_1_BF7E
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BF7E:
        mov    eax, dword ptr [edi + 19fh]
        or     eax, eax
        je     SHORT loc_1_BF96
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp + edx - 18h], eax
        add    dword ptr [ebp - 1ch], 4
        add    ecx, 4
loc_1_BF96:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_BFB6
        mov    dword ptr [ebp - 20h], 2ch
        push   ecx
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   2
        call   sub_1_0287
        add    esp, 10h
loc_1_BFB6:
        cmp    word ptr [edi + 16fh], 0
        je     SHORT loc_1_BFD1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 030h, 0D4h, 000h, 000h
        call   sub_1_02D4
        add    esp, 8
loc_1_BFD1:
        push   2100h
        push   edi
        call   PPPMAC_Service_05
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_BFF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 24h], 6
        mov    eax, dword ptr [esi + 20h]
        add    eax, 8
        lea    edx, [ebp - 20h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 10h], 6
        push   1ch
        push   eax
        push   -1
        push   2
        call   sub_1_0287
        add    esp, 10h
        mov    dword ptr [ebp - 10h], 2ch
        push   1ch
        lea    eax, [ebp - 24h]
        push   eax
        push   -1
        push   2
        call   sub_1_0287
        add    esp, 10h
        push   esi
        call   sub_1_F290
        add    esp, 4
        push   2100h
        push   esi
        call   PPPMAC_Service_06
        add    esp, 8
        push   0
        push   0
        push   0
        push   2100h
        push   esi
        call   sub_1_1620
        add    esp, 14h
        mov    dword ptr [esi + 187h], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_C090:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    ebx, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ecx + 187h], 0
        je     SHORT loc_1_C0C6
        mov    eax, dword ptr [ecx + 18bh]
        and    eax, 4
        cmp    eax, 1
        sbb    al, al
        neg    al
        db  0A2h, 008h, 08Ch, 001h, 000h
loc_1_C0C6:
        db  080h, 03Dh, 008h, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_C0D3
        mov    byte ptr [ebx + 21h], 1
loc_1_C0D3:
        mov    word ptr [ebx + 8], 2d00h
        mov    byte ptr [ebx + 0ah], 0fh
        mov    byte ptr [ebx + 0bh], 1
        mov    dword ptr [ebx + 85h], 0
        mov    esi, dword ptr [ebp - 4]
        add    esi, 17bh
        mov    edi, dword ptr [ebp - 4]
        add    edi, 16fh
        mov    eax, dword ptr [ebp - 4]
        add    eax, 17fh
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 177h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 173h
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   edi
        mov    eax, dword ptr [ecx + 20h]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ecx + 16bh]
        push   eax
        call   sub_1_EA00
        add    esp, 1ch
        cmp    eax, 802h
        jne    SHORT loc_1_C141
        mov    word ptr [edi], 0
        jmp    SHORT loc_1_C155
loc_1_C141:
        mov    byte ptr [ebx + 22h], 1
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 183h], eax
loc_1_C155:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 187h], 0
        je     SHORT loc_1_C177
        test   byte ptr [ecx + 18bh], 1
        je     SHORT loc_1_C177
        mov    byte ptr [ebx + 22h], 1
        mov    eax, dword ptr [ecx + 18fh]
        mov    dword ptr [ebx + 0ch], eax
loc_1_C177:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_C1C4
        mov    ecx, dword ptr [ebx + 0ch]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 0A8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 073h, 05Ah, 0FDh, 0FFh
        add    esp, 0ch
loc_1_C1C4:
        lea    edi, [ebx + 27h]
        mov    esi, ebx
        mov    ecx, 9
        rep movsd
        movsw
        movsb
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0feh]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_C236
        mov    byte ptr [ebx + 4ah], 1
        mov    byte ptr [ebx + 4bh], 1
        mov    byte ptr [ebx + 4ch], 1
        mov    byte ptr [ebx + 4dh], 1
        mov    dword ptr [ebx + 75h], 1
        mov    dword ptr [ebx + 79h], 0
        cmp    dword ptr [ecx + 187h], 0
        je     SHORT loc_1_C236
        test   byte ptr [ecx + 18bh], 2
        je     SHORT loc_1_C236
        mov    eax, dword ptr [ecx + 193h]
        mov    dword ptr [ebx + 37h], eax
        mov    eax, dword ptr [ecx + 197h]
        mov    dword ptr [ebx + 3fh], eax
        mov    eax, dword ptr [ecx + 19bh]
        mov    dword ptr [ebx + 3bh], eax
        mov    eax, dword ptr [ecx + 19fh]
        mov    dword ptr [ebx + 43h], eax
loc_1_C236:
        db  080h, 03Dh, 008h, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_C25A
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        lea    eax, [ebx + 81h]
        push   eax
        call   sub_1_D6C0
        cmp    eax, 800h
        je     SHORT loc_1_C25A
        mov    byte ptr [ebx + 48h], 0
loc_1_C25A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_C270:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 72h]
        add    eax, 4bh
        sub    si, si
        cmp    dword ptr [eax + 75h], 0
        je     SHORT loc_1_C2AF
        cmp    dword ptr [eax + 7dh], 0
        jne    SHORT loc_1_C2AF
        mov    edx, dword ptr [eax + 79h]
        inc    edx
        mov    dword ptr [eax + 79h], edx
        cmp    edx, 2
        jbe    SHORT loc_1_C2AF
        mov    dword ptr [eax + 75h], 0
        mov    byte ptr [eax + 4ah], 0
        mov    byte ptr [eax + 4bh], 0
        mov    byte ptr [eax + 4ch], 0
        mov    byte ptr [eax + 4dh], 0
loc_1_C2AF:
        cmp    byte ptr [ecx + 20h], 0
        je     SHORT loc_1_C2B9
        mov    si, 0ah
loc_1_C2B9:
        cmp    byte ptr [ecx + 21h], 0
        je     SHORT loc_1_C2C3
        add    si, 6
loc_1_C2C3:
        cmp    byte ptr [ecx + 22h], 0
        je     SHORT loc_1_C2CD
        add    si, 6
loc_1_C2CD:
        cmp    byte ptr [ecx + 23h], 0
        je     SHORT loc_1_C2D7
        add    si, 6
loc_1_C2D7:
        cmp    byte ptr [ecx + 24h], 0
        je     SHORT loc_1_C2E1
        add    si, 6
loc_1_C2E1:
        cmp    byte ptr [ecx + 25h], 0
        je     SHORT loc_1_C2EB
        add    si, 6
loc_1_C2EB:
        cmp    byte ptr [ecx + 26h], 0
        je     SHORT loc_1_C2F5
        add    si, 6
loc_1_C2F5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], si
        mov    al, 1
        pop    esi
        pop    ebp
        ret
loc_1_C300:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [eax + 72h]
        add    eax, 4bh
        cmp    byte ptr [edx + 20h], 0
        mov    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_1_C32E
        mov    byte ptr [ecx], 1
        inc    ecx
        mov    byte ptr [ecx], 0ah
        inc    ecx
        mov    esi, dword ptr [edx + 0ch]
        mov    dword ptr [ecx], esi
        add    ecx, 4
        mov    eax, dword ptr [eax + 5ah]
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_C32E:
        cmp    byte ptr [edx + 21h], 0
        je     SHORT loc_1_C352
        mov    byte ptr [ecx], 2
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    ax, word ptr [edx + 8]
        mov    word ptr [ecx], ax
        add    ecx, 2
        mov    al, byte ptr [edx + 0ah]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    al, byte ptr [edx + 0bh]
        mov    byte ptr [ecx], al
        inc    ecx
loc_1_C352:
        cmp    byte ptr [edx + 22h], 0
        je     SHORT loc_1_C368
        mov    byte ptr [ecx], 3
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_C368:
        cmp    byte ptr [edx + 23h], 0
        je     SHORT loc_1_C37E
        mov    byte ptr [ecx], 81h
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    eax, dword ptr [edx + 10h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_C37E:
        cmp    byte ptr [edx + 24h], 0
        je     SHORT loc_1_C394
        mov    byte ptr [ecx], 82h
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_C394:
        cmp    byte ptr [edx + 25h], 0
        je     SHORT loc_1_C3AA
        mov    byte ptr [ecx], 83h
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_C3AA:
        cmp    byte ptr [edx + 26h], 0
        je     SHORT loc_1_C3BD
        mov    byte ptr [ecx], 84h
        inc    ecx
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [edx + 1ch]
        mov    dword ptr [ecx + 1], eax
loc_1_C3BD:
        pop    esi
        pop    ebp
        ret
loc_1_C3C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 10h
        lea    eax, [ebx + 4bh]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebx + 2fh]
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 2
        jl     SHORT loc_1_C3F8
        cmp    eax, 4
        jle    SHORT loc_1_C3EC
        cmp    eax, 6
        jne    SHORT loc_1_C3F8
loc_1_C3EC:
        mov    dl, byte ptr [ecx + 1]
        cmp    byte ptr [ebx + 15h], dl
        jne    NEAR PTR loc_1_C566
loc_1_C3F8:
        dec    eax
        cmp    eax, 6
        ja     SHORT loc_1_C405
        db  0FFh, 024h, 085h, 074h, 0C5h, 000h, 000h
loc_1_C405:
        mov    byte ptr [ebp - 1], 0ch
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_C416:
        cmp    byte ptr [ebx + 10h], 9
        jne    SHORT loc_1_C42F
        mov    esi, dword ptr [ebp - 8]
        add    esi, 4eh
        lea    edi, [ebp - 30h]
        mov    ecx, 9
        rep movsd
        movsw
        movsb
loc_1_C42F:
        lea    eax, [ebx + 39h]
        push   eax
        mov    eax, dword ptr [ebx + 35h]
        push   eax
        mov    ax, word ptr [ebx + 33h]
        push   eax
        mov    eax, dword ptr [ebx + 2fh]
        push   eax
        push   ebx
        call   sub_1_C590
        or     al, al
        mov    byte ptr [ebp - 1], 6
        jne    SHORT loc_1_C452
        mov    byte ptr [ebp - 1], 7
loc_1_C452:
        cmp    byte ptr [ebx + 10h], 9
        jne    NEAR PTR loc_1_C566
        cmp    byte ptr [ebp - 1], 6
        jne    NEAR PTR loc_1_C566
        mov    edi, dword ptr [ebp - 8]
        lea    esi, [ebp - 30h]
        lea    edi, [edi + 4eh]
        mov    ecx, 9
        repe cmpsd
        jne    NEAR PTR loc_1_C566
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_1_C566
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
        jne    NEAR PTR loc_1_C566
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 3bh]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 28h], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebx + 3bh]
        mov    ax, word ptr [ebx + 39h]
        push   1
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebx + 3bh]
        push   eax
        call   sub_1_4360
        mov    dword ptr [ebx + 3bh], 0
        mov    dword ptr [ebx + 35h], 0
        mov    word ptr [ebx + 39h], 0
        mov    byte ptr [ebp - 1], 10h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_C4E0:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_CAF0
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_C500:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_CD90
        or     al, al
        je     SHORT loc_1_C566
        mov    byte ptr [ebp - 1], 9
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_C521:
        mov    ax, word ptr [ebx + 33h]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_D210
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_C540:
        mov    byte ptr [ebp - 1], 0ah
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_C551:
        mov    byte ptr [ebp - 1], 0bh
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_C562:
        mov    byte ptr [ebp - 1], 0dh
loc_1_C566:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 11h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 016h, 0C4h, 000h, 000h, 0E0h, 0C4h, 000h, 000h, 000h, 0C5h, 000h, 000h, 021h, 0C5h, 000h
        db  000h, 040h, 0C5h, 000h, 000h, 051h, 0C5h, 000h, 000h, 062h, 0C5h, 000h, 000h
sub_1_C590:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 28h], eax
        mov    ecx, eax
        add    ecx, 4eh
        mov    dword ptr [ebp - 0ch], ecx
        add    eax, 27h
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], eax
        lea    esi, [edi + 4]
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        mov    word ptr [ebp - 6], ax
        mov    byte ptr [ebp - 1], 2
        push   ecx
        call   sub_1_BBF0
        mov    edx, dword ptr [ebp + 14h]
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [edx + 1], al
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 1ch], eax
        mov    edi, eax
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_1_C660
loc_1_C5E6:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 2], al
        mov    cl, byte ptr [esi + 1]
        inc    esi
        inc    esi
        mov    al, cl
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 10h], ax
        mov    ebx, eax
        and    ebx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, ebx
        jb     NEAR PTR loc_1_CA92
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        cmp    eax, 1
        je     NEAR PTR loc_1_C720
        cmp    eax, 2
        je     NEAR PTR loc_1_C847
        cmp    eax, 3
        je     NEAR PTR loc_1_C9F0
loc_1_C631:
        cmp    byte ptr [ebp - 1], 4
        je     SHORT loc_1_C63E
        mov    edi, dword ptr [ebp - 1ch]
        mov    byte ptr [ebp - 1], 4
loc_1_C63E:
        sub    esi, 2
        push   ebx
        push   esi
        push   edi
        call   sub_1_083C
        add    edi, ebx
        add    esi, ebx
loc_1_C64D:
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
loc_1_C659:
        cmp    word ptr [ebp - 6], 0
        jne    SHORT loc_1_C5E6
loc_1_C660:
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 85h], 0
        jne    NEAR PTR loc_1_CAA5
        mov    eax, dword ptr [ebp - 14h]
        cmp    byte ptr [eax + 21h], 0
        je     NEAR PTR loc_1_CAA5
        mov    byte ptr [eax + 21h], 0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_C6A7
        xor    eax, eax
        db  066h, 0A1h, 0C4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 090h, 055h, 0FDh, 0FFh
        add    esp, 8
loc_1_C6A7:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 10h], 7
        jne    NEAR PTR loc_1_CAA5
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_C6DA
        xor    eax, eax
        db  066h, 0A1h, 0C6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 05Dh, 055h, 0FDh, 0FFh
        add    esp, 8
loc_1_C6DA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    byte ptr [eax + 10h], 6
        mov    byte ptr [eax + 11h], 4
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        cmp    byte ptr [ebp - 1], 2
        jne    NEAR PTR loc_1_CAAB
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [eax], 0
        push   ecx
        call   sub_1_BBF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_C720:
        cmp    cl, 0ah
        jne    NEAR PTR loc_1_C631
        mov    eax, dword ptr [ebp - 14h]
        cmp    byte ptr [ebp - 1], 4
        mov    byte ptr [eax + 22h], 0
        mov    byte ptr [eax + 20h], 0
        jne    SHORT loc_1_C743
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_C64D
loc_1_C743:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 24h], eax
        mov    edx, dword ptr [esi + 4]
        add    esi, 4
        lea    esi, [ebx + esi - 6]
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        or     edx, edx
        jne    SHORT loc_1_C7A5
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_C7A1
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_C7A1
        mov    edi, dword ptr [ebp - 1ch]
loc_1_C774:
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], cl
        inc    edi
        inc    edi
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ebp - 0ch]
        add    edi, 4
        mov    dword ptr [ecx + 0ch], eax
        or     edx, edx
        jne    SHORT loc_1_C7B4
        mov    ebx, dword ptr [ebp - 14h]
        add    ebx, 0ch
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_C7B4
        mov    byte ptr [ebp - 1], 3
        jmp    SHORT loc_1_C7BD
loc_1_C7A1:
        or     edx, edx
        je     SHORT loc_1_C774
loc_1_C7A5:
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_1_C774
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_C64D
loc_1_C7B4:
        mov    ebx, dword ptr [ebp - 14h]
        mov    dword ptr [ebx + 0ch], edx
        add    ebx, 0ch
loc_1_C7BD:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ebp - 0ch]
        add    edi, 4
        mov    byte ptr [ecx + 20h], 0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C659
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    ebx, edx
        and    ebx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ebx, eax
        shl    ebx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ebx, eax
        push   ebx
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 0AAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F5h, 053h, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_C659
loc_1_C847:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 3], al
        mov    eax, dword ptr [ebp - 28h]
        cmp    cl, 4
        mov    edx, dword ptr [eax + 81h]
        mov    dword ptr [eax + 85h], 1
        jb     NEAR PTR loc_1_C631
        or     edx, edx
        je     NEAR PTR loc_1_C631
        cmp    word ptr [esi], 2d00h
        jne    NEAR PTR loc_1_C9AA
        cmp    cl, 6
        jne    NEAR PTR loc_1_C631
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_C894
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_C64D
loc_1_C894:
        mov    byte ptr [ebp - 30h], 2
        mov    byte ptr [ebp - 2fh], 6
        mov    word ptr [ebp - 2eh], 2d00h
        lea    eax, [esi + 2]
        mov    dword ptr [ebp - 18h], eax
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2ch], al
        lea    eax, [esi + 3]
        mov    dword ptr [ebp - 20h], eax
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2bh], al
        mov    eax, dword ptr [ebp - 18h]
        cmp    byte ptr [eax], 3
        jae    SHORT loc_1_C8C8
        mov    byte ptr [ebp - 2ch], 0fh
        mov    byte ptr [ebp - 3], 3
loc_1_C8C8:
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   eax, 0fffffffeh
        je     SHORT loc_1_C8E1
        mov    byte ptr [ebp - 2bh], 1
        mov    byte ptr [ebp - 3], 3
loc_1_C8E1:
        cmp    byte ptr [ebp - 3], 3
        jne    SHORT loc_1_C910
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_C8F0
        mov    edi, dword ptr [ebp - 1ch]
loc_1_C8F0:
        mov    byte ptr [ebp - 1], 3
        push   6
        lea    eax, [ebp - 30h]
        push   eax
        push   edi
        call   sub_1_083C
        add    edi, 6
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_C64D
        db  08Dh, 064h, 024h, 000h
loc_1_C910:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        inc    ax
        push   eax
        push   0
        push   edx
        call   sub_1_D4F0
        cmp    eax, 800h
        jne    NEAR PTR loc_1_C631
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 21h], 1
        mov    word ptr [ecx + 8], 2d00h
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp - 20h]
        push   6
        mov    al, byte ptr [eax]
        lea    esi, [ebx + esi - 2]
        mov    byte ptr [ecx + 0bh], al
        lea    eax, [ebp - 30h]
        push   eax
        push   edi
        call   sub_1_083C
        add    edi, 6
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C659
        mov    ecx, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    al, byte ptr [ecx + 0bh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 0ah]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0ACh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 092h, 052h, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_C659
loc_1_C9AA:
        push   10h
        push   0
        push   edx
        call   sub_1_D4F0
        cmp    eax, 800h
        jne    NEAR PTR loc_1_C631
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_C9C8
        mov    edi, dword ptr [ebp - 1ch]
loc_1_C9C8:
        mov    byte ptr [ebp - 1], 3
        mov    byte ptr [edi], 2
        mov    byte ptr [edi + 1], 6
        mov    word ptr [edi + 2], 2d00h
        lea    esi, [ebx + esi - 2]
        mov    byte ptr [edi + 4], 0fh
        mov    byte ptr [edi + 5], 1
        add    edi, 6
        jmp    NEAR PTR loc_1_C64D
        db  08Dh, 049h, 000h
loc_1_C9F0:
        cmp    cl, 6
        jne    NEAR PTR loc_1_C631
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_CA08
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_C64D
loc_1_CA08:
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], cl
        inc    edi
        inc    edi
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, 4
        nop
loc_1_CA20:
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        inc    esi
        inc    edi
        dec    eax
        jne    SHORT loc_1_CA20
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 6]
        mov    byte ptr [ecx + 22h], 1
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_C659
        mov    ecx, dword ptr [ecx + 0ch]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        push   edx
        xor    eax, eax
        db  066h, 0A1h, 0AEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0AAh, 051h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_C659
loc_1_CA92:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_CAA5:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_CAB4
loc_1_CAAB:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        call   sub_1_BBF0
loc_1_CAB4:
        mov    eax, dword ptr [ebp + 18h]
        sub    edi, dword ptr [ebp + 14h]
        mov    word ptr [eax], di
        mov    edx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 2], cx
        mov    al, byte ptr [ebp - 1]
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_CAF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [edx + 72h]
        add    edx, 4bh
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_CB8F
        cmp    di, 0ah
        jb     NEAR PTR loc_1_CBC4
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 1
        jne    NEAR PTR loc_1_CBC4
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0ah
        jne    NEAR PTR loc_1_CBC4
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [ecx], eax
        je     SHORT loc_1_CB43
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CB43:
        or     eax, eax
        jne    SHORT loc_1_CB75
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_CB6D
        xor    eax, eax
        db  066h, 0A1h, 0BCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0CAh, 050h, 0FDh, 0FFh
        add    esp, 8
loc_1_CB6D:
        mov    al, 3
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CB75:
        mov    eax, dword ptr [ecx + 4]
        add    ecx, 4
        cmp    dword ptr [edx + 5ah], eax
        je     SHORT loc_1_CB88
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CB88:
        add    ecx, 4
        sub    di, 0ah
loc_1_CB8F:
        cmp    byte ptr [esi + 21h], 0
        je     SHORT loc_1_CBFB
        cmp    di, 6
        jb     NEAR PTR loc_1_CC32
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 2
        jne    NEAR PTR loc_1_CC32
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_CC32
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 8], ax
        je     SHORT loc_1_CBD0
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CBC4:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_CBD0:
        add    ecx, 2
        mov    eax, ecx
        inc    ecx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + 0ah], al
        je     SHORT loc_1_CBE5
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CBE5:
        mov    eax, ecx
        inc    ecx
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + 0bh], al
        je     SHORT loc_1_CBF7
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CBF7:
        sub    di, 6
loc_1_CBFB:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_CC77
        cmp    di, 6
        jb     NEAR PTR loc_1_CCA2
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 3
        jne    NEAR PTR loc_1_CCA2
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    NEAR PTR loc_1_CCA2
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [ecx], eax
        je     SHORT loc_1_CC3A
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CC32:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CC3A:
        or     eax, eax
        jne    SHORT loc_1_CC70
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_CC64
        xor    eax, eax
        db  066h, 0A1h, 0BCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D3h, 04Fh, 0FDh, 0FFh
        add    esp, 8
loc_1_CC64:
        mov    al, 3
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_CC70:
        add    ecx, 4
        sub    di, 6
loc_1_CC77:
        cmp    byte ptr [esi + 23h], 0
        je     SHORT loc_1_CCB7
        cmp    di, 6
        jb     SHORT loc_1_CCE2
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 81h
        jne    SHORT loc_1_CCE2
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_CCE2
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [esi + 10h], eax
        je     SHORT loc_1_CCB0
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CCA2:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_CCB0:
        add    ecx, 4
        sub    di, 6
loc_1_CCB7:
        cmp    byte ptr [esi + 24h], 0
        je     SHORT loc_1_CCF7
        cmp    di, 6
        jb     SHORT loc_1_CD22
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 82h
        jne    SHORT loc_1_CD22
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_CD22
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [esi + 14h], eax
        je     SHORT loc_1_CCF0
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CCE2:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_CCF0:
        add    ecx, 4
        sub    di, 6
loc_1_CCF7:
        cmp    byte ptr [esi + 25h], 0
        je     SHORT loc_1_CD37
        cmp    di, 6
        jb     SHORT loc_1_CD62
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 83h
        jne    SHORT loc_1_CD62
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_CD62
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [esi + 18h], eax
        je     SHORT loc_1_CD30
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CD22:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_CD30:
        add    ecx, 4
        sub    di, 6
loc_1_CD37:
        cmp    byte ptr [esi + 26h], 0
        je     SHORT loc_1_CD70
        cmp    di, 6
        jb     SHORT loc_1_CD80
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 84h
        jne    SHORT loc_1_CD80
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 6
        jne    SHORT loc_1_CD80
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [esi + 1ch], eax
        je     SHORT loc_1_CD70
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_CD62:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_CD70:
        mov    dword ptr [edx + 7dh], 1
        mov    al, 8
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  090h
loc_1_CD80:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_CD90:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 4bh]
        mov    dword ptr [ebp - 14h], ecx
        mov    esi, ecx
        mov    eax, dword ptr [eax + 4]
        add    esi, 27h
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 4
        mov    bx, word ptr [ebp + 10h]
        sub    bx, 4
        je     NEAR PTR loc_1_D1E8
loc_1_CDC4:
        xor    eax, eax
        mov    al, byte ptr [edi]
        cmp    eax, 81h
        jg     SHORT loc_1_CDF7
        je     NEAR PTR loc_1_D077
        cmp    eax, 1
        je     SHORT loc_1_CE23
        cmp    eax, 2
        je     NEAR PTR loc_1_CEE1
        cmp    eax, 3
        je     NEAR PTR loc_1_CFD0
loc_1_CDEC:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_CDF7:
        cmp    eax, 82h
        je     NEAR PTR loc_1_D0D0
        cmp    eax, 83h
        je     NEAR PTR loc_1_D125
        cmp    eax, 84h
        je     NEAR PTR loc_1_D177
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_CE23:
        cmp    byte ptr [edi + 1], 0ah
        jne    SHORT loc_1_CDEC
        mov    byte ptr [esi + 20h], 1
        mov    byte ptr [esi + 22h], 0
        mov    eax, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 4]
        add    edi, 4
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 5ah], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_CED5
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [eax + 17bh]
        mov    dword ptr [ebp - 8], eax
        and    ecx, 0ff00h
        mov    eax, dword ptr [esi + 0ch]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [esi + 0ch]
        and    eax, 0ff0000h
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 10h], edx
        shr    dword ptr [ebp - 10h], 10h
        or     eax, dword ptr [ebp - 10h]
        shr    eax, 8
        or     ecx, eax
        push   ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ff00h
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff0000h
        mov    edx, dword ptr [ebp - 8]
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0B0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 062h, 04Dh, 0FDh, 0FFh
        add    esp, 10h
loc_1_CED5:
        add    edi, 4
        sub    bx, 0ah
        jmp    NEAR PTR loc_1_D1DF
loc_1_CEE1:
        mov    ecx, dword ptr [ebp - 14h]
        cmp    word ptr [edi + 2], 2d00h
        mov    eax, dword ptr [ecx + 81h]
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_1_CF10
        mov    byte ptr [esi + 21h], 0
        mov    al, byte ptr [edi + 1]
        and    eax, 0ffff00ffh
        sub    bx, ax
        and    eax, 0ffffh
        add    edi, eax
        jmp    NEAR PTR loc_1_D1DF
loc_1_CF10:
        cmp    byte ptr [esi + 21h], 0
        je     NEAR PTR loc_1_CFC1
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        mov    al, byte ptr [esi + 0ah]
        cmp    byte ptr [edi + 4], al
        jne    SHORT loc_1_CF60
        mov    al, byte ptr [esi + 0bh]
        cmp    byte ptr [edi + 5], al
        jne    SHORT loc_1_CF60
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_CFC1
        xor    eax, eax
        db  066h, 0A1h, 0B2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D9h, 04Ch, 0FDh, 0FFh
        add    esp, 8
        jmp    SHORT loc_1_CFC1
loc_1_CF60:
        mov    al, byte ptr [edi + 5]
        cmp    byte ptr [esi + 0bh], al
        je     SHORT loc_1_CF70
        or     al, al
        jne    SHORT loc_1_CF70
        mov    byte ptr [esi + 0bh], 0
loc_1_CF70:
        mov    al, byte ptr [edi + 4]
        cmp    byte ptr [esi + 0ah], al
        je     SHORT loc_1_CFC1
        cmp    al, 3
        jb     SHORT loc_1_CFC1
        push   0
        and    ax, 0ffh
        inc    ax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_D4F0
        cmp    eax, 800h
        jne    SHORT loc_1_CFA0
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [esi + 0ah], al
        jmp    SHORT loc_1_CFC1
        db  08Dh, 049h, 000h
loc_1_CFA0:
        push   0
        mov    al, byte ptr [esi + 0ah]
        and    eax, 0ffff00ffh
        inc    ax
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_D4F0
        cmp    eax, 800h
        je     SHORT loc_1_CFC1
        mov    byte ptr [esi + 21h], 0
loc_1_CFC1:
        add    edi, 6
        jmp    NEAR PTR loc_1_D1DB
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_CFD0:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        cmp    byte ptr [esi + 22h], 0
        jne    SHORT loc_1_CFE4
        mov    byte ptr [esi + 22h], 1
loc_1_CFE4:
        mov    ecx, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 0ch], ecx
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D1D8
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, ecx
        mov    eax, dword ptr [eax + 17bh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, ecx
        shr    eax, 10h
        and    edx, 0ff0000h
        or     eax, edx
        shr    eax, 8
        mov    edx, ecx
        shl    edx, 10h
        and    ecx, 0ff00h
        or     edx, ecx
        shl    edx, 8
        or     eax, edx
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ff00h
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ff0000h
        mov    edx, dword ptr [ebp - 4]
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0B0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0C5h, 04Bh, 0FDh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_1_D1D8
loc_1_D077:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        mov    byte ptr [esi + 23h], 1
        mov    eax, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 10h], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D1D8
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        and    edx, 0ff0000h
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 10h
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        or     ecx, edx
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0B4h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D1C3
        db  08Dh, 064h, 024h, 000h
loc_1_D0D0:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        mov    byte ptr [esi + 24h], 1
        mov    eax, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 14h], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D1D8
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        and    edx, 0ff0000h
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 10h
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        or     ecx, edx
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0B6h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_D1C3
loc_1_D125:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        mov    byte ptr [esi + 25h], 1
        mov    eax, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 18h], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_D1D8
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        and    edx, 0ff0000h
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 10h
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        or     ecx, edx
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0B8h, 000h, 000h, 000h
        jmp    SHORT loc_1_D1C3
loc_1_D177:
        cmp    byte ptr [edi + 1], 6
        jne    NEAR PTR loc_1_CDEC
        mov    byte ptr [esi + 26h], 1
        mov    eax, dword ptr [edi + 2]
        add    edi, 2
        mov    dword ptr [esi + 1ch], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_D1D8
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        and    edx, 0ff0000h
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 10h
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        or     ecx, edx
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0BAh, 000h, 000h, 000h
loc_1_D1C3:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 05Fh, 04Ah, 0FDh, 0FFh
        add    esp, 0ch
loc_1_D1D8:
        add    edi, 4
loc_1_D1DB:
        sub    bx, 6
loc_1_D1DF:
        or     bx, bx
        jne    NEAR PTR loc_1_CDC4
loc_1_D1E8:
        or     bx, bx
        jne    NEAR PTR loc_1_CDEC
        mov    ecx, dword ptr [ebp - 14h]
        mov    al, 1
        mov    dword ptr [ecx + 7dh], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D210:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [edx + 72h]
        add    edx, 4bh
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_D290
        cmp    byte ptr [eax], 1
        jne    SHORT loc_1_D290
        cmp    byte ptr [eax + 1], 0ah
        jne    NEAR PTR loc_1_D3BB
        mov    ecx, dword ptr [eax + 2]
        add    eax, 2
        cmp    dword ptr [esi + 0ch], ecx
        jne    NEAR PTR loc_1_D3BB
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [edx + 5ah], eax
        jne    NEAR PTR loc_1_D3BB
        mov    byte ptr [esi + 22h], 0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_D286
        xor    eax, eax
        db  066h, 0A1h, 0BCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B1h, 049h, 0FDh, 0FFh
        add    esp, 8
loc_1_D286:
        mov    al, 3
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_D290:
        cmp    byte ptr [esi + 21h], 0
        je     SHORT loc_1_D2D6
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_D2D6
        cmp    byte ptr [eax + 1], 6
        jne    NEAR PTR loc_1_D3BB
        mov    cx, word ptr [esi + 8]
        cmp    word ptr [eax + 2], cx
        jne    NEAR PTR loc_1_D3BB
        mov    cl, byte ptr [esi + 0ah]
        cmp    byte ptr [eax + 4], cl
        jne    NEAR PTR loc_1_D3BB
        mov    cl, byte ptr [esi + 0bh]
        cmp    byte ptr [eax + 5], cl
        jne    NEAR PTR loc_1_D3BB
        mov    byte ptr [esi + 21h], 0
        add    eax, 6
        sub    di, 6
loc_1_D2D6:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_D309
        cmp    byte ptr [eax], 3
        jne    SHORT loc_1_D309
        cmp    byte ptr [eax + 1], 6
        jne    NEAR PTR loc_1_D3BB
        mov    ecx, dword ptr [eax + 2]
        add    eax, 2
        cmp    dword ptr [esi + 0ch], ecx
        jne    NEAR PTR loc_1_D3BB
        mov    byte ptr [esi + 22h], 0
        mov    byte ptr [esi + 20h], 1
        add    eax, 4
        sub    di, 6
loc_1_D309:
        cmp    byte ptr [esi + 23h], 0
        je     SHORT loc_1_D338
        cmp    byte ptr [eax], 81h
        jne    SHORT loc_1_D338
        cmp    byte ptr [eax + 1], 6
        jne    NEAR PTR loc_1_D3BB
        mov    ecx, dword ptr [eax + 2]
        add    eax, 2
        cmp    dword ptr [esi + 10h], ecx
        jne    NEAR PTR loc_1_D3BB
        mov    byte ptr [esi + 23h], 0
        add    eax, 4
        sub    di, 6
loc_1_D338:
        cmp    byte ptr [esi + 24h], 0
        je     SHORT loc_1_D35F
        cmp    byte ptr [eax], 82h
        jne    SHORT loc_1_D35F
        cmp    byte ptr [eax + 1], 6
        jne    SHORT loc_1_D3BB
        mov    ecx, dword ptr [eax + 2]
        add    eax, 2
        cmp    dword ptr [esi + 14h], ecx
        jne    SHORT loc_1_D3BB
        mov    byte ptr [esi + 24h], 0
        add    eax, 4
        sub    di, 6
loc_1_D35F:
        cmp    byte ptr [esi + 25h], 0
        je     SHORT loc_1_D386
        cmp    byte ptr [eax], 83h
        jne    SHORT loc_1_D386
        cmp    byte ptr [eax + 1], 6
        jne    SHORT loc_1_D3BB
        mov    ecx, dword ptr [eax + 2]
        add    eax, 2
        cmp    dword ptr [esi + 18h], ecx
        jne    SHORT loc_1_D3BB
        mov    byte ptr [esi + 25h], 0
        add    eax, 4
        sub    di, 6
loc_1_D386:
        cmp    byte ptr [esi + 26h], 0
        je     SHORT loc_1_D3A7
        cmp    byte ptr [eax], 84h
        jne    SHORT loc_1_D3A7
        cmp    byte ptr [eax + 1], 6
        jne    SHORT loc_1_D3BB
        mov    eax, dword ptr [eax + 2]
        cmp    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_D3BB
        mov    byte ptr [esi + 26h], 0
        sub    di, 6
loc_1_D3A7:
        or     di, di
        jne    SHORT loc_1_D3BB
        mov    dword ptr [edx + 7dh], 1
        mov    al, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_D3BB:
        mov    al, 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D3D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        add    ebx, 4bh
        cmp    dword ptr [esi + 8], 62h
        jae    SHORT loc_1_D3F8
        mov    dword ptr [esi + 8], 62h
        mov    dword ptr [esi + 0ch], 80bh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_D3F8:
        lea    edi, [esi + 14h]
        push   27h
        lea    eax, [ebx + 27h]
        push   eax
        push   edi
        call   sub_1_083C
        push   27h
        add    ebx, 4eh
        push   ebx
        add    edi, 27h
        push   edi
        call   sub_1_083C
        mov    dword ptr [esi + 0ch], 800h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D430:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        cld
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    ecx, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax + 7eh]
        push   eax
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        call   sub_1_F110
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_D475
        mov    eax, 802h
        pop    esi
        pop    ebp
        ret    0ch
loc_1_D475:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_D4AC
        mov    ax, word ptr [esi + 0ch]
        or     ax, ax
        je     SHORT loc_1_D4AC
        push   0
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 3
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 10h], 0
        mov    word ptr [esi + 0ch], 0
loc_1_D4AC:
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_1_D4E3
        mov    ax, word ptr [esi + 0eh]
        or     ax, ax
        je     SHORT loc_1_D4E3
        push   0
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 3
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 14h], 0
        mov    word ptr [esi + 0eh], 0
loc_1_D4E3:
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_D4F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_D510
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_D510:
        mov    bx, word ptr [ebp + 0ch]
        or     bx, bx
        je     NEAR PTR loc_1_D5B0
        mov    eax, ebx
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 3
        mov    dword ptr [ebp - 8], eax
        cmp    word ptr [esi + 0ch], bx
        je     SHORT loc_1_D56A
        push   0
        push   1
        push   esi
        call   sub_1_D460
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_D570
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D56A:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_D570:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    word ptr [esi + 0ch], bx
        sub    cx, cx
        cmp    bx, cx
        je     SHORT loc_1_D5B0
        mov    eax, 88h
        lea    ecx, [ecx]
loc_1_D5A0:
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 6], cl
        inc    cx
        add    dword ptr [ebp - 4], eax
        cmp    cx, bx
        jb     SHORT loc_1_D5A0
loc_1_D5B0:
        cmp    word ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_D6A7
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 0ffffh
        mov    eax, ebx
        shl    eax, 4
        add    eax, ebx
        shl    eax, 3
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ebp + 10h]
        cmp    word ptr [esi + 0eh], ax
        je     SHORT loc_1_D610
        push   1
        push   0
        push   esi
        call   sub_1_D460
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_D616
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_D610:
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
loc_1_D616:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [esi + 0eh], ax
        sub    cx, cx
        dec    ebx
        or     ebx, ebx
        jle    SHORT loc_1_D675
loc_1_D642:
        mov    edx, ecx
        and    edx, 0ffffh
        mov    eax, edx
        shl    edx, 4
        add    edx, eax
        shl    edx, 3
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [edx + eax + 6], cl
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        inc    cx
        lea    edx, [eax + 88h]
        mov    dword ptr [eax], edx
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    eax, ebx
        jl     SHORT loc_1_D642
loc_1_D675:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    edx, eax
        shl    eax, 4
        add    eax, edx
        lea    edx, [eax*8]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [edx + eax + 6], cl
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [edx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4], eax
        mov    byte ptr [esi + 8], 0ffh
        mov    byte ptr [esi + 9], 0ffh
loc_1_D6A7:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D6C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_1_D6E0
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_D6E0:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_D710
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   18h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_D753
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_D710:
        db  0C7h, 045h, 0FCh, 01Ch, 08Ch, 001h, 000h
        db  083h, 03Dh, 01Ch, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_D731
loc_1_D720:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [esi], eax
        je     SHORT loc_1_D731
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_D720
loc_1_D731:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_D747
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_D747:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
loc_1_D753:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    word ptr [eax + 0ah], 1
        push   10h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_D4F0
        cmp    eax, 800h
        je     SHORT loc_1_D7A0
        push   0
        push   18h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi], 0
        mov    eax, 802h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_D7A0:
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 01Ch, 08Ch, 001h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 01Ch, 08Ch, 001h, 000h
        mov    dword ptr [esi], eax
        mov    eax, 800h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D7D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        or     edx, edx
        je     SHORT loc_1_D83A
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     SHORT loc_1_D83A
        db  0BEh, 01Ch, 08Ch, 001h, 000h
        db  083h, 03Dh, 01Ch, 08Ch, 001h, 000h, 000h
        je     SHORT loc_1_D7FD
        nop
loc_1_D7F0:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_1_D7FD
        mov    esi, eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_D7F0
loc_1_D7FD:
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    SHORT loc_1_D810
        mov    eax, 802h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_D810:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [edx]
        mov    dword ptr [edx], 0
        push   1
        push   1
        push   esi
        call   sub_1_D460
        push   0
        push   18h
        push   esi
        call   sub_1_075C
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    4
loc_1_D83A:
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D850:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    esi, dword ptr [ebp + 8]
        lea    edx, [ebp - 48h]
        mov    ax, word ptr [esi + 0ah]
        and    eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, dword ptr [ecx + 20h]
        mov    ecx, eax
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ecx]
        and    eax, 0fh
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, eax
        lea    eax, [ecx + esi*4]
        mov    dword ptr [ebp - 0ch], eax
        cmp    byte ptr [ecx + 9], 6
        je     SHORT loc_1_D8A3
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_D8A3:
        mov    ecx, dword ptr [ebp - 4]
        test   word ptr [ecx + 6], 0ff3fh
        jne    NEAR PTR loc_1_DE24
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 24h], 28h
        jb     NEAR PTR loc_1_DE24
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0dh]
        and    al, 17h
        cmp    al, 10h
        je     SHORT loc_1_D8D7
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_D8D7:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [esi + 4]
        mov    ebx, dword ptr [esi]
        mov    edi, dword ptr [ecx + 0ch]
        cmp    dword ptr [ebx + 14h], edi
        jne    SHORT loc_1_D907
        mov    eax, dword ptr [ecx + 10h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_D907
        mov    cl, byte ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0ch]
        and    ecx, 0fh
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebx + ecx*4 + 8], eax
        je     NEAR PTR loc_1_D9AE
loc_1_D907:
        mov    dword ptr [ebp - 14h], ebx
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 14h], edi
        jne    SHORT loc_1_D92D
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 10h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_D92D
        mov    cl, byte ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0ch]
        and    ecx, 0fh
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebx + ecx*4 + 8], eax
        je     SHORT loc_1_D992
loc_1_D92D:
        cmp    esi, ebx
        jne    SHORT loc_1_D907
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 4], ecx
        mov    al, byte ptr [eax + 0ch]
        shr    al, 4
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp - 8]
        shl    esi, 2
        mov    dword ptr [ebp - 8], esi
loc_1_D954:
        mov    eax, dword ptr [ebp - 8]
        lea    edi, [ebx + 8]
        mov    esi, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ecx + 9], al
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [esi + 9], al
        mov    eax, 800h
        mov    ecx, dword ptr [ebp + 0ch]
        pop    edi
        mov    word ptr [ecx + 28h], 2f00h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_D992:
        cmp    esi, ebx
        jne    SHORT loc_1_D9A1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 4], ecx
        jmp    SHORT loc_1_D9AE
loc_1_D9A1:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi], ebx
loc_1_D9AE:
        mov    esi, dword ptr [ebp - 8]
        lea    esi, [ebx + esi*4 + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], esi
        mov    al, byte ptr [eax + 0ch]
        mov    byte ptr [ebp - 34h], al
        shr    al, 4
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp - 8]
        shl    esi, 2
        mov    dword ptr [ebp - 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 8]
        cmp    word ptr [ecx], ax
        jne    NEAR PTR loc_1_D954
        mov    ax, word ptr [ecx + 6]
        cmp    word ptr [ebx + 0eh], ax
        jne    NEAR PTR loc_1_D954
        mov    ax, word ptr [ebx + 10h]
        cmp    word ptr [ecx + 8], ax
        jne    NEAR PTR loc_1_D954
        mov    esi, dword ptr [ebp - 18h]
        mov    al, byte ptr [esi + 0ch]
        xor    al, byte ptr [ebp - 34h]
        test   al, 0f0h
        jne    NEAR PTR loc_1_D954
        cmp    dword ptr [ebp - 30h], 5
        jbe    SHORT loc_1_DA3B
        mov    esi, ecx
        add    esi, 14h
        lea    edi, [ebx + 1ch]
        mov    eax, dword ptr [ebp - 30h]
        lea    ecx, [eax*4 - 14h]
        shr    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_D954
loc_1_DA3B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0ch]
        mov    cl, al
        and    cl, 0f0h
        cmp    cl, 50h
        jbe    SHORT loc_1_DA7E
        mov    esi, dword ptr [ebp - 0ch]
        add    esi, 14h
        mov    edi, dword ptr [ebp - 18h]
        add    edi, 14h
        and    al, 0f3h
        shr    al, 2
        and    eax, 0ffh
        sub    eax, 14h
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_D954
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_D954
loc_1_DA7E:
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax + 0dh], 20h
        je     SHORT loc_1_DAD0
        mov    ax, word ptr [eax + 12h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        cmp    cx, 100h
        jae    SHORT loc_1_DAB6
        or     cx, cx
        je     SHORT loc_1_DAB6
        mov    byte ptr [ebp - 48h], cl
        lea    edx, [ebp - 47h]
        jmp    SHORT loc_1_DAC3
loc_1_DAB6:
        mov    byte ptr [ebp - 48h], 0
        mov    byte ptr [ebp - 46h], cl
        mov    byte ptr [ebp - 47h], ch
        lea    edx, [ebp - 45h]
loc_1_DAC3:
        mov    dword ptr [ebp - 10h], 1
        jmp    SHORT loc_1_DAE4
        db  08Dh, 064h, 024h, 000h
loc_1_DAD0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 12h]
        cmp    word ptr [esi + 12h], ax
        jne    NEAR PTR loc_1_D954
loc_1_DAE4:
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 18h]
        mov    cx, word ptr [eax + 0eh]
        mov    ax, word ptr [esi + 0eh]
        mov    word ptr [ebp - 2ch], ax
        mov    ah, cl
        mov    al, ch
        mov    ch, byte ptr [ebp - 2ch]
        mov    cl, byte ptr [ebp - 2bh]
        sub    ax, cx
        and    eax, 0ffffh
        or     eax, eax
        je     SHORT loc_1_DB28
        cmp    ax, 100h
        jb     SHORT loc_1_DB21
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 2], al
        mov    byte ptr [edx + 1], ah
        inc    edx
        add    edx, 2
        jmp    SHORT loc_1_DB24
loc_1_DB21:
        mov    byte ptr [edx], al
        inc    edx
loc_1_DB24:
        or     dword ptr [ebp - 10h], 2
loc_1_DB28:
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 18h]
        mov    edi, dword ptr [eax + 8]
        mov    eax, dword ptr [esi + 8]
        mov    ecx, edi
        and    ecx, 0ff00h
        mov    esi, edi
        shl    esi, 10h
        or     ecx, esi
        shl    ecx, 8
        mov    esi, edi
        and    esi, 0ff0000h
        shr    edi, 10h
        or     esi, edi
        shr    esi, 8
        or     ecx, esi
        mov    esi, eax
        and    esi, 0ff00h
        mov    edi, eax
        shl    edi, 10h
        or     esi, edi
        shl    esi, 8
        mov    edi, eax
        and    edi, 0ff0000h
        shr    eax, 10h
        or     edi, eax
        shr    edi, 8
        or     esi, edi
        sub    ecx, esi
        je     SHORT loc_1_DBA9
        cmp    ecx, 0ffffh
        ja     NEAR PTR loc_1_D954
        cmp    cx, 100h
        jb     SHORT loc_1_DBA2
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 2], cl
        mov    byte ptr [edx + 1], ch
        inc    edx
        add    edx, 2
        jmp    SHORT loc_1_DBA5
loc_1_DBA2:
        mov    byte ptr [edx], cl
        inc    edx
loc_1_DBA5:
        or     dword ptr [ebp - 10h], 4
loc_1_DBA9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 18h]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 4]
        mov    esi, edi
        mov    dword ptr [ebp - 20h], eax
        mov    eax, edi
        and    eax, 0ff00h
        shl    esi, 10h
        or     eax, esi
        shl    eax, 8
        mov    esi, edi
        and    esi, 0ff0000h
        shr    edi, 10h
        or     esi, edi
        shr    esi, 8
        or     eax, esi
        mov    esi, dword ptr [ebp - 20h]
        and    esi, 0ff00h
        mov    edi, dword ptr [ebp - 20h]
        shl    edi, 10h
        or     esi, edi
        mov    edi, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 1ch], edi
        and    dword ptr [ebp - 1ch], 0ff0000h
        mov    dword ptr [ebp - 28h], edi
        shl    esi, 8
        shr    dword ptr [ebp - 28h], 10h
        mov    edi, dword ptr [ebp - 1ch]
        or     edi, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 1ch], edi
        shr    dword ptr [ebp - 1ch], 8
        or     esi, dword ptr [ebp - 1ch]
        sub    eax, esi
        je     SHORT loc_1_DC3E
        cmp    eax, 0ffffh
        ja     NEAR PTR loc_1_D954
        cmp    ax, 100h
        jb     SHORT loc_1_DC37
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 2], al
        mov    byte ptr [edx + 1], ah
        inc    edx
        add    edx, 2
        jmp    SHORT loc_1_DC3A
loc_1_DC37:
        mov    byte ptr [edx], al
        inc    edx
loc_1_DC3A:
        or     dword ptr [ebp - 10h], 8
loc_1_DC3E:
        mov    esi, dword ptr [ebp - 10h]
        or     esi, esi
        je     SHORT loc_1_DC66
        cmp    esi, 8
        je     SHORT loc_1_DC99
        cmp    esi, 0bh
        je     NEAR PTR loc_1_D954
        cmp    esi, 0ch
        je     SHORT loc_1_DCC5
        cmp    esi, 0fh
        je     NEAR PTR loc_1_D954
        jmp    NEAR PTR loc_1_DCF6
loc_1_DC66:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 0ah]
        cmp    word ptr [ecx + 2], ax
        je     NEAR PTR loc_1_D954
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        cmp    ecx, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_D954
        jmp    SHORT loc_1_DCF6
loc_1_DC99:
        mov    cx, word ptr [ebx + 0ah]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        sub    esi, eax
        cmp    esi, dword ptr [ebp - 8]
        jne    SHORT loc_1_DCF6
        mov    dword ptr [ebp - 10h], 0fh
        jmp    SHORT loc_1_DCF3
loc_1_DCC5:
        cmp    ecx, eax
        jne    SHORT loc_1_DCF6
        mov    cx, word ptr [ebx + 0ah]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        sub    esi, eax
        cmp    esi, dword ptr [ebp - 8]
        jne    SHORT loc_1_DCF6
        mov    dword ptr [ebp - 10h], 0bh
loc_1_DCF3:
        lea    edx, [ebp - 48h]
loc_1_DCF6:
        mov    ecx, dword ptr [ebp - 4]
        mov    si, word ptr [ebx + 0ch]
        mov    ax, word ptr [ecx + 4]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    di, si
        and    edi, 0ffh
        shr    si, 8
        shl    edi, 8
        mov    eax, esi
        and    eax, 0ffffh
        or     edi, eax
        sub    ecx, edi
        cmp    ecx, 1
        je     SHORT loc_1_DD5A
        cmp    cx, 100h
        jae    SHORT loc_1_DD49
        or     cx, cx
        je     SHORT loc_1_DD49
        mov    byte ptr [edx], cl
        inc    edx
        jmp    SHORT loc_1_DD56
loc_1_DD49:
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 2], cl
        mov    byte ptr [edx + 1], ch
        inc    edx
        add    edx, 2
loc_1_DD56:
        or     dword ptr [ebp - 10h], 20h
loc_1_DD5A:
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax + 0dh], 8
        je     SHORT loc_1_DD67
        or     dword ptr [ebp - 10h], 10h
loc_1_DD67:
        mov    eax, dword ptr [ebp - 0ch]
        mov    cx, word ptr [eax + 10h]
        mov    ax, cx
        and    eax, 0ffh
        shl    eax, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 24h], ecx
        lea    edi, [ebx + 8]
        mov    esi, dword ptr [ebp - 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 3
        rep movsb
        lea    ecx, [ebp - 48h]
        sub    edx, ecx
        cmp    dword ptr [ebp - 38h], 0
        je     SHORT loc_1_DDC7
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebx + 6]
        cmp    byte ptr [esi + 9], cl
        jne    SHORT loc_1_DDC7
        lea    ecx, [edx + 3]
        sub    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        add    edi, ecx
        mov    cl, byte ptr [ebp - 10h]
        jmp    SHORT loc_1_DDE8
loc_1_DDC7:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebx + 6]
        mov    byte ptr [esi + 9], cl
        lea    ecx, [edx + 4]
        sub    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [ebp - 8]
        add    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        or     cl, 40h
        mov    byte ptr [edi], cl
        inc    edi
        mov    cl, byte ptr [ebx + 6]
loc_1_DDE8:
        mov    byte ptr [edi], cl
        inc    edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp - 8]
        sub    dword ptr [ecx + 24h], esi
        add    dword ptr [ecx + 20h], esi
        lea    esi, [ebp - 48h]
        mov    byte ptr [edi], ah
        mov    byte ptr [edi + 1], al
        inc    edi
        inc    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, 800h
        mov    word ptr [ecx + 28h], 2d00h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_DE24:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_DE30:
        push   ebp
        mov    ebp, esp
        sub    esp, 90h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 24h], 0
        je     NEAR PTR loc_1_E810
        xor    eax, eax
        mov    ax, word ptr [ecx + 28h]
        cmp    eax, 2100h
        je     NEAR PTR loc_1_E780
        cmp    eax, 2f00h
        je     NEAR PTR loc_1_E790
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebp - 30h], eax
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    edx, dword ptr [ebp - 30h]
        inc    edx
        test   al, 40h
        je     NEAR PTR loc_1_DF36
        mov    al, byte ptr [edx]
        mov    edi, dword ptr [ebp + 8]
        and    eax, 0ffff00ffh
        cmp    word ptr [edi + 0ch], ax
        jbe    NEAR PTR loc_1_E810
        and    word ptr [edi + 0ah], 0fffeh
        mov    al, byte ptr [edx]
        mov    byte ptr [edi + 8], al
        inc    edx
loc_1_DEA4:
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 8]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        shl    ecx, 3
        add    ecx, dword ptr [edi + 10h]
        mov    edi, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    ax, word ptr [edi + 4]
        cmp    ax, 28h
        jb     NEAR PTR loc_1_E810
        cmp    ax, 80h
        ja     NEAR PTR loc_1_E810
        mov    esi, ecx
        mov    al, byte ptr [esi + 8]
        add    esi, 8
        and    eax, 0fh
        shl    eax, 2
        mov    dword ptr [ebp - 2ch], eax
        mov    edi, ecx
        lea    eax, [eax + edi + 8]
        mov    dword ptr [ebp - 4], eax
        mov    bl, byte ptr [edx]
        and    ebx, 0ffff00ffh
        mov    ecx, ebx
        and    ecx, 0ffffh
        shl    ecx, 8
        sar    ecx, 8
        mov    al, byte ptr [edx + 1]
        shr    al, 8
        and    ax, 0ffh
        or     cx, ax
        shl    bx, 8
        or     bl, byte ptr [edx + 1]
        mov    eax, dword ptr [ebp - 4]
        mov    ch, bl
        mov    word ptr [eax + 10h], cx
        add    edx, 2
        test   byte ptr [ebp - 0ch], 10h
        je     SHORT loc_1_DF50
        mov    ecx, eax
        add    ecx, 0dh
        or     byte ptr [ecx], 8
        jmp    SHORT loc_1_DF59
loc_1_DF36:
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [edi + 0ah], 1
        je     NEAR PTR loc_1_DEA4
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_DF50:
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0dh
        and    byte ptr [ecx], 0f7h
loc_1_DF59:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0fh
        cmp    eax, 0bh
        je     NEAR PTR loc_1_E3B0
        cmp    eax, 0fh
        je     NEAR PTR loc_1_E4D3
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_DFE0
        or     byte ptr [ecx], 20h
        mov    al, byte ptr [edx]
        or     al, al
        jne    SHORT loc_1_DFC0
        mov    bl, byte ptr [edx + 1]
        and    ebx, 0ffff00ffh
        mov    ecx, ebx
        and    ecx, 0ffffh
        shl    ecx, 8
        sar    ecx, 8
        mov    al, byte ptr [edx + 2]
        shr    al, 8
        and    ax, 0ffh
        or     cx, ax
        shl    bx, 8
        or     bl, byte ptr [edx + 2]
        mov    eax, dword ptr [ebp - 4]
        mov    ch, bl
        mov    word ptr [eax + 12h], cx
        add    edx, 3
        jmp    SHORT loc_1_DFE3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_DFC0:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        shr    al, 8
        and    ax, 0ffh
        or     cx, ax
        mov    eax, dword ptr [ebp - 4]
        inc    edx
        mov    word ptr [eax + 12h], cx
        jmp    SHORT loc_1_DFE3
loc_1_DFE0:
        and    byte ptr [ecx], 0dfh
loc_1_DFE3:
        test   byte ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_E0D3
        mov    cl, byte ptr [edx]
        mov    eax, dword ptr [ebp - 4]
        or     cl, cl
        mov    di, word ptr [eax + 0eh]
        jne    NEAR PTR loc_1_E093
        mov    ecx, edi
        and    ecx, 0ffffh
        shl    ecx, 8
        mov    al, byte ptr [edx + 1]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 88h], ax
        mov    al, byte ptr [edx + 2]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 84h], ax
        mov    ax, di
        shr    ax, 8
        mov    ebx, eax
        and    ebx, 0ffffh
        xor    ebx, ecx
        and    ebx, 0ffh
        xor    ebx, ecx
        mov    ecx, dword ptr [ebp - 88h]
        and    ecx, 0ffffh
        shl    ecx, 8
        mov    eax, dword ptr [ebp - 84h]
        and    eax, 0ffffh
        or     ecx, eax
        add    ebx, ecx
        shr    ebx, 8
        and    bx, 0ffh
        mov    ax, word ptr [ebp - 88h]
        shl    ax, 8
        or     ax, word ptr [ebp - 84h]
        shl    ax, 8
        and    di, 0ff00h
        add    ax, di
        or     bx, ax
        mov    eax, dword ptr [ebp - 4]
        add    edx, 3
        mov    word ptr [eax + 0eh], bx
        jmp    SHORT loc_1_E0D3
loc_1_E093:
        mov    eax, edi
        and    eax, 0ffffh
        shl    eax, 8
        shr    di, 8
        mov    bl, cl
        and    ebx, 0ffff00ffh
        mov    ecx, edi
        and    ecx, 0ffffh
        xor    ecx, eax
        and    ecx, 0ffh
        xor    ecx, eax
        mov    eax, ebx
        and    eax, 0ffffh
        add    ecx, eax
        mov    cl, ch
        lea    eax, [edi + ebx]
        mov    ch, al
        mov    eax, dword ptr [ebp - 4]
        inc    edx
        mov    word ptr [eax + 0eh], cx
loc_1_E0D3:
        test   byte ptr [ebp - 0ch], 4
        je     NEAR PTR loc_1_E23E
        mov    cl, byte ptr [edx]
        or     cl, cl
        mov    eax, dword ptr [ebp - 4]
        jne    NEAR PTR loc_1_E1B5
        mov    ecx, dword ptr [eax + 8]
        mov    eax, ecx
        and    eax, 0ff0000h
        mov    edi, ecx
        shr    edi, 10h
        or     eax, edi
        shr    eax, 8
        mov    dword ptr [ebp - 80h], eax
        xor    eax, eax
        mov    al, byte ptr [edx + 1]
        mov    dword ptr [ebp - 48h], eax
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        mov    edi, ecx
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [ebp - 48h]
        shl    eax, 8
        or     eax, dword ptr [ebp - 44h]
        and    edi, 0ff00h
        mov    ebx, ecx
        shl    ebx, 10h
        or     edi, ebx
        shl    edi, 8
        or     edi, dword ptr [ebp - 80h]
        add    edi, eax
        shr    edi, 10h
        mov    ebx, ecx
        and    ebx, 0ffffff00h
        shl    ebx, 8
        or     ebx, dword ptr [ebp - 80h]
        add    ebx, eax
        and    ebx, 0ff0000h
        or     edi, ebx
        shr    edi, 8
        mov    ebx, ecx
        and    ebx, 0ff00ffffh
        shr    ebx, 10h
        mov    eax, ecx
        and    eax, 0ffff0000h
        or     ebx, eax
        mov    eax, dword ptr [ebp - 48h]
        mov    dword ptr [ebp - 28h], eax
        shl    dword ptr [ebp - 28h], 10h
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [ebp - 7ch], eax
        shl    dword ptr [ebp - 7ch], 8
        mov    eax, dword ptr [ebp - 28h]
        or     eax, dword ptr [ebp - 7ch]
        add    ebx, eax
        mov    dword ptr [ebp - 28h], eax
        and    ebx, 0ff0000h
        or     edi, ebx
        mov    eax, dword ptr [ebp - 48h]
        shl    eax, 20h
        mov    ebx, dword ptr [ebp - 44h]
        shl    ebx, 18h
        or     eax, ebx
        and    ecx, 0ff000000h
        add    eax, ecx
        and    eax, 0ff000000h
        or     edi, eax
        mov    eax, dword ptr [ebp - 4]
        add    edx, 3
        mov    dword ptr [eax + 8], edi
        jmp    NEAR PTR loc_1_E23E
loc_1_E1B5:
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 14h], eax
        shr    eax, 10h
        mov    dword ptr [ebp - 78h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ff0000h
        or     eax, dword ptr [ebp - 78h]
        shr    eax, 8
        mov    edi, ecx
        and    edi, 0ffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ff00h
        mov    ebx, dword ptr [ebp - 14h]
        shl    ebx, 10h
        or     ecx, ebx
        shl    ecx, 8
        or     ecx, eax
        add    ecx, edi
        shr    ecx, 10h
        mov    ebx, dword ptr [ebp - 14h]
        and    ebx, 0ffffff00h
        shl    ebx, 8
        or     ebx, eax
        add    ebx, edi
        and    ebx, 0ff0000h
        or     ecx, ebx
        shr    ecx, 8
        mov    eax, dword ptr [ebp - 14h]
        xor    eax, dword ptr [ebp - 78h]
        and    eax, 0ff00h
        xor    eax, dword ptr [ebp - 14h]
        mov    ebx, edi
        shl    ebx, 8
        add    eax, ebx
        and    eax, 0ff0000h
        or     ecx, eax
        shl    edi, 18h
        add    edi, dword ptr [ebp - 14h]
        and    edi, 0ff000000h
        mov    eax, dword ptr [ebp - 4]
        or     ecx, edi
        mov    dword ptr [eax + 8], ecx
        inc    edx
loc_1_E23E:
        test   byte ptr [ebp - 0ch], 8
        je     NEAR PTR loc_1_E5BB
        mov    cl, byte ptr [edx]
        or     cl, cl
        mov    eax, dword ptr [ebp - 4]
        jne    NEAR PTR loc_1_E320
        mov    ecx, dword ptr [eax + 4]
        mov    eax, ecx
        and    eax, 0ff0000h
        mov    edi, ecx
        shr    edi, 10h
        or     eax, edi
        shr    eax, 8
        mov    dword ptr [ebp - 74h], eax
        xor    eax, eax
        mov    al, byte ptr [edx + 1]
        mov    dword ptr [ebp - 40h], eax
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        mov    edi, ecx
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 40h]
        shl    eax, 8
        or     eax, dword ptr [ebp - 3ch]
        and    edi, 0ff00h
        mov    ebx, ecx
        shl    ebx, 10h
        or     edi, ebx
        shl    edi, 8
        or     edi, dword ptr [ebp - 74h]
        add    edi, eax
        shr    edi, 10h
        mov    ebx, ecx
        and    ebx, 0ffffff00h
        shl    ebx, 8
        or     ebx, dword ptr [ebp - 74h]
        add    ebx, eax
        and    ebx, 0ff0000h
        or     edi, ebx
        shr    edi, 8
        mov    ebx, ecx
        and    ebx, 0ff00ffffh
        shr    ebx, 10h
        mov    eax, ecx
        and    eax, 0ffff0000h
        or     ebx, eax
        mov    eax, dword ptr [ebp - 40h]
        mov    dword ptr [ebp - 24h], eax
        shl    dword ptr [ebp - 24h], 10h
        mov    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [ebp - 70h], eax
        shl    dword ptr [ebp - 70h], 8
        mov    eax, dword ptr [ebp - 24h]
        or     eax, dword ptr [ebp - 70h]
        add    ebx, eax
        mov    dword ptr [ebp - 24h], eax
        and    ebx, 0ff0000h
        or     edi, ebx
        mov    eax, dword ptr [ebp - 40h]
        shl    eax, 20h
        mov    ebx, dword ptr [ebp - 3ch]
        shl    ebx, 18h
        or     eax, ebx
        and    ecx, 0ff000000h
        add    eax, ecx
        and    eax, 0ff000000h
        or     edi, eax
        mov    eax, dword ptr [ebp - 4]
        add    edx, 3
        mov    dword ptr [eax + 4], edi
        jmp    NEAR PTR loc_1_E5BB
loc_1_E320:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        shr    eax, 10h
        mov    dword ptr [ebp - 6ch], eax
        mov    ebx, dword ptr [ebp - 10h]
        and    ebx, 0ff0000h
        or     ebx, eax
        shr    ebx, 8
        xor    eax, eax
        mov    al, cl
        mov    edi, dword ptr [ebp - 10h]
        and    edi, 0ff00h
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 10h
        or     edi, ecx
        shl    edi, 8
        or     edi, ebx
        add    edi, eax
        shr    edi, 10h
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffff00h
        shl    ecx, 8
        or     ecx, ebx
        add    ecx, eax
        and    ecx, 0ff0000h
        or     edi, ecx
        shr    edi, 8
        mov    ecx, dword ptr [ebp - 10h]
        xor    ecx, dword ptr [ebp - 6ch]
        and    ecx, 0ff00h
        xor    ecx, dword ptr [ebp - 10h]
        mov    ebx, eax
        shl    ebx, 8
        add    ecx, ebx
        and    ecx, 0ff0000h
        or     edi, ecx
        shl    eax, 18h
        add    eax, dword ptr [ebp - 10h]
        and    eax, 0ff000000h
        or     edi, eax
        mov    eax, dword ptr [ebp - 4]
        inc    edx
        mov    dword ptr [eax + 4], edi
        jmp    NEAR PTR loc_1_E5BB
        db  005h, 000h, 000h, 000h, 000h
loc_1_E3B0:
        mov    edi, dword ptr [ebp - 8]
        mov    ax, word ptr [edi + 0ah]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        sub    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [eax + 8]
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [ebp - 68h], eax
        mov    ebx, ecx
        and    ebx, 0ff0000h
        or     ebx, eax
        shr    ebx, 8
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 8
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 18h
        mov    dword ptr [ebp - 5ch], eax
        mov    edi, ecx
        and    edi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        or     edi, ebx
        add    edi, dword ptr [ebp - 18h]
        shr    edi, 10h
        mov    eax, ecx
        and    eax, 0ffffff00h
        shl    eax, 8
        or     eax, ebx
        add    eax, dword ptr [ebp - 18h]
        and    eax, 0ff0000h
        or     edi, eax
        shr    edi, 8
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 68h]
        and    eax, 0ff00h
        xor    eax, ecx
        add    eax, dword ptr [ebp - 60h]
        and    eax, 0ff0000h
        or     edi, eax
        add    ecx, dword ptr [ebp - 5ch]
        and    ecx, 0ff000000h
        mov    eax, dword ptr [ebp - 4]
        or     edi, ecx
        mov    dword ptr [eax + 8], edi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [ebp - 64h], eax
        mov    eax, ecx
        and    eax, 0ff0000h
        or     eax, dword ptr [ebp - 64h]
        shr    eax, 8
        mov    edi, ecx
        and    edi, 0ff00h
        mov    ebx, ecx
        shl    ebx, 10h
        or     edi, ebx
        shl    edi, 8
        or     edi, eax
        add    edi, dword ptr [ebp - 18h]
        shr    edi, 10h
        mov    ebx, ecx
        and    ebx, 0ffffff00h
        shl    ebx, 8
        or     ebx, eax
        add    ebx, dword ptr [ebp - 18h]
        and    ebx, 0ff0000h
        or     edi, ebx
        shr    edi, 8
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 64h]
        and    eax, 0ff00h
        xor    eax, ecx
        add    eax, dword ptr [ebp - 60h]
        and    eax, 0ff0000h
        or     edi, eax
        add    ecx, dword ptr [ebp - 5ch]
        and    ecx, 0ff000000h
        or     edi, ecx
        jmp    NEAR PTR loc_1_E5B5
loc_1_E4D3:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, ecx
        and    eax, 0ff0000h
        mov    edi, ecx
        shr    edi, 10h
        or     eax, edi
        shr    eax, 8
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 58h], eax
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebp - 38h], ax
        shr    ax, 8
        and    eax, 0ffffh
        mov    dword ptr [ebp - 34h], eax
        mov    bx, word ptr [ebp - 38h]
        and    ebx, 0ffh
        shl    ebx, 8
        or     ebx, eax
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        mov    edi, ecx
        mov    dword ptr [ebp - 1ch], eax
        and    edi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        or     edi, dword ptr [ebp - 58h]
        sub    edi, dword ptr [ebp - 1ch]
        add    edi, ebx
        shr    edi, 10h
        mov    eax, ecx
        and    eax, 0ffffff00h
        shl    eax, 8
        or     eax, dword ptr [ebp - 58h]
        sub    eax, dword ptr [ebp - 1ch]
        add    eax, ebx
        and    eax, 0ff0000h
        or     edi, eax
        shr    edi, 8
        mov    ebx, ecx
        and    ebx, 0ff00ffffh
        shr    ebx, 10h
        mov    eax, ecx
        and    eax, 0ffff0000h
        or     ebx, eax
        mov    eax, dword ptr [ebp - 38h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        shl    dword ptr [ebp - 20h], 10h
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 54h], eax
        shl    dword ptr [ebp - 54h], 8
        mov    eax, dword ptr [ebp - 20h]
        or     eax, dword ptr [ebp - 54h]
        add    ebx, eax
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 1ch]
        shl    eax, 8
        sub    ebx, eax
        and    ebx, 0ff0000h
        or     edi, ebx
        mov    eax, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 1ch]
        shl    eax, 18h
        and    ecx, 0ff000000h
        add    eax, ecx
        and    eax, 0ff000000h
        or     edi, eax
loc_1_E5B5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], edi
loc_1_E5BB:
        test   byte ptr [ebp - 0ch], 20h
        je     NEAR PTR loc_1_E6A0
        mov    cl, byte ptr [edx]
        or     cl, cl
        mov    edi, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_E658
        mov    cx, word ptr [edi + 0ch]
        mov    al, byte ptr [edx + 1]
        mov    edi, ecx
        and    edi, 0ffffh
        shl    edi, 8
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 50h], ax
        mov    al, byte ptr [edx + 2]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 4ch], ax
        mov    ax, cx
        shr    ax, 8
        and    eax, 0ffffh
        xor    eax, edi
        and    eax, 0ffh
        xor    eax, edi
        mov    edi, dword ptr [ebp - 50h]
        and    edi, 0ffffh
        shl    edi, 8
        mov    ebx, dword ptr [ebp - 4ch]
        and    ebx, 0ffffh
        or     edi, ebx
        add    eax, edi
        shr    eax, 8
        and    ax, 0ffh
        mov    di, word ptr [ebp - 50h]
        shl    di, 8
        or     di, word ptr [ebp - 4ch]
        shl    di, 8
        and    cx, 0ff00h
        add    di, cx
        or     ax, di
        mov    edi, dword ptr [ebp - 8]
        add    edx, 3
        mov    word ptr [edi + 0ch], ax
        jmp    NEAR PTR loc_1_E6E6
loc_1_E658:
        mov    ax, word ptr [edi + 0ch]
        mov    bl, cl
        mov    edi, eax
        and    edi, 0ffffh
        shl    edi, 8
        shr    ax, 8
        and    ebx, 0ffff00ffh
        mov    ecx, eax
        and    ecx, 0ffffh
        xor    ecx, edi
        and    ecx, 0ffh
        xor    ecx, edi
        mov    edi, ebx
        and    edi, 0ffffh
        add    ecx, edi
        mov    cl, ch
        add    ax, bx
        mov    edi, dword ptr [ebp - 8]
        mov    ch, al
        mov    word ptr [edi + 0ch], cx
        inc    edx
        jmp    SHORT loc_1_E6E6
loc_1_E6A0:
        mov    edi, dword ptr [ebp - 8]
        mov    ax, word ptr [edi + 0ch]
        mov    di, ax
        mov    ecx, eax
        and    ecx, 0ffffh
        shr    di, 8
        shl    ecx, 8
        mov    ebx, edi
        and    ebx, 0ffffh
        xor    ebx, ecx
        and    ebx, 0ffh
        xor    ebx, ecx
        inc    ebx
        shr    ebx, 8
        and    bx, 0ffh
        and    ax, 0ff00h
        add    ax, 100h
        mov    edi, dword ptr [ebp - 8]
        or     bx, ax
        mov    word ptr [edi + 0ch], bx
loc_1_E6E6:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ecx + 24h]
        sub    ebx, edx
        xor    eax, eax
        add    ebx, dword ptr [ebp - 30h]
        mov    ax, word ptr [edi + 4]
        add    ebx, eax
        sub    edx, eax
        mov    al, bh
        mov    ah, bl
        mov    word ptr [edi + 0ah], ax
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 0ch]
        sub    edi, edi
        mov    eax, dword ptr [ecx + 24h]
        sub    ebx, eax
        add    eax, ebx
        mov    dword ptr [ecx + 24h], eax
        sub    dword ptr [ecx + 20h], ebx
        mov    ecx, edx
        cmp    dword ptr [ebp - 2ch], edi
        je     SHORT loc_1_E748
        mov    eax, dword ptr [ebp - 2ch]
        inc    eax
        shr    eax, 1
loc_1_E73B:
        xor    esi, esi
        mov    si, word ptr [ecx]
        add    ecx, 2
        add    edi, esi
        dec    eax
        jne    SHORT loc_1_E73B
loc_1_E748:
        mov    eax, edi
        shr    eax, 10h
        and    edi, 0ffffh
        add    eax, edi
        mov    ecx, eax
        shr    ecx, 10h
        and    eax, 0ffffh
        add    eax, ecx
        not    ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 0ah], ax
        mov    word ptr [ecx + 28h], 2100h
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_E780:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_E790:
        mov    ecx, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    esi, dword ptr [ecx + 1ch]
        add    esi, dword ptr [ecx + 20h]
        mov    edi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 9]
        mov    dl, cl
        xor    eax, eax
        mov    ax, word ptr [edi + 0ch]
        cmp    eax, edx
        jbe    SHORT loc_1_E810
        mov    byte ptr [edi + 8], cl
        mov    eax, dword ptr [edi + 10h]
        mov    ecx, edx
        shl    ecx, 4
        add    ecx, edx
        lea    ebx, [eax + ecx*8]
        and    word ptr [edi + 0ah], 0fffeh
        mov    byte ptr [esi + 9], 6
        mov    dl, byte ptr [esi]
        and    edx, 0fh
        mov    al, byte ptr [esi + edx*4 + 0ch]
        shr    al, 4
        and    eax, 0ffh
        add    edx, eax
        shl    edx, 2
        lea    edi, [ebx + 8]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    word ptr [ebx + 12h], 0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ebx + 4], dx
        mov    eax, 800h
        mov    word ptr [ecx + 28h], 2100h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_E810:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 802h
        or     word ptr [edi + 0ah], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
