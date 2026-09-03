; declicl_pcode_02.asm -- DECLICL object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DECLICL_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        cmp    eax, 16h
        je     SHORT loc_2_0023
        cmp    eax, 1ch
        je     SHORT loc_2_0023
        cmp    eax, 14h
        je     SHORT loc_2_0036
        clc
        ret
loc_2_0023:
        db  03Bh, 01Dh, 000h, 036h, 002h, 000h
        jne    SHORT loc_2_0034
        push   ebx
        call   sub_2_01B0
        add    esp, 4
loc_2_0034:
        clc
        ret
loc_2_0036:
        call   sub_2_01C0
        clc
        ret
        db  000h, 000h, 000h
sub_2_0040:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        cld
        cmp    ecx, 8
        jb     SHORT loc_2_007F
        mov    eax, esi
        and    eax, 3
        db  0FFh, 024h, 085h, 091h, 000h, 002h, 000h
loc_2_0060:
        movsb
loc_2_0061:
        movsb
loc_2_0062:
        movsb
loc_2_0063:
        sub    ecx, eax
        xor    eax, eax
        shr    ecx, 1
        rcl    eax, 1
        shr    ecx, 1
        rcl    eax, 1
        rep movsd
        db  0FFh, 024h, 085h, 0A1h, 000h, 002h, 000h
loc_2_0078:
        movsb
loc_2_0079:
        movsb
loc_2_007A:
        movsb
loc_2_007B:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_007F:
        db  0FFh, 024h, 08Dh, 0B1h, 000h, 002h, 000h
loc_2_0086:
        movsb
loc_2_0087:
        movsb
loc_2_0088:
        movsb
loc_2_0089:
        movsb
loc_2_008A:
        movsb
loc_2_008B:
        movsb
loc_2_008C:
        movsb
loc_2_008D:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  063h, 000h, 002h, 000h, 062h, 000h, 002h, 000h, 061h, 000h, 002h, 000h, 060h, 000h, 002h, 000h
        db  07Bh, 000h, 002h, 000h, 079h, 000h, 002h, 000h, 07Ah, 000h, 002h, 000h, 078h, 000h, 002h, 000h
        db  08Dh, 000h, 002h, 000h, 08Ch, 000h, 002h, 000h, 08Bh, 000h, 002h, 000h, 08Ah, 000h, 002h, 000h
        db  089h, 000h, 002h, 000h, 088h, 000h, 002h, 000h, 087h, 000h, 002h, 000h, 086h, 000h, 002h, 000h
sub_2_00D1:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ah, cl
        mov    al, cl
        shl    eax, 10h
        mov    al, cl
        mov    ah, cl
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        cld
        cmp    ecx, 8
        jb     SHORT loc_2_011B
        mov    edx, edi
        and    edx, 3
        db  0FFh, 024h, 095h, 02Dh, 001h, 002h, 000h
loc_2_00FC:
        stosb
loc_2_00FD:
        stosb
loc_2_00FE:
        stosb
loc_2_00FF:
        sub    ecx, edx
        xor    edx, edx
        shr    ecx, 1
        rcl    edx, 1
        shr    ecx, 1
        rcl    edx, 1
        rep stosd
        db  0FFh, 024h, 095h, 03Dh, 001h, 002h, 000h
loc_2_0114:
        stosb
loc_2_0115:
        stosb
loc_2_0116:
        stosb
loc_2_0117:
        pop    edi
        pop    edx
        pop    ebp
        ret
loc_2_011B:
        db  0FFh, 024h, 08Dh, 04Dh, 001h, 002h, 000h
loc_2_0122:
        stosb
loc_2_0123:
        stosb
loc_2_0124:
        stosb
loc_2_0125:
        stosb
loc_2_0126:
        stosb
loc_2_0127:
        stosb
loc_2_0128:
        stosb
loc_2_0129:
        pop    edi
        pop    edx
        pop    ebp
        ret
        db  0FFh, 000h, 002h, 000h, 0FEh, 000h, 002h, 000h, 0FDh, 000h, 002h, 000h, 0FCh, 000h, 002h, 000h
        db  017h, 001h, 002h, 000h, 015h, 001h, 002h, 000h, 016h, 001h, 002h, 000h, 014h, 001h, 002h, 000h
        db  029h, 001h, 002h, 000h, 028h, 001h, 002h, 000h, 027h, 001h, 002h, 000h, 026h, 001h, 002h, 000h
        db  025h, 001h, 002h, 000h, 024h, 001h, 002h, 000h, 023h, 001h, 002h, 000h, 022h, 001h, 002h, 000h
sub_2_016D:
        enter  0, 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        cld
        repe cmpsb
        sete   al
        movzx  eax, al
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0190:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 0F4h, 004h, 004h, 000h
        call   sub_2_0EC0
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
sub_2_01B0:
        call   sub_2_0510
        xor    eax, eax
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_01C0:
        db  068h, 000h, 000h, 004h, 000h
        push   300h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_01E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 1
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [eax + 5], 1
        mov    byte ptr [eax + 6], dl
        mov    dl, byte ptr [ebp + 0ch]
        mov    byte ptr [eax + 7], dl
        mov    dl, byte ptr [ebp + 1ch]
        mov    byte ptr [eax + 8], dl
        db  08Ah, 015h, 018h, 01Fh, 004h, 000h
        mov    byte ptr [eax + 9], dl
        mov    dl, byte ptr [ebp + 14h]
        mov    byte ptr [eax + 0ah], dl
        mov    dl, byte ptr [ebp + 10h]
        mov    byte ptr [eax + 0bh], dl
        pop    ebp
        ret
        db  08Dh, 040h, 000h
sub_2_0220:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_2_0245
        db  08Bh, 015h, 06Ch, 01Eh, 004h, 000h
        push   edx
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        add    eax, 0ch
        push   eax
        call   sub_2_0040
        add    esp, 0ch
loc_2_0245:
        mov    ebx, dword ptr [ebp + 1ch]
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [ebp + 18h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        push   0
        xor    eax, eax
        push   0
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        push   1
        call   sub_2_01E0
        add    esp, 1ch
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 044h, 020h, 000h
sub_2_0270:
        push   ebp
        mov    ebp, esp
        mov    dh, byte ptr [ebp + 8]
        db  03Ah, 035h, 018h, 01Fh, 004h, 000h
        jae    SHORT loc_2_029B
        db  0A1h, 02Ch, 01Fh, 004h, 000h
        xor    dl, dl
        test   dh, dh
        ja     SHORT loc_2_0297
        pop    ebp
        ret
loc_2_028B:
        mov    eax, dword ptr [eax]
        test   eax, eax
        je     SHORT loc_2_029B
        inc    dl
        cmp    dl, dh
        jae    SHORT loc_2_029B
loc_2_0297:
        test   eax, eax
        jne    SHORT loc_2_028B
loc_2_029B:
        pop    ebp
        ret
        db  08Dh, 040h, 000h
sub_2_02A0:
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        call   sub_2_0270
        add    esp, 4
        mov    edx, dword ptr [ebx + 4]
        push   edx
        xor    edx, edx
        mov    dl, byte ptr [esi + 8]
        push   edx
        xor    edx, edx
        db  08Ah, 015h, 069h, 01Eh, 004h, 000h
        push   edx
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 7]
        push   eax
        call   sub_2_0220
        add    esp, 14h
        push   0a6h
        push   ebx
        call   sub_2_05C0
        add    esp, 8
        pop    ebp
        pop    esi
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_02F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    dl, byte ptr [eax + 4]
        cmp    dl, 1
        jne    SHORT loc_2_031B
        cmp    dl, byte ptr [eax + 5]
        jne    SHORT loc_2_031B
        cmp    byte ptr [eax + 6], 0
        jne    SHORT loc_2_031B
        push   eax
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_2_02A0
        add    esp, 4
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
loc_2_031B:
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0330:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    edx, esi
        xor    ebx, ebx
        shr    edx, 2
        and    esi, 3
loc_2_0346:
        mov    ecx, edx
        dec    edx
        test   ecx, ecx
        je     SHORT loc_2_0358
        mov    ecx, eax
        mov    edi, dword ptr [ecx]
        add    eax, 4
        add    ebx, edi
        jmp    SHORT loc_2_0346
loc_2_0358:
        cmp    esi, 2
        jb     SHORT loc_2_0366
        jbe    SHORT loc_2_0374
        cmp    esi, 3
        je     SHORT loc_2_037D
        jmp    SHORT loc_2_0386
loc_2_0366:
        cmp    esi, 1
        jne    SHORT loc_2_0386
        mov    eax, dword ptr [eax]
        and    eax, 0ffh
        jmp    SHORT loc_2_0384
loc_2_0374:
        mov    eax, dword ptr [eax]
        and    eax, 0ffffh
        jmp    SHORT loc_2_0384
loc_2_037D:
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffh
loc_2_0384:
        add    ebx, eax
loc_2_0386:
        mov    eax, ebx
        neg    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_2_0390:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edi, dword ptr [ebp + 20h]
        xor    eax, eax
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    ebx, dword ptr [ebp + 14h]
        push   ebx
        xor    esi, esi
        call   sub_2_0040
        add    esp, 0ch
        mov    ecx, ebx
        jmp    SHORT loc_2_03C0
loc_2_03B5:
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        xor    eax, edi
        inc    esi
        mov    dword ptr [ecx - 4], eax
loc_2_03C0:
        xor    ebx, ebx
        mov    bl, byte ptr [ebp + 1ch]
        mov    edx, ebx
        mov    eax, ebx
        sar    edx, 1fh
        shl    edx, 2
        sbb    eax, edx
        sar    eax, 2
        cmp    esi, eax
        jl     SHORT loc_2_03B5
        mov    edx, dword ptr [ebp + 14h]
        mov    al, byte ptr [ebp + 1ch]
        add    ebx, edx
        add    al, 4
        mov    dword ptr [ebx], edi
        mov    byte ptr [edx + 36h], al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 044h, 020h, 000h
sub_2_03F0:
        push   ebx
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edi, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 1ch]
        add    edx, 4
        sub    eax, 4
        je     SHORT loc_2_0440
loc_2_0409:
        xor    bl, bl
        mov    bh, byte ptr [ebp + 14h]
        mov    byte ptr [ebp - 4], bl
        test   bh, bh
        ja     SHORT loc_2_0438
        jmp    SHORT loc_2_043C
loc_2_0417:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    ebx, edx
        mov    cl, byte ptr [ecx + edi]
        mov    ch, byte ptr [ebx]
        xor    ch, cl
        dec    eax
        mov    byte ptr [ebx], ch
        mov    bl, byte ptr [ebp - 4]
        inc    edx
        inc    bl
        mov    bh, byte ptr [ebp + 14h]
        mov    byte ptr [ebp - 4], bl
        cmp    bl, bh
        jae    SHORT loc_2_043C
loc_2_0438:
        test   eax, eax
        jne    SHORT loc_2_0417
loc_2_043C:
        test   eax, eax
        jne    SHORT loc_2_0409
loc_2_0440:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_0450:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp + 24h]
        push   edx
        push   ebx
        mov    dword ptr [ebx], 0
        call   sub_2_0330
        add    esp, 8
        mov    dword ptr [ebx], eax
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        push   esi
        call   sub_2_0390
        add    esp, 10h
        mov    edi, dword ptr [ebp + 24h]
        push   edi
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [esi + 36h]
        push   eax
        push   esi
        call   sub_2_03F0
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0C0h
sub_2_04A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 20h]
        push   4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   edx
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        push   ebx
        call   sub_2_0390
        add    esp, 10h
        mov    edi, dword ptr [ebp + 24h]
        push   edi
        xor    eax, eax
        push   esi
        mov    al, byte ptr [ebx + 36h]
        push   eax
        push   ebx
        call   sub_2_03F0
        add    esp, 10h
        push   edi
        push   esi
        call   sub_2_0330
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        setne  al
        and    eax, 0ffh
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0510:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  08Bh, 01Dh, 078h, 024h, 004h, 000h
        xor    ah, ah
        db  08Ah, 015h, 00Ch, 024h, 004h, 000h
        mov    byte ptr [ebp - 4], ah
        test   dl, dl
        jbe    SHORT loc_2_0577
loc_2_052C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 6
        mov    dh, 35h
        add    eax, ebx
        db  088h, 035h, 020h, 024h, 004h, 000h
        mov    dl, byte ptr [eax + 30h]
        db  068h, 020h, 024h, 004h, 000h
        db  088h, 015h, 050h, 024h, 004h, 000h
        db  0A3h, 024h, 024h, 004h, 000h
        call   sub_2_0990
        add    esp, 4
        push   ebx
        mov    byte ptr [ebx], 31h
        call   sub_2_0990
        mov    cl, byte ptr [ebp - 4]
        db  08Ah, 02Dh, 00Ch, 024h, 004h, 000h
        inc    cl
        add    esp, 4
        mov    byte ptr [ebp - 4], cl
        cmp    cl, ch
        jb     SHORT loc_2_052C
loc_2_0577:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret
        db  08Bh, 0C0h
loc_2_0580:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0bh
        jb     SHORT loc_2_058F
        jbe    SHORT loc_2_05BD
        cmp    al, 17h
        je     SHORT loc_2_05BD
        jmp    SHORT loc_2_05AA
loc_2_058F:
        test   al, al
        jne    SHORT loc_2_05AA
        mov    byte ptr [ebx], 95h
        db  0C7h, 043h, 02Ch, 000h, 006h, 002h, 000h
        mov    word ptr [ebx + 8], 0a6h
        push   ebx
        mov    byte ptr [ebx + 2ah], 14h
        jmp    SHORT loc_2_05B5
loc_2_05AA:
        mov    byte ptr [ebx], 92h
        push   ebx
        db  0C7h, 043h, 02Ch, 0B0h, 006h, 002h, 000h
loc_2_05B5:
        call   sub_2_0990
        add    esp, 4
loc_2_05BD:
        ret
        db  08Bh, 0C0h
sub_2_05C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        push   10h
        lea    eax, [ebx + 1ah]
        push   eax
        db  068h, 0D2h, 023h, 004h, 000h
        call   sub_2_0450
        add    esp, 14h
        db  0C7h, 043h, 02Ch, 070h, 006h, 002h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [ebx], 94h
        push   ebx
        mov    word ptr [ebx + 8], ax
        call   sub_2_0990
        add    esp, 4
        pop    ebp
        pop    ebx
        ret
        db  090h
loc_2_0600:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 6
        jb     SHORT loc_2_0615
        jbe    SHORT loc_2_0649
        cmp    al, 0bh
        jb     SHORT loc_2_0655
        jbe    SHORT loc_2_066C
        cmp    al, 0ffh
        je     SHORT loc_2_0649
        jmp    SHORT loc_2_0655
loc_2_0615:
        test   al, al
        jne    SHORT loc_2_0655
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        push   eax
        mov    edx, dword ptr [ebx + 4]
        push   edx
        push   10h
        lea    eax, [ebx + 1ah]
        push   eax
        db  068h, 0D2h, 023h, 004h, 000h
        call   sub_2_04A0
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0649
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        push   ebx
        call   sub_2_02F0
        add    esp, 8
        ret
loc_2_0649:
        mov    byte ptr [ebx], 95h
        push   ebx
        mov    word ptr [ebx + 8], 0a6h
        jmp    SHORT loc_2_0664
loc_2_0655:
        mov    byte ptr [ebx], 92h
        db  0C7h, 043h, 02Ch, 0B0h, 006h, 002h, 000h
        push   ebx
        mov    byte ptr [ebx + 0ah], 2ah
loc_2_0664:
        call   sub_2_0990
        add    esp, 4
loc_2_066C:
        ret
        db  08Dh, 040h, 000h
loc_2_0670:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0ah
        jb     SHORT loc_2_0690
        jbe    SHORT loc_2_067F
        cmp    al, 0bh
        je     SHORT loc_2_06A9
        jmp    SHORT loc_2_0690
loc_2_067F:
        mov    byte ptr [ebx], 92h
        db  0C7h, 043h, 02Ch, 0B0h, 006h, 002h, 000h
        push   ebx
        mov    byte ptr [ebx + 0ah], 2ah
        jmp    SHORT loc_2_06A1
loc_2_0690:
        db  0C7h, 043h, 02Ch, 000h, 006h, 002h, 000h
        mov    byte ptr [ebx], 95h
        push   ebx
        mov    word ptr [ebx + 8], 156h
loc_2_06A1:
        call   sub_2_0990
        add    esp, 4
loc_2_06A9:
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h
loc_2_06B0:
        mov    byte ptr [ebx], 91h
        db  0C7h, 043h, 02Ch, 080h, 005h, 002h, 000h
        push   ebx
        mov    byte ptr [ebx + 0ah], 2ah
        call   sub_2_0990
        add    esp, 4
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_06D0:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 078h, 024h, 004h, 000h
        test   ebx, ebx
        jne    SHORT loc_2_06E6
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_06E6:
        db  068h, 010h, 024h, 004h, 000h
        push   52h
        xor    esi, esi
        call   sub_2_0BB0
        db  08Bh, 015h, 010h, 026h, 004h, 000h
        add    esp, 8
        test   edx, edx
        jle    NEAR PTR loc_2_0781
        lea    edi, [ebx + 1ah]
loc_2_0708:
        push   40h
        push   0
        push   ebx
        call   sub_2_00D1
        add    esp, 0ch
        push   10h
        db  068h, 010h, 024h, 004h, 000h
        db  08Ah, 086h, 014h, 026h, 004h, 000h
        push   edi
        mov    byte ptr [ebx + 30h], al
        call   sub_2_0040
        add    esp, 0ch
        push   ebx
        mov    byte ptr [ebx], 30h
        call   sub_2_0990
        mov    ah, byte ptr [ebx + 3]
        add    esp, 4
        test   ah, ah
        jne    SHORT loc_2_074A
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_074A:
        db  0C7h, 043h, 004h, 02Ch, 023h, 004h, 000h
        mov    byte ptr [ebx + 0ah], 2ah
        mov    byte ptr [ebx + 2ah], 0
        mov    byte ptr [ebx + 2bh], 0
        mov    byte ptr [ebx], 91h
        push   ebx
        db  0C7h, 043h, 02Ch, 080h, 005h, 002h, 000h
        call   sub_2_0990
        add    edi, 40h
        inc    esi
        db  08Bh, 00Dh, 010h, 026h, 004h, 000h
        add    esp, 4
        add    ebx, 40h
        cmp    esi, ecx
        jl     SHORT loc_2_0708
loc_2_0781:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0790:
        enter  207h, 0
        aad    7
        add    al, byte ptr [eax]
        loop   loc_2_07A1
        add    al, byte ptr [eax]
        out    dx, eax
        pop    es
        add    al, byte ptr [eax]
        push   ebp
loc_2_07A1:
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 1], 1
        mov    byte ptr [eax + 2], 1
        cmp    edx, 3
        jne    SHORT loc_2_080C
        test   edx, edx
        jne    SHORT loc_2_07FC
        dec    edx
        cmp    edx, 3
        ja     SHORT loc_2_080C
        db  02Eh, 0FFh, 024h, 095h, 090h, 007h, 002h, 000h
loc_2_07C8:
        push   eax
        call   sub_2_0820
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret
loc_2_07D5:
        push   eax
        call   sub_2_0810
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret
loc_2_07E2:
        push   eax
        call   sub_2_0830
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret
loc_2_07EF:
        push   eax
        call   sub_2_0840
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret
loc_2_07FC:
        push   eax
        call   sub_2_0810
        mov    eax, 1
        add    esp, 4
        pop    ebp
        ret
loc_2_080C:
        xor    eax, eax
        pop    ebp
        ret
sub_2_0810:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3], 1
        pop    ebp
        ret
        db  08Dh, 044h, 020h, 000h
sub_2_0820:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3], 0
        pop    ebp
        ret
        db  08Dh, 044h, 020h, 000h
sub_2_0830:
        push   ebp
        mov    ebp, esp
        call   sub_2_09C0
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 3], al
        pop    ebp
        ret
sub_2_0840:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    al, byte ptr [ebx + 0ch]
        push   eax
        call   sub_2_0270
        add    esp, 4
        mov    edx, eax
        test   eax, eax
        sete   al
        mov    byte ptr [ebx + 3], al
        lea    eax, [ebx + 4]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0ch]
        push   eax
        push   2
        xor    eax, eax
        push   edx
        mov    al, byte ptr [ebx + 0bh]
        push   eax
        call   sub_2_0220
        add    esp, 14h
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 050h, 001h, 080h, 0FAh, 001h
        db  075h, 006h, 080h, 078h, 002h, 000h, 074h, 007h, 0C6h, 040h, 003h, 001h, 05Dh, 05Bh, 0C3h, 08Ah
        db  058h, 008h, 038h, 0D3h, 075h, 005h, 03Ah, 050h, 009h, 074h, 007h, 0C6h, 040h, 003h, 001h, 05Dh
        db  05Bh, 0C3h, 08Ah, 010h, 080h, 0FAh, 006h, 072h, 006h, 088h, 058h, 003h, 05Dh, 05Bh, 0C3h, 081h
        db  0E2h, 0FFh, 000h, 000h, 000h, 050h, 0FFh, 014h, 095h, 0E8h, 01Eh, 004h, 000h, 083h, 0C4h, 004h
        db  05Dh, 05Bh, 0C3h
sub_2_08D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edi, dword ptr [ebp + 14h]
        lea    esi, [edi*4]
        mov    ebx, dword ptr [ebp + 18h]
        add    esi, edi
        lea    eax, [ebx*8]
        shl    esi, 4
        add    eax, ebx
        mov    ebx, dword ptr [ebp + 1ch]
        add    esi, edi
        lea    eax, [eax*8]
        add    esi, esi
        shl    ebx, 6
        add    eax, esi
        add    eax, ebx
        cmp    eax, 0ffffh
        jle    SHORT loc_2_0917
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0917:
        push   eax
        call   sub_2_2BE0
        add    esp, 4
        mov    ebx, eax
        test   eax, eax
        jne    SHORT loc_2_0930
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0930:
        mov    eax, edi
        mov    esi, eax
        dec    eax
        test   esi, esi
        jne    SHORT loc_2_093E
        mov    esi, dword ptr [ebp + 18h]
        jmp    SHORT loc_2_097E
loc_2_093E:
        db  08Bh, 035h, 038h, 01Fh, 004h, 000h
        mov    dword ptr [ebx], esi
        db  089h, 01Dh, 038h, 01Fh, 004h, 000h
        add    ebx, 0a2h
        mov    esi, eax
        dec    eax
        test   esi, esi
        jne    SHORT loc_2_093E
        mov    esi, dword ptr [ebp + 18h]
        jmp    SHORT loc_2_097E
loc_2_095E:
        push   48h
        push   0
        push   ebx
        mov    edi, ebx
        call   sub_2_00D1
        db  0A1h, 040h, 01Fh, 004h, 000h
        add    ebx, 48h
        mov    dword ptr [edi + 44h], eax
        add    esp, 0ch
        db  089h, 03Dh, 040h, 01Fh, 004h, 000h
loc_2_097E:
        mov    eax, esi
        dec    esi
        test   eax, eax
        jne    SHORT loc_2_095E
        db  089h, 01Dh, 078h, 024h, 004h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_0990:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        db  0FFh, 015h, 004h, 020h, 004h, 000h
        mov    ah, byte ptr [esi + 1]
        add    esp, 4
        cmp    ah, 23h
        jne    SHORT loc_2_09AF
        cmp    ah, byte ptr [esi + 31h]
        je     SHORT loc_2_09B5
loc_2_09AF:
        xor    al, al
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_09B5:
        mov    al, 1
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_09C0:
        push   ebx
        db  08Bh, 015h, 02Ch, 01Fh, 004h, 000h
        mov    eax, edx
        test   edx, edx
        je     SHORT loc_2_09E2
loc_2_09CD:
        db  08Bh, 01Dh, 038h, 01Fh, 004h, 000h
        mov    edx, dword ptr [eax]
        mov    dword ptr [eax], ebx
        db  0A3h, 038h, 01Fh, 004h, 000h
        mov    eax, edx
        test   edx, edx
        jne    SHORT loc_2_09CD
loc_2_09E2:
        xor    edx, edx
        xor    ah, ah
        xor    al, al
        db  089h, 015h, 02Ch, 01Fh, 004h, 000h
        db  088h, 025h, 018h, 01Fh, 004h, 000h
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_0A00:
        push   ebp
        mov    ebp, esp
        push   8
        call   sub_2_0E50
        add    esp, 4
        db  0A2h, 00Ch, 024h, 004h, 000h
        xor    al, al
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0A20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    esi, dword ptr [ebp + 14h]
        push   0ah
        lea    eax, [ebp - 8]
        xor    edx, edx
        push   eax
        mov    edi, 10h
        mov    dword ptr [ebp - 8], edx
        push   edx
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 10h], edi
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        mov    eax, dword ptr [ebp - 8]
        db  0BBh, 018h, 000h, 004h, 000h
        mov    dx, word ptr [eax + 2]
        add    esp, 0ch
        cmp    dx, 30bh
        ja     SHORT loc_2_0AB9
        test   byte ptr [eax + 4], 8
        je     SHORT loc_2_0AB9
        lea    eax, [ebp - 4]
        push   eax
        push   3
        db  068h, 054h, 000h, 004h, 000h
        db  068h, 064h, 000h, 004h, 000h
        push   10000001h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_0A91
        mov    eax, 1
        jmp    NEAR PTR loc_2_0B36
loc_2_0A91:
        xor    bl, bl
        lea    eax, [eax]
        lea    edx, [edx]
        nop
loc_2_0AA0:
        xor    eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    al, bl
        mov    dl, byte ptr [edx + eax]
        inc    bl
        mov    byte ptr [esi + eax], dl
        cmp    bl, 10h
        jb     SHORT loc_2_0AA0
        jmp    NEAR PTR loc_2_0B34
loc_2_0AB9:
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 2], 400h
        jb     SHORT loc_2_0B34
        lea    eax, [ebp - 0ch]
        push   eax
        push   ebx
        push   80000002h
        push   40000900h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_0AE6
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0AF2
loc_2_0AE6:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0AF2:
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        push   0
        push   0
        db  068h, 054h, 000h, 004h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        push   edx
        push   40000901h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_2_0B22
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0B22:
        mov    ebx, dword ptr [ebp - 0ch]
        push   ebx
        push   40000902h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
loc_2_0B34:
        xor    eax, eax
loc_2_0B36:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 040h, 000h
sub_2_0B40:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   0ah
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_016D
        add    esp, 0ch
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_0B60:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 10h]
        mov    edx, ebx
        xor    eax, eax
loc_2_0B6B:
        mov    cl, byte ptr [edx]
        cmp    cl, 20h
        je     SHORT loc_2_0B7D
        test   cl, cl
        je     SHORT loc_2_0B7D
        inc    eax
        inc    edx
        cmp    eax, 10h
        jle    SHORT loc_2_0B6B
loc_2_0B7D:
        lea    edx, [eax + ebx]
        cmp    eax, 10h
        jl     SHORT loc_2_0B90
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebx + 0fh], al
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
loc_2_0B90:
        inc    eax
        mov    byte ptr [edx], 20h
        inc    edx
        cmp    eax, 10h
        jl     SHORT loc_2_0B90
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebx + 0fh], al
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_0BB0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        call   sub_2_0A20
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        call   sub_2_0B60
        add    esp, 8
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_0BE0:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   4
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_2820
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0C13
        mov    eax, dword ptr [ebp + 10h]
        push   9
        add    eax, 4
        push   eax
        lea    eax, [ebx + 4]
        push   eax
        call   sub_2_2820
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0C18
loc_2_0C13:
        mov    eax, 2
loc_2_0C18:
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_0C20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    esi, eax
        db  08Bh, 01Dh, 02Ch, 01Fh, 004h, 000h
        xor    edx, edx
        xor    edi, edi
        mov    dword ptr [ebp - 4], edx
        test   ebx, ebx
        je     SHORT loc_2_0C8B
loc_2_0C3C:
        push   esi
        lea    eax, [ebx + 4]
        push   eax
        call   sub_2_0BE0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_0C83
        lea    eax, [ebx + 6ah]
        push   eax
        lea    eax, [esi + 66h]
        push   eax
        call   sub_2_0B40
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_0C7A
        test   byte ptr [ebp + 14h], 4
        je     NEAR PTR loc_2_0D21
        db  0C7h, 005h, 0F4h, 01Fh, 004h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_0D21
loc_2_0C7A:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_0C8B
loc_2_0C83:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_2_0C3C
loc_2_0C8B:
        db  08Bh, 00Dh, 038h, 01Fh, 004h, 000h
        test   ecx, ecx
        jne    SHORT loc_2_0CAB
        db  0A1h, 024h, 01Fh, 004h, 000h
        inc    eax
        xor    ebx, ebx
        db  0A3h, 024h, 01Fh, 004h, 000h
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0CAB:
        push   9ah
        mov    eax, dword ptr [ecx]
        mov    ebx, ecx
        db  0A3h, 038h, 01Fh, 004h, 000h
        push   esi
        lea    eax, [ecx + 4]
        mov    dword ptr [ecx], 0
        push   eax
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 9eh], 0
        call   sub_2_0040
        add    esp, 0ch
        test   esi, esi
        jne    SHORT loc_2_0D0D
        test   edi, edi
        jne    SHORT loc_2_0CE9
        db  089h, 01Dh, 02Ch, 01Fh, 004h, 000h
        jmp    SHORT loc_2_0CEB
loc_2_0CE9:
        mov    dword ptr [edi], ebx
loc_2_0CEB:
        db  08Ah, 015h, 018h, 01Fh, 004h, 000h
        mov    edi, 1
        inc    dl
        db  089h, 03Dh, 0F4h, 01Fh, 004h, 000h
        db  088h, 015h, 018h, 01Fh, 004h, 000h
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_0D0D:
        or     byte ptr [ebx + 9eh], 4
        db  0A1h, 034h, 01Fh, 004h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 034h, 01Fh, 004h, 000h
loc_2_0D21:
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_2_0D30:
        xor    eax, eax
        ret
        db  090h, 084h, 00Dh, 002h, 000h, 02Dh, 00Eh, 002h, 000h, 0D5h, 00Dh, 002h, 000h, 0EBh, 00Dh, 002h
        db  000h, 017h, 00Eh, 002h, 000h, 001h, 00Eh, 002h, 000h, 08Dh, 044h, 020h, 000h
loc_2_0D50:
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 10h]
        cmp    dword ptr [edx + 28h], 0
        je     NEAR PTR loc_2_0E49
        mov    eax, dword ptr [edx + 2ch]
        mov    edx, dword ptr [edx + 40h]
        mov    dl, byte ptr [edx]
        and    edx, 0ffh
        sub    edx, 3
        cmp    edx, 5
        ja     NEAR PTR loc_2_0E49
        db  02Eh, 0FFh, 024h, 095h, 034h, 00Dh, 002h, 000h
loc_2_0D84:
        push   0
        add    eax, 4
        push   eax
        call   sub_2_0C20
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_2_0E49
        mov    edx, 2045h
        db  068h, 024h, 026h, 004h, 000h
        db  0BEh, 0CCh, 000h, 004h, 000h
        mov    edi, 0c8h
        push   200h
        db  066h, 089h, 015h, 02Ch, 026h, 004h, 000h
        db  089h, 035h, 034h, 026h, 004h, 000h
        db  089h, 03Dh, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_0DD5:
        push   10h
        db  068h, 00Ch, 020h, 004h, 000h
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_0DEB:
        push   16h
        db  068h, 01Ch, 020h, 004h, 000h
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_0E01:
        push   21h
        db  068h, 032h, 020h, 004h, 000h
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_0E17:
        push   2
        db  068h, 038h, 026h, 004h, 000h
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_0E2D:
        db  08Bh, 00Dh, 02Ch, 01Fh, 004h, 000h
        test   ecx, ecx
        je     SHORT loc_2_0E49
        push   9ah
        lea    edx, [ecx + 4]
        push   edx
        push   eax
        call   sub_2_0040
        add    esp, 0ch
loc_2_0E49:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret
        db  090h
sub_2_0E50:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        mov    esi, dword ptr [ebp + 10h]
        xor    edx, edx
        xor    ebx, ebx
        db  089h, 015h, 010h, 026h, 004h, 000h
        test   esi, esi
        jle    SHORT loc_2_0EA7
loc_2_0E69:
        mov    byte ptr [ebp - 40h], 7fh
        lea    eax, [ebp - 40h]
        mov    dl, 23h
        push   eax
        mov    byte ptr [ebp - 10h], bl
        mov    byte ptr [ebp - 3fh], dl
        call   sub_2_0990
        mov    dh, byte ptr [ebp - 3fh]
        add    esp, 4
        cmp    dh, 23h
        jne    SHORT loc_2_0E8E
        cmp    dh, byte ptr [ebp - 0fh]
        je     SHORT loc_2_0EA2
loc_2_0E8E:
        db  0A1h, 010h, 026h, 004h, 000h
        lea    edx, [eax + 1]
        db  089h, 015h, 010h, 026h, 004h, 000h
        db  088h, 098h, 014h, 026h, 004h, 000h
loc_2_0EA2:
        inc    ebx
        cmp    ebx, esi
        jl     SHORT loc_2_0E69
loc_2_0EA7:
        db  0A0h, 010h, 026h, 004h, 000h
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_0EC0:
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0b0h
        sub    ebp, 80h
        mov    bl, 8
        mov    ecx, 1
        db  0BEh, 0F4h, 000h, 004h, 000h
        db  0BFh, 061h, 01Dh, 004h, 000h
        db  068h, 024h, 026h, 004h, 000h
        xor    edx, edx
        xor    ah, ah
        push   200h
        mov    dword ptr [ebp + 74h], edx
        mov    dword ptr [ebp + 78h], edx
        db  088h, 025h, 00Ch, 020h, 004h, 000h
        db  088h, 025h, 01Ch, 020h, 004h, 000h
        db  088h, 025h, 032h, 020h, 004h, 000h
        db  088h, 01Dh, 038h, 026h, 004h, 000h
        db  089h, 00Dh, 024h, 026h, 004h, 000h
        db  089h, 035h, 028h, 026h, 004h, 000h
        db  089h, 03Dh, 034h, 026h, 004h, 000h
        mov    ebx, 2053h
        xor    dl, dl
        xor    dh, dh
        mov    eax, 0c8h
        db  066h, 089h, 015h, 02Eh, 026h, 004h, 000h
        db  066h, 089h, 01Dh, 02Ch, 026h, 004h, 000h
        db  0A3h, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        db  068h, 024h, 026h, 004h, 000h
        db  0BAh, 077h, 01Dh, 004h, 000h
        mov    ebx, 0c8h
        push   200h
        db  089h, 015h, 034h, 026h, 004h, 000h
        db  089h, 01Dh, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        db  068h, 024h, 026h, 004h, 000h
        mov    esi, 0c9h
        db  0B9h, 0A1h, 01Dh, 004h, 000h
        push   200h
        db  089h, 035h, 030h, 026h, 004h, 000h
        db  089h, 00Dh, 034h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        push   8
        db  068h, 0FCh, 000h, 004h, 000h
        lea    eax, [ebp + 58h]
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        lea    eax, [ebp + 58h]
        push   eax
        push   101h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1004
        push   eax
        db  068h, 008h, 001h, 004h, 000h
        db  068h, 0C4h, 022h, 004h, 000h
        call   sub_2_16C0
        add    esp, 0ch
        mov    edi, 2045h
        db  068h, 024h, 026h, 004h, 000h
        db  066h, 089h, 03Dh, 02Ch, 026h, 004h, 000h
        db  0BFh, 0C4h, 022h, 004h, 000h
        push   200h
        db  089h, 01Dh, 030h, 026h, 004h, 000h
        db  089h, 03Dh, 034h, 026h, 004h, 000h
        jmp    NEAR PTR loc_2_12CB
loc_2_1004:
        db  068h, 010h, 026h, 004h, 000h
        mov    eax, dword ptr [ebp + 6ch]
        db  068h, 020h, 024h, 004h, 000h
        db  0A3h, 004h, 020h, 004h, 000h
        call   sub_2_0A00
        add    esp, 8
        db  08Bh, 03Dh, 010h, 026h, 004h, 000h
        push   edi
        push   0
        push   1
        call   sub_2_08D0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_105A
        db  068h, 024h, 026h, 004h, 000h
        mov    esi, 2045h
        db  0B9h, 038h, 01Eh, 004h, 000h
        push   200h
        db  066h, 089h, 035h, 02Ch, 026h, 004h, 000h
        mov    esi, 24h
        jmp    NEAR PTR loc_2_12BF
loc_2_105A:
        lea    eax, [ebp - 30h]
        push   eax
        db  068h, 030h, 001h, 004h, 000h
        db  068h, 00Dh, 022h, 004h, 000h
        call   sub_2_2A10
        add    esp, 0ch
        lea    eax, [ebp - 30h]
        push   eax
        db  068h, 0B8h, 020h, 004h, 000h
        call   sub_2_23E0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_10CA
        lea    eax, [ebp - 30h]
        push   eax
        db  068h, 03Ch, 001h, 004h, 000h
        db  068h, 0C4h, 022h, 004h, 000h
        call   sub_2_16C0
        add    esp, 0ch
        mov    ecx, 2045h
        db  068h, 024h, 026h, 004h, 000h
        db  0B8h, 0C4h, 022h, 004h, 000h
        db  089h, 01Dh, 030h, 026h, 004h, 000h
        push   200h
        db  066h, 089h, 00Dh, 02Ch, 026h, 004h, 000h
        db  0A3h, 034h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
loc_2_10CA:
        call   sub_2_06D0
        test   eax, eax
        je     SHORT loc_2_1112
        mov    ecx, 2045h
        db  068h, 024h, 026h, 004h, 000h
        db  0BAh, 0FAh, 01Dh, 004h, 000h
        mov    ebx, 8
        push   200h
        db  066h, 089h, 00Dh, 02Ch, 026h, 004h, 000h
        db  089h, 015h, 034h, 026h, 004h, 000h
        db  089h, 01Dh, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        lea    esp, [ebp + 80h]
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_1112:
        mov    esi, 2053h
        db  0BBh, 050h, 001h, 004h, 000h
        db  068h, 024h, 026h, 004h, 000h
        mov    ecx, 12ch
        xor    edi, edi
        push   200h
        db  066h, 089h, 035h, 02Ch, 026h, 004h, 000h
        db  089h, 01Dh, 034h, 026h, 004h, 000h
        db  089h, 00Dh, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        mov    esi, 200000h
        mov    bh, 5
        add    esp, 8
        mov    cl, 14h
        db  0A1h, 008h, 020h, 004h, 000h
        push   4
        mov    dword ptr [ebp + 48h], eax
        lea    eax, [ebp + 7ch]
        mov    word ptr [ebp + 7eh], di
        push   eax
        lea    eax, [ebp + 44h]
        mov    dword ptr [ebp + 4ch], esi
        push   eax
        mov    byte ptr [ebp + 7ch], bh
        mov    byte ptr [ebp + 7dh], cl
        call   sub_2_0040
        add    esp, 0ch
        push   8
        db  068h, 070h, 001h, 004h, 000h
        lea    eax, [ebp + 34h]
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        push   8
        db  068h, 07Ch, 001h, 004h, 000h
        lea    eax, [ebp + 3ch]
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        lea    eax, [ebp + 34h]
        push   eax
        xor    edi, edi
        push   100h
        mov    dword ptr [ebp + 54h], edi
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_120B
        push   eax
        db  068h, 088h, 001h, 004h, 000h
        db  068h, 0C4h, 022h, 004h, 000h
        call   sub_2_16C0
        add    esp, 0ch
        mov    ebx, 2045h
        db  0BFh, 0C4h, 022h, 004h, 000h
        db  068h, 024h, 026h, 004h, 000h
        mov    eax, 0c8h
        db  066h, 089h, 01Dh, 02Ch, 026h, 004h, 000h
        push   200h
        db  089h, 03Dh, 034h, 026h, 004h, 000h
        db  0A3h, 030h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        lea    esp, [ebp + 80h]
        pop    ebp
        pop    edi
        pop    esi
        ret
loc_2_120B:
        push   1ch
        push   edi
        lea    eax, [ebp + 34h]
        push   eax
        call   sub_2_00D1
        add    esp, 0ch
        push   8
        db  068h, 070h, 001h, 004h, 000h
        lea    eax, [ebp + 34h]
        push   eax
        call   sub_2_0040
        add    esp, 0ch
        lea    eax, [ebp + 34h]
        push   eax
        push   101h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_128B
        push   eax
        db  068h, 0B0h, 001h, 004h, 000h
        db  068h, 0C4h, 022h, 004h, 000h
        call   sub_2_16C0
        add    esp, 0ch
        mov    eax, 2045h
        mov    edx, 0c8h
        db  068h, 024h, 026h, 004h, 000h
        db  066h, 0A3h, 02Ch, 026h, 004h, 000h
        db  0B8h, 0C4h, 022h, 004h, 000h
        push   200h
        db  089h, 015h, 030h, 026h, 004h, 000h
        db  0A3h, 034h, 026h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        jmp    SHORT loc_2_1293
loc_2_128B:
        mov    eax, dword ptr [ebp + 48h]
        db  0A3h, 008h, 020h, 004h, 000h
loc_2_1293:
        db  081h, 03Dh, 008h, 020h, 004h, 000h, 050h, 00Dh, 002h, 000h
        je     SHORT loc_2_12D4
        mov    edx, 2045h
        db  068h, 024h, 026h, 004h, 000h
        db  0B9h, 0D8h, 001h, 004h, 000h
        mov    esi, 0c8h
        push   200h
        db  066h, 089h, 015h, 02Ch, 026h, 004h, 000h
loc_2_12BF:
        db  089h, 00Dh, 034h, 026h, 004h, 000h
        db  089h, 035h, 030h, 026h, 004h, 000h
loc_2_12CB:
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
loc_2_12D4:
        lea    esp, [ebp + 80h]
        pop    ebp
        pop    edi
        pop    esi
        ret
        db  000h, 000h
sub_2_12E0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    ecx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 20h]
        lea    ebx, [ebp - 10h]
        xor    esi, esi
loc_2_12F4:
        mov    eax, ecx
        xor    edx, edx
        div    dword ptr [ebp + 1ch]
        mov    eax, ecx
        db  08Ah, 092h, 0C8h, 020h, 004h, 000h
        mov    ecx, dword ptr [ebp + 1ch]
        mov    byte ptr [ebx], dl
        xor    edx, edx
        div    ecx
        inc    ebx
        inc    esi
        mov    ecx, eax
        test   eax, eax
        jne    SHORT loc_2_12F4
        test   edi, edi
        je     SHORT loc_2_1328
        cmp    edi, esi
        jle    SHORT loc_2_1328
        sub    edi, esi
        je     SHORT loc_2_1328
loc_2_1320:
        inc    esi
        mov    byte ptr [ebx], 30h
        inc    ebx
        dec    edi
        jne    SHORT loc_2_1320
loc_2_1328:
        lea    edx, [ebp - 10h]
        cmp    ebx, edx
        je     SHORT loc_2_1340
        mov    ecx, dword ptr [ebp + 14h]
        mov    dl, byte ptr [ebx - 1]
        dec    ebx
        lea    edi, [ecx + 1]
        mov    byte ptr [ecx], dl
        mov    dword ptr [ebp + 14h], edi
        jmp    SHORT loc_2_1328
loc_2_1340:
        mov    eax, esi
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_1350:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, dword ptr [ebp + 14h]
        mov    dl, byte ptr [eax]
        mov    edi, eax
        test   dl, dl
        je     NEAR PTR loc_2_16A4
loc_2_136B:
        mov    cl, byte ptr [edi]
        cmp    cl, 25h
        je     SHORT loc_2_137C
        inc    ebx
        inc    edi
        mov    byte ptr [ebx - 1], cl
        jmp    NEAR PTR loc_2_169B
loc_2_137C:
        mov    edx, dword ptr [ebp + 1ch]
        mov    al, byte ptr [edi + 1]
        inc    edi
        add    edx, 4
        cmp    al, 6eh
        jb     SHORT loc_2_13D5
        jbe    NEAR PTR loc_2_1583
        cmp    al, 75h
        jb     SHORT loc_2_13BA
        jbe    NEAR PTR loc_2_15D8
        cmp    al, 78h
        jb     SHORT loc_2_13AD
        jbe    NEAR PTR loc_2_15F5
        cmp    al, 79h
        je     SHORT loc_2_140B
        jmp    NEAR PTR loc_2_1694
loc_2_13AD:
        cmp    al, 77h
        je     NEAR PTR loc_2_1654
        jmp    NEAR PTR loc_2_1694
loc_2_13BA:
        cmp    al, 70h
        jb     NEAR PTR loc_2_1694
        jbe    NEAR PTR loc_2_1612
        cmp    al, 73h
        je     NEAR PTR loc_2_14C5
        jmp    NEAR PTR loc_2_1694
loc_2_13D5:
        cmp    al, 62h
        jb     SHORT loc_2_13F0
        jbe    NEAR PTR loc_2_1674
        cmp    al, 63h
        jbe    SHORT loc_2_143D
        cmp    al, 69h
        je     NEAR PTR loc_2_1537
        jmp    NEAR PTR loc_2_1694
loc_2_13F0:
        cmp    al, 4eh
        jb     NEAR PTR loc_2_1694
        jbe    NEAR PTR loc_2_146A
        cmp    al, 61h
        je     NEAR PTR loc_2_14E7
        jmp    NEAR PTR loc_2_1694
loc_2_140B:
        mov    eax, dword ptr [ebp + 1ch]
        xor    esi, esi
        mov    dword ptr [ebp + 1ch], edx
        mov    eax, dword ptr [eax]
loc_2_1415:
        mov    ch, byte ptr [eax]
        cmp    ch, 20h
        jb     SHORT loc_2_1427
        cmp    ch, 7eh
        ja     SHORT loc_2_1427
        xor    edx, edx
        mov    dl, ch
        jmp    SHORT loc_2_142C
loc_2_1427:
        mov    edx, 2eh
loc_2_142C:
        inc    ebx
        inc    eax
        inc    esi
        mov    byte ptr [ebx - 1], dl
        cmp    esi, 10h
        jae    NEAR PTR loc_2_169A
        jmp    SHORT loc_2_1415
loc_2_143D:
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 1ch], edx
        test   eax, eax
        jne    SHORT loc_2_144E
        db  0B8h, 008h, 002h, 004h, 000h
loc_2_144E:
        movzx  esi, byte ptr [eax]
        inc    eax
        test   esi, esi
        je     NEAR PTR loc_2_169A
loc_2_145A:
        inc    ebx
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebx - 1], dl
        dec    esi
        je     NEAR PTR loc_2_169A
        jmp    SHORT loc_2_145A
loc_2_146A:
        mov    eax, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax]
        add    edx, 4
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp + 1ch], edx
        test   eax, eax
        jne    SHORT loc_2_1486
        db  0C7h, 045h, 0F8h, 008h, 002h, 004h, 000h
loc_2_1486:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 8]
        inc    eax
        movzx  esi, byte ptr [esi]
        mov    dword ptr [ebp - 8], eax
        cmp    esi, 20h
        jbe    SHORT loc_2_149D
        mov    esi, 20h
loc_2_149D:
        test   esi, esi
        je     NEAR PTR loc_2_169A
        push   2
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        push   10h
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        lea    eax, [edx + 1]
        dec    esi
        mov    dword ptr [ebp - 8], eax
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    SHORT loc_2_149D
loc_2_14C5:
        mov    esi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [esi]
        mov    dword ptr [ebp + 1ch], edx
        test   esi, esi
        jne    SHORT loc_2_14D6
        db  0BEh, 008h, 002h, 004h, 000h
loc_2_14D6:
        mov    dh, byte ptr [esi]
        test   dh, dh
        je     NEAR PTR loc_2_169A
        inc    ebx
        inc    esi
        mov    byte ptr [ebx - 1], dh
        jmp    SHORT loc_2_14D6
loc_2_14E7:
        mov    eax, dword ptr [ebp + 1ch]
        xor    esi, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 1ch], edx
        mov    dword ptr [ebp - 4], eax
loc_2_14F4:
        push   2
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   10h
        mov    al, byte ptr [edx]
        push   eax
        lea    ecx, [edx + 1]
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        call   sub_2_12E0
        add    ebx, eax
        inc    esi
        add    esp, 10h
        mov    byte ptr [ebx], 2dh
        inc    ebx
        cmp    esi, 5
        jb     SHORT loc_2_14F4
        push   2
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   10h
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    NEAR PTR loc_2_169A
loc_2_1537:
        mov    eax, dword ptr [ebp + 1ch]
        xor    esi, esi
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp + 1ch], edx
loc_2_1542:
        push   0
        mov    edx, dword ptr [ebp - 0ch]
        xor    eax, eax
        push   0ah
        mov    al, byte ptr [edx]
        push   eax
        inc    edx
        push   ebx
        mov    dword ptr [ebp - 0ch], edx
        call   sub_2_12E0
        add    ebx, eax
        inc    esi
        add    esp, 10h
        mov    byte ptr [ebx], 2eh
        inc    ebx
        cmp    esi, 3
        jb     SHORT loc_2_1542
        push   0
        mov    edx, dword ptr [ebp - 0ch]
        xor    eax, eax
        push   0ah
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    NEAR PTR loc_2_169A
loc_2_1583:
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax]
        movzx  si, byte ptr [eax + 1]
        and    esi, 0ffffh
        mov    al, byte ptr [eax]
        shl    esi, 8
        xor    ah, ah
        and    eax, 0ffffh
        add    esi, eax
        push   0
        mov    eax, esi
        push   0ah
        shr    eax, 0ah
        push   eax
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        push   0
        push   0ah
        and    esi, 3ffh
        add    ebx, eax
        push   esi
        mov    byte ptr [ebx], 2eh
        inc    ebx
        push   ebx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    NEAR PTR loc_2_169A
loc_2_15D8:
        push   0
        mov    esi, dword ptr [ebp + 1ch]
        push   0ah
        mov    esi, dword ptr [esi]
        push   esi
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    NEAR PTR loc_2_169A
loc_2_15F5:
        push   8
        mov    esi, dword ptr [ebp + 1ch]
        push   10h
        mov    esi, dword ptr [esi]
        push   esi
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    NEAR PTR loc_2_169A
loc_2_1612:
        mov    esi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [esi]
        mov    eax, esi
        push   4
        shr    eax, 10h
        push   10h
        and    eax, 0ffffh
        and    eax, 0ffffh
        push   eax
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        push   4
        add    ebx, eax
        xor    eax, eax
        push   10h
        mov    ax, si
        push   eax
        mov    byte ptr [ebx], 3ah
        inc    ebx
        push   ebx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    SHORT loc_2_169A
loc_2_1654:
        mov    eax, dword ptr [ebp + 1ch]
        push   4
        mov    esi, dword ptr [eax]
        push   10h
        and    esi, 0ffffh
        push   esi
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    SHORT loc_2_169A
loc_2_1674:
        mov    eax, dword ptr [ebp + 1ch]
        push   2
        mov    esi, dword ptr [eax]
        push   10h
        and    esi, 0ffh
        push   esi
        push   ebx
        mov    dword ptr [ebp + 1ch], edx
        call   sub_2_12E0
        add    esp, 10h
        add    ebx, eax
        jmp    SHORT loc_2_169A
loc_2_1694:
        inc    ebx
        mov    al, byte ptr [edi]
        mov    byte ptr [ebx - 1], al
loc_2_169A:
        inc    edi
loc_2_169B:
        cmp    byte ptr [edi], 0
        jne    NEAR PTR loc_2_136B
loc_2_16A4:
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, ebx
        mov    byte ptr [ebx], 0
        sub    eax, edx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_16C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        lea    eax, [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_1350
        add    esp, 0ch
        pop    ebp
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_2_16E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dl, byte ptr [eax]
        test   dl, dl
        jne    SHORT loc_2_16F3
        mov    eax, 1
        pop    ebp
        ret
loc_2_16F3:
        cmp    dl, 41h
        jne    SHORT loc_2_16FF
        mov    eax, 1
        pop    ebp
        ret
loc_2_16FF:
        cmp    dl, 61h
        jne    SHORT loc_2_170B
        mov    eax, 1
        pop    ebp
        ret
loc_2_170B:
        xor    eax, eax
        pop    ebp
        ret
        db  090h, 053h, 056h, 057h, 055h, 089h, 0E5h, 083h, 0ECh, 010h, 08Bh, 04Dh, 018h, 08Bh, 07Dh, 020h
        db  08Dh, 05Dh, 0F0h, 031h, 0F6h, 089h, 0C8h, 031h, 0D2h, 0F7h, 075h, 01Ch, 0A1h, 0F8h, 021h, 004h
        db  000h, 001h, 0C2h, 089h, 0C8h, 08Ah, 012h, 08Bh, 04Dh, 01Ch, 088h, 013h, 031h, 0D2h, 0F7h, 0F1h
        db  043h, 046h, 089h, 0C1h, 085h, 0C0h, 075h, 0DDh, 085h, 0FFh, 074h, 010h, 039h, 0F7h, 07Eh, 00Ch
        db  029h, 0F7h, 074h, 008h, 046h, 0C6h, 003h, 030h, 043h, 04Fh, 075h, 0F8h, 08Dh, 055h, 0F0h, 039h
        db  0D3h, 074h, 011h, 08Bh, 045h, 014h, 08Ah, 053h, 0FFh, 04Bh, 08Dh, 078h, 001h, 088h, 010h, 089h
        db  07Dh, 014h, 0EBh, 0E8h, 089h, 0F0h, 089h, 0ECh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 044h, 020h
        db  000h
sub_2_1780:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 1ch]
        test   ebx, ebx
        je     SHORT loc_2_179D
        test   byte ptr [ebx + 5], 80h
        jne    NEAR PTR loc_2_1813
loc_2_179D:
        db  08Bh, 01Dh, 0D4h, 027h, 004h, 000h
        db  0A3h, 0C0h, 027h, 004h, 000h
        test   ebx, ebx
        je     SHORT loc_2_17E9
        test   edx, edx
        jne    SHORT loc_2_17B5
        db  0BAh, 090h, 004h, 004h, 000h
loc_2_17B5:
        push   edx
        db  08Bh, 034h, 085h, 07Ch, 021h, 004h, 000h
        push   esi
        db  08Bh, 03Dh, 0D8h, 027h, 004h, 000h
        push   edi
        db  08Bh, 015h, 0D4h, 027h, 004h, 000h
        push   edx
        db  068h, 094h, 004h, 004h, 000h
        add    eax, 3e8h
        push   eax
        push   2045h
        call   sub_2_30F0
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_17E9:
        test   edx, edx
        jne    SHORT loc_2_17F2
        db  0BAh, 090h, 004h, 004h, 000h
loc_2_17F2:
        push   edx
        db  08Bh, 00Ch, 085h, 07Ch, 021h, 004h, 000h
        push   ecx
        db  068h, 0B0h, 004h, 004h, 000h
        add    eax, 3e8h
        push   eax
        push   2045h
        call   sub_2_30F0
        add    esp, 14h
loc_2_1813:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_1820:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 61h
        jb     SHORT loc_2_1830
        cmp    al, 7ah
        ja     SHORT loc_2_1830
        sub    al, 20h
loc_2_1830:
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_1840:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    ah, byte ptr [ebx]
        mov    esi, dword ptr [ebp + 14h]
        test   ah, ah
        je     SHORT loc_2_188A
loc_2_1854:
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_188A
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_1820
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        add    esp, 4
        mov    al, byte ptr [esi]
        push   eax
        call   sub_2_1820
        mov    ah, byte ptr [ebp - 4]
        add    esp, 4
        cmp    al, ah
        je     SHORT loc_2_1881
        xor    eax, eax
        jmp    SHORT loc_2_18A1
loc_2_1881:
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        inc    esi
        test   dl, dl
        jne    SHORT loc_2_1854
loc_2_188A:
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_2_1894
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_189C
loc_2_1894:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_189C:
        mov    eax, 1
loc_2_18A1:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_18B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebx]
        mov    esi, dword ptr [ebp + 18h]
        test   edx, edx
        je     SHORT loc_2_18E4
loc_2_18C2:
        mov    edi, dword ptr [ebx]
        push   edi
        push   esi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_18DA
        mov    eax, dword ptr [ebx + 4]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_18DA:
        mov    ecx, dword ptr [ebx + 8]
        add    ebx, 8
        test   ecx, ecx
        jne    SHORT loc_2_18C2
loc_2_18E4:
        mov    eax, 0ffffffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0C0h
sub_2_18F0:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 20h
        je     SHORT loc_2_1906
        cmp    al, 9
        je     SHORT loc_2_1906
        cmp    al, 0dh
        je     SHORT loc_2_1906
        cmp    al, 0ah
        jne    SHORT loc_2_190D
loc_2_1906:
        mov    eax, 1
        pop    ebp
        ret
loc_2_190D:
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Dh, 040h, 000h
sub_2_1920:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 30h
        jb     SHORT loc_2_1935
        cmp    al, 39h
        ja     SHORT loc_2_1935
        mov    eax, 1
        pop    ebp
        ret
loc_2_1935:
        cmp    dword ptr [ebp + 0ch], 0ah
        je     SHORT loc_2_194A
        cmp    al, 41h
        jb     SHORT loc_2_194A
        cmp    al, 46h
        ja     SHORT loc_2_194A
        mov    eax, 1
        pop    ebp
        ret
loc_2_194A:
        xor    eax, eax
        pop    ebp
        ret
        db  08Bh, 0C0h
sub_2_1950:
        push   ebp
        mov    ebp, esp
        mov    ah, byte ptr [ebp + 8]
        mov    al, ah
        and    eax, 0ffh
        push   eax
        call   sub_2_1820
        add    esp, 4
        mov    ah, al
        cmp    al, 30h
        jb     SHORT loc_2_197A
        cmp    al, 39h
        ja     SHORT loc_2_197A
        and    eax, 0ffh
        sub    eax, 30h
        pop    ebp
        ret
loc_2_197A:
        cmp    dword ptr [ebp + 0ch], 10h
        jne    SHORT loc_2_1996
        cmp    ah, 41h
        jb     SHORT loc_2_1996
        cmp    ah, 46h
        ja     SHORT loc_2_1996
        mov    al, ah
        and    eax, 0ffh
        sub    eax, 37h
        pop    ebp
        ret
loc_2_1996:
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_19A0:
        push   ebx
        push   esi
        db  0BAh, 0BCh, 026h, 004h, 000h
        push   edx
        push   100h
        db  08Bh, 01Dh, 0E8h, 020h, 004h, 000h
        push   ebx
        push   601h
        db  089h, 015h, 0C4h, 027h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 10h
        test   eax, eax
        jg     SHORT loc_2_19E5
        db  0B9h, 0BCh, 026h, 004h, 000h
        mov    esi, 1
        db  089h, 00Dh, 0D0h, 027h, 004h, 000h
        db  089h, 035h, 0C8h, 027h, 004h, 000h
        pop    esi
        pop    ebx
        ret
loc_2_19E5:
        db  0BAh, 0BCh, 026h, 004h, 000h
        add    edx, eax
        db  089h, 015h, 0D0h, 027h, 004h, 000h
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_1A00:
        push   ebx
        db  0A1h, 0C4h, 027h, 004h, 000h
        db  03Bh, 005h, 0D0h, 027h, 004h, 000h
        jb     SHORT loc_2_1A31
        db  083h, 03Dh, 0C8h, 027h, 004h, 000h, 000h
        je     SHORT loc_2_1A1B
        xor    al, al
        pop    ebx
        ret
loc_2_1A1B:
        call   sub_2_19A0
        db  0A1h, 0C4h, 027h, 004h, 000h
        db  03Bh, 005h, 0D0h, 027h, 004h, 000h
        jb     SHORT loc_2_1A31
        xor    al, al
        pop    ebx
        ret
loc_2_1A31:
        db  0A1h, 0C4h, 027h, 004h, 000h
        mov    bl, byte ptr [eax]
        lea    edx, [eax + 1]
        test   bl, bl
        jne    SHORT loc_2_1A49
        mov    al, 20h
        db  089h, 015h, 0C4h, 027h, 004h, 000h
        pop    ebx
        ret
loc_2_1A49:
        db  089h, 015h, 0C4h, 027h, 004h, 000h
        mov    al, byte ptr [eax]
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
sub_2_1A60:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        db  08Bh, 015h, 0C8h, 027h, 004h, 000h
        dec    esi
        test   edx, edx
        je     SHORT loc_2_1A8C
        db  0A1h, 0C4h, 027h, 004h, 000h
        db  03Bh, 005h, 0D0h, 027h, 004h, 000h
        jb     SHORT loc_2_1A8C
        mov    eax, 1
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_1A8C:
        test   esi, esi
        je     SHORT loc_2_1AAC
loc_2_1A90:
        call   sub_2_1A00
        mov    byte ptr [ebx], al
        cmp    al, 0dh
        je     SHORT loc_2_1A90
        test   al, al
        je     SHORT loc_2_1AA3
        cmp    al, 0ah
        jne    SHORT loc_2_1AA8
loc_2_1AA3:
        mov    byte ptr [ebx], 0
        jmp    SHORT loc_2_1AAC
loc_2_1AA8:
        inc    ebx
        dec    esi
        jmp    SHORT loc_2_1A8C
loc_2_1AAC:
        mov    byte ptr [ebx], 0
        db  08Bh, 035h, 0D8h, 027h, 004h, 000h
        inc    esi
        xor    eax, eax
        db  089h, 035h, 0D8h, 027h, 004h, 000h
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_1AD0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_2_1AE4
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
loc_2_1AE4:
        mov    dl, byte ptr [ebx]
        test   dl, dl
        je     SHORT loc_2_1B0B
        xor    eax, eax
        mov    al, dl
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_1B08
        cmp    byte ptr [ebx], 3bh
        jne    SHORT loc_2_1B0B
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
loc_2_1B08:
        inc    ebx
        jmp    SHORT loc_2_1AE4
loc_2_1B0B:
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
sub_2_1B10:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx], 0
        je     SHORT loc_2_1B42
loc_2_1B1C:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_1B3A
        cmp    byte ptr [ebx], 5bh
        jne    SHORT loc_2_1B42
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
loc_2_1B3A:
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        test   dl, dl
        jne    SHORT loc_2_1B1C
loc_2_1B42:
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_1B50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 18h]
        mov    ebx, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebx]
        mov    ah, byte ptr [ebx]
        mov    edi, 1fh
        test   ah, ah
        je     SHORT loc_2_1B82
loc_2_1B69:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_1B82
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        test   dl, dl
        jne    SHORT loc_2_1B69
loc_2_1B82:
        mov    dh, byte ptr [ebx]
        test   dh, dh
        jne    SHORT loc_2_1B8F
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1B8F:
        lea    eax, [ebx + 1]
        cmp    dh, 3dh
        jne    SHORT loc_2_1BA2
        inc    esi
        mov    ebx, eax
        mov    byte ptr [esi - 1], dh
        jmp    NEAR PTR loc_2_1C05
loc_2_1BA2:
        cmp    dh, 22h
        jne    SHORT loc_2_1BDC
        mov    ebx, eax
        test   edi, edi
        jle    SHORT loc_2_1BC4
loc_2_1BAD:
        mov    dl, byte ptr [ebx]
        test   dl, dl
        je     SHORT loc_2_1BC4
        cmp    dl, 22h
        je     SHORT loc_2_1BC4
        inc    esi
        mov    al, byte ptr [ebx]
        inc    ebx
        dec    edi
        mov    byte ptr [esi - 1], al
        test   edi, edi
        jg     SHORT loc_2_1BAD
loc_2_1BC4:
        cmp    byte ptr [ebx], 22h
        jne    SHORT loc_2_1C05
        inc    ebx
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [esi], 0
        mov    dword ptr [eax], ebx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1BDC:
        test   edi, edi
        jle    SHORT loc_2_1C05
        cmp    byte ptr [ebx], 0
        je     SHORT loc_2_1C05
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_1C05
        mov    ah, byte ptr [ebx]
        cmp    ah, 3dh
        je     SHORT loc_2_1C05
        inc    esi
        inc    ebx
        dec    edi
        mov    byte ptr [esi - 1], ah
        jmp    SHORT loc_2_1BDC
loc_2_1C05:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [esi], 0
        mov    dword ptr [eax], ebx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_1C20:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1C6A
        db  08Bh, 01Dh, 0CCh, 027h, 004h, 000h
        cmp    dword ptr [ebx], 0
        je     SHORT loc_2_1C68
loc_2_1C47:
        lea    eax, [ebp - 20h]
        push   eax
        mov    esi, dword ptr [ebx]
        push   esi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1C5E
        mov    eax, ebx
        jmp    SHORT loc_2_1C6A
loc_2_1C5E:
        mov    ecx, dword ptr [ebx + 8]
        add    ebx, 8
        test   ecx, ecx
        jne    SHORT loc_2_1C47
loc_2_1C68:
        xor    eax, eax
loc_2_1C6A:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
sub_2_1C70:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1CB8
        push   ebx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1CB8
        cmp    byte ptr [ebx], 3dh
        je     SHORT loc_2_1CA5
        xor    eax, eax
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_1CA5:
        push   ebx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_1CB8
        mov    eax, 1
loc_2_1CB8:
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 044h, 020h, 000h
sub_2_1CC0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 18h]
        xor    edi, edi
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [eax], edi
        cmp    byte ptr [ebx], 30h
        jne    SHORT loc_2_1CE8
        cmp    byte ptr [ebx + 1], 78h
        jne    SHORT loc_2_1CE8
        mov    esi, 10h
        add    ebx, 2
loc_2_1CE8:
        mov    dh, byte ptr [ebx]
        test   dh, dh
        je     SHORT loc_2_1D27
        cmp    dh, 2eh
        je     SHORT loc_2_1D27
        xor    eax, eax
        push   esi
        mov    al, dh
        push   eax
        call   sub_2_1920
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_1D11
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ebx
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1D11:
        imul   edi, esi
        xor    eax, eax
        push   esi
        mov    al, byte ptr [ebx]
        push   eax
        inc    ebx
        call   sub_2_1950
        add    esp, 8
        add    edi, eax
        jmp    SHORT loc_2_1CE8
loc_2_1D27:
        cmp    byte ptr [ebx], 2eh
        jne    SHORT loc_2_1D2D
        inc    ebx
loc_2_1D2D:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], edi
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 095h, 01Dh, 002h, 000h, 0CAh, 01Dh, 002h, 000h, 00Bh, 01Eh, 002h, 000h, 00Bh
        db  01Eh, 002h, 000h, 00Bh, 01Eh, 002h, 000h, 00Bh, 01Eh, 002h, 000h, 00Bh, 01Eh, 002h, 000h, 0CAh
        db  01Dh, 002h, 000h, 095h, 01Dh, 002h, 000h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
loc_2_1D70:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    ebx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 8
        ja     NEAR PTR loc_2_1E0B
        db  02Eh, 0FFh, 024h, 085h, 044h, 01Dh, 002h, 000h
loc_2_1D95:
        push   esi
        call   sub_2_16E0
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_2_1E35
        push   0ah
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_1CC0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1DEC
        push   esi
        push   ebx
        push   1
        call   sub_2_1780
        add    esp, 0ch
        jmp    SHORT loc_2_1DEC
loc_2_1DCA:
        push   10h
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_1CC0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1DEC
        push   esi
        push   ebx
        push   2
        call   sub_2_1780
        add    esp, 0ch
loc_2_1DEC:
        mov    edx, dword ptr [ebx + 10h]
        test   edx, edx
        je     SHORT loc_2_1E0F
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebx + 0ch]
        jb     SHORT loc_2_1DFF
        cmp    eax, edx
        jbe    SHORT loc_2_1E0F
loc_2_1DFF:
        push   esi
        push   ebx
        push   3
        call   sub_2_1780
        add    esp, 0ch
loc_2_1E0B:
        xor    eax, eax
        jmp    SHORT loc_2_1E3A
loc_2_1E0F:
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 1
        jbe    SHORT loc_2_1E23
        cmp    eax, 7
        jb     SHORT loc_2_1E35
        cmp    eax, 8
        jbe    SHORT loc_2_1E2D
        jmp    SHORT loc_2_1E35
loc_2_1E23:
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_2_1E35
loc_2_1E2D:
        mov    ebx, dword ptr [ebx + 8]
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx], al
loc_2_1E35:
        mov    eax, 1
loc_2_1E3A:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_1E40:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    esi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [esi + 8]
        mov    ah, byte ptr [ebx]
        mov    esi, dword ptr [esi + 10h]
        test   ah, ah
        je     SHORT loc_2_1E9B
loc_2_1E5B:
        test   esi, esi
        je     SHORT loc_2_1E9B
        xor    eax, eax
        push   10h
        mov    al, byte ptr [ebx]
        push   eax
        inc    ebx
        call   sub_2_1950
        add    esp, 8
        mov    dh, byte ptr [ebx]
        mov    dword ptr [ebp - 4], eax
        test   dh, dh
        je     SHORT loc_2_1E9B
        xor    eax, eax
        push   10h
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_1950
        mov    ah, byte ptr [ebp - 4]
        shl    ah, 4
        inc    edi
        add    al, ah
        inc    ebx
        mov    byte ptr [edi - 1], al
        dec    esi
        mov    dl, byte ptr [ebx]
        add    esp, 8
        test   dl, dl
        jne    SHORT loc_2_1E5B
loc_2_1E9B:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
loc_2_1EB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    esi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        push   0ah
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    dword ptr [ebp - 0ch], ebx
        call   sub_2_1CC0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_1EEE
        push   0ah
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_1CC0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_1EFE
loc_2_1EEE:
        push   ebx
        push   esi
        push   1
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_2_1F47
loc_2_1EFE:
        mov    edx, dword ptr [ebp - 4]
        test   edx, edx
        je     SHORT loc_2_1F19
        cmp    edx, 3fh
        ja     SHORT loc_2_1F19
        mov    edi, dword ptr [ebp - 8]
        test   edi, edi
        je     SHORT loc_2_1F19
        cmp    edi, 3ffh
        jbe    SHORT loc_2_1F2E
loc_2_1F19:
        push   ebx
        push   esi
        push   4
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_1F2E:
        mov    eax, edx
        shl    eax, 0ah
        lea    edx, [edi + eax]
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edx], ax
        mov    eax, 1
loc_2_1F47:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0C0h
loc_2_1F50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 14h]
        xor    edx, edx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 4], edx
        mov    edi, dword ptr [edi + 10h]
        mov    esi, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 4]
        dec    edi
        cmp    ecx, 9
        je     SHORT loc_2_1F7E
        cmp    ecx, 0ah
        jne    SHORT loc_2_1F7F
loc_2_1F7E:
        inc    esi
loc_2_1F7F:
        test   edi, edi
        je     SHORT loc_2_1FB7
        mov    ah, byte ptr [ebx]
        test   ah, ah
        je     SHORT loc_2_1FB7
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 4]
        mov    al, ah
        cmp    ecx, 9
        je     SHORT loc_2_1F9B
        cmp    ecx, 5
        jne    SHORT loc_2_1FA9
loc_2_1F9B:
        and    eax, 0ffh
        push   eax
        call   sub_2_1820
        add    esp, 4
loc_2_1FA9:
        mov    byte ptr [esi], al
        inc    esi
        mov    eax, dword ptr [ebp - 4]
        dec    edi
        inc    eax
        inc    ebx
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_1F7F
loc_2_1FB7:
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [eax + 4]
        cmp    ebx, 9
        je     SHORT loc_2_1FC7
        cmp    ebx, 0ah
        jne    SHORT loc_2_1FD4
loc_2_1FC7:
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 8]
        mov    bl, byte ptr [ebp - 4]
        mov    byte ptr [eax], bl
        jmp    SHORT loc_2_1FD7
loc_2_1FD4:
        mov    byte ptr [esi], 0
loc_2_1FD7:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
loc_2_1FF0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        call   sub_2_16E0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_2035
        push   ebx
        db  068h, 0ECh, 020h, 004h, 000h
        call   sub_2_18B0
        add    esp, 8
        mov    edx, eax
        cmp    eax, -1
        jne    SHORT loc_2_202D
        push   ebx
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        push   5
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
loc_2_202D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax], dl
loc_2_2035:
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 040h, 000h
loc_2_2040:
        push   ebp
        mov    ebp, esp
        mov    eax, 1
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h
loc_2_2050:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        db  068h, 054h, 021h, 004h, 000h
        call   sub_2_18B0
        add    esp, 8
        mov    edx, eax
        cmp    eax, -1
        jne    SHORT loc_2_2083
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   6
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
loc_2_2083:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax], dl
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
loc_2_20A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    eax, dword ptr [ebp + 18h]
        lea    esi, [ebp - 14h]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], eax
loc_2_20B4:
        push   0ah
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        lea    edi, [ebx*4]
        call   sub_2_1CC0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_20E4
        mov    esi, dword ptr [ebp + 18h]
        push   esi
        mov    edi, dword ptr [ebp + 14h]
        push   edi
        push   7
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_2_2134
loc_2_20E4:
        cmp    dword ptr [ebp + edi - 14h], 0ffh
        jbe    SHORT loc_2_2109
        mov    ebx, dword ptr [ebp + 18h]
        push   ebx
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   7
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_2109:
        inc    ebx
        add    esi, 4
        cmp    ebx, 4
        jl     SHORT loc_2_20B4
        mov    ebx, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebx + 8]
        mov    al, byte ptr [ebp - 14h]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 10h]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 0ch]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebx], al
        mov    eax, 1
loc_2_2134:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_2140:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        db  083h, 03Dh, 0BCh, 027h, 004h, 000h, 000h
        jne    SHORT loc_2_215C
        mov    eax, 1
        jmp    NEAR PTR loc_2_2200
loc_2_215C:
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 40h]
        push   eax
        lea    eax, [ebp + 14h]
        push   eax
        call   sub_2_1C70
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2189
        push   eax
        push   eax
        push   8
        call   sub_2_1780
        add    esp, 0ch
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_2189:
        db  08Bh, 01Dh, 0BCh, 027h, 004h, 000h
        mov    ebx, dword ptr [ebx + 4]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_2_21B4
loc_2_2197:
        lea    eax, [ebp - 40h]
        push   eax
        mov    edi, dword ptr [ebx]
        push   edi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_21B4
        mov    esi, dword ptr [ebx + 14h]
        add    ebx, 14h
        test   esi, esi
        jne    SHORT loc_2_2197
loc_2_21B4:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_2_21E3
        db  0F6h, 005h, 0E4h, 020h, 004h, 000h, 002h
        jne    SHORT loc_2_21D2
        lea    eax, [ebp - 40h]
        push   eax
        push   0
        push   9
        call   sub_2_1780
        add    esp, 0ch
loc_2_21D2:
        xor    edx, edx
        xor    eax, eax
        db  089h, 015h, 0C0h, 027h, 004h, 000h
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_21E3:
        mov    eax, dword ptr [ebx + 4]
        and    eax, 0ffh
        lea    edx, [eax*4]
        lea    eax, [ebp - 20h]
        push   eax
        push   ebx
        db  0FFh, 092h, 0BCh, 021h, 004h, 000h
        add    esp, 8
loc_2_2200:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_2210:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        db  0BAh, 0BCh, 026h, 004h, 000h
        db  0A3h, 0D4h, 027h, 004h, 000h
        xor    ecx, ecx
        db  089h, 015h, 0C4h, 027h, 004h, 000h
        db  089h, 015h, 0D0h, 027h, 004h, 000h
        db  089h, 00Dh, 0C8h, 027h, 004h, 000h
        db  089h, 00Dh, 0D8h, 027h, 004h, 000h
        db  08Bh, 015h, 0E8h, 020h, 004h, 000h
        db  089h, 00Dh, 0C0h, 027h, 004h, 000h
        test   edx, edx
        jl     SHORT loc_2_2265
        test   byte ptr [ebp + 10h], 1
        jne    SHORT loc_2_225D
        push   eax
        push   ecx
        push   0ah
        call   sub_2_1780
        add    esp, 0ch
loc_2_225D:
        db  0A1h, 0C0h, 027h, 004h, 000h
        pop    ebp
        pop    ebx
        ret
loc_2_2265:
        push   ecx
        push   eax
        call   sub_2_2F70
        add    esp, 8
        db  0A3h, 0E8h, 020h, 004h, 000h
        test   eax, eax
        jge    SHORT loc_2_2299
        test   byte ptr [ebp + 10h], 1
        jne    SHORT loc_2_2291
        db  08Bh, 01Dh, 0D4h, 027h, 004h, 000h
        push   ebx
        push   0
        push   0ah
        call   sub_2_1780
        add    esp, 0ch
loc_2_2291:
        db  0A1h, 0C0h, 027h, 004h, 000h
        pop    ebp
        pop    ebx
        ret
loc_2_2299:
        call   sub_2_19A0
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
sub_2_22B0:
        db  08Bh, 015h, 0E8h, 020h, 004h, 000h
        test   edx, edx
        jl     SHORT loc_2_22CE
        push   edx
        call   sub_2_2FB0
        mov    ecx, 0ffffffffh
        add    esp, 4
        db  089h, 00Dh, 0E8h, 020h, 004h, 000h
loc_2_22CE:
        ret
        db  090h, 053h, 057h, 055h, 089h, 0E5h, 081h, 0ECh, 080h, 000h, 000h, 000h, 083h, 0EDh, 07Eh, 08Bh
        db  09Dh, 08Eh, 000h, 000h, 000h, 08Bh, 015h, 0E8h, 020h, 004h, 000h, 085h, 0D2h, 00Fh, 08Ch, 08Ch
        db  000h, 000h, 000h, 031h, 0FFh, 057h, 0B9h, 0BCh, 026h, 004h, 000h, 057h, 089h, 00Dh, 0C4h, 027h
        db  004h, 000h, 089h, 00Dh, 0D0h, 027h, 004h, 000h, 052h, 089h, 03Dh, 0C8h, 027h, 004h, 000h, 089h
        db  03Dh, 0D8h, 027h, 004h, 000h, 0E8h, 017h, 00Dh, 000h, 000h, 083h, 0C4h, 00Ch, 0E8h, 07Fh, 0F6h
        db  0FFh, 0FFh, 068h, 080h, 000h, 000h, 000h, 08Dh, 045h, 0FEh, 050h, 0E8h, 031h, 0F7h, 0FFh, 0FFh
        db  083h, 0C4h, 008h, 085h, 0C0h, 075h, 048h, 08Dh, 045h, 0FEh, 050h, 0E8h, 091h, 0F7h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 085h, 0C0h, 075h, 0DBh, 08Dh, 045h, 0FEh, 050h, 0E8h, 0C1h, 0F7h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 085h, 0C0h, 074h, 0CBh, 06Ah, 05Dh, 08Dh, 045h, 0FEh, 050h, 0E8h, 04Fh, 006h
        db  000h, 000h, 083h, 0C4h, 008h, 085h, 0C0h, 074h, 003h, 0C6h, 000h, 000h, 053h, 08Dh, 045h, 0FFh
        db  050h, 0E8h, 0CBh, 003h, 000h, 000h, 083h, 0C4h, 008h, 085h, 0C0h, 075h, 0A5h, 0EBh, 005h, 0B8h
        db  0FFh, 0FFh, 0FFh, 0FFh, 08Dh, 065h, 07Eh, 05Dh, 05Fh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h
        db  000h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 05Dh, 010h, 08Bh, 075h, 014h, 056h, 053h, 0E8h, 0BEh
        db  0F6h, 0FFh, 0FFh, 083h, 0C4h, 008h, 083h, 0F8h, 001h, 075h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Eh, 05Bh, 0C3h, 080h, 03Bh, 05Bh, 075h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Eh
        db  05Bh, 0C3h, 053h, 0E8h, 009h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 085h, 0C0h, 075h, 0CDh, 05Dh
        db  05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh
        db  0C0h
sub_2_23E0:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   0
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_2400
        add    esp, 0ch
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_2400:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        db  0A3h, 0CCh, 027h, 004h, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  089h, 015h, 0BCh, 027h, 004h, 000h
        db  0A3h, 0E4h, 020h, 004h, 000h
        call   sub_2_2A70
        call   sub_2_2B90
        cmp    ax, 400h
        jb     SHORT loc_2_243D
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_2_2550
        add    esp, 4
        pop    ebp
        pop    esi
        pop    ebx
        ret
loc_2_243D:
        mov    ebx, dword ptr [ebp + 18h]
        push   ebx
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        call   sub_2_2210
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_24B1
        db  0BBh, 03Ch, 026h, 004h, 000h
        xor    esi, esi
loc_2_2458:
        push   80h
        push   ebx
        call   sub_2_1A60
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_24A7
        push   ebx
        call   sub_2_1AD0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_2_2458
        push   ebx
        call   sub_2_1B10
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_2494
        push   ebx
        call   sub_2_1C20
        add    esp, 4
        db  0A3h, 0BCh, 027h, 004h, 000h
        jmp    SHORT loc_2_2458
loc_2_2494:
        db  03Bh, 035h, 0BCh, 027h, 004h, 000h
        je     SHORT loc_2_2458
        push   ebx
        call   sub_2_2140
        add    esp, 4
        jmp    SHORT loc_2_2458
loc_2_24A7:
        call   sub_2_22B0
        db  0A1h, 0C0h, 027h, 004h, 000h
loc_2_24B1:
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_24C0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        mov    ebx, dword ptr [ebp + 14h]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_2_2543
        mov    esi, 100h
loc_2_24D9:
        push   100h
        push   0
        lea    eax, [ebp - 108h]
        push   eax
        call   sub_2_00D1
        add    esp, 0ch
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 8], esi
        push   0
        mov    edi, dword ptr [ebx]
        push   edi
        db  0A1h, 0E0h, 020h, 004h, 000h
        push   eax
        push   40000901h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_2539
        lea    eax, [ebp - 108h]
        mov    edx, dword ptr [ebx + 4]
        push   eax
        and    edx, 0ffh
        push   ebx
        db  0FFh, 014h, 095h, 0BCh, 021h, 004h, 000h
        add    esp, 8
loc_2_2539:
        mov    ecx, dword ptr [ebx + 14h]
        add    ebx, 14h
        test   ecx, ecx
        jne    SHORT loc_2_24D9
loc_2_2543:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_2550:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 140h
        mov    ebx, dword ptr [ebp + 14h]
        xor    edx, edx
        db  089h, 015h, 0D4h, 027h, 004h, 000h
        db  089h, 015h, 0D8h, 027h, 004h, 000h
        cmp    dword ptr [ebx], 0
        je     NEAR PTR loc_2_263F
        lea    edi, [ebp - 40h]
        inc    edi
loc_2_257A:
        mov    edx, dword ptr [ebx]
        push   edx
        lea    eax, [ebp - 40h]
        push   eax
        lea    esi, [ebp - 40h]
        call   sub_2_2650
        mov    ah, byte ptr [ebp - 40h]
        add    esp, 8
        cmp    ah, 5bh
        jne    SHORT loc_2_2596
        mov    esi, edi
loc_2_2596:
        push   5dh
        push   esi
        call   sub_2_29B0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_25A8
        mov    byte ptr [eax], 0
loc_2_25A8:
        push   esi
        call   sub_2_29E0
        add    esp, 4
        push   esi
        db  08Bh, 00Dh, 0DCh, 020h, 004h, 000h
        push   ecx
        lea    eax, [ebp - 140h]
        push   eax
        call   sub_2_16C0
        add    esp, 0ch
        db  068h, 0E0h, 020h, 004h, 000h
        lea    eax, [ebp - 140h]
        push   eax
        push   80000002h
        push   40000900h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_2615
        mov    esi, dword ptr [ebx + 4]
        push   esi
        call   sub_2_24C0
        add    esp, 4
        db  0A1h, 0E0h, 020h, 004h, 000h
        push   eax
        push   40000902h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        xor    edx, edx
        add    esp, 8
        db  089h, 015h, 0E0h, 020h, 004h, 000h
        jmp    SHORT loc_2_2631
loc_2_2615:
        db  0F6h, 005h, 0E4h, 020h, 004h, 000h, 001h
        jne    SHORT loc_2_2631
        lea    eax, [ebp - 140h]
        push   eax
        push   0
        push   0ch
        call   sub_2_1780
        add    esp, 0ch
loc_2_2631:
        mov    eax, dword ptr [ebx + 8]
        add    ebx, 8
        test   eax, eax
        jne    NEAR PTR loc_2_257A
loc_2_263F:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2650:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        mov    bl, byte ptr [eax]
        mov    ecx, edx
        test   bl, bl
        je     SHORT loc_2_266E
loc_2_2662:
        mov    bl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], bl
        mov    bh, byte ptr [eax]
        inc    edx
        test   bh, bh
        jne    SHORT loc_2_2662
loc_2_266E:
        mov    eax, ecx
        mov    byte ptr [edx], 0
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_2680:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    bl, byte ptr [eax]
        mov    ecx, eax
        test   bl, bl
        je     SHORT loc_2_269A
loc_2_2692:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        test   bh, bh
        jne    SHORT loc_2_2692
loc_2_269A:
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     SHORT loc_2_26A7
        inc    eax
        inc    edx
        mov    byte ptr [eax - 1], bl
        jmp    SHORT loc_2_269A
loc_2_26A7:
        mov    byte ptr [eax], bl
        mov    eax, ecx
        pop    ebp
        pop    ebx
        ret
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 055h, 010h, 08Ah, 008h, 08Bh, 05Dh
        db  014h, 084h, 0C9h, 074h, 008h, 08Ah, 068h, 001h, 040h, 084h, 0EDh, 075h, 0F8h, 089h, 0D9h, 04Bh
        db  085h, 0C9h, 074h, 00Dh, 08Ah, 00Ah, 084h, 0C9h, 074h, 007h, 040h, 042h, 088h, 048h, 0FFh, 0EBh
        db  0ECh, 0C6h, 000h, 000h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
        db  08Bh, 0DBh
sub_2_26F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [eax]
        xor    edx, edx
        test   bl, bl
        je     SHORT loc_2_2708
loc_2_26FF:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        inc    edx
        test   bh, bh
        jne    SHORT loc_2_26FF
loc_2_2708:
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 040h, 000h
sub_2_2710:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        mov    esi, edx
        test   ebx, ebx
        jle    SHORT loc_2_2735
loc_2_2724:
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_2735
        inc    edx
        mov    cl, byte ptr [eax]
        inc    eax
        dec    ebx
        mov    byte ptr [edx - 1], cl
        test   ebx, ebx
        jg     SHORT loc_2_2724
loc_2_2735:
        mov    eax, esi
        mov    byte ptr [edx], 0
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 018h, 08Bh, 055h, 010h, 084h, 0DBh
        db  074h, 043h, 08Ah, 00Ah, 084h, 0C9h, 074h, 03Dh, 08Ah, 038h, 088h, 0CBh, 080h, 0FFh, 061h, 072h
        db  008h, 080h, 0FFh, 07Ah, 077h, 003h, 080h, 0EFh, 020h, 080h, 0FBh, 061h, 072h, 008h, 080h, 0FBh
        db  07Ah, 077h, 003h, 080h, 0EBh, 020h, 038h, 0DFh, 073h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh
        db  05Bh, 0C3h, 076h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Bh, 0C3h, 08Ah, 078h, 001h, 040h
        db  042h, 084h, 0FFh, 075h, 0BDh, 080h, 038h, 000h, 074h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh
        db  05Bh, 0C3h, 080h, 03Ah, 000h, 074h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 031h
        db  0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 018h, 08Bh, 055h, 010h, 084h, 0DBh
        db  074h, 027h, 08Ah, 00Ah, 084h, 0C9h, 074h, 021h, 08Ah, 018h, 038h, 0CBh, 073h, 008h, 0B8h, 0FFh
        db  0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 076h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Bh, 0C3h
        db  08Ah, 078h, 001h, 040h, 042h, 084h, 0FFh, 075h, 0D9h, 080h, 038h, 000h, 074h, 008h, 0B8h, 001h
        db  000h, 000h, 000h, 05Dh, 05Bh, 0C3h, 080h, 03Ah, 000h, 074h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Bh, 0C3h, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h
        db  022h, 000h
sub_2_2820:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, 1
        cmp    esi, eax
        jl     SHORT loc_2_287F
loc_2_283A:
        mov    ch, byte ptr [ebx]
        mov    cl, byte ptr [edx]
        mov    byte ptr [ebp - 4], ch
        cmp    cl, 61h
        jb     SHORT loc_2_284E
        cmp    cl, 7ah
        ja     SHORT loc_2_284E
        sub    cl, 20h
loc_2_284E:
        mov    ch, byte ptr [ebp - 4]
        cmp    ch, 61h
        jb     SHORT loc_2_2861
        cmp    ch, 7ah
        ja     SHORT loc_2_2861
        sub    ch, 20h
        mov    byte ptr [ebp - 4], ch
loc_2_2861:
        mov    ch, byte ptr [ebp - 4]
        cmp    cl, ch
        jae    SHORT loc_2_286F
        mov    eax, 0ffffffffh
        jmp    SHORT loc_2_2881
loc_2_286F:
        jbe    SHORT loc_2_2878
        mov    eax, 1
        jmp    SHORT loc_2_2881
loc_2_2878:
        inc    edx
        inc    eax
        inc    ebx
        cmp    eax, esi
        jle    SHORT loc_2_283A
loc_2_287F:
        xor    eax, eax
loc_2_2881:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 045h
        db  010h, 08Bh, 055h, 014h, 08Bh, 075h, 018h, 0BBh, 001h, 000h, 000h, 000h, 039h, 0DEh, 07Ch, 023h
        db  08Ah, 008h, 08Ah, 02Ah, 038h, 0E9h, 073h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Eh, 05Bh
        db  0C3h, 076h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Eh, 05Bh, 0C3h, 040h, 043h, 042h, 039h
        db  0F3h, 07Eh, 0DDh, 031h, 0C0h, 05Dh, 05Eh, 05Bh, 0C3h, 053h, 056h, 057h, 055h, 089h, 0E5h, 083h
        db  0ECh, 00Ch, 08Bh, 05Dh, 014h, 08Bh, 075h, 018h, 0BFh, 00Ah, 000h, 000h, 000h, 031h, 0C9h, 089h
        db  0D8h, 031h, 0D2h, 0F7h, 0F7h, 081h, 0E2h, 0FFh, 000h, 000h, 000h, 083h, 0C2h, 030h, 088h, 0D0h
        db  00Fh, 0BFh, 0D1h, 088h, 044h, 015h, 0F4h, 089h, 0D8h, 031h, 0D2h, 0F7h, 0F7h, 041h, 089h, 0C3h
        db  085h, 0C0h, 075h, 0DBh, 089h, 0C8h, 066h, 085h, 0C9h, 074h, 011h, 049h, 00Fh, 0BFh, 0D1h, 046h
        db  08Ah, 054h, 015h, 0F4h, 088h, 056h, 0FFh, 066h, 085h, 0C9h, 075h, 0EFh, 098h, 0C6h, 006h, 000h
        db  089h, 0ECh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch
        db  08Ah, 018h, 031h, 0D2h, 084h, 0DBh, 074h, 020h, 08Ah, 008h, 080h, 0F9h, 030h, 072h, 019h, 080h
        db  0F9h, 039h, 077h, 014h, 06Bh, 0D2h, 00Ah, 08Ah, 018h, 030h, 0FFh, 083h, 0EBh, 030h, 001h, 0DAh
        db  08Ah, 078h, 001h, 040h, 084h, 0FFh, 075h, 0E0h, 031h, 0C0h, 066h, 089h, 0D0h, 05Dh, 05Bh, 0C3h
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 00Ch
        db  08Bh, 04Dh, 010h, 089h, 0D3h, 089h, 0C8h, 080h, 03Ah, 000h, 074h, 025h, 08Ah, 010h, 08Ah, 033h
        db  084h, 0D2h, 074h, 013h, 08Ah, 010h, 040h, 038h, 0D6h, 074h, 007h, 080h, 038h, 000h, 075h, 0F4h
        db  0EBh, 005h, 089h, 0D8h, 05Dh, 05Bh, 0C3h, 089h, 0C8h, 08Ah, 053h, 001h, 043h, 084h, 0D2h, 075h
        db  0DBh, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 040h, 000h
sub_2_29B0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    dh, byte ptr [eax]
        mov    dl, byte ptr [ebp + 10h]
        test   dh, dh
        je     SHORT loc_2_29CC
loc_2_29C0:
        cmp    dl, byte ptr [eax]
        je     SHORT loc_2_29CC
        mov    bl, byte ptr [eax + 1]
        inc    eax
        test   bl, bl
        jne    SHORT loc_2_29C0
loc_2_29CC:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_29D3
        xor    eax, eax
loc_2_29D3:
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_29E0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [eax]
        mov    edx, eax
        test   bl, bl
        je     SHORT loc_2_2A0A
loc_2_29EF:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jb     SHORT loc_2_2A02
        cmp    cl, 7ah
        ja     SHORT loc_2_2A02
        mov    bl, cl
        sub    bl, 20h
        mov    byte ptr [eax], bl
loc_2_2A02:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        test   bh, bh
        jne    SHORT loc_2_29EF
loc_2_2A0A:
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret
        db  090h
sub_2_2A10:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        push   ebx
        call   sub_2_2650
        add    esp, 8
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        call   sub_2_26F0
        add    eax, ebx
        mov    dl, byte ptr [eax - 1]
        add    esp, 4
        cmp    dl, 5ch
        jne    SHORT loc_2_2A3F
        mov    byte ptr [eax - 1], 0
loc_2_2A3F:
        mov    eax, dword ptr [ebp + 14h]
        cmp    byte ptr [eax], 5ch
        je     SHORT loc_2_2A55
        db  068h, 0C0h, 004h, 004h, 000h
        push   ebx
        call   sub_2_2680
        add    esp, 8
loc_2_2A55:
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        push   ebx
        call   sub_2_2680
        add    esp, 8
        pop    ebp
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2A70:
        push   ebx
        db  083h, 03Dh, 0FCh, 021h, 004h, 000h, 000h
        jne    NEAR PTR loc_2_2B3B
        db  0C7h, 005h, 0FCh, 021h, 004h, 000h, 001h, 000h, 000h, 000h
        call   sub_2_33D0
        db  08Bh, 01Dh, 0FCh, 035h, 002h, 000h
        add    ebx, 0ch
        xor    edx, edx
        mov    cl, byte ptr [ebx]
        mov    eax, ebx
        test   cl, cl
        je     SHORT loc_2_2AB9
loc_2_2AA0:
        mov    bl, byte ptr [eax]
        cmp    bl, 20h
        je     SHORT loc_2_2AB9
        inc    edx
        inc    eax
        db  088h, 09Ah, 003h, 022h, 004h, 000h
        cmp    edx, 8
        jge    SHORT loc_2_2AB9
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_2AA0
loc_2_2AB9:
        push   0ah
        db  068h, 000h, 022h, 004h, 000h
        xor    bh, bh
        push   0
        db  088h, 0BAh, 004h, 022h, 004h, 000h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2AE1
        db  0A1h, 0C4h, 004h, 004h, 000h
        db  0A3h, 00Dh, 022h, 004h, 000h
loc_2_2AE1:
        db  08Bh, 00Dh, 000h, 022h, 004h, 000h
        test   ecx, ecx
        je     SHORT loc_2_2B2D
        mov    edx, dword ptr [ecx + 14h]
        xor    eax, eax
loc_2_2AF0:
        mov    bl, byte ptr [edx]
        db  088h, 098h, 00Dh, 022h, 004h, 000h
        test   bl, bl
        je     SHORT loc_2_2B03
        inc    eax
        inc    edx
        cmp    eax, 40h
        jb     SHORT loc_2_2AF0
loc_2_2B03:
        db  0A1h, 000h, 022h, 004h, 000h
        test   byte ptr [eax + 4], 1
        je     SHORT loc_2_2B18
        db  0C7h, 005h, 050h, 022h, 004h, 000h, 001h, 000h, 000h, 000h
loc_2_2B18:
        db  0A1h, 000h, 022h, 004h, 000h
        test   byte ptr [eax + 4], 4
        je     SHORT loc_2_2B2D
        db  0C7h, 005h, 054h, 022h, 004h, 000h, 001h, 000h, 000h, 000h
loc_2_2B2D:
        db  0A1h, 0B4h, 022h, 004h, 000h
        push   eax
        call   sub_2_32C0
        add    esp, 4
loc_2_2B3B:
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 053h, 083h, 03Dh, 0FCh, 021h, 004h, 000h, 000h, 074h, 00Dh, 031h, 0DBh, 0E8h
        db  08Fh, 008h, 000h, 000h, 089h, 01Dh, 0FCh, 021h, 004h, 000h, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h
        db  000h, 000h, 090h
sub_2_2B60:
        push   ebp
        mov    ebp, esp
        call   sub_2_2A70
        db  0A1h, 0FCh, 035h, 002h, 000h
        push   8
        add    eax, 0ch
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_0040
        add    esp, 0ch
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Dh, 040h, 000h
sub_2_2B90:
        db  08Bh, 015h, 000h, 022h, 004h, 000h
        test   edx, edx
        jne    SHORT loc_2_2B9D
        xor    eax, eax
        ret
loc_2_2B9D:
        mov    ax, word ptr [edx + 2]
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h, 055h, 089h
        db  0E5h, 083h, 0ECh, 004h, 08Dh, 045h, 0FCh, 050h, 031h, 0D2h, 068h, 001h, 007h, 000h, 000h, 089h
        db  055h, 0FCh, 0FFh, 015h, 0F4h, 004h, 004h, 000h, 08Bh, 045h, 0FCh, 083h, 0C4h, 008h, 089h, 0ECh
        db  05Dh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2BE0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        add    ebx, 3
        shr    ebx, 2
        lea    ebx, [ebx*4]
        call   sub_2_2A70
        test   ebx, ebx
        jne    SHORT loc_2_2C09
        xor    eax, eax
        jmp    NEAR PTR loc_2_2C99
loc_2_2C09:
        lea    eax, [ebp - 4]
        push   eax
        add    ebx, 8
        push   ebx
        push   400h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2C2C
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_2_2C2C:
        db  083h, 03Dh, 09Ch, 022h, 004h, 000h, 000h
        jne    SHORT loc_2_2C64
        push   eax
        db  068h, 098h, 022h, 004h, 000h
        push   4
        push   1
        db  068h, 0C8h, 004h, 004h, 000h
        call   sub_2_3250
        add    esp, 14h
        push   0
        db  068h, 09Ch, 022h, 004h, 000h
        push   4
        push   1
        db  068h, 0D4h, 004h, 004h, 000h
        call   sub_2_3250
        add    esp, 14h
loc_2_2C64:
        mov    esi, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 098h, 022h, 004h, 000h
        db  08Bh, 03Dh, 09Ch, 022h, 004h, 000h
        push   esi
        inc    edi
        add    ecx, ebx
        db  068h, 0A0h, 022h, 004h, 000h
        db  089h, 00Dh, 098h, 022h, 004h, 000h
        db  089h, 03Dh, 09Ch, 022h, 004h, 000h
        call   sub_2_3490
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_2C99:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  055h, 089h, 0E5h, 08Bh, 055h, 008h, 083h, 0EAh, 008h, 052h, 089h, 055h, 008h, 0E8h, 0FEh, 007h
        db  000h, 000h, 083h, 0C4h, 004h, 08Dh, 045h, 008h, 050h, 068h, 001h, 004h, 000h, 000h, 0FFh, 015h
        db  0F4h, 004h, 004h, 000h, 083h, 0C4h, 008h, 05Dh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 090h
        db  053h, 056h, 057h, 055h, 089h, 0E5h, 083h, 0ECh, 008h, 08Bh, 07Dh, 014h, 08Bh, 05Dh, 018h, 08Bh
        db  045h, 01Ch, 089h, 0DEh, 083h, 0C0h, 00Ch, 00Fh, 0AFh, 0F0h, 0C7h, 047h, 028h, 000h, 000h, 000h
        db  000h, 0C7h, 047h, 02Ch, 000h, 000h, 000h, 000h, 0C7h, 047h, 00Ch, 000h, 000h, 000h, 000h, 0C7h
        db  047h, 034h, 000h, 000h, 000h, 000h, 089h, 05Fh, 020h, 089h, 05Fh, 024h, 089h, 047h, 01Ch, 08Bh
        db  045h, 020h, 089h, 077h, 018h, 089h, 047h, 008h, 08Dh, 047h, 010h, 08Bh, 057h, 018h, 089h, 047h
        db  014h, 089h, 045h, 0F8h, 089h, 047h, 010h, 085h, 0D2h, 074h, 059h, 052h, 0E8h, 0AFh, 0FEh, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 089h, 047h, 00Ch, 085h, 0C0h, 075h, 007h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0EBh, 044h, 089h, 0C6h, 085h, 0DBh, 074h, 01Bh, 08Bh, 045h, 0F8h, 089h, 045h, 0FCh, 056h, 08Bh
        db  045h, 0FCh, 050h, 0E8h, 038h, 007h, 000h, 000h, 08Bh, 057h, 01Ch, 083h, 0C4h, 008h, 001h, 0D6h
        db  04Bh, 075h, 0EBh, 057h, 068h, 058h, 022h, 004h, 000h, 0E8h, 022h, 007h, 000h, 000h, 083h, 0C4h
        db  008h, 06Ah, 000h, 057h, 06Ah, 038h, 06Ah, 002h, 08Bh, 05Fh, 008h, 053h, 0E8h, 0CFh, 004h, 000h
        db  000h, 083h, 0C4h, 014h, 031h, 0C0h, 089h, 0ECh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 040h, 000h
        db  053h, 056h, 055h, 089h, 0E5h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 089h, 0C6h, 089h
        db  0C2h, 08Bh, 045h, 010h, 08Bh, 058h, 0FCh, 08Bh, 048h, 0F4h, 083h, 0E8h, 00Ch, 085h, 0C9h, 075h
        db  006h, 083h, 078h, 004h, 000h, 074h, 004h, 089h, 0D0h, 0EBh, 016h, 050h, 08Dh, 043h, 010h, 050h
        db  0E8h, 0CBh, 006h, 000h, 000h, 08Bh, 043h, 024h, 040h, 083h, 0C4h, 008h, 089h, 043h, 024h, 089h
        db  0F0h, 09Ch, 009h, 004h, 024h, 09Dh, 05Dh, 05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
        db  053h, 056h, 055h, 089h, 0E5h, 08Bh, 04Dh, 010h, 08Bh, 05Dh, 014h, 08Bh, 051h, 00Ch, 039h, 0D3h
        db  072h, 00Bh, 089h, 0D0h, 08Bh, 051h, 018h, 001h, 0C2h, 039h, 0D3h, 072h, 006h, 031h, 0C0h, 05Dh
        db  05Eh, 05Bh, 0C3h, 089h, 0DAh, 029h, 0C2h, 08Bh, 071h, 01Ch, 089h, 0D0h, 031h, 0D2h, 0F7h, 0F6h
        db  089h, 0D8h, 029h, 0D0h, 083h, 0C0h, 00Ch, 05Dh, 05Eh, 05Bh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h
        db  053h, 056h, 057h, 055h, 089h, 0E5h, 08Bh, 05Dh, 014h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h
        db  000h, 089h, 0C6h, 08Bh, 053h, 030h, 08Dh, 043h, 010h, 042h, 050h, 089h, 053h, 030h, 0E8h, 00Dh
        db  006h, 000h, 000h, 083h, 0C4h, 004h, 085h, 0C0h, 075h, 011h, 0FFh, 043h, 02Ch, 089h, 0F0h, 09Ch
        db  009h, 004h, 024h, 09Dh, 031h, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 0C7h, 000h, 000h, 000h, 000h
        db  000h, 0C7h, 040h, 004h, 000h, 000h, 000h, 000h, 089h, 058h, 008h, 08Bh, 04Bh, 024h, 08Dh, 078h
        db  00Ch, 049h, 08Bh, 043h, 020h, 089h, 04Bh, 024h, 029h, 0C8h, 03Bh, 043h, 028h, 076h, 003h, 089h
        db  043h, 028h, 089h, 0F0h, 09Ch, 009h, 004h, 024h, 09Dh, 089h, 0F8h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h
        db  053h, 0B8h, 058h, 022h, 004h, 000h, 03Bh, 005h, 058h, 022h, 004h, 000h, 074h, 01Ah, 08Bh, 000h
        db  0C7h, 040h, 028h, 000h, 000h, 000h, 000h, 08Bh, 018h, 0C7h, 040h, 02Ch, 000h, 000h, 000h, 000h
        db  081h, 0FBh, 058h, 022h, 004h, 000h, 075h, 0E6h, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
        db  056h, 055h, 089h, 0E5h, 083h, 03Dh, 0A8h, 022h, 004h, 000h, 000h, 074h, 005h, 031h, 0C0h, 05Dh
        db  05Eh, 0C3h, 0C7h, 005h, 0A8h, 022h, 004h, 000h, 001h, 000h, 000h, 000h, 0E8h, 08Fh, 0FBh, 0FFh
        db  0FFh, 08Bh, 04Dh, 010h, 068h, 0E0h, 004h, 004h, 000h, 083h, 0F9h, 034h, 076h, 004h, 089h, 0C8h
        db  0EBh, 005h, 0B8h, 034h, 000h, 000h, 000h, 050h, 08Bh, 075h, 00Ch, 056h, 068h, 060h, 022h, 004h
        db  000h, 0E8h, 0CAh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 010h, 085h, 0C0h, 074h, 005h, 0B8h, 0FFh, 0FFh
        db  0FFh, 0FFh, 05Dh, 05Eh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
        db  053h, 056h, 083h, 03Dh, 0A8h, 022h, 004h, 000h, 000h, 074h, 040h, 08Bh, 01Dh, 0A0h, 022h, 004h
        db  000h, 081h, 0FBh, 0A0h, 022h, 004h, 000h, 074h, 00Eh, 08Dh, 043h, 008h, 050h, 0E8h, 05Eh, 0FDh
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0E4h, 0B9h, 058h, 022h, 004h, 000h, 0BEh, 058h, 022h, 004h
        db  000h, 089h, 01Dh, 0A4h, 022h, 004h, 000h, 031h, 0D2h, 089h, 00Dh, 05Ch, 022h, 004h, 000h, 089h
        db  035h, 058h, 022h, 004h, 000h, 089h, 015h, 0A8h, 022h, 004h, 000h, 05Eh, 05Bh, 0C3h, 000h, 000h
sub_2_2F70:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        push   600h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_2F9F
        mov    eax, 0ffffffffh
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret
loc_2_2F9F:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_2FB0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        push   edx
        push   604h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 010h
        db  052h, 08Bh, 05Dh, 014h, 053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 001h, 006h, 000h, 000h, 0FFh, 015h
        db  0F4h, 004h, 004h, 000h, 083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 010h
        db  052h, 08Bh, 05Dh, 014h, 053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 001h, 006h, 000h, 000h, 0FFh, 015h
        db  0F4h, 004h, 004h, 000h, 083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 014h
        db  052h, 08Bh, 05Dh, 010h, 053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 002h, 006h, 000h, 000h, 0FFh, 015h
        db  0F4h, 004h, 004h, 000h, 083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Bh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3060:
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        call   sub_2_2A70
        push   3
        db  068h, 0E4h, 004h, 004h, 000h
        mov    edx, 1
        db  068h, 004h, 022h, 004h, 000h
        mov    dword ptr [ebp - 14h], edx
        call   sub_2_016D
        add    esp, 0ch
        cmp    eax, 1
        jne    SHORT loc_2_3096
        db  0C7h, 045h, 0F0h, 007h, 022h, 004h, 000h
        jmp    SHORT loc_2_309D
loc_2_3096:
        db  0C7h, 045h, 0F0h, 004h, 022h, 004h, 000h
loc_2_309D:
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 035h, 050h, 022h, 004h, 000h
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [ebp - 0ah], dx
        mov    dword ptr [ebp - 4], eax
        test   esi, esi
        je     SHORT loc_2_30CF
        push   eax
        push   300h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
loc_2_30CF:
        lea    eax, [ebp - 14h]
        push   eax
        push   200h
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        mov    esp, ebp
        pop    ebp
        pop    esi
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_30F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 0c8h
        sub    ebp, 72h
        lea    eax, [ebp + 8ah]
        push   eax
        mov    edx, dword ptr [ebp + 86h]
        push   edx
        lea    eax, [ebp - 56h]
        push   eax
        call   sub_2_1350
        add    esp, 0ch
        lea    eax, [ebp - 56h]
        push   eax
        mov    ebx, dword ptr [ebp + 82h]
        push   ebx
        mov    ecx, dword ptr [ebp + 7eh]
        push   ecx
        call   sub_2_3060
        add    esp, 0ch
        lea    esp, [ebp + 72h]
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h
        db  081h, 0ECh, 0C8h, 000h, 000h, 000h, 083h, 0EDh, 076h, 08Dh, 085h, 08Ah, 000h, 000h, 000h, 050h
        db  08Bh, 095h, 086h, 000h, 000h, 000h, 052h, 08Dh, 045h, 0AEh, 050h, 0E8h, 0ECh, 0E1h, 0FFh, 0FFh
        db  083h, 0C4h, 00Ch, 08Dh, 045h, 0AEh, 050h, 068h, 028h, 023h, 000h, 000h, 08Bh, 09Dh, 082h, 000h
        db  000h, 000h, 053h, 0E8h, 0E4h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Dh, 065h, 076h, 05Dh, 05Bh
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3190:
        push   ebx
        push   ebp
        mov    ebp, esp
        db  083h, 03Dh, 0ACh, 022h, 004h, 000h, 000h
        jne    SHORT loc_2_31A5
        mov    eax, 0bh
        pop    ebp
        pop    ebx
        ret
loc_2_31A5:
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        db  0FFh, 015h, 0ACh, 022h, 004h, 000h
        add    esp, 4
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_31C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   2ah
        push   0
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_2_00D1
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   9
        mov    dword ptr [ebp - 20h], eax
        db  068h, 004h, 022h, 004h, 000h
        lea    eax, [ebp - 1ch]
        mov    edx, 3
        push   eax
        mov    word ptr [ebp - 2ch], dx
        call   sub_2_2710
        add    esp, 0ch
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_2_3190
        add    esp, 4
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h, 055h, 089h, 0E5h, 083h, 0ECh, 02Ch, 06Ah, 02Ah
        db  06Ah, 000h, 08Dh, 045h, 0D4h, 050h, 0E8h, 0AEh, 0CEh, 0FFh, 0FFh, 0BAh, 008h, 000h, 000h, 000h
        db  08Dh, 045h, 0D4h, 083h, 0C4h, 00Ch, 066h, 089h, 055h, 0D4h, 0BAh, 058h, 022h, 004h, 000h, 050h
        db  089h, 055h, 0D8h, 0E8h, 050h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 089h, 0ECh, 05Dh, 0C3h, 08Dh
        db  080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_3250:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   2ah
        push   0
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_2_00D1
        add    esp, 0ch
        push   9
        db  068h, 004h, 022h, 004h, 000h
        lea    eax, [ebp - 1ch]
        mov    edx, 4
        push   eax
        mov    word ptr [ebp - 2ch], dx
        call   sub_2_2710
        add    esp, 0ch
        push   11h
        mov    edx, dword ptr [ebp + 8]
        push   edx
        lea    eax, [ebp - 13h]
        push   eax
        call   sub_2_2710
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 22h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 18h]
        add    esp, 0ch
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_2_3190
        add    esp, 4
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0C0h
sub_2_32C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        db  083h, 03Dh, 0B0h, 022h, 004h, 000h, 000h
        jne    SHORT loc_2_3342
        push   8
        mov    eax, dword ptr [ebp + 0ch]
        db  068h, 0E8h, 004h, 004h, 000h
        db  0A3h, 0B4h, 022h, 004h, 000h
        lea    eax, [ebp - 24h]
        mov    ebx, 1
        push   eax
        db  089h, 01Dh, 0B0h, 022h, 004h, 000h
        call   sub_2_0040
        add    esp, 0ch
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_2_2B60
        db  0A1h, 00Ch, 036h, 002h, 000h
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 24h]
        xor    ecx, ecx
        push   eax
        mov    dword ptr [ebp - 14h], ecx
        mov    dword ptr [ebp - 10h], ecx
        push   101h
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 4], ecx
        db  0FFh, 015h, 0F4h, 004h, 004h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_3342
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        db  0A3h, 0ACh, 022h, 004h, 000h
        call   sub_2_31C0
        add    esp, 4
loc_2_3342:
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 055h, 089h, 0E5h, 083h, 0ECh, 02Ch, 083h
        db  03Dh, 0B0h, 022h, 004h, 000h, 000h, 074h, 038h, 06Ah, 02Ah, 06Ah, 000h, 08Dh, 045h, 0D4h, 050h
        db  0E8h, 065h, 0CDh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 06Ah, 009h, 068h, 004h, 022h, 004h, 000h, 08Dh
        db  045h, 0E4h, 0BAh, 009h, 000h, 000h, 000h, 050h, 066h, 089h, 055h, 0D4h, 0E8h, 088h, 0F3h, 0FFh
        db  0FFh, 083h, 0C4h, 00Ch, 08Dh, 045h, 0D4h, 050h, 0E8h, 0FCh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  089h, 0ECh, 05Dh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h, 053h, 083h, 03Dh, 0B0h, 022h, 004h, 000h
        db  000h, 074h, 017h, 08Bh, 00Dh, 0B4h, 022h, 004h, 000h, 031h, 0DBh, 051h, 089h, 01Dh, 0B0h, 022h
        db  004h, 000h, 0E8h, 002h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 05Bh, 0C3h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_33D0:
        xor    edx, edx
        db  089h, 015h, 0C0h, 022h, 004h, 000h
        ret
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h, 0C7h, 005h, 0C0h, 022h, 004h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 05Dh, 010h, 08Bh
        db  075h, 014h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 053h, 004h, 089h, 032h, 089h
        db  01Eh, 08Bh, 053h, 004h, 089h, 056h, 004h, 089h, 073h, 004h, 09Ch, 009h, 004h, 024h, 09Dh, 05Dh
        db  05Eh, 05Bh, 0C3h, 08Dh, 044h, 020h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 05Dh, 00Ch, 09Ch, 0FAh
        db  058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 04Bh, 004h, 08Bh, 013h, 089h, 011h, 08Bh, 05Bh, 004h
        db  089h, 05Ah, 004h, 09Ch, 009h, 004h, 024h, 09Dh, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h
        db  000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 05Dh, 00Ch, 09Ch, 0FAh
        db  058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 013h, 039h, 0D3h, 075h, 00Ah, 09Ch, 009h, 004h, 024h
        db  09Dh, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Bh, 04Ah, 004h, 089h, 0D3h, 08Bh, 012h, 089h, 011h, 08Bh
        db  00Bh, 08Bh, 053h, 004h, 089h, 051h, 004h, 09Ch, 009h, 004h, 024h, 09Dh, 089h, 0D8h, 05Dh, 05Bh
        db  0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_3490:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx], eax
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [eax + 4], edx
        pop    ebp
        pop    ebx
        ret
        db  08Dh, 040h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 058h, 004h, 08Bh, 010h, 089h
        db  013h, 08Bh, 040h, 004h, 089h, 042h, 004h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
        db  08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 010h, 039h, 0D0h, 075h, 005h
        db  031h, 0D0h, 05Dh, 05Bh, 0C3h, 08Bh, 05Ah, 004h, 089h, 0D0h, 08Bh, 012h, 089h, 013h, 08Bh, 018h
        db  08Bh, 050h, 004h, 089h, 053h, 004h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh
        db  054h, 022h, 000h, 053h, 056h, 057h, 055h, 089h, 0E5h, 08Bh, 075h, 014h, 08Bh, 07Dh, 01Ch, 08Bh
        db  006h, 039h, 0F0h, 074h, 018h, 08Bh, 055h, 020h, 052h, 057h, 050h, 08Bh, 018h, 0FFh, 055h, 018h
        db  083h, 0C4h, 00Ch, 085h, 0C0h, 075h, 008h, 089h, 0D8h, 039h, 0F3h, 075h, 0E8h, 031h, 0C0h, 05Dh
        db  05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
        db  08Dh, 040h, 000h, 053h, 056h, 057h, 055h, 089h, 0E5h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h
        db  000h, 08Bh, 055h, 020h, 052h, 08Bh, 04Dh, 01Ch, 051h, 08Bh, 075h, 018h, 056h, 08Bh, 07Dh, 014h
        db  057h, 089h, 0C3h, 0E8h, 09Bh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 089h, 0C6h, 089h, 0D8h, 09Ch
        db  009h, 004h, 024h, 09Dh, 089h, 0F0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h
        db  000h, 08Bh, 0D2h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 05Dh, 010h, 089h, 0D8h, 08Bh, 00Bh, 031h
        db  0D2h, 039h, 0CBh, 074h, 009h, 08Bh, 000h, 08Bh, 030h, 042h, 039h, 0F3h, 075h, 0F7h, 089h, 0D0h
        db  05Dh, 05Eh, 05Bh, 0C3h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 0B4h, 033h, 003h, 00Ah, 000h, 000h, 044h, 045h, 043h, 04Ch
        db  049h, 043h, 04Ch, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
