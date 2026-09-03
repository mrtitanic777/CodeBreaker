; el59x_lcode_01_02.asm -- EL59X object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_5478:
        push   ebp
        mov    eax, 803h
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 176h]
        mov    ecx, dword ptr [ebp + 10h]
        out    dx, ax
        mov    edx, dword ptr [esi + 172h]
        mov    edi, dword ptr [ebp + 0ch]
        in     eax, dx
        mov    edx, eax
        and    eax, 0ff8fffffh
        and    edx, 700000h
        shr    edx, 14h
        mov    dword ptr [ecx], edx
        mov    ecx, edi
        shl    ecx, 14h
        mov    ebx, dword ptr [esi + 172h]
        or     eax, ecx
        mov    edx, ebx
        out    dx, eax
        mov    ecx, dword ptr [esi + 176h]
        mov    eax, 804h
        mov    edx, ecx
        out    dx, ax
        lea    edx, [ebx + 0ah]
        in     ax, dx
        and    ax, 0ff37h
        cmp    edi, 1
        jne    SHORT loc_1_54DC
        or     al, 8
loc_1_54DC:
        test   edi, edi
        je     SHORT loc_1_54EA
        cmp    edi, 4
        je     SHORT loc_1_54EA
        cmp    edi, 5
        jne    SHORT loc_1_54F7
loc_1_54EA:
        or     al, 40h
        cmp    byte ptr [esi + 0b9h], 0
        jne    SHORT loc_1_54F7
        or     al, 80h
loc_1_54F7:
        out    dx, ax
        mov    edx, ecx
        cmp    edi, 3
        jne    SHORT loc_1_5507
        mov    eax, 1000h
        jmp    SHORT loc_1_550C
loc_1_5507:
        mov    eax, 0b800h
loc_1_550C:
        out    dx, ax
        push   0bb8h
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 807h
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5538:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0b8h
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        add    edx, 0b6h
        push   edi
        mov    dword ptr [ebp - 14h], eax
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [edx + 2e0h]
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [ebx + 192h]
        mov    ecx, dword ptr [edx + 2e4h]
        mov    dword ptr [ebp - 64h], ecx
        mov    dword ptr [ebp - 30h], eax
        mov    ecx, dword ptr [edx + 1ch]
        lea    eax, [ecx + ecx*4]
        add    eax, eax
        shr    eax, 5
        cmp    eax, 0ffh
        jb     SHORT loc_1_558D
        mov    eax, 0ffh
loc_1_558D:
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0b4h], eax
        mov    eax, 805h
        mov    esi, dword ptr [edx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 172h]
        lea    edx, [ecx + 0ah]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [ecx + 0ch]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    edx, esi
        mov    eax, 807h
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, 7840h
        out    dx, ax
        mov    eax, 7040h
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [esi + 118h], eax
        mov    dword ptr [ebp - 9ch], eax
        mov    dword ptr [esi + 114h], eax
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 18h], 400h
        mov    byte ptr [ebp - 2], al
loc_1_55FB:
        cmp    dword ptr [ebp - 18h], 20h
        jb     NEAR PTR loc_1_571A
        mov    byte ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 0a0h], 3
loc_1_5613:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    eax, 6841h
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 90h], eax
        mov    eax, dword ptr [esi + 2e0h]
        inc    eax
        mov    dword ptr [ebp - 94h], eax
        mov    eax, dword ptr [esi + 2e4h]
        inc    eax
        mov    dword ptr [ebp - 98h], eax
        pushaw
        mov    edi, dword ptr [ebp - 98h]
        mov    esi, dword ptr [ebp - 94h]
        mov    edx, dword ptr [ebp - 90h]
loc_1_566C:
        test   edi, 3
        je     SHORT loc_1_567A
        movsb
        dec    edx
        jne    SHORT loc_1_566C
        jmp    SHORT loc_1_568C
loc_1_567A:
        mov    ecx, edx
        shr    ecx, 2
        je     SHORT loc_1_568C
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        je     SHORT loc_1_568C
        rep movsb
loc_1_568C:
        popaw
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 1ah
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_56AD
        inc    dword ptr [ebp - 5ch]
        jmp    SHORT loc_1_56C7
loc_1_56AD:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 13ch], ecx
        jbe    SHORT loc_1_56C7
        mov    dword ptr [esi + 13ch], ecx
loc_1_56C7:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_56E7
        mov    dword ptr [esi + 140h], eax
loc_1_56E7:
        dec    dword ptr [ebp - 0a0h]
        jne    NEAR PTR loc_1_5613
        cmp    dword ptr [ebp - 5ch], 3
        je     SHORT loc_1_56FF
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_5710
loc_1_56FF:
        shr    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 5ch], 0
        inc    dword ptr [ebp - 9ch]
loc_1_5710:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_55FB
loc_1_571A:
        mov    cl, byte ptr [ebp - 9ch]
        mov    esi, dword ptr [ebp - 0ch]
        xor    edi, edi
        shr    dword ptr [esi + 13ch], cl
        mov    dword ptr [ebp - 18h], 100h
        xor    ecx, ecx
        mov    byte ptr [ebp - 2], cl
loc_1_5737:
        cmp    dword ptr [ebp - 18h], 8
        jb     NEAR PTR loc_1_580C
        mov    byte ptr [ebp - 14h], 0
        mov    ebx, 3
loc_1_574A:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        shr    esi, 3
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        test   esi, esi
        out    dx, ax
        je     SHORT loc_1_5792
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 0eh
loc_1_577F:
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        dec    esi
        jne    SHORT loc_1_577F
loc_1_5792:
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 1ah
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_57AF
        inc    edi
        jmp    SHORT loc_1_57C9
loc_1_57AF:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    edx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 134h], edx
        jbe    SHORT loc_1_57C9
        mov    dword ptr [esi + 134h], edx
loc_1_57C9:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_57E9
        mov    dword ptr [esi + 140h], eax
loc_1_57E9:
        dec    ebx
        jne    NEAR PTR loc_1_574A
        cmp    edi, 3
        je     SHORT loc_1_57FB
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_5802
loc_1_57FB:
        inc    ecx
        xor    edi, edi
        shr    dword ptr [ebp - 18h], 1
loc_1_5802:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5737
loc_1_580C:
        mov    esi, dword ptr [ebp - 0ch]
        xor    edi, edi
        shr    dword ptr [esi + 134h], cl
        mov    dword ptr [ebp - 18h], 100h
        xor    ecx, ecx
        mov    byte ptr [ebp - 2], cl
loc_1_5823:
        cmp    dword ptr [ebp - 18h], 8
        jb     NEAR PTR loc_1_58FA
        mov    byte ptr [ebp - 14h], 0
        mov    ebx, 3
loc_1_5836:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        shr    esi, 3
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        test   esi, esi
        out    dx, ax
        je     SHORT loc_1_5880
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 6
loc_1_586B:
        xor    eax, eax
        dec    esi
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        jne    SHORT loc_1_586B
loc_1_5880:
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 1ah
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_589D
        inc    edi
        jmp    SHORT loc_1_58B7
loc_1_589D:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    edx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 138h], edx
        jbe    SHORT loc_1_58B7
        mov    dword ptr [esi + 138h], edx
loc_1_58B7:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_58D7
        mov    dword ptr [esi + 140h], eax
loc_1_58D7:
        dec    ebx
        jne    NEAR PTR loc_1_5836
        cmp    edi, 3
        je     SHORT loc_1_58E9
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_58F0
loc_1_58E9:
        inc    ecx
        xor    edi, edi
        shr    dword ptr [ebp - 18h], 1
loc_1_58F0:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5823
loc_1_58FA:
        mov    esi, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    ebx, dword ptr [ebp - 0b8h]
        mov    dword ptr [ebp - 5ch], eax
        shr    dword ptr [esi + 134h], cl
        mov    dword ptr [ebp - 18h], 8000h
        mov    byte ptr [ebp - 2], al
loc_1_5918:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_59F1
        mov    byte ptr [ebp - 14h], 0
        mov    edi, 3
loc_1_592B:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 8ch], eax
        push   ecx
        mov    ecx, dword ptr [ebp - 8ch]
loc_1_595A:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_595A
        pop    ecx
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 1ah
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_597F
        inc    dword ptr [ebp - 5ch]
        jmp    SHORT loc_1_59A9
loc_1_597F:
        mov    eax, dword ptr [ebp - 14h]
        sub    dword ptr [ebp - 10h], eax
        jne    SHORT loc_1_598E
        mov    dword ptr [ebp - 10h], 1
loc_1_598E:
        mov    eax, dword ptr [ebp - 18h]
        sub    edx, edx
        div    dword ptr [ebp - 10h]
        mov    ebx, eax
        mov    esi, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 148h], ebx
        jbe    SHORT loc_1_59A9
        mov    dword ptr [esi + 148h], ebx
loc_1_59A9:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_59C9
        mov    dword ptr [esi + 140h], eax
loc_1_59C9:
        dec    edi
        jne    NEAR PTR loc_1_592B
        cmp    dword ptr [ebp - 5ch], 3
        je     SHORT loc_1_59DC
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_59E7
loc_1_59DC:
        shr    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 5ch], 0
loc_1_59E7:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5918
loc_1_59F1:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 804h
        mov    esi, dword ptr [edx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 6
        in     ax, dx
        or     ah, 10h
        out    dx, ax
        mov    edx, esi
        mov    eax, 807h
        out    dx, ax
        mov    eax, 997ah
        out    dx, ax
        mov    eax, 2800h
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 17ah]
        mov    edx, ecx
loc_1_5A38:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_5A38
        mov    edx, esi
        mov    eax, 2000h
        out    dx, ax
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_5A51:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_5A51
        mov    edx, esi
        mov    eax, 4800h
        mov    dword ptr [ebp - 18h], 400h
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [ebp - 9ch], eax
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 6ch], eax
        mov    byte ptr [ebp - 2], al
loc_1_5A79:
        cmp    dword ptr [ebp - 18h], 20h
        jb     NEAR PTR loc_1_5C42
        mov    dword ptr [ebp - 58h], 0
loc_1_5A8A:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    byte ptr [ebp - 14h], 0
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 172h]
        mov    eax, dword ptr [ebp - 18h]
        lea    edx, [edi + 10h]
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 64h]
        mov    edx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        lea    edx, [edi + 1ah]
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_5AEC
        inc    dword ptr [ebp - 5ch]
        jmp    SHORT loc_1_5B06
loc_1_5AEC:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 11ch], ecx
        jbe    SHORT loc_1_5B06
        mov    dword ptr [esi + 11ch], ecx
loc_1_5B06:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_5B26
        mov    dword ptr [esi + 140h], eax
loc_1_5B26:
        mov    ecx, 4e20h
loc_1_5B2B:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_1_5B4A
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 18h
        in     ax, dx
        and    eax, 1fffh
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_5B2B
loc_1_5B4A:
        test   ecx, ecx
        je     NEAR PTR loc_1_5D20
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    byte ptr [ebp - 14h], 0
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        push   edi
        mov    edi, dword ptr [ebp - 60h]
        mov    edx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep insd
        and    eax, 3
        mov    ecx, eax
        rep insb
        pop    edi
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 1ah
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_5BAD
        inc    dword ptr [ebp - 6ch]
        jmp    SHORT loc_1_5BC7
loc_1_5BAD:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 120h], ecx
        jbe    SHORT loc_1_5BC7
        mov    dword ptr [esi + 120h], ecx
loc_1_5BC7:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_5BE7
        mov    dword ptr [esi + 140h], eax
loc_1_5BE7:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 4000h
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 17ah]
loc_1_5C00:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_5C00
        inc    dword ptr [ebp - 58h]
        cmp    dword ptr [ebp - 58h], 3
        jb     NEAR PTR loc_1_5A8A
        cmp    dword ptr [ebp - 5ch], 3
        je     SHORT loc_1_5C26
        cmp    dword ptr [ebp - 6ch], 3
        je     SHORT loc_1_5C26
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_5C38
loc_1_5C26:
        xor    eax, eax
        inc    dword ptr [ebp - 9ch]
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 6ch], eax
        shr    dword ptr [ebp - 18h], 1
loc_1_5C38:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5A79
loc_1_5C42:
        mov    cl, byte ptr [ebp - 9ch]
        mov    esi, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    dword ptr [ebp - 9ch], eax
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 6ch], eax
        mov    byte ptr [ebp - 2], al
        shl    dword ptr [esi + 11ch], cl
        shl    dword ptr [esi + 120h], cl
        mov    dword ptr [ebp - 18h], 400h
loc_1_5C6F:
        mov    esi, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 1e8h], 3
        je     NEAR PTR loc_1_5ED0
        cmp    dword ptr [ebp - 18h], 20h
        jb     NEAR PTR loc_1_5ED0
        mov    dword ptr [ebp - 58h], 0
loc_1_5C90:
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    ecx, dword ptr [edx + 176h]
        mov    byte ptr [ebp - 14h], 0
        mov    edx, ecx
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [edx + 172h]
        lea    edx, [edi + 10h]
        out    dx, eax
        mov    edx, edi
        mov    eax, dword ptr [esi + 220h]
        out    dx, eax
        mov    ax, word ptr [ebp - 18h]
        lea    edx, [edi + 6]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0a001h
        out    dx, ax
        mov    esi, 64000h
        lea    edx, [edi + 0eh]
loc_1_5CEB:
        mov    dword ptr [ebp - 54h], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 54h]
loc_1_5CF2:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_5CF2
        pop    ecx
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_5D02
        dec    esi
        jne    SHORT loc_1_5CEB
loc_1_5D02:
        test   esi, esi
        je     NEAR PTR loc_1_5E95
        lea    edx, [edi + 1ah]
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_5D2E
        inc    dword ptr [ebp - 5ch]
        jmp    SHORT loc_1_5D48
loc_1_5D20:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_5D2E:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 124h], ecx
        jbe    SHORT loc_1_5D48
        mov    dword ptr [esi + 124h], ecx
loc_1_5D48:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_5D68
        mov    dword ptr [esi + 140h], eax
loc_1_5D68:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 1000h
        mov    ecx, 4e20h
        mov    edi, dword ptr [edx + 172h]
        lea    edx, [edi + 0ch]
        out    dx, ax
loc_1_5D80:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_1_5D96
        lea    edx, [edi + 18h]
        in     ax, dx
        and    eax, 1fffh
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_5D80
loc_1_5D96:
        test   ecx, ecx
        je     NEAR PTR loc_1_60CB
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 7040h
        mov    ecx, dword ptr [edx + 176h]
        mov    byte ptr [ebp - 14h], 0
        mov    edx, ecx
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [esi + 21ch]
        out    dx, eax
        mov    ax, word ptr [ebp - 18h]
        lea    edx, [edi + 6]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0a000h
        out    dx, ax
        mov    esi, 64000h
        lea    edx, [edi + 0eh]
loc_1_5DE9:
        mov    dword ptr [ebp - 50h], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 50h]
loc_1_5DF0:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_5DF0
        pop    ecx
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_5E00
        dec    esi
        jne    SHORT loc_1_5DE9
loc_1_5E00:
        test   esi, esi
        je     NEAR PTR loc_1_5E95
        lea    edx, [edi + 1ah]
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_5E1E
        inc    dword ptr [ebp - 6ch]
        jmp    SHORT loc_1_5E38
loc_1_5E1E:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 128h], ecx
        jbe    SHORT loc_1_5E38
        mov    dword ptr [esi + 128h], ecx
loc_1_5E38:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_5E58
        mov    dword ptr [esi + 140h], eax
loc_1_5E58:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 4000h
        mov    edx, dword ptr [edx + 172h]
        add    edx, 0ch
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 17ah]
loc_1_5E7F:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_5E7F
        inc    dword ptr [ebp - 58h]
        cmp    dword ptr [ebp - 58h], 3
        jb     NEAR PTR loc_1_5C90
        jmp    SHORT loc_1_5EA2
loc_1_5E95:
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1e8h], 3
loc_1_5EA2:
        cmp    dword ptr [ebp - 5ch], 3
        je     SHORT loc_1_5EB4
        cmp    dword ptr [ebp - 6ch], 3
        je     SHORT loc_1_5EB4
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_5EC6
loc_1_5EB4:
        xor    eax, eax
        inc    dword ptr [ebp - 9ch]
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 6ch], eax
        shr    dword ptr [ebp - 18h], 1
loc_1_5EC6:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5C6F
loc_1_5ED0:
        mov    cl, byte ptr [ebp - 9ch]
        mov    esi, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    dword ptr [ebp - 9ch], eax
        mov    dword ptr [ebp - 6ch], eax
        shl    dword ptr [esi + 124h], cl
        shl    dword ptr [esi + 128h], cl
        mov    dword ptr [ebp - 18h], 400h
        mov    byte ptr [ebp - 2], al
loc_1_5EFA:
        mov    esi, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 1e8h], 3
        je     NEAR PTR loc_1_619A
        cmp    dword ptr [ebp - 18h], 20h
        jb     NEAR PTR loc_1_619A
        mov    dword ptr [ebp - 58h], 0
loc_1_5F1B:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 172h]
        mov    dword ptr [ebp - 24h], eax
        add    eax, 10h
        mov    edx, eax
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 80h], eax
        mov    eax, dword ptr [ebp - 18h]
        out    dx, eax
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [esi + 220h]
        out    dx, eax
        lea    eax, [edx + 6]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        mov    edx, eax
        mov    dword ptr [ebp - 88h], eax
        mov    dword ptr [ebp - 84h], ecx
        mov    eax, ecx
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 64000h
        mov    eax, dword ptr [edx + 176h]
        mov    edx, eax
        mov    dword ptr [ebp - 68h], eax
        mov    eax, 0a001h
        out    dx, ax
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0eh
        mov    dword ptr [ebp - 2ch], eax
loc_1_5F81:
        mov    dword ptr [ebp - 4ch], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 4ch]
loc_1_5F88:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_5F88
        pop    ecx
        mov    edx, dword ptr [ebp - 2ch]
        in     ax, dx
        test   ax, 800h
        mov    di, ax
        je     SHORT loc_1_5F9F
        dec    esi
        jne    SHORT loc_1_5F81
loc_1_5F9F:
        test   esi, esi
        je     NEAR PTR loc_1_6166
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, 1000h
        add    edx, 0ch
        mov    ecx, 4e20h
        out    dx, ax
loc_1_5FB9:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_1_5FD2
        mov    edx, dword ptr [ebp - 24h]
        add    edx, 18h
        in     ax, dx
        and    eax, 1fffh
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_5FB9
loc_1_5FD2:
        test   ecx, ecx
        je     NEAR PTR loc_1_6380
        cli
        mov    edx, dword ptr [ebp - 68h]
        mov    eax, 7040h
        mov    byte ptr [ebp - 14h], 0
        out    dx, ax
        mov    eax, 6841h
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [esi + 21ch]
        out    dx, eax
        mov    edx, dword ptr [ebp - 88h]
        mov    eax, dword ptr [ebp - 84h]
        out    dx, ax
        mov    edx, dword ptr [ebp - 68h]
        mov    eax, 0a000h
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, dword ptr [ebp - 80h]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 34h], eax
        test   eax, eax
        out    dx, eax
        je     SHORT loc_1_607F
        shr    eax, 3
        mov    dword ptr [ebp - 3ch], eax
loc_1_6034:
        mov    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [ebp - 1ch], eax
        push   esi
        mov    esi, dword ptr [ebp - 64h]
        mov    edx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_6077
        mov    edx, dword ptr [ebp - 2ch]
        in     ax, dx
        test   ax, 800h
        mov    di, ax
        jne    SHORT loc_1_6077
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 1ah
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 10h], al
loc_1_6077:
        mov    eax, dword ptr [ebp - 3ch]
        sub    dword ptr [ebp - 34h], eax
        jne    SHORT loc_1_6034
loc_1_607F:
        mov    eax, dword ptr [ebp - 24h]
        test   di, 800h
        lea    esi, [eax + 1ah]
        mov    edx, esi
        in     al, dx
        je     SHORT loc_1_60BC
        mov    edi, 64000h
loc_1_6094:
        mov    dword ptr [ebp - 48h], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 48h]
loc_1_609B:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_609B
        pop    ecx
        mov    edx, dword ptr [ebp - 2ch]
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_60AE
        dec    edi
        jne    SHORT loc_1_6094
loc_1_60AE:
        test   edi, edi
        je     NEAR PTR loc_1_6166
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 10h], al
loc_1_60BC:
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_60D9
        inc    dword ptr [ebp - 6ch]
        jmp    SHORT loc_1_60F3
loc_1_60CB:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_60D9:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 130h], ecx
        jbe    SHORT loc_1_60F3
        mov    dword ptr [esi + 130h], ecx
loc_1_60F3:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_6113
        mov    dword ptr [esi + 140h], eax
loc_1_6113:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 4000h
        mov    edx, dword ptr [edx + 172h]
        add    edx, 0ch
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 17ah]
        mov    edx, ecx
loc_1_613E:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_613E
        mov    edx, esi
        mov    eax, 4000h
        out    dx, ax
        mov    edx, ecx
loc_1_6150:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_6150
        inc    dword ptr [ebp - 58h]
        cmp    dword ptr [ebp - 58h], 3
        jb     NEAR PTR loc_1_5F1B
        jmp    SHORT loc_1_6173
loc_1_6166:
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1e8h], 3
loc_1_6173:
        cmp    dword ptr [ebp - 6ch], 3
        je     SHORT loc_1_617F
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_6190
loc_1_617F:
        shr    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 6ch], 0
        inc    dword ptr [ebp - 9ch]
loc_1_6190:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_5EFA
loc_1_619A:
        mov    cl, byte ptr [ebp - 9ch]
        mov    esi, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    dword ptr [ebp - 9ch], eax
        mov    dword ptr [ebp - 5ch], eax
        shl    dword ptr [esi + 130h], cl
        mov    dword ptr [ebp - 18h], 400h
        mov    byte ptr [ebp - 2], al
loc_1_61BE:
        mov    esi, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 1e8h], 3
        je     NEAR PTR loc_1_6454
        cmp    dword ptr [ebp - 18h], 20h
        jb     NEAR PTR loc_1_6454
        mov    dword ptr [ebp - 58h], 0
loc_1_61DF:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 172h]
        mov    dword ptr [ebp - 20h], eax
        add    eax, 10h
        mov    edx, eax
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 7ch], eax
        mov    eax, dword ptr [ebp - 18h]
        out    dx, eax
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [esi + 220h]
        out    dx, eax
        lea    eax, [edx + 6]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        mov    edx, eax
        mov    dword ptr [ebp - 78h], eax
        mov    dword ptr [ebp - 74h], ecx
        mov    eax, ecx
        mov    esi, 64000h
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0a001h
        mov    edi, dword ptr [edx + 176h]
        mov    edx, edi
        out    dx, ax
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 0eh
        mov    dword ptr [ebp - 28h], eax
loc_1_623C:
        mov    dword ptr [ebp - 44h], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 44h]
loc_1_6243:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_6243
        pop    ecx
        mov    edx, dword ptr [ebp - 28h]
        in     ax, dx
        test   ah, 8
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_625A
        dec    esi
        jne    SHORT loc_1_623C
loc_1_625A:
        test   esi, esi
        je     NEAR PTR loc_1_6420
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, 1000h
        add    edx, 0ch
        mov    ecx, 4e20h
        out    dx, ax
loc_1_6274:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_1_628D
        mov    edx, dword ptr [ebp - 20h]
        add    edx, 18h
        in     ax, dx
        and    eax, 1fffh
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_6274
loc_1_628D:
        test   ecx, ecx
        je     NEAR PTR loc_1_6514
        cli
        mov    edx, edi
        mov    eax, 7040h
        mov    byte ptr [ebp - 14h], 0
        out    dx, ax
        mov    eax, 6841h
        out    dx, ax
        mov    eax, 6000h
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    edx, dword ptr [ebp - 7ch]
        mov    eax, dword ptr [ebp - 18h]
        out    dx, eax
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [esi + 220h]
        out    dx, eax
        mov    edx, dword ptr [ebp - 78h]
        mov    eax, dword ptr [ebp - 74h]
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, edi
        mov    eax, 0a001h
        out    dx, ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 34h], eax
        test   eax, eax
        je     SHORT loc_1_6335
        shr    eax, 3
        mov    dword ptr [ebp - 38h], eax
loc_1_62EA:
        mov    eax, dword ptr [ebp - 38h]
        mov    dword ptr [ebp - 1ch], eax
        push   edi
        mov    edi, dword ptr [ebp - 60h]
        mov    edx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep insd
        and    eax, 3
        mov    ecx, eax
        rep insb
        pop    edi
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_632D
        mov    edx, dword ptr [ebp - 28h]
        in     ax, dx
        test   ah, 8
        mov    word ptr [ebp - 4], ax
        jne    SHORT loc_1_632D
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 1ah
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 10h], al
loc_1_632D:
        mov    eax, dword ptr [ebp - 38h]
        sub    dword ptr [ebp - 34h], eax
        jne    SHORT loc_1_62EA
loc_1_6335:
        mov    eax, dword ptr [ebp - 20h]
        test   byte ptr [ebp - 3], 8
        lea    esi, [eax + 1ah]
        mov    edx, esi
        in     al, dx
        je     SHORT loc_1_6371
        mov    edi, 64000h
loc_1_6349:
        mov    dword ptr [ebp - 40h], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 40h]
loc_1_6350:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_6350
        pop    ecx
        mov    edx, dword ptr [ebp - 28h]
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_6363
        dec    edi
        jne    SHORT loc_1_6349
loc_1_6363:
        test   edi, edi
        je     NEAR PTR loc_1_6420
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 10h], al
loc_1_6371:
        sti
        cmp    dword ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_638E
        inc    dword ptr [ebp - 5ch]
        jmp    SHORT loc_1_63A8
loc_1_6380:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_638E:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 12ch], ecx
        jbe    SHORT loc_1_63A8
        mov    dword ptr [esi + 12ch], ecx
loc_1_63A8:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 144h], eax
        inc    dword ptr [esi + 14ch]
        cmp    dword ptr [esi + 140h], eax
        jbe    SHORT loc_1_63C8
        mov    dword ptr [esi + 140h], eax
loc_1_63C8:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 1000h
        mov    edx, dword ptr [edx + 172h]
        add    edx, 0ch
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 4000h
        mov    esi, dword ptr [edx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 17ah]
        mov    edx, ecx
loc_1_63F8:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_63F8
        mov    edx, esi
        mov    eax, 4000h
        out    dx, ax
        mov    edx, ecx
loc_1_640A:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_640A
        inc    dword ptr [ebp - 58h]
        cmp    dword ptr [ebp - 58h], 3
        jb     NEAR PTR loc_1_61DF
        jmp    SHORT loc_1_642D
loc_1_6420:
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1e8h], 3
loc_1_642D:
        cmp    dword ptr [ebp - 5ch], 3
        je     SHORT loc_1_6439
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_644A
loc_1_6439:
        shr    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 5ch], 0
        inc    dword ptr [ebp - 9ch]
loc_1_644A:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_61BE
loc_1_6454:
        mov    cl, byte ptr [ebp - 9ch]
        mov    esi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 2800h
        shl    dword ptr [esi + 12ch], cl
        mov    edi, dword ptr [edx + 176h]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 17ah]
        mov    edx, ecx
loc_1_6480:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_6480
        mov    edx, edi
        mov    eax, 2000h
        out    dx, ax
        mov    esi, dword ptr [ebp - 0ch]
        mov    ax, word ptr [esi + 4ch]
        out    dx, ax
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_64A2:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_64A2
        mov    edx, edi
        mov    eax, 4800h
        out    dx, ax
        mov    eax, 804h
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 6
        mov    esi, dword ptr [ebp - 0ch]
        in     ax, dx
        and    ax, 0efffh
        mov    ecx, dword ptr [esi + 11ch]
        out    dx, ax
        mov    edx, edi
        mov    eax, 807h
        out    dx, ax
        mov    eax, dword ptr [ebp - 0b4h]
        sub    edx, edx
        shl    eax, 0ah
        mov    dword ptr [ebp - 70h], ecx
        div    ecx
        and    eax, 0fffffffch
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [esi + 124h]
        cmp    dword ptr [esi + 11ch], eax
        ja     SHORT loc_1_6522
        lea    edi, [esi + 234h]
        mov    dword ptr [edi], 7fffh
        jmp    NEAR PTR loc_1_65B3
loc_1_6514:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_6522:
        mov    edi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edi + 234h], 80h
        add    edi, 234h
        cmp    dword ptr [edx + 142h], 0
        jne    SHORT loc_1_65B3
        mov    ebx, dword ptr [edx + 196h]
        cmp    ebx, 0ffh
        je     SHORT loc_1_65B3
        mov    esi, dword ptr [edx + 176h]
        mov    eax, 804h
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 172h]
        add    edx, 4
        in     ax, dx
        mov    ecx, eax
        mov    edx, esi
        and    ecx, 0ffffh
        mov    eax, 807h
        mov    esi, dword ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, dword ptr [esi + 124h]
        shl    eax, 3
        mov    dword ptr [ebp - 70h], eax
        mov    eax, 0f8h
        cmp    dword ptr [ebp - 70h], eax
        jbe    SHORT loc_1_6599
        mov    dword ptr [ebp - 70h], eax
loc_1_6599:
        and    cl, 3eh
        cmp    cl, 1
        ja     SHORT loc_1_65B3
        push   1
        lea    eax, [ebp - 70h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   0dh
        push   ebx
        push   edx
        call   sub_1_8A78
loc_1_65B3:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 100h]
        cmp    eax, 0ffffh
        je     SHORT loc_1_65C5
        mov    dword ptr [edi], eax
loc_1_65C5:
        cmp    dword ptr [edi], 20h
        jae    SHORT loc_1_65D0
        mov    dword ptr [edi], 20h
loc_1_65D0:
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 0ch]
        add    edi, 16ch
loc_1_65DB:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 0f8h]
        cmp    eax, 0ffffh
        je     SHORT loc_1_65EE
        mov    word ptr [edi], ax
loc_1_65EE:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 11ch]
        mov    dword ptr [ebp - 0b0h], eax
        mov    eax, dword ptr [esi + 110h]
        cmp    dword ptr [ebp - 0b0h], eax
        jbe    NEAR PTR loc_1_6691
        mov    ebx, ecx
        shl    ebx, 9
        lea    esi, [ebx - 100h]
        add    ebx, 100h
        test   esi, esi
        jge    SHORT loc_1_6626
        xor    esi, esi
loc_1_6626:
        sub    edx, edx
        mov    dword ptr [ebp - 0ach], esi
        imul   eax, esi
        div    dword ptr [ebp - 0b0h]
        sub    dword ptr [ebp - 0ach], eax
        xor    eax, eax
        mov    edx, dword ptr [ebp - 0ach]
        mov    dword ptr [ebp - 70h], edx
        mov    ax, word ptr [edi]
        cmp    eax, edx
        ja     SHORT loc_1_6651
        mov    eax, edx
loc_1_6651:
        mov    word ptr [edi], ax
        lea    eax, [esi + ebx]
        cdq
        sub    eax, edx
        mov    esi, dword ptr [ebp - 0ch]
        sar    eax, 1
        sub    edx, edx
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [esi + 110h]
        imul   eax, dword ptr [ebp - 70h]
        div    dword ptr [esi + 11ch]
        sub    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 70h]
        shr    eax, 2
        add    dword ptr [ebp - 70h], eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        mov    edx, dword ptr [ebp - 70h]
        cmp    eax, edx
        ja     SHORT loc_1_669E
        mov    eax, edx
        jmp    SHORT loc_1_669E
loc_1_6691:
        mov    ax, word ptr [edi]
        cmp    ax, 20h
        ja     SHORT loc_1_669E
        mov    ax, 20h
loc_1_669E:
        mov    word ptr [edi - 2], ax
        mov    ax, word ptr [edi - 2]
        mov    dx, word ptr [edi]
        cmp    ax, dx
        ja     SHORT loc_1_66B1
        mov    ax, dx
loc_1_66B1:
        mov    word ptr [edi - 2], ax
        add    edi, 10h
        shr    ax, 2
        inc    ecx
        sub    ax, 6800h
        cmp    ecx, 4
        mov    word ptr [edi - 14h], ax
        jb     NEAR PTR loc_1_65DB
        mov    dword ptr [ebp - 0a4h], 0
        mov    ecx, dword ptr [ebp - 0ch]
        add    ecx, 1ach
loc_1_66E1:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 0f8h]
        cmp    eax, 0ffffh
        je     SHORT loc_1_66F4
        mov    word ptr [ecx], ax
loc_1_66F4:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 110h]
        mov    edx, dword ptr [esi + 124h]
        mov    dword ptr [ebp - 0a8h], edx
        cmp    eax, edx
        jae    SHORT loc_1_6787
        mov    edi, dword ptr [ebp - 0a4h]
        shl    edi, 9
        lea    esi, [edi - 100h]
        add    edi, 100h
        test   esi, esi
        jge    SHORT loc_1_6728
        xor    esi, esi
loc_1_6728:
        mov    ebx, esi
        sub    edx, edx
        imul   eax, ebx
        div    dword ptr [ebp - 0a8h]
        sub    ebx, eax
        xor    eax, eax
        mov    edx, ebx
        mov    dword ptr [ebp - 70h], ebx
        mov    ax, word ptr [ecx]
        cmp    eax, edx
        ja     SHORT loc_1_6747
        mov    eax, edx
loc_1_6747:
        mov    word ptr [ecx], ax
        lea    eax, [esi + edi]
        cdq
        sub    eax, edx
        mov    esi, dword ptr [ebp - 0ch]
        sar    eax, 1
        sub    edx, edx
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [esi + 110h]
        imul   eax, dword ptr [ebp - 70h]
        div    dword ptr [esi + 124h]
        sub    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 70h]
        shr    eax, 2
        add    dword ptr [ebp - 70h], eax
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp - 70h]
        cmp    eax, edx
        ja     SHORT loc_1_679B
        mov    eax, edx
        jmp    SHORT loc_1_679B
loc_1_6787:
        mov    dword ptr [ebp - 70h], 20h
        mov    ax, word ptr [ecx]
        cmp    ax, 20h
        ja     SHORT loc_1_679B
        mov    ax, 20h
loc_1_679B:
        mov    word ptr [ecx - 2], ax
        mov    ax, word ptr [ecx - 2]
        mov    dx, word ptr [ecx]
        cmp    ax, dx
        ja     SHORT loc_1_67AE
        mov    ax, dx
loc_1_67AE:
        mov    word ptr [ecx - 2], ax
        add    ecx, 10h
        shr    ax, 2
        inc    dword ptr [ebp - 0a4h]
        sub    ax, 6800h
        cmp    dword ptr [ebp - 0a4h], 4
        mov    word ptr [ecx - 14h], ax
        jb     NEAR PTR loc_1_66E1
        mov    esi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 128h]
        mov    edi, dword ptr [esi + 120h]
        cmp    edx, edi
        jb     SHORT loc_1_67F5
        lea    ecx, [esi + 238h]
        mov    dword ptr [ecx], 7fffh
        jmp    SHORT loc_1_6815
loc_1_67F5:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 0ch]
        add    ecx, 238h
        sub    edi, edx
        mov    eax, dword ptr [esi + 138h]
        sub    edx, edx
        lea    eax, [eax + eax*2]
        shl    eax, 2
        div    edi
        mov    dword ptr [ecx], eax
loc_1_6815:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 120h]
        mov    dword ptr [ebp - 70h], eax
        cmp    dword ptr [esi + 0a4h], 5f5e100h
        jne    SHORT loc_1_684B
        mov    eax, dword ptr [esi + 120h]
        lea    eax, [eax + eax*4]
        add    eax, eax
        shr    eax, 2
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 128h]
        lea    eax, [eax + eax*4]
        add    eax, eax
        jmp    SHORT loc_1_6860
loc_1_684B:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 120h]
        shr    eax, 2
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 128h]
loc_1_6860:
        shr    eax, 2
        mov    dword ptr [esi + 28h], eax
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 104h]
        cmp    eax, 0ffffh
        je     SHORT loc_1_6878
        mov    dword ptr [ecx], eax
loc_1_6878:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 0ffffh
        je     SHORT loc_1_688B
        mov    dword ptr [esi + 30h], eax
loc_1_688B:
        mov    esi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [esi + 108h]
        test   eax, eax
        je     SHORT loc_1_68AA
        mov    dword ptr [esi + 160h], eax
        lea    eax, [eax + eax*2]
        shr    eax, 1
        mov    dword ptr [esi + 164h], eax
loc_1_68AA:
        mov    ax, word ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        add    ax, 7800h
        mov    edx, dword ptr [edx + 176h]
        out    dx, ax
        mov    ax, word ptr [ebp - 6]
        pop    edi
        add    ax, 7000h
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_68D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 7840h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0b6h]
        mov    edx, dword ptr [edi + 176h]
        out    dx, ax
        mov    eax, 6000h
        xor    ebx, ebx
        mov    byte ptr [esi + 9], 0
        mov    edx, dword ptr [edi + 176h]
        out    dx, ax
loc_1_690A:
        cmp    byte ptr [esi + 9], 0
        jne    SHORT loc_1_693D
        mov    edx, dword ptr [edi + 17ah]
        in     ax, dx
        test   al, 1
        je     SHORT loc_1_6939
        mov    edx, dword ptr [edi + 182h]
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_1_693D
        push   1
        inc    ebx
        call   sub_1_8F80
        cmp    ebx, 1388h
        jb     SHORT loc_1_690A
        jmp    SHORT loc_1_693D
loc_1_6939:
        mov    byte ptr [esi + 9], 1
loc_1_693D:
        cmp    ebx, 320h
        jae    SHORT loc_1_694A
        mov    ebx, 320h
loc_1_694A:
        lea    eax, [ebx*4]
        mov    dword ptr [ebp - 4], 0
        lea    ecx, [eax + eax*4]
        lea    edx, [ecx + ecx*2]
        lea    ebx, [edx + edx*4]
        test   ebx, ebx
        je     SHORT loc_1_6993
loc_1_6965:
        cmp    byte ptr [esi + 9], 0
        jne    SHORT loc_1_69A7
        mov    edx, dword ptr [edi + 17ah]
        in     ax, dx
        test   al, 1
        je     SHORT loc_1_698F
        mov    edx, dword ptr [edi + 182h]
        push   1
        in     al, dx
        call   sub_1_8F80
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], ebx
        jb     SHORT loc_1_6965
        jmp    SHORT loc_1_6993
loc_1_698F:
        mov    byte ptr [esi + 9], 1
loc_1_6993:
        cmp    byte ptr [esi + 9], 0
        jne    SHORT loc_1_69A7
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_69A7:
        xor    bx, bx
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 802h
        xor    ecx, ecx
        out    dx, ax
        mov    eax, dword ptr [edi + 172h]
        mov    dword ptr [ebp - 4], eax
loc_1_69C2:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, ebx
        inc    bx
        add    edx, ecx
        and    eax, 0ffffh
        inc    ecx
        cmp    bx, 6
        mov    al, byte ptr [eax + edi + 3eh]
        out    dx, al
        jb     SHORT loc_1_69C2
        mov    ebx, 6
        mov    ecx, ebx
loc_1_69E3:
        mov    edx, dword ptr [edi + 172h]
        xor    al, al
        add    edx, ecx
        inc    ecx
        out    dx, al
        dec    bx
        jne    SHORT loc_1_69E3
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 807h
        out    dx, ax
        mov    eax, 803h
        out    dx, ax
        mov    edx, dword ptr [edi + 172h]
        add    edx, 8
        in     ax, dx
        mov    bx, ax
        mov    edx, dword ptr [edi + 172h]
        in     eax, dx
        mov    ecx, eax
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 807h
        cmp    dword ptr [esi + 54h], 0ffh
        out    dx, ax
        jne    SHORT loc_1_6A50
        mov    eax, ecx
        and    eax, 700000h
        shr    eax, 14h
        test   ecx, 1000000h
        mov    dword ptr [esi + 54h], eax
        mov    dword ptr [esi + 50h], eax
        je     SHORT loc_1_6A50
        mov    byte ptr [esi + 5], 1
loc_1_6A50:
        cmp    byte ptr [esi + 5], 0
        je     NEAR PTR loc_1_6BA5
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 0b000h
        mov    dword ptr [esi + 50h], 0ffh
        test   bl, 2
        out    dx, ax
        je     SHORT loc_1_6A9A
        push   4
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     SHORT loc_1_6A9A
        mov    dword ptr [esi + 50h], 4
        mov    dword ptr [esi + 0a4h], 5f5e100h
        mov    dword ptr [esi + 110h], 1ah
loc_1_6A9A:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6AF0
        test   bl, 4
        je     SHORT loc_1_6ACF
        push   5
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     SHORT loc_1_6ACF
        mov    dword ptr [esi + 50h], 5
        mov    dword ptr [esi + 0a4h], 5f5e100h
        mov    dword ptr [esi + 110h], 1ah
loc_1_6ACF:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6B25
        test   bl, 41h
        je     SHORT loc_1_6AF0
        push   6
        push   edi
        call   sub_1_5178
        test   al, al
        je     SHORT loc_1_6AF0
        mov    dword ptr [esi + 50h], 6
loc_1_6AF0:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6B5A
        test   bl, 8
        je     SHORT loc_1_6B25
        push   0
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     SHORT loc_1_6B25
        mov    dword ptr [esi + 50h], 0
        mov    dword ptr [esi + 0a4h], 989680h
        mov    dword ptr [esi + 110h], 100h
loc_1_6B25:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6B8F
        test   bl, 20h
        je     SHORT loc_1_6B5A
        push   1
        push   edi
        call   sub_1_4BF8
        test   al, al
        je     SHORT loc_1_6B5A
        mov    dword ptr [esi + 50h], 1
        mov    dword ptr [esi + 0a4h], 989680h
        mov    dword ptr [esi + 110h], 100h
loc_1_6B5A:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6BB3
        test   bl, 10h
        je     SHORT loc_1_6B8F
        push   3
        push   edi
        call   sub_1_4BF8
        test   al, al
        je     SHORT loc_1_6B8F
        mov    dword ptr [esi + 50h], 3
        mov    dword ptr [esi + 0a4h], 989680h
        mov    dword ptr [esi + 110h], 100h
loc_1_6B8F:
        cmp    dword ptr [esi + 50h], 0ffh
        jne    SHORT loc_1_6BB3
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [esi + 50h], eax
        cmp    eax, 6
        jne    SHORT loc_1_6BB3
        jmp    SHORT loc_1_6BAB
loc_1_6BA5:
        cmp    dword ptr [esi + 50h], 6
        jne    SHORT loc_1_6BB3
loc_1_6BAB:
        push   6
        push   edi
        call   sub_1_5178
loc_1_6BB3:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        push   ecx
        push   edi
        call   sub_1_5478
        push   1e8480h
        call   sub_1_8F80
        push   edi
        call   sub_1_5538
        test   eax, eax
        jne    NEAR PTR loc_1_6C6D
        cmp    dword ptr [esi + 0a4h], 989680h
        jne    SHORT loc_1_6C1F
        mov    eax, dword ptr [esi + 50h]
        cmp    eax, 3
        je     SHORT loc_1_6C1F
        cmp    eax, 1
        je     SHORT loc_1_6C1F
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_1_6C1F
        mov    ecx, dword ptr [edi + 176h]
        mov    eax, 803h
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 172h]
        add    edx, 6
        in     ax, dx
        or     al, 20h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 807h
        out    dx, ax
loc_1_6C1F:
        mov    ax, word ptr [esi + 18h]
        mov    ebx, dword ptr [edi + 176h]
        mov    edx, ebx
        out    dx, ax
        mov    eax, 71feh
        mov    ecx, dword ptr [edi + 17ah]
        out    dx, ax
        mov    eax, 0a800h
        out    dx, ax
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_6C4A:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_6C4A
        mov    edx, ebx
        mov    eax, 2800h
        out    dx, ax
        mov    edx, ecx
loc_1_6C5C:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_6C5C
        mov    ax, word ptr [esi + 2ch]
        mov    edx, ebx
        out    dx, ax
        xor    eax, eax
loc_1_6C6D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_6C78:
        push   ebp
        mov    ebp, esp
        sub    esp, 88h
        db  0C7h, 085h, 07Ch, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        push   ebx
        mov    eax, 6
        mov    word ptr [ebp - 68h], ax
        push   esi
        mov    word ptr [ebp - 66h], ax
        push   edi
        db  0C7h, 045h, 09Ch, 000h, 000h, 000h, 000h
        mov    esi, 4
        mov    ecx, 0ah
        db  0C7h, 045h, 0A4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 60h], 5
        mov    word ptr [ebp - 5eh], 5
        db  0C7h, 045h, 0ACh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 88h], si
        mov    eax, 0bh
        mov    word ptr [ebp - 86h], si
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 58h], cx
        mov    word ptr [ebp - 56h], cx
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 40h], ax
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 3eh], ax
        mov    word ptr [ebp - 38h], ax
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 36h], ax
        mov    eax, 0ch
        mov    word ptr [ebp - 30h], ax
        mov    word ptr [ebp - 2eh], ax
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 28h], ax
        mov    word ptr [ebp - 26h], ax
        db  0C7h, 045h, 084h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 78h], 5
        mov    word ptr [ebp - 76h], 5
        db  0C7h, 045h, 08Ch, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], cx
        mov    word ptr [ebp - 1eh], cx
        db  0C7h, 045h, 094h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 80h], si
        mov    word ptr [ebp - 7eh], si
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 70h], si
        mov    eax, 3
        mov    word ptr [ebp - 6eh], si
        db  0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 48h], si
        mov    word ptr [ebp - 46h], si
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 50h], ax
        mov    word ptr [ebp - 4eh], ax
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 18h], cx
        mov    word ptr [ebp - 16h], cx
        mov    word ptr [ebp - 10h], cx
        mov    word ptr [ebp - 0eh], cx
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [edi + 19eh], 0ffh
        mov    dword ptr [edi + 1a2h], 0ffffffffh
        mov    dword ptr [edi + 1a6h], 0ffffffffh
        mov    dword ptr [edi + 1aah], 1
        mov    dword ptr [edi + 1aeh], 0ffffh
        mov    dword ptr [edi + 1b2h], 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        push   2
        lea    eax, [ebp - 88h]
        mov    dword ptr [edi + 142h], ecx
        push   eax
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        lea    ecx, [ebp - 4]
        push   ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [edi + 1b6h], 0ffffh
        mov    dword ptr [edi + 1bah], 0ffffh
        mov    dword ptr [edi + 1beh], 0
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6E88
        push   1
        lea    eax, [ebp - 80h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, esi
        push   eax
        call   sub_1_9016
        test   al, al
        je     SHORT loc_1_6E48
        mov    dword ptr [edi + 19eh], 5
loc_1_6E48:
        push   1
        lea    eax, [ebp - 78h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     SHORT loc_1_6E68
        mov    dword ptr [edi + 19eh], 2
loc_1_6E68:
        push   1
        lea    eax, [ebp - 70h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     SHORT loc_1_6E88
        mov    dword ptr [edi + 19eh], 1
loc_1_6E88:
        push   1
        lea    eax, [ebp - 68h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6EAE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1a2h], ecx
loc_1_6EAE:
        push   1
        lea    eax, [ebp - 60h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6ED9
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1fh
        ja     SHORT loc_1_6ED9
        mov    dword ptr [edi + 1a6h], eax
loc_1_6ED9:
        push   2
        lea    eax, [ebp - 58h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        mov    dword ptr [edi + 1aah], 0
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6F3F
        push   1
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     SHORT loc_1_6F1F
        mov    dword ptr [edi + 1aah], 2
        jmp    SHORT loc_1_6F3F
loc_1_6F1F:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     SHORT loc_1_6F3F
        mov    dword ptr [edi + 1aah], 1
loc_1_6F3F:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6F65
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1aeh], ecx
loc_1_6F65:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6F8B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1b2h], ecx
loc_1_6F8B:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6FB1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1b6h], ecx
loc_1_6FB1:
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6FD7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1bah], ecx
loc_1_6FD7:
        push   0
        lea    eax, [ebp - 20h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_6FFD
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 1beh], ecx
loc_1_6FFD:
        push   1
        lea    eax, [ebp - 18h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        mov    dword ptr [edi + 16eh], 0
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_702D
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 16eh], ecx
loc_1_702D:
        push   1
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        mov    dword ptr [edi + 14ah], 0
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_7085
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        je     SHORT loc_1_707F
        cmp    eax, 5
        je     SHORT loc_1_707F
        cmp    eax, 7
        je     SHORT loc_1_707F
        cmp    eax, 9
        je     SHORT loc_1_707F
        cmp    eax, 0ah
        je     SHORT loc_1_707F
        cmp    eax, 0bh
        je     SHORT loc_1_707F
        cmp    eax, 0ch
        je     SHORT loc_1_707F
        cmp    eax, 0fh
        jne    SHORT loc_1_7085
loc_1_707F:
        mov    dword ptr [edi + 14ah], eax
loc_1_7085:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7098:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 142h], 4
        mov    eax, dword ptr [esi + 16eh]
        test   eax, eax
        je     SHORT loc_1_70D1
        cmp    dword ptr [esi + 14ah], 0
        je     SHORT loc_1_70D1
        lea    ecx, [ebp - 104h]
        push   ecx
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        jmp    SHORT loc_1_70DE
loc_1_70D1:
        lea    eax, [ebp - 104h]
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   ecx
loc_1_70DE:
        push   esi
        call   sub_1_7278
        cmp    eax, 1
        je     NEAR PTR loc_1_7191
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0f4h]
        and    ecx, 1fh
        and    eax, 0fffffffeh
        xor    edx, edx
        mov    dword ptr [esi + 196h], ecx
        mov    dl, byte ptr [ebp - 0c8h]
        mov    dword ptr [esi + 16eh], eax
        mov    eax, dword ptr [esi + 1aah]
        mov    dword ptr [esi + 14ah], edx
        mov    dword ptr [esi + 142h], 0
        mov    dword ptr [esi + 146h], 2
        test   eax, eax
        jne    SHORT loc_1_7149
        test   byte ptr [ebp - 100h], 4
        je     SHORT loc_1_716A
        mov    dword ptr [esi + 29eh], 0
        jmp    SHORT loc_1_7174
loc_1_7149:
        cmp    eax, 1
        jne    SHORT loc_1_7161
        or     byte ptr [ebp - 100h], 4
        mov    dword ptr [esi + 29eh], 0
        jmp    SHORT loc_1_7174
loc_1_7161:
        and    word ptr [ebp - 100h], 0fffbh
loc_1_716A:
        mov    dword ptr [esi + 29eh], 3
loc_1_7174:
        push   2
        lea    eax, [ebp - 100h]
        and    word ptr [ebp - 100h], 0ffbfh
        push   eax
        push   4
        push   ecx
        push   esi
        call   sub_1_8A78
        xor    eax, eax
loc_1_7191:
        mov    edx, dword ptr [esi + 142h]
        lea    ecx, [edx - 3]
        cmp    ecx, 1
        lea    ecx, [edx - 1]
        sbb    edi, edi
        neg    edi
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        or     edi, ecx
        jne    SHORT loc_1_71B4
        cmp    edx, 4
        jne    SHORT loc_1_71E8
loc_1_71B4:
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_73B8
        cmp    eax, 1
        jne    SHORT loc_1_71D0
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_71D0:
        mov    dword ptr [esi + 142h], 1
        mov    eax, dword ptr [esi + 13ah]
        mov    dword ptr [esi + 196h], eax
        xor    eax, eax
loc_1_71E8:
        mov    ecx, dword ptr [esi + 196h]
        mov    edx, dword ptr [esi + 16eh]
        mov    word ptr [esi + 5aah], 20h
        mov    word ptr [esi + 5a6h], cx
        mov    word ptr [esi + 5a8h], dx
        xor    edx, edx
        mov    word ptr [esi + 5ach], dx
        mov    dword ptr [esi + 5b0h], edx
        mov    word ptr [esi + 5aeh], dx
        mov    cl, byte ptr [esi + 14ah]
        mov    word ptr [esi + 5b4h], dx
        mov    dword ptr [esi + 5b6h], edx
        mov    word ptr [esi + 5bah], dx
        mov    edi, dword ptr [esi + 142h]
        mov    word ptr [esi + 5c0h], di
        mov    byte ptr [esi + 5bch], cl
        mov    word ptr [esi + 5c2h], dx
        pop    edi
        mov    byte ptr [esi + 5beh], dl
        mov    byte ptr [esi + 5bfh], dl
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7278:
        push   ebp
        mov    eax, 1
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    dword ptr [ebp - 0ch], eax
        push   esi
        mov    dword ptr [ebp - 10h], eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 1a6h]
        cmp    ecx, -1
        jne    SHORT loc_1_72A2
        and    dword ptr [edi], 0ffffffe0h
        jmp    SHORT loc_1_72AF
loc_1_72A2:
        mov    eax, dword ptr [edi]
        mov    edx, eax
        xor    edx, ecx
        and    edx, 1fh
        xor    edx, eax
        mov    dword ptr [edi], edx
loc_1_72AF:
        mov    eax, dword ptr [edi]
        and    eax, 1fh
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 20h
        jae    NEAR PTR loc_1_739D
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 14h]
        lea    edx, [ecx + eax*4]
        mov    dword ptr [ebp - 8], edx
loc_1_72CF:
        mov    ecx, dword ptr [edi]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 4]
        and    eax, 1fh
        xor    eax, ecx
        mov    dword ptr [edi], eax
        and    eax, 0ffffff1fh
        mov    dword ptr [edi], eax
        and    eax, 0ffh
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [esi + 4ah]
        cmp    byte ptr [ecx + edx + 12ch], 0
        jne    SHORT loc_1_7352
        push   ebx
        lea    eax, [ebp - 0ch]
        push   edi
        push   esi
        push   eax
        call   sub_1_89F8
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_7352
        mov    ax, word ptr [ebx]
        cmp    ax, 0ffffh
        je     SHORT loc_1_7352
        cmp    ax, 10b7h
        jne    SHORT loc_1_7352
        cmp    byte ptr [ebx + 3], 59h
        jne    SHORT loc_1_7352
        lea    eax, [ebx + 0dh]
        push   1
        push   eax
        push   0dh
        mov    byte ptr [eax], 0f8h
        mov    eax, dword ptr [edi]
        and    eax, 1fh
        push   eax
        push   esi
        call   sub_1_8A78
        push   ebx
        push   edi
        push   esi
        call   sub_1_8A69
        mov    eax, dword ptr [ebx + 10h]
        and    eax, 0fffffffeh
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_7372
        cmp    dword ptr [ebp + 10h], eax
        je     SHORT loc_1_7372
loc_1_7352:
        add    dword ptr [ebp - 8], 0dh
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], 1a0h
        jb     NEAR PTR loc_1_72CF
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_7372:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ecx + ecx*2]
        lea    edx, [ecx + eax*4]
        mov    eax, dword ptr [esi + 4ah]
        mov    dword ptr [ebp - 10h], 0
        mov    byte ptr [eax + edx + 12ch], 1
        mov    ecx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [esi + 4ah]
        and    ecx, 0fffffffeh
        mov    dword ptr [eax + edx + 12eh], ecx
loc_1_739D:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_73B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 1a6h]
        test   esi, esi
        je     SHORT loc_1_7414
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    esi, -1
        je     SHORT loc_1_7417
        cmp    byte ptr [ebx + esi + 2cbh], 0
        jne    SHORT loc_1_7417
        push   esi
        push   edi
        call   sub_1_7478
        test   al, al
        je     SHORT loc_1_7417
        mov    eax, esi
        mov    dword ptr [edi + 13ah], esi
        mov    dword ptr [edi + 14ah], 0
        shl    eax, 0ch
        mov    dword ptr [edi + 16eh], eax
        xor    eax, eax
        mov    byte ptr [ebx + esi + 2cbh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_7414:
        mov    ebx, dword ptr [ebp + 0ch]
loc_1_7417:
        mov    esi, 1
loc_1_741C:
        cmp    byte ptr [ebx + esi + 2cbh], 0
        jne    SHORT loc_1_7431
        push   esi
        push   edi
        call   sub_1_7478
        test   al, al
        jne    SHORT loc_1_7443
loc_1_7431:
        inc    esi
        cmp    esi, 0fh
        jbe    SHORT loc_1_741C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_7443:
        mov    eax, esi
        mov    dword ptr [edi + 13ah], esi
        mov    dword ptr [edi + 14ah], 0
        shl    eax, 0ch
        mov    dword ptr [edi + 16eh], eax
        xor    eax, eax
        mov    byte ptr [ebx + esi + 2cbh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7478:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        shl    edx, 0ch
        mov    ebp, esp
        add    edx, 0c80h
        in     ax, dx
        cmp    ax, 6d50h
        je     SHORT loc_1_7496
        xor    al, al
        pop    ebp
        ret    8
loc_1_7496:
        add    edx, 2
        in     ax, dx
        and    eax, 0f0ffh
        cmp    eax, 2059h
        je     SHORT loc_1_74B4
        cmp    eax, 7059h
        je     SHORT loc_1_74B4
        xor    al, al
        pop    ebp
        ret    8
loc_1_74B4:
        mov    al, 1
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_74C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [esi + 396h]
        lea    ebx, [esi + 0b6h]
        push   ecx
        mov    edx, dword ptr [esi + 3ah]
        push   0
        push   0c00h
        push   edx
        call   sub_1_8F12
        mov    edi, dword ptr [ebx + 2e0h]
        test   edi, edi
        jne    SHORT loc_1_750D
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_750D:
        xor    eax, eax
        mov    ecx, 300h
        rep stosd
        mov    eax, dword ptr [ebx + 2e0h]
        add    eax, 600h
        mov    dword ptr [ebx + 2e4h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 21ch], eax
        add    eax, 600h
        push   0
        mov    dword ptr [ebx + 220h], eax
        mov    ecx, dword ptr [ebx + 2e0h]
        lea    eax, [ebp - 10h]
        push   eax
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebx + 2e4h]
        push   2
        push   600h
        lea    ecx, [ebp - 18h]
        push   ecx
        mov    dword ptr [ebp - 14h], edx
        push   esi
        call   sub_1_8928
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebx + 23ch], ecx
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 240h], edx
        lea    edx, [ebp - 4]
        mov    eax, dword ptr [ebx + 2e0h]
        push   edx
        mov    dword ptr [ebx + 2b5h], eax
        lea    eax, [ebx + 2e8h]
        mov    ecx, dword ptr [ebx + 2e4h]
        push   eax
        push   0
        mov    dword ptr [ebx + 2c9h], ecx
        push   0c00h
        mov    ecx, dword ptr [esi + 3ah]
        push   ecx
        call   sub_1_8F12
        cmp    ebx, 0fffffd18h
        jne    SHORT loc_1_75BF
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_75BF:
        mov    edi, dword ptr [ebx + 2e8h]
        xor    eax, eax
        mov    ecx, 300h
        rep stosd
        mov    eax, dword ptr [ebx + 2e8h]
        push   1
        add    eax, 600h
        lea    ecx, [ebx + 244h]
        push   ecx
        mov    dword ptr [ebx + 2ech], eax
        mov    eax, dword ptr [ebp - 4]
        push   2
        mov    dword ptr [ebx + 224h], eax
        push   600h
        add    eax, 600h
        add    ebx, 2e8h
        push   ebx
        push   esi
        add    esi, 366h
        mov    dword ptr [ebx - 0c0h], eax
        call   sub_1_8928
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        push   1000h
        push   esi
        call   sub_1_8F86
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_7638:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 2d2h]
        mov    esi, dword ptr [ebx + 2dah]
        lea    ecx, [ebx + 396h]
        lea    edi, [ebx + 0b6h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_7670
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [ebx + 3ah]
        push   0c00h
        push   eax
        call   sub_1_8F18
loc_1_7670:
        add    edi, 2e8h
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_768D
        push   esi
        mov    eax, dword ptr [ebx + 3ah]
        push   edi
        push   0
        push   0c00h
        push   eax
        call   sub_1_8F18
loc_1_768D:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_7698:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 172h]
        mov    si, word ptr [ebp + 0ch]
        mov    eax, 800h
        lea    edi, [ecx + 0eh]
        and    esi, 0ffff00ffh
        add    si, 80h
        mov    edx, edi
        out    dx, ax
        mov    ax, si
        lea    edx, [ecx + 0ah]
        out    dx, ax
loc_1_76CA:
        in     ax, dx
        test   ah, 80h
        jne    SHORT loc_1_76CA
        lea    edx, [ecx + 0ch]
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        mov    eax, 807h
        pop    edi
        out    dx, ax
        mov    ax, cx
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_76F8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_3A18
        mov    eax, dword ptr [esi + 4]
        add    esi, 62h
        push   eax
        push   esi
        call   sub_1_8F62
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_7718:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        mov    eax, 0
        pop    ebp
        cmp    byte ptr [ecx + 83h], 1
        adc    eax, -1
        and    eax, 0c001000dh
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7748:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        xor    eax, eax
        cmp    byte ptr [ecx + 83h], al
        je     SHORT loc_1_7764
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_7764:
        mov    edx, dword ptr [ebp + 0ch]
        test   edx, 0f010h
        je     SHORT loc_1_7778
        mov    eax, 0c0000001h
        pop    ebp
        ret    14h
loc_1_7778:
        test   dl, 1
        je     SHORT loc_1_7782
        mov    eax, 1
loc_1_7782:
        test   dl, 2
        je     SHORT loc_1_778A
        or     eax, 2
loc_1_778A:
        test   dl, 4
        je     SHORT loc_1_7792
        or     eax, 2
loc_1_7792:
        test   dl, 8
        je     SHORT loc_1_779A
        or     eax, 4
loc_1_779A:
        mov    byte ptr [ecx + 82h], 0
        test   dl, 20h
        je     SHORT loc_1_77B0
        mov    byte ptr [ecx + 82h], 1
        or     eax, 8
loc_1_77B0:
        add    ax, 8000h
        pop    ebp
        mov    word ptr [ecx + 102h], ax
        mov    edx, dword ptr [ecx + 176h]
        out    dx, ax
        xor    eax, eax
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_77D8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_1_77FD:
        test   edx, edx
        je     SHORT loc_1_780C
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_77FD
        mov    dword ptr [ecx], edx
loc_1_780C:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_781F
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_781F:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   49h
        push   ecx
        call   sub_1_8F86
        test   eax, eax
        je     SHORT loc_1_7844
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_7844:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        add    eax, 14h
        mov    esi, dword ptr [ebp + 20h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 5eh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_785F:
        test   al, al
        jne    SHORT loc_1_787E
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   49h
        push   eax
        call   sub_1_8F8C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_787E:
        cmp    byte ptr [esi + 83h], 0
        je     SHORT loc_1_789D
        mov    eax, dword ptr [esi + 5ah]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 103h
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 5ah], ecx
        jmp    SHORT loc_1_78AE
loc_1_789D:
        mov    eax, dword ptr [esi + 52h]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 52h], ecx
loc_1_78AE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 4ah]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 2ah], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 18h], eax
        cmp    dword ptr [esi + 56h], ecx
        jne    SHORT loc_1_7902
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esi + 62h]
        push   eax
        push   ecx
        call   sub_1_8F62
loc_1_7902:
        push   esi
        inc    dword ptr [esi + 56h]
        call   sub_1_7BE8
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_1_7918:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 20h], 0
        mov    ecx, dword ptr [esi]
        je     SHORT loc_1_7931
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    4
loc_1_7931:
        mov    byte ptr [esi + 20h], 1
        inc    dword ptr [esi + 1ch]
        mov    eax, dword ptr [ecx + 52h]
        cmp    esi, eax
        jne    SHORT loc_1_7947
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 52h], eax
        jmp    SHORT loc_1_795A
loc_1_7947:
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_1_7954
loc_1_794C:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_794C
loc_1_7954:
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], edx
loc_1_795A:
        lea    eax, [esi + 21h]
        mov    edx, dword ptr [esi + 14h]
        push   eax
        mov    ecx, dword ptr [ecx + 5eh]
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_796B:
        test   eax, eax
        je     SHORT loc_1_7976
        cmp    eax, 103h
        jne    SHORT loc_1_797C
loc_1_7976:
        push   esi
        call   sub_1_79B8
loc_1_797C:
        push   esi
        call   sub_1_79B8
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7998:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_79B8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_79B8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 1ch]
        dec    eax
        mov    dword ptr [ecx + 1ch], eax
        jne    SHORT loc_1_79D4
        push   ecx
        call   sub_1_79E8
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_79D4:
        mov    eax, 103h
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_79E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_88F8
        mov    eax, dword ptr [edi + 56h]
        mov    ecx, 1
        dec    eax
        push   edi
        mov    dword ptr [edi + 56h], eax
        cmp    ecx, eax
        mov    al, cl
        adc    al, 0ffh
        mov    byte ptr [edi + 0ch], al
        call   sub_1_7C18
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_1_8EC4
        push   0
        push   49h
        push   esi
        call   sub_1_8F8C
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7A38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax + 20h], 0
        mov    esi, dword ptr [eax]
        je     SHORT loc_1_7A52
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    4
loc_1_7A52:
        mov    cl, byte ptr [esi + 21h]
        test   cl, 2
        je     SHORT loc_1_7A64
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    4
loc_1_7A64:
        or     cl, 2
        push   40010004h
        push   esi
        mov    byte ptr [esi + 21h], cl
        inc    dword ptr [eax + 1ch]
        mov    dword ptr [esi + 6eh], eax
        call   sub_1_7B68
        mov    byte ptr [esi + 83h], 1
        push   esi
        call   sub_1_7CB8
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_7A98:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 11h]
loc_1_7AA6:
        push   edi
        call   sub_1_7B48
        test   eax, eax
        je     SHORT loc_1_7ACF
        or     byte ptr [eax + 2eh], 0ah
        mov    dword ptr [eax + 24h], ebx
        cmp    dword ptr [esi + 1dh], ebx
        jne    SHORT loc_1_7AC4
        mov    dword ptr [esi + 1dh], eax
        mov    dword ptr [esi + 19h], eax
        jmp    SHORT loc_1_7AA6
loc_1_7AC4:
        mov    ecx, dword ptr [esi + 19h]
        mov    dword ptr [ecx + 24h], eax
        mov    dword ptr [esi + 19h], eax
        jmp    SHORT loc_1_7AA6
loc_1_7ACF:
        lea    edi, [esi + 19h]
loc_1_7AD2:
        push   edi
        call   sub_1_7B48
        test   eax, eax
        je     SHORT loc_1_7AE5
        push   eax
        push   esi
        call   sub_1_0828
        jmp    SHORT loc_1_7AD2
loc_1_7AE5:
        mov    edi, dword ptr [esi + 6eh]
        push   40010005h
        and    byte ptr [esi + 21h], 0fdh
        mov    byte ptr [esi + 83h], 0
        push   esi
        call   sub_1_7B68
        push   0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8ED6
        push   edi
        call   sub_1_79B8
        mov    ecx, dword ptr [esi + 5ah]
        test   ecx, ecx
        je     SHORT loc_1_7B3C
loc_1_7B16:
        mov    eax, dword ptr [esi + 5ah]
        mov    ebx, dword ptr [esi + 52h]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 5ah], edx
        mov    dword ptr [ecx + 0ch], ebx
        mov    dword ptr [esi + 52h], ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_8EBE
        mov    ecx, dword ptr [esi + 5ah]
        test   ecx, ecx
        jne    SHORT loc_1_7B16
loc_1_7B3C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_7B48:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     SHORT loc_1_7B5C
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 4], edx
loc_1_7B5C:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7B68:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 52h]
        test   edi, edi
        je     SHORT loc_1_7BB1
        mov    esi, dword ptr [ebp + 0ch]
loc_1_7B7F:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 1ch]
        mov    ebx, dword ptr [edi + 0ch]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_8EEE
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_8EF4
        push   edi
        call   sub_1_79B8
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_7B7F
loc_1_7BB1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7BC8:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 5eh]
        push   edx
        call   sub_1_8DB0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7BE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 1
        jne    SHORT loc_1_7C09
        mov    edx, dword ptr [eax + 176h]
        mov    eax, 2000h
        out    dx, ax
        mov    eax, 4800h
        out    dx, ax
loc_1_7C09:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7C18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 0
        jne    SHORT loc_1_7C32
        mov    edx, dword ptr [eax + 176h]
        mov    eax, 1800h
        out    dx, ax
loc_1_7C32:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  076h, 001h, 000h, 000h, 08Bh, 089h, 07Ah, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 069h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_7C88:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0348
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7CA8:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7CB8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0c0h], 1
        call   sub_1_1BC8
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 1800h
        mov    dword ptr [esi + 10eh], 0
        out    dx, ax
        mov    eax, 7840h
        pop    esi
        out    dx, ax
        mov    eax, 6000h
        pop    ebp
        out    dx, ax
        ret    4
        db  08Bh, 0FFh
loc_1_7CF8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 1ch]
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_7D12
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_7D4E
loc_1_7D12:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 10h]
        test   edx, edx
        jne    SHORT loc_1_7D28
        push   eax
        push   esi
        push   ecx
        call   sub_1_82B8
        mov    edi, eax
        jmp    SHORT loc_1_7D4E
loc_1_7D28:
        cmp    edx, 1
        jne    SHORT loc_1_7D49
        cmp    byte ptr [ecx + 83h], 0
        je     SHORT loc_1_7D3D
        mov    edi, 0c001000dh
        jmp    SHORT loc_1_7D4E
loc_1_7D3D:
        push   eax
        push   esi
        push   ecx
        call   sub_1_8308
        mov    edi, eax
        jmp    SHORT loc_1_7D4E
loc_1_7D49:
        mov    edi, 10001h
loc_1_7D4E:
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7D68:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 14eh]
        mov    word ptr [ebp - 6], bx
        mov    dword ptr [ebp - 20h], ecx
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], ebx
        cmp    edx, 1010101h
        mov    eax, edx
        mov    byte ptr [ebp - 4], bl
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 1ch], ebx
        ja     SHORT loc_1_7DB3
        je     NEAR PTR loc_1_80F9
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_7DD4
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_7DB3:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_8115
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_8131
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_81DF
loc_1_7DD4:
        mov    eax, edx
        and    eax, 0ff000000h
        cmp    eax, 0ff000000h
        je     SHORT loc_1_7DEE
        mov    dword ptr [ebp - 10h], 0c00000bbh
        jmp    NEAR PTR loc_1_81F0
loc_1_7DEE:
        add    edx, 0fefeffh
        cmp    edx, 8
        ja     SHORT loc_1_7E00
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_7E00:
        mov    dword ptr [ebp - 10h], 0c00000bbh
        jmp    NEAR PTR loc_1_81F0
loc_1_7E0C:
        mov    al, byte ptr [esi + 5c8h]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E22:
        mov    ax, word ptr [esi + 5c9h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E3A:
        mov    ax, word ptr [esi + 5cbh]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E52:
        mov    ax, word ptr [esi + 5cdh]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E6A:
        mov    al, byte ptr [esi + 5cfh]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E80:
        mov    ax, word ptr [esi + 5d2h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_81F6
loc_1_7E98:
        mov    ax, word ptr [esi + 5d0h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_81F6
loc_1_7EB0:
        xor    eax, eax
        mov    ax, word ptr [esi + 5d2h]
        lea    ecx, [eax + eax*4]
        lea    edi, [ecx*2]
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_7ED9
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], edi
        jmp    NEAR PTR loc_1_81F0
loc_1_7ED9:
        push   edi
        mov    eax, dword ptr [esi + 5c4h]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        mov    bl, 1
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], edi
        jmp    NEAR PTR loc_1_81F0
loc_1_7EF6:
        cmp    dword ptr [ebp + 1ch], 1eh
        jae    SHORT loc_1_7F11
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], 1eh
        jmp    NEAR PTR loc_1_81F0
loc_1_7F11:
        push   1eh
        mov    eax, dword ptr [esi + 5c4h]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        mov    bl, 1
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1eh
        jmp    NEAR PTR loc_1_81F0
loc_1_7F33:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_7F48
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_1_81F6
loc_1_7F48:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 9ch
        jmp    NEAR PTR loc_1_81F6
loc_1_7F57:
        cmp    byte ptr [esi + 83h], 0
        mov    dword ptr [ebp - 1ch], 2
        jne    SHORT loc_1_7F6E
        mov    dword ptr [ebp - 1ch], 0
loc_1_7F6E:
        lea    eax, [ebp - 1ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_81F6
loc_1_7F7B:
        lea    eax, [ebp - 20h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_81F6
loc_1_7F88:
        mov    eax, dword ptr [esi + 152h]
        mov    esi, 4
        sub    eax, 0eh
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_7FA1:
        mov    eax, dword ptr [esi + 156h]
        mov    esi, 4
        sub    eax, 0eh
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_7FBA:
        mov    ecx, 64h
        mov    eax, dword ptr [esi + 15ah]
        sub    edx, edx
        mov    esi, 4
        div    ecx
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_7FD9:
        mov    eax, dword ptr [esi + 15eh]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_7FEF:
        mov    eax, dword ptr [esi + 162h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_8005:
        mov    eax, dword ptr [esi + 166h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_801B:
        mov    eax, dword ptr [esi + 16ah]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_8031:
        push   3
        add    esi, 44h
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        mov    esi, 4
        call   sub_1_906C
        and    dword ptr [ebp - 0ch], 0ffffff00h
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_8054:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 18h
        jmp    NEAR PTR loc_1_81F6
loc_1_8063:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_807A
        test   ecx, ecx
        jne    SHORT loc_1_8075
        mov    eax, dword ptr [esi + 5eh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_8089
loc_1_8075:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_1_8089
loc_1_807A:
        xor    eax, eax
        test   ecx, ecx
        jne    SHORT loc_1_8089
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 14h]
        mov    eax, dword ptr [eax + 8]
loc_1_8089:
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_8099:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_80A6
        mov    eax, dword ptr [esi]
        sub    eax, 0eh
        jmp    SHORT loc_1_80AC
loc_1_80A6:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 18h]
loc_1_80AC:
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_80BC:
        lea    eax, [ebp - 6]
        mov    esi, 2
        mov    word ptr [ebp - 6], 300h
        jmp    NEAR PTR loc_1_81F6
loc_1_80CF:
        mov    eax, dword ptr [esi + 156h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_81F6
loc_1_80E5:
        lea    eax, [ebp - 0ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], 7
        jmp    NEAR PTR loc_1_81F6
loc_1_80F9:
        push   6
        add    esi, 44h
        push   esi
        lea    eax, [ebp - 28h]
        push   eax
        mov    esi, 6
        call   sub_1_906C
        lea    eax, [ebp - 28h]
        jmp    NEAR PTR loc_1_81F6
loc_1_8115:
        push   6
        add    esi, 3eh
        push   esi
        lea    eax, [ebp - 28h]
        push   eax
        mov    esi, 6
        call   sub_1_906C
        lea    eax, [ebp - 28h]
        jmp    NEAR PTR loc_1_81F6
loc_1_8131:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_8186
        mov    ecx, dword ptr [esi + 5eh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_815B
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_81F0
loc_1_815B:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 18h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 10h]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8171:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 10h], 0
        lea    eax, [eax + eax*2]
        mov    bl, 1
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_81F0
loc_1_8186:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 5eh]
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8196:
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_81B1
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_81F0
loc_1_81B1:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [edi + 14h]
        push   edx
        mov    ecx, dword ptr [esi + 5eh]
        push   eax
        lea    edx, [ebp - 10h]
        push   ecx
        push   edx
        mov    bl, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_81D0:
        mov    ecx, dword ptr [ebp - 18h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_81F0
loc_1_81DF:
        lea    eax, [ebp - 0ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], 10h
        jmp    SHORT loc_1_81F6
loc_1_81F0:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 14h]
loc_1_81F6:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_822D
        test   bl, bl
        jne    SHORT loc_1_822D
        cmp    dword ptr [ebp + 1ch], esi
        jae    SHORT loc_1_821D
        mov    ecx, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_821D:
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], esi
loc_1_822D:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_82B8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_7D68
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8308:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_1_8352
        je     NEAR PTR loc_1_8527
        cmp    eax, 1010eh
        je     NEAR PTR loc_1_8495
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_84D1
        cmp    eax, 10112h
        je     NEAR PTR loc_1_851E
loc_1_8352:
        mov    edx, eax
        and    edx, 0ff000000h
        cmp    edx, 0ff000000h
        je     SHORT loc_1_8376
        mov    dword ptr [ebp - 4], 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_8376:
        add    eax, 0fefefeh
        cmp    eax, 3
        ja     SHORT loc_1_8387
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_8387:
        mov    dword ptr [ebp - 4], 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_839B:
        cmp    ebx, 2
        je     SHORT loc_1_83B4
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_83B4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 5c9h
        push   edi
        call   sub_1_906C
        jmp    NEAR PTR loc_1_8565
loc_1_83CB:
        cmp    ebx, 2
        je     SHORT loc_1_83E4
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_83E4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 5c9h
        push   edi
        call   sub_1_906C
        jmp    NEAR PTR loc_1_8565
loc_1_83FB:
        cmp    ebx, 2
        je     SHORT loc_1_8414
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_8414:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 5c9h
        push   edi
        call   sub_1_906C
        jmp    NEAR PTR loc_1_8565
loc_1_842B:
        cmp    ebx, 1
        je     SHORT loc_1_8444
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_8444:
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 5cfh], al
        je     NEAR PTR loc_1_8565
        mov    byte ptr [edi + 5cfh], al
        test   al, al
        jne    NEAR PTR loc_1_8565
        xor    edx, edx
        xor    eax, eax
        mov    word ptr [edi + 5d0h], 0
        mov    dx, word ptr [edi + 5d2h]
        mov    edi, dword ptr [edi + 5c4h]
        lea    edx, [edx + edx*4]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        jmp    NEAR PTR loc_1_8565
loc_1_8495:
        cmp    ebx, 4
        je     SHORT loc_1_84AE
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_8565
loc_1_84AE:
        push   4
        lea    eax, [ebp - 0ch]
        push   ecx
        push   eax
        call   sub_1_906C
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edi
        call   sub_1_8888
        jmp    NEAR PTR loc_1_8562
loc_1_84D1:
        cmp    ebx, 4
        je     SHORT loc_1_84E7
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_8565
loc_1_84E7:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_906C
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 152h]
        sub    eax, 0eh
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_8515
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [eax + 18h], ecx
        call   sub_1_88F8
        jmp    SHORT loc_1_8565
loc_1_8515:
        mov    dword ptr [ebp - 4], 0c0010014h
        jmp    SHORT loc_1_8565
loc_1_851E:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_8565
loc_1_8527:
        mov    edi, 6
        mov    eax, ebx
        sub    edx, edx
        div    edi
        test   edx, edx
        je     SHORT loc_1_8547
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_8565
loc_1_8547:
        push   ecx
        mov    eax, ebx
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_1_8818
loc_1_8562:
        mov    dword ptr [ebp - 4], eax
loc_1_8565:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_8575
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], ebx
loc_1_8575:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_8598:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 0ch]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_7D68
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_873E
        xor    edi, edi
        push   ebx
        call   sub_1_7C88
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2020dh
        ja     SHORT loc_1_861C
        je     NEAR PTR loc_1_86B7
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_864A
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_861C:
        cmp    eax, 1020201h
        ja     SHORT loc_1_8654
        je     NEAR PTR loc_1_86D7
        cmp    eax, 1020101h
        je     NEAR PTR loc_1_86BF
        cmp    eax, 1020102h
        je     NEAR PTR loc_1_86C7
        cmp    eax, 1020103h
        je     NEAR PTR loc_1_86CF
loc_1_864A:
        mov    edi, 0c0010017h
        jmp    NEAR PTR loc_1_8710
loc_1_8654:
        sub    eax, 1020202h
        cmp    eax, 5
        ja     SHORT loc_1_864A
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_8665:
        mov    eax, dword ptr [ebx + 302h]
        jmp    NEAR PTR loc_1_870D
loc_1_8670:
        mov    eax, dword ptr [ebx + 306h]
        jmp    NEAR PTR loc_1_870D
loc_1_867B:
        mov    eax, dword ptr [ebx + 316h]
        add    eax, dword ptr [ebx + 33ah]
        add    eax, dword ptr [ebx + 336h]
        add    eax, dword ptr [ebx + 326h]
        jmp    SHORT loc_1_870D
loc_1_8695:
        mov    eax, dword ptr [ebx + 34ah]
        add    eax, dword ptr [ebx + 33eh]
        add    eax, dword ptr [ebx + 342h]
        add    eax, dword ptr [ebx + 346h]
        jmp    SHORT loc_1_870D
loc_1_86AF:
        mov    eax, dword ptr [ebx + 34ah]
        jmp    SHORT loc_1_870D
loc_1_86B7:
        mov    eax, dword ptr [ebx + 342h]
        jmp    SHORT loc_1_870D
loc_1_86BF:
        mov    eax, dword ptr [ebx + 33eh]
        jmp    SHORT loc_1_870D
loc_1_86C7:
        mov    eax, dword ptr [ebx + 31eh]
        jmp    SHORT loc_1_870D
loc_1_86CF:
        mov    eax, dword ptr [ebx + 322h]
        jmp    SHORT loc_1_870D
loc_1_86D7:
        mov    eax, dword ptr [ebx + 332h]
        jmp    SHORT loc_1_870D
loc_1_86DF:
        mov    eax, dword ptr [ebx + 326h]
        jmp    SHORT loc_1_870D
loc_1_86E7:
        mov    eax, dword ptr [ebx + 34ah]
        jmp    SHORT loc_1_870D
loc_1_86EF:
        mov    eax, dword ptr [ebx + 316h]
        jmp    SHORT loc_1_870D
loc_1_86F7:
        mov    eax, dword ptr [ebx + 31ah]
        jmp    SHORT loc_1_870D
loc_1_86FF:
        mov    eax, dword ptr [ebx + 32ah]
        jmp    SHORT loc_1_870D
loc_1_8707:
        mov    eax, dword ptr [ebx + 32eh]
loc_1_870D:
        mov    dword ptr [ebp - 4], eax
loc_1_8710:
        push   ebx
        call   sub_1_7CA8
        test   edi, edi
        jne    SHORT loc_1_873E
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_8735
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_906C
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_873E
loc_1_8735:
        add    dword ptr [ebp - 8], 4
        mov    edi, 0c0010014h
loc_1_873E:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh
        db  064h, 024h, 000h
loc_1_8788:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 14h]
        cmp    ecx, 20105h
        ja     SHORT loc_1_87BB
        cmp    ecx, 20101h
        jae    SHORT loc_1_8805
        cmp    ecx, 10101h
        jb     SHORT loc_1_87FC
        cmp    ecx, 10111h
        jbe    SHORT loc_1_8805
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_87BB:
        cmp    ecx, 1010104h
        ja     SHORT loc_1_87DC
        cmp    ecx, 1010101h
        jae    SHORT loc_1_8805
        cmp    ecx, 2020dh
        je     SHORT loc_1_8805
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_87DC:
        cmp    ecx, 1020207h
        ja     SHORT loc_1_87FC
        cmp    ecx, 1020201h
        jae    SHORT loc_1_8805
        cmp    ecx, 1020101h
        jb     SHORT loc_1_87FC
        cmp    ecx, 1020103h
        jbe    SHORT loc_1_8805
loc_1_87FC:
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_8805:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8598
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_8818:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 83h], 0
        push   edi
        jne    SHORT loc_1_886E
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_8861
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        mov    eax, dword ptr [esi + 14h]
        push   eax
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 5eh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8851:
        mov    edi, eax
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
loc_1_8861:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
loc_1_886E:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8888:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        cmp    byte ptr [ecx + 83h], 0
        jne    SHORT loc_1_88E4
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_88D7
        inc    dword ptr [esi + 1ch]
        cmp    dword ptr [ecx + 14eh], edi
        jne    SHORT loc_1_88C9
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    edi, dword ptr [esi + 14h]
        push   edi
        mov    eax, dword ptr [ecx + 5eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_88C7:
        mov    edi, eax
loc_1_88C9:
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_88D7:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_88E4:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_88F8:
        push   ebp
        mov    ecx, 3ch
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 52h]
        test   edx, edx
        je     SHORT loc_1_891B
loc_1_890B:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_8914
        mov    ecx, esi
loc_1_8914:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_890B
loc_1_891B:
        add    ecx, 0eh
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    4
        db  08Dh, 049h, 000h
sub_1_8928:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        push   eax
        push   ecx
        call   sub_1_8FB6
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_1_8956
        mov    dword ptr [ecx + 0b2h], eax
        jmp    SHORT loc_1_895C
loc_1_8956:
        mov    dword ptr [ecx + 0aeh], eax
loc_1_895C:
        test   esi, esi
        je     SHORT loc_1_8984
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 0ch]
loc_1_8966:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        add    edi, 4
        push   ebx
        push   eax
        add    ebx, 4
        call   sub_1_8FC2
        dec    esi
        jne    SHORT loc_1_8966
loc_1_8984:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8998:
        push   ebp
        mov    ecx, 0
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_1_89BB
        mov    edx, dword ptr [esi + 4]
        lea    ecx, [eax + edx]
        shr    edx, 0ch
        dec    ecx
        shr    ecx, 0ch
        sub    ecx, edx
        inc    ecx
loc_1_89BB:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax], ecx
        jb     SHORT loc_1_89D2
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        call   sub_1_8FDA
        pop    esi
        pop    ebp
        ret    14h
loc_1_89D2:
        pop    esi
        mov    dword ptr [eax], ecx
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_89E8:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_89F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0adh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_8A39
        push   100h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        and    eax, 1fh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 3ah]
        push   eax
        call   sub_1_8F42
        jmp    NEAR PTR loc_1_8A59
loc_1_8A39:
        push   100h
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        and    eax, 1fh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_8F36
loc_1_8A59:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_8A69:
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8A78:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 10h]
        push   ecx
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 0adh], 0
        push   edi
        push   esi
        je     SHORT loc_1_8ABE
        mov    eax, dword ptr [ebx + 3ah]
        push   eax
        call   sub_1_8F48
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebx + 3ah]
        push   ecx
        push   edi
        push   esi
        push   edx
        call   sub_1_8F42
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_8ABE:
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_8F3C
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   edi
        push   esi
        push   edx
        call   sub_1_8F36
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8AE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_00A8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8B08:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        call   sub_1_00F8
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8B30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8B60
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_8B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_8C00
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_8BA0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_8B95
        cmp    al, 0ffh
        jne    SHORT loc_1_8B95
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_8B95
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_8C07
loc_1_8B95:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_8C07
        db  08Bh, 0FFh
loc_1_8BA0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_8BF0
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_8BC6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_8BCD
loc_1_8BC6:
        mov    dword ptr [ebp - 10h], 1
loc_1_8BCD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_8BE0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_8C0A
        db  08Dh, 064h, 024h, 000h
loc_1_8BE0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_8C0A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8BF0:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_8C0A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8C00:
        mov    dword ptr [ebp - 4], 20h
loc_1_8C07:
        mov    esi, dword ptr [ebp + 8]
loc_1_8C0A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_8D01
loc_1_8C15:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_8C6D
        test   al, 2
        je     SHORT loc_1_8C59
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 086h, 07Bh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_8C59
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_8C6D
loc_1_8C59:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_8C64
        test   bl, 4
        jne    SHORT loc_1_8C6D
loc_1_8C64:
        test   bl, 20h
        je     NEAR PTR loc_1_8CF6
loc_1_8C6D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_8EE2
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_8CF6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_8CAF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_8CAF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_8CC0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_8CC6
        db  08Bh, 0FFh
loc_1_8CC0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_8CC6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_8CD5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8EE8
loc_1_8CD5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_8F8C
        mov    edi, ebx
        jmp    SHORT loc_1_8CF9
loc_1_8CF6:
        mov    edi, dword ptr [edi + 10h]
loc_1_8CF9:
        or     edi, edi
        jne    NEAR PTR loc_1_8C15
loc_1_8D01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_8D10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8D20
        pop    ebp
        ret    4
sub_1_8D20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_8D9E
loc_1_8D30:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_8D97
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8EE8
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_8D97
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_8D5C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_8D5C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_8D70
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_8D76
        db  005h, 000h, 000h, 000h, 000h
loc_1_8D70:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_8D76:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_8F8C
        mov    edi, ebx
        jmp    SHORT loc_1_8D9A
loc_1_8D97:
        mov    edi, dword ptr [edi + 10h]
loc_1_8D9A:
        or     edi, edi
        jne    SHORT loc_1_8D30
loc_1_8D9E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8DB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_8DC5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DC5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_8E01
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_8DF2
        cmp    bl, 0ffh
        jne    SHORT loc_1_8DF2
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_8DF2
        test   al, 8
        je     SHORT loc_1_8DEB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DEB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DF2:
        test   al, 4
        jne    SHORT loc_1_8DFA
        test   al, 2
        je     SHORT loc_1_8E19
loc_1_8DFA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8E01:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_8E19
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_8E19
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8E19:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_8E9A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_8EA0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_8EA6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_8EAC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_8EB2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_8EB8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_8EBE:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_8EC4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_8ECA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_8ED6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_8EE2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_8EE8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_8EEE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_8EF4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_8F0C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_8F12:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_8F18:
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
sub_1_8F36:
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
sub_1_8F3C:
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
sub_1_8F42:
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
sub_1_8F48:
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_8F62:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_8F68:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_8F6E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_8F74:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_8F80:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_8F86:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_8F8C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_8FB0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_8FB6:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_8FC2:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_8FD4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
sub_1_8FDA:
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_8FF8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_8FFE:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_9004:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_900A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_9010:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_9016:
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_9058:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_9064:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_906C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_908C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_90AC
        or     ecx, ecx
        je     SHORT loc_1_90C2
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_90C0
loc_1_90AC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_90C0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_90C0:
        rep movsb
loc_1_90C2:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 04Ch, 035h, 039h
        db  058h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_911C:
        dec    ecx
        push   ebx
        push   edx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9124:
        inc    esp
        push   eax
        inc    ebx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_912C:
        inc    esp
        push   eax
        inc    ebx
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9134:
        inc    esp
        push   eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_913C:
        dec    ecx
        push   edx
        inc    ebp
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9144:
        inc    esi
        inc    ecx
        dec    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_914C:
        dec    ecx
        inc    esp
        dec    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9154:
        push   ebp
        push   ebx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_915C:
        push   ebp
        push   ebx
        inc    esp
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9164:
        push   ebp
        push   ebx
        inc    esp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_916C:
        push   ebp
        push   ebx
        inc    esp
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9174:
        push   ebp
        push   ebx
        inc    esp
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_917C:
        push   ebp
        dec    ecx
        inc    ebx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9184:
        push   ebp
        dec    ecx
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edi + edx*2 + 4eh], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9194:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_919C:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91A4:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91AC:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_91B4:
        inc    esp
        push   edi
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_91BC:
        dec    ecx
        inc    esp
        dec    esi
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91C4:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91CC:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91D4:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91DC:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_91E4:
        dec    ecx
        inc    esp
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_91EC:
        push   edx
        inc    esp
        dec    esi
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91F4:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91FC:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9204:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_920C:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9214:
        push   edx
        inc    esp
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + ebx*2 + 43h], dl
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9224:
        push   esp
        pop    eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + ebx*2 + 41h], dl
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9234:
        push   esp
        pop    eax
        inc    ecx
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_923C:
        inc    ecx
        inc    esp
        dec    edx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9244:
        push   esp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_924C:
        inc    esp
        dec    edi
        inc    esi
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9254:
        push   esp
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_925C:
        dec    ecx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9264:
        dec    esi
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_926C:
        push   ebp
        pop    eax
        inc    esi
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9274:
        push   edx
        pop    eax
        inc    ebp
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_927C:
        inc    ebp
        xor    cl, byte ptr [ebp + 53h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9284:
        inc    ebp
        xor    cl, byte ptr [ebp + 50h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_928C:
        inc    esp
        dec    esp
        xor    dword ptr [esi], edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9294:
        inc    esp
        dec    esp
        xor    dword ptr [eax + eax], edi
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 41h], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92A4:
        inc    ebp
        xor    dl, byte ptr [eax + 49h]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92AC:
        push   edx
        pop    eax
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92B4:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92BC:
        push   edx
        pop    eax
        inc    ebp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx + 58h], dl
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + 31h], al
        inc    esp
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92D4:
        inc    esp
        dec    esp
        xor    bh, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92DC:
        inc    esp
        dec    esp
        xor    bh, byte ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 41h], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92EC:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92F4:
        inc    ebp
        xor    dword ptr [eax + 4ch], edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92FC:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9304:
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_930C:
        push   edx
        inc    esp
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 50h], dl
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 58h], dl
        inc    ebp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx + 58h], dl
        inc    ebx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 58h], dl
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9334:
        push   edx
        pop    eax
        inc    ebx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_933C:
        push   edx
        inc    ebx
        inc    esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9344:
        push   edx
        inc    ebx
        inc    esp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_934C:
        inc    esp
        dec    esp
        xor    edi, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9354:
        inc    esp
        dec    esp
        xor    edi, dword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 41h], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9364:
        push   edx
        inc    ebx
        push   eax
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_936C:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9374:
        push   edx
        pop    eax
        inc    ebx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_1_937C:
        push   edx
        inc    ebx
        inc    esp
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx*2 + 34h], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_938C:
        inc    esp
        dec    esp
        xor    al, 3ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9394:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_939C:
        push   edx
        pop    eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ebp + 50h], dl
        dec    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_93AC:
        push   ebp
        push   eax
        dec    esp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + edx*2 + 49h], cl
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93BC:
        dec    esp
        push   eax
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93C4:
        push   edx
        inc    ebp
        dec    edx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 52h], cl
        push   ebp
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93D4:
        push   edx
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93DC:
        inc    ecx
        dec    esp
        inc    edi
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93E4:
        inc    ebx
        push   edx
        inc    ebx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93F4:
        inc    esp
        dec    ecx
        push   ebx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93FC:
        push   edx
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9404:
        dec    eax
        inc    ecx
        dec    ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_940C:
        dec    eax
        inc    ecx
        dec    ebx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
loc_1_9418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr ds:[eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
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
        inc    ebp
        dec    esp
        xor    eax, 5839h
        db  000h
loc_1_94BC:
        inc    ebp
        dec    esp
        xor    eax, 5839h
        add    byte ptr [ebp + 61h], cl
        db  078h, 069h
        insd
        jne    SHORT loc_1_9538
        dec    ebp
        jne    SHORT loc_1_953A
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_9521
        imul   esi, dword ptr [ebx + 74h], 0
        dec    ebp
        popad
        js     SHORT loc_1_9549
        insd
        jne    SHORT loc_1_9550
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_9551
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd
        jne    SHORT loc_1_956D
        arpl   word ptr [ebp + 43h], sp
        outsd
        jne    SHORT loc_1_956F
        je     SHORT loc_1_9503
loc_1_9503:
        add    byte ptr [esp + edx*2 + 41h], al
        push   ebx
        outsd
        jne    SHORT loc_1_957D
        arpl   word ptr [ebp + 43h], sp
        outsd
        jne    SHORT loc_1_957F
        je     SHORT loc_1_9513
loc_1_9513:
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
loc_1_9521:
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        jne    SHORT loc_1_95A2
        push   esp
        jns    SHORT loc_1_95A2
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_958C
loc_1_9538:
        jns    SHORT loc_1_95AA
loc_1_953A:
        add    byte ptr gs:[edx + 55h], al
        push   ebx
        db  000h
loc_1_9540:
        inc    edx
        push   ebp
        push   ebx
        add    byte ptr [edx + 55h], al
        push   ebx
        dec    esi
        dec    edi
loc_1_9549:
        add    byte ptr [eax], al
        db  000h
loc_1_954C:
        inc    edx
        push   ebp
        push   ebx
        dec    esi
loc_1_9550:
        dec    edi
loc_1_9551:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4ch], dl
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_955C:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_956D:
        inc    ebp
        push   ebx
loc_1_956F:
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_957D:
        inc    ebp
        push   ebx
loc_1_957F:
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        push   ebp
        push   ebx
        dec    ebp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
loc_1_958C:
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        push   ebx
        dec    ebp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 49h], cl
        dec    esi
        push   esp
        pop    eax
        push   ebx
loc_1_95A2:
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
loc_1_95A8:
        dec    ebp
        dec    ecx
loc_1_95AA:
        dec    esi
        push   esp
        pop    eax
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        dec    ebp
        dec    ecx
        dec    esi
        push   edx
        pop    eax
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        add    byte ptr [eax], al
loc_1_95C0:
        dec    ebp
        dec    ecx
        dec    esi
        push   edx
        pop    eax
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        add    byte ptr [eax], al
        push   esp
        pop    eax
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [eax + ebx*2 + 43h], dl
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + 58h], dl
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        db  000h
loc_1_95F0:
        push   edx
        pop    eax
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 4eh], dl
        inc    esp
        inc    ebp
        push   edx
        inc    edi
        dec    edi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4eh], dl
        inc    esp
        inc    ebp
        push   edx
        inc    edi
        dec    edi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        add    byte ptr [eax + 43h], dl
        dec    ecx
        add    byte ptr [ebp + 49h], al
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9624:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   ebx
        inc    ecx
        db  000h
loc_1_9630:
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [ecx + 45h], bl
        push   ebx
        db  000h
loc_1_9638:
        pop    ecx
        inc    ebp
        push   ebx
        add    byte ptr [esi + 4fh], cl
        add    byte ptr [eax], al
loc_1_9640:
        dec    esi
        dec    edi
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_9678:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0d000201h
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, 6010202h
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx + eax], eax
        add    al, byte ptr [ecx]
loc_1_9714:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_9770:
        inc    esi
        popad
        jae    SHORT loc_1_97E8
        and    byte ptr [ebp + 74h], al
        push   656e7265h
        je     SHORT loc_1_979E
        inc    ecx
        popad
        jo     SHORT loc_1_97F7
        jb     NEAR PTR loc_1_9786
loc_1_9786:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_979E:
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
loc_1_97C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97C8:
        add    byte ptr [eax], al
loc_1_97CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97E6:
        add    byte ptr [eax], al
loc_1_97E8:
        add    byte ptr [eax], al
loc_1_97EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_97F7:
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
        db  000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [9790h]
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 069h, 08Fh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_1_A034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_A034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_1_A04E
        mov    esi, 2
loc_1_A04E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 025h, 08Fh, 0FEh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_1_A082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_A082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0EDh, 08Eh, 0FEh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_1_A0B1
        push   eax
        call   sub_1_A120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_A0B1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_LTEXT ends
        end
