; vcomm_pcode_04.asm -- VCOMM object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VCOMM_Get_Version:
        mov    eax, 100h
        clc
        ret
_VCOMM_Register_Port_Driver:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        jecxz  loc_4_003F
        push   esi
        push   edi
        push   ebx
        push   ecx
        call   sub_4_1505
        pop    ecx
        db  08Bh, 015h, 0B4h, 000h, 000h, 000h
        lea    edx, [edx + 0ch]
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        je     SHORT loc_4_0030
        lea    edx, [edx + 9]
loc_4_0030:
        push   esi
        push   ebx
        push   edi
        push   edx
        push   eax
        push   0
        call   ecx
        add    esp, 18h
        pop    ebx
        pop    edi
        pop    esi
loc_4_003F:
        leave
        ret
_VCOMM_Add_Port:
        push   esi
        xor    eax, eax
        cmp    dword ptr [esp + 0ch], eax
        je     SHORT loc_4_0092
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_4_006E
        mov    ecx, 8
        mov    eax, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        jb     SHORT loc_4_0092
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_4_006E:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_0092
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_0092
        mov    dword ptr [ecx + 4], eax
loc_4_0092:
        pop    esi
        ret
_VCOMM_Acquire_Port:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     NEAR PTR loc_4_012D
        test   byte ptr [ebp + 14h], 2
        jne    SHORT loc_4_00AF
        cmp    eax, 4
        ja     SHORT loc_4_012D
loc_4_00AF:
        db  081h, 03Dh, 004h, 000h, 000h, 000h, 000h, 004h, 000h, 000h
        jb     SHORT loc_4_012D
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 3bh
        mov    ebx, dword ptr [ebp + 10h]
        cmp    ebx, -1
        je     SHORT loc_4_0131
        mov    ecx, dword ptr [ebp + 14h]
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        jae    SHORT loc_4_012D
        jne    SHORT loc_4_012A
        push   eax
        test   byte ptr [ebp + 14h], 2
        jne    SHORT loc_4_00F5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   0c0001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        jmp    SHORT loc_4_0109
loc_4_00F5:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        push   0c0003h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
loc_4_0109:
        xchg   ecx, eax
        call   sub_4_0142
        add    esp, dword ptr [esp]
        cmp    eax, 6
        pop    eax
        jne    SHORT loc_4_0126
        mov    ecx, dword ptr [ebp + 14h]
        or     ecx, 1
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        jae    SHORT loc_4_012D
loc_4_0126:
        xor    eax, eax
        jmp    SHORT loc_4_012D
loc_4_012A:
        mov    eax, dword ptr [ebp + 0ch]
loc_4_012D:
        pop    edi
        pop    esi
        leave
        ret
loc_4_0131:
        mov    ecx, dword ptr [ebp + 14h]
        mov    edi, esi
        db  0CDh, 020h, 004h, 000h, 00Eh, 000h
        jae    SHORT loc_4_012D
        jne    SHORT loc_4_012A
        jmp    SHORT loc_4_0126
sub_4_0142:
        pushad
        db  00Fh, 0B7h, 03Dh, 00Eh, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        xor    esi, esi
        mov    eax, 34h
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
_VCOMM_Release_Port:
        push   ebp
        mov    ebp, esp
        push   ebx
        db  081h, 03Dh, 004h, 000h, 000h, 000h, 000h, 004h, 000h, 000h
        jb     SHORT loc_4_018B
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0ffffh
        jb     SHORT loc_4_018B
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, -1
        je     SHORT loc_4_018E
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
loc_4_018B:
        pop    ebx
        leave
        ret
loc_4_018E:
        db  0CDh, 020h, 005h, 000h, 00Eh, 000h
        jmp    SHORT loc_4_018B
_VCOMM_Steal_Port:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 008h, 000h, 00Eh, 000h
        pop    ebx
        pop    esi
        leave
        ret
        db  000h
VCOMM_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 0dh
        ja     SHORT loc_4_01BC
        db  0FFh, 024h, 085h, 048h, 000h, 000h, 000h
loc_4_01BC:
        mov    word ptr [ebp + 1ch], 0
        ret
loc_4_01C3:
        movzx  eax, word ptr [ebp + 10h]
        db  08Dh, 03Dh, 080h, 000h, 000h, 000h
        test   al, 80h
        je     SHORT loc_4_01D9
        db  08Dh, 03Dh, 087h, 000h, 000h, 000h
        and    al, 7fh
loc_4_01D9:
        inc    al
        mov    esi, edi
        add    edi, 3
        cmp    al, 9
        ja     SHORT loc_4_01E9
        add    al, 30h
        stosb
        jmp    SHORT loc_4_0201
loc_4_01E9:
        cmp    al, 63h
        ja     SHORT loc_4_01F9
loc_4_01ED:
        mov    cl, 0ah
        div    cl
        add    ax, 3030h
        stosw
        jmp    SHORT loc_4_0201
loc_4_01F9:
        mov    byte ptr [edi], 31h
        inc    edi
        sub    al, 64h
        jmp    SHORT loc_4_01ED
loc_4_0201:
        mov    byte ptr [edi], 0
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebx
        push   esi
        db  0CDh, 020h, 004h, 000h, 02Bh, 000h
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_4_0221:
        db  0BFh, 00Ch, 000h, 000h, 000h
        xchg   esi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    dword ptr [edi], 30h
        movzx  eax, word ptr [esi + 1]
        mov    dword ptr [edi + 4], eax
        movzx  eax, word ptr [esi + 0ch]
        mov    dword ptr [edi + 8], eax
        movzx  eax, word ptr [esi + 10h]
        mov    dword ptr [edi + 0ch], eax
        movzx  eax, word ptr [esi + 12h]
        mov    dword ptr [edi + 10h], eax
        mov    al, byte ptr [esi + 3]
        mov    byte ptr [edi + 16h], al
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 17h], al
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [edi + 18h], al
        mov    al, byte ptr [esi + 0eh]
        mov    byte ptr [edi + 19h], al
        mov    al, byte ptr [esi + 0fh]
        mov    byte ptr [edi + 1ah], al
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 1bh], al
        mov    al, byte ptr [esi + 15h]
        mov    byte ptr [edi + 1ch], al
        mov    al, byte ptr [esi + 16h]
        mov    byte ptr [edi + 1dh], al
        movzx  eax, word ptr [esi + 17h]
        mov    dword ptr [edi + 2ch], eax
        movzx  eax, word ptr [esi + 6]
        mov    dword ptr [edi + 20h], eax
        movzx  eax, word ptr [esi + 8]
        mov    dword ptr [edi + 24h], eax
        movzx  eax, word ptr [esi + 0ah]
        mov    dword ptr [edi + 28h], eax
        mov    esi, dword ptr [ebp + 10h]
        push   -1
        push   edi
        push   esi
        db  0CDh, 020h, 005h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_02B8:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi + 6]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 1ch], ax
        xchg   edi, eax
        mov    ax, 1c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_4_0322
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 1ch], ax
        xchg   edx, eax
        mov    ax, 1c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        cmp    edx, -1
        je     SHORT loc_4_0322
        movzx  ecx, word ptr [esi + 0ah]
        movzx  eax, word ptr [esi + 10h]
        mov    esi, dword ptr [esi + 2]
        push   eax
        push   edx
        push   ecx
        push   edi
        push   esi
        db  0CDh, 020h, 007h, 000h, 02Bh, 000h
        add    esp, 14h
        mov    word ptr [ebp + 1ch], 0ffffh
        ret
loc_4_0322:
        mov    word ptr [ebp + 1ch], 0
        ret
loc_4_0329:
        mov    esi, dword ptr [ebp + 10h]
        movzx  eax, byte ptr [ebp + 18h]
        push   eax
        push   esi
        db  0CDh, 020h, 008h, 000h, 02Bh, 000h
        add    esp, 8
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_0343:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        db  0CDh, 020h, 009h, 000h, 02Bh, 000h
        add    esp, 4
        ret
loc_4_0351:
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 1ch]
        lea    ebx, [ebp + 1ch]
        push   ebx
        db  068h, 03Ch, 000h, 000h, 000h
        push   esi
        db  0CDh, 020h, 00Bh, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    di, word ptr [ebp + 1ch]
        mov    dword ptr [ebp + 1ch], edi
        mov    di, word ptr [ebp + 38h]
        or     di, word ptr [ebp + 14h]
        je     SHORT loc_4_03A3
        xchg   edi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_4_03A3
        db  0BEh, 03Ch, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    byte ptr [edi], al
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [edi + 1], ax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [edi + 3], ax
loc_4_03A3:
        ret
loc_4_03A4:
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 1ch]
        lea    edx, [ebp + 1ch]
        push   edx
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebp + 1ch]
        mov    di, ax
        mov    dword ptr [ebp + 1ch], edi
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_4_03D0:
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   esi
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_03EA:
        mov    esi, dword ptr [ebp + 10h]
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 18h]
        push   eax
        push   ecx
        push   esi
        db  0CDh, 020h, 010h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_040F:
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        push   ecx
        push   esi
        db  0CDh, 020h, 011h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_0434:
        mov    esi, dword ptr [ebp + 10h]
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_4_0452
        push   eax
        push   esi
        call   dword ptr [edi + 1ch]
        add    esp, 8
loc_4_0452:
        ret
loc_4_0453:
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 1
        jne    SHORT loc_4_0464
        lea    eax, [ebp + 4]
        jmp    SHORT loc_4_046E
loc_4_0464:
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
loc_4_046E:
        xor    edi, edi
        xchg   dword ptr [ebp + 1ch], edi
        lea    ebx, [ebp + 1ch]
        push   ebx
        push   ecx
        push   eax
        push   esi
        db  0CDh, 020h, 012h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    di, word ptr [ebp + 1ch]
        mov    dword ptr [ebp + 1ch], edi
        ret
loc_4_048B:
        and    byte ptr [ebp + 2ch], 0bfh
        mov    esi, dword ptr [ebp + 10h]
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    ax, 414h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [edi], 0
        movzx  ecx, word ptr [ebp + 18h]
        push   edi
        push   ecx
        push   eax
        push   esi
        db  0CDh, 020h, 013h, 000h, 02Bh, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_04C7
        or     byte ptr [ebp + 2ch], 40h
loc_4_04C7:
        mov    eax, dword ptr [edi]
        mov    word ptr [ebp + 1ch], ax
        ret
loc_4_04CE:
        mov    esi, dword ptr [ebp + 10h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [esi + 2ch], ebx
        movzx  ecx, word ptr [ebp + 18h]
        mov    dword ptr [esi + 20h], ecx
        mov    edi, dword ptr [ebp]
        mov    dword ptr [esi + 14h], edi
        movzx  edx, word ptr [ebp + 14h]
        inc    dx
        dec    edx
        movzx  eax, word ptr [ebp + 4]
        inc    ax
        dec    eax
        xor    ebx, ebx
        jecxz  loc_4_04FE
        db  0BBh, 020h, 000h, 000h, 000h
loc_4_04FE:
        push   edx
        push   eax
        push   ecx
        push   ecx
        push   ebx
        push   esi
        db  0CDh, 020h, 014h, 000h, 02Bh, 000h
        add    esp, 0ch
        pop    ecx
        pop    eax
        push   ecx
        push   ecx
        push   ebx
        push   eax
        push   esi
        db  0CDh, 020h, 018h, 000h, 02Bh, 000h
        add    esp, 10h
        pop    ecx
        pop    edx
        push   ecx
        push   ebx
        push   edx
        push   esi
        db  0CDh, 020h, 017h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    word ptr [ebp + 1ch], ax
        ret
        db  000h, 000h, 000h
sub_4_0534:
        push   eax
        push   edi
        xor    eax, eax
loc_4_0538:
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        or     eax, eax
        je     SHORT loc_4_054B
        inc    edi
        inc    esi
        or     al, 20h
        or     ah, 20h
        cmp    al, ah
        je     SHORT loc_4_0538
loc_4_054B:
        pop    edi
        pop    eax
        ret
_VCOMM_OpenComm:
        enter  0ch, 0
        push   ebx
        push   esi
        push   edi
        db  08Bh, 00Dh, 090h, 000h, 000h, 000h
        jecxz  loc_4_0569
        push   1
        push   ecx
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
loc_4_0569:
        push   dword ptr [ebp + 8]
        call   sub_4_11E7
        pop    ecx
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp - 8], edx
        mov    edi, eax
        push   edi
        mov    al, 5ch
        xor    ebx, ebx
        db  08Bh, 035h, 0BCh, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], esi
        db  089h, 01Dh, 0BCh, 000h, 000h, 000h
loc_4_058E:
        cmp    byte ptr [edi], bl
        je     SHORT loc_4_059E
        scasb
        jne    SHORT loc_4_058E
        mov    byte ptr [edi - 1], bl
        db  089h, 03Dh, 0BCh, 000h, 000h, 000h
loc_4_059E:
        pop    edi
        push   dword ptr [ebp + 8]
        call   sub_4_1406
        pop    ecx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_4_05BA
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_4_05B8:
        mov    edx, eax
loc_4_05BA:
        mov    eax, 0ffffffffh
        je     NEAR PTR loc_4_0646
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx + 4]
        call   sub_4_0534
        pop    esi
        je     SHORT loc_4_05DE
        mov    eax, edx
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_4_05B8
loc_4_05DE:
        xor    edi, edi
        db  087h, 03Dh, 0B4h, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_0616
        push   edx
        test   byte ptr [edi + 1eh], 20h
        je     SHORT loc_4_0604
        or     byte ptr [edi + 1eh], 10h
        push   1
        push   1
        push   dword ptr [edi]
        db  0CDh, 020h, 064h, 000h, 033h, 000h
        add    esp, 0ch
loc_4_0604:
        pop    edx
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        jne    SHORT loc_4_0613
        lea    edi, [edi + 0ch]
        jmp    SHORT loc_4_0616
loc_4_0613:
        lea    edi, [edi + 15h]
loc_4_0616:
        lea    esi, [ebp - 4]
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   dword ptr [edx]
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_0636
        mov    ecx, edi
        jecxz  loc_4_0632
        call   sub_4_149D
loc_4_0632:
        mov    eax, dword ptr [esi]
        jmp    SHORT loc_4_0646
loc_4_0636:
        mov    dword ptr [eax + 64h], 534d5446h
        mov    word ptr [eax + 39h], 0
        mov    dword ptr [eax + 68h], edi
loc_4_0646:
        db  08Bh, 03Dh, 0BCh, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_0655
        dec    edi
        mov    bl, 5ch
        mov    byte ptr [edi], bl
loc_4_0655:
        mov    edi, dword ptr [ebp - 0ch]
        db  089h, 03Dh, 0BCh, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_4_127B
        pop    ecx
        db  08Bh, 00Dh, 090h, 000h, 000h, 000h
        jecxz  loc_4_067B
        push   eax
        push   ecx
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        pop    eax
loc_4_067B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_VCOMM_GetCommState:
        mov    cl, 1
        db  0E9h, 069h, 0FAh, 0FBh, 0FFh
_VCOMM_SetupComm:
        mov    cl, 2
        db  0E9h, 062h, 0FAh, 0FBh, 0FFh
_VCOMM_CloseComm:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        db  0E8h, 024h, 0FBh, 0FBh, 0FFh
        jne    NEAR PTR loc_4_072C
        or     byte ptr [edi + 39h], 10h
        push   3
        push   edi
        db  0E8h, 0F8h, 0FAh, 0FBh, 0FFh
        pop    ecx
        pop    ecx
        xor    esi, esi
        mov    dword ptr [edi + 64h], esi
        xchg   dword ptr [edi + 84h], esi
        or     esi, esi
        je     SHORT loc_4_06C2
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_06C2:
        xor    esi, esi
        xchg   dword ptr [edi + 80h], esi
        or     esi, esi
        je     SHORT loc_4_06D4
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_06D4:
        push   10h
        pop    ecx
        mov    edx, dword ptr [edi + 4]
        mov    edx, dword ptr [ecx + edx]
        mov    esi, dword ptr [edi + 68h]
        push   edi
        call   edx
        pop    ecx
        mov    ecx, esi
        or     eax, eax
        jne    SHORT loc_4_06F3
        mov    dword ptr [edi + 64h], 534d5446h
        jmp    SHORT loc_4_072C
loc_4_06F3:
        jecxz  loc_4_072C
        call   sub_4_149D
        push   eax
        mov    eax, 0fffffff4h
        cmp    dword ptr [ecx - 4], 5245454bh
        je     SHORT loc_4_070E
        mov    eax, 0ffffffebh
loc_4_070E:
        add    eax, ecx
        test   byte ptr [eax + 1eh], 20h
        je     SHORT loc_4_072B
        and    byte ptr [eax + 1eh], 0efh
        push   0
        push   eax
        db  068h, 02Fh, 007h, 000h, 000h
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_4_072B:
        pop    eax
loc_4_072C:
        pop    edi
        pop    esi
        ret
loc_4_072F:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 1eh], 10h
        jne    SHORT loc_4_0748
        push   0
        push   8
        push   dword ptr [eax]
        db  0CDh, 020h, 064h, 000h, 033h, 000h
        add    esp, 0ch
loc_4_0748:
        ret
_VCOMM_GetCommProperties:
        mov    cl, 8
        db  0E9h, 0A0h, 0F9h, 0FBh, 0FFh
_VCOMM_EnableCommNotification:
        mov    cl, 0fh
        db  0E9h, 099h, 0F9h, 0FBh, 0FFh
_VCOMM_SetReadCallBack:
        mov    cl, 10h
        db  0E9h, 092h, 0F9h, 0FBh, 0FFh
_VCOMM_SetWriteCallBack:
        mov    cl, 11h
        db  0E9h, 08Bh, 0F9h, 0FBh, 0FFh
_VCOMM_SetCommEventMask:
        mov    cl, 0bh
        db  0E9h, 084h, 0F9h, 0FBh, 0FFh
_VCOMM_GetSetCommTimeouts:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        lea    esi, [edx + 6ch]
        mov    ecx, 14h
        xor    eax, eax
        cmp    dword ptr [esp + 14h], eax
        je     SHORT loc_4_07A2
        and    byte ptr [edx + 39h], 0dfh
        dec    eax
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_4_07A0
        cmp    eax, dword ptr [edi + 4]
        jne    SHORT loc_4_07A0
        xor    eax, dword ptr [edi + 8]
        je     SHORT loc_4_07A0
        inc    eax
        je     SHORT loc_4_07A0
        or     byte ptr [edx + 39h], 20h
loc_4_07A0:
        xchg   edi, esi
loc_4_07A2:
        cld
        rep movsb
        or     al, 1
        pop    edi
        pop    esi
        ret
_VCOMM_SetWriteRequest:
        enter  10h, 0
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        db  0E8h, 0FDh, 0F9h, 0FBh, 0FFh
        jne    NEAR PTR loc_4_0878
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_4_082E
        mov    dword ptr [eax + 14h], edi
        mov    esi, dword ptr [edi + 84h]
        cmp    esi, ecx
        je     SHORT loc_4_07E5
        call   sub_4_0B0E
        jne    SHORT loc_4_0836
        je     SHORT loc_4_07FD
loc_4_07E5:
        mov    ecx, 34h
        mov    eax, 5
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_4_0871
        mov    dword ptr [edi + 84h], esi
loc_4_07FD:
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_4_087F
        jb     SHORT loc_4_085C
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        db  0CDh, 020h, 00Ah, 000h, 02Bh, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_0850
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_0843
        push   1
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_0850
loc_4_082E:
        xor    eax, eax
        mov    dword ptr [edi + 0ch], eax
        dec    eax
        jmp    SHORT loc_4_087A
loc_4_0836:
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_4_087F
        jb     SHORT loc_4_0871
        mov    dword ptr [ebp - 4], eax
loc_4_0843:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        db  0E8h, 0ABh, 0FBh, 0FBh, 0FFh
loc_4_0850:
        mov    dword ptr [edi + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_4_087A
loc_4_085C:
        call   sub_4_0B0E
        jne    SHORT loc_4_0871
        xor    esi, esi
        xchg   dword ptr [edi + 84h], esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_0871:
        mov    dword ptr [edi + 0ch], 0fffffffch
loc_4_0878:
        xor    eax, eax
loc_4_087A:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_4_087F:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_08AB
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        xchg   edi, eax
        xchg   ebx, esi
        mov    ecx, 0dh
        push   edi
        cld
        rep movsd
        pop    edi
        xchg   edi, eax
        mov    esi, ebx
        mov    dword ptr [eax + 30h], 444e4153h
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], ecx
        clc
loc_4_08AB:
        ret
_VCOMM_SetReadRequest:
        enter  10h, 0
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        db  0E8h, 0FBh, 0F8h, 0FBh, 0FFh
        jne    NEAR PTR loc_4_09CD
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_4_096D
        mov    dword ptr [eax + 14h], edi
        cmp    dword ptr [edi + 6ch], -1
        jne    SHORT loc_4_08E6
        mov    esi, dword ptr [edi + 70h]
        or     esi, dword ptr [edi + 74h]
        je     SHORT loc_4_0958
loc_4_08E6:
        mov    esi, dword ptr [edi + 80h]
        cmp    esi, ecx
        je     SHORT loc_4_08FD
        call   sub_4_0B0E
        jne    NEAR PTR loc_4_098C
        je     SHORT loc_4_0919
loc_4_08FD:
        mov    ecx, 34h
        mov    eax, 5
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_09C6
        mov    dword ptr [edi + 80h], esi
loc_4_0919:
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        db  0CDh, 020h, 00Ah, 000h, 02Bh, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_09A5
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 4]
        cmp    dword ptr [ebp - 0ch], ecx
        jae    SHORT loc_4_0958
        mov    ebx, dword ptr [ebp + 0ch]
        test   byte ptr [edi + 39h], 20h
        je     SHORT loc_4_094C
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_0958
        mov    dword ptr [ebx + 4], 1
loc_4_094C:
        call   sub_4_087F
        jb     SHORT loc_4_09B1
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_4_0974
loc_4_0958:
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [edx + 4]
        push   dword ptr [edx]
        push   edi
        db  0CDh, 020h, 013h, 000h, 02Bh, 000h
        add    esp, 10h
loc_4_096D:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_09A8
loc_4_0974:
        mov    edx, eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        db  0E8h, 07Bh, 0FAh, 0FBh, 0FFh
        push   0
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_09A5
loc_4_098C:
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_4_087F
        jb     SHORT loc_4_09C6
        mov    dword ptr [ebp - 4], eax
        mov    edx, eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        db  0E8h, 056h, 0FAh, 0FBh, 0FFh
loc_4_09A5:
        mov    eax, dword ptr [ebp - 4]
loc_4_09A8:
        mov    dword ptr [edi + 0ch], 0
        jmp    SHORT loc_4_09CF
loc_4_09B1:
        call   sub_4_0B0E
        jne    SHORT loc_4_09C6
        xor    esi, esi
        xchg   dword ptr [edi + 80h], esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_09C6:
        mov    dword ptr [edi + 0ch], 0fffffffch
loc_4_09CD:
        xor    eax, eax
loc_4_09CF:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_4_09D4:
        push   eax
        push   esi
        push   edi
        mov    edi, eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0A09
loc_4_09E1:
        xor    ecx, ecx
        inc    ecx
        cmp    esi, dword ptr [edi + 80h]
        je     SHORT loc_4_09EE
        mov    cl, 2
loc_4_09EE:
        mov    edx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        push   eax
        push   0
        push   ecx
        push   edx
        push   edi
        db  0E8h, 023h, 0F7h, 0FBh, 0FFh
        add    esp, 10h
        pop    eax
        or     eax, eax
        jne    SHORT loc_4_09E1
loc_4_0A09:
        pop    edi
        pop    esi
        pop    eax
        ret
loc_4_0A0D:
        enter  0ch, 0
        mov    edi, edx
        db  0E8h, 0A5h, 0F7h, 0FBh, 0FFh
        jne    NEAR PTR loc_4_0AA8
        push   0
        push   0
        push   -1
        push   edi
        db  0CDh, 020h, 018h, 000h, 02Bh, 000h
        add    esp, 10h
        lea    eax, [ebp - 0ch]
        cli
        push   eax
        push   edi
        db  0CDh, 020h, 00Ah, 000h, 02Bh, 000h
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_0A4F
        push   0
        push   edi
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
loc_4_0A4F:
        sti
        mov    esi, dword ptr [edi + 84h]
        or     esi, esi
        je     SHORT loc_4_0A79
        call   sub_4_0B1A
        je     SHORT loc_4_0A6F
        mov    edx, dword ptr [eax + 0ch]
        sub    edx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], edx
        call   sub_4_0AF9
loc_4_0A6F:
        push   1
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
loc_4_0A79:
        test   byte ptr [edi + 39h], 4
        je     SHORT loc_4_0AA8
        mov    esi, dword ptr [edi + 84h]
        or     esi, esi
        je     SHORT loc_4_0A91
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        jne    SHORT loc_4_0AA8
loc_4_0A91:
        and    byte ptr [edi + 39h], 0fbh
        push   dword ptr [edi + 90h]
        push   0
        push   0
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
loc_4_0AA8:
        leave
        ret
loc_4_0AAA:
        mov    edi, edx
        db  0E8h, 00Ch, 0F7h, 0FBh, 0FFh
        jne    SHORT loc_4_0AF8
        push   0
        push   0
        push   -1
        push   edi
        db  0CDh, 020h, 017h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    esi, dword ptr [edi + 80h]
        or     esi, esi
        je     SHORT loc_4_0AF8
        call   sub_4_0B1A
        je     SHORT loc_4_0AEE
        lea    edx, [eax + 0ch]
        push   eax
        push   edx
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   edi
        db  0CDh, 020h, 013h, 000h, 02Bh, 000h
        add    esp, 10h
        pop    eax
        call   sub_4_0AF9
loc_4_0AEE:
        push   0
        push   edi
        call   sub_4_0C07
        pop    ecx
        pop    ecx
loc_4_0AF8:
        ret
sub_4_0AF9:
        push   eax
        mov    edx, eax
        call   dword ptr [eax + 8]
        pop    eax
        push   eax
        call   sub_4_0C85
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        ret
sub_4_0B0E:
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        or     eax, eax
        ret
sub_4_0B1A:
        pushfd
        cli
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        popfd
        or     eax, eax
        ret
loc_4_0B26:
        mov    dword ptr [edx + 1ch], 0
        mov    edi, dword ptr [edx + 14h]
        db  0E8h, 088h, 0F6h, 0FBh, 0FFh
        jne    SHORT loc_4_0B62
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_4_0B51
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        sub    eax, dword ptr [edi + 88h]
        cmp    eax, dword ptr [edi + 6ch]
        ja     SHORT loc_4_0B6D
loc_4_0B51:
        mov    eax, dword ptr [edi + 6ch]
        db  0BEh, 026h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 1ch], esi
loc_4_0B62:
        ret
loc_4_0B63:
        mov    dword ptr [edx + 20h], 0
        mov    edi, dword ptr [edx + 14h]
loc_4_0B6D:
        mov    eax, 1
loc_4_0B72:
        db  0E8h, 046h, 0F6h, 0FBh, 0FFh
        jne    SHORT loc_4_0BB4
        or     byte ptr [edi + 39h], 2
        push   0
        push   eax
        push   edx
        push   edi
        db  0E8h, 09Dh, 0F5h, 0FBh, 0FFh
        add    esp, 10h
        ret
loc_4_0B8B:
        mov    dword ptr [edx + 20h], 0
        mov    edi, dword ptr [edx + 14h]
        mov    eax, 2
        jmp    SHORT loc_4_0B72
sub_4_0B9C:
        push   esi
        mov    edx, ebx
        mov    eax, dword ptr [edi + 6ch]
        or     eax, eax
        je     SHORT loc_4_0BB4
        db  0BEh, 026h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 1ch], esi
loc_4_0BB4:
        mov    eax, dword ptr [edi + 70h]
        imul   eax, dword ptr [ebx + 4]
        add    eax, dword ptr [edi + 74h]
        or     eax, eax
        je     SHORT loc_4_0BDB
        mov    edx, ebx
        db  0BEh, 063h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 20h], esi
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [ebx + 18h], eax
loc_4_0BDB:
        pop    esi
        ret
sub_4_0BDD:
        push   esi
        mov    eax, dword ptr [edi + 78h]
        imul   eax, dword ptr [ebx + 4]
        add    eax, dword ptr [edi + 7ch]
        or     eax, eax
        je     SHORT loc_4_0C05
        mov    edx, ebx
        db  0BEh, 08Bh, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 20h], esi
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [ebx + 18h], eax
loc_4_0C05:
        pop    esi
        ret
sub_4_0C07:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        db  0E8h, 0A8h, 0F5h, 0FBh, 0FFh
        jne    SHORT loc_4_0C80
        mov    esi, dword ptr [edi + 84h]
        cmp    dword ptr [ebp + 0ch], 1
        je     SHORT loc_4_0C29
        mov    esi, dword ptr [edi + 80h]
loc_4_0C29:
        or     esi, esi
        je     SHORT loc_4_0C80
        call   sub_4_0B0E
        je     SHORT loc_4_0C80
        mov    ebx, eax
        cmp    dword ptr [ebp + 0ch], 1
        je     SHORT loc_4_0C56
        call   sub_4_0B9C
        push   ebx
        db  068h, 024h, 001h, 000h, 000h
        push   dword ptr [ebx + 4]
        push   edi
        db  0CDh, 020h, 017h, 000h, 02Bh, 000h
        add    esp, 10h
        jmp    SHORT loc_4_0C80
loc_4_0C56:
        call   sub_4_0BDD
        push   ebx
        db  068h, 024h, 001h, 000h, 000h
        push   1
        push   edi
        db  0CDh, 020h, 018h, 000h, 02Bh, 000h
        add    esp, 10h
        lea    eax, [ebx + 0ch]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        push   edi
        db  0CDh, 020h, 012h, 000h, 02Bh, 000h
        add    esp, 10h
loc_4_0C80:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_4_0C85:
        btr    dword ptr [eax + 24h], 0
        jae    SHORT loc_4_0CAC
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        add    ecx, edx
        shr    edx, 0ch
        shr    ecx, 0ch
        sub    ecx, edx
        inc    ecx
        push   40000000h
        push   ecx
        push   edx
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_4_0CAC:
        ret
_VCOMM_Dequeue_Request:
        push   ebp
        mov    ebp, esp
        leave
        ret
        db  000h, 000h
loc_4_0CB4:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 044h, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    8
loc_4_0CC8:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 036h, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_4_0CDC:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 05Ah, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_4_0CF0:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 077h, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D04:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 07Fh, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D18:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 07Bh, 0F3h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D2C:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 032h, 0F4h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D40:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 047h, 0F4h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D54:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 047h, 0F4h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D68:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 0A6h, 0F4h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D7C:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 076h, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0D90:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 086h, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0DA4:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 08Bh, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0DB8:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 07Dh, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    18h
loc_4_0DCC:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 0BFh, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0DE0:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 0B1h, 0F5h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    20h
loc_4_0DF4:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 037h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    20h
loc_4_0E08:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 06Eh, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_4_0E1C:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 060h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_4_0E30:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 066h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_4_0E44:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 058h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0E58:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 045h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_4_0E6C:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 03Fh, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_4_0E80:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 033h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_4_0E94:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 027h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_4_0EA8:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 047h, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_4_0EBC:
        pop    ebx
        pop    esi
        pop    edi
        db  0E8h, 06Fh, 0F6h, 0FBh, 0FFh
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    28h
sub_4_0ED0:
        push   esi
        push   ebx
        push   edi
        db  0BEh, 0FDh, 002h, 000h, 000h
        db  0BBh, 001h, 003h, 000h, 000h
        mov    dword ptr [ebx], 0
        push   2
        push   dword ptr [esp + 14h]
        push   esi
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_0F13
        push   eax
        push   eax
        push   2
        push   dword ptr [esi]
        push   ebx
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_0F13
        mov    eax, dword ptr [ebx]
        movzx  eax, word ptr [eax + 4]
        jmp    SHORT loc_4_0F15
loc_4_0F13:
        xor    eax, eax
loc_4_0F15:
        pop    edi
        pop    ebx
        pop    esi
        ret
sub_4_0F19:
        push   0
        push   100h
        push   esi
        push   1
        push   eax
        push   0
        push   dword ptr [ebx]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        ret
sub_4_0F32:
        sub    esp, 34h
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 44h]
        mov    edx, dword ptr [edi]
        db  0C7h, 005h, 001h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        push   1
        db  068h, 001h, 003h, 000h, 000h
        db  068h, 049h, 003h, 000h, 000h
        push   3
        db  068h, 04Ah, 003h, 000h, 000h
        push   0
        push   edx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        mov    ebx, edi
        jne    NEAR PTR loc_4_109A
        mov    edx, dword ptr [edi]
        push   edx
        call   sub_4_0ED0
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_4_109A
        db  080h, 03Dh, 049h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_0FC0
        db  0BEh, 018h, 003h, 000h, 000h
        mov    edi, 408h
        push   3
        pop    ecx
        cld
        repne scasw
        jne    NEAR PTR loc_4_113A
        neg    ecx
        add    ecx, 33h
        mov    byte ptr [esi + 3], cl
        push   dword ptr [esp + 44h]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_101E
        jne    NEAR PTR loc_4_113A
loc_4_0FC0:
        call   sub_4_16E6
        jb     SHORT loc_4_0FCB
        or     byte ptr [edi + 1eh], 20h
loc_4_0FCB:
        db  0BEh, 012h, 003h, 000h, 000h
        db  0BFh, 01Dh, 003h, 000h, 000h
        push   4
        pop    ecx
        cld
        repne scasw
        jne    SHORT loc_4_0FFB
        neg    ecx
        add    ecx, 34h
        mov    byte ptr [esi + 3], cl
        db  0BFh, 041h, 003h, 000h, 000h
        push   4
        pop    ecx
        repne scasw
        jne    SHORT loc_4_0FF9
        mov    word ptr [edi - 2], 0
loc_4_0FF9:
        jmp    SHORT loc_4_100A
loc_4_0FFB:
        cmp    eax, 3220h
        jne    NEAR PTR loc_4_10BE
        mov    byte ptr [esi + 3], 33h
loc_4_100A:
        push   dword ptr [esp + 44h]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    NEAR PTR loc_4_10FB
loc_4_101E:
        mov    ebx, dword ptr [esp + 44h]
        db  0B8h, 0CEh, 000h, 000h, 000h
        call   sub_4_0F19
        db  080h, 03Dh, 049h, 003h, 000h, 000h, 002h
        je     SHORT loc_4_109A
        mov    edi, esi
        db  0B8h, 014h, 001h, 000h, 000h
        mov    edx, dword ptr [ebx]
        call   sub_4_114D
        jne    SHORT loc_4_109A
        push   esi
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        push   edx
        push   eax
        push   edi
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        mov    ecx, eax
        inc    ecx
        pop    eax
        pop    edx
        add    eax, 4
        add    eax, ecx
        sub    eax, 100h
        jae    SHORT loc_4_109A
        mov    dword ptr [edx], 2820h
        add    edx, 2
        push   ecx
        push   edi
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    word ptr [edx], 29h
        db  0B8h, 01Fh, 001h, 000h, 000h
        call   sub_4_0F19
        db  0B8h, 0E2h, 000h, 000h, 000h
        call   sub_4_0F19
loc_4_109A:
        db  0B8h, 0FEh, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        call   sub_4_114D
        jne    NEAR PTR loc_4_113A
        push   0
        push   0
        push   esi
        push   dword ptr [ebx]
        db  0CDh, 020h, 02Fh, 000h, 033h, 000h
        add    esp, 10h
        jmp    SHORT loc_4_113A
loc_4_10BE:
        mov    edi, esi
        mov    ebx, dword ptr [esp + 44h]
        db  0B8h, 0CEh, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        call   sub_4_1149
        jne    SHORT loc_4_10F9
        lea    edi, [esp + 0ch]
        push   esi
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        inc    eax
        cmp    eax, 31h
        ja     SHORT loc_4_113A
        push   eax
        push   esi
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    esi, edi
        jmp    NEAR PTR loc_4_101E
loc_4_10F9:
        mov    esi, edi
loc_4_10FB:
        mov    bl, 35h
        xor    edi, edi
        mov    byte ptr [esi + 4], 0
loc_4_1103:
        mov    byte ptr [edi + esi + 3], bl
        push   dword ptr [esp + 44h]
        push   esi
        call   sub_4_1292
        pop    ecx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_4_101E
        inc    bl
        cmp    bl, 39h
        jbe    SHORT loc_4_1103
        mov    bl, 30h
        or     edi, edi
        jne    SHORT loc_4_112D
        mov    byte ptr [esi + 3], 30h
        inc    edi
loc_4_112D:
        inc    byte ptr [esi + 3]
        mov    byte ptr [esi + 5], 0
        cmp    byte ptr [esi + 3], 3ah
        jb     SHORT loc_4_1103
loc_4_113A:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 34h
        ret
sub_4_1141:
        cld
loc_4_1142:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_4_1142
        ret
sub_4_1149:
        xor    ecx, ecx
        jmp    SHORT loc_4_1150
sub_4_114D:
        xor    ecx, ecx
        inc    ecx
loc_4_1150:
        push   edx
        db  0C7h, 005h, 001h, 003h, 000h, 000h, 000h, 001h, 000h, 000h
        db  0BEh, 0A3h, 001h, 000h, 000h
        push   ecx
        db  068h, 001h, 003h, 000h, 000h
        push   esi
        push   1
        push   eax
        push   0
        push   edx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        pop    edx
        test   eax, eax
        ret
sub_4_117A:
        mov    edi, esi
        db  08Bh, 035h, 005h, 003h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_4_119B
        mov    ecx, 18h
        xor    eax, eax
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_4_11E4
        db  089h, 035h, 005h, 003h, 000h, 000h
loc_4_119B:
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_11B8
loc_4_11A3:
        push   esi
        mov    esi, dword ptr [eax]
        call   sub_4_0534
        pop    esi
        jne    SHORT loc_4_11B0
        jmp    SHORT loc_4_11E4
loc_4_11B0:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_4_11A3
loc_4_11B8:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, eax
        push   0
        push   0dh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [esi], eax
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 10h], ecx
        xchg   edi, esi
        xchg   edi, eax
        mov    cl, 0dh
        rep movsb
loc_4_11E4:
        mov    esi, eax
        ret
sub_4_11E7:
        enter  0ch, 0
        push   esi
        push   edi
        push   ebx
        lea    edi, [ebp - 4]
        push   edi
        db  068h, 06Ch, 001h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_1274
        push   1
        push   100h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_1268
        mov    esi, eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0ch], 100h
        lea    ebx, [ebp - 8]
        push   eax
        push   esi
        push   ebx
        push   0
        push   dword ptr [ebp + 8]
        push   dword ptr [edi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_125C
        cmp    dword ptr [ebx], 1
        jne    SHORT loc_4_125C
        push   dword ptr [edi]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    eax, esi
        mov    edx, esi
loc_4_1257:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_4_125C:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_1268:
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_4_1274:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        jmp    SHORT loc_4_1257
sub_4_127B:
        push   eax
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_4_1290
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_1290:
        pop    eax
        ret
sub_4_1292:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_4_11E7
        pop    ecx
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp - 8], edx
        mov    edi, eax
        mov    al, 5ch
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], ebx
loc_4_12B1:
        cmp    byte ptr [edi], bl
        je     SHORT loc_4_12BE
        scasb
        jne    SHORT loc_4_12B1
        dec    edi
        mov    byte ptr [edi], bl
        mov    dword ptr [ebp - 4], edi
loc_4_12BE:
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        xor    eax, eax
        test   esi, esi
        je     SHORT loc_4_132C
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_132C
loc_4_12D5:
        mov    ebx, eax
        cmp    ebx, dword ptr [ebp + 0ch]
        je     SHORT loc_4_130A
        test   byte ptr [ebx + 1eh], 2
        jne    SHORT loc_4_130A
        mov    edx, dword ptr [ebx]
        db  0B8h, 0CEh, 000h, 000h, 000h
        call   sub_4_1149
        jne    SHORT loc_4_12F7
        call   sub_4_0534
        je     SHORT loc_4_1323
loc_4_12F7:
        db  0B8h, 0E2h, 000h, 000h, 000h
        call   sub_4_1149
        jne    SHORT loc_4_130A
        call   sub_4_0534
        je     SHORT loc_4_1323
loc_4_130A:
        mov    eax, ebx
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_4_12D5
        xor    eax, eax
        db  0A3h, 00Eh, 003h, 000h, 000h
        jmp    SHORT loc_4_132C
loc_4_1323:
        db  089h, 01Dh, 00Eh, 003h, 000h, 000h
        xor    eax, eax
        inc    eax
loc_4_132C:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_1337
        mov    bl, 5ch
        mov    byte ptr [edi], bl
loc_4_1337:
        push   dword ptr [ebp - 8]
        call   sub_4_127B
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_4_1345:
        push   esi
        push   edi
        push   eax
        push   ebx
        test   byte ptr [eax + 1eh], 2
        jne    NEAR PTR loc_4_1401
        db  083h, 025h, 009h, 003h, 000h, 000h, 001h
        mov    ebx, eax
        mov    edx, dword ptr [ebx]
        db  0B8h, 0CEh, 000h, 000h, 000h
        call   sub_4_1149
        jne    SHORT loc_4_1399
        call   sub_4_0534
        jne    SHORT loc_4_1399
        db  080h, 00Dh, 009h, 003h, 000h, 000h, 002h
        db  0B8h, 0D7h, 000h, 000h, 000h
        call   sub_4_114D
        je     SHORT loc_4_13D2
        db  080h, 00Dh, 009h, 003h, 000h, 000h, 004h
        db  0B8h, 0EFh, 000h, 000h, 000h
        call   sub_4_114D
        je     SHORT loc_4_13D2
        jne    SHORT loc_4_1401
loc_4_1399:
        db  0B8h, 0E2h, 000h, 000h, 000h
        call   sub_4_1149
        jne    SHORT loc_4_1401
        call   sub_4_0534
        jne    SHORT loc_4_1401
        db  080h, 00Dh, 009h, 003h, 000h, 000h, 004h
        db  0B8h, 0EFh, 000h, 000h, 000h
        call   sub_4_114D
        je     SHORT loc_4_13D2
        db  080h, 025h, 009h, 003h, 000h, 000h, 0FBh
        db  0B8h, 0D7h, 000h, 000h, 000h
        call   sub_4_114D
        jne    SHORT loc_4_1401
loc_4_13D2:
        db  083h, 03Dh, 0BCh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_13E1
        db  08Bh, 035h, 0BCh, 000h, 000h, 000h
loc_4_13E1:
        push   ebx
        call   sub_4_117A
        pop    ebx
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        jne    SHORT loc_4_13F9
        mov    dword ptr [ebx + 0ch], esi
        inc    byte ptr [ebx + 14h]
        jmp    SHORT loc_4_13FF
loc_4_13F9:
        mov    dword ptr [ebx + 15h], esi
        inc    byte ptr [ebx + 1dh]
loc_4_13FF:
        xor    eax, eax
loc_4_1401:
        pop    ebx
        pop    eax
        pop    edi
        pop    esi
        ret
sub_4_1406:
        push   esi
        push   edi
        push   ebx
        db  089h, 03Dh, 0B8h, 000h, 000h, 000h
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        test   esi, esi
        je     SHORT loc_4_147F
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_147F
loc_4_1421:
        call   sub_4_1345
        je     SHORT loc_4_1432
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_4_1421
        jmp    SHORT loc_4_147F
loc_4_1432:
        db  0A3h, 0B4h, 000h, 000h, 000h
        mov    esi, dword ptr [eax + 0ch]
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        je     SHORT loc_4_1446
        mov    esi, dword ptr [eax + 15h]
loc_4_1446:
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_4_145A
        push   dword ptr [esi + 10h]
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h
        add    esp, 4
        jmp    SHORT loc_4_147C
loc_4_145A:
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_4_1483
        pop    esi
        jb     SHORT loc_4_147F
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        jne    SHORT loc_4_1473
        mov    esi, dword ptr [esi + 0ch]
        jmp    SHORT loc_4_1476
loc_4_1473:
        mov    esi, dword ptr [esi + 15h]
loc_4_1476:
        mov    dword ptr [esi + 8], edx
        mov    dword ptr [esi + 0ch], eax
loc_4_147C:
        inc    dword ptr [esi + 4]
loc_4_147F:
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_4_1483:
        db  08Bh, 03Dh, 0F4h, 002h, 000h, 000h
        call   sub_4_1141
        db  0BAh, 0A4h, 002h, 000h, 000h
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        ret
sub_4_149D:
        pushad
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_4_14A7
        dec    dword ptr [eax + 4]
loc_4_14A7:
        dec    byte ptr [ecx + 8]
        jne    SHORT loc_4_1503
        xor    eax, eax
        xchg   dword ptr [ecx + 4], eax
        test   eax, eax
        je     SHORT loc_4_14C7
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_14C7:
        mov    eax, dword ptr [esp + 18h]
        xor    ecx, ecx
        xchg   dword ptr [eax], ecx
        jecxz  loc_4_1503
        cmp    dword ptr [ecx + 4], 0
        jne    SHORT loc_4_1503
        xor    ebx, ebx
        dec    ebx
        mov    esi, dword ptr [ecx]
        db  0BFh, 0A3h, 001h, 000h, 000h
        mov    edx, edi
loc_4_14E3:
        lodsb
        cmp    al, 2eh
        je     SHORT loc_4_14F5
        cmp    al, 61h
        jb     SHORT loc_4_14F2
        cmp    al, 7ah
        ja     SHORT loc_4_14F2
        and    al, 0dfh
loc_4_14F2:
        stosb
        jmp    SHORT loc_4_14E3
loc_4_14F5:
        mov    byte ptr [edi], 0
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        jae    SHORT loc_4_1503
        inc    dword ptr [ecx + 4]
loc_4_1503:
        popad
        ret
sub_4_1505:
        db  08Bh, 03Dh, 0B4h, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_1565
        mov    esi, dword ptr [edi + 0ch]
        db  0F6h, 005h, 009h, 003h, 000h, 000h, 004h
        je     SHORT loc_4_151E
        mov    esi, dword ptr [edi + 15h]
loc_4_151E:
        mov    dword ptr [esi + 10h], ecx
        push   dword ptr [edi]
        call   sub_4_0ED0
        pop    ecx
        mov    edi, eax
        xor    ebx, ebx
        test   eax, eax
        je     SHORT loc_4_1556
        db  0BEh, 001h, 003h, 000h, 000h
        push   0
        push   0
        push   4
        db  0FFh, 035h, 0FDh, 002h, 000h, 000h
        push   esi
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_1556
        mov    ebx, dword ptr [esi]
        movzx  ebx, word ptr [ebx + 2]
loc_4_1556:
        db  08Bh, 035h, 0B4h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  08Bh, 035h, 0B8h, 000h, 000h, 000h
        ret
loc_4_1565:
        xor    eax, eax
        ret
_VCOMM_Enumerate_DevNodes:
        db  0A1h, 0F8h, 002h, 000h, 000h
        ret
_VCOMM_Get_Contention_Handler:
        push   esi
        push   edi
        push   0
        push   dword ptr [esp + 10h]
        call   sub_4_1292
        pop    ecx
        pop    ecx
        db  0A1h, 00Eh, 003h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_1589
        mov    eax, dword ptr [eax + 4]
loc_4_1589:
        pop    edi
        pop    esi
        ret
_VCOMM_Map_Name_To_Resource:
        push   esi
        push   edi
        push   ebx
        push   0
        push   dword ptr [esp + 14h]
        call   sub_4_1292
        pop    ecx
        pop    ecx
        db  0A1h, 00Eh, 003h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_15C7
        db  081h, 078h, 004h, 0CCh, 015h, 000h, 000h
        je     SHORT loc_4_15C7
        mov    edi, eax
        mov    esi, dword ptr [edi]
        push   esi
        call   sub_4_0ED0
        pop    ecx
        push   eax
        push   esi
        push   dword ptr [esp + 18h]
        push   0
        call   dword ptr [edi + 4]
        add    esp, 10h
loc_4_15C7:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_4_15CB:
        ret
loc_4_15CC:
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, 5
        ja     SHORT loc_4_15DC
        db  0FFh, 024h, 08Dh, 025h, 003h, 000h, 000h
loc_4_15DC:
        xor    eax, eax
        ret
loc_4_15DF:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_1638
        test   byte ptr [ecx + 1eh], 8
        jne    SHORT loc_4_1638
        bts    word ptr [ecx + 1eh], 2
        jae    SHORT loc_4_1626
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_1638
        cmp    dword ptr [ecx + 20h], 0
        je     SHORT loc_4_1638
        push   ecx
        push   0
        push   dword ptr [ecx + 24h]
        call   dword ptr [ecx + 20h]
        pop    ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_163A
        test   byte ptr [ecx + 1eh], 4
        je     SHORT loc_4_1626
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ecx + 30h], eax
        or     byte ptr [ecx + 1eh], 8
loc_4_1626:
        mov    eax, ecx
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_4_163A
loc_4_1638:
        xor    eax, eax
loc_4_163A:
        ret
loc_4_163B:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_166F
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [ecx + 2ch], eax
        je     SHORT loc_4_166B
        btr    word ptr [ecx + 1eh], 3
        jae    SHORT loc_4_1667
        mov    edx, dword ptr [ecx + 2ch]
        mov    dword ptr [ecx + 20h], edx
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [ecx + 24h], eax
        push   1
        push   eax
        call   edx
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_166F
loc_4_1667:
        and    byte ptr [ecx + 1eh], 0fbh
loc_4_166B:
        and    byte ptr [ecx + 1eh], 0f7h
loc_4_166F:
        ret
loc_4_1670:
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_4_16AC
        test   byte ptr [ecx + 1eh], 8
        je     SHORT loc_4_16B0
        cmp    dword ptr [ecx + 20h], 0
        je     SHORT loc_4_16AC
        push   ecx
        push   0
        push   dword ptr [ecx + 24h]
        call   dword ptr [ecx + 20h]
        pop    ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_16B2
        test   byte ptr [ecx + 1eh], 8
        je     SHORT loc_4_16B2
        mov    eax, dword ptr [ecx + 20h]
        mov    edx, dword ptr [ecx + 24h]
        xchg   dword ptr [ecx + 2ch], eax
        xchg   dword ptr [ecx + 30h], edx
        mov    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 24h], edx
        jmp    SHORT loc_4_16B0
loc_4_16AC:
        xor    eax, eax
        jmp    SHORT loc_4_16B2
loc_4_16B0:
        or     al, 1
loc_4_16B2:
        ret
loc_4_16B3:
        xor    eax, eax
        inc    eax
        ret
loc_4_16B7:
        ret
sub_4_16B8:
        mov    ecx, dword ptr [edx + 0ch]
        or     ecx, dword ptr [edx + 15h]
        jne    SHORT loc_4_16D5
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        mov    eax, edx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        ret
loc_4_16D5:
        mov    eax, 3e8h
        db  0BEh, 0B8h, 016h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
sub_4_16E6:
        enter  0ch, 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        xor    ebx, ebx
        mov    dword ptr [ebp - 0ch], 4
        lea    ecx, [ebp - 4]
        push   ecx
        db  068h, 02Ah, 001h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    ebx, eax
        jb     SHORT loc_4_1747
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   eax
        push   0
        push   0
        db  068h, 056h, 001h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        cmp    ebx, eax
        jb     SHORT loc_4_1737
        cmp    dword ptr [ebp - 8], 1
loc_4_1737:
        pushfd
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
        popfd
loc_4_1747:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave
        ret
_PTEXT ends
        end
