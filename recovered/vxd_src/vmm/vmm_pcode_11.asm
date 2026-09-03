; vmm_pcode_11.asm -- VMM object 11 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_11_0000:
        pushfd
        push   ebp
        push   esi
        push   edi
        cli
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    ebp, ecx
        shl    ecx, 0ah
loc_11_001B:
        mov    edx, dword ptr [esi]
        xchg   dword ptr [edi], edx
        mov    dword ptr [esi], edx
        add    esi, 4
        add    edi, 4
        loop   loc_11_001B
        mov    ecx, ebp
        shr    esi, 0ch
        shr    edi, 0ch
        sub    esi, ecx
        sub    edi, ecx
        lea    esi, [esi*4 - 800000h]
        lea    edi, [edi*4 - 800000h]
loc_11_0043:
        mov    edx, dword ptr [esi]
        xchg   dword ptr [edi], edx
        mov    dword ptr [esi], edx
        add    esi, 4
        add    edi, 4
        loop   loc_11_0043
        mov    cr3, eax
        pop    edi
        pop    esi
        pop    ebp
        popfd
        ret    0ch
        db  000h, 000h, 000h, 000h, 000h
sub_11_0060:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  0A3h, 024h, 0EFh, 000h, 000h
        db  0C7h, 005h, 0C0h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 07Ch, 0F8h, 000h, 000h
        db  02Bh, 00Dh, 0F4h, 0FAh, 000h, 000h
        db  089h, 00Dh, 0B4h, 0F8h, 000h, 000h
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 002h
        je     NEAR PTR loc_11_012F
        db  08Bh, 03Dh, 058h, 0F9h, 000h, 000h
        db  08Bh, 035h, 074h, 0F8h, 000h, 000h
        lea    ebx, [esi*4 - 800000h]
        or     edi, edi
        je     SHORT loc_11_00D0
loc_11_00B3:
        test   byte ptr [ebx + 1], 2
        je     SHORT loc_11_00C7
        push   0
        push   1
        push   esi
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
loc_11_00C7:
        inc    esi
        dec    edi
        add    ebx, 4
        or     edi, edi
        jne    SHORT loc_11_00B3
loc_11_00D0:
        db  0CDh, 020h, 005h, 000h, 027h, 000h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_11_012F
loc_11_00DC:
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_11_0129
        mov    ebx, dword ptr [esi + 13h]
        mov    edi, dword ptr [esi + 17h]
        or     ebx, ebx
        je     SHORT loc_11_0129
loc_11_00EC:
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_11_00F9
        cmp    eax, 3
        jne    SHORT loc_11_0121
loc_11_00F9:
        mov    eax, dword ptr [edi]
        push   0
        mov    ecx, dword ptr [edi + 4]
        mov    edx, eax
        and    edx, 0fffh
        add    ecx, edx
        add    ecx, 0fffh
        shr    ecx, 0ch
        push   ecx
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
loc_11_0121:
        dec    ebx
        add    edi, 10h
        or     ebx, ebx
        jne    SHORT loc_11_00EC
loc_11_0129:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_11_00DC
loc_11_012F:
        call   sub_11_016E
        sub    esi, esi
        sub    edi, edi
loc_11_0138:
        push   edi
        lea    eax, [ebp - 18h]
        push   eax
        push   esi
        db  0E8h, 071h, 00Ah, 0F5h, 0FFh
        add    esp, 0ch
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_11_015D
        cmp    dword ptr [ebp - 10h], edi
        je     SHORT loc_11_0138
        lea    eax, [ebp - 18h]
        push   eax
        call   dword ptr [ebp - 10h]
        add    esp, 4
        jmp    SHORT loc_11_0138
loc_11_015D:
        db  0C7h, 005h, 0B4h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_11_016E:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        db  0A1h, 010h, 0F7h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        db  08Bh, 03Dh, 064h, 0FAh, 000h, 000h
        or     edi, edi
        je     NEAR PTR loc_11_0219
loc_11_0194:
        mov    esi, dword ptr [edi + 14h]
        push   0
        push   0
        push   dword ptr [edi + 8]
        push   esi
        db  0FFh, 015h, 080h, 0D4h, 000h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_11_0239
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 10h]
        push   esi
        call   0
        mov    esi, dword ptr [edi + 4]
        mov    ebx, dword ptr [edi + 8]
        cmp    esi, 110h
        jae    SHORT loc_11_01FD
loc_11_01CA:
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     SHORT loc_11_01FD
        mov    ebp, dword ptr [edi + 0ch]
        test   ebp, 1
        je     SHORT loc_11_01EF
        push   1
        push   1
        push   esi
        push   dword ptr [esp + 1ch]
        push   esi
        db  0FFh, 015h, 0A0h, 0D4h, 000h, 000h
        add    esp, 14h
loc_11_01EF:
        shr    ebp, 1
        mov    dword ptr [edi + 0ch], ebp
        inc    esi
        cmp    esi, 110h
        jb     SHORT loc_11_01CA
loc_11_01FD:
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_11_020F
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        call   eax
        add    esp, 8
loc_11_020F:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    NEAR PTR loc_11_0194
loc_11_0219:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], 1
        mov    eax, dword ptr [eax + 4]
        movzx  ecx, word ptr [eax + 703h]
        shl    ecx, 4
        and    word ptr [eax + ecx + 2], 0fffeh
loc_11_0239:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h, 000h
sub_11_0248:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  0A1h, 078h, 0F6h, 000h, 000h
        mov    edi, eax
        shl    edi, 2
        db  08Bh, 00Dh, 070h, 0F6h, 000h, 000h
        inc    ecx
        sub    ecx, eax
        shl    eax, 0ch
        or     eax, 0a07h
        add    edi, dword ptr [ebx - 10h]
        cld
loc_11_0270:
        stosd
        add    eax, 1000h
        loop   loc_11_0270
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
_PTEXT ends
        end
