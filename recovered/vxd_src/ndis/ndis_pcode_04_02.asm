; ndis_pcode_04_02.asm -- NDIS object 4 segment _PTEXT  (part 2/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_5B90:
        sub    esp, 214h
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 220h]
        push   edi
        push   ebp
        xor    edi, edi
        mov    dword ptr [esp + 10h], edi
        mov    dword ptr [esp + 14h], edi
        mov    dword ptr [eax], 0c000009ah
        call   sub_4_5EF0
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_4_5CC3
        mov    eax, dword ptr [esp + 230h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    NEAR PTR loc_4_5C82
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_4_95D0
        mov    ebx, eax
        call   sub_4_5EF0
        test   eax, eax
        je     NEAR PTR loc_4_5CBD
        push   eax
        push   esi
        call   sub_4_5F80
        test   ebx, ebx
        je     NEAR PTR loc_4_5CA9
        mov    dword ptr [esi + 14h], ebx
        call   sub_4_5EF0
        cmp    eax, edi
        mov    ebp, eax
        je     NEAR PTR loc_4_5CBD
        push   ebp
        push   esi
        call   sub_4_5F80
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp + 4]
        push   eax
        call   sub_4_CD10
        test   eax, eax
        jne    NEAR PTR loc_4_5CBD
        push   edi
        mov    dword ptr [ebp], 0ffffh
        mov    eax, dword ptr [ebx + 74h]
        push   edi
        push   edi
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 18h], 1
        push   esi
        push   edi
        push   1
        push   ebx
        call   sub_4_6630
        mov    eax, dword ptr [ebx + 3ch]
        cmp    eax, 4
        jne    SHORT loc_4_5C5A
        db  0B8h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_5C6A
loc_4_5C5A:
        cmp    eax, 5
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5C6A
        xor    eax, eax
        lea    edi, [esp + 10h]
loc_4_5C6A:
        lea    ecx, [esp + 10h]
        push   1
        push   ecx
        push   edi
        push   eax
        mov    eax, dword ptr [ebx + 38h]
        push   eax
        call   sub_4_9350
        test   eax, eax
        je     SHORT loc_4_5CA9
        jmp    SHORT loc_4_5CA1
loc_4_5C82:
        cmp    eax, 1
        jne    SHORT loc_4_5CA9
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [ecx + 4]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5C9A:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_5CA9
loc_4_5CA1:
        mov    dword ptr [esp + 10h], 0
loc_4_5CA9:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_4_5CE4
        mov    eax, dword ptr [esp + 228h]
        mov    dword ptr [eax], 0c0000001h
loc_4_5CBD:
        push   esi
        call   sub_4_5ED0
loc_4_5CC3:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_5CD7
        mov    eax, dword ptr [esp + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5CD4:
        add    esp, 4
loc_4_5CD7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 214h
        ret    0ch
loc_4_5CE4:
        xor    edi, edi
        mov    ebx, 100h
loc_4_5CEB:
        mov    ecx, dword ptr [esp + 14h]
        lea    eax, [esp + 18h]
        lea    edx, [esp + 24h]
        push   eax
        lea    ebp, [esp + 24h]
        push   edx
        lea    eax, [esp + 24h]
        push   ebp
        lea    edx, [esp + 130h]
        push   edi
        mov    dword ptr [esp + 2ch], ebx
        push   eax
        mov    dword ptr [esp + 2ch], ebx
        push   edx
        mov    dword ptr [esp + 3ch], edi
        push   ecx
        mov    ecx, dword ptr [esp + 2ch]
        inc    dword ptr [esp + 30h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5D27:
        add    esp, 20h
        test   eax, eax
        jne    NEAR PTR loc_4_5E16
        cmp    dword ptr [esp + 18h], edi
        je     SHORT loc_4_5CEB
        cmp    dword ptr [esp + 20h], 3
        jne    NEAR PTR loc_4_5DC5
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   esi
        call   sub_4_5FA0
        test   eax, eax
        je     SHORT loc_4_5D6C
        lea    ecx, [esp + 124h]
        add    eax, 4
        push   ecx
        push   eax
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_5CEB
        jmp    NEAR PTR loc_4_5CBD
loc_4_5D6C:
        mov    eax, dword ptr [esp + 230h]
        cmp    dword ptr [eax + 8], edi
        jne    NEAR PTR loc_4_5CEB
        cmp    dword ptr [esp + 24h], 4
        jbe    NEAR PTR loc_4_5CEB
        call   sub_4_5EF0
        mov    ebp, eax
        test   ebp, ebp
        je     NEAR PTR loc_4_5CBD
        push   ebp
        push   esi
        call   sub_4_5F80
        lea    eax, [esp + 124h]
        lea    ecx, [ebp + 4]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        jne    NEAR PTR loc_4_5CBD
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 18h], edi
        jmp    NEAR PTR loc_4_5CEB
loc_4_5DC5:
        call   sub_4_5EF0
        mov    ebp, eax
        test   ebp, ebp
        je     NEAR PTR loc_4_5CBD
        push   ebp
        push   esi
        call   sub_4_5F80
        lea    eax, [esp + 124h]
        lea    ecx, [ebp + 4]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        jne    NEAR PTR loc_4_5CBD
        lea    eax, [esp + 24h]
        lea    ecx, [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        jne    NEAR PTR loc_4_5CBD
        mov    dword ptr [ebp + 18h], 2
        jmp    NEAR PTR loc_4_5CEB
loc_4_5E16:
        mov    eax, dword ptr [esp + 22ch]
        mov    ecx, dword ptr [esp + 228h]
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx], 0
        jmp    NEAR PTR loc_4_5CC3
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_5E40:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        push   edi
        mov    ebx, dword ptr [esp + 20h]
        push   ebp
        push   ebx
        mov    dword ptr [esi], 0c0000001h
        call   sub_4_61A0
        test   eax, eax
        jne    SHORT loc_4_5E73
        pop    ebp
        mov    dword ptr [esi], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_5E73:
        push   ebp
        push   ebx
        call   sub_4_5FC0
        test   eax, eax
        je     SHORT loc_4_5EBB
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_5EA4
        mov    ecx, dword ptr [esp + 24h]
        test   ecx, ecx
        je     SHORT loc_4_5E90
        cmp    ecx, 1
        jne    SHORT loc_4_5EBB
loc_4_5E90:
        add    eax, 18h
        pop    ebp
        mov    dword ptr [esi], 0
        mov    dword ptr [eax], ecx
        mov    dword ptr [edi], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_5EA4:
        mov    ecx, dword ptr [esp + 24h]
        push   edi
        push   ecx
        push   eax
        push   ebx
        call   sub_4_6290
        test   eax, eax
        jne    SHORT loc_4_5EBB
        mov    dword ptr [esi], 0
loc_4_5EBB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_5ED0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        test   eax, eax
        je     SHORT loc_4_5EE8
loc_4_5ED9:
        mov    esi, dword ptr [eax + 24h]
        push   eax
        call   sub_4_5F20
        mov    eax, esi
        test   esi, esi
        jne    SHORT loc_4_5ED9
loc_4_5EE8:
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_5EF0:
        push   edi
        push   0
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5EFA:
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     SHORT loc_4_5F0E
        mov    edi, edx
        xor    eax, eax
        mov    ecx, 0ah
        rep stosd
loc_4_5F0E:
        mov    eax, edx
        pop    edi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_5F20:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     SHORT loc_4_5F70
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_4_5F3B
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5F38:
        add    esp, 8
loc_4_5F3B:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_5F4D
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5F4A:
        add    esp, 8
loc_4_5F4D:
        cmp    dword ptr [esi + 18h], 3
        jne    SHORT loc_4_5F65
        mov    eax, dword ptr [esi + 20h]
        test   eax, eax
        je     SHORT loc_4_5F65
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5F62:
        add    esp, 8
loc_4_5F65:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5F6D:
        add    esp, 8
loc_4_5F70:
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_5F80:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [ecx + 24h], edx
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_5FA0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 24h]
        test   eax, eax
        je     SHORT loc_4_5FBA
        mov    ecx, dword ptr [esp + 8]
loc_4_5FAF:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_5FBA
        mov    eax, dword ptr [eax + 24h]
        test   eax, eax
        jne    SHORT loc_4_5FAF
loc_4_5FBA:
        ret    8
        db  08Dh, 049h, 000h
sub_4_5FC0:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 24h]
        test   edi, edi
        je     SHORT loc_4_5FF0
        mov    esi, dword ptr [esp + 14h]
        xor    ebx, ebx
loc_4_5FD4:
        cmp    dword ptr [edi + 8], ebx
        je     SHORT loc_4_5FE9
        push   1
        lea    eax, [edi + 4]
        push   eax
        push   esi
        call   sub_4_CE10
        test   al, al
        jne    SHORT loc_4_5FF0
loc_4_5FE9:
        mov    edi, dword ptr [edi + 24h]
        test   edi, edi
        jne    SHORT loc_4_5FD4
loc_4_5FF0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_6000:
        sub    esp, 18h
        mov    eax, 0fh
        mov    word ptr [esp + 4], ax
        push   ebx
        mov    word ptr [esp + 0ah], ax
        push   esi
        push   edi
        mov    eax, 0bh
        mov    word ptr [esp + 18h], ax
        push   ebp
        mov    word ptr [esp + 1eh], ax
        mov    ecx, dword ptr [esp + 38h]
        db  0C7h, 044h, 024h, 018h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 14h]
        push   2
        add    eax, 3ch
        lea    edx, [esp + 18h]
        db  0C7h, 044h, 024h, 024h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax]
        push   edx
        mov    ebp, dword ptr [esp + 34h]
        mov    dword ptr [esp + 2ch], ecx
        lea    ecx, [esp + 18h]
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [esp + 40h]
        push   eax
        push   ecx
        push   ebp
        call   sub_4_5E40
        cmp    dword ptr [ebp], 0
        je     SHORT loc_4_608D
        lea    eax, [esp + 1ch]
        push   2
        mov    ecx, dword ptr [esp + 3ch]
        push   eax
        lea    edx, [esp + 18h]
        push   ecx
        push   edx
        push   ebp
        call   sub_4_5E40
        cmp    dword ptr [ebp], 0
        jne    NEAR PTR loc_4_617B
loc_4_608D:
        mov    eax, dword ptr [esp + 10h]
        xor    ebx, ebx
        xor    edi, edi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi], bl
        je     SHORT loc_4_60BC
loc_4_609C:
        mov    al, byte ptr [esi]
        cmp    al, 39h
        ja     SHORT loc_4_60A6
        cmp    al, 30h
        jae    SHORT loc_4_60BC
loc_4_60A6:
        cmp    al, 66h
        ja     SHORT loc_4_60AE
        cmp    al, 61h
        jae    SHORT loc_4_60BC
loc_4_60AE:
        cmp    al, 46h
        ja     SHORT loc_4_60B6
        cmp    al, 41h
        jae    SHORT loc_4_60BC
loc_4_60B6:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_4_609C
loc_4_60BC:
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_4_6164
loc_4_60C5:
        lea    ecx, [esi + edi]
        mov    al, byte ptr [ecx]
        cmp    al, 39h
        ja     SHORT loc_4_60D2
        cmp    al, 30h
        jae    SHORT loc_4_60E6
loc_4_60D2:
        cmp    al, 66h
        ja     SHORT loc_4_60DA
        cmp    al, 61h
        jae    SHORT loc_4_60E6
loc_4_60DA:
        cmp    al, 46h
        ja     NEAR PTR loc_4_6164
        cmp    al, 41h
        jb     SHORT loc_4_6164
loc_4_60E6:
        cmp    al, 39h
        jbe    SHORT loc_4_60F6
        cmp    al, 5ah
        jbe    SHORT loc_4_60F2
        sub    al, 57h
        jmp    SHORT loc_4_60F8
loc_4_60F2:
        sub    al, 37h
        jmp    SHORT loc_4_60F8
loc_4_60F6:
        sub    al, 30h
loc_4_60F8:
        mov    cl, byte ptr [ecx + 1]
        cmp    cl, 39h
        ja     SHORT loc_4_6105
        cmp    cl, 30h
        jae    SHORT loc_4_6119
loc_4_6105:
        cmp    cl, 66h
        ja     SHORT loc_4_610F
        cmp    cl, 61h
        jae    SHORT loc_4_6119
loc_4_610F:
        cmp    cl, 46h
        ja     SHORT loc_4_6146
        cmp    cl, 41h
        jb     SHORT loc_4_6146
loc_4_6119:
        mov    dl, byte ptr [esi + edi + 1]
        inc    edi
        cmp    dl, 39h
        jbe    SHORT loc_4_613A
        cmp    dl, 5ah
        jbe    SHORT loc_4_6131
        xor    ecx, ecx
        mov    cl, dl
        sub    ecx, 57h
        jmp    SHORT loc_4_6141
loc_4_6131:
        xor    ecx, ecx
        mov    cl, dl
        sub    ecx, 37h
        jmp    SHORT loc_4_6141
loc_4_613A:
        xor    ecx, ecx
        mov    cl, dl
        sub    ecx, 30h
loc_4_6141:
        shl    al, 4
        add    al, cl
loc_4_6146:
        mov    cl, byte ptr [esi + edi + 1]
        cmp    cl, 2eh
        je     SHORT loc_4_6154
        cmp    cl, 2dh
        jne    SHORT loc_4_6155
loc_4_6154:
        inc    edi
loc_4_6155:
        inc    edi
        mov    byte ptr [esi + ebx], al
        inc    ebx
        cmp    byte ptr [esi + edi], 0
        jne    NEAR PTR loc_4_60C5
loc_4_6164:
        test   edi, edi
        jne    SHORT loc_4_616F
        mov    dword ptr [ebp], 0c0000001h
loc_4_616F:
        mov    eax, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx], ebx
loc_4_617B:
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [esp + 24h]
        pop    ebp
        pop    edi
        mov    edx, dword ptr [eax + 14h]
        pop    esi
        pop    ebx
        mov    dword ptr [edx + 3ch], ecx
        add    esp, 18h
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_61A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_CE10
        test   al, al
        je     SHORT loc_4_61CB
        mov    dword ptr [ebp - 4], 0
        mov    edi, 1
loc_4_61CB:
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_CE10
        test   al, al
        je     SHORT loc_4_61E8
        mov    dword ptr [ebp - 4], 0
        mov    edi, 1
loc_4_61E8:
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_CE10
        test   al, al
        je     SHORT loc_4_620C
        mov    edi, 1
        push   ebx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        pop    ebx
        movzx  eax, ah
        mov    dword ptr [ebp - 4], eax
loc_4_620C:
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_CE10
        test   al, al
        je     SHORT loc_4_6230
        mov    edi, 1
        push   ebx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        pop    ebx
        movzx  eax, al
        mov    dword ptr [ebp - 4], eax
loc_4_6230:
        test   edi, edi
        jne    SHORT loc_4_6242
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_6242:
        call   sub_4_5EF0
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_625B
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_625B:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        add    esi, 18h
        call   sub_4_5F80
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        pop    edi
        mov    dword ptr [esi + 4], eax
        xor    eax, eax
        mov    dword ptr [esi], 1
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6290:
        mov    eax, dword ptr [esp + 0ch]
        sub    esp, 14h
        cmp    eax, 3
        push   ebx
        mov    dword ptr [esp + 14h], 1
        push   esi
        push   edi
        push   ebp
        ja     SHORT loc_4_62AF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_62AF:
        mov    dword ptr [esp + 20h], 0
        mov    esi, dword ptr [esp + 2ch]
        jmp    NEAR PTR loc_4_64A1
loc_4_62C0:
        mov    esi, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_4_6550
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_4_64A1
loc_4_62D5:
        mov    esi, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_4_64F0
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_4_64A1
loc_4_62EA:
        mov    esi, dword ptr [esp + 2ch]
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_4_6301
        mov    dword ptr [esp + 20h], 0
        jmp    NEAR PTR loc_4_64A1
loc_4_6301:
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        je     SHORT loc_4_6343
        cmp    dword ptr [eax + 3ch], 6
        jne    SHORT loc_4_6343
        lea    edi, [esi + 0ch]
        lea    eax, [esp + 14h]
        push   edi
        push   eax
        call   sub_4_CD70
        test   eax, eax
        jne    SHORT loc_4_6358
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_6336
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6333:
        add    esp, 8
loc_4_6336:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], ecx
loc_4_6343:
        mov    ebx, dword ptr [esi + 10h]
        lea    eax, [esi + 0ch]
        lea    ecx, [esi + 1ch]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], ebx
        jmp    NEAR PTR loc_4_64A1
loc_4_6358:
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_4_6367:
        mov    esi, dword ptr [esp + 2ch]
        xor    ecx, ecx
        mov    dword ptr [esp + 1ch], 1
        cmp    word ptr [esi + 0ch], cx
        jbe    SHORT loc_4_6395
        lea    edx, [esi + 10h]
loc_4_637E:
        mov    eax, dword ptr [edx]
        cmp    byte ptr [eax + ecx], 2ch
        jne    SHORT loc_4_638A
        inc    dword ptr [esp + 1ch]
loc_4_638A:
        inc    ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0ch]
        cmp    eax, ecx
        ja     SHORT loc_4_637E
loc_4_6395:
        mov    eax, dword ptr [esp + 1ch]
        push   0
        shl    eax, 3
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_63A4:
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_63BC
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_4_63BC:
        mov    eax, dword ptr [esi + 10h]
        lea    ecx, [esp + 1ch]
        push   eax
        push   edi
        push   ecx
        call   sub_4_6580
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0ch]
        add    eax, 2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_63DC:
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_6401
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_63EF:
        add    esp, 8
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_4_6401:
        mov    ebx, dword ptr [esp + 10h]
        xor    edx, edx
        mov    ecx, dword ptr [edi + 4]
loc_4_640A:
        cmp    edx, dword ptr [esp + 1ch]
        jae    SHORT loc_4_6420
loc_4_6410:
        mov    al, byte ptr [ecx]
        mov    ebp, ebx
        inc    ebx
        inc    ecx
        mov    byte ptr [ebp], al
        test   al, al
        jne    SHORT loc_4_6410
        inc    edx
        jmp    SHORT loc_4_640A
loc_4_6420:
        push   0
        inc    ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6429:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        mov    byte ptr [ebx - 1], 0
        sub    ebx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        mov    word ptr [esp + 16h], bx
        mov    dword ptr [esp + 18h], eax
        mov    word ptr [esp + 14h], bx
        mov    eax, dword ptr [ecx + 14h]
        test   eax, eax
        je     SHORT loc_4_6491
        cmp    dword ptr [eax + 3ch], 6
        jne    SHORT loc_4_6491
        lea    eax, [esp + 14h]
        lea    ecx, [esi + 1ch]
        push   eax
        push   ecx
        call   sub_4_CD70
        push   0
        test   eax, eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        jne    SHORT loc_4_647A
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6475:
        add    esp, 8
        jmp    SHORT loc_4_64A1
loc_4_647A:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_647F:
        add    esp, 8
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_4_6491:
        lea    edx, [esi + 1ch]
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
loc_4_64A1:
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_4_64B7
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
loc_4_64B7:
        add    esi, 18h
        mov    eax, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 34h]
        pop    ebp
        pop    edi
        mov    dword ptr [esi], eax
        xor    eax, eax
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    10h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_64F0:
        push   ebx
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        cmp    byte ptr [ecx], al
        je     SHORT loc_4_6547
loc_4_64FB:
        mov    dl, byte ptr [ecx]
        cmp    dl, 30h
        jb     SHORT loc_4_6515
        cmp    dl, 39h
        ja     SHORT loc_4_6515
        sub    eax, 3
        xor    ebx, ebx
        shl    eax, 4
        mov    bl, dl
        add    eax, ebx
        jmp    SHORT loc_4_6541
loc_4_6515:
        cmp    dl, 61h
        jb     SHORT loc_4_652C
        cmp    dl, 66h
        ja     SHORT loc_4_652C
        shl    eax, 4
        xor    ebx, ebx
        mov    bl, dl
        lea    eax, [ebx + eax - 57h]
        jmp    SHORT loc_4_6541
loc_4_652C:
        cmp    dl, 41h
        jb     SHORT loc_4_6547
        cmp    dl, 46h
        ja     SHORT loc_4_6547
        shl    eax, 4
        xor    ebx, ebx
        mov    bl, dl
        lea    eax, [ebx + eax - 37h]
loc_4_6541:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_4_64FB
loc_4_6547:
        pop    ebx
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_6550:
        push   ebx
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        cmp    byte ptr [ecx], al
        je     SHORT loc_4_6578
loc_4_655B:
        mov    bl, byte ptr [ecx]
        cmp    bl, 30h
        jb     SHORT loc_4_6578
        cmp    bl, 39h
        ja     SHORT loc_4_6578
        xor    edx, edx
        lea    eax, [eax + eax*4]
        mov    dl, bl
        inc    ecx
        lea    eax, [edx + eax*2 - 30h]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_4_655B
loc_4_6578:
        pop    ebx
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_6580:
        mov    edx, dword ptr [esp + 0ch]
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    esi, esi
loc_4_658A:
        mov    al, byte ptr [edx]
        cmp    al, 20h
        je     SHORT loc_4_6598
        cmp    al, 9
        jb     SHORT loc_4_659B
        cmp    al, 0dh
        ja     SHORT loc_4_659B
loc_4_6598:
        inc    edx
        jmp    SHORT loc_4_658A
loc_4_659B:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 14h]
        add    eax, 2
loc_4_65A6:
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     SHORT loc_4_6627
        cmp    dword ptr [ecx], esi
        jbe    SHORT loc_4_6627
        xor    ebp, ebp
        mov    edi, edx
        test   bl, bl
        je     SHORT loc_4_65D5
loc_4_65B8:
        mov    bl, byte ptr [edx]
        cmp    bl, 2ch
        jne    SHORT loc_4_65C3
        test   ebp, ebp
        je     SHORT loc_4_65D5
loc_4_65C3:
        cmp    bl, 22h
        jne    SHORT loc_4_65CF
        cmp    ebp, 1
        sbb    ebp, ebp
        neg    ebp
loc_4_65CF:
        inc    edx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_4_65B8
loc_4_65D5:
        lea    ebp, [edx - 1]
        cmp    byte ptr [edx], 0
        je     SHORT loc_4_65DE
        inc    edx
loc_4_65DE:
        cmp    ebp, edi
        jbe    SHORT loc_4_65F9
loc_4_65E2:
        mov    bl, byte ptr [ebp]
        cmp    bl, 20h
        je     SHORT loc_4_65F4
        cmp    bl, 9
        jb     SHORT loc_4_65F9
        cmp    bl, 0dh
        ja     SHORT loc_4_65F9
loc_4_65F4:
        dec    ebp
        cmp    ebp, edi
        ja     SHORT loc_4_65E2
loc_4_65F9:
        mov    byte ptr [ebp + 1], 0
        inc    ebp
        inc    ebp
        cmp    dword ptr [ecx], esi
        jbe    SHORT loc_4_660F
        sub    ebp, edi
        mov    word ptr [eax], bp
        mov    word ptr [eax - 2], bp
        mov    dword ptr [eax + 2], edi
loc_4_660F:
        add    eax, 8
        inc    esi
loc_4_6613:
        mov    bl, byte ptr [edx]
        cmp    bl, 20h
        je     SHORT loc_4_6624
        cmp    bl, 9
        jb     SHORT loc_4_65A6
        cmp    bl, 0dh
        ja     SHORT loc_4_65A6
loc_4_6624:
        inc    edx
        jmp    SHORT loc_4_6613
loc_4_6627:
        pop    ebp
        mov    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_4_6630:
        sub    esp, 30h
        mov    dword ptr [esp + 28h], 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        cmp    dword ptr [esp + 48h], 3
        mov    dword ptr [esp + 28h], ebp
        jne    SHORT loc_4_665F
        mov    edi, dword ptr [esp + 50h]
        mov    word ptr [edi], bp
        lea    esi, [edi + 8]
        mov    word ptr [edi + 2], bp
        mov    dword ptr [edi + 4], ebp
        jmp    SHORT loc_4_6667
loc_4_665F:
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 50h]
loc_4_6667:
        mov    eax, dword ptr [esp + 44h]
        push   2
        mov    ecx, dword ptr [eax + 38h]
        lea    eax, [esp + 38h]
        mov    dword ptr [esp + 30h], ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_667F:
        mov    ecx, dword ptr [esp + 40h]
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_68A7
loc_4_668E:
        lea    eax, [esp + 10h]
        push   0
        mov    edx, dword ptr [esp + 50h]
        push   eax
        push   edx
        push   ecx
        lea    ecx, [esp + 40h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_66A5:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_4_68A7
        lea    eax, [esp + 18h]
        push   0
        mov    ecx, dword ptr [esp + 34h]
        push   10h
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_66C3:
        mov    dword ptr [esp + 24h], eax
        add    esp, 10h
        cmp    eax, 1ah
        jne    SHORT loc_4_66D7
        mov    dword ptr [esp + 14h], 0
loc_4_66D7:
        cmp    dword ptr [esp + 14h], 0
        jne    NEAR PTR loc_4_68A7
        mov    eax, dword ptr [esp + 48h]
        cmp    eax, 3
        ja     NEAR PTR loc_4_6898
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_66F6:
        add    ebp, 10h
        cmp    dword ptr [esp + 54h], ebp
        jb     NEAR PTR loc_4_6898
        lea    eax, [esp + 18h]
        push   10h
        push   eax
        mov    eax, dword ptr [esp + 30h]
        add    eax, edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6716:
        add    dword ptr [esp + 28h], 10h
        jmp    NEAR PTR loc_4_6898
loc_4_6720:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        push   edi
        call   sub_4_5FA0
        test   eax, eax
        jne    NEAR PTR loc_4_6898
        call   sub_4_5EF0
        mov    ebx, eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 10h]
        dec    eax
        cmp    eax, 3
        ja     SHORT loc_4_67B1
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_6751:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_4_67B1
loc_4_675A:
        mov    eax, dword ptr [esp + 1ch]
        and    eax, 0ffffh
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_4_67B1
loc_4_6768:
        xor    eax, eax
        mov    al, byte ptr [esp + 19h]
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_4_67B1
loc_4_6773:
        xor    eax, eax
        push   1
        mov    ax, word ptr [esp + 1eh]
        lea    ecx, [esp + 3ch]
        lea    edx, [esp + 40h]
        push   ecx
        push   edx
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esp + 38h]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_679C:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_67B1
        cmp    byte ptr [esp + 3ch], 31h
        jne    SHORT loc_4_67B1
        mov    dword ptr [ebx + 1ch], 2
loc_4_67B1:
        mov    dword ptr [ebx + 18h], 0
        push   ebx
        push   edi
        call   sub_4_5F80
        jmp    NEAR PTR loc_4_6898
loc_4_67C4:
        add    ebp, 10h
        jmp    NEAR PTR loc_4_6898
loc_4_67CC:
        mov    eax, dword ptr [esp + 10h]
        dec    eax
        cmp    eax, 3
        ja     NEAR PTR loc_4_6898
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_67E1:
        inc    dword ptr [edi + 4]
        mov    byte ptr [esi], 3
        mov    byte ptr [esi + 1], 1
        test   byte ptr [esp + 24h], 1
        mov    word ptr [esi + 2], 0
        jne    SHORT loc_4_67FE
        mov    word ptr [esi + 2], 1
loc_4_67FE:
        mov    dword ptr [esi + 8], 0
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 20h]
        sub    eax, dword ptr [esp + 1ch]
        inc    eax
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_4_6895
loc_4_681A:
        inc    dword ptr [edi + 4]
        mov    byte ptr [esi], 1
        mov    byte ptr [esi + 1], 1
        mov    word ptr [esi + 2], 1
        mov    dword ptr [esi + 8], 0
        mov    eax, dword ptr [esp + 1ch]
        and    eax, 0ffffh
        mov    dword ptr [esi + 4], eax
        xor    eax, eax
        mov    ax, word ptr [esp + 1eh]
        mov    ecx, dword ptr [esp + 1ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        inc    eax
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_4_6895
loc_4_6856:
        xor    eax, eax
        inc    dword ptr [edi + 4]
        mov    byte ptr [esi], 4
        mov    byte ptr [esi + 1], 1
        mov    word ptr [esi + 2], 0
        mov    al, byte ptr [esp + 19h]
        mov    dword ptr [esi + 4], eax
        jmp    SHORT loc_4_6895
loc_4_6871:
        xor    eax, eax
        inc    dword ptr [edi + 4]
        mov    byte ptr [esi], 2
        mov    byte ptr [esi + 1], 3
        mov    word ptr [esi + 2], 0
        mov    ax, word ptr [esp + 1ah]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0ch], 1
loc_4_6895:
        add    esi, 10h
loc_4_6898:
        cmp    dword ptr [esp + 14h], 0
        mov    ecx, dword ptr [esp + 30h]
        je     NEAR PTR loc_4_668E
loc_4_68A7:
        mov    eax, dword ptr [esp + 58h]
        test   eax, eax
        je     SHORT loc_4_68B5
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [eax], ecx
loc_4_68B5:
        mov    eax, dword ptr [esp + 5ch]
        test   eax, eax
        je     SHORT loc_4_68BF
        mov    dword ptr [eax], ebp
loc_4_68BF:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret    1ch
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 08Dh, 064h, 024h, 000h
loc_4_6900:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_95D0
        mov    ecx, dword ptr [esp + 0ch]
        test   eax, eax
        jne    SHORT loc_4_6929
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [ecx], 0c0000001h
        jmp    SHORT loc_4_6950
loc_4_6929:
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 18h]
        push   edi
        push   esi
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 24h]
        push   ecx
        push   edx
        mov    ecx, dword ptr [esp + 24h]
        push   ecx
        push   0
        push   eax
        call   sub_4_6630
loc_4_6950:
        mov    dword ptr [esi], 0
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        ret
        db  090h
loc_4_6960:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esp + 4], 0
        push   edi
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_95D0
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_6992
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0c0000001h
        jmp    SHORT loc_4_69E6
loc_4_6992:
        lea    eax, [esp + 8]
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   2
        push   edi
        call   sub_4_6630
        add    dword ptr [esp + 8], 8
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 1ch]
        cmp    dword ptr [esi], eax
        jae    SHORT loc_4_69C4
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0c000009ah
        jmp    SHORT loc_4_69E6
loc_4_69C4:
        mov    eax, dword ptr [esp + 8]
        push   0
        mov    ecx, dword ptr [esp + 1ch]
        push   0
        push   eax
        push   ecx
        push   0
        push   3
        push   edi
        call   sub_4_6630
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], 0
loc_4_69E6:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        mov    dword ptr [esi], eax
        pop    esi
        add    esp, 4
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_6A00:
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6A0A:
        ret    10h
        db  08Dh, 049h, 000h
sub_4_6A10:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        test   edi, edi
        jne    SHORT loc_4_6A24
        push   esi
        call   sub_4_6BA0
loc_4_6A24:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi + 24h]
        push   eax
        push   edi
        push   ecx
        mov    eax, dword ptr [esi + 1ch]
        call   dword ptr [eax + 3ch]
        test   edi, edi
        je     SHORT loc_4_6A55
        mov    eax, dword ptr [esi + 14h]
        push   eax
        call   sub_4_6DD0
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_4_9EF0
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6A52:
        add    esp, 8
loc_4_6A55:
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_6A60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   esi
        call   sub_4_6BF0
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_4_9D90
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_4_9EF0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        call   sub_4_6DD0
        mov    eax, dword ptr [esi + 5ch]
        test   eax, eax
        je     SHORT loc_4_6AAA
loc_4_6A92:
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        mov    dword ptr [esi + 5ch], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6AA0:
        add    esp, 8
        mov    eax, dword ptr [esi + 5ch]
        test   eax, eax
        jne    SHORT loc_4_6A92
loc_4_6AAA:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6AB2:
        add    esp, 8
        pop    esi
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6AC0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 6], 0
        je     SHORT loc_4_6ACF
        xor    al, al
        ret    4
loc_4_6ACF:
        inc    word ptr [eax + 4]
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_6AE0:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        dec    cx
        mov    word ptr [eax + 4], cx
        xor    ax, ax
        cmp    ax, cx
        sbb    al, al
        inc    al
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_6B00:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 4], cx
        mov    byte ptr [eax + 6], cl
        ret    4
        db  08Bh, 044h, 024h, 004h, 080h, 078h, 006h, 000h, 074h, 005h, 032h, 0C0h, 0C2h, 004h, 000h, 0C6h
        db  040h, 006h, 001h, 0B0h, 001h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_6B30:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 0ah], 0
        je     SHORT loc_4_6B3F
        xor    al, al
        ret    4
loc_4_6B3F:
        inc    word ptr [eax + 8]
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_6B50:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 8]
        dec    cx
        mov    word ptr [eax + 8], cx
        xor    ax, ax
        cmp    ax, cx
        sbb    al, al
        inc    al
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_6B70:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 8], cx
        mov    byte ptr [eax + 0ah], cl
        ret    4
        db  08Bh, 044h, 024h, 004h, 080h, 078h, 00Ah, 000h, 074h, 005h, 032h, 0C0h, 0C2h, 004h, 000h, 0C6h
        db  040h, 00Ah, 001h, 0B0h, 001h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_6BA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   esi
        call   sub_4_6BC0
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_4_9D70
        mov    al, 1
        pop    esi
        ret    4
        db  090h
sub_4_6BC0:
        mov    ecx, dword ptr [esp + 8]
        cmp    byte ptr [ecx + 26h], 0
        je     SHORT loc_4_6BCF
        xor    al, al
        ret    8
loc_4_6BCF:
        mov    edx, dword ptr [ecx + 18h]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx + 18h], eax
        mov    al, 1
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6BF0:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 18h]
        cmp    edx, eax
        jne    SHORT loc_4_6C08
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 18h], eax
        ret    8
loc_4_6C08:
        cmp    dword ptr [edx + 4], eax
        je     SHORT loc_4_6C15
loc_4_6C0D:
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_4_6C0D
loc_4_6C15:
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6C30:
        mov    ecx, dword ptr [esp + 8]
        cmp    byte ptr [ecx + 12h], 0
        je     SHORT loc_4_6C3F
        xor    al, al
        ret    8
loc_4_6C3F:
        mov    edx, dword ptr [ecx + 8]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], edx
        mov    dword ptr [ecx + 8], eax
        mov    al, 1
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6C60:
        mov    edx, dword ptr [esp + 8]
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx + 8]
        cmp    ecx, esi
        jne    SHORT loc_4_6C7A
        mov    eax, dword ptr [ecx + 1ch]
        pop    esi
        mov    dword ptr [edx + 8], eax
        ret    8
loc_4_6C7A:
        test   esi, esi
        je     SHORT loc_4_6C98
loc_4_6C7E:
        mov    eax, dword ptr [esi + 1ch]
        cmp    ecx, eax
        je     SHORT loc_4_6C8B
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_6C7E
loc_4_6C8B:
        test   esi, esi
        je     SHORT loc_4_6C98
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [esi + 1ch], ecx
loc_4_6C98:
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_6CA0:
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 004h, 000h, 000h, 000h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6CC0:
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_6CE8
        mov    ecx, dword ptr [esp + 4]
loc_4_6CD2:
        cmp    eax, ecx
        je     SHORT loc_4_6CE2
        mov    edx, eax
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_6CD2
        ret    4
loc_4_6CE2:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
loc_4_6CE8:
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_6CF0:
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 004h, 000h, 000h, 000h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6D10:
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_6D38
        mov    ecx, dword ptr [esp + 4]
loc_4_6D22:
        cmp    eax, ecx
        je     SHORT loc_4_6D32
        mov    edx, eax
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_6D22
        ret    4
loc_4_6D32:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
loc_4_6D38:
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_6D40:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 64h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_6D5B
        lea    ecx, [esp + 7]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6D5B:
        push   esi
        call   sub_4_6CC0
        mov    eax, dword ptr [esi + 5ch]
        test   eax, eax
        je     SHORT loc_4_6D73
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6D70:
        add    esp, 8
loc_4_6D73:
        mov    eax, dword ptr [esi + 50h]
        test   eax, eax
        je     SHORT loc_4_6D85
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6D82:
        add    esp, 8
loc_4_6D85:
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_4_6D97
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6D94:
        add    esp, 8
loc_4_6D97:
        mov    eax, dword ptr [esi + 48h]
        test   eax, eax
        je     SHORT loc_4_6DA9
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6DA6:
        add    esp, 8
loc_4_6DA9:
        mov    eax, dword ptr [esi + 70h]
        test   eax, eax
        je     SHORT loc_4_6DBB
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6DB8:
        add    esp, 8
loc_4_6DBB:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6DC3:
        add    esp, 8
        pop    esi
        add    esp, 4
        ret    4
        db  08Dh, 049h, 000h
sub_4_6DD0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 8]
        test   edi, edi
        je     SHORT loc_4_6DE6
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_4_6AC0
loc_4_6DE6:
        lea    eax, [esi + 20h]
        push   eax
        call   sub_4_6AE0
        test   al, al
        je     SHORT loc_4_6E16
        test   byte ptr [esi + 40h], 4
        jne    SHORT loc_4_6E10
        mov    eax, dword ptr [esi + 28h]
        test   eax, eax
        je     SHORT loc_4_6E06
        mov    ecx, dword ptr [esi + 2ch]
        push   ecx
        call   eax
loc_4_6E06:
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 8]
        call   dword ptr [eax + 60h]
loc_4_6E10:
        push   esi
        call   sub_4_6D40
loc_4_6E16:
        test   edi, edi
        je     SHORT loc_4_6E20
        push   edi
        call   sub_4_6E30
loc_4_6E20:
        pop    edi
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6E30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_4_6AE0
        test   al, al
        je     SHORT loc_4_6E7F
        mov    eax, dword ptr [esi + 18h]
        push   eax
        call   dword ptr [esi + 58h]
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_6E7F
        lea    edi, [esi + 24h]
        push   edi
        call   sub_4_92D0
        push   edi
        call   sub_4_9310
        mov    eax, dword ptr [esi + 68h]
        test   eax, eax
        je     SHORT loc_4_6E74
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6E71:
        add    esp, 8
loc_4_6E74:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6E7C:
        add    esp, 8
loc_4_6E7F:
        pop    edi
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6E90:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        xor    edi, edi
        cmp    esi, edi
        jne    SHORT loc_4_6EAA
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_6EAA:
        lea    eax, [esi + 20h]
        push   eax
        call   sub_4_6AC0
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     SHORT loc_4_6ED7
        mov    bl, byte ptr [esp + 18h]
loc_4_6EBE:
        test   bl, bl
        je     SHORT loc_4_6EC6
        or     dword ptr [eax + 38h], 1
loc_4_6EC6:
        mov    ebp, dword ptr [eax + 4]
        push   eax
        call   sub_4_93F0
        mov    edi, eax
        test   ebp, ebp
        mov    eax, ebp
        jne    SHORT loc_4_6EBE
loc_4_6ED7:
        push   esi
        call   sub_4_6DD0
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_6EF0:
        lea    eax, [esp + 0ch]
        push   1
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        mov    edx, dword ptr [esp + 10h]
        push   ecx
        mov    eax, dword ptr [esp + 10h]
        push   edx
        mov    ecx, dword ptr [eax + 0f8h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6F11:
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6F20:
        lea    eax, [esp + 0ch]
        push   0
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        mov    edx, dword ptr [esp + 10h]
        push   ecx
        mov    eax, dword ptr [esp + 10h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6F3B:
        add    esp, 14h
        ret
        db  090h
loc_4_6F40:
        sub    esp, 4
        push   esi
        push   edi
        push   0
        call   sub_4_2C1E
        mov    esi, dword ptr [esp + 14h]
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_6F77
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        lea    edx, [esp + 8]
        push   eax
        push   ecx
        push   edi
        push   edx
        call   sub_4_2D51
        add    esp, 10h
        push   edi
        call   sub_4_2E4B
        add    esp, 4
loc_4_6F77:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6F81:
        add    esp, 8
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6F8C:
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_6FA0:
        mov    eax, dword ptr [esp + 4]
        push   0
        mov    ecx, dword ptr [eax + 38h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_6FAF:
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_6FC0:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 60h], 5
        ja     SHORT loc_4_6FD1
        mov    dword ptr [eax + 60h], 0
loc_4_6FD1:
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_6FE0:
        sub    esp, 4
        lea    eax, [esp]
        push   ebx
        push   esi
        mov    edx, 6
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   0
        mov    ecx, esi
        test   byte ptr [esi + 3dh], 20h
        push   eax
        je     SHORT loc_4_7048
        call   dword ptr [esi + 124h]
        and    dword ptr [esi + 3ch], 7fffffffh
        mov    ecx, dword ptr [esi + 18h]
        test   ecx, ecx
        je     SHORT loc_4_7090
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
loc_4_701C:
        mov    eax, dword ptr [esi + 8]
        mov    ebx, dword ptr [eax + 70h]
        mov    eax, dword ptr [ebx + 44h]
        mov    ebx, dword ptr [ecx + 0ch]
        mov    dword ptr [ebx + 3ch], eax
        mov    eax, dword ptr [ecx + 0ch]
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_703C
        mov    dword ptr [eax + 58h], edx
        jmp    SHORT loc_4_703F
loc_4_703C:
        mov    dword ptr [eax + 58h], edi
loc_4_703F:
        mov    ecx, dword ptr [ecx + 18h]
        test   ecx, ecx
        jne    SHORT loc_4_701C
        jmp    SHORT loc_4_7090
loc_4_7048:
        call   dword ptr [esi + 124h]
        and    dword ptr [esi + 3ch], 7fffffffh
        mov    ecx, dword ptr [esi + 18h]
        test   ecx, ecx
        je     SHORT loc_4_7090
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
loc_4_7066:
        mov    eax, dword ptr [esi + 8]
        mov    ebx, dword ptr [eax + 70h]
        mov    eax, dword ptr [ebx + 44h]
        mov    ebx, dword ptr [ecx + 0ch]
        mov    dword ptr [ebx + 3ch], eax
        mov    eax, dword ptr [ecx + 0ch]
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_7086
        mov    dword ptr [eax + 58h], edx
        jmp    SHORT loc_4_7089
loc_4_7086:
        mov    dword ptr [eax + 58h], edi
loc_4_7089:
        mov    ecx, dword ptr [ecx + 18h]
        test   ecx, ecx
        jne    SHORT loc_4_7066
loc_4_7090:
        lea    edi, [esi + 1d8h]
        push   4
        push   edi
        push   40010005h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_70A4:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_70AA:
        cmp    dword ptr [esp + 0ch], esi
        je     SHORT loc_4_70D9
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esp + 0ch]
        push   eax
        mov    edx, dword ptr [ecx + 10h]
        push   edx
        mov    eax, dword ptr [ecx + 8]
        call   dword ptr [eax + 4ch]
        mov    ecx, dword ptr [esp + 0ch]
        dec    dword ptr [ecx + 34h]
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [ecx + 34h], 0
        jne    SHORT loc_4_70D9
        push   ecx
        push   esi
        call   sub_4_7740
loc_4_70D9:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_70F0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_70FC:
        test   byte ptr [esi + 3eh], 1
        jne    SHORT loc_4_7125
        cmp    byte ptr [esi + 170h], 1
        mov    edi, dword ptr [esp + 10h]
        jne    SHORT loc_4_7129
        test   edi, edi
        jne    SHORT loc_4_711C
        mov    byte ptr [esi + 170h], 0
        jmp    SHORT loc_4_7129
loc_4_711C:
        mov    byte ptr [esi + 170h], 0fh
        jmp    SHORT loc_4_7129
loc_4_7125:
        mov    edi, dword ptr [esp + 10h]
loc_4_7129:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_7160
        test   edi, edi
        jne    SHORT loc_4_7160
        cmp    dword ptr [esi + 0d8h], 0
        jne    SHORT loc_4_7158
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_4_7158
        cmp    dword ptr [esi + 0e0h], 0
        jne    SHORT loc_4_7158
        cmp    dword ptr [esi + 0e4h], 0
        je     SHORT loc_4_7160
loc_4_7158:
        push   0
        push   esi
        call   sub_4_A220
loc_4_7160:
        mov    dword ptr [esi + 1d8h], edi
        pop    edi
        pop    esi
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_4_7170:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        push   edi
        push   0
        mov    ecx, esi
        test   byte ptr [esi + 3dh], 20h
        push   0
        je     SHORT loc_4_71C6
        call   dword ptr [esi + 124h]
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esi + 18h]
        and    eax, 0bfffffffh
        mov    dword ptr [esi + 3ch], eax
        or     eax, 80000000h
        mov    dword ptr [esi + 3ch], eax
        test   ecx, ecx
        je     SHORT loc_4_7203
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
loc_4_71B1:
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 3ch], edx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 58h], edi
        mov    ecx, dword ptr [ecx + 18h]
        test   ecx, ecx
        jne    SHORT loc_4_71B1
        jmp    SHORT loc_4_7203
loc_4_71C6:
        call   dword ptr [esi + 124h]
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esi + 18h]
        and    eax, 0bfffffffh
        mov    dword ptr [esi + 3ch], eax
        or     eax, 80000000h
        mov    dword ptr [esi + 3ch], eax
        test   ecx, ecx
        je     SHORT loc_4_7203
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
loc_4_71F0:
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 3ch], edx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 58h], edi
        mov    ecx, dword ptr [ecx + 18h]
        test   ecx, ecx
        jne    SHORT loc_4_71F0
loc_4_7203:
        push   0
        push   0
        push   40010004h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7212:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7218:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        mov    esi, dword ptr [esi + 8]
        push   ecx
        call   dword ptr [esi + 44h]
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 0C2h, 00Ch, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
loc_4_7240:
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebx
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   esi
        call   sub_4_70F0
        test   bl, bl
        je     SHORT loc_4_725F
        test   edi, edi
        je     SHORT loc_4_7265
loc_4_725F:
        push   esi
        call   sub_4_6FE0
loc_4_7265:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_4_7270:
        mov    eax, 103h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_7280:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   0
        mov    edx, 1
        mov    edi, dword ptr [esi + 4]
        push   esi
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
        test   eax, eax
        je     SHORT loc_4_72A9
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_72A9:
        inc    dword ptr [esi + 34h]
        lea    esi, [edi + 2eh]
        or     dword ptr [edi + 3ch], 40000000h
        cmp    byte ptr [esi], 0
        je     SHORT loc_4_72BF
        xor    bl, bl
        jmp    SHORT loc_4_72F1
loc_4_72BF:
        mov    ebx, 1
        mov    byte ptr [esi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_72F1
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_72F1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Ch, 00Ch, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_72F1:
        test   bl, bl
        je     SHORT loc_4_7346
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        test   bl, bl
        je     SHORT loc_4_7346
        mov    byte ptr [esi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_7333
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_7333
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 036h, 00Ch, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_7333:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_4_7346
        mov    byte ptr [edi + 2ch], 0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7343:
        add    esp, 4
loc_4_7346:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7360:
        push   ebx
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 8]
        push   esi
        mov    cx, word ptr [eax]
        push   edi
        xor    edx, edx
        mov    edi, dword ptr [esp + 14h]
        mov    dx, word ptr [edi]
        cmp    edx, ecx
        je     SHORT loc_4_7381
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_7381:
        mov    esi, dword ptr [eax + 4]
        mov    edi, dword ptr [edi + 4]
        cmp    byte ptr [esp + 18h], 0
        je     SHORT loc_4_73D4
        xor    eax, eax
        test   ecx, ecx
        jle    SHORT loc_4_73E7
loc_4_7394:
        mov    dl, byte ptr [eax + esi]
        cmp    dl, 61h
        jb     SHORT loc_4_73AA
        cmp    dl, 7ah
        ja     SHORT loc_4_73AA
        xor    ebx, ebx
        mov    bl, dl
        sub    ebx, 20h
        jmp    SHORT loc_4_73AE
loc_4_73AA:
        xor    ebx, ebx
        mov    bl, dl
loc_4_73AE:
        cmp    byte ptr [eax + edi], 61h
        jb     SHORT loc_4_73C4
        cmp    byte ptr [eax + edi], 7ah
        ja     SHORT loc_4_73C4
        xor    edx, edx
        mov    dl, byte ptr [eax + edi]
        sub    edx, 20h
        jmp    SHORT loc_4_73C9
loc_4_73C4:
        xor    edx, edx
        mov    dl, byte ptr [eax + edi]
loc_4_73C9:
        cmp    ebx, edx
        jne    SHORT loc_4_73E7
        inc    eax
        cmp    ecx, eax
        jg     SHORT loc_4_7394
        jmp    SHORT loc_4_73E7
loc_4_73D4:
        xor    eax, eax
        test   ecx, ecx
        jle    SHORT loc_4_73E7
loc_4_73DA:
        mov    dl, byte ptr [eax + esi]
        cmp    byte ptr [eax + edi], dl
        jne    SHORT loc_4_73E7
        inc    eax
        cmp    ecx, eax
        jg     SHORT loc_4_73DA
loc_4_73E7:
        sub    ecx, eax
        pop    edi
        pop    esi
        cmp    ecx, 1
        sbb    al, al
        pop    ebx
        neg    al
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7400:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        cmp    dword ptr [esi + 2d4h], 0
        je     SHORT loc_4_741D
        mov    ebx, 0c0000001h
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        ret    0ch
loc_4_741D:
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        push   ebx
        lea    eax, [edi + 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_742D:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_7453
        mov    dword ptr [esi + 2d4h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], edi
        pop    edi
        pop    esi
        mov    dword ptr [eax + 10h], ebx
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax + 18h], ebx
        mov    eax, ebx
        pop    ebx
        ret    0ch
loc_4_7453:
        mov    ebx, 0c000009ah
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        ret    0ch
loc_4_7460:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 2d4h], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7478:
        add    esp, 8
        ret    4
        db  08Bh, 0FFh
loc_4_7480:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 18h], ecx
        ret    4
        db  090h
sub_4_7490:
        xor    edx, edx
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, edx
        je     SHORT loc_4_74A3
loc_4_749A:
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax + ecx], 0
        jne    SHORT loc_4_749A
loc_4_74A3:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 2], dx
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax], dx
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_74C0:
        xor    dx, dx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        test   esi, esi
        je     SHORT loc_4_74DB
loc_4_74CC:
        mov    ax, dx
        inc    dx
        movzx  ecx, ax
        cmp    word ptr [esi + ecx*2], 0
        jne    SHORT loc_4_74CC
loc_4_74DB:
        mov    ecx, dword ptr [esp + 8]
        lea    eax, [edx*2]
        mov    word ptr [ecx + 2], ax
        mov    dword ptr [ecx + 4], esi
        mov    word ptr [ecx], ax
        pop    esi
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7500:
        mov    edx, dword ptr [esp + 8]
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        cmp    word ptr [edx], di
        jbe    SHORT loc_4_752C
loc_4_7511:
        mov    ecx, dword ptr [edx + 4]
        inc    edi
        movzx  ax, byte ptr [ecx + edi - 1]
        mov    ecx, dword ptr [esi + 4]
        mov    word ptr [ecx + edi*2 - 2], ax
        xor    eax, eax
        mov    ax, word ptr [edx]
        cmp    eax, edi
        jg     SHORT loc_4_7511
loc_4_752C:
        lea    eax, [edi*2]
        pop    edi
        mov    word ptr [esi + 2], ax
        mov    word ptr [esi], ax
        xor    eax, eax
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_7550:
        mov    edx, dword ptr [esp + 8]
        push   esi
        push   edi
        xor    esi, esi
        test   word ptr [edx], 0fffeh
        mov    ecx, dword ptr [esp + 0ch]
        jbe    SHORT loc_4_757E
loc_4_7563:
        mov    eax, dword ptr [edx + 4]
        mov    edi, dword ptr [ecx + 4]
        mov    al, byte ptr [eax + esi*2]
        mov    byte ptr [edi + esi], al
        inc    esi
        mov    ax, word ptr [edx]
        shr    ax, 1
        movzx  edi, ax
        cmp    edi, esi
        jg     SHORT loc_4_7563
loc_4_757E:
        mov    word ptr [ecx + 2], si
        xor    eax, eax
        mov    word ptr [ecx], si
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_4_7590:
        sub    esp, 4
        mov    byte ptr [esp + 3], 1
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [edi + 4]
        mov    eax, dword ptr [edi + 20h]
        test   al, 1
        jne    NEAR PTR loc_4_76FA
        or     eax, 1
        lea    ebp, [esi + 2eh]
        mov    dword ptr [edi + 20h], eax
        cmp    byte ptr [ebp], 0
        je     SHORT loc_4_75C1
        xor    bl, bl
        jmp    SHORT loc_4_75F4
loc_4_75C1:
        mov    ebx, 1
        mov    byte ptr [ebp], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_75F4
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_75F4
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 076h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_75F4:
        mov    ecx, dword ptr [esi + 0fch]
        cmp    ecx, 7
        ja     SHORT loc_4_760E
        xor    eax, eax
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_760E:
        xor    eax, eax
        jmp    SHORT loc_4_7666
loc_4_7612:
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    ecx, dword ptr [esi + 0dch]
        push   ecx
        call   sub_4_1060
        jmp    SHORT loc_4_7666
loc_4_7626:
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    ecx, dword ptr [esi + 0e0h]
        push   ecx
        call   sub_4_1B30
        jmp    SHORT loc_4_7666
loc_4_763A:
        test   byte ptr [edi + 20h], 2
        jne    SHORT loc_4_7654
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    ecx, dword ptr [esi + 0e4h]
        push   ecx
        call   sub_4_0220
        jmp    SHORT loc_4_7666
loc_4_7654:
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    ecx, dword ptr [esi + 0d8h]
        push   ecx
        call   sub_4_0730
loc_4_7666:
        cmp    eax, 0c001000eh
        je     SHORT loc_4_7677
        push   edi
        dec    dword ptr [edi + 34h]
        push   esi
        call   sub_4_A220
loc_4_7677:
        test   bl, bl
        je     SHORT loc_4_76C1
        mov    byte ptr [ebp], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_76AE
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_76AE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0BAh, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_76AE:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_76C1
        mov    byte ptr [esi + 2ch], 0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_76BE:
        add    esp, 4
loc_4_76C1:
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_4_76DC
        push   0fh
        mov    eax, dword ptr [esi + 128h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_76D5:
        mov    byte ptr [esp + 13h], 0
        jmp    SHORT loc_4_76FA
loc_4_76DC:
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   edi
        call   sub_4_7780
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_4_7820
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_76F7:
        add    esp, 8
loc_4_76FA:
        mov    al, byte ptr [esp + 13h]
        pop    ebp
        dec    al
        pop    edi
        pop    esi
        cmp    al, 1
        mov    eax, 0
        pop    ebx
        adc    eax, -1
        add    esp, 4
        and    eax, 103h
        ret    4
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 003h, 08Dh
        db  064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_7740:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7750:
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   esi
        call   sub_4_7780
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_7820
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_776B:
        add    esp, 8
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_7780:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 18h]
        cmp    eax, edx
        jne    SHORT loc_4_7798
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ecx + 18h], eax
        ret    8
loc_4_7798:
        cmp    dword ptr [edx + 18h], eax
        je     SHORT loc_4_77A5
loc_4_779D:
        mov    edx, dword ptr [edx + 18h]
        cmp    dword ptr [edx + 18h], eax
        jne    SHORT loc_4_779D
loc_4_77A5:
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [edx + 18h], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_77C0:
        mov    ecx, dword ptr [esp + 8]
        cmp    byte ptr [ecx + 12h], 0
        je     SHORT loc_4_77CF
        xor    al, al
        ret    8
loc_4_77CF:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], edx
        mov    dword ptr [ecx], eax
        mov    al, 1
        ret    8
        db  090h
sub_4_77E0:
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_7803
        mov    ecx, dword ptr [esp + 4]
loc_4_77ED:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_4_77FE
        lea    edx, [eax + 1ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_77ED
        ret    8
loc_4_77FE:
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [edx], eax
loc_4_7803:
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_7810:
        mov    eax, dword ptr [esp + 4]
        add    eax, 8
        push   eax
        call   sub_4_6B50
        ret    4
sub_4_7820:
        mov    eax, dword ptr [esp + 4]
        add    eax, 20h
        push   eax
        call   sub_4_6B50
        ret    4
sub_4_7830:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        lea    ecx, [esp + 0fh]
        mov    eax, dword ptr [esi + 3ch]
        or     eax, 40h
        push   ecx
        lea    edx, [esi + 90h]
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        push   edx
        or     dword ptr [esi + 1a4h], 4
        mov    dword ptr [esi + 3ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7861:
        cmp    byte ptr [esp + 0fh], 0
        jne    SHORT loc_4_7872
        push   7a120h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7872:
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_787F
        xor    al, al
        jmp    SHORT loc_4_78B0
loc_4_787F:
        mov    eax, 1
        mov    byte ptr [edi], al
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_78B0
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_78B0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 027h, 002h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_78B0:
        test   al, al
        jne    SHORT loc_4_7900
loc_4_78B4:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_78BE:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_78CB
        xor    al, al
        jmp    SHORT loc_4_78FC
loc_4_78CB:
        mov    eax, 1
        mov    byte ptr [edi], al
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_78FC
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_78FC
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 027h, 002h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_78FC:
        test   al, al
        je     SHORT loc_4_78B4
loc_4_7900:
        sti
        mov    eax, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebx + 2ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7911:
        push   esi
        call   sub_4_4010
        push   esi
        call   sub_4_7820
        mov    eax, dword ptr [esi + 0d8h]
        test   eax, eax
        je     SHORT loc_4_792D
        push   eax
        call   sub_4_0600
loc_4_792D:
        mov    eax, dword ptr [esi + 0dch]
        test   eax, eax
        je     SHORT loc_4_793D
        push   eax
        call   sub_4_0F80
loc_4_793D:
        mov    eax, dword ptr [esi + 0e0h]
        test   eax, eax
        je     SHORT loc_4_794D
        push   eax
        call   sub_4_19A0
loc_4_794D:
        mov    eax, dword ptr [esi + 0e4h]
        test   eax, eax
        je     SHORT loc_4_795D
        push   eax
        call   sub_4_00D0
loc_4_795D:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_4_7982
loc_4_7963:
        mov    eax, dword ptr [esi + 6ch]
        mov    ecx, eax
        test   eax, eax
        je     SHORT loc_4_7971
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 6ch], eax
loc_4_7971:
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7979:
        add    esp, 8
        cmp    dword ptr [esi + 6ch], 0
        jne    SHORT loc_4_7963
loc_4_7982:
        xor    ebx, ebx
        lea    edi, [esi + 44h]
loc_4_7987:
        cmp    ebx, 0ah
        jae    SHORT loc_4_79B3
        cmp    dword ptr [edi], 0
        je     SHORT loc_4_79AD
loc_4_7991:
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        test   eax, eax
        je     SHORT loc_4_799D
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
loc_4_799D:
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_79A5:
        add    esp, 8
        cmp    dword ptr [edi], 0
        jne    SHORT loc_4_7991
loc_4_79AD:
        add    edi, 4
        inc    ebx
        jmp    SHORT loc_4_7987
loc_4_79B3:
        lea    ebx, [esi + 138h]
        mov    edi, 6
loc_4_79BE:
        mov    eax, dword ptr [ebx]
        mov    ecx, eax
        test   eax, eax
        je     SHORT loc_4_79CA
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx], eax
loc_4_79CA:
        test   ecx, ecx
        je     SHORT loc_4_79D9
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_79D6:
        add    esp, 8
loc_4_79D9:
        add    ebx, 4
        dec    edi
        jne    SHORT loc_4_79BE
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_79F9
        push   0
        mov    eax, dword ptr [esi + 1c4h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_79F6:
        add    esp, 8
loc_4_79F9:
        lea    eax, [esp + 0eh]
        mov    ecx, dword ptr [esi + 128h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7A0A:
        cmp    byte ptr [esp + 0eh], 0
        je     SHORT loc_4_7A18
        push   0fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7A18:
        push   0
        mov    eax, dword ptr [esi + 128h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7A26:
        add    esp, 8
        mov    eax, dword ptr [esi + 180h]
        test   eax, eax
        je     SHORT loc_4_7A3E
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7A3B:
        add    esp, 8
loc_4_7A3E:
        mov    eax, dword ptr [esi + 8]
        push   eax
        push   esi
        call   sub_4_77E0
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_4_7810
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        call   sub_4_3AE0
        test   eax, eax
        jne    SHORT loc_4_7A6C
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7A69:
        add    esp, 8
loc_4_7A6C:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7A80:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     NEAR PTR loc_4_7B2E
        mov    eax, dword ptr [esp + 14h]
loc_4_7A98:
        cmp    esi, eax
        je     SHORT loc_4_7AA3
        mov    esi, dword ptr [esi + 6ch]
        test   esi, esi
        jne    SHORT loc_4_7A98
loc_4_7AA3:
        test   esi, esi
        je     NEAR PTR loc_4_7B2E
        mov    byte ptr [esi + 84h], 1
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     SHORT loc_4_7AE7
loc_4_7AB8:
        mov    ebx, dword ptr [edi + 1ch]
        push   0
        push   0
        mov    edx, 4
        or     dword ptr [edi + 3ch], 40h
        or     dword ptr [edi + 1a4h], 4
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
        and    dword ptr [edi + 3ch], 0fffffffeh
        push   edi
        call   sub_4_7830
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_4_7AB8
loc_4_7AE7:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_7AF9
        mov    eax, dword ptr [esi + 6ch]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_7B15
loc_4_7AF9:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 6ch], esi
        je     SHORT loc_4_7B0C
loc_4_7B04:
        mov    ecx, dword ptr [ecx + 6ch]
        cmp    dword ptr [ecx + 6ch], esi
        jne    SHORT loc_4_7B04
loc_4_7B0C:
        mov    eax, dword ptr [ecx + 6ch]
        mov    edx, dword ptr [eax + 6ch]
        mov    dword ptr [ecx + 6ch], edx
loc_4_7B15:
        mov    eax, dword ptr [esi + 68h]
        lea    ecx, [esp + 0ch]
        push   eax
        push   ecx
        call   sub_4_3920
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7B2B:
        add    esp, 8
loc_4_7B2E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7B40:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7B57:
        add    esp, 8
        xor    eax, eax
        ret
        db  08Dh, 049h, 000h
sub_4_7B60:
        sub    esp, 11ch
        mov    eax, 4
        mov    word ptr [esp], ax
        push   esi
        mov    word ptr [esp + 6], ax
        mov    eax, 9
        db  0C7h, 044h, 024h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [esp + 0ch], ax
        mov    word ptr [esp + 0eh], ax
        jne    SHORT loc_4_7BAB
        mov    eax, 21h
        pop    esi
        add    esp, 11ch
        ret
loc_4_7BAB:
        cmp    dword ptr [esp + 128h], 1
        je     SHORT loc_4_7BC2
        mov    eax, 19h
        pop    esi
        add    esp, 11ch
        ret
loc_4_7BC2:
        lea    eax, [esp + 14h]
        push   0
        lea    ecx, [esp + 24h]
        push   eax
        mov    esi, dword ptr [esp + 12ch]
        push   ecx
        mov    dword ptr [esp + 20h], 100h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7BEC:
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_4_7C7A
        lea    eax, [esp + 20h]
        lea    ecx, [esp + 18h]
        push   eax
        push   ecx
        call   sub_4_7490
        lea    ecx, [esp + 4]
        push   1
        lea    edx, [esp + 1ch]
        push   ecx
        push   edx
        call   sub_4_CE10
        test   al, al
        je     SHORT loc_4_7C29
        push   esi
        call   sub_4_83E0
        pop    esi
        add    esp, 11ch
        ret
loc_4_7C29:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_7C3F
        mov    eax, 21h
        pop    esi
        add    esp, 11ch
        ret
loc_4_7C3F:
        lea    eax, [esp + 0ch]
        push   1
        lea    ecx, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_4_CE10
        push   esi
        test   al, al
        je     SHORT loc_4_7C6D
        call   sub_4_8990
        cmp    eax, 26h
        jne    SHORT loc_4_7C7F
        push   esi
        call   sub_4_8E70
        pop    esi
        add    esp, 11ch
        ret
loc_4_7C6D:
        call   sub_4_8E70
        pop    esi
        add    esp, 11ch
        ret
loc_4_7C7A:
        mov    eax, 13h
loc_4_7C7F:
        pop    esi
        add    esp, 11ch
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_7C90:
        sub    esp, 10h
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 24h]
        push   edi
        push   ebp
        xor    esi, esi
        push   ebx
        call   sub_4_7FB0
        mov    edi, eax
        mov    ecx, dword ptr [esp + 24h]
        cmp    ecx, 0eh
        ja     SHORT loc_4_7CBD
        xor    eax, eax
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_7CBD:
        mov    esi, 1
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7CCC:
        test   edi, edi
        jne    SHORT loc_4_7CDF
        mov    esi, 13h
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7CDF:
        cmp    dword ptr [edi + 74h], 5
        jne    SHORT loc_4_7D41
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   2
        push   edi
        call   sub_4_6630
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_7D41
        add    dword ptr [esp + 10h], 18h
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7D12:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_4_7D2A
        mov    esi, 2
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7D2A:
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   0
        push   eax
        push   ebp
        push   0
        push   3
        push   edi
        call   sub_4_6630
        mov    dword ptr [edi + 70h], ebp
loc_4_7D41:
        mov    eax, dword ptr [edi + 48h]
        push   eax
        call   sub_4_9670
        mov    ebp, eax
        test   ebp, ebp
        je     NEAR PTR loc_4_7E00
        mov    eax, dword ptr [ebp + 1ch]
        lea    ecx, [edi + 4ch]
        lea    edx, [ebp + 0ch]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 10h], ecx
        push   edx
        mov    dword ptr [esp + 1ch], 0
        call   sub_4_6AC0
        lea    ecx, [edi + 10h]
        lea    edx, [esp + 10h]
        push   ecx
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        push   edx
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   dword ptr [ebp + 5ch]
        test   eax, eax
        je     SHORT loc_4_7D92
        mov    esi, 13h
loc_4_7D92:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_4_7DA6
        push   edi
        call   sub_4_6D40
loc_4_7DA6:
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_4_7DD1
        test   esi, esi
        jne    SHORT loc_4_7DBB
        and    dword ptr [edi + 40h], 0fffffffbh
        push   edi
        call   sub_4_6DD0
loc_4_7DBB:
        push   ebp
        call   sub_4_6E30
        push   ebp
        call   sub_4_6E30
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7DD1:
        test   esi, esi
        jne    SHORT loc_4_7DF0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7DE2:
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_7DF0
        mov    esi, 13h
loc_4_7DF0:
        push   ebp
        call   sub_4_6E30
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7E00:
        mov    esi, 13h
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7E0F:
        test   edi, edi
        je     NEAR PTR loc_4_7F69
        or     dword ptr [edi + 40h], 4
        lea    eax, [edi + 20h]
        push   eax
        call   sub_4_6AC0
        mov    eax, dword ptr [edi + 8]
        add    eax, 0ch
        push   eax
        call   sub_4_6AC0
        push   1
        push   edi
        call   sub_4_6E90
        mov    eax, dword ptr [edi + 28h]
        test   eax, eax
        je     SHORT loc_4_7E45
        mov    ecx, dword ptr [edi + 2ch]
        push   ecx
        call   eax
loc_4_7E45:
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [edi + 8]
        push   eax
        call   dword ptr [ebx + 60h]
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7E59:
        cmp    dword ptr [esp + 28h], 1
        jbe    SHORT loc_4_7E6F
        mov    esi, 1
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7E6F:
        test   edi, edi
        je     NEAR PTR loc_4_7F69
        mov    eax, dword ptr [edi + 3ch]
        cmp    eax, 4
        je     SHORT loc_4_7E91
        cmp    eax, 5
        je     SHORT loc_4_7E91
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_7F69
loc_4_7E91:
        mov    esi, 13h
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7EA0:
        xor    ebp, ebp
        test   edi, edi
        je     SHORT loc_4_7EB5
        test   byte ptr [edi + 40h], 4
        je     SHORT loc_4_7EAF
        mov    ebp, dword ptr [edi + 8]
loc_4_7EAF:
        push   edi
loc_4_7EB0:
        call   sub_4_6DD0
loc_4_7EB5:
        push   ebx
        call   sub_4_7FB0
        test   eax, eax
        je     SHORT loc_4_7EC2
        push   eax
        jmp    SHORT loc_4_7EB0
loc_4_7EC2:
        test   ebp, ebp
        je     NEAR PTR loc_4_7F69
        push   ebp
        call   sub_4_6E30
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7EDA:
        mov    eax, dword ptr [esp + 28h]
        and    eax, 7fffffffh
        cmp    eax, 1
        je     SHORT loc_4_7EFC
        cmp    eax, 5
        je     SHORT loc_4_7F1C
        mov    esi, 1
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7EFC:
        test   edi, edi
        je     SHORT loc_4_7F69
        mov    eax, dword ptr [edi + 3ch]
        cmp    eax, 4
        je     SHORT loc_4_7F0D
        cmp    eax, 5
        jne    SHORT loc_4_7F69
loc_4_7F0D:
        mov    esi, 13h
        pop    ebp
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7F1C:
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_7F24:
        add    esp, 8
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7F33:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_4_7F44
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_4_7F44:
        test   edi, edi
        je     SHORT loc_4_7F69
        lea    eax, [edi + 20h]
        push   eax
        call   sub_4_6AC0
        mov    eax, esi
        pop    ebp
        or     dword ptr [edi + 40h], 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_4_7F5F:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_4_7F69:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 009h, 009h
        db  009h, 005h, 009h, 009h, 009h, 006h, 007h, 008h, 005h, 000h, 000h, 000h, 000h
sub_4_7FB0:
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_7FC9
        mov    ecx, dword ptr [esp + 4]
loc_4_7FBD:
        cmp    dword ptr [eax + 38h], ecx
        je     SHORT loc_4_7FC9
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_7FBD
loc_4_7FC9:
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_7FD0:
        sub    esp, 138h
        xor    ecx, ecx
        lea    eax, [esp + 1ch]
        push   ebx
        mov    dword ptr [esp + 1ch], ecx
        push   esi
        mov    esi, dword ptr [esp + 144h]
        push   edi
        mov    edi, 13h
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        cmp    esi, ecx
        mov    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_4_83C7
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 24h]
        push   eax
        push   ecx
        mov    edx, dword ptr [esi + 38h]
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_4_9350
        test   eax, eax
        jne    NEAR PTR loc_4_83C7
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_8278
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_8147
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_9670
        test   eax, eax
        jne    SHORT loc_4_8053
        mov    edi, 21h
        jmp    NEAR PTR loc_4_83AD
loc_4_8053:
        lea    eax, [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8067:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_8147
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 44h]
        lea    edx, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        mov    dword ptr [esp + 14h], 100h
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_809A:
        add    esp, 18h
        test   eax, eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        jne    NEAR PTR loc_4_813F
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_80AF:
        lea    eax, [esp + 48h]
        lea    ecx, [esp + 38h]
        add    esp, 4
        push   eax
        push   ecx
        call   sub_4_7490
        lea    ecx, [esp + 34h]
        push   ecx
        call   sub_4_3032
        add    esp, 4
        test   al, al
        je     SHORT loc_4_8147
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_80E5
        mov    edi, 13h
        jmp    NEAR PTR loc_4_83AD
loc_4_80E5:
        lea    eax, [esp + 44h]
        lea    ecx, [esi + 10h]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_8101
        mov    edi, 13h
        jmp    NEAR PTR loc_4_83AD
loc_4_8101:
        lea    ebx, [esi + 48h]
        mov    dword ptr [esi + 3ch], 4
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_4_811C
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8119:
        add    esp, 8
loc_4_811C:
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 44h]
        push   eax
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_8138
        mov    edi, 13h
        jmp    NEAR PTR loc_4_8395
loc_4_8138:
        xor    edi, edi
        jmp    NEAR PTR loc_4_83AD
loc_4_813F:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8144:
        add    esp, 4
loc_4_8147:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_8278
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_9670
        test   eax, eax
        jne    SHORT loc_4_816C
        mov    edi, 21h
        jmp    NEAR PTR loc_4_83C7
loc_4_816C:
        lea    eax, [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8180:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_8278
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 44h]
        lea    edx, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   ecx
        mov    dword ptr [esp + 14h], 100h
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_81B3:
        add    esp, 18h
        test   eax, eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        jne    NEAR PTR loc_4_8270
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_81C8:
        lea    ebx, [esp + 48h]
        add    esp, 4
        mov    edi, ebx
        cmp    byte ptr [esp + 44h], 0
        je     NEAR PTR loc_4_8278
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_81EC
loc_4_81E1:
        cmp    byte ptr [edi], 2ch
        je     SHORT loc_4_81F1
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_4_81E1
loc_4_81EC:
        cmp    byte ptr [edi], 2ch
        jne    SHORT loc_4_81F5
loc_4_81F1:
        mov    byte ptr [edi], 0
        inc    edi
loc_4_81F5:
        lea    eax, [esp + 3ch]
        push   ebx
        push   eax
        call   sub_4_7490
        lea    eax, [esp + 3ch]
        push   eax
        call   sub_4_3038
        add    esp, 4
        test   al, al
        jne    SHORT loc_4_821A
        mov    ebx, edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_4_81E1
        jmp    SHORT loc_4_8278
loc_4_821A:
        push   ebx
        lea    eax, [esi + 10h]
        push   eax
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_8232
        mov    edi, 13h
        jmp    NEAR PTR loc_4_83AD
loc_4_8232:
        lea    ebx, [esi + 48h]
        mov    dword ptr [esi + 3ch], 5
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_4_824D
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_824A:
        add    esp, 8
loc_4_824D:
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 44h]
        push   eax
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_8269
        mov    edi, 13h
        jmp    NEAR PTR loc_4_8395
loc_4_8269:
        xor    edi, edi
        jmp    NEAR PTR loc_4_83AD
loc_4_8270:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8275:
        add    esp, 4
loc_4_8278:
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 44h]
        lea    edx, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   ecx
        mov    dword ptr [esp + 14h], 100h
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_82A0:
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_4_82B1
        mov    edi, 13h
        jmp    NEAR PTR loc_4_83AD
loc_4_82B1:
        mov    eax, dword ptr [esi + 50h]
        lea    ecx, [esi + 10h]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_82CC
        mov    edi, 13h
        jmp    NEAR PTR loc_4_83AD
loc_4_82CC:
        lea    ebx, [esi + 48h]
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_4_82E0
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_82DD:
        add    esp, 8
loc_4_82E0:
        lea    eax, [esp + 44h]
        lea    ecx, [esi + 44h]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_82FC
        mov    edi, 13h
        jmp    NEAR PTR loc_4_8395
loc_4_82FC:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_4_9670
        test   eax, eax
        jne    SHORT loc_4_8319
        lea    eax, [esp + 24h]
        push   esi
        push   0
        mov    ecx, dword ptr [esi + 38h]
        push   eax
        push   ecx
        call   sub_4_8EA0
loc_4_8319:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_4_9670
        test   eax, eax
        je     SHORT loc_4_8361
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_4_834A
        lea    edx, [esp + 24h]
        add    eax, 24h
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [eax + 8], ecx
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [eax + 0ch], edx
        jmp    SHORT loc_4_8354
loc_4_834A:
        lea    eax, [esp + 24h]
        push   eax
        call   sub_4_9310
loc_4_8354:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_837F
        xor    edi, edi
        jmp    SHORT loc_4_83AD
loc_4_8361:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_8378
        mov    dword ptr [esi + 3ch], 0
        mov    edi, 13h
        jmp    SHORT loc_4_8384
loc_4_8378:
        mov    dword ptr [esi + 3ch], 1
loc_4_837F:
        mov    edi, 21h
loc_4_8384:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_4_8395
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8392:
        add    esp, 8
loc_4_8395:
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_4_83A7
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_83A4:
        add    esp, 8
loc_4_83A7:
        mov    dword ptr [ebx], 0
loc_4_83AD:
        mov    eax, dword ptr [esp + 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_83B7:
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_83C4:
        add    esp, 4
loc_4_83C7:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 138h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_83E0:
        sub    esp, 110h
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 11ch]
        push   edi
        push   ebp
        push   ebx
        call   sub_4_7FB0
        test   eax, eax
        je     SHORT loc_4_8405
        mov    esi, 10h
        jmp    NEAR PTR loc_4_86E7
loc_4_8405:
        lea    eax, [esp + 20h]
        push   1
        mov    dword ptr [esp + 14h], 100h
        push   100h
        push   eax
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8421:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_8432
        mov    esi, 13h
        jmp    NEAR PTR loc_4_86E7
loc_4_8432:
        lea    eax, [esp + 20h]
        xor    edx, edx
        mov    esi, dword ptr [esp + 20h]
        cmp    dword ptr [esp + 10h], edx
        jbe    SHORT loc_4_8457
loc_4_8442:
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     SHORT loc_4_8457
        inc    eax
        cmp    cl, 5ch
        jne    SHORT loc_4_8450
        mov    esi, eax
loc_4_8450:
        inc    edx
        cmp    edx, dword ptr [esp + 10h]
        jb     SHORT loc_4_8442
loc_4_8457:
        push   0
        push   78h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8460:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_4_8473
        mov    esi, 2
        jmp    NEAR PTR loc_4_86E7
loc_4_8473:
        mov    edi, ebp
        xor    eax, eax
        mov    ecx, 1eh
        rep stosd
        lea    edi, [ebp + 20h]
        push   edi
        call   sub_4_6B00
        push   esi
        lea    eax, [ebp + 4ch]
        push   eax
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_849F
        mov    esi, 2
        jmp    NEAR PTR loc_4_86DC
loc_4_849F:
        lea    eax, [esp + 10h]
        push   0
        lea    ecx, [esp + 24h]
        push   eax
        push   ecx
        mov    dword ptr [ebp + 38h], ebx
        mov    dword ptr [ebp + 64h], 0
        db  0C7h, 045h, 068h, 000h, 000h, 000h, 000h
        push   1
        mov    dword ptr [ebp + 6ch], ebp
        mov    dword ptr [esp + 20h], 100h
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_84D6:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_84F9
        lea    eax, [esp + 20h]
        lea    ecx, [ebp + 58h]
        push   eax
        push   ecx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_84F9
        mov    esi, 2
        jmp    NEAR PTR loc_4_86CA
loc_4_84F9:
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 24h]
        push   eax
        mov    dword ptr [esp + 18h], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_851C:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_8531
        cmp    byte ptr [esp + 20h], 31h
        jne    SHORT loc_4_8531
        or     dword ptr [ebp + 40h], 200h
loc_4_8531:
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 24h]
        push   eax
        mov    dword ptr [esp + 18h], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8554:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_8568
        cmp    byte ptr [esp + 20h], 31h
        jne    SHORT loc_4_8568
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_4_8568:
        push   ebp
        call   sub_4_7FD0
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_4_86A6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_857D:
        test   eax, 40000h
        jne    SHORT loc_4_85F2
        test   eax, 80000h
        je     SHORT loc_4_8592
        mov    esi, 2
        jmp    SHORT loc_4_85F7
loc_4_8592:
        mov    esi, 1
        push   0
        lea    eax, [esp + 20h]
        lea    ecx, [esp + 18h]
        lea    edx, [esp + 1ch]
        push   eax
        mov    dword ptr [esp + 1ch], 4
        push   ecx
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_85B6:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_85F7
        mov    ecx, dword ptr [esp + 18h]
        dec    ecx
        cmp    ecx, 1fh
        ja     SHORT loc_4_85F7
        xor    eax, eax
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_85D6:
        mov    esi, 1
        jmp    SHORT loc_4_85F7
loc_4_85DD:
        mov    esi, 2
        jmp    SHORT loc_4_85F7
loc_4_85E4:
        mov    esi, 5
        jmp    SHORT loc_4_85F7
loc_4_85EB:
        mov    esi, 8
        jmp    SHORT loc_4_85F7
loc_4_85F2:
        mov    esi, 3
loc_4_85F7:
        lea    eax, [esp + 1ch]
        push   0
        lea    ecx, [esp + 18h]
        push   eax
        lea    edx, [esp + 20h]
        push   ecx
        mov    dword ptr [esp + 20h], 4
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8616:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_8629
        cmp    dword ptr [esp + 18h], 4
        jne    SHORT loc_4_8629
        mov    esi, 5
loc_4_8629:
        mov    dword ptr [ebp + 74h], esi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_4_9580
        test   eax, eax
        je     SHORT loc_4_8640
        mov    esi, 13h
        jmp    SHORT loc_4_86A6
loc_4_8640:
        push   ebp
        call   sub_4_6CA0
        push   edi
        call   sub_4_6AC0
        or     dword ptr [ebp + 40h], 20h
        mov    eax, dword ptr [ebp + 3ch]
        cmp    eax, 4
        je     SHORT loc_4_8662
        cmp    eax, 5
        mov    eax, 3
        jne    SHORT loc_4_8664
loc_4_8662:
        xor    eax, eax
loc_4_8664:
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8672:
        add    esp, 10h
        mov    esi, eax
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_868B
loc_4_8680:
        cmp    eax, ebp
        je     SHORT loc_4_868B
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_8680
loc_4_868B:
        mov    ebp, eax
        test   eax, eax
        je     SHORT loc_4_8697
        test   byte ptr [eax + 40h], 2
        jne    SHORT loc_4_86E7
loc_4_8697:
        mov    esi, 13h
        test   eax, eax
        je     SHORT loc_4_86E7
        push   eax
        call   sub_4_6CC0
loc_4_86A6:
        mov    eax, dword ptr [ebp + 70h]
        test   eax, eax
        je     SHORT loc_4_86B8
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_86B5:
        add    esp, 8
loc_4_86B8:
        mov    eax, dword ptr [ebp + 5ch]
        test   eax, eax
        je     SHORT loc_4_86CA
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_86C7:
        add    esp, 8
loc_4_86CA:
        mov    eax, dword ptr [ebp + 50h]
        test   eax, eax
        je     SHORT loc_4_86DC
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_86D9:
        add    esp, 8
loc_4_86DC:
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_86E4:
        add    esp, 8
loc_4_86E7:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 110h
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  006h, 002h, 006h, 006h, 006h, 003h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 004h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 005h, 005h, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_8740:
        sub    esp, 31ch
        mov    eax, dword ptr [esp + 328h]
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    esi, esi
        xor    ebp, ebp
        push   eax
        call   sub_4_7FB0
        cmp    eax, ebp
        mov    ebx, eax
        jne    SHORT loc_4_87B2
        mov    eax, dword ptr [esp + 338h]
        push   esi
        lea    ecx, [esp + 24h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8774:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_87B2
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_4_7FB0
        test   eax, eax
        je     SHORT loc_4_87B2
        mov    ebp, dword ptr [eax + 18h]
        test   ebp, ebp
        je     SHORT loc_4_87B2
loc_4_8790:
        mov    eax, dword ptr [esp + 338h]
        cmp    dword ptr [ebp + 20h], eax
        je     SHORT loc_4_87A3
        mov    ebp, dword ptr [ebp + 4]
        test   ebp, ebp
        jne    SHORT loc_4_8790
loc_4_87A3:
        test   ebp, ebp
        je     SHORT loc_4_87B2
        test   byte ptr [ebp + 38h], 80h
        je     SHORT loc_4_87B2
        mov    esi, 1
loc_4_87B2:
        cmp    dword ptr [esp + 330h], 5
        je     SHORT loc_4_87CC
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 31ch
        ret
loc_4_87CC:
        test   esi, esi
        je     SHORT loc_4_87DD
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 31ch
        ret
loc_4_87DD:
        lea    eax, [esp + 18h]
        push   0
        mov    ecx, dword ptr [esp + 33ch]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_4_9350
        test   eax, eax
        je     SHORT loc_4_8809
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 31ch
        ret
loc_4_8809:
        lea    eax, [esp + 10h]
        lea    ecx, [esp + 2ch]
        lea    edx, [esp + 28h]
        push   eax
        lea    eax, [esp + 18h]
        push   ecx
        lea    ecx, [esp + 134h]
        push   edx
        mov    dword ptr [esp + 20h], 100h
        mov    edx, dword ptr [esp + 24h]
        push   0
        mov    dword ptr [esp + 20h], 100h
        mov    dword ptr [esp + 2ch], 1
        push   eax
        push   ecx
        push   0
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_884B:
        add    esp, 20h
        test   eax, eax
        jne    NEAR PTR loc_4_8957
loc_4_8856:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   0
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [esp + 230h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 130h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [esp + 230h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        lea    edx, [esp + 28h]
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 33ch]
        lea    ecx, [esp + 230h]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_88D2:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_890B
        test   ebx, ebx
        je     SHORT loc_4_88E8
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_4_6AC0
        jmp    SHORT loc_4_890B
loc_4_88E8:
        test   ebp, ebp
        je     SHORT loc_4_890B
        push   0
        push   8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_88F5:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_890B
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [ebp + 5ch]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebp + 5ch], eax
loc_4_890B:
        mov    ecx, dword ptr [esp + 1ch]
        lea    eax, [esp + 10h]
        lea    edx, [esp + 2ch]
        push   eax
        lea    esi, [esp + 2ch]
        push   edx
        lea    eax, [esp + 1ch]
        push   esi
        lea    edx, [esp + 138h]
        push   0
        inc    dword ptr [esp + 2ch]
        push   eax
        mov    dword ptr [esp + 28h], 100h
        mov    dword ptr [esp + 24h], 100h
        push   edx
        push   ecx
        mov    ecx, dword ptr [esp + 34h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_894C:
        add    esp, 20h
        test   eax, eax
        je     NEAR PTR loc_4_8856
loc_4_8957:
        test   ebx, ebx
        je     SHORT loc_4_896E
        mov    eax, dword ptr [ebx + 40h]
        test   al, 20h
        je     SHORT loc_4_896E
        and    eax, 0ffffffdfh
        push   ebx
        mov    dword ptr [ebx + 40h], eax
        call   sub_4_6DD0
loc_4_896E:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8978:
        add    esp, 4
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 31ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_8990:
        sub    esp, 118h
        lea    eax, [esp + 0ch]
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    dword ptr [esp + 10h], ebx
        push   edi
        mov    edi, dword ptr [esp + 128h]
        push   ebp
        mov    ebp, 13h
        push   ebx
        push   edi
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        lea    eax, [esp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_89C4:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_8AAE
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_4_7FB0
        cmp    eax, ebx
        mov    esi, eax
        jne    SHORT loc_4_89E9
        mov    ebp, 26h
        jmp    NEAR PTR loc_4_8AAE
loc_4_89E9:
        lea    eax, [esp + 14h]
        push   1
        lea    ecx, [esp + 2ch]
        push   eax
        mov    dword ptr [esp + 1ch], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8A0F:
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_4_8AA3
        lea    eax, [esp + 28h]
        push   eax
        call   sub_4_9510
        test   eax, eax
        jne    SHORT loc_4_8A37
        lea    eax, [esp + 18h]
        push   0
        push   1
        push   eax
        push   edi
        call   sub_4_8EA0
loc_4_8A37:
        lea    eax, [esp + 28h]
        push   eax
        call   sub_4_9620
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_8A58
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        sbb    ebp, ebp
        and    ebp, 0eh
        add    ebp, 13h
        jmp    SHORT loc_4_8AA3
loc_4_8A58:
        cmp    dword ptr [ebx + 28h], 0
        jne    SHORT loc_4_8A7D
        lea    edx, [esp + 18h]
        lea    ecx, [ebx + 1ch]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ebp, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], ebp
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        jmp    SHORT loc_4_8A87
loc_4_8A7D:
        lea    eax, [esp + 18h]
        push   eax
        call   sub_4_9310
loc_4_8A87:
        push   3
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8A95:
        add    esp, 10h
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_8AA3
        mov    ebp, 13h
loc_4_8AA3:
        cmp    ebp, 21h
        je     SHORT loc_4_8AAE
        push   esi
        call   sub_4_6DD0
loc_4_8AAE:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 118h
        ret    4
        db  08Dh, 049h, 000h
loc_4_8AC0:
        sub    esp, 8
        mov    dword ptr [esp + 4], 0
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 20h]
        push   edi
        mov    edi, dword ptr [esp + 20h]
        push   ebp
        lea    eax, [esp + 14h]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8AE4:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_8AF8
        mov    dword ptr [esp + 10h], 13h
        jmp    NEAR PTR loc_4_8C6E
loc_4_8AF8:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_4_7FB0
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_8B15
        mov    dword ptr [esp + 10h], 13h
        jmp    NEAR PTR loc_4_8C6E
loc_4_8B15:
        mov    ebp, dword ptr [ebx + 18h]
        test   ebp, ebp
        je     SHORT loc_4_8B2F
loc_4_8B1C:
        cmp    dword ptr [ebp + 20h], edi
        je     SHORT loc_4_8B28
        mov    ebp, dword ptr [ebp + 4]
        test   ebp, ebp
        jne    SHORT loc_4_8B1C
loc_4_8B28:
        test   ebp, ebp
        je     SHORT loc_4_8B2F
        mov    esi, dword ptr [ebp + 1ch]
loc_4_8B2F:
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [esp + 1ch]
        dec    eax
        cmp    eax, 0dh
        ja     SHORT loc_4_8B48
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_8B48:
        mov    dword ptr [esp + 10h], 1
        jmp    NEAR PTR loc_4_8C6E
loc_4_8B55:
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_4_6AC0
        push   esi
        push   ebx
        push   edi
        call   sub_4_8CC0
        mov    dword ptr [esp + 10h], eax
        push   esi
        call   sub_4_9EF0
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_4_8B85
        push   esi
        call   sub_4_9EF0
        push   ebx
        call   sub_4_6DD0
        jmp    SHORT loc_4_8BA0
loc_4_8B85:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_4_8BAB
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8B99:
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_4_8BA0:
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_4_8C6E
loc_4_8BAB:
        mov    dword ptr [esp + 10h], 13h
        jmp    NEAR PTR loc_4_8C6E
loc_4_8BB8:
        test   ebp, ebp
        je     NEAR PTR loc_4_8C6E
        add    ebx, 20h
        add    esi, 0ch
        push   ebx
        call   sub_4_6AC0
        push   esi
        call   sub_4_6AC0
        push   ebp
        call   sub_4_93F0
        jmp    NEAR PTR loc_4_8C6E
loc_4_8BDD:
        cmp    dword ptr [esp + 20h], 1
        jbe    NEAR PTR loc_4_8C6E
        mov    dword ptr [esp + 10h], 1
        jmp    SHORT loc_4_8C6E
loc_4_8BF2:
        test   ebp, ebp
        je     SHORT loc_4_8C6E
        cmp    byte ptr [ebp + 2ch], 0
        je     SHORT loc_4_8C02
        push   ebp
        call   sub_4_6A60
loc_4_8C02:
        push   esi
        call   sub_4_9EF0
        push   ebx
        call   sub_4_6DD0
        jmp    SHORT loc_4_8C6E
loc_4_8C10:
        cmp    dword ptr [esp + 20h], 1
        jbe    SHORT loc_4_8C6E
        mov    dword ptr [esp + 10h], 1
        jmp    SHORT loc_4_8C6E
loc_4_8C21:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_8C6E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_4_8C6E
loc_4_8C34:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_4_8C6E
loc_4_8C40:
        test   ebp, ebp
        je     SHORT loc_4_8C6E
        add    esi, 0ch
        add    ebx, 20h
        or     dword ptr [ebp + 38h], 100h
        push   esi
        call   sub_4_6AC0
        push   ebx
        call   sub_4_6AC0
        push   ebp
        call   sub_4_93F0
        test   eax, eax
        je     SHORT loc_4_8C6E
        and    dword ptr [ebp + 38h], 0fffffeffh
loc_4_8C6E:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh
        db  0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_8CC0:
        sub    esp, 10ch
        cmp    dword ptr [esp + 118h], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    SHORT loc_4_8CDE
        mov    edi, 13h
        jmp    NEAR PTR loc_4_8E4D
loc_4_8CDE:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_4_8CEC
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8CEC:
        mov    ebx, dword ptr [esp + 120h]
        mov    ecx, dword ptr [esp + 128h]
        push   0
        push   0ch
        mov    dword ptr [ecx + 2ch], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8D06:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_8D19
        mov    edi, 2
        jmp    NEAR PTR loc_4_8E4D
loc_4_8D19:
        lea    eax, [esp + 1ch]
        push   1
        push   100h
        push   eax
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8D2D:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_8D3E
        mov    edi, 13h
        jmp    NEAR PTR loc_4_8DE2
loc_4_8D3E:
        lea    eax, [esp + 1ch]
        lea    ecx, [esp + 14h]
        push   eax
        lea    edi, [esi + 8]
        push   ecx
        call   sub_4_7490
        lea    eax, [esp + 14h]
        push   ebx
        lea    ecx, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 12ch]
        mov    ebp, dword ptr [esp + 130h]
        add    eax, 10h
        mov    dword ptr [edi], 0
        push   eax
        push   esi
        push   ecx
        call   dword ptr [ebp + 64h]
        cmp    dword ptr [esp + 10h], 103h
        jne    SHORT loc_4_8D90
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8D87:
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 10h], eax
loc_4_8D90:
        mov    ecx, dword ptr [esp + 128h]
        mov    ebp, dword ptr [ecx + 8]
        test   ebp, ebp
        je     SHORT loc_4_8DAA
loc_4_8D9E:
        cmp    dword ptr [ebp + 20h], ebx
        je     SHORT loc_4_8DAA
        mov    ebp, dword ptr [ebp + 8]
        test   ebp, ebp
        jne    SHORT loc_4_8D9E
loc_4_8DAA:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_8DC7
        test   ebp, ebp
        je     SHORT loc_4_8DC0
        lea    eax, [esp + 10h]
        push   ebp
        push   eax
        call   sub_4_9CD0
loc_4_8DC0:
        mov    edi, 13h
        jmp    SHORT loc_4_8DE2
loc_4_8DC7:
        test   ebp, ebp
        jne    SHORT loc_4_8E48
        push   0
        push   60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8DD4:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_4_8DEF
        mov    edi, 2
loc_4_8DE2:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8DEA:
        add    esp, 8
        jmp    SHORT loc_4_8E4D
loc_4_8DEF:
        mov    edi, ebp
        xor    eax, eax
        mov    ecx, 18h
        rep stosd
        mov    eax, dword ptr [esp + 124h]
        add    eax, 20h
        push   eax
        call   sub_4_6AC0
        mov    eax, dword ptr [esp + 128h]
        add    eax, 0ch
        push   eax
        call   sub_4_6AC0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 124h]
        mov    edx, dword ptr [esp + 128h]
        mov    byte ptr [ebp + 2ch], al
        mov    dword ptr [ebp + 14h], ecx
        mov    dword ptr [ebp + 1ch], edx
        mov    dword ptr [ebp + 24h], eax
        mov    edi, dword ptr [ecx + 8]
        mov    dword ptr [ebp + 0ch], edi
        mov    edx, dword ptr [ecx + 38h]
        mov    dword ptr [ebp + 18h], edx
        mov    dword ptr [ebp + 20h], ebx
        mov    dword ptr [ebp + 38h], eax
loc_4_8E48:
        xor    edi, edi
        mov    dword ptr [ebp + 28h], esi
loc_4_8E4D:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10ch
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_4_8E60:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_8E70:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0ch]
        push   2
        push   0
        push   esi
        call   sub_4_8EA0
        test   eax, eax
        jne    SHORT loc_4_8E95
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8E92:
        add    esp, 0ch
loc_4_8E95:
        xor    eax, eax
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_8EA0:
        sub    esp, 138h
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        push   ebx
        mov    dword ptr [esp + 4], eax
        push   esi
        cmp    dword ptr [esp + 14ch], 2
        push   edi
        push   ebp
        jne    SHORT loc_4_8ED8
        mov    esi, dword ptr [esp + 14ch]
        push   3
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8ED3:
        add    esp, 10h
        jmp    SHORT loc_4_8EDF
loc_4_8ED8:
        mov    esi, dword ptr [esp + 14ch]
loc_4_8EDF:
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 18h]
        push   eax
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_4_9350
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_4_8F07
        mov    esi, 13h
        jmp    NEAR PTR loc_4_9298
loc_4_8F07:
        lea    eax, [esp + 1ch]
        lea    ecx, [esp + 48h]
        lea    edx, [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        mov    dword ptr [esp + 24h], 100h
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8F2F:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_4_9296
        mov    byte ptr [esp + 30h], 2ah
        cmp    dword ptr [esp + 10h], 0
        mov    eax, 13h
        je     SHORT loc_4_8F72
        lea    ecx, [esp + 31h]
        lea    edx, [esp + 2ch]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 18h]
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8F6F:
        add    esp, 18h
loc_4_8F72:
        test   eax, eax
        je     SHORT loc_4_8FA4
        mov    eax, 1
loc_4_8F7B:
        mov    cl, byte ptr [esp + eax + 47h]
        test   cl, cl
        je     SHORT loc_4_8F9F
        cmp    cl, 2eh
        je     SHORT loc_4_8F9F
        cmp    cl, 61h
        jb     SHORT loc_4_8F95
        cmp    cl, 7ah
        ja     SHORT loc_4_8F95
        sub    cl, 20h
loc_4_8F95:
        mov    byte ptr [esp + eax + 30h], cl
        inc    eax
        cmp    eax, 13h
        jb     SHORT loc_4_8F7B
loc_4_8F9F:
        mov    byte ptr [esp + eax + 30h], 0
loc_4_8FA4:
        lea    eax, [esp + 31h]
        push   eax
        call   sub_4_9510
        cmp    dword ptr [esp + 154h], 2
        jne    SHORT loc_4_8FF8
        test   eax, eax
        lea    eax, [esp + 48h]
        je     SHORT loc_4_8FC4
        lea    eax, [esp + 30h]
loc_4_8FC4:
        push   0
        push   2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_8FCF:
        add    esp, 10h
        mov    esi, eax
        lea    eax, [esp + 31h]
        push   eax
        call   sub_4_9510
        test   eax, eax
        jne    NEAR PTR loc_4_9296
        test   esi, esi
        je     NEAR PTR loc_4_9298
        mov    esi, 13h
        jmp    NEAR PTR loc_4_9298
loc_4_8FF8:
        test   eax, eax
        jne    NEAR PTR loc_4_9296
        lea    esi, [esp + 48h]
        xor    eax, eax
        mov    ebx, esi
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 18h], eax
        cmp    dword ptr [esp + 158h], eax
        je     SHORT loc_4_9027
        mov    ecx, dword ptr [esp + 158h]
        mov    dword ptr [ecx + 3ch], 2
loc_4_9027:
        mov    edi, dword ptr [esp + 150h]
loc_4_902E:
        mov    al, byte ptr [esi]
        cmp    al, 2ch
        je     SHORT loc_4_90A5
        test   al, al
        je     SHORT loc_4_90A1
        cmp    al, 2eh
        jne    SHORT loc_4_909B
        mov    al, byte ptr [esi + 1]
        cmp    al, 61h
        jb     SHORT loc_4_9050
        cmp    al, 7ah
        ja     SHORT loc_4_9050
        xor    ecx, ecx
        mov    cl, al
        sub    ecx, 20h
        jmp    SHORT loc_4_9054
loc_4_9050:
        xor    ecx, ecx
        mov    cl, al
loc_4_9054:
        cmp    ecx, 53h
        jne    SHORT loc_4_909B
        mov    al, byte ptr [esi + 2]
        cmp    al, 61h
        jb     SHORT loc_4_906D
        cmp    al, 7ah
        ja     SHORT loc_4_906D
        xor    ecx, ecx
        mov    cl, al
        sub    ecx, 20h
        jmp    SHORT loc_4_9071
loc_4_906D:
        xor    ecx, ecx
        mov    cl, al
loc_4_9071:
        cmp    ecx, 59h
        jne    SHORT loc_4_909B
        mov    al, byte ptr [esi + 3]
        cmp    al, 61h
        jb     SHORT loc_4_908A
        cmp    al, 7ah
        ja     SHORT loc_4_908A
        xor    ecx, ecx
        mov    cl, al
        sub    ecx, 20h
        jmp    SHORT loc_4_908E
loc_4_908A:
        xor    ecx, ecx
        mov    cl, al
loc_4_908E:
        cmp    ecx, 53h
        jne    SHORT loc_4_909B
        mov    dword ptr [esp + 18h], 1
loc_4_909B:
        inc    esi
        jmp    NEAR PTR loc_4_928B
loc_4_90A1:
        cmp    al, 2ch
        jne    SHORT loc_4_90AB
loc_4_90A5:
        mov    byte ptr [esi], 0
        inc    esi
        jmp    SHORT loc_4_90B3
loc_4_90AB:
        mov    dword ptr [esp + 28h], 1
loc_4_90B3:
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_4_912A
        lea    eax, [esp + 20h]
        push   1
        lea    ecx, [esp + 48h]
        push   ebx
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_90CC:
        test   eax, eax
        jne    SHORT loc_4_911F
        test   edi, edi
        je     SHORT loc_4_911F
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_911F
        push   0
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_90E4:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_911F
        mov    eax, dword ptr [esp + 20h]
        lea    edx, [ebp + 4]
        mov    ecx, dword ptr [eax + 0bh]
        push   ecx
        push   edx
        call   sub_4_CD10
        test   eax, eax
        jne    SHORT loc_4_9114
        mov    dword ptr [ebp], 0
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [edi + 0ch], ebp
        jmp    SHORT loc_4_911F
loc_4_9114:
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_911C:
        add    esp, 8
loc_4_911F:
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_4_9289
loc_4_912A:
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [esp + 158h], 0
        jne    SHORT loc_4_9152
        cmp    dword ptr [esp + 154h], 1
        jne    NEAR PTR loc_4_9289
loc_4_9152:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_91F3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0dch
        push   0dch
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_918E:
        add    esp, 24h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0dh
        push   0dh
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_91BA:
        add    esp, 24h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   17h
        push   17h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_91E6:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        add    esp, 24h
loc_4_91F3:
        lea    eax, [esp + 24h]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9205:
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_9289
        mov    eax, dword ptr [esp + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9216:
        add    esp, 4
        cmp    dword ptr [esp + 154h], 0
        jne    SHORT loc_4_9239
        mov    ecx, dword ptr [esp + 158h]
        push   0
        mov    dword ptr [ecx + 3ch], 6
        mov    edx, dword ptr [ecx + 48h]
        push   edx
        jmp    SHORT loc_4_9256
loc_4_9239:
        mov    ecx, ebx
        cmp    byte ptr [ebx], 0
        je     SHORT loc_4_924B
loc_4_9240:
        cmp    byte ptr [ecx], 2eh
        je     SHORT loc_4_9250
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_4_9240
loc_4_924B:
        cmp    byte ptr [ecx], 2eh
        jne    SHORT loc_4_9253
loc_4_9250:
        mov    byte ptr [ecx], 0
loc_4_9253:
        push   0
        push   ebx
loc_4_9256:
        call   eax
        test   edi, edi
        je     SHORT loc_4_9289
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_9289
        push   0
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_926C:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_9289
        mov    dword ptr [eax], 1
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax + 4], ecx
        mov    edx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [edi + 0ch], eax
loc_4_9289:
        mov    ebx, esi
loc_4_928B:
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_4_902E
loc_4_9296:
        xor    esi, esi
loc_4_9298:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_92AC
        mov    eax, dword ptr [esp + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_92A9:
        add    esp, 4
loc_4_92AC:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_92C0
        mov    eax, dword ptr [esp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_92BD:
        add    esp, 4
loc_4_92C0:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 138h
        ret    10h
        db  090h
sub_4_92D0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        test   esi, esi
        je     SHORT loc_4_9309
loc_4_92DC:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_92F1
        mov    eax, dword ptr [esi + 8]
        push   eax
        push   0ffffh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_92EF:
        jmp    SHORT loc_4_9302
loc_4_92F1:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_92FF:
        add    esp, 8
loc_4_9302:
        mov    esi, dword ptr [esi + 0ch]
        test   esi, esi
        jne    SHORT loc_4_92DC
loc_4_9309:
        pop    esi
        ret    4
        db  08Dh, 049h, 000h
sub_4_9310:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0ch]
        test   esi, esi
        je     SHORT loc_4_9348
loc_4_931D:
        mov    edi, dword ptr [esi + 0ch]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_9337
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_4_9337
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9334:
        add    esp, 8
loc_4_9337:
        push   0
        push   esi
        mov    esi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9341:
        add    esp, 8
        test   edi, edi
        jne    SHORT loc_4_931D
loc_4_9348:
        pop    edi
        pop    esi
        ret    4
        db  08Dh, 049h, 000h
sub_4_9350:
        mov    eax, dword ptr [esp + 14h]
        sub    esp, 104h
        lea    ecx, [esp + 4]
        push   esi
        mov    edx, dword ptr [esp + 10ch]
        push   eax
        push   100h
        push   ecx
        push   0
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9375:
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_93DE
        lea    eax, [esp + 4]
        lea    ecx, [esp + 8]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9392:
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_93DE
        mov    eax, dword ptr [esp + 110h]
        test   eax, eax
        je     SHORT loc_4_93BE
        mov    ecx, dword ptr [esp + 118h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_93B9:
        add    esp, 0ch
        mov    esi, eax
loc_4_93BE:
        mov    eax, dword ptr [esp + 114h]
        test   eax, eax
        je     SHORT loc_4_93D1
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_4_93DE
loc_4_93D1:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_93DB:
        add    esp, 4
loc_4_93DE:
        mov    eax, esi
        pop    esi
        add    esp, 104h
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_93F0:
        sub    esp, 14h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 20h]
        push   edi
        mov    byte ptr [esp + 0fh], 0
        push   ebp
        mov    edi, dword ptr [esi + 1ch]
        cmp    dword ptr [esi + 28h], 0
        mov    ebx, dword ptr [esi + 24h]
        jne    SHORT loc_4_9418
        mov    byte ptr [esp + 13h], 1
        lea    eax, [esp + 18h]
        mov    dword ptr [esi + 28h], eax
loc_4_9418:
        mov    ebp, dword ptr [esi + 28h]
        lea    eax, [edi + 0ch]
        push   eax
        mov    dword ptr [ebp + 8], 0
        call   sub_4_6AC0
        push   ebp
        mov    eax, dword ptr [esi + 24h]
        lea    ecx, [esp + 18h]
        push   eax
        push   ecx
        call   dword ptr [edi + 68h]
        cmp    dword ptr [esp + 14h], 103h
        jne    SHORT loc_4_9455
        lea    eax, [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_944B:
        add    esp, 4
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 14h], eax
loc_4_9455:
        push   edi
        call   sub_4_9EF0
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_4_947E
        test   ebx, ebx
        jne    SHORT loc_4_946C
        push   esi
        call   sub_4_6A60
loc_4_946C:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_4_947E
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_947B:
        add    esp, 8
loc_4_947E:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_9490:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 38h], 80h
        ret    4
        db  08Bh, 0FFh
loc_4_94A0:
        mov    eax, dword ptr [esp + 4]
        push   0
        mov    ecx, dword ptr [eax + 20h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_94AF:
        add    esp, 8
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_94C0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 5ch]
        test   esi, esi
        je     SHORT loc_4_94F7
loc_4_94CD:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_94DC:
        add    esp, 0ch
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 5ch], eax
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_94ED:
        add    esp, 8
        mov    esi, dword ptr [edi + 5ch]
        test   esi, esi
        jne    SHORT loc_4_94CD
loc_4_94F7:
        pop    edi
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_9500:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_950C:
        add    esp, 8
        ret
sub_4_9510:
        sub    esp, 8
        mov    eax, 20202020h
        xor    ecx, ecx
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 0ch]
loc_4_9526:
        mov    dl, byte ptr [eax + ecx]
        test   dl, dl
        je     SHORT loc_4_9544
        cmp    dl, 61h
        jb     SHORT loc_4_953A
        cmp    dl, 7ah
        ja     SHORT loc_4_953A
        sub    dl, 20h
loc_4_953A:
        mov    byte ptr [esp + ecx], dl
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_4_9526
loc_4_9544:
        lea    eax, [esp]
        push   eax
        push   0
        call   sub_4_3018
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_956E
        mov    ax, word ptr [eax + 0ah]
        and    ax, 8000h
        cmp    ax, 1
        sbb    eax, eax
        add    esp, 8
        add    eax, 2
        ret    4
loc_4_956E:
        xor    eax, eax
        add    esp, 8
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9580:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        lea    ecx, [esp]
        push   esi
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_4_7490
        test   esi, esi
        je     SHORT loc_4_95B8
loc_4_959D:
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    ecx, [esi + 10h]
        push   ecx
        call   sub_4_CE10
        test   al, al
        jne    SHORT loc_4_95B8
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_4_959D
loc_4_95B8:
        mov    eax, esi
        pop    esi
        add    esp, 8
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_95D0:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        lea    ecx, [esp]
        push   esi
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_4_7490
        test   esi, esi
        je     SHORT loc_4_9608
loc_4_95ED:
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    ecx, [esi + 4ch]
        push   ecx
        call   sub_4_CE10
        test   al, al
        jne    SHORT loc_4_9608
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_4_95ED
loc_4_9608:
        mov    eax, esi
        pop    esi
        add    esp, 8
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9620:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        lea    ecx, [esp]
        push   esi
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_4_7490
        test   esi, esi
        je     SHORT loc_4_9658
loc_4_963D:
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    ecx, [esi + 70h]
        push   ecx
        call   sub_4_CE10
        test   al, al
        jne    SHORT loc_4_9658
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_4_963D
loc_4_9658:
        mov    eax, esi
        pop    esi
        add    esp, 8
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9670:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        lea    ecx, [esp]
        push   esi
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_4_7490
        test   esi, esi
        je     SHORT loc_4_96A8
loc_4_968D:
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    ecx, [esi + 64h]
        push   ecx
        call   sub_4_CE10
        test   al, al
        jne    SHORT loc_4_96A8
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_4_968D
loc_4_96A8:
        mov    eax, esi
        pop    esi
        add    esp, 8
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_96C0:
        sub    esp, 204h
        mov    eax, dword ptr [esp + 20ch]
        push   ebx
        mov    cx, word ptr [eax]
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        xor    esi, esi
        test   cx, 0fffeh
        jbe    SHORT loc_4_96F8
        shr    cx, 1
        mov    edx, dword ptr [eax + 4]
        movzx  eax, cx
loc_4_96E9:
        mov    cl, byte ptr [esi + edx]
        inc    esi
        cmp    esi, eax
        mov    byte ptr [esp + esi + 113h], cl
        jl     SHORT loc_4_96E9
loc_4_96F8:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    byte ptr [esp + esi + 114h], 0
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [esp + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 218h]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 68h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [edx + 68h]
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebp, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [esp + 14h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    word ptr [edi - 1], ax
        lea    edi, [esp + 114h]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebp, ecx
        mov    esi, edi
        lea    edi, [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [edx + 68h]
        push   eax
        call   sub_4_9620
        test   eax, eax
        je     SHORT loc_4_97BB
        mov    ebx, dword ptr [eax + 2ch]
loc_4_97BB:
        mov    eax, 0c0000001h
        test   ebx, ebx
        je     SHORT loc_4_97E9
        lea    eax, [esp + 14h]
        push   0
        lea    ecx, [esp + 14h]
        push   ebx
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_97D6:
        add    esp, 10h
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
loc_4_97E9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 204h
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9800:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        push   edi
        test   ebx, ebx
        jne    SHORT loc_4_981B
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0000001h
        ret    10h
loc_4_981B:
        movzx  ax, byte ptr [ebx]
        shl    ax, 8
        movzx  cx, byte ptr [ebx + 1]
        add    ax, cx
        cmp    ax, 30ah
        jae    SHORT loc_4_9841
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0010004h
        ret    10h
loc_4_9841:
        push   0
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_984D:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_9866
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c000009ah
        ret    10h
loc_4_9866:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 20h
        rep stosd
        mov    eax, dword ptr [esp + 1ch]
        lea    ecx, [esi + 36h]
        mov    dword ptr [esi + 14h], 80h
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9887:
        mov    ecx, dword ptr [ebx + 3eh]
        lea    edx, [esi + 70h]
        push   ecx
        push   edx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_98B3
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_98A0:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        pop    edi
        mov    dword ptr [eax], 0c000009ah
        pop    esi
        pop    ebx
        ret    10h
loc_4_98B3:
        mov    dword ptr [esi + 8], 0
        lea    edi, [esi + 0ch]
        push   edi
        call   sub_4_6B00
        mov    eax, dword ptr [esp + 14h]
        push   esi
        mov    dword ptr [eax], esi
        call   sub_4_9F50
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_98DE
        push   edi
        call   sub_4_6AC0
loc_4_98DE:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_98EB:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        pop    ebx
        ret    10h
        db  08Bh, 0FFh
loc_4_9900:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        mov    dword ptr [eax], 0
        jne    SHORT loc_4_991B
        mov    dword ptr [eax], 0c0000001h
        ret    8
loc_4_991B:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_4_992A
        mov    dword ptr [eax], 10003h
        ret    8
loc_4_992A:
        push   ecx
        call   sub_4_9F70
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9940:
        sub    esp, 8
        cmp    dword ptr [esp + 24h], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     NEAR PTR loc_4_9CBB
        cmp    dword ptr [esp + 38h], 0
        je     NEAR PTR loc_4_9CBB
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        call   sub_4_9580
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_9984
        mov    ecx, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0c0010006h
        add    esp, 8
        ret    2ch
loc_4_9984:
        push   0
        push   60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_998D:
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_4_99AA
        mov    ecx, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0c000009ah
        add    esp, 8
        ret    2ch
loc_4_99AA:
        mov    edi, ebp
        xor    eax, eax
        mov    ecx, 18h
        rep stosd
        lea    eax, [ebx + 20h]
        mov    dword ptr [ebp], 4e504f4eh
        push   eax
        call   sub_4_6AC0
        mov    eax, dword ptr [esp + 34h]
        add    eax, 0ch
        push   eax
        call   sub_4_6AC0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 38h]
        mov    byte ptr [ebp + 2ch], al
        mov    dword ptr [ebp + 14h], ebx
        mov    dword ptr [ebp + 1ch], ecx
        mov    dword ptr [ebp + 24h], edx
        mov    esi, dword ptr [ebx + 38h]
        mov    dword ptr [ebp + 18h], esi
        mov    edx, dword ptr [ecx + 2ch]
        mov    dword ptr [ebp + 20h], edx
        mov    dword ptr [ebp + 38h], eax
        mov    esi, dword ptr [ebx + 8]
        mov    ecx, dword ptr [esi + 44h]
        mov    dword ptr [ebp + 3ch], ecx
        mov    ecx, dword ptr [esp + 34h]
        mov    esi, dword ptr [ebx + 8]
        mov    edx, dword ptr [esi + 48h]
        mov    dword ptr [ebp + 40h], edx
        mov    esi, dword ptr [ecx + 44h]
        mov    dword ptr [ebp + 44h], esi
        mov    edx, dword ptr [ecx + 48h]
        mov    dword ptr [ebp + 48h], edx
        mov    esi, dword ptr [ecx + 54h]
        mov    dword ptr [ebp + 4ch], esi
        mov    edx, dword ptr [ecx + 58h]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 50h], edx
        mov    dword ptr [ebp + 54h], eax
        db  0C7h, 045h, 058h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebp + 0ch], esi
        mov    dword ptr [ecx], ebp
        mov    ecx, dword ptr [ebx + 8]
        cmp    byte ptr [ecx + 34h], al
        je     NEAR PTR loc_4_9C26
        mov    esi, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [esp + 30h]
        mov    byte ptr [esp + 13h], al
        cmp    eax, edx
        je     SHORT loc_4_9A70
        mov    edi, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esi + 0fch]
loc_4_9A60:
        cmp    dword ptr [edi], ecx
        je     SHORT loc_4_9A6C
        add    edi, 4
        inc    eax
        cmp    eax, edx
        jb     SHORT loc_4_9A60
loc_4_9A6C:
        cmp    eax, edx
        jne    SHORT loc_4_9AB1
loc_4_9A70:
        cmp    dword ptr [esi + 0fch], 7
        jne    NEAR PTR loc_4_9B7A
        xor    eax, eax
        test   edx, edx
        je     SHORT loc_4_9A94
        mov    ecx, dword ptr [esp + 2ch]
loc_4_9A87:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_9A94
        add    ecx, 4
        inc    eax
        cmp    eax, edx
        jb     SHORT loc_4_9A87
loc_4_9A94:
        cmp    eax, edx
        jne    SHORT loc_4_9AAC
        mov    ecx, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0c0010019h
        add    esp, 8
        ret    2ch
loc_4_9AAC:
        mov    byte ptr [esp + 13h], 1
loc_4_9AB1:
        mov    edx, dword ptr [esp + 28h]
        lea    ecx, [esp + 14h]
        mov    dword ptr [edx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   2ch
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9ACB:
        mov    ecx, dword ptr [esp + 1ch]
        test   eax, eax
        mov    dword ptr [ecx], eax
        jne    NEAR PTR loc_4_9C96
        mov    edi, dword ptr [esp + 14h]
        xor    eax, eax
        mov    ecx, 0bh
        rep stosd
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [eax + 14h], edx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [esp + 3ch]
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx + 1ch], eax
        mov    edx, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ecx + 20h], edx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 24h], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [esp + 14h]
        cmp    byte ptr [esp + 13h], 0
        mov    dword ptr [ecx + 0ch], ebp
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 10h], 0
        je     SHORT loc_4_9B50
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 28h], 1
loc_4_9B50:
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   eax
        mov    edx, 8
        mov    ecx, esi
        lea    edi, [esi + 2eh]
        call   dword ptr [esi + 120h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 103h
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_9B89
        xor    bl, bl
        jmp    SHORT loc_4_9BBB
loc_4_9B7A:
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 0c0010019h
        jmp    NEAR PTR loc_4_9C96
loc_4_9B89:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_9BBB
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_9BBB
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 093h, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_9BBB:
        test   bl, bl
        je     NEAR PTR loc_4_9CC5
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     NEAR PTR loc_4_9CC5
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_9C05
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_9C05
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0A1h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_9C05:
        cmp    byte ptr [esi + 2ch], 1
        jne    NEAR PTR loc_4_9CC5
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9C19:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    2ch
loc_4_9C26:
        mov    edx, dword ptr [esp + 44h]
        mov    eax, dword ptr [esp + 40h]
        push   edx
        mov    esi, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   esi
        mov    esi, dword ptr [esp + 34h]
        push   ebp
        push   edx
        push   eax
        push   esi
        lea    edx, [ebp + 10h]
        mov    eax, dword ptr [esp + 3ch]
        push   edx
        push   eax
        call   dword ptr [ecx + 3ch]
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_9C74
        push   ebp
        call   sub_4_6BA0
        test   al, al
        je     SHORT loc_4_9C74
        mov    ecx, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        add    esp, 8
        ret    2ch
loc_4_9C74:
        mov    ecx, dword ptr [esp + 1ch]
        cmp    esi, 103h
        jne    SHORT loc_4_9C90
        pop    ebp
        mov    dword ptr [ecx], 103h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    2ch
loc_4_9C90:
        mov    dword ptr [ecx], 0c0010007h
loc_4_9C96:
        push   ebx
        call   sub_4_6DD0
        mov    ecx, dword ptr [esp + 34h]
        push   ecx
        call   sub_4_9EF0
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9CAE:
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    2ch
loc_4_9CBB:
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 0c0000001h
loc_4_9CC5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    2ch
        db  090h
sub_4_9CD0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [esp + 20h]
        test   esi, esi
        jne    SHORT loc_4_9CF3
        mov    eax, dword ptr [esp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0000001h
        add    esp, 0ch
        ret    8
loc_4_9CF3:
        cmp    byte ptr [esi + 2ch], 0
        je     SHORT loc_4_9D0C
        mov    eax, dword ptr [esp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        add    esp, 0ch
        ret    8
loc_4_9D0C:
        mov    byte ptr [esi + 2ch], 1
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_4_9D1F
        lea    eax, [esp + 0ch]
        mov    bl, 1
        mov    dword ptr [esi + 28h], eax
loc_4_9D1F:
        mov    edi, dword ptr [esi + 28h]
        mov    dword ptr [edi + 8], 0
        add    edi, 8
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        call   dword ptr [eax + 40h]
        cmp    eax, 103h
        jne    SHORT loc_4_9D46
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9D43:
        add    esp, 4
loc_4_9D46:
        test   bl, bl
        je     SHORT loc_4_9D51
        mov    dword ptr [esi + 28h], 0
loc_4_9D51:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0
        test   byte ptr [esi + 39h], 1
        jne    SHORT loc_4_9D67
        push   esi
        call   sub_4_6A60
loc_4_9D67:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    8
sub_4_9D70:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [ecx + 8], eax
        mov    al, 1
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9D90:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 8]
        cmp    eax, edx
        jne    SHORT loc_4_9DA8
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
        ret    8
loc_4_9DA8:
        cmp    dword ptr [edx + 8], eax
        je     SHORT loc_4_9DB5
loc_4_9DAD:
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 8], eax
        jne    SHORT loc_4_9DAD
loc_4_9DB5:
        mov    eax, dword ptr [edx + 8]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edx + 8], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9DD0:
        sub    esp, 220h
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 234h]
        push   edi
        mov    eax, dword ptr [ebx + 4]
        mov    esi, eax
        mov    edx, eax
        cmp    byte ptr [eax], 0
        je     SHORT loc_4_9DFB
loc_4_9DEC:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 5ch
        jne    SHORT loc_4_9DF6
        mov    esi, edx
loc_4_9DF6:
        cmp    byte ptr [edx], 0
        jne    SHORT loc_4_9DEC
loc_4_9DFB:
        cmp    esi, eax
        jbe    SHORT loc_4_9E10
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 0ch], ebx
        jmp    NEAR PTR loc_4_9EB7
loc_4_9E10:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [esp + 12ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 12ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    eax, eax
        mov    word ptr [edi - 1], cx
        mov    edi, dword ptr [ebx + 4]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [esp + 12ch]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 12ch]
        lea    ecx, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_4_7490
        lea    ecx, [esp + 1ch]
        mov    edx, dword ptr [ebx + 4]
        lea    eax, [esp + 24h]
        push   edx
        mov    dword ptr [esp + 10h], ecx
        push   eax
        call   sub_4_7490
        lea    ecx, [esp + 24h]
        mov    dword ptr [esp + 18h], ecx
loc_4_9EB7:
        lea    eax, [esp + 0ch]
        mov    ecx, dword ptr [esp + 234h]
        mov    edx, dword ptr [esp + 230h]
        push   eax
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 18h], 1
        push   ecx
        push   edx
        call   sub_4_5B90
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 220h
        ret    0ch
        db  08Dh, 049h, 000h
sub_4_9EF0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_4_6AE0
        test   al, al
        je     SHORT loc_4_9F48
        call   dword ptr [esi + 6ch]
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_9F48
        lea    edi, [esi + 1ch]
        push   edi
        call   sub_4_92D0
        mov    eax, dword ptr [esi + 74h]
        push   eax
        call   sub_4_9620
        test   eax, eax
        jne    SHORT loc_4_9F48
        push   edi
        call   sub_4_9310
        mov    eax, dword ptr [esi + 74h]
        test   eax, eax
        je     SHORT loc_4_9F3D
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9F3A:
        add    esp, 8
loc_4_9F3D:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_9F45:
        add    esp, 8
loc_4_9F48:
        pop    edi
        pop    esi
        ret    4
        db  08Dh, 049h, 000h
sub_4_9F50:
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 004h, 000h, 000h, 000h
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_9F70:
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_9F98
        mov    ecx, dword ptr [esp + 4]
loc_4_9F82:
        cmp    eax, ecx
        je     SHORT loc_4_9F92
        mov    edx, eax
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_9F82
        ret    4
loc_4_9F92:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
loc_4_9F98:
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_9FA0:
        sub    esp, 4ch
        xor    eax, eax
        mov    ecx, 12h
        push   esi
        push   edi
        lea    edi, [esp + 8]
        rep stosd
        stosw
        mov    esi, dword ptr [esp + 60h]
        mov    al, byte ptr [esi]
        mov    cl, byte ptr [esi + 1]
        mov    edx, dword ptr [esi + 8]
        mov    byte ptr [esp + 8], al
        mov    eax, dword ptr [esi + 0ch]
        mov    byte ptr [esp + 9], cl
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [esp + 0eh], edx
        mov    edx, dword ptr [esi + 14h]
        mov    dword ptr [esp + 12h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esp + 16h], ecx
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 1ah], edx
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [esp + 1eh], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esp + 22h], ecx
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [esp + 26h], edx
        mov    edx, dword ptr [esi + 2ch]
        mov    dword ptr [esp + 2ah], eax
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [esp + 2eh], ecx
        mov    ecx, dword ptr [esi + 40h]
        push   0
        mov    dword ptr [esp + 36h], edx
        mov    dword ptr [esp + 3ah], eax
        mov    edx, dword ptr [esi + 48h]
        xor    eax, eax
        mov    ax, word ptr [esi + 32h]
        mov    dword ptr [esp + 3eh], ecx
        add    eax, 2
        mov    dword ptr [esp + 42h], edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A031:
        mov    dword ptr [esp + 4eh], eax
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_A04E
        mov    esi, dword ptr [esp + 58h]
        pop    edi
        mov    dword ptr [esi], 0c000009ah
        pop    esi
        add    esp, 4ch
        ret    10h
loc_4_A04E:
        lea    eax, [esi + 30h]
        lea    ecx, [esp + 42h]
        push   eax
        push   ecx
        call   sub_4_7550
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 46h]
        mov    cx, word ptr [esp + 42h]
        push   4ah
        mov    byte ptr [ecx + eax], 0
        mov    ecx, dword ptr [esi + 44h]
        mov    edx, dword ptr [esi + 38h]
        lea    eax, [esp + 0ch]
        mov    edi, dword ptr [esp + 60h]
        push   eax
        mov    esi, dword ptr [esp + 60h]
        push   edi
        mov    dword ptr [esp + 56h], edx
        push   esi
        mov    dword ptr [esp + 5eh], ecx
        call   sub_4_9800
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_A09C
        mov    eax, dword ptr [edi]
        mov    word ptr [eax + 34h], 0ffffh
loc_4_A09C:
        mov    eax, dword ptr [esp + 46h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A0A8:
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4ch
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_A0C0:
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esp + 28h]
        mov    edx, dword ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   ecx
        mov    ecx, dword ptr [esp + 24h]
        push   edx
        mov    edx, dword ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   ecx
        mov    ecx, dword ptr [esp + 24h]
        push   edx
        mov    edx, dword ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   ecx
        mov    ecx, dword ptr [esp + 24h]
        push   edx
        push   eax
        push   ecx
        call   sub_4_9940
        ret    2ch
        db  090h
loc_4_A100:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   eax
        push   ecx
        call   sub_4_9CD0
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A120:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        mov    dword ptr [ecx], 0
        je     SHORT loc_4_A138
        mov    dword ptr [ecx + 4], eax
        inc    dword ptr [eax + 34h]
loc_4_A138:
        mov    eax, dword ptr [esp + 4]
        add    eax, 1dch
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_A14D
loc_4_A146:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_A146
loc_4_A14D:
        mov    dword ptr [eax], ecx
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A160:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 1ch]
        push   edi
        push   ebp
        push   0
        lea    ebp, [ebx + 28h]
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A173:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_A192
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0c000009ah
        ret    14h
loc_4_A192:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    ebp, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], ebp
        test   ebx, ebx
        je     SHORT loc_4_A1CB
        cmp    edi, eax
        je     SHORT loc_4_A1CB
        test   ebp, ebp
        je     SHORT loc_4_A1D9
        cmp    ebp, 2
        je     SHORT loc_4_A1CB
        push   ebx
        lea    eax, [esi + 28h]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A1CB:
        test   ebp, ebp
        je     SHORT loc_4_A1D9
        cmp    ebp, 1
        je     SHORT loc_4_A1EA
        cmp    ebp, 2
        jne    SHORT loc_4_A202
loc_4_A1D9:
        mov    eax, dword ptr [esp + 1ch]
        test   ebx, ebx
        mov    dword ptr [esi + 14h], eax
        je     SHORT loc_4_A202
        test   edi, edi
        je     SHORT loc_4_A202
        jmp    SHORT loc_4_A1F9
loc_4_A1EA:
        mov    eax, dword ptr [esp + 1ch]
        test   ebx, ebx
        mov    dword ptr [esi + 14h], eax
        je     SHORT loc_4_A202
        test   edi, edi
        je     SHORT loc_4_A202
loc_4_A1F9:
        lea    eax, [esi + 28h]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 1ch], ebx
loc_4_A202:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A220:
        sub    esp, 18h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 24h]
        push   edi
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 7
        ja     SHORT loc_4_A2A2
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_A244:
        mov    eax, dword ptr [esi + 0d8h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_4_A2A2
loc_4_A253:
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_4_A2A2
loc_4_A262:
        mov    eax, dword ptr [esi + 0e0h]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_4_A2A2
loc_4_A271:
        mov    eax, dword ptr [esi + 0e4h]
        mov    edx, dword ptr [esi + 0d8h]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [edx + 0ch]
        or     dword ptr [esp + 10h], eax
        test   byte ptr [esi + 3ch], 4
        jne    SHORT loc_4_A298
        test   byte ptr [esp + 10h], 2
        je     SHORT loc_4_A2A2
loc_4_A298:
        and    dword ptr [esp + 10h], 0fffffffdh
        or     dword ptr [esp + 10h], 8
loc_4_A2A2:
        lea    eax, [esp + 10h]
        push   4
        lea    ecx, [esp + 10h]
        push   eax
        push   1010eh
        push   1
        push   ecx
        call   sub_4_A160
        mov    edi, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 14h], eax
        push   edi
        push   ecx
        push   esi
        call   sub_4_A120
        mov    eax, dword ptr [esi + 0fch]
        test   eax, eax
        je     SHORT loc_4_A2EB
        cmp    eax, 1
        je     SHORT loc_4_A322
        cmp    eax, 2
        je     NEAR PTR loc_4_A3BD
        jmp    NEAR PTR loc_4_A44D
loc_4_A2EB:
        lea    ebx, [esi + 20ch]
        lea    eax, [esp + 18h]
        push   ebx
        mov    ecx, dword ptr [esi + 0d8h]
        lea    edx, [esp + 18h]
        push   eax
        push   0c0h
        push   ecx
        push   edx
        call   sub_4_0DC0
        mov    ecx, dword ptr [esp + 18h]
        lea    eax, [ecx + ecx*2]
        add    eax, eax
        push   eax
        push   ebx
        push   1010103h
        jmp    NEAR PTR loc_4_A431
loc_4_A322:
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 4]
        mov    dx, cx
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [esp + 1ch], ecx
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        mov    cx, ax
        push   4
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        movzx  eax, cx
        add    edx, eax
        lea    ecx, [esp + 20h]
        lea    eax, [esp + 10h]
        push   ecx
        mov    dword ptr [esp + 24h], edx
        push   2010103h
        push   1
        push   eax
        call   sub_4_A160
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        mov    dword ptr [esp + 18h], eax
        push   ecx
        push   esi
        call   sub_4_A120
        mov    ecx, dword ptr [esi + 0dch]
        mov    edx, dword ptr [ecx + 8]
        mov    cx, dx
        mov    eax, edx
        shr    eax, 10h
        mov    ch, cl
        mov    cl, dh
        mov    dword ptr [esp + 20h], edx
        movzx  edx, cx
        mov    cx, ax
        push   4
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        movzx  eax, cx
        add    edx, eax
        lea    eax, [esp + 10h]
        mov    dword ptr [esp + 24h], edx
        lea    edx, [esp + 24h]
        push   edx
        push   2010104h
        push   1
        push   eax
        jmp    SHORT loc_4_A438
loc_4_A3BD:
        lea    ebx, [esi + 20ch]
        lea    eax, [esp + 18h]
        push   ebx
        mov    ecx, dword ptr [esi + 0e0h]
        lea    edx, [esp + 18h]
        push   eax
        push   0c0h
        push   ecx
        push   edx
        call   sub_4_2540
        mov    ecx, dword ptr [esp + 18h]
        lea    eax, [ecx + ecx*2]
        lea    ecx, [esp + 0ch]
        add    eax, eax
        push   eax
        push   ebx
        push   3010103h
        push   1
        push   ecx
        call   sub_4_A160
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        mov    dword ptr [esp + 18h], eax
        push   ecx
        push   esi
        call   sub_4_A120
        lea    ecx, [esp + 18h]
        push   ebx
        push   ecx
        mov    edx, dword ptr [esi + 0e0h]
        lea    eax, [esp + 1ch]
        push   40h
        push   edx
        push   eax
        call   sub_4_25A0
        mov    eax, dword ptr [esp + 18h]
        add    eax, eax
        push   eax
        push   ebx
        push   3010107h
loc_4_A431:
        lea    ecx, [esp + 18h]
        push   1
        push   ecx
loc_4_A438:
        call   sub_4_A160
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        mov    dword ptr [esp + 18h], eax
        push   ecx
        push   esi
        call   sub_4_A120
loc_4_A44D:
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 003h, 005h, 000h, 000h
        db  000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_A490:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 4]
        test   byte ptr [esi + 3fh], 80h
        je     SHORT loc_4_A4AB
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_A4AB:
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_A4B7
        xor    bl, bl
        jmp    SHORT loc_4_A4E9
loc_4_A4B7:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A4E9
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A4E9
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0F8h, 001h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_A4E9:
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_4_A120
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
        test   bl, bl
        je     SHORT loc_4_A55B
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_4_A55B
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A548
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A548
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 010h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_A548:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_A55B
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A558:
        add    esp, 4
loc_4_A55B:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A570:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0d8h]
        push   eax
        call   sub_4_0600
        mov    dword ptr [esi + 0d8h], 0
        mov    eax, dword ptr [esi + 0dch]
        push   eax
        call   sub_4_0F80
        mov    dword ptr [esi + 0dch], 0
        mov    eax, dword ptr [esi + 0e0h]
        push   eax
        call   sub_4_19A0
        mov    dword ptr [esi + 0e0h], 0
        mov    eax, dword ptr [esi + 0e4h]
        push   eax
        call   sub_4_00D0
        mov    dword ptr [esi + 0e4h], 0
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A5E0:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        cli
        mov    esi, dword ptr [esp + 2ch]
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_4_A5F8
        xor    bl, bl
        jmp    SHORT loc_4_A62B
loc_4_A5F8:
        mov    ebx, 1
        mov    byte ptr [esi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A62B
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A62B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 04Bh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_A62B:
        test   bl, bl
        jne    SHORT loc_4_A681
loc_4_A62F:
        mov    byte ptr [esi + 2ch], 1
        lea    eax, [esi + 2eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A63C:
        add    esp, 4
        cli
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_4_A64A
        xor    bl, bl
        jmp    SHORT loc_4_A67D
loc_4_A64A:
        mov    ebx, 1
        mov    byte ptr [esi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A67D
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A67D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 04Bh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_A67D:
        test   bl, bl
        je     SHORT loc_4_A62F
loc_4_A681:
        sti
        cmp    byte ptr [esp + 30h], 0
        mov    dword ptr [esi + 1e0h], esi
        je     SHORT loc_4_A6B4
        lea    eax, [esp + 14h]
        lea    ecx, [esp + 10h]
        mov    edx, dword ptr [esp + 3ch]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   ecx
        mov    ecx, dword ptr [esp + 3ch]
        push   edx
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   edx
        call   dword ptr [eax + 3ch]
        jmp    SHORT loc_4_A6D7
loc_4_A6B4:
        lea    edx, [esp + 14h]
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esp + 3ch]
        push   edx
        mov    edx, dword ptr [esp + 3ch]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        mov    ebp, dword ptr [esi + 8]
        push   ecx
        call   dword ptr [ebp + 4ch]
loc_4_A6D7:
        cmp    eax, 103h
        mov    ebp, eax
        je     SHORT loc_4_A6E4
        test   ebp, ebp
        jne    SHORT loc_4_A6FC
loc_4_A6E4:
        push   0
        lea    eax, [esi + 70h]
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
        mov    ecx, esi
        call   dword ptr [esi + 118h]
loc_4_A6FC:
        test   bl, bl
        je     SHORT loc_4_A749
        mov    byte ptr [esi + 2eh], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A733
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A733
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 07Ah, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_A733:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_A749
        mov    byte ptr [esi + 2ch], 0
        lea    eax, [esi + 2eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A746:
        add    esp, 4
loc_4_A749:
        cmp    ebp, 103h
        jne    NEAR PTR loc_4_A88B
        lea    eax, [esp + 20h]
        lea    ecx, [esi + 1fch]
        mov    dword ptr [esp + 20h], 0ff676980h
        mov    dword ptr [esp + 24h], 0ffffffffh
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A776:
        mov    ebp, eax
        mov    eax, dword ptr [esi + 1e4h]
        test   ebp, ebp
        jne    SHORT loc_4_A78A
        test   eax, eax
        je     NEAR PTR loc_4_A87F
loc_4_A78A:
        cli
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_4_A795
        xor    bl, bl
        jmp    SHORT loc_4_A7C8
loc_4_A795:
        mov    ebx, 1
        mov    byte ptr [esi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A7C8
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A7C8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 090h, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_A7C8:
        test   bl, bl
        jne    SHORT loc_4_A81E
loc_4_A7CC:
        mov    byte ptr [esi + 2ch], 1
        lea    eax, [esi + 2eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A7D9:
        add    esp, 4
        cli
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_4_A7E7
        xor    bl, bl
        jmp    SHORT loc_4_A81A
loc_4_A7E7:
        mov    ebx, 1
        mov    byte ptr [esi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A81A
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A81A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 090h, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_A81A:
        test   bl, bl
        je     SHORT loc_4_A7CC
loc_4_A81E:
        sti
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [esi + 8]
        push   eax
        call   dword ptr [edi + 2ch]
        test   bl, bl
        je     SHORT loc_4_A876
        mov    byte ptr [esi + 2eh], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_A860
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_A860
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 096h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_A860:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_A876
        mov    byte ptr [esi + 2ch], 0
        lea    eax, [esi + 2eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A873:
        add    esp, 4
loc_4_A876:
        mov    edi, dword ptr [esp + 40h]
        test   ebp, ebp
        jne    SHORT loc_4_A87F
        inc    edi
loc_4_A87F:
        add    esi, 1fch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A88B:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
