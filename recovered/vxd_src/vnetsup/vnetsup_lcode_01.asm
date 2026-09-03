; vnetsup_lcode_01.asm -- VNETSUP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 046h, 030h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 06Bh, 000h
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        db  000h, 0C5h
        add    dword ptr [eax], eax
        add    byte ptr [17h], bh
        jae    SHORT loc_1_006C
        db  02Eh, 0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_006B:
        clc
loc_1_006C:
        ret
sub_1_006D:
        or     byte ptr [edx], al
        add    byte ptr [eax], al
        scasd
        add    byte ptr [eax], al
        db  000h, 0CDh
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        mov    bh, 4dh
        sbb    al, 81h
        stc
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        jae    SHORT loc_1_00A3
        sub    eax, eax
        push   eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        db  02Eh, 0FFh, 014h, 08Dh, 06Dh, 000h, 000h, 000h
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_00A3:
        mov    word ptr [ebp + 1ch], 1
        or     word ptr [ebp + 2ch], 1
        ret
loc_1_00AF:
        db  08Bh, 01Dh, 092h, 011h, 000h, 000h
        db  0FFh, 005h, 092h, 011h, 000h, 000h
        db  0FFh, 034h, 09Dh, 09Ah, 011h, 000h, 000h
        push   0
        db  0FFh, 034h, 09Dh, 096h, 011h, 000h, 000h
        jmp    SHORT loc_1_00E9
loc_1_00CD:
        db  08Bh, 01Dh, 002h, 012h, 000h, 000h
        db  0FFh, 005h, 002h, 012h, 000h, 000h
        db  0FFh, 034h, 09Dh, 00Ah, 012h, 000h, 000h
        push   1
        db  0FFh, 034h, 09Dh, 006h, 012h, 000h, 000h
loc_1_00E9:
        mov    edi, eax
        mov    esi, eax
        mov    ecx, 0ffffffffh
        sub    al, al
        repne scasb
        not    ecx
        cmp    ecx, 1
        jbe    SHORT loc_1_0125
        mov    edi, ecx
        push   ecx
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_0125
        push   eax
        push   edi
        mov    ecx, edi
        mov    edi, eax
        rep movsb
        pop    edi
        push   edi
        push   eax
        call   sub_1_02F8
        add    esp, 8
        db  0CDh, 020h, 026h, 000h, 040h, 000h
        pop    ecx
loc_1_0125:
        add    esp, 8
        pop    eax
        ret
loc_1_012A:
        call   sub_1_1038
        cmp    eax, 1
        jae    SHORT loc_1_0135
        ret
loc_1_0135:
        ja     SHORT loc_1_013D
        db  0FFh, 025h, 019h, 011h, 000h, 000h
loc_1_013D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0FFh, 015h, 019h, 011h, 000h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 1ah], 1
        jne    SHORT loc_1_0162
        mov    word ptr [eax + 1ah], 0
loc_1_0162:
        pop    ebp
        ret
sub_1_0164:
        push   esi
        push   edi
        push   ebx
        movzx  ecx, byte ptr [esp + 10h]
        cmp    ecx, 3
        jae    SHORT loc_1_0193
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        push   ebx
        push   eax
        db  02Eh, 0FFh, 014h, 08Dh, 06Dh, 000h, 000h, 000h
        add    esp, 8
        mov    ebx, dword ptr [esp + 18h]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    SHORT loc_1_0198
loc_1_0193:
        mov    eax, 1
loc_1_0198:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_019C:
        db  0A1h, 010h, 011h, 000h, 000h
        clc
        ret
loc_1_01A3:
        db  0B8h, 034h, 011h, 000h, 000h
        db  080h, 03Dh, 018h, 011h, 000h, 000h, 001h
        ret
loc_1_01B0:
        jmp    NEAR PTR sub_1_026C
loc_1_01B5:
        ret
        db  090h, 090h
loc_1_01B8:
        jmp    NEAR PTR loc_1_0344
        db  090h, 090h, 090h
loc_1_01C0:
        jmp    NEAR PTR loc_1_0B98
loc_1_01C5:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_01CF
        clc
        ret
loc_1_01CF:
        db  08Bh, 00Dh, 01Dh, 011h, 000h, 000h
        jecxz  loc_1_01E2
loc_1_01D7:
        push   ecx
        db  0FFh, 014h, 08Dh, 01Dh, 011h, 000h, 000h
        pop    ecx
        loop   loc_1_01D7
loc_1_01E2:
        clc
        ret
sub_1_01E4:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
        db  0CDh, 020h, 041h, 080h, 040h, 000h
sub_1_01F0:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
        db  0CDh, 020h, 043h, 080h, 040h, 000h
sub_1_01FC:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_0202:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_0208:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 020h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0228
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0ch]
loc_1_021D:
        cmp    dword ptr [esi + 10h], eax
        je     SHORT loc_1_0234
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_021D
loc_1_0228:
        pop    edi
        mov    eax, 2
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_0234:
        movzx  edx, word ptr [esi + 14h]
        add    edx, 8
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_0254
        cmp    dword ptr [eax], edx
        jge    SHORT loc_1_0254
        mov    dword ptr [eax], edx
        mov    eax, 0eah
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_0254:
        mov    ecx, edx
        add    esi, 4
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0A1h, 000h, 020h, 000h, 000h
        mov    edx, eax
        mov    edi, eax
        or     eax, eax
        mov    esi, dword ptr [ebp + 10h]
        je     SHORT loc_1_02AF
        mov    ecx, dword ptr [esi + 4]
loc_1_0284:
        cmp    dword ptr [edx + 10h], ecx
        je     SHORT loc_1_0294
        mov    edi, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_0284
        jmp    SHORT loc_1_02AF
        db  090h
loc_1_0294:
        cmp    eax, edx
        mov    eax, dword ptr [edx]
        jne    SHORT loc_1_02A4
        db  0A3h, 000h, 020h, 000h, 000h
        jmp    SHORT loc_1_02A6
        db  08Dh, 049h, 000h
loc_1_02A4:
        mov    dword ptr [edi], eax
loc_1_02A6:
        push   edx
        call   sub_1_0202
        add    esp, 4
loc_1_02AF:
        movzx  eax, word ptr [esi + 8]
        add    eax, 0ch
        push   eax
        call   sub_1_01FC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_02F1
        db  08Bh, 00Dh, 000h, 020h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 000h, 020h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], ecx
        movzx  edx, word ptr [esi + 8]
        lea    edi, [eax + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_1_02F1:
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_02F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [edi*2]
        push   eax
        call   sub_1_01FC
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_033E
        push   0
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   ebx
        call   sub_1_01E4
        add    esp, 10h
        push   1
        push   edi
        push   eax
        push   ebx
        push   esi
        call   sub_1_01F0
        add    esp, 14h
        push   ebx
        call   sub_1_0202
        add    esp, 4
loc_1_033E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_0344:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        push   15h
        lea    eax, [ebp - 18h]
        push   eax
        db  068h, 004h, 020h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        call   sub_1_0E58
        add    esp, 14h
        lea    eax, [esi + 18h]
        push   eax
        push   18h
        push   esi
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_0E58
        add    esp, 14h
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_0380:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 8], 67452301h
        mov    dword ptr [eax + 0ch], 0efcdab89h
        mov    dword ptr [eax + 10h], 98badcfeh
        mov    dword ptr [eax + 14h], 10325476h
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_03B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        mov    ecx, edx
        and    ecx, 1f8h
        shr    ecx, 3
        mov    eax, dword ptr [ebp + 10h]
        lea    eax, [edx + eax*8]
        cmp    eax, edx
        jae    SHORT loc_1_03DA
        inc    dword ptr [esi + 4]
loc_1_03DA:
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 10h]
        shr    eax, 1dh
        add    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 10h]
        dec    dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_0452
        mov    edi, dword ptr [ebp + 0ch]
loc_1_03F2:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + esi + 18h], al
        inc    edi
        inc    ecx
        cmp    ecx, 40h
        jne    SHORT loc_1_0448
        sub    edx, edx
        lea    ecx, [ebp - 40h]
loc_1_0404:
        movzx  eax, byte ptr [edx + esi + 1bh]
        shl    eax, 10h
        movzx  ebx, byte ptr [edx + esi + 19h]
        or     eax, ebx
        shl    eax, 8
        movzx  ebx, byte ptr [edx + esi + 1ah]
        shl    ebx, 10h
        or     eax, ebx
        movzx  ebx, byte ptr [edx + esi + 18h]
        or     eax, ebx
        mov    dword ptr [ecx], eax
        add    ecx, 4
        add    edx, 4
        lea    eax, [ebp]
        cmp    ecx, eax
        jb     SHORT loc_1_0404
        lea    eax, [ebp - 40h]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_0518
        add    esp, 8
        sub    ecx, ecx
loc_1_0448:
        mov    eax, dword ptr [ebp + 10h]
        dec    dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_1_03F2
loc_1_0452:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_0458:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi]
        and    eax, 1f8h
        shr    eax, 3
        cmp    eax, 38h
        jl     SHORT loc_1_0484
        mov    ecx, 78h
        jmp    SHORT loc_1_0489
loc_1_0484:
        mov    ecx, 38h
loc_1_0489:
        sub    ecx, eax
        push   ecx
        db  068h, 00Ch, 020h, 000h, 000h
        push   edi
        call   sub_1_03B4
        add    esp, 0ch
        sub    edx, edx
        inc    ecx
        lea    ecx, [ebp - 40h]
loc_1_04A0:
        movzx  esi, byte ptr [edx + edi + 1bh]
        shl    esi, 10h
        movzx  eax, byte ptr [edx + edi + 19h]
        or     esi, eax
        shl    esi, 8
        movzx  eax, byte ptr [edx + edi + 1ah]
        shl    eax, 10h
        or     esi, eax
        movzx  eax, byte ptr [edx + edi + 18h]
        or     esi, eax
        mov    dword ptr [ecx], esi
        add    ecx, 4
        add    edx, 4
        lea    eax, [ebp - 8]
        cmp    ecx, eax
        jb     SHORT loc_1_04A0
        lea    esi, [edi + 8]
        lea    eax, [ebp - 40h]
        push   eax
        push   esi
        call   sub_1_0518
        add    esp, 8
        sub    eax, eax
        mov    edx, 4
loc_1_04E9:
        mov    cl, byte ptr [esi]
        mov    byte ptr [eax + edi + 58h], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [eax + edi + 59h], cl
        mov    ecx, dword ptr [esi]
        shr    ecx, 10h
        mov    byte ptr [eax + edi + 5ah], cl
        mov    ecx, dword ptr [esi]
        shr    ecx, 18h
        mov    byte ptr [eax + edi + 5bh], cl
        add    esi, 4
        add    eax, 4
        dec    edx
        jne    SHORT loc_1_04E9
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_0518:
        push   ebp
        mov    ebp, esp
        sub    esp, 9ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 10h], eax
        mov    esi, eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    ecx, eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 9ch], edi
        and    dword ptr [ebp - 9ch], ebx
        or     eax, dword ptr [ebp - 9ch]
        add    eax, dword ptr [edx]
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 1dh
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 98h], ebx
        and    dword ptr [ebp - 98h], eax
        or     esi, dword ptr [ebp - 98h]
        add    esi, dword ptr [edx + 4]
        add    ecx, esi
        mov    esi, ecx
        shl    esi, 7
        shr    ecx, 19h
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 94h], esi
        and    dword ptr [ebp - 94h], eax
        or     ecx, dword ptr [ebp - 94h]
        add    ecx, dword ptr [edx + 8]
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 15h
        shl    edi, 0bh
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 90h], esi
        and    dword ptr [ebp - 90h], ecx
        or     edi, dword ptr [ebp - 90h]
        add    edi, dword ptr [edx + 0ch]
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 13h
        shr    ebx, 0dh
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 8ch], ecx
        and    dword ptr [ebp - 8ch], edi
        or     ebx, dword ptr [ebp - 8ch]
        add    ebx, dword ptr [edx + 10h]
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 1dh
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 88h], edi
        and    dword ptr [ebp - 88h], ebx
        or     eax, dword ptr [ebp - 88h]
        add    eax, dword ptr [edx + 14h]
        add    esi, eax
        mov    eax, esi
        shl    eax, 7
        shr    esi, 19h
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 84h], eax
        and    dword ptr [ebp - 84h], ebx
        or     esi, dword ptr [ebp - 84h]
        add    esi, dword ptr [edx + 18h]
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 15h
        shl    ecx, 0bh
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 80h], eax
        and    dword ptr [ebp - 80h], esi
        or     ecx, dword ptr [ebp - 80h]
        add    ecx, dword ptr [edx + 1ch]
        add    edi, ecx
        mov    ecx, edi
        shl    ecx, 13h
        shr    edi, 0dh
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 7ch], esi
        and    dword ptr [ebp - 7ch], ecx
        or     edi, dword ptr [ebp - 7ch]
        add    edi, dword ptr [edx + 20h]
        add    ebx, edi
        lea    edi, [ebx*8]
        shr    ebx, 1dh
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 78h], ecx
        and    dword ptr [ebp - 78h], edi
        or     ebx, dword ptr [ebp - 78h]
        add    ebx, dword ptr [edx + 24h]
        add    eax, ebx
        mov    ebx, eax
        shl    ebx, 7
        shr    eax, 19h
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 74h], ebx
        and    dword ptr [ebp - 74h], edi
        or     eax, dword ptr [ebp - 74h]
        add    eax, dword ptr [edx + 28h]
        add    esi, eax
        mov    eax, esi
        shr    eax, 15h
        shl    esi, 0bh
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 70h], ebx
        and    dword ptr [ebp - 70h], eax
        or     esi, dword ptr [ebp - 70h]
        add    esi, dword ptr [edx + 2ch]
        add    ecx, esi
        mov    esi, ecx
        shl    esi, 13h
        shr    ecx, 0dh
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 6ch], eax
        and    dword ptr [ebp - 6ch], esi
        or     ecx, dword ptr [ebp - 6ch]
        add    ecx, dword ptr [edx + 30h]
        add    edi, ecx
        lea    ecx, [edi*8]
        shr    edi, 1dh
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 68h], esi
        and    dword ptr [ebp - 68h], ecx
        or     edi, dword ptr [ebp - 68h]
        add    edi, dword ptr [edx + 34h]
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 7
        shr    ebx, 19h
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 64h], edi
        and    dword ptr [ebp - 64h], ecx
        or     ebx, dword ptr [ebp - 64h]
        add    ebx, dword ptr [edx + 38h]
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 15h
        shl    eax, 0bh
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 60h], edi
        and    dword ptr [ebp - 60h], ebx
        or     eax, dword ptr [ebp - 60h]
        add    eax, dword ptr [edx + 3ch]
        add    esi, eax
        mov    eax, esi
        shl    eax, 13h
        shr    esi, 0dh
        or     eax, esi
        mov    esi, ebx
        or     esi, eax
        and    esi, edi
        mov    dword ptr [ebp - 5ch], ebx
        and    dword ptr [ebp - 5ch], eax
        or     esi, dword ptr [ebp - 5ch]
        add    esi, dword ptr [edx]
        add    esi, 5a827999h
        add    ecx, esi
        lea    esi, [ecx*8]
        shr    ecx, 1dh
        or     esi, ecx
        mov    ecx, eax
        or     ecx, esi
        mov    dword ptr [ebp - 58h], ecx
        mov    ecx, eax
        and    ecx, esi
        mov    dword ptr [ebp - 54h], ecx
        mov    ecx, ebx
        and    ecx, dword ptr [ebp - 58h]
        or     ecx, dword ptr [ebp - 54h]
        add    ecx, dword ptr [edx + 10h]
        add    ecx, 5a827999h
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 1bh
        shl    edi, 5
        or     ecx, edi
        mov    edi, ecx
        and    edi, dword ptr [ebp - 58h]
        or     edi, dword ptr [ebp - 54h]
        add    edi, dword ptr [edx + 20h]
        add    edi, 5a827999h
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 17h
        shl    ebx, 9
        or     edi, ebx
        mov    ebx, edi
        or     ebx, esi
        and    ebx, ecx
        mov    dword ptr [ebp - 50h], edi
        and    dword ptr [ebp - 50h], esi
        or     ebx, dword ptr [ebp - 50h]
        add    ebx, dword ptr [edx + 30h]
        add    ebx, 5a827999h
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 13h
        shl    eax, 0dh
        or     ebx, eax
        mov    eax, edi
        or     eax, ebx
        and    eax, ecx
        mov    dword ptr [ebp - 4ch], edi
        and    dword ptr [ebp - 4ch], ebx
        or     eax, dword ptr [ebp - 4ch]
        add    eax, dword ptr [edx + 4]
        add    eax, 5a827999h
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 1dh
        or     eax, esi
        mov    esi, ebx
        or     esi, eax
        mov    dword ptr [ebp - 48h], esi
        mov    esi, ebx
        and    esi, eax
        mov    dword ptr [ebp - 44h], esi
        mov    esi, edi
        and    esi, dword ptr [ebp - 48h]
        or     esi, dword ptr [ebp - 44h]
        add    esi, dword ptr [edx + 14h]
        add    esi, 5a827999h
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 1bh
        shl    ecx, 5
        or     esi, ecx
        mov    ecx, esi
        and    ecx, dword ptr [ebp - 48h]
        or     ecx, dword ptr [ebp - 44h]
        add    ecx, dword ptr [edx + 24h]
        add    ecx, 5a827999h
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 17h
        shl    edi, 9
        or     ecx, edi
        mov    edi, ecx
        or     edi, eax
        and    edi, esi
        mov    dword ptr [ebp - 40h], ecx
        and    dword ptr [ebp - 40h], eax
        or     edi, dword ptr [ebp - 40h]
        add    edi, dword ptr [edx + 34h]
        add    edi, 5a827999h
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 13h
        shl    ebx, 0dh
        or     edi, ebx
        mov    ebx, ecx
        or     ebx, edi
        and    ebx, esi
        mov    dword ptr [ebp - 3ch], ecx
        and    dword ptr [ebp - 3ch], edi
        or     ebx, dword ptr [ebp - 3ch]
        add    ebx, dword ptr [edx + 8]
        add    ebx, 5a827999h
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 1dh
        or     ebx, eax
        mov    eax, edi
        or     eax, ebx
        mov    dword ptr [ebp - 38h], eax
        mov    eax, edi
        and    eax, ebx
        mov    dword ptr [ebp - 34h], eax
        mov    eax, ecx
        and    eax, dword ptr [ebp - 38h]
        or     eax, dword ptr [ebp - 34h]
        add    eax, dword ptr [edx + 18h]
        add    eax, 5a827999h
        add    esi, eax
        mov    eax, esi
        shr    eax, 1bh
        shl    esi, 5
        or     eax, esi
        mov    esi, eax
        and    esi, dword ptr [ebp - 38h]
        or     esi, dword ptr [ebp - 34h]
        add    esi, dword ptr [edx + 28h]
        add    esi, 5a827999h
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 17h
        shl    ecx, 9
        or     esi, ecx
        mov    ecx, esi
        or     ecx, ebx
        and    ecx, eax
        mov    dword ptr [ebp - 30h], esi
        and    dword ptr [ebp - 30h], ebx
        or     ecx, dword ptr [ebp - 30h]
        add    ecx, dword ptr [edx + 38h]
        add    ecx, 5a827999h
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 13h
        shl    edi, 0dh
        or     ecx, edi
        mov    edi, esi
        or     edi, ecx
        and    edi, eax
        mov    dword ptr [ebp - 2ch], esi
        and    dword ptr [ebp - 2ch], ecx
        or     edi, dword ptr [ebp - 2ch]
        add    edi, dword ptr [edx + 0ch]
        add    edi, 5a827999h
        add    ebx, edi
        lea    edi, [ebx*8]
        shr    ebx, 1dh
        or     edi, ebx
        mov    ebx, ecx
        or     ebx, edi
        mov    dword ptr [ebp - 28h], ebx
        mov    ebx, ecx
        and    ebx, edi
        mov    dword ptr [ebp - 24h], ebx
        mov    ebx, esi
        and    ebx, dword ptr [ebp - 28h]
        or     ebx, dword ptr [ebp - 24h]
        add    ebx, dword ptr [edx + 1ch]
        add    ebx, 5a827999h
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 1bh
        shl    eax, 5
        or     ebx, eax
        mov    eax, ebx
        and    eax, dword ptr [ebp - 28h]
        or     eax, dword ptr [ebp - 24h]
        add    eax, dword ptr [edx + 2ch]
        add    eax, 5a827999h
        add    esi, eax
        mov    eax, esi
        shr    eax, 17h
        shl    esi, 9
        or     eax, esi
        mov    esi, eax
        or     esi, edi
        and    esi, ebx
        mov    dword ptr [ebp - 20h], eax
        and    dword ptr [ebp - 20h], edi
        or     esi, dword ptr [ebp - 20h]
        add    esi, dword ptr [edx + 3ch]
        add    esi, 5a827999h
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 13h
        shl    ecx, 0dh
        or     esi, ecx
        mov    ecx, ebx
        xor    ecx, eax
        xor    ecx, esi
        add    ecx, dword ptr [edx]
        add    ecx, 6ed9eba1h
        add    edi, ecx
        lea    ecx, [edi*8]
        shr    edi, 1dh
        or     ecx, edi
        mov    edi, eax
        xor    edi, esi
        xor    edi, ecx
        add    edi, dword ptr [edx + 20h]
        add    edi, 6ed9eba1h
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 17h
        shl    ebx, 9
        or     edi, ebx
        mov    ebx, edi
        xor    ebx, esi
        xor    ebx, ecx
        add    ebx, dword ptr [edx + 10h]
        add    ebx, 6ed9eba1h
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 15h
        shl    eax, 0bh
        or     ebx, eax
        mov    eax, edi
        xor    eax, ebx
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 1ch]
        add    eax, dword ptr [edx + 30h]
        add    eax, 6ed9eba1h
        add    esi, eax
        mov    eax, esi
        shl    eax, 0fh
        shr    esi, 11h
        or     eax, esi
        mov    esi, eax
        xor    esi, dword ptr [ebp - 1ch]
        add    esi, dword ptr [edx + 8]
        add    esi, 6ed9eba1h
        add    ecx, esi
        lea    esi, [ecx*8]
        shr    ecx, 1dh
        or     esi, ecx
        mov    ecx, ebx
        xor    ecx, eax
        xor    ecx, esi
        add    ecx, dword ptr [edx + 28h]
        add    ecx, 6ed9eba1h
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 17h
        shl    edi, 9
        or     ecx, edi
        mov    edi, ecx
        xor    edi, eax
        xor    edi, esi
        add    edi, dword ptr [edx + 18h]
        add    edi, 6ed9eba1h
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 15h
        shl    ebx, 0bh
        or     edi, ebx
        mov    ebx, ecx
        xor    ebx, edi
        mov    dword ptr [ebp - 18h], ebx
        mov    ebx, esi
        xor    ebx, dword ptr [ebp - 18h]
        add    ebx, dword ptr [edx + 38h]
        add    ebx, 6ed9eba1h
        add    eax, ebx
        mov    ebx, eax
        shl    ebx, 0fh
        shr    eax, 11h
        or     ebx, eax
        mov    eax, ebx
        xor    eax, dword ptr [ebp - 18h]
        add    eax, dword ptr [edx + 4]
        add    eax, 6ed9eba1h
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 1dh
        or     eax, esi
        mov    esi, edi
        xor    esi, ebx
        xor    esi, eax
        add    esi, dword ptr [edx + 24h]
        add    esi, 6ed9eba1h
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 17h
        shl    ecx, 9
        or     esi, ecx
        mov    ecx, esi
        xor    ecx, ebx
        xor    ecx, eax
        add    ecx, dword ptr [edx + 14h]
        add    ecx, 6ed9eba1h
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 15h
        shl    edi, 0bh
        or     ecx, edi
        mov    edi, esi
        xor    edi, ecx
        mov    dword ptr [ebp - 14h], edi
        mov    edi, eax
        xor    edi, dword ptr [ebp - 14h]
        add    edi, dword ptr [edx + 34h]
        add    edi, 6ed9eba1h
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 0fh
        shr    ebx, 11h
        or     edi, ebx
        mov    ebx, edi
        xor    ebx, dword ptr [ebp - 14h]
        add    ebx, dword ptr [edx + 0ch]
        add    ebx, 6ed9eba1h
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 1dh
        or     ebx, eax
        mov    eax, ecx
        xor    eax, edi
        xor    eax, ebx
        add    eax, dword ptr [edx + 2ch]
        add    eax, 6ed9eba1h
        add    esi, eax
        mov    eax, esi
        shr    eax, 17h
        shl    esi, 9
        or     eax, esi
        mov    esi, eax
        xor    esi, edi
        xor    esi, ebx
        add    esi, dword ptr [edx + 1ch]
        add    esi, 6ed9eba1h
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 15h
        shl    ecx, 0bh
        or     esi, ecx
        mov    ecx, eax
        xor    ecx, esi
        xor    ecx, ebx
        add    ecx, dword ptr [edx + 3ch]
        add    ecx, 6ed9eba1h
        add    edi, ecx
        mov    edx, edi
        shl    edx, 0fh
        shr    edi, 11h
        or     edx, edi
        add    ebx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], ebx
        add    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4], edx
        add    esi, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], esi
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_0B98:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   esi
        push   edi
        lea    edi, [ebp - 18h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        stosb
        lea    eax, [ebp - 80h]
        push   eax
        call   sub_1_0380
        add    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 80h]
        push   eax
        call   sub_1_03B4
        add    esp, 0ch
        lea    eax, [ebp - 80h]
        push   eax
        call   sub_1_0458
        add    esp, 4
        lea    esi, [ebp - 28h]
        lea    edi, [ebp - 18h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [eax + 18h]
        push   ecx
        push   18h
        push   eax
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_0E58
        add    esp, 14h
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_0C00:
        pop    es
        daa
        comiss xmm2, dword ptr [edi]
        aaa
        pop    ds
        aas
        push   es
        push   cs
        push   ss
        push   ds
        add    eax, 152d0d25h
        xor    eax, 24043d1dh
        or     al, 2ch
        adc    al, 34h
        sbb    al, 3ch
        add    esp, dword ptr [ebx]
        or     ebp, dword ptr [ebx]
        adc    esi, dword ptr [ebx]
        sbb    edi, dword ptr [ebx]
        add    ah, byte ptr [edx]
        or     ch, byte ptr [edx]
        adc    dh, byte ptr [edx]
        sbb    bh, byte ptr [edx]
        add    dword ptr [ecx], esp
        or     dword ptr [ecx], ebp
        adc    dword ptr [ecx], esi
        sbb    dword ptr [ecx], edi
        add    byte ptr [eax], ah
        or     byte ptr [eax], ch
        adc    byte ptr [eax], dh
        sbb    byte ptr [eax], bh
loc_1_0C40:
        cmp    dword ptr [ecx], esi
        sub    dword ptr [ecx], esp
        sbb    dword ptr [ecx], edx
        or     dword ptr [ecx], eax
        cmp    esi, dword ptr [ebx]
        sub    esp, dword ptr [ebx]
        sbb    edx, dword ptr [ebx]
        or     eax, dword ptr [ebx]
        cmp    eax, 1d252d35h
        adc    eax, 373f050dh
        das
        daa
        pop    ds
        pop    ss
        sysret
        cmp    byte ptr [eax], dh
        sub    byte ptr [eax], ah
        sbb    byte ptr [eax], dl
        or     byte ptr [eax], al
        cmp    dh, byte ptr [edx]
        sub    ah, byte ptr [edx]
        sbb    dl, byte ptr [edx]
        or     al, byte ptr [edx]
        cmp    al, 34h
        sub    al, 24h
        sbb    al, 14h
        or     al, 4
        push   ds
        push   ss
        push   cs
        push   es
loc_1_0C80:
        cmp    byte ptr [eax], dh
        sub    byte ptr [eax], ah
        sbb    byte ptr [eax], dl
        or     byte ptr [eax], al
        cmp    dword ptr [ecx], esi
        sub    dword ptr [ecx], esp
        sbb    dword ptr [ecx], edx
        or     dword ptr [ecx], eax
        cmp    dh, byte ptr [edx]
        sub    ah, byte ptr [edx]
        sbb    dl, byte ptr [edx]
        or     al, byte ptr [edx]
        cmp    esi, dword ptr [ebx]
        sub    esp, dword ptr [ebx]
loc_1_0C9C:
        push   ds
        push   ss
        push   cs
        push   es
        cmp    eax, 1d252d35h
        adc    eax, 343c050dh
        sub    al, 24h
        sbb    al, 14h
        or     al, 4
        sbb    edx, dword ptr [ebx]
        or     eax, dword ptr [ebx]
loc_1_0CB8:
        add    dword ptr [edx], eax
        add    al, 6
        or     byte ptr [edx], cl
        or     al, 0eh
        movups xmmword ptr [ebx], xmm2
        adc    eax, 1c1b1917h
        sbb    al, 1bh
        sbb    dword ptr [edi], edx
        adc    eax, 0e0f1113h
        or     al, 0ah
        or     byte ptr [esi], al
        add    al, 2
        add    dword ptr [170a10h], ecx
        add    al, 2
        sbb    ecx, dword ptr [esi]
        add    eax, 12160914h
        or     eax, dword ptr [ebx]
        sbb    dword ptr [edi], eax
        clts
        sbb    dl, byte ptr [ebx]
        or     al, 1
        or     al, 17h
        add    cl, byte ptr [eax]
        adc    bl, byte ptr [edx]
        add    dword ptr [ebx], ecx
        push   ss
        adc    byte ptr [ebx + edx], al
        unpcklps xmm1, xmmword ptr [edx]
        sbb    eax, dword ptr [150d1118h]
        pop    es
        add    byte ptr [ebx], al
loc_1_0D08:
        pop    ds
        add    byte ptr [ecx], al
        add    al, byte ptr [ebx]
        add    al, 3
        add    al, 5
        push   es
        pop    es
        or     byte ptr [edi], al
        or     byte ptr [ecx], cl
loc_1_0D17:
        or     cl, byte ptr [ebx]
        or     al, 0bh
        or     al, 0dh
        push   cs
        movups xmm1, xmmword ptr [edi]
        adc    byte ptr [ecx], dl
        adc    dl, byte ptr [ebx]
        adc    al, 13h
        adc    al, 15h
        push   ss
        pop    ss
        sbb    byte ptr [edi], dl
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1bh
        sbb    al, 1dh
        push   ds
        pop    ds
        db  000h
loc_1_0D38:
        push   cs
        hlt
        db  07Dh, 041h
        db  0E2h, 02Fh
        fistp  dword ptr [eax]
        mov    dword ptr [95bcc66ah], eax
        pop    ecx
        xor    byte ptr [edi + 288ec1f4h], al
        dec    ebp
        xchg   esi, eax
        adc    bh, byte ptr [ebx + 5fh]
        mov    esp, 0aa3e739h
        sar    byte ptr gs:[edi], 1
        ror    dword ptr [eax + 7eh], 1
        imul   byte ptr [ebx]
        and    esp, 0ffffffc9h
        pop    es
        adc    ch, byte ptr [ebp + 5ab5906ch]
        ror    byte ptr [esi - 0bc5e459h], 1
        dec    ebp
        and    dword ptr [ebp + 9c67c68h], esi
        push   ebx
        loop   loc_1_0D17
        fidiv  dword ptr [eax + 9]
        sahf
        inc    ebx
        outsd
        movsd
        and    dword ptr [ebp - 4b3418a4h], ecx
        sbb    byte ptr [6809d4a6h], bl
        lahf
        xor    dword ptr [eax + 4bh], 0fffffff1h
        loop   loc_1_0DD0
        mov    ch, 5ah
        db  02Eh, 0C7h, 0D7h, 08Dh, 0BEh, 053h, 060h, 0F6h, 009h, 03Ah
loc_1_0DA0:
        inc    ecx
        db  072h, 028h
        lds    ebx, ptr [ebx]
        lodsb
        in     al, 9fh
        cmp    dh, dh
        db  009h
loc_1_0DAB:
        pushad
        lodsb
        sbb    edx, edi
        lea    ebx, [edi - 3a41acbfh]
        jb     SHORT loc_1_0DDF
        in     al, 0e2h
        mov    esp, 7a47c124h
        fist   dword ptr [esi]
        pop    eax
        add    eax, 903daff3h
        mov    gs, word ptr [ecx - 4ch]
        add    cl, 7bh
        sbb    ch, ch
        daa
        db  0D8h
loc_1_0DD0:
        outsd
        stc
        or     al, 95h
        cmpsb
        inc    ebx
        push   eax
        lodsb
        int1
        dec    edx
        das
        jns    SHORT loc_1_0DA0
        xchg   esi, eax
loc_1_0DDF:
        pop    eax
        pushad
        sbb    eax, 0b70ee4d3h
        xor    eax, 2f3e498bh
        lds    edx, ptr [edx - 485c03a8h]
        db  0E0h, 014h
        db  07Ah, 061h
        or     eax, 0bd4d68bh
        mov    dl, 7eh
        dec    edi
        nop
        sbb    byte ptr [ebp - 38a6c31dh], ch
        and    eax, 616186fah
        mov    ah, 0dbh
        lea    ebx, [ebx + eax*2]
        cmpsd
        jle    SHORT loc_1_0DAB
        pop    edi
        push   es
        clc
        loopne loc_1_0E3B
        cmp    edx, eax
        sbb    eax, 0a684d8f2h
        aas
        jnp    SHORT loc_1_0E61
        retf   6359h
        db  0BEh, 005h, 0E0h, 09Ch, 027h, 027h, 01Bh, 0E4h, 071h, 049h, 0ACh, 08Eh, 0D2h, 0F0h, 0C6h, 09Ah
        db  00Dh, 03Fh, 053h, 065h, 0B8h
loc_1_0E38:
        or     byte ptr [eax], dl
        push   ss
loc_1_0E3B:
        push   ds
        or     al, 1bh
        add    dword ptr [ecx], edx
        pop    ss
        nop    dword ptr [91319h]
        pop    es
        or     eax, 1c030218h
        or     dl, byte ptr [edx]
        pop    ds
        or     edx, dword ptr [0e1a0406h]
        db  014h
sub_1_0E58:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 10h]
loc_1_0E61:
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 8]
        call   sub_1_0E8A
        add    dword ptr [ebp + 8], 7
        mov    ecx, dword ptr [ebp + 14h]
        sub    ecx, 8
        mov    dword ptr [ebp + 14h], ecx
        cmp    ecx, 8
        jae    SHORT loc_1_0E61
        xor    eax, eax
        rep stosb
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_1_0E8A:
        push   ebp
        mov    ebp, ebx
        and    ebp, 0fffffffch
        push   edi
        push   eax
        lea    edi, [ebp + 4ch]
        call   sub_1_0FF0
        db  0BEh, 040h, 00Ch, 000h, 000h
        lea    ebx, [ebp + 4ch]
        lea    edi, [ebp + 8ch]
        mov    ecx, 40h
loc_1_0EAD:
        lodsb
        xlatb
        stosb
        loop   loc_1_0EAD
        pop    esi
        lea    edi, [ebp + 4ch]
        call   sub_1_1006
        db  0C7h, 045h, 004h, 0B8h, 00Ch, 000h, 000h
        mov    byte ptr [ebp + 3], 10h
loc_1_0EC6:
        lea    esi, [ebp + 0ach]
        lea    edi, [ebp + 0ch]
        mov    ecx, 8
        rep movsd
        xor    esi, esi
        mov    ebx, esi
        mov    edx, 3
        mov    dword ptr [ebp + 8], ebx
loc_1_0EE2:
        mov    ecx, 6
        xor    eax, eax
        cmp    esi, 18h
        jge    SHORT loc_1_0F25
loc_1_0EEE:
        mov    edi, dword ptr [ebp + 4]
        mov    bl, byte ptr [edi]
        db  002h, 09Eh, 0D8h, 00Ch, 000h, 000h
        cmp    bl, 1ch
        jl     SHORT loc_1_0F01
        sub    bl, 1ch
loc_1_0F01:
        db  08Ah, 09Bh, 080h, 00Ch, 000h, 000h
        lea    edi, [ebp + 4ch]
        mov    al, byte ptr [ebx + edi]
        db  08Ah, 09Eh, 008h, 00Dh, 000h, 000h
        lea    edi, [ebp + 0ach]
        xor    al, byte ptr [edi + ebx]
        shr    al, 1
        rcl    ah, 1
        inc    esi
        loop   loc_1_0EEE
        jmp    SHORT loc_1_0F5A
loc_1_0F25:
        mov    edi, dword ptr [ebp + 4]
        mov    bl, byte ptr [edi]
        db  002h, 09Eh, 0D8h, 00Ch, 000h, 000h
        cmp    bl, 1ch
        jl     SHORT loc_1_0F38
        sub    bl, 1ch
loc_1_0F38:
        db  08Ah, 09Bh, 09Ch, 00Ch, 000h, 000h
        lea    edi, [ebp + 4ch]
        mov    al, byte ptr [ebx + edi]
        db  08Ah, 09Eh, 008h, 00Dh, 000h, 000h
        lea    edi, [ebp + 0ach]
        xor    al, byte ptr [edi + ebx]
        shr    al, 1
        rcl    ah, 1
        inc    esi
        loop   loc_1_0F25
loc_1_0F5A:
        mov    ecx, 4
        mov    bl, ah
        add    ebx, dword ptr [ebp + 8]
        shr    ebx, 1
        db  08Ah, 083h, 038h, 00Dh, 000h, 000h
        jae    SHORT loc_1_0F70
        shr    al, cl
loc_1_0F70:
        mov    edi, edx
loc_1_0F72:
        xor    ebx, ebx
        db  08Ah, 09Fh, 038h, 00Eh, 000h, 000h
        mov    ah, byte ptr [ebp + ebx + 8ch]
        xor    ah, al
        mov    byte ptr [ebp + ebx + 2ch], ah
        shr    al, 1
        dec    edi
        loop   loc_1_0F72
        xor    ebx, ebx
        add    dword ptr [ebp + 8], 40h
        add    edx, 4
        cmp    esi, 30h
        jne    NEAR PTR loc_1_0EE2
        lea    esi, [ebp + 2ch]
        lea    edi, [ebp + 0ach]
        mov    ecx, 8
        rep movsd
        lea    esi, [ebp + 0ch]
        lea    edi, [ebp + 8ch]
        mov    ecx, 8
        rep movsd
        inc    dword ptr [ebp + 4]
        dec    byte ptr [ebp + 3]
        jne    NEAR PTR loc_1_0EC6
        db  0BEh, 000h, 00Ch, 000h, 000h
        lea    ebx, [ebp + 8ch]
        pop    edi
        mov    edx, 8
loc_1_0FDB:
        mov    ecx, 8
loc_1_0FE0:
        lodsb
        xlatb
        shr    al, 1
        adc    ah, ah
        loop   loc_1_0FE0
        mov    byte ptr [edi], ah
        inc    edi
        dec    edx
        jne    SHORT loc_1_0FDB
        pop    ebp
        ret
sub_1_0FF0:
        mov    edx, 8
loc_1_0FF5:
        mov    ecx, 8
        lodsb
loc_1_0FFB:
        add    eax, eax
        mov    byte ptr [edi], ah
        inc    edi
        loop   loc_1_0FFB
        dec    edx
        jne    SHORT loc_1_0FF5
        ret
sub_1_1006:
        mov    edx, 7
        mov    ebx, 2
loc_1_1010:
        mov    ecx, 9
        lodsb
loc_1_1016:
        cmp    ecx, ebx
        jne    SHORT loc_1_1029
        mov    byte ptr [edi], 1
        cmp    ebx, 9
        jne    SHORT loc_1_102D
        mov    ebx, 2
        jmp    SHORT loc_1_102D
loc_1_1029:
        shl    al, 1
        rcl    byte ptr [edi]
loc_1_102D:
        inc    edi
        loop   loc_1_1016
        inc    ebx
        dec    edx
        jne    SHORT loc_1_1010
        ret
        db  000h, 000h, 000h
sub_1_1038:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        movzx  eax, word ptr [esi + 1ch]
        cmp    eax, 5f8ah
        je     SHORT loc_1_1058
        mov    eax, 1
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_1058:
        cmp    dword ptr [ebp + 18h], 0
        jne    SHORT loc_1_1070
        mov    edi, dword ptr [ebp + 0ch]
        mov    word ptr [edi + 1ah], 32h
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_1070:
        mov    edx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0164
        add    esp, 10h
        mov    word ptr [edi + 1ah], ax
        or     ax, ax
        jne    SHORT loc_1_10A6
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 1ch], ax
        mov    word ptr [edi + 1ah], ax
loc_1_10A6:
        cmp    dword ptr [ebp - 8], 0eah
        jne    SHORT loc_1_10B5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
loc_1_10B5:
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_10BC:
        pushfd
        add    dword ptr [eax], eax
        db  000h, 0A3h, 001h, 000h, 000h, 0B3h
        xor    byte ptr [eax], al
        db  000h, 0B0h, 001h, 000h, 000h, 0B5h
        add    dword ptr [eax], eax
        db  000h, 0B8h, 001h, 000h, 000h, 0C0h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    eax, dword ptr [eax + 304h]
        add    byte ptr [esi + 4eh], dl
        db  065h, 074h, 053h
        jne    SHORT loc_1_115B
        and    byte ptr [eax], al
        add    byte ptr [ecx], 0a0h
        pop    esp
        add    byte ptr [eax], al
        db  000h, 079h, 000h
        add    byte ptr [eax], al
        jns    SHORT loc_1_10FA
loc_1_10FA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    esp, 7000010h
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        db  000h
loc_1_1118:
        add    edi, edi
        db  0FFh, 0FFh, 0FFh
sub_1_111D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1121:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_115B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1179:
        add    byte ptr [eax], al
loc_1_117B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_1196:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_119A:
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        xor    al, 0
        add    byte ptr [eax], al
        xor    eax, 36000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        cmp    eax, 3e000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [ebp], al
        add    byte ptr [eax], al
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        dec    edx
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_1206:
        add    byte ptr [eax + eax], 0
loc_1_120A:
        add    dword ptr [eax + eax], 48200h
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
