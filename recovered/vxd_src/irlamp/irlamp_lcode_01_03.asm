; irlamp_lcode_01_03.asm -- IRLAMP object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_AB18:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_AB32:
        mov    eax, dword ptr [ebp - 4]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_AB71
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_AB61
        jmp    NEAR PTR loc_1_AB71
loc_1_AB61:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_AB32
loc_1_AB71:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1F70
        add    esp, 8
        call   sub_1_A820
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_AB95:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        call   sub_1_A820
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 10h]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_ABBE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0eeh], 1
        db  0A1h, 000h, 000h, 000h, 000h
        inc    word ptr [eax + 167h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_ABDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0eeh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_ACA5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0eeh], 0
        db  0A1h, 000h, 000h, 000h, 000h
        dec    word ptr [eax + 167h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 167h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_ACA5
loc_1_AC2D:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 10h
        cmp    dword ptr [eax + 10h], ecx
        je     NEAR PTR loc_1_AC69
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AB95
        add    esp, 4
        jmp    NEAR PTR loc_1_AC2D
loc_1_AC69:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 8
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_1_ACA5
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AB18
        add    esp, 4
        jmp    NEAR PTR loc_1_AC69
loc_1_ACA5:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0E2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_ACCF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 1
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        test   eax, eax
        jle    NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 3ch
        jg     NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fh]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fh]
        and    eax, 0ffffh
        cmp    eax, 100h
        jg     NEAR PTR loc_1_AE84
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_AD81
loc_1_AD7E:
        inc    dword ptr [ebp - 8]
loc_1_AD81:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_AE68
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 21h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_ADCB
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 21h]
        and    eax, 0ffh
        cmp    eax, 3ch
        jle    NEAR PTR loc_1_ADD0
loc_1_ADCB:
        jmp    NEAR PTR loc_1_AE68
loc_1_ADD0:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_AE31
loc_1_ADF1:
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 27h]
        and    eax, 0ffffh
        cmp    eax, 400h
        jle    NEAR PTR loc_1_AE16
        mov    dword ptr [ebp - 4], 0
loc_1_AE16:
        jmp    NEAR PTR loc_1_AE54
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_AE20:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_AE54
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_AE31:
        cmp    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_1_AE54
        cmp    dword ptr [ebp - 10h], 2
        je     NEAR PTR loc_1_ADF1
        cmp    dword ptr [ebp - 10h], 3
        je     NEAR PTR loc_1_AE54
        jmp    NEAR PTR loc_1_AE20
loc_1_AE54:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_AE63
        jmp    NEAR PTR loc_1_AE68
loc_1_AE63:
        jmp    NEAR PTR loc_1_AD7E
loc_1_AE68:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_AE84
        mov    dword ptr [ebp - 0ch], 0
loc_1_AE84:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_AE8C
loc_1_AE8C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_AE94:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 3fh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0efh], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e6h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0efh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_AF3A
loc_1_AEDE:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0F1h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0F5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 2
        jmp    NEAR PTR loc_1_AF49
loc_1_AF07:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 0ffh
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 1
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0F1h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0F5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_AF49
loc_1_AF3A:
        cmp    dword ptr [ebp - 4], 4
        je     NEAR PTR loc_1_AEDE
        jmp    NEAR PTR loc_1_AF07
loc_1_AF49:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AF4E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AF59:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AF64:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
loc_1_AF74:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_B112
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f9h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B0B6
loc_1_AF98:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0eah], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 3ch
        ja     NEAR PTR loc_1_AFD2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 0
        jne    NEAR PTR loc_1_AFEB
loc_1_AFD2:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 1
        jmp    NEAR PTR loc_1_AFF5
loc_1_AFEB:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 3
loc_1_AFF5:
        jmp    NEAR PTR loc_1_B0E2
loc_1_AFFA:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0eah]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 0
        jne    NEAR PTR loc_1_B01D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 4
loc_1_B01D:
        jmp    NEAR PTR loc_1_B0E2
loc_1_B022:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0eah], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 3ch
        ja     NEAR PTR loc_1_B05C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 0
        jne    NEAR PTR loc_1_B075
loc_1_B05C:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 2
        jmp    NEAR PTR loc_1_B07F
loc_1_B075:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 5
loc_1_B07F:
        jmp    NEAR PTR loc_1_B0E2
loc_1_B084:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0eah]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0eah], 0
        jne    NEAR PTR loc_1_B0A7
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f9h], 0
loc_1_B0A7:
        jmp    NEAR PTR loc_1_B0E2
loc_1_B0AC:
        jmp    NEAR PTR loc_1_B112
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_B0B6:
        cmp    dword ptr [ebp - 8], 5
        ja     NEAR PTR loc_1_B0E2
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B0E2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0e6h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 10fh], al
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0e6h]
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_AF74
loc_1_B112:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B117:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0fah]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B1F0
loc_1_B136:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B218
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_B1B8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0efh]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 50fh], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fbh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 510h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 511h], ah
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 512h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 107h], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B27B
        add    esp, 4
        jmp    NEAR PTR loc_1_B1C4
loc_1_B1B8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AA92
        add    esp, 4
loc_1_B1C4:
        jmp    NEAR PTR loc_1_B213
loc_1_B1C9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B27B
        add    esp, 4
        jmp    NEAR PTR loc_1_B213
loc_1_B1DA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B397
        add    esp, 4
        jmp    NEAR PTR loc_1_B213
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B1F0:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_B136
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_B1C9
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_B1DA
        jmp    NEAR PTR loc_1_B213
loc_1_B213:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B218:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fbh], 0
loc_1_B23F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B5A8
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f0h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_B266
        jmp    NEAR PTR loc_1_B26E
loc_1_B266:
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B23F
loc_1_B26E:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B276
loc_1_B276:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B27B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 510h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 103h], 1
loc_1_B29C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B5A8
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 0ch], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f0h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_B2D6
        jmp    NEAR PTR loc_1_B386
loc_1_B2D6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 107h]
        add    eax, dword ptr [ebp - 0ch]
        cmp    eax, 40ah
        jbe    NEAR PTR loc_1_B2FE
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0fbh], eax
        jmp    NEAR PTR loc_1_B386
loc_1_B2FE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ffh]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0fbh]
        add    eax, 21h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 107h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    al, byte ptr [eax + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 107h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 107h]
        add    eax, dword ptr [ebp - 8]
        add    eax, 2
        push   eax
        call   sub_1_B413
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 107h], eax
        jmp    NEAR PTR loc_1_B29C
loc_1_B386:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B397
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B397:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 10bh]
        cmp    dword ptr [eax + 107h], ecx
        jbe    NEAR PTR loc_1_B3CB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0fah], 2
        jmp    NEAR PTR loc_1_B3FE
loc_1_B3CB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0f0h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_B3F4
        mov    byte ptr [ebp - 4], 80h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0fah], 0
        jmp    NEAR PTR loc_1_B3FE
loc_1_B3F4:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0fah], 1
loc_1_B3FE:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B83F
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B413:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_B4F5
loc_1_B43A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4], al
        jmp    NEAR PTR loc_1_B518
loc_1_B475:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        add    ecx, 3
        call   sub_1_0FF9
        jmp    NEAR PTR loc_1_B518
loc_1_B4B6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        add    ecx, 3
        call   sub_1_0FF9
        jmp    NEAR PTR loc_1_B518
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B4F5:
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_B43A
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_B475
        cmp    dword ptr [ebp - 4], 3
        je     NEAR PTR loc_1_B4B6
        jmp    NEAR PTR loc_1_B518
loc_1_B518:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B51D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_B577
loc_1_B539:
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_B59A
loc_1_B545:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_B59A
loc_1_B55C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_B59A
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B577:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_B539
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_B545
        cmp    dword ptr [ebp - 8], 3
        je     NEAR PTR loc_1_B55C
        jmp    NEAR PTR loc_1_B59A
loc_1_B59A:
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        jmp    NEAR PTR loc_1_B5A3
loc_1_B5A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B5A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 110h
        mov    dword ptr [ebp - 18h], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 110h]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 111h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0fbh], 0
        jne    NEAR PTR loc_1_B607
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_B613
loc_1_B607:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 0ch], eax
loc_1_B613:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_B6CA
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0fbh], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0fbh], 0
        jne    NEAR PTR loc_1_B651
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 1
        jmp    NEAR PTR loc_1_B6BE
loc_1_B651:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        push   eax
        call   sub_1_B73C
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ffh], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ffh], -1
        jne    NEAR PTR loc_1_B695
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0f0h], 2
        jmp    NEAR PTR loc_1_B6BE
loc_1_B695:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ffh]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0fbh]
        add    eax, 21h
        push   eax
        call   sub_1_B51D
        add    esp, 4
        jmp    NEAR PTR loc_1_B6C5
loc_1_B6BE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_B6C5
loc_1_B6C5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B6CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 8], eax
loc_1_B6DB:
        mov    eax, dword ptr [ebp + 8]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_B730
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 17h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_B7BE
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_B723
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B737
loc_1_B723:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 8], eax
        jmp    NEAR PTR loc_1_B6DB
loc_1_B730:
        xor    eax, eax
        jmp    NEAR PTR loc_1_B737
loc_1_B737:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B73C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_B754
loc_1_B751:
        inc    dword ptr [ebp - 4]
loc_1_B754:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fh]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_1_B7AF
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 21h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 22h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_B7BE
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_B7AA
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B7B9
loc_1_B7AA:
        jmp    NEAR PTR loc_1_B751
loc_1_B7AF:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_B7B9
loc_1_B7B9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B7BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp + 14h], eax
        jne    NEAR PTR loc_1_B832
loc_1_B7E1:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_B81F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + edx]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_B817
        jmp    NEAR PTR loc_1_B81F
loc_1_B817:
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B7E1
loc_1_B81F:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_1_B832
        mov    dword ptr [ebp - 8], 1
loc_1_B832:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_B83A
loc_1_B83A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B83F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 4bh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        add    eax, dword ptr [ebp + 8]
        add    eax, 50fh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 45h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 41h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_B91F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 10bh]
        cmp    dword ptr [eax + 107h], ecx
        ja     NEAR PTR loc_1_B8C8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50fh]
        and    eax, 0ffh
        or     al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 50fh], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 107h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 51h], ax
        jmp    NEAR PTR loc_1_B900
loc_1_B8C8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10bh]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 51h], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 10bh]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 107h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10bh]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 103h], eax
loc_1_B900:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 41h], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 42h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    NEAR PTR loc_1_B92B
loc_1_B91F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_B92B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_B930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 25h
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 20h], 0
        je     NEAR PTR loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 0
        je     NEAR PTR loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 31h
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 28h], 36ee80h
        jbe    NEAR PTR loc_1_B99B
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 36ee80h
loc_1_B99B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_BA40
loc_1_B9AE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2dh], 0
        je     NEAR PTR loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 35h], 0
        je     NEAR PTR loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dh]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 3ch
        jg     NEAR PTR loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dh]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 3ch
        jg     NEAR PTR loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 33h]
        and    eax, 0ffffh
        cmp    eax, 0bh
        jb     NEAR PTR loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 010h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0
loc_1_BA25:
        jmp    NEAR PTR loc_1_BA6F
loc_1_BA2A:
        mov    dword ptr [ebp - 4], 7
        jmp    NEAR PTR loc_1_BA6F
        db  0E9h, 034h, 000h, 000h, 000h, 0E9h, 02Fh, 000h, 000h, 000h
loc_1_BA40:
        dec    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 5
        ja     NEAR PTR loc_1_BA6F
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BA6F:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_BA77
loc_1_BA77:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_BA7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 163h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 2dh]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 2dh]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 1]
        inc    ecx
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 151h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 2dh]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 151h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 151h]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 2dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 151h]
        add    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 6]
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 151h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_BB3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    ax, word ptr [eax + 33h]
        and    eax, 0ffffh
        sub    eax, 9
        mov    dword ptr [ebp - 8], eax
loc_1_BB64:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_BD4C
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 15dh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_BCF8
loc_1_BB88:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 1]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C0A9
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_BBC1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 35h]
        mov    word ptr [eax], 0
loc_1_BBC1:
        jmp    NEAR PTR loc_1_BD44
loc_1_BBC6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 2
        jmp    NEAR PTR loc_1_BD44
loc_1_BBF9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 3
        jmp    NEAR PTR loc_1_BD44
loc_1_BC41:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 4
        jmp    NEAR PTR loc_1_BD44
loc_1_BC74:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 5
        jmp    NEAR PTR loc_1_BD44
loc_1_BCBC:
        jmp    NEAR PTR loc_1_BD44
loc_1_BCC1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 35h]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BD51
        add    esp, 10h
        jmp    NEAR PTR loc_1_BD44
        db  0E9h, 04Ch, 000h, 000h, 000h
loc_1_BCF8:
        cmp    dword ptr [ebp - 0ch], 10h
        ja     NEAR PTR loc_1_BCC1
        mov    eax, dword ptr [ebp - 0ch]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h
        db  004h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 005h
loc_1_BD44:
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_BB64
loc_1_BD4C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_BD51:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 15dh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_BFE3
loc_1_BD70:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    byte ptr [eax + 32h], 0
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_BDD5
loc_1_BD92:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
        jmp    NEAR PTR loc_1_BDF9
loc_1_BDA3:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 6
        jmp    NEAR PTR loc_1_BDF9
loc_1_BDB2:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0ah
        jmp    NEAR PTR loc_1_BDF9
loc_1_BDC1:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0dh
        jmp    NEAR PTR loc_1_BDF9
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_BDD5:
        cmp    dword ptr [ebp - 8], 3
        ja     NEAR PTR loc_1_BDF9
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BDF9:
        jmp    NEAR PTR loc_1_C027
loc_1_BDFE:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        shl    eax, 18h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 7
        jmp    NEAR PTR loc_1_C027
loc_1_BE1B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        shl    eax, 10h
        mov    ecx, dword ptr [ebp + 14h]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 8
        jmp    NEAR PTR loc_1_C027
loc_1_BE38:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 14h]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 9
        jmp    NEAR PTR loc_1_C027
loc_1_BE55:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
        jmp    NEAR PTR loc_1_C027
loc_1_BE71:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 1], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0bh
        jmp    NEAR PTR loc_1_C027
loc_1_BE8F:
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 1]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx + 1], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 159h], 0
        jmp    NEAR PTR loc_1_C027
loc_1_BEC5:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 159h], ecx
        jae    NEAR PTR loc_1_BEEF
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 159h]
        mov    edx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + edx + 3], al
        jmp    NEAR PTR loc_1_BEFC
loc_1_BEEF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    byte ptr [eax + 32h], 1
loc_1_BEFC:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 159h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 1]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 159h]
        jne    NEAR PTR loc_1_BF2C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
loc_1_BF2C:
        jmp    NEAR PTR loc_1_C027
loc_1_BF31:
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0eh
        jmp    NEAR PTR loc_1_C027
loc_1_BF49:
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 0fh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 159h], 0
        jmp    NEAR PTR loc_1_C027
loc_1_BF6E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 159h], ecx
        jae    NEAR PTR loc_1_BF98
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 159h]
        mov    edx, dword ptr [ebp + 14h]
        mov    byte ptr [ecx + edx + 3], al
        jmp    NEAR PTR loc_1_BFA5
loc_1_BF98:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    byte ptr [eax + 32h], 1
loc_1_BFA5:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 159h]
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 159h]
        jne    NEAR PTR loc_1_BFD4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
loc_1_BFD4:
        jmp    NEAR PTR loc_1_C027
        db  0E9h, 049h, 000h, 000h, 000h, 0E9h, 044h, 000h, 000h, 000h
loc_1_BFE3:
        sub    dword ptr [ebp - 4], 5
        cmp    dword ptr [ebp - 4], 0ah
        ja     NEAR PTR loc_1_C027
        mov    eax, dword ptr [ebp - 4]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C027:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C02C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 35h]
        inc    word ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        mov    eax, dword ptr [eax + 35h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    ecx, dword ptr [ecx + 35h]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_C08E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B33
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 10h
        jmp    NEAR PTR loc_1_C098
loc_1_C08E:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 5
loc_1_C098:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C103
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C0A9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 15eh]
        mov    byte ptr [ecx + 31h], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_C0F4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B33
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C103
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 15dh], 10h
        xor    eax, eax
        jmp    NEAR PTR loc_1_C0FE
loc_1_C0F4:
        mov    eax, 1
        jmp    NEAR PTR loc_1_C0FE
loc_1_C0FE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C103:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 15eh]
        call   dword ptr [eax + 1ch]
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 62h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_C144:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_C1B3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_C1B3
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        ja     NEAR PTR loc_1_C1B3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        ja     NEAR PTR loc_1_C1C2
loc_1_C1B3:
        mov    eax, 20h
        jmp    NEAR PTR loc_1_C1C9
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_C1C2:
        xor    eax, eax
        jmp    NEAR PTR loc_1_C1C9
loc_1_C1C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C1CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_C25A
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 15h]
        mov    dword ptr [ebp - 8], eax
loc_1_C1EA:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 15h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_C24E
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_C241
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_C241
        mov    eax, 24h
        jmp    NEAR PTR loc_1_C269
loc_1_C241:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_C1EA
loc_1_C24E:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_C261
loc_1_C25A:
        mov    dword ptr [ebp - 0ch], 20h
loc_1_C261:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_C269
loc_1_C269:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C26E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_C2F6
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_C2F6
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_C2F6
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        ja     NEAR PTR loc_1_C2F6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        jbe    NEAR PTR loc_1_C2F6
        xor    eax, eax
        jmp    NEAR PTR loc_1_C300
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_C2F6:
        mov    eax, 20h
        jmp    NEAR PTR loc_1_C300
loc_1_C300:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C305:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_C399
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_C399
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_C38D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_C399
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        ja     NEAR PTR loc_1_C399
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ccfh]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        db  003h, 005h, 000h, 000h, 000h, 000h
        add    eax, 0cd1h
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 10h]
        jbe    NEAR PTR loc_1_C399
loc_1_C38D:
        xor    eax, eax
        jmp    NEAR PTR loc_1_C3A3
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_C399:
        mov    eax, 20h
        jmp    NEAR PTR loc_1_C3A3
loc_1_C3A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C3A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_37A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_1_C3DF
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 34h]
loc_1_C3DF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8308
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C3FD
loc_1_C3FD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C402:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_37A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_1_C439
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 34h]
loc_1_C439:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C44B
loc_1_C44B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C450:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C4CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D022
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 0
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_C54D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   13h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 18h]
        add    esp, 0ch
loc_1_C54D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C552:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], 0
        je     NEAR PTR loc_1_C574
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_C574:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C5A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 15h
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF3E
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C5FF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 15h
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF87
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C64E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    byte ptr [eax + 3fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    byte ptr [ecx + 47h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    byte ptr [ecx + 48h], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 14h]
        mov    byte ptr [ecx + 49h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    byte ptr [eax + 4ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    byte ptr [eax + 44h], 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C6BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C144
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_C6E1
        mov    eax, 20h
        jmp    NEAR PTR loc_1_C790
loc_1_C6E1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_C70C
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C790
loc_1_C70C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C72A
        mov    eax, 29h
        jmp    NEAR PTR loc_1_C790
loc_1_C72A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 47h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 48h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 44h], 2
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_37A8
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_1_C788
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 34h]
loc_1_C788:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C790
loc_1_C790:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C795:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        cmp    eax, dword ptr [ebp + 0ch]
        je     NEAR PTR loc_1_C7D9
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0bh], al
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_1_C7D0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        inc    dword ptr [eax + 38h]
        jmp    NEAR PTR loc_1_C7D9
loc_1_C7D0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        dec    dword ptr [eax + 38h]
loc_1_C7D9:
        xor    eax, eax
        jmp    NEAR PTR loc_1_C7E0
loc_1_C7E0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C7E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 34h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_C84A
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 34h], 0
        jne    NEAR PTR loc_1_C84A
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 18h]
        add    esp, 0ch
loc_1_C84A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C84F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 34h], 0
        jbe    NEAR PTR loc_1_C87C
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 4
        mov    eax, 2
        jmp    NEAR PTR loc_1_C883
loc_1_C87C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_C883
loc_1_C883:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C888:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C8D5
loc_1_C8B8:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_C8FB
loc_1_C8C4:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_C8FB
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_C8D5:
        cmp    dword ptr [ebp - 14h], 1
        je     NEAR PTR loc_1_C8B8
        cmp    dword ptr [ebp - 14h], 8
        je     NEAR PTR loc_1_C8B8
        cmp    dword ptr [ebp - 14h], 101h
        je     NEAR PTR loc_1_C8C4
        jmp    NEAR PTR loc_1_C8FB
loc_1_C8FB:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_C94B
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_C923
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3fh], 3
loc_1_C923:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_8281
        add    esp, 14h
loc_1_C94B:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_C98B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
        cmp    dword ptr [ebp + 10h], 9
        jne    NEAR PTR loc_1_C98B
        mov    dword ptr [ebp - 4], 1
loc_1_C98B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_D14E
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C9A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 48h], 7d0h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 050h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_C9EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 8
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_1_CA62
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 10h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 44h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
loc_1_CA62:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_CA6A
loc_1_CA6A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CA6F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 10h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CAAF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 0ah
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 3
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 20h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CADF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 14h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 40h], 19h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        add    edi, 20h
        add    esi, 84h
        mov    ecx, 6
        rep movsd
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [eax + 20h], 2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ch], -1
        jne    NEAR PTR loc_1_CB47
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
loc_1_CB47:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 3
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0ah
        jne    NEAR PTR loc_1_CB75
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 20h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CD19
        add    esp, 4
loc_1_CB75:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CB7A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 14h
        jne    NEAR PTR loc_1_CBC1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 1eh
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 24h]
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 28h]
loc_1_CBC1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CBC6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 14h
        jne    NEAR PTR loc_1_CC12
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 28h
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 24h]
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 28h]
        jmp    NEAR PTR loc_1_CC2D
loc_1_CC12:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 1eh
        jne    NEAR PTR loc_1_CC2D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 28h
loc_1_CC2D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CC32:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 1eh
        jne    NEAR PTR loc_1_CC6A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 64h], 14h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 24h]
        db  0A1h, 000h, 000h, 000h, 000h
        dec    dword ptr [eax + 28h]
loc_1_CC6A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CC6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_C9EF
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_CC94
        mov    eax, 14h
        jmp    NEAR PTR loc_1_CD14
loc_1_CC94:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_2124
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_CCEC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CA6F
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CD14
loc_1_CCEC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CAAF
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CD14
loc_1_CD14:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CD19:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DC25
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_CD40:
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_CDEC
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 1ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_CDAF
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_CD9B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CF87
        add    esp, 4
loc_1_CD9B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C3A8
        add    esp, 4
        inc    dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_CDE1
loc_1_CDAF:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 20h], 0
        jne    NEAR PTR loc_1_CDE1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        push   3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
loc_1_CDE1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_CD40
loc_1_CDEC:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_CE02
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
loc_1_CE02:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CE07:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 7fh
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 6fh
        jg     NEAR PTR loc_1_CE46
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4 + 34h]
        mov    dword ptr [ebp - 4], eax
loc_1_CE46:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_CEB7
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 15h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_CE59:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 15h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_CEB7
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_CEAA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], ecx
        jne    NEAR PTR loc_1_CEAA
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_CEBE
loc_1_CEAA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_CE59
loc_1_CEB7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_CEBE
loc_1_CEBE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CEC3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_CECC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        je     NEAR PTR loc_1_CF13
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 1ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 21h]
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
        jmp    NEAR PTR loc_1_CECC
loc_1_CF13:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CF18:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        call   sub_1_387C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CBC6
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CF3E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 48h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        inc    dword ptr [eax + 3ch]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CF87:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_CFBF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 48h
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    byte ptr [eax + 65h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        inc    dword ptr [eax + 3ch]
loc_1_CFBF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_CFC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 29h]
        mov    dword ptr [ebp - 4], eax
loc_1_CFD6:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 29h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_D016
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 2ch
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D009
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_D01D
loc_1_D009:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_CFD6
loc_1_D016:
        xor    eax, eax
        jmp    NEAR PTR loc_1_D01D
loc_1_D01D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D022:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_D05B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        dec    dword ptr [eax + 3ch]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_D05B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        dec    dword ptr [eax + 38h]
loc_1_D05B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D060:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 14h
        jne    NEAR PTR loc_1_D09F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 3ch]
        cmp    dword ptr [eax + 38h], ecx
        jne    NEAR PTR loc_1_D09F
        cmp    dword ptr [ebp + 0ch], 1bh
        je     NEAR PTR loc_1_D0C7
loc_1_D09F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 1eh
        jne    NEAR PTR loc_1_D0DA
        cmp    dword ptr [ebp + 0ch], 2eh
        je     NEAR PTR loc_1_D0C7
        cmp    dword ptr [ebp + 0ch], 1bh
        jne    NEAR PTR loc_1_D0DA
loc_1_D0C7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_D0E4
loc_1_D0DA:
        mov    eax, 1
        jmp    NEAR PTR loc_1_D0E4
loc_1_D0E4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D0E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 1
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 20h], 0
        jne    NEAR PTR loc_1_D141
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 28h], 0
        jne    NEAR PTR loc_1_D141
        call   sub_1_7D98
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_D13A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    NEAR PTR loc_1_D141
loc_1_D13A:
        mov    dword ptr [ebp - 8], 0
loc_1_D141:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_D149
loc_1_D149:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D14E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 3ch], 0
        jne    NEAR PTR loc_1_D20C
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 65h], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    al, byte ptr [eax + 0b4h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D1B0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    al, byte ptr [eax + 0b2h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D1B0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_D1C1
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_D1C1
loc_1_D1B0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    NEAR PTR loc_1_D20C
loc_1_D1C1:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_D1E5
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 44h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CB7A
        add    esp, 4
        jmp    NEAR PTR loc_1_D20C
loc_1_D1E5:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_D1FD
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 44h], eax
        jmp    NEAR PTR loc_1_D20C
loc_1_D1FD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_D20C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D211:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 44h], ecx
        jne    NEAR PTR loc_1_D26D
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 64h]
        and    eax, 0ffh
        cmp    eax, 1eh
        jne    NEAR PTR loc_1_D24B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    NEAR PTR loc_1_D26D
loc_1_D24B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 65h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_D26D
        mov    eax, dword ptr [ebp + 8]
        add    eax, 48h
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_D26D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_D272:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 48h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 65h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_D2A3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CF18
        add    esp, 4
loc_1_D2A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D2A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0caah], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca8h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca9h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0cc7h], 0
        jne    NEAR PTR loc_1_D309
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax], 0
loc_1_D309:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_D318
loc_1_D315:
        inc    dword ptr [ebp - 0ch]
loc_1_D318:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_D342
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9dh], 0
        jmp    NEAR PTR loc_1_D315
loc_1_D342:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_D351
loc_1_D34E:
        inc    dword ptr [ebp - 0ch]
loc_1_D351:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_D386
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        jmp    NEAR PTR loc_1_D34E
loc_1_D386:
        call   sub_1_D4E0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 30h], 0
        call   sub_1_D8D4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D3A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_D3C6
loc_1_D3C3:
        inc    dword ptr [ebp - 8]
loc_1_D3C6:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_D402
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    NEAR PTR loc_1_D3FD
        jmp    NEAR PTR loc_1_D402
loc_1_D3FD:
        jmp    NEAR PTR loc_1_D3C3
loc_1_D402:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_D420
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_D42C
loc_1_D420:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_D433
loc_1_D42C:
        mov    dword ptr [ebp - 4], 0
loc_1_D433:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_D44E
        cmp    dword ptr [ebp - 8], 14h
        jb     NEAR PTR loc_1_D44E
        mov    dword ptr [ebp - 8], 13h
loc_1_D44E:
        cmp    dword ptr [ebp - 8], 14h
        jae    NEAR PTR loc_1_D4CC
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_D47F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0ch]
        lea    edi, [eax + ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 0ah
        rep movsd
        movsb
loc_1_D47F:
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9dh], 1
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_D4C7
        mov    eax, dword ptr [ebp - 0ch]
        inc    word ptr [eax]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca8h], 1
loc_1_D4C7:
        jmp    NEAR PTR loc_1_D4D3
loc_1_D4CC:
        mov    dword ptr [ebp - 8], 0ffffffffh
loc_1_D4D3:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_D4DB
loc_1_D4DB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D4E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 0ffffffffh
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_D51A
loc_1_D517:
        inc    dword ptr [ebp - 8]
loc_1_D51A:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_D67A
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*4 + 0b9dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D5A9
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        inc    byte ptr [ecx + eax*4 + 0b9ch]
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*4 + 0b9ch]
        and    eax, 0ffh
        cmp    eax, 2
        jl     NEAR PTR loc_1_D5A9
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9eh], 1
        cmp    dword ptr [ebp - 4], -1
        jne    NEAR PTR loc_1_D592
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_D592:
        mov    eax, dword ptr [ebp - 10h]
        dec    word ptr [eax]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca9h], 1
        jmp    NEAR PTR loc_1_D517
loc_1_D5A9:
        cmp    dword ptr [ebp - 4], -1
        je     NEAR PTR loc_1_D664
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 4]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 10h]
        lea    edi, [edx + ebx + 4]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4 + 0b9ch]
        mov    ecx, dword ptr [ebp - 4]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + ecx*4 + 0b9ch], eax
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9eh], 0
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9eh], 1
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_D62C
loc_1_D629:
        inc    dword ptr [ebp - 4]
loc_1_D62C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jb     NEAR PTR loc_1_D65F
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*4 + 0b9eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_D65A
        jmp    NEAR PTR loc_1_D65F
loc_1_D65A:
        jmp    NEAR PTR loc_1_D629
loc_1_D65F:
        jmp    NEAR PTR loc_1_D675
loc_1_D664:
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9eh], 0
loc_1_D675:
        jmp    NEAR PTR loc_1_D517
loc_1_D67A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_D67F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D6D6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca7h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_D6D1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0caah], 1
loc_1_D6D1:
        call   sub_1_D6DB
loc_1_D6D6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D6DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0cc7h], 0
        jbe    NEAR PTR loc_1_D77C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D77C
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cabh
        push   eax
        call   sub_1_15D7
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ccbh], 1
        push   8
        push   0
        push   -1
        call   sub_1_D781
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ccbh], 0
        cmp    dword ptr [ebp - 4], 17h
        jne    NEAR PTR loc_1_D770
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca7h], 1
        call   sub_1_DBB6
        jmp    NEAR PTR loc_1_D77C
loc_1_D770:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca7h], 0
loc_1_D77C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D781:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_D7CD
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_D7C6
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 4]
        add    esp, 8
loc_1_D7C6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_D880
loc_1_D7CD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D854
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0ccbh], 0
        je     NEAR PTR loc_1_D809
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
        jmp    NEAR PTR loc_1_D816
loc_1_D809:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        call   dword ptr [eax + 3eh]
        add    esp, 4
loc_1_D816:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1624
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_D848
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_D880
loc_1_D848:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 1
loc_1_D854:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_D876
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0dh
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 18h
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_D876:
        mov    eax, 2
        jmp    NEAR PTR loc_1_D880
loc_1_D880:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D885:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_D8C0
loc_1_D899:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca7h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_D8B6
        call   sub_1_D6DB
loc_1_D8B6:
        jmp    NEAR PTR loc_1_D8CF
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_D8C0:
        cmp    dword ptr [ebp - 4], 18h
        je     NEAR PTR loc_1_D899
        jmp    NEAR PTR loc_1_D8CF
loc_1_D8CF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D8D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca6h], 0
        jmp    NEAR PTR loc_1_D903
loc_1_D8FB:
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 30h]
loc_1_D903:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ecx + 30h]
        jbe    NEAR PTR loc_1_D9A4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + eax*4 + 0b9dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_D99F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ccbh], 1
        push   1
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 8]
        push   eax
        call   sub_1_D781
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ccbh], 0
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_1_D99A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca6h], 1
loc_1_D99A:
        jmp    NEAR PTR loc_1_D9A4
loc_1_D99F:
        jmp    NEAR PTR loc_1_D8FB
loc_1_D9A4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_D9A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 1f4h
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_D9ED
        db  0A1h, 000h, 000h, 000h, 000h
        inc    dword ptr [eax + 30h]
        jmp    NEAR PTR loc_1_DB0F
loc_1_D9ED:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_DA23
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [eax + ecx + 8], edx
        je     NEAR PTR loc_1_DAE3
loc_1_DA23:
        mov    eax, dword ptr [ebp - 0ch]
        dec    word ptr [eax]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0ca9h], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ecx + 30h]
        jbe    NEAR PTR loc_1_DADE
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ecx + 30h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [edx + 30h]
        mov    edx, ebx
        lea    ebx, [ebx + ebx*4]
        lea    ebx, [edx + ebx*8]
        mov    esi, dword ptr [ebp - 0ch]
        lea    edx, [eax + ecx + 4]
        lea    ecx, [ebx + esi + 4]
        call   sub_1_0FF9
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [edx + 30h]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + eax*4 + 0ba0h]
        lea    ecx, [esi + ebx*4 + 0b9ch]
        call   sub_1_0FF9
loc_1_DADE:
        jmp    NEAR PTR loc_1_DB0F
loc_1_DAE3:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9dh], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax*4 + 0b9ch], 0
loc_1_DB0F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DB14:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 0c3ch], 0
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca6h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_DB50
        call   sub_1_D9A9
        call   sub_1_D8D4
        jmp    NEAR PTR loc_1_DB55
loc_1_DB50:
        call   sub_1_D2A8
loc_1_DB55:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_DBB1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca8h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_DB9D
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ca9h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_DBA2
loc_1_DB9D:
        call   sub_1_DC87
loc_1_DBA2:
        call   sub_1_DCF1
        call   sub_1_7C61
        call   sub_1_DBB6
loc_1_DBB1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DBB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 0cc7h], 0
        jbe    NEAR PTR loc_1_DC20
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0c3ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_DC20
        call   sub_1_10B1
        mov    ecx, 37h
        sub    edx, edx
        div    ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 0cc7h]
        add    eax, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0cabh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 0cabh
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_DC20:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DC25:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        mov    dword ptr [ebp - 4], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 6], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DC87:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_DC9A:
        mov    eax, dword ptr [ebp - 0ch]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_DCEC
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 5
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        cmp    dword ptr [eax + 18h], 0
        je     NEAR PTR loc_1_DCE1
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 18h]
        add    esp, 4
loc_1_DCE1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_DC9A
loc_1_DCEC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DCF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_1_DCFA:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 18h
        cmp    dword ptr [eax + 18h], ecx
        je     NEAR PTR loc_1_DD6F
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 18h
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 0dh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 864h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1dh]
        call   dword ptr [eax + 4]
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_DCFA
loc_1_DD6F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_DD74:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_DD7C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_DD84:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_DDA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_1_DDB0:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 049h, 052h, 04Ch, 041h
        db  04Dh, 050h, 045h, 058h, 001h, 000h, 000h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_DE10:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_DE14:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h
loc_1_DE24:
        add    dword ptr [edx - 797a7b7dh], eax
        or     byte ptr [eax], al
loc_1_DE2C:
        add    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    byte ptr [eax], 1
        db  000h
loc_1_DE38:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_DE3C:
        pushad
        or     dword ptr [eax], eax
        add    byte ptr [eax + 25h], al
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [esi - 1f000000h], dl
        add    byte ptr [eax], al
        add    dl, al
        add    dword ptr [eax], eax
        add    dl, cl
        or     byte ptr [eax], al
        add    byte ptr [ecx + edx + 3d090000h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    edx, edi
        add    byte ptr [eax + eax], ah
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [100h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        or     al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        adc    al, 0
        add    al, 0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        sbb    eax, 3000600h
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], dh
        cmp    al, byte ptr [eax]
        or     al, 0
        push   es
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, dl
        add    ch, byte ptr [eax + 1]
        dec    eax
        add    byte ptr [eax + eax], ah
        pop    es
        add    byte ptr [eax + eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        mov    al, byte ptr [9002d005h]
        add    byte ptr [eax], cl
        push   cs
        add    byte ptr [edi], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 0bf63bedh], cl
        add    edx, edi
        add    byte ptr [eax + eax], bh
        sbb    dword ptr [eax], eax
        add    eax, 64000000h
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        nop
        add    dword ptr [eax], eax
        add    al, cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    eax, dword ptr [eax]
        add    byte ptr [eax - 5fffffffh], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + 40000000h], ah
        push   es
        add    byte ptr [eax], al
        and    byte ptr [ebx], al
        add    byte ptr [eax], al
        inc    eax
        add    dword ptr [eax], eax
        add    byte ptr [eax + 1], al
        add    byte ptr [eax], al
        cmp    byte ptr [ecx], cl
        add    byte ptr [eax], al
        pushfd
        add    al, 0
        add    al, bl
        add    dword ptr [eax], eax
        add    al, bl
        add    dword ptr [eax], eax
        add    al, al
        adc    al, byte ptr [eax]
        add    byte ptr [eax + 9], ah
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 0
        add    al, ah
        add    dword ptr [eax], eax
        add    byte ptr [eax + 70h], al
        sub    eax, 16800000h
        add    byte ptr [eax], al
        inc    eax
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        loope  loc_1_DF6F
loc_1_DF6F:
        add    byte ptr [eax + 70h], al
        sub    eax, 16800000h
        add    byte ptr [eax], al
        inc    eax
        or     eax, 86a00003h
        add    dword ptr [eax], eax
        inc    eax
        pushfd
        add    byte ptr [eax], al
        and    byte ptr [esi], cl
        add    byte ptr [eax], dh
        sbb    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
loc_1_DF94:
        xor    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 4000000bh], bh
        pop    ds
        add    byte ptr [eax], al
        db  0E0h, 02Eh
        add    byte ptr [eax], al
        cmp    byte ptr [esi], 0
        add    byte ptr [eax], ah
        dec    esi
        add    byte ptr [eax], al
        test   al, 61h
        add    byte ptr [eax], al
        xor    byte ptr [ebp], dh
        add    byte ptr [eax - 64h], al
        add    byte ptr [eax], al
loc_1_DFCC:
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        push   ds
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, cl
        add    dword ptr [eax], eax
        add    ah, bl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_DFE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E000:
        mov    byte ptr [eax], al
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_E094:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_E098:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E09C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0A8:
        and    byte ptr [eax], al
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
loc_1_E0C8:
        sbb    al, 0
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
loc_1_E0E4:
        dec    ecx
        db  072h, 04Ch
        dec    ebp
        push   eax
        and    byte ptr [ecx + 41h], cl
        push   ebx
        and    byte ptr [ebx + 6ch], al
        imul   esp, dword ptr [ebp + 6eh], 74h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_E100:
        and    byte ptr [eax], al
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
loc_1_E120:
        sbb    al, 0
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
loc_1_E13C:
        add    al, 0
        add    byte ptr [eax], al
loc_1_E140:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_E144:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp], sp
        add    byte ptr [ecx + 72h], cl
        dec    esp
        dec    ebp
        push   eax
        push   ebx
        db  075h, 070h
        db  070h, 06Fh
        db  072h, 074h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E15C:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 4eh], sp
        popad
        insd
        add    byte ptr gs:[eax], al
loc_1_E168:
        dec    ecx
        db  072h, 04Ch
        dec    ebp
        push   eax
        and    byte ptr [ecx + 41h], cl
        push   ebx
        and    byte ptr [ebx + 65h], dl
        db  072h, 076h
        jb     NEAR PTR loc_1_E179
loc_1_E179:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
