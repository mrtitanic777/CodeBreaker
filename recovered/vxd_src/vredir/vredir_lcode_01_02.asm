; vredir_lcode_01_02.asm -- VREDIR object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_41B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_5B7C
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_41CB
        mov    eax, 35h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_41CB:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        test   ebx, ebx
        je     SHORT loc_1_420F
loc_1_41D8:
        test   edi, edi
        je     SHORT loc_1_420F
        cmp    byte ptr [ebx + 2], 0
        jne    SHORT loc_1_4208
        mov    eax, 1
        mov    cl, byte ptr [ebx + 5ah]
        shl    eax, cl
        test   edi, eax
        je     SHORT loc_1_4208
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_41F7:
        add    esp, 8
        mov    eax, 1
        mov    cl, byte ptr [ebx + 5ah]
        shl    eax, cl
        not    eax
        and    edi, eax
loc_1_4208:
        mov    ebx, dword ptr [ebx + 18h]
        test   ebx, ebx
        jne    SHORT loc_1_41D8
loc_1_420F:
        xor    ebx, ebx
loc_1_4211:
        cli
        cmp    dword ptr [esi + 76h], ebx
        jne    SHORT loc_1_4221
        mov    eax, 1
        cmp    byte ptr [esi + 7bh], bl
        jne    SHORT loc_1_4223
loc_1_4221:
        xor    eax, eax
loc_1_4223:
        test   eax, eax
        je     SHORT loc_1_4232
        push   esi
        call   sub_1_5BFC
        add    esp, 4
        jmp    SHORT loc_1_4211
loc_1_4232:
        cmp    dword ptr [esi + 76h], 0
        jne    SHORT loc_1_4243
        sti
        mov    eax, 35h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_4243:
        mov    byte ptr [esi + 7ch], 0
        sti
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_1_4250:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        lea    edi, [ebp - 0dh]
        mov    dword ptr [ebp - 0ch], esi
loc_1_4265:
        cmp    esi, 10h
        jge    NEAR PTR loc_1_430C
        xor    eax, eax
        xor    edx, edx
        mov    byte ptr [ebp - 5], al
        mov    byte ptr [ebp - 1], al
loc_1_4278:
        cmp    edx, 4
        jge    SHORT loc_1_42C9
        mov    bl, byte ptr [ecx]
        cmp    bl, 30h
        jl     SHORT loc_1_429A
        cmp    bl, 39h
        jg     SHORT loc_1_429A
        mov    byte ptr [ebp - 8], 0ah
        imul   byte ptr [ebp - 8]
        mov    byte ptr [ebp - 5], 1
        add    al, bl
        sub    al, 30h
        jmp    SHORT loc_1_42C0
loc_1_429A:
        cmp    bl, 2eh
        je     SHORT loc_1_42A8
        cmp    bl, 20h
        je     SHORT loc_1_42A8
        test   bl, bl
        jne    SHORT loc_1_42E4
loc_1_42A8:
        mov    byte ptr [edi], al
        dec    edi
        inc    dword ptr [ebp - 0ch]
        cmp    bl, 2eh
        jne    SHORT loc_1_42B7
        mov    byte ptr [ebp - 1], 1
loc_1_42B7:
        cmp    bl, 20h
        je     SHORT loc_1_42C9
        test   bl, bl
        je     SHORT loc_1_42C9
loc_1_42C0:
        inc    ecx
        inc    esi
        inc    edx
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_4278
loc_1_42C9:
        cmp    byte ptr [ebp - 5], 0
        je     SHORT loc_1_42ED
        cmp    dword ptr [ebp - 0ch], 4
        je     SHORT loc_1_42F6
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4265
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_42E4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_42ED:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_42F6:
        cmp    esi, 10h
        jge    SHORT loc_1_430C
loc_1_42FB:
        mov    al, byte ptr [ecx]
        cmp    al, 20h
        je     SHORT loc_1_4305
        test   al, al
        jne    SHORT loc_1_4316
loc_1_4305:
        inc    esi
        inc    ecx
        cmp    esi, 10h
        jl     SHORT loc_1_42FB
loc_1_430C:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4316:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_1_4320:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2ch], 0
        je     NEAR PTR loc_1_44AD
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_4346
        cmp    byte ptr [esi + 2], 0
        jne    SHORT loc_1_4346
        mov    byte ptr [esi + 2ch], 0
loc_1_4346:
        cmp    byte ptr [esi + 3], 0
        je     NEAR PTR loc_1_43D2
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_1_43D2
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 8]
loc_1_436B:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax]
        cli
        mov    ax, word ptr [ebx + 2]
        test   al, 1
        je     SHORT loc_1_43C5
        cmp    dword ptr [ebx + 10h], esi
        jne    SHORT loc_1_43C5
        test   ah, 10h
        je     SHORT loc_1_43B8
        test   edi, edi
        je     NEAR PTR loc_1_4437
        cmp    dword ptr [edi + 30h], 0
        jne    NEAR PTR loc_1_4437
        push   ebx
        mov    dword ptr [ebx + 1ch], edi
        call   sub_1_3B10
        add    esp, 4
        push   ebx
        call   sub_1_3A20
        add    esp, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [edi + 1ah], 0
        jmp    SHORT loc_1_43C5
loc_1_43B8:
        db  081h, 07Bh, 024h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_4446
loc_1_43C5:
        sti
        add    dword ptr [ebp - 8], 4
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], ecx
        jb     SHORT loc_1_436B
loc_1_43D2:
        mov    al, byte ptr [esi + 1dh]
        test   al, 80h
        jne    SHORT loc_1_441A
        or     al, 80h
        mov    ecx, dword ptr [esi + 0ch]
        mov    word ptr [esi + 1ah], 0
        mov    byte ptr [esi + 1dh], al
        mov    eax, ecx
        test   eax, eax
        je     SHORT loc_1_43F8
loc_1_43ED:
        or     byte ptr [eax + 1ch], 80h
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ecx
        jne    SHORT loc_1_43ED
loc_1_43F8:
        mov    eax, dword ptr [esi + 76h]
        mov    ecx, dword ptr [esi + 72h]
        push   ecx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 7ah]
        push   ecx
        push   eax
        call   dword ptr [eax + 2ch]
        mov    dword ptr [esi + 72h], 0
        mov    dword ptr [esi + 76h], 0
        add    esp, 0ch
loc_1_441A:
        cmp    byte ptr [esi + 2ch], 0
        jne    NEAR PTR loc_1_44AD
        mov    eax, dword ptr [esi + 8]
        cmp    esi, eax
        jne    SHORT loc_1_446E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4485
loc_1_4437:
        db  0C7h, 043h, 024h, 000h, 000h, 000h, 000h
        sti
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4446:
        test   edi, edi
        je     SHORT loc_1_4466
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_1_4466
        push   ebx
        call   sub_1_3B10
        add    esp, 4
        mov    word ptr [edi + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4466:
        sti
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_446E:
        mov    ecx, dword ptr [esi + 4]
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx + 8], eax
        jne    SHORT loc_1_4485
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_1_4485:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_4498
        dec    ax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_1_4498:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_5CAB
        add    esp, 4
        push   esi
        call   sub_1_5B38
        add    esp, 4
loc_1_44AD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_44C0:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        mov    eax, ecx
        push   ebx
        test   ecx, ecx
        push   esi
        je     SHORT loc_1_44EC
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
loc_1_44D7:
        cmp    dword ptr [eax + 76h], edx
        jne    SHORT loc_1_44E5
        xor    ebx, ebx
        mov    bl, byte ptr [eax + 7ah]
        cmp    ebx, esi
        je     SHORT loc_1_44EE
loc_1_44E5:
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ecx
        jne    SHORT loc_1_44D7
loc_1_44EC:
        xor    eax, eax
loc_1_44EE:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4500:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   1
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4512:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4520:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
loc_1_4527:
        cli
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        jne    SHORT loc_1_4537
        sti
        call   sub_1_5C08
        jmp    SHORT loc_1_4527
loc_1_4537:
        mov    ecx, dword ptr [eax + 40h]
        mov    dword ptr [esi + 0ch], ecx
        sti
        mov    dl, byte ptr [esi + 5ah]
        mov    dword ptr [eax + 44h], esi
        pop    esi
        mov    byte ptr [eax + 30h], dl
        mov    dword ptr [eax + 2ch], 0
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4560:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 44h]
        cli
        mov    dword ptr [eax + 44h], 0
        mov    edx, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 40h], edx
        mov    dword ptr [ecx + 0ch], eax
        sti
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_4580:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        cli
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_459F
        mov    eax, dword ptr [ebp + 0ch]
        lea    ecx, [ecx + eax*4 + 48h]
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_1_459F
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
loc_1_459F:
        sti
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_45B0:
        push   ebp
        mov    ebp, esp
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 10h]
        mov    ecx, dword ptr [edx + 0ch]
        lea    eax, [ecx + eax*4 + 48h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
        sti
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_45D0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 7
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, edx
        rep stosd
        stosb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 3], 3dh
        mov    esi, dword ptr [edi + 28h]
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [edx], 8
        mov    word ptr [edx + 0dh], 1
        mov    word ptr [edx + 11h], 0ah
        mov    dword ptr [edi + 28h], eax
        mov    al, byte ptr [ecx + 40h]
        mov    di, word ptr [edi + 35h]
        mov    byte ptr [edx + 1], al
        mov    word ptr [edx + 5], di
        mov    eax, dword ptr [esi + 10h]
        mov    word ptr [edx + 13h], ax
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 8]
        add    edx, 1dh
        sub    edi, eax
        push   esi
        inc    edi
        mov    dword ptr [edx - 8], eax
        mov    dword ptr [edx - 4], edi
        mov    dword ptr [ecx + 38h], edx
        mov    byte ptr [ecx + 40h], 24h
        call   sub_1_5B38
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4650:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        repne scasb
        not    ecx
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ebx, [ecx - 1]
        test   eax, eax
        je     SHORT loc_1_4690
loc_1_4672:
        lea    esi, [eax + 4]
        mov    ecx, ebx
        shr    ecx, 2
        mov    edi, edx
        repe cmpsd
        jne    SHORT loc_1_4687
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
loc_1_4687:
        je     SHORT loc_1_4690
        mov    eax, dword ptr [eax + 1ch]
        test   eax, eax
        jne    SHORT loc_1_4672
loc_1_4690:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_46A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   208h
        call   sub_1_5B32
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_46C8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_46C8:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, ebx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, ebx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BAh, 000h, 000h, 000h, 000h
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        xor    eax, eax
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [edi + 4], cl
        lea    edi, [ebp - 44h]
        mov    ecx, 0dh
        rep stosd
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 4], 0ffh
        mov    byte ptr [ebp - 44h], dl
        lea    edx, [ebp - 44h]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ah], ebx
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 30h], 0ffffh
        mov    dword ptr [ebp - 10h], edx
        lea    edx, [ebp - 10h]
        push   edx
        push   eax
        push   0
        call   sub_1_5C68
        add    esp, 0ch
        mov    esi, eax
        push   ebx
        call   sub_1_5B38
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_10:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 80h
        mov    dword ptr [eax], 0ffffffffh
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        cmp    dword ptr [ecx], 8
        jae    SHORT loc_1_4800
        mov    eax, 6fh
        pop    edi
        pop    esi
        mov    dword ptr [ecx], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4800:
        push   410h
        call   sub_1_5B32
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_1_4820
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4820:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, 208h
        push   ecx
        mov    dword ptr [ebp - 4], eax
        call   sub_1_4650
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_4846
        mov    dword ptr [ebp - 0ch], 0
        jmp    SHORT loc_1_4868
loc_1_4846:
        push   20h
        call   sub_1_5B32
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_4861
        mov    word ptr [ebp - 66h], 8
        jmp    NEAR PTR loc_1_49FE
loc_1_4861:
        mov    dword ptr [ebp - 0ch], 1
loc_1_4868:
        lea    edi, [ebp - 80h]
        xor    eax, eax
        mov    ecx, 1dh
        rep stosd
        mov    word ptr [ebp - 68h], 1
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    byte ptr [ebp - 7ch], 4
        mov    word ptr [ebp - 7ah], 0ffh
        mov    byte ptr [ebp - 7bh], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 74h], eax
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 4]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 74h]
        mov    cl, byte ptr [edx + 4]
        mov    edx, dword ptr [ebp - 4]
        push   edx
        mov    byte ptr [edi + 4], cl
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_493B:
        add    esp, 8
        lea    ecx, [ebp - 80h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4947:
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_49EF
        mov    esi, dword ptr [ebp - 64h]
        lea    edi, [esi + 1ch]
        test   byte ptr [edi], 80h
        je     SHORT loc_1_4989
        push   4ffh
        lea    eax, [ebp - 80h]
        push   eax
        call   sub_1_3700
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_4982
        mov    ecx, dword ptr [eax + 20h]
        push   eax
        mov    word ptr [ebp - 66h], cx
        call   sub_1_3A20
        add    esp, 4
loc_1_4982:
        cmp    word ptr [ebp - 66h], 0
        jne    SHORT loc_1_49EF
loc_1_4989:
        or     word ptr [edi], 110h
        mov    edx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [edx + 6ah]
        mov    dword ptr [ebx], esi
        mov    edi, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 10h]
        sub    eax, eax
        mov    dword ptr [ecx], 8
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 4]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], ebx
        je     SHORT loc_1_49FE
        inc    byte ptr [edx + 2]
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 1ch], eax
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        sti
        jmp    SHORT loc_1_49FE
loc_1_49EF:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_49FE
        push   ebx
        call   sub_1_5B38
        add    esp, 4
loc_1_49FE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5B38
        add    esp, 4
        movsx  eax, word ptr [ebp - 66h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   208h
        call   sub_1_5B32
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_4A48
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4A48:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, ebx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, ebx
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BAh, 000h, 000h, 000h, 000h
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    cl, byte ptr [edx + 4]
        lea    edx, [ebp - 10h]
        push   edx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    byte ptr [edi + 4], cl
        push   0
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 0ch], 3
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 4], 0ffh
        call   sub_1_5C6E
        add    esp, 0ch
        push   ebx
        call   sub_1_5B38
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_11:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 78h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        cmp    esi, -1
        jne    SHORT loc_1_4B50
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4B50:
        mov    ebx, dword ptr [esi]
        push   208h
        call   sub_1_5B32
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_1_4B72
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4B72:
        lea    edi, [ebp - 78h]
        xor    eax, eax
        mov    ecx, 1dh
        rep stosd
        lea    ecx, [ebp - 78h]
        mov    dword ptr [ebp - 5ch], ebx
        push   ecx
        mov    byte ptr [ebp - 73h], al
        mov    eax, dword ptr [ebx + 10h]
        and    word ptr [ebx + 1ch], 0feefh
        dec    byte ptr [eax + 2]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4B99:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4BAC
        lea    eax, [ebp - 78h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4BA9:
        add    esp, 4
loc_1_4BAC:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0ffffffffh
        cli
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    esi, edx
        jne    SHORT loc_1_4BCA
        mov    eax, dword ptr [esi + 1ch]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4BEB
loc_1_4BCA:
        cmp    dword ptr [edx + 1ch], 0
        je     SHORT loc_1_4BEB
        xor    ecx, ecx
loc_1_4BD2:
        mov    eax, dword ptr [edx + 1ch]
        cmp    esi, eax
        je     SHORT loc_1_4BE2
        cmp    dword ptr [eax + 1ch], ecx
        mov    edx, eax
        jne    SHORT loc_1_4BD2
        jmp    SHORT loc_1_4BEB
loc_1_4BE2:
        mov    eax, dword ptr [edx + 1ch]
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [edx + 1ch], ecx
loc_1_4BEB:
        sti
        push   esi
        call   sub_1_5B38
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5B38
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4C10:
        push   ebp
        mov    ebp, esp
        push   esi
        cli
        mov    esi, dword ptr [ebp + 0ch]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_4C5A
        db  0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        test   eax, eax
        jne    SHORT loc_1_4C51
        inc    eax
        push   1f4h
        db  068h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  088h, 00Dh, 014h, 000h, 000h, 000h
        call   sub_1_5C0E
        add    esp, 8
loc_1_4C51:
        push   esi
        call   sub_1_5BFC
        add    esp, 4
loc_1_4C5A:
        sti
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_4C60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10h]
        cli
        mov    edi, dword ptr [eax + 40h]
        test   edi, edi
        je     SHORT loc_1_4CA5
        mov    cl, byte ptr [eax + 62h]
        inc    cl
        mov    byte ptr [eax + 62h], cl
        cmp    cl, 4
        ja     SHORT loc_1_4CA5
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_1_4C92
        sti
        push   eax
        push   3
        call   sub_1_604A
        add    esp, 8
loc_1_4C92:
        sti
        push   2710h
        push   esi
        call   sub_1_5C0E
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_4CA5:
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_1_4CB6
        push   eax
        call   sub_1_5870
        add    esp, 4
        jmp    SHORT loc_1_4CBD
loc_1_4CB6:
        mov    dword ptr [eax + 40h], 0
loc_1_4CBD:
        test   edi, edi
        je     SHORT loc_1_4CD7
        and    byte ptr [edi + 1eh], 0efh
        mov    dword ptr [edi + 18h], 0
        push   3ah
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4CD4:
        add    esp, 8
loc_1_4CD7:
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4CE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 10h]
        cli
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_4CFD
        push   esi
        call   sub_1_5870
        add    esp, 4
        jmp    SHORT loc_1_4D04
loc_1_4CFD:
        mov    dword ptr [esi + 40h], 0
loc_1_4D04:
        sti
        mov    eax, dword ptr [esi + 40h]
        test   eax, eax
        je     SHORT loc_1_4D35
        and    byte ptr [eax + 1eh], 0efh
        mov    dword ptr [eax + 18h], 0
        mov    cl, byte ptr [eax + 5ch]
        cmp    cl, 0d0h
        je     SHORT loc_1_4D29
        cmp    cl, 0d6h
        mov    ecx, 3ah
        jne    SHORT loc_1_4D2B
loc_1_4D29:
        xor    ecx, ecx
loc_1_4D2B:
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4D32:
        add    esp, 8
loc_1_4D35:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_4D40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 40h], 0
        jne    SHORT loc_1_4D57
        push   eax
        call   sub_1_5870
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4D57:
        cmp    byte ptr [eax + 9], 0ffh
        jne    SHORT loc_1_4D72
        mov    cl, byte ptr [eax + 62h]
        inc    cl
        mov    byte ptr [eax + 62h], cl
        cmp    cl, 4
        jae    SHORT loc_1_4D72
        push   eax
        push   3
        call   sub_1_604A
loc_1_4D72:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D80:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 10h]
        mov    cl, byte ptr [esi + 24h]
        inc    cl
        mov    byte ptr [esi + 24h], cl
        cmp    cl, 4
        jbe    SHORT loc_1_4DF5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4DB9
loc_1_4DA6:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     SHORT loc_1_4DB5
        cmp    dword ptr [eax], 0
        mov    ecx, eax
        jne    SHORT loc_1_4DA6
        jmp    SHORT loc_1_4DB9
loc_1_4DB5:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
loc_1_4DB9:
        mov    eax, dword ptr [esi + 1eh]
        test   eax, eax
        je     SHORT loc_1_4DCB
        push   43h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4DC8:
        add    esp, 8
loc_1_4DCB:
        cli
        mov    eax, dword ptr [esi + 1ah]
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_4DDE
        mov    dword ptr [eax + 40h], 0
        jmp    SHORT loc_1_4DE7
loc_1_4DDE:
        push   eax
        call   sub_1_5870
        add    esp, 4
loc_1_4DE7:
        sti
        push   esi
        call   sub_1_5B38
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_4DF5:
        mov    eax, dword ptr [esi + 1ah]
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_4E05
        dec    cl
        mov    byte ptr [esi + 24h], cl
        jmp    SHORT loc_1_4E14
loc_1_4E05:
        mov    byte ptr [esi + 2bh], 14h
        push   eax
        push   3
        call   sub_1_604A
        add    esp, 8
loc_1_4E14:
        push   1f4h
        push   edi
        call   sub_1_5C0E
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4E30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 9], 0
        mov    edi, dword ptr [esi + 24h]
        je     SHORT loc_1_4E4A
        push   esi
        call   sub_1_5870
        add    esp, 4
loc_1_4E4A:
        mov    edx, dword ptr [edi + 16h]
        lea    ecx, [edi + 12h]
        mov    byte ptr [edi + 5], 4
        add    edi, 6
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 8]
        push   esi
        push   3
        mov    dword ptr [edi], eax
        mov    byte ptr [edi + 38h], 0f4h
        db  0C7h, 046h, 004h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 8], ecx
        call   sub_1_604A
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_4E80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_5B38
        add    esp, 4
        push   esi
        call   sub_1_5870
        add    esp, 4
        pop    esi
        pop    ebp
        ret
        db  090h
loc_1_4EA0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 40h]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4EB4:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_4EC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 40h]
        and    byte ptr [eax + 1eh], 0fbh
        mov    cx, word ptr [eax + 5fh]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4EDB:
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_4EE0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 40h]
        cmp    byte ptr [ecx + 9], 0
        jne    SHORT loc_1_4F1A
        test   byte ptr [edx + 1eh], 1
        je     SHORT loc_1_4F0A
        xor    eax, eax
        mov    ax, word ptr [ecx + 28h]
        add    eax, 20h
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4F06:
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4F0A:
        xor    eax, eax
        mov    ax, word ptr [ecx + 28h]
        add    eax, 20h
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4F1A:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 048h, 040h, 080h, 078h, 009h, 000h
        db  075h, 016h, 08Ah, 041h, 05Ch, 02Ch, 0D5h, 03Ch, 001h, 01Bh, 0C0h, 083h, 0E0h, 002h, 083h, 0C0h
        db  023h, 050h, 051h, 0E8h, 000h, 000h, 000h, 000h, 08Bh, 0E5h, 05Dh, 0C3h, 08Dh, 09Bh, 000h, 000h
        db  000h, 000h
sub_1_4F50:
        push   ebp
        mov    ebp, esp
        push   120h
        call   sub_1_5B32
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4F68
        xor    eax, eax
        pop    ebp
        ret
loc_1_4F68:
        mov    dword ptr [eax + 58h], 424d53ffh
        mov    word ptr [eax + 6ch], 1
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_4F82:
        inc    cx
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [eax + 6ah], cx
        je     SHORT loc_1_4F82
        cli
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 000h, 000h, 000h, 000h
        sti
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4FB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4FBD:
        add    esp, 4
        cli
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, esi
        jne    SHORT loc_1_4FD4
        mov    eax, dword ptr [esi]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4FE8
loc_1_4FD4:
        test   ecx, ecx
        je     SHORT loc_1_4FE4
loc_1_4FD8:
        mov    eax, dword ptr [ecx]
        cmp    eax, esi
        je     SHORT loc_1_4FE4
        mov    ecx, eax
        test   ecx, ecx
        jne    SHORT loc_1_4FD8
loc_1_4FE4:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
loc_1_4FE8:
        sti
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_1_4FF9
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4FF6:
        add    esp, 4
loc_1_4FF9:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_1_5009
        push   eax
        call   sub_1_5B38
        add    esp, 4
loc_1_5009:
        push   esi
        call   sub_1_5B38
        add    esp, 4
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi + 1ch], 5105h
        jne    SHORT loc_1_5054
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   esi
        push   edx
        call   sub_1_5480
        add    esp, 10h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5054:
        add    esi, 1eh
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ecx]
        sub    eax, 1eh
        mov    dword ptr [ecx], eax
        cmp    eax, 23h
        jb     NEAR PTR loc_1_51F8
        cmp    dword ptr [esi], 424d53ffh
        jne    NEAR PTR loc_1_51F8
        mov    ecx, dword ptr [ebp + 14h]
        sub    ecx, 1eh
        push   ecx
        push   eax
        push   esi
        call   sub_1_3660
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_51F8
        mov    cx, word ptr [esi + 1eh]
        cmp    cx, 0ffffh
        jne    SHORT loc_1_50C1
        push   esi
        xor    eax, eax
        mov    ax, word ptr [esi + 12h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_44C0
        add    esp, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_50B4:
        add    esp, 8
        sti
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_50C1:
        xor    eax, eax
        mov    ax, cx
        and    eax, 3fh
        je     NEAR PTR loc_1_51F8
        db  08Bh, 03Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        test   edi, edi
        je     NEAR PTR loc_1_51F8
        cli
        cmp    word ptr [edi], cx
        jne    NEAR PTR loc_1_51F8
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_51F8
        mov    dx, word ptr [edi + 2]
        test   dl, 10h
        je     NEAR PTR loc_1_51F8
        cmp    byte ptr [esi + 5], 2
        jne    SHORT loc_1_5127
        cmp    word ptr [esi + 7], 11h
        jne    SHORT loc_1_5127
        test   dh, 80h
        jne    SHORT loc_1_5127
        mov    byte ptr [edi + 6], 0
        or     dl, 8
        mov    word ptr [edi + 2], dx
        sti
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5127:
        mov    cx, word ptr [esi + 14h]
        test   cx, cx
        je     SHORT loc_1_5158
        mov    eax, dword ptr [eax + 72h]
        cmp    word ptr [eax + 2], cx
        jne    NEAR PTR loc_1_51F8
loc_1_513D:
        inc    word ptr [eax + 2]
        je     SHORT loc_1_513D
        cmp    word ptr [eax + 4], 0
        jne    SHORT loc_1_5158
        cmp    byte ptr [esi + 5], 0
        jne    SHORT loc_1_5158
        mov    cx, word ptr [esi + 12h]
        mov    word ptr [eax + 4], cx
loc_1_5158:
        cmp    byte ptr [edi + 40h], 1bh
        je     SHORT loc_1_5167
        and    dx, 0ffefh
        mov    word ptr [edi + 2], dx
loc_1_5167:
        sti
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 4], 0
        push   eax
        push   ecx
        push   esi
        push   edi
        call   sub_1_26C0
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_51F8
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5820
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_51F8
        mov    word ptr [ebx + 0ah], 5205h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ebx + 22h], 0
        mov    eax, dword ptr [ecx]
        add    eax, 1eh
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 14h]
        sub    cx, ax
        mov    eax, dword ptr [edi + 30h]
        mov    word ptr [ebx + 28h], cx
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 24h], eax
        inc    byte ptr [edi + 7]
        mov    word ptr [ebx + 22h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 5
        inc    eax
        mov    dword ptr [ebx + 40h], edi
        cdq
        db  089h, 01Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        db  0A3h, 000h, 000h, 000h, 000h
        idiv   ecx
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
loc_1_51F8:
        sti
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5210:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 40h]
        cli
        xor    ecx, ecx
        push   eax
        mov    cx, word ptr [eax + 28h]
        dec    byte ptr [esi + 7]
        mov    dword ptr [esi + 34h], ecx
        call   sub_1_5870
        add    esp, 4
        push   esi
        call   sub_1_3B60
        add    esp, 4
        sti
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_5240:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 40h]
        mov    cl, byte ptr [esi + 9]
        cmp    cl, 0ffh
        jne    SHORT loc_1_5273
        mov    al, byte ptr [esi + 62h]
        dec    al
        mov    byte ptr [esi + 62h], al
        je     SHORT loc_1_5273
        push   esi
        push   3
        call   sub_1_604A
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5273:
        test   edi, edi
        je     SHORT loc_1_52A6
        dec    byte ptr [edi + 7]
        test   edi, edi
        je     SHORT loc_1_52A6
        test   byte ptr [edi + 2], 2
        je     SHORT loc_1_52B6
        test   cl, cl
        je     SHORT loc_1_529D
        xor    eax, eax
        mov    al, cl
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5297:
        add    esp, 8
        mov    dword ptr [edi + 20h], eax
loc_1_529D:
        push   edi
        call   sub_1_3B60
        add    esp, 4
loc_1_52A6:
        push   esi
        call   sub_1_5870
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_52B6:
        lea    eax, [edi + 5ch]
        mov    ebx, dword ptr [edi + 1ch]
        mov    edx, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 8], ebx
        mov    ecx, dword ptr [ebx + 44h]
        mov    ebx, ecx
        mov    dword ptr [ebp - 0ch], ecx
loc_1_52CA:
        mov    ecx, dword ptr [eax + 11h]
        sub    dword ptr [edi + 2ch], edx
        add    ecx, ecx
        add    dword ptr [eax + 7], edx
        mov    dword ptr [eax + 11h], ecx
        test   ecx, ebx
        jne    SHORT loc_1_52CA
        mov    ecx, dword ptr [edi + 10h]
        mov    ebx, dword ptr [ecx + 72h]
        mov    cl, byte ptr [esi + 63h]
        inc    cl
        mov    dword ptr [ebp - 4], ebx
        mov    byte ptr [esi + 63h], cl
        cmp    byte ptr [ebx + 0ah], cl
        jbe    SHORT loc_1_5300
        cmp    dword ptr [eax + 11h], 80000000h
        je     SHORT loc_1_5300
        cmp    dword ptr [edi + 2ch], edx
        ja     SHORT loc_1_5321
loc_1_5300:
        mov    ecx, dword ptr [edi + 2ch]
        cmp    edx, ecx
        jbe    SHORT loc_1_530E
        mov    word ptr [eax + 15h], cx
        mov    dword ptr [edi + 18h], ecx
loc_1_530E:
        mov    ecx, dword ptr [eax + 11h]
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 11h], 0
        or     ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 44h], ecx
loc_1_5321:
        mov    byte ptr [esi + 62h], 4
        mov    dword ptr [esi + 40h], 0
        mov    ebx, dword ptr [ebp - 4]
        push   ebx
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5337:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5350:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        mov    edx, dword ptr [edi + 76h]
        test   edx, edx
        je     NEAR PTR loc_1_5438
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_53B5
        cli
        mov    al, byte ptr [esi + 6]
        inc    al
        mov    byte ptr [esi + 6], al
        cmp    al, 2
        jb     SHORT loc_1_5384
        cmp    byte ptr [esi + 7], 0
        jbe    SHORT loc_1_5393
        dec    al
        mov    byte ptr [esi + 6], al
loc_1_5384:
        mov    eax, dword ptr [edi + 72h]
        mov    ax, word ptr [eax]
        mov    word ptr [esi + 4], ax
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_5393:
        inc    word ptr [edi + 8dh]
        push   esi
        and    word ptr [esi + 2], 0ffefh
        mov    dword ptr [esi + 34h], 0
        call   sub_1_3B60
        add    esp, 4
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_53B5:
        test   byte ptr [esi + 3], 80h
        je     SHORT loc_1_53CF
        mov    dword ptr [esi + 20h], 37h
        push   esi
        call   sub_1_3B60
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_53CF:
        cmp    byte ptr [esi + 6], 0ah
        jae    SHORT loc_1_542D
        mov    ax, word ptr [esi + 8]
        test   ax, ax
        jne    SHORT loc_1_53E5
        mov    eax, 64h
        jmp    SHORT loc_1_53E8
loc_1_53E5:
        movzx  eax, ax
loc_1_53E8:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0ah]
        cmp    ecx, eax
        jge    SHORT loc_1_542D
        cmp    byte ptr [esi + 40h], 1eh
        jne    SHORT loc_1_53FF
        cmp    word ptr [esi + 50h], 0
        je     SHORT loc_1_5438
loc_1_53FF:
        push   1
        push   edx
        call   sub_1_5820
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_5438
        inc    word ptr [esi + 0ah]
        test   byte ptr [esi + 2], 8
        jne    SHORT loc_1_541B
        inc    byte ptr [esi + 6]
loc_1_541B:
        mov    ecx, dword ptr [edi + 72h]
        push   ecx
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5426:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_542D:
        push   1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5435:
        add    esp, 8
loc_1_5438:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_5440:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 40h], 0
        jne    SHORT loc_1_5457
        push   eax
        call   sub_1_5870
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5457:
        cmp    byte ptr [eax + 9], 0ffh
        jne    SHORT loc_1_546F
        mov    cl, byte ptr [eax + 62h]
        dec    cl
        mov    byte ptr [eax + 62h], cl
        je     SHORT loc_1_546F
        push   eax
        push   3
        call   sub_1_604A
loc_1_546F:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5480:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        cmp    dword ptr [ebp + 14h], 62h
        push   ebx
        push   esi
        push   edi
        jne    NEAR PTR loc_1_5685
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 3eh], 0f4h
        jne    NEAR PTR loc_1_5685
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        je     NEAR PTR loc_1_5685
        mov    ax, word ptr [eax + 40h]
loc_1_54B9:
        cmp    word ptr [ebx + 26h], ax
        je     SHORT loc_1_54CF
        mov    dword ptr [ebp - 4], ebx
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_1_54B9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_54CF:
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [ebx + 6dh]
        mov    ecx, 10h
        lea    esi, [esi + 42h]
        repe cmpsb
        jne    NEAR PTR loc_1_5685
        mov    edi, dword ptr [ebx + 22h]
        lea    eax, [ebx + 4]
        mov    dword ptr [ebx + 22h], 0
        push   eax
        call   sub_1_5C1A
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebx + 1ah]
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        lea    ecx, [edi + 14h]
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [edi + 1eh], 5005h
        lea    ecx, [edi + 0eh]
        mov    edx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    word ptr [ecx + 4], ax
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 6]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi + 0dh], 0
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_5552
        test   ecx, ecx
        je     SHORT loc_1_5552
        mov    byte ptr [edi + 0dh], 1
loc_1_5552:
        mov    word ptr [edi], 2
        mov    al, byte ptr [ebx + 29h]
        mov    ecx, dword ptr [esi + 76h]
        dec    byte ptr [esi + 2]
        not    al
        and    byte ptr [esi + 7bh], al
        test   ecx, ecx
        je     SHORT loc_1_5575
        mov    al, byte ptr [esi + 7ch]
        cmp    byte ptr [ebx + 2ah], al
        jae    NEAR PTR loc_1_563D
loc_1_5575:
        mov    al, byte ptr [esi + 7ah]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [esi + 72h]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebx + 2ah]
        mov    dword ptr [ebp - 10h], ecx
        mov    ecx, dword ptr [ebp + 8]
        push   7
        push   esi
        mov    byte ptr [esi + 7ch], al
        mov    dword ptr [esi + 76h], ecx
        mov    dword ptr [esi + 72h], edi
        mov    byte ptr [esi + 7ah], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_559F:
        add    esp, 8
        mov    eax, 2
        mov    byte ptr [edi + 0ah], al
        mov    byte ptr [edi + 0bh], al
        mov    byte ptr [edi + 0ch], 0
        mov    dword ptr [edi + 6], 0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_55D2
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   ecx
        call   dword ptr [ecx + 2ch]
        add    esp, 0ch
loc_1_55D2:
        lea    eax, [ebp - 24h]
        lea    ecx, [esi + 2ch]
        push   eax
        mov    edi, ecx
        push   ecx
        sub    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_55EE:
        add    esp, 0ch
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 24h], 2
        lea    ecx, [ebp - 23h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], edi
        mov    dx, word ptr [edx + 8]
        mov    word ptr [ecx + 8], dx
        lea    edx, [ebp - 19h]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
        lea    ecx, [esi + 2ch]
        lea    edx, [ebp - 24h]
        mov    edi, ecx
        push   edx
        sub    eax, eax
        push   ecx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_563B:
        jmp    SHORT loc_1_5647
loc_1_563D:
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   dword ptr [eax + 2ch]
loc_1_5647:
        add    esp, 0ch
        cli
        mov    eax, dword ptr [ebx + 1eh]
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_565D
        mov    dword ptr [eax + 40h], 0
        jmp    SHORT loc_1_5666
loc_1_565D:
        push   eax
        call   sub_1_5870
        add    esp, 4
loc_1_5666:
        sti
        push   ebx
        call   sub_1_5B38
        add    esp, 4
        cmp    byte ptr [esi + 7ch], 1
        jbe    SHORT loc_1_567C
        cmp    byte ptr [esi + 7bh], 0
        jne    SHORT loc_1_5685
loc_1_567C:
        push   esi
        call   sub_1_5C02
        add    esp, 4
loc_1_5685:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_5690:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 9]
        cmp    cl, 0ffh
        jne    SHORT loc_1_56B9
        mov    al, byte ptr [esi + 62h]
        dec    al
        mov    byte ptr [esi + 62h], al
        je     SHORT loc_1_56B9
        push   esi
        push   3
        call   sub_1_604A
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_56B9:
        mov    edi, dword ptr [esi + 40h]
        test   edi, edi
        je     SHORT loc_1_56E8
        test   byte ptr [edi + 2], 2
        je     SHORT loc_1_56E8
        test   cl, cl
        je     SHORT loc_1_56DF
        xor    eax, eax
        mov    al, cl
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_56D9:
        add    esp, 8
        mov    dword ptr [edi + 20h], eax
loc_1_56DF:
        push   edi
        call   sub_1_3B60
        add    esp, 4
loc_1_56E8:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_1_5B38
        add    esp, 4
        push   esi
        call   sub_1_5870
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5710:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 40h]
        push   eax
        call   sub_1_5870
        add    esp, 4
        push   208h
        call   sub_1_5B32
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     NEAR PTR loc_1_5803
        lea    eax, [ebx + 24h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        lea    esi, [ebx + 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5751:
        add    esp, 8
        lea    ecx, [ebp - 30h]
        push   1
        push   10h
        push   esi
        push   ecx
        call   sub_1_5C4A
        add    esp, 10h
        lea    eax, [ebx + 34h]
        lea    ecx, [ebx + 14h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, 10h
        repe cmpsb
        jne    SHORT loc_1_57D5
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        lea    esi, [ebx + 24h]
        db  0BFh, 000h, 000h, 000h, 000h
        not    ecx
        mov    edx, ecx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_57D5
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_57D5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_57D5
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 30h]
        push   eax
        mov    edx, dword ptr [ebx + 46h]
        push   ecx
        mov    al, byte ptr [edx + 5ah]
        push   eax
        mov    ecx, dword ptr [ebx]
        push   ecx
        lea    edx, [ebx + 4ah]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_57D0:
        add    esp, 14h
        jmp    SHORT loc_1_57F7
loc_1_57D5:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 30h]
        push   eax
        mov    edx, dword ptr [ebx + 46h]
        push   ecx
        mov    al, byte ptr [edx + 5ah]
        push   eax
        mov    ecx, dword ptr [ebx]
        push   ecx
        lea    edx, [ebx + 4ah]
        push   edx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5C32
        add    esp, 18h
loc_1_57F7:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5B38
        add    esp, 4
loc_1_5803:
        push   ebx
        call   sub_1_58D0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5820:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
loc_1_5827:
        db  0A1h, 000h, 000h, 000h, 000h
        cli
        test   eax, eax
        jne    SHORT loc_1_583D
        sti
        test   esi, esi
        jne    SHORT loc_1_5861
        call   sub_1_5C08
        jmp    SHORT loc_1_5827
loc_1_583D:
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        sti
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], ecx
        mov    byte ptr [eax + 8], cl
        pop    esi
        mov    dword ptr [eax + 40h], ecx
        pop    ebp
        mov    byte ptr [eax + 63h], cl
        mov    byte ptr [eax + 62h], 4
        mov    dword ptr [eax + 3ch], edx
        ret
loc_1_5861:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5870:
        push   ebp
        mov    ebp, esp
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    dword ptr [ecx + 3ch], eax
        mov    dword ptr [ecx + 40h], eax
        sti
        pop    ebp
        ret
        db  090h
sub_1_5890:
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_58A4
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        sti
        ret
loc_1_58A4:
        sti
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        jge    SHORT loc_1_58C5
        push   20ch
        call   sub_1_5B32
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_58C5
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_1_58C5:
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_58D0:
        push   ebp
        mov    ebp, esp
        cli
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        sti
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VREDIR_Control:
        cmp    eax, 35h
        jae    SHORT loc_1_59D4
        db  02Eh, 0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_59D3:
        clc
loc_1_59D4:
        ret
VREDIR_W32_DEVICEIOCONTROL:
        cmp    ecx, 0
        jne    SHORT loc_1_59DE
        xor    eax, eax
        jmp    SHORT loc_1_59E7
loc_1_59DE:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_59E4:
        add    esp, 4
loc_1_59E7:
        ret
VREDIR_END_PM_APP:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_5A09
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_59F6:
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 004h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A08:
        pop    ebx
loc_1_5A09:
        clc
        ret
loc_1_5A0B:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A10:
        ret
VREDIR_SYS_VM_INIT:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A17:
        pop    ebx
        clc
        ret
        db  0CCh, 0C3h
loc_1_5A1C:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A21:
        cmp    eax, 1
        jae    SHORT loc_1_5A27
        ret
loc_1_5A27:
        ja     SHORT loc_1_5A2F
        db  0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_5A2F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 1ah], 1
        jne    SHORT loc_1_5A54
        mov    word ptr [eax + 1ah], 0
loc_1_5A54:
        pop    ebp
        ret
VRedir_Get_Version:
        mov    eax, 86h
        clc
        ret
VREDIR_PNP_NEW_DEVNODE:
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A64:
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_5A6C
        stc
loc_1_5A6C:
        ret
sub_1_5A6D:
        db  0CDh, 020h, 061h, 080h, 040h, 000h
        ret
        db  0E8h, 000h, 000h, 000h, 000h, 068h, 000h, 000h, 002h, 000h, 06Ah, 022h, 068h, 000h, 000h, 000h
        db  000h, 0CDh, 020h, 002h, 000h, 040h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 073h, 027h, 0A3h
        db  000h, 000h, 000h, 000h, 06Ah, 022h, 068h, 000h, 000h, 000h, 000h, 0CDh, 020h, 003h, 000h, 040h
        db  000h, 083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 073h, 00Ah, 0A3h, 000h, 000h, 000h, 000h, 0E8h, 000h
        db  000h, 000h, 000h, 033h, 0C0h, 0C3h, 0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_1_5AC0:
        push   20000h
        push   22h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 040h, 000h
        add    esp, 0ch
        ret
sub_1_5AD6:
        push   22h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 040h, 000h
        add    esp, 8
        ret
sub_1_5AE7:
        mov    ecx, dword ptr [esp + 4]
        db  0CDh, 020h, 006h, 000h, 014h, 000h
        jae    SHORT loc_1_5AF8
        mov    eax, 0ffffffffh
loc_1_5AF8:
        ret
sub_1_5AF9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_5B18
        mov    ebx, 3
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 087h, 004h
        jae    SHORT loc_1_5B1A
loc_1_5B18:
        xor    eax, eax
loc_1_5B1A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh
sub_1_5B20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        leave
        ret
loc_1_5B2E:
        sub    eax, eax
        ret
        db  090h
sub_1_5B32:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_5B38:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_5B3E:
        db  0CDh, 020h, 005h, 080h, 080h, 004h
sub_1_5B44:
        db  0CDh, 020h, 006h, 080h, 080h, 004h
        mov    eax, eax
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 080h, 001h, 000h
        nop
sub_1_5B58:
        db  0CDh, 020h, 020h, 080h, 001h, 000h
        mov    eax, eax
sub_1_5B60:
        db  0CDh, 020h, 060h, 080h, 040h, 000h
        mov    eax, eax
sub_1_5B68:
        db  0CDh, 020h, 005h, 000h, 006h, 000h
        movzx  eax, cl
        ret
        db  08Bh, 0C0h
sub_1_5B74:
        db  0CDh, 020h, 001h, 080h, 09Bh, 004h
        mov    eax, eax
sub_1_5B7C:
        db  0CDh, 020h, 002h, 080h, 09Bh, 004h
        mov    eax, eax
sub_1_5B84:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Dh, 000h, 006h, 000h
        jb     SHORT loc_1_5BA1
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], esi
        xchg   edi, eax
        pop    esi
        pop    edi
        leave
        ret
loc_1_5BA1:
        xor    eax, eax
        pop    esi
        pop    edi
        leave
        ret
        db  090h
sub_1_5BA8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 006h, 000h
        pop    esi
        leave
        ret
        db  0CDh, 020h, 05Fh, 080h, 001h, 000h, 08Bh, 0C0h, 053h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh
        db  043h, 004h, 05Bh, 0C3h, 053h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 0C3h, 05Bh, 0C3h, 090h
sub_1_5BD8:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
sub_1_5BDE:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
sub_1_5BE4:
        db  0CDh, 020h, 008h, 080h, 040h, 000h
sub_1_5BEA:
        db  0CDh, 020h, 009h, 080h, 040h, 000h
sub_1_5BF0:
        db  0CDh, 020h, 025h, 080h, 040h, 000h
sub_1_5BF6:
        db  0CDh, 020h, 02Eh, 080h, 040h, 000h
sub_1_5BFC:
        db  0CDh, 020h, 012h, 080h, 040h, 000h
sub_1_5C02:
        db  0CDh, 020h, 013h, 080h, 040h, 000h
sub_1_5C08:
        db  0CDh, 020h, 014h, 080h, 040h, 000h
sub_1_5C0E:
        db  0CDh, 020h, 015h, 080h, 040h, 000h
sub_1_5C14:
        db  0CDh, 020h, 016h, 080h, 040h, 000h
sub_1_5C1A:
        db  0CDh, 020h, 017h, 080h, 040h, 000h
sub_1_5C20:
        db  0CDh, 020h, 018h, 080h, 040h, 000h
sub_1_5C26:
        db  0CDh, 020h, 00Bh, 080h, 040h, 000h
sub_1_5C2C:
        db  0CDh, 020h, 019h, 080h, 040h, 000h
sub_1_5C32:
        db  0CDh, 020h, 01Bh, 080h, 040h, 000h
        db  0CDh, 020h, 01Ah, 080h, 040h, 000h
sub_1_5C3E:
        db  0CDh, 020h, 03Eh, 080h, 040h, 000h
sub_1_5C44:
        db  0CDh, 020h, 03Dh, 080h, 040h, 000h
sub_1_5C4A:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
sub_1_5C50:
        db  0CDh, 020h, 041h, 080h, 040h, 000h
sub_1_5C56:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_1_5C5C:
        db  0CDh, 020h, 043h, 080h, 040h, 000h
sub_1_5C62:
        db  0CDh, 020h, 02Ch, 080h, 040h, 000h
sub_1_5C68:
        db  0CDh, 020h, 02Ah, 080h, 040h, 000h
sub_1_5C6E:
        db  0CDh, 020h, 02Bh, 080h, 040h, 000h
sub_1_5C74:
        db  0CDh, 020h, 037h, 080h, 040h, 000h
sub_1_5C7A:
        db  0CDh, 020h, 038h, 080h, 040h, 000h
sub_1_5C80:
        db  0CDh, 020h, 039h, 080h, 040h, 000h
sub_1_5C86:
        db  0CDh, 020h, 03Bh, 080h, 040h, 000h
sub_1_5C8C:
        db  0CDh, 020h, 03Ah, 080h, 040h, 000h
sub_1_5C92:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_5C98:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        leave
        ret
sub_1_5CAB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    eax, 8
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_5D06
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], esi
        db  0A3h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_5D21
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 001h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        sub    eax, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 0bh
        mov    edx, 0
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        jmp    SHORT loc_1_5D21
loc_1_5D06:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        sub    eax, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 0bh
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 014h, 000h, 001h, 000h
loc_1_5D21:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_1_5D26:
        push   ebp
        mov    ebp, esp
        db  080h, 025h, 000h, 000h, 000h, 000h, 0FEh
loc_1_5D30:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_5D58
        mov    edx, dword ptr [eax + 4]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    eax, dword ptr [edx]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   edx
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        jmp    SHORT loc_1_5D30
loc_1_5D58:
        leave
        ret
loc_1_5D5A:
        push   ebp
        mov    ebp, esp
        mov    eax, edx
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        leave
        ret
        db  090h
sub_1_5D68:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        pop    ebx
        leave
        ret
sub_1_5D78:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ebx
        leave
        ret
        db  02Eh, 08Bh, 0C0h
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_1_5DAD
        shr    ecx, 2
        rep stosd
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 3
loc_1_5DAD:
        rep stosb
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    edi
        leave
        ret
sub_1_5DB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_1_5DED
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 3ch]
        ror    eax, 10h
        mov    word ptr [ebx + 3ch], ax
        shr    eax, 10h
        mov    dword ptr [ebx + 4], eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    dword ptr [ebx + 3ch]
        pop    dword ptr [ebx + 4]
loc_1_5DED:
        pop    ebx
        leave
        ret
sub_1_5DF0:
        db  0CDh, 020h, 007h, 000h, 040h, 000h
        push   eax
        db  0CDh, 020h, 038h, 000h, 040h, 000h
        add    esp, 4
        ret
        db  090h
sub_1_5E02:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        push   ecx
        push   edx
        sub    edx, edx
        mov    ecx, 3e8h
        div    ecx
        pop    edx
        pop    ecx
        ret
sub_1_5E16:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_5E20:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        and    eax, 0ffh
        jne    SHORT loc_1_5E38
        pop    ebx
        leave
        ret
loc_1_5E38:
        cmp    al, 0ffh
        jne    SHORT loc_1_5E41
        xor    eax, eax
        pop    ebx
        leave
        ret
loc_1_5E41:
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5E4C:
        add    esp, 8
        pop    ebx
        leave
        ret
loc_1_5E52:
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5E7A
loc_1_5E59:
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5E7A
loc_1_5E60:
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5E7A
loc_1_5E67:
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5E7A
loc_1_5E6E:
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5E7A
loc_1_5E75:
        db  0BAh, 000h, 000h, 000h, 000h
loc_1_5E7A:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_5E8C
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_1_5E94
loc_1_5E8C:
        mov    dword ptr [ebx + 44h], 0
        ret
loc_1_5E94:
        mov    dword ptr [ebx + 36h], edx
        pushfd
        cli
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        jne    SHORT loc_1_5EC7
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 32h], ecx
        popfd
        db  0BEh, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        mov    eax, 400000h
        mov    ecx, 801h
        db  0CDh, 020h, 014h, 080h, 001h, 000h
loc_1_5EC4:
        mov    ecx, dword ptr [ecx + 32h]
loc_1_5EC7:
        cmp    dword ptr [ecx + 32h], 0
        jne    SHORT loc_1_5EC4
        mov    dword ptr [ecx + 32h], ebx
        mov    dword ptr [ebx + 32h], 0
        popfd
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_5EDC:
        cld
        cli
loc_1_5EDE:
        cli
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_1_5EFC
        mov    eax, dword ptr [ecx + 32h]
        db  0A3h, 000h, 000h, 000h, 000h
        sti
        push   dword ptr [ecx + 4]
        push   ecx
        call   dword ptr [ecx + 36h]
        add    esp, 8
        jmp    SHORT loc_1_5EDE
loc_1_5EFC:
        sti
        ret
        db  08Bh, 0C0h
loc_1_5F00:
        push   ecx
        push   esp
        movzx  ebx, word ptr [edi + 2]
        xchg   bh, bl
        push   ebx
        push   esi
        push   edi
        push   edx
        call   sub_1_5020
        add    esp, 14h
        mov    esi, eax
        pop    ebx
        ret
sub_1_5F18:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        cld
        jb     SHORT loc_1_5F45
        mov    edx, 5205h
        mov    ebx, 0eh
        xor    eax, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
loc_1_5F41:
        pop    esi
        pop    ebx
        leave
        ret
loc_1_5F45:
        mov    eax, 3ah
        jmp    SHORT loc_1_5F41
sub_1_5F4C:
        push   ebx
        push   ecx
        mov    ebx, 1ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ecx
        pop    ebx
        ret
loc_1_5F5C:
        push   ecx
        push   esp
        movzx  ebx, word ptr [edi + 2]
        xchg   bh, bl
        push   ebx
        push   esi
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5F6D:
        add    esp, 14h
        mov    esi, eax
        pop    ebx
        ret
sub_1_5F74:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, 5305h
        mov    ebx, 0eh
        xor    eax, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        test   eax, eax
        jne    SHORT loc_1_5F9E
        mov    eax, edx
loc_1_5F9A:
        pop    esi
        pop    ebx
        leave
        ret
loc_1_5F9E:
        xor    eax, eax
        jmp    SHORT loc_1_5F9A
        db  08Bh, 0C0h
loc_1_5FA4:
        push   ecx
        push   esp
        movzx  ebx, word ptr [edi + 2]
        xchg   bh, bl
        push   ebx
        push   esi
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5FB5:
        add    esp, 14h
        mov    esi, eax
        pop    ebx
        ret
sub_1_5FBC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, 5405h
        mov    ebx, 0eh
        xor    eax, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        test   eax, eax
        jne    SHORT loc_1_5FE6
        mov    eax, edx
loc_1_5FE2:
        pop    esi
        pop    ebx
        leave
        ret
loc_1_5FE6:
        xor    eax, eax
        int3
        jmp    SHORT loc_1_5FE2
        db  090h
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        mov    eax, 0ffh
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        and    eax, eax
        jne    SHORT loc_1_600A
        mov    eax, edx
loc_1_6007:
        pop    ebx
        leave
        ret
loc_1_600A:
        xor    eax, eax
        jmp    SHORT loc_1_6007
        db  08Bh, 0C0h
sub_1_6010:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, 1
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    ebx
        leave
        ret
sub_1_6026:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        cmp    al, 1
        cmc
        sbb    eax, eax
        or     eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_604A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0C0h
sub_1_6064:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + edi], 0
        sub    eax, eax
        repne scasb
        mov    edx, edi
        repne scasb
        mov    esi, edi
        dec    esi
        lea    edi, [edi + 2]
        std
        mov    ecx, esi
        sub    ecx, edx
        rep movsb
        dec    edx
        mov    ecx, edx
        mov    esi, edx
        sub    ecx, dword ptr [ebp + 8]
        rep movsb
        cld
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  06Ah, 001h, 068h, 000h, 000h, 000h, 000h, 0CDh, 020h, 00Ch, 000h, 040h, 000h, 083h, 0C4h, 008h
        db  0C3h
sub_1_60AA:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_60BD:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
        db  06Ah, 000h, 0CDh, 020h, 059h, 000h, 001h, 000h, 083h, 0C4h, 004h, 092h, 0C1h, 0E0h, 00Ch, 0C3h
sub_1_60E0:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mul    edx
        test   edx, 0ffffh
        je     SHORT loc_1_60F9
        mov    eax, 0
loc_1_60F9:
        pop    edx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh
sub_1_6100:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6115:
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_6126
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_6126:
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_612D:
        add    esp, 8
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_1_613E
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_613E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + 14h]
        je     SHORT loc_1_615A
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_6157:
        add    esp, 4
loc_1_615A:
        cli
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Ch, 0B5h, 000h, 000h, 000h, 000h
        cmp    ecx, edx
        jne    SHORT loc_1_6176
        mov    eax, dword ptr [edx + 18h]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_6192
loc_1_6176:
        test   edx, edx
        je     SHORT loc_1_6192
loc_1_617A:
        mov    eax, dword ptr [edx + 18h]
        cmp    ecx, eax
        je     SHORT loc_1_6189
        mov    edx, eax
        test   edx, edx
        jne    SHORT loc_1_617A
        jmp    SHORT loc_1_6192
loc_1_6189:
        mov    eax, dword ptr [edx + 18h]
        mov    edi, dword ptr [eax + 18h]
        mov    dword ptr [edx + 18h], edi
loc_1_6192:
        db  066h, 0FFh, 00Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ecx + 5ah]
        db  0C6h, 080h, 000h, 000h, 000h, 000h, 0FFh
        sti
        cmp    byte ptr [ecx + 2], 2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        jne    SHORT loc_1_61C6
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_61B6:
        add    esp, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_61BE:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_61C6:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_61CB:
        add    esp, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_61D3:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
