; cdvsd_pcode_03.asm -- CDVSD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 0ch
        mov    ecx, esp
        mov    word ptr [ecx], 0dh
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 10h
        ret
loc_3_0023:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        sub    esp, 0ch
        mov    ecx, esp
        mov    word ptr [ecx], 0eh
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 10h
        leave
        ret
loc_3_0049:
        inc    ecx
        db  075h, 074h
        outsd
        dec    ecx
        outsb
        jae    SHORT loc_3_00B6
        jb     SHORT loc_3_00C7
        dec    esi
        outsd
        db  074h, 069h
        imul   sp, word ptr [ebx + 61h], 6974h
        outsd
        outsb
        add    byte ptr [esi - 75h], dl
        db  073h, 00Ch
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_0227
        cmp    byte ptr [esi + 74h], 1
        jne    NEAR PTR loc_3_0227
        cmp    byte ptr [esi + 40h], 5
        jne    NEAR PTR loc_3_0227
        db  08Bh, 015h, 070h, 00Fh, 000h, 000h
        test   byte ptr [edx + 28h], 10h
        jne    SHORT loc_3_00CD
        mov    eax, dword ptr [esi + 1ch]
        test   eax, 100000h
        jne    SHORT loc_3_00CD
        test   byte ptr [edx + 29h], 8
        jne    NEAR PTR loc_3_0227
        test   byte ptr [esi + 482h], 20h
        jne    SHORT loc_3_00C3
        mov    al, byte ptr [esi + 84h]
        and    al, 7
loc_3_00B6:
        cmp    al, 2
        jae    SHORT loc_3_00CD
        test   byte ptr [esi + 482h], 8
        jne    SHORT loc_3_00CD
loc_3_00C3:
        test   byte ptr [edx + 29h], 10h
loc_3_00C7:
        jne    NEAR PTR loc_3_0227
loc_3_00CD:
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        or     eax, 40000h
        test   byte ptr [esi + 83h], 80h
        je     SHORT loc_3_0133
        or     dword ptr [esi + 20h], 10004h
        mov    dword ptr [esi + 494h], 0ff02ff01h
        mov    dword ptr [esi + 498h], 0
        or     eax, 30000h
        and    eax, 0ffefffffh
        push   eax
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edx, eax
        pop    eax
        or     edx, edx
        je     NEAR PTR loc_3_0227
        db  08Bh, 00Dh, 014h, 000h, 000h, 000h
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], esi
        db  089h, 015h, 014h, 000h, 000h, 000h
loc_3_0133:
        push   esi
        push   edi
        mov    esi, edi
        sub    esp, 18h
        mov    edi, esp
        mov    word ptr [edi], 5
        or     eax, 8
        and    eax, 0fffffffeh
        mov    dword ptr [edi + 12h], eax
        mov    dword ptr [edi + 4], esi
        db  0C7h, 047h, 008h, 048h, 005h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 16h], al
        mov    dword ptr [edi + 0ch], edx
        movzx  eax, word ptr [esi + 80h]
        add    eax, 62h
        mov    word ptr [edi + 10h], ax
        push   edi
        db  02Eh, 0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 1ch
        pop    edi
        pop    esi
        mov    byte ptr [esi + 7fh], 0eh
        sub    esp, 1ch
        mov    ecx, esp
        mov    word ptr [ecx], 11h
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0ch], 3
        mov    dword ptr [ecx + 10h], 1
        lea    edx, [ecx + 18h]
        mov    dword ptr [ecx + 14h], edx
        db  0C7h, 041h, 008h, 049h, 000h, 000h, 000h
        push   ecx
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 20h
        or     ecx, ecx
        mov    ecx, edx
        db  08Bh, 015h, 014h, 000h, 000h, 000h
        jne    SHORT loc_3_01D7
        or     byte ptr [edx + 9], 4
        or     ecx, ecx
        je     SHORT loc_3_01D7
        test   byte ptr [esi + 22h], 4
        jne    SHORT loc_3_01D7
        and    byte ptr [edx + 9], 0fbh
        or     byte ptr [edx + 9], 1
loc_3_01D7:
        cmp    byte ptr [esi + 7dh], 1
        jb     SHORT loc_3_0210
        ja     SHORT loc_3_0202
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     SHORT loc_3_0202
        or     byte ptr [ecx + 9], 2
        mov    eax, dword ptr [ecx + 4]
        or     byte ptr [eax + 482h], 80h
        test   byte ptr [ecx + 9], 1
        jne    SHORT loc_3_0202
        and    byte ptr [ecx + 8], 0feh
        call   sub_3_0229
loc_3_0202:
        or     byte ptr [edx + 9], 2
        mov    eax, dword ptr [edx + 4]
        or     byte ptr [eax + 482h], 80h
loc_3_0210:
        mov    ecx, edx
        call   sub_3_0229
        test   byte ptr [edx + 9], 1
        jne    SHORT loc_3_0223
        test   byte ptr [edx + 9], 6
        jne    SHORT loc_3_0227
loc_3_0223:
        or     byte ptr [edx + 8], 1
loc_3_0227:
        pop    esi
        ret
sub_3_0229:
        enter  0ch, 0
        pushad
        mov    dword ptr [ebp - 0ch], 1
        mov    esi, dword ptr [ecx + 4]
        mov    edx, dword ptr [esi + 70h]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 0
        test   byte ptr [ecx + 9], 4
        jne    SHORT loc_3_0256
        test   byte ptr [ecx + 9], 1
        jne    SHORT loc_3_0253
        test   byte ptr [ecx + 9], 2
        jne    SHORT loc_3_0256
loc_3_0253:
        mov    byte ptr [eax], 1
loc_3_0256:
        push   0
        push   1
        push   eax
        push   3
        db  068h, 049h, 000h, 000h, 000h
        push   0
        push   edx
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        popad
        leave
        ret
loc_3_0271:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_029A
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_029A
        db  0B8h, 014h, 000h, 000h, 000h
        db  08Bh, 015h, 014h, 000h, 000h, 000h
loc_3_028B:
        or     edx, edx
        je     SHORT loc_3_029A
        cmp    dword ptr [edx + 4], esi
        je     SHORT loc_3_029B
        mov    eax, edx
        mov    edx, dword ptr [edx]
        jmp    SHORT loc_3_028B
loc_3_029A:
        ret
loc_3_029B:
        cli
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], edx
        or     byte ptr [edx + 8], 10h
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        bts    dword ptr [edx + 8], 5
        sti
        jb     SHORT loc_3_02C0
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_3_02C0:
        ret
loc_3_02C1:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_02DE
        xor    esi, esi
        db  087h, 035h, 01Ch, 000h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    word ptr [ebx + 2], 0ffffh
loc_3_02DE:
        db  080h, 00Dh, 018h, 000h, 000h, 000h, 002h
        ret
        db  000h, 000h
loc_3_02E8:
        push   ebp
        push   edi
        push   esi
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 20h]
        mov    esi, dword ptr [ebx + 4]
        call   dword ptr [edi - 4]
        db  0E9h, 0BDh, 002h, 0FDh, 0FFh
        db  000h
loc_3_02FC:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 20h]
        lea    eax, [ebx + 45ch]
        mov    byte ptr [eax], 0
        mov    dword ptr [edi + 80h], eax
        xor    ecx, ecx
        mov    cl, 14h
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [edi + 7ch], ecx
        or     byte ptr [edi + 6dh], 2
        mov    dword ptr [esi + 0ch], 288h
        mov    byte ptr [esi + 34h], cl
        mov    cl, byte ptr [ebx + 7dh]
        shl    cl, 5
        or     cl, 10h
        mov    byte ptr [esi + 31h], cl
        mov    ecx, dword ptr [edi + 90h]
        mov    edx, dword ptr [ecx + 7ch]
        test   byte ptr [ecx + 6dh], 2
        jne    SHORT loc_3_034B
        mov    edx, 800h
loc_3_034B:
        xchg   dl, dh
        mov    word ptr [eax + 0ah], dx
        mov    byte ptr [esi + 30h], 15h
        mov    byte ptr [esi + 0ah], 6
        xor    edx, edx
        mov    word ptr [esi + 32h], dx
        mov    byte ptr [esi + 35h], dl
        mov    byte ptr [eax + 3], 8
        db  0C7h, 047h, 070h, 082h, 007h, 000h, 000h
        xchg   edi, ebx
        xchg   esi, edi
        db  0E9h, 051h, 00Bh, 0FDh, 0FFh
        db  000h, 000h
sub_3_0378:
        push   es
        add    byte ptr [eax], al
        inc    esi
        push   es
        add    byte ptr [eax], al
        jns    SHORT loc_3_0388
        add    byte ptr [eax], al
        stosb
        push   es
        add    byte ptr [eax], al
loc_3_0388:
        push   7
        add    byte ptr [eax], al
        jnp    SHORT loc_3_0395
        add    byte ptr [eax], al
        mov    al, byte ptr [edi]
        add    byte ptr [eax], al
        lahf
loc_3_0395:
        pop    es
        add    byte ptr [eax], al
        db  0A9h, 007h, 000h, 000h, 0B3h
        pop    es
        add    byte ptr [eax], al
        mov    bl, 7
        add    byte ptr [eax], al
        hlt
        pop    es
        add    byte ptr [eax], al
        xchg   esi, eax
        or     dword ptr [eax], eax
        db  000h, 027h
        or     al, byte ptr [eax]
        db  000h, 0C1h
        add    al, 0
        db  000h, 02Eh
        or     al, byte ptr [eax]
        db  000h, 057h, 00Ah
        add    byte ptr [eax], al
        daa
        or     al, byte ptr [eax]
        db  000h, 0EBh
        push   es
        add    byte ptr [eax], al
        or     eax, dword ptr [edi]
        add    byte ptr [eax], al
        xor    al, byte ptr [edi]
        add    byte ptr [eax], al
        daa
        or     al, byte ptr [eax]
        db  000h, 027h
        or     al, byte ptr [eax]
        db  000h, 027h
        or     al, byte ptr [eax]
        db  000h, 027h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03F5:
        test   byte ptr [ebx + 6dh], 40h
        jne    SHORT loc_3_0405
        test   byte ptr [esi + 22h], 8
        jne    NEAR PTR loc_3_0494
loc_3_0405:
        movzx  eax, word ptr [ebx + 92h]
        cmp    al, 19h
        jae    SHORT loc_3_046D
        cmp    ah, 0cfh
        je     SHORT loc_3_0470
        cmp    ah, 0cdh
        jne    SHORT loc_3_046D
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [ebx + 7ch], 0
        xor    ah, ah
        mov    byte ptr [edi + 31h], ah
        db  080h, 0B8h, 0DCh, 003h, 000h, 000h, 001h
        je     SHORT loc_3_0466
        db  0FFh, 014h, 085h, 078h, 003h, 000h, 000h
        or     al, al
        jne    SHORT loc_3_0465
loc_3_0441:
        mov    dl, byte ptr [esi + 7dh]
        shl    dl, 5
        or     byte ptr [edi + 31h], dl
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 8
        db  0C7h, 000h, 004h, 004h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  0C7h, 047h, 0FCh, 0A8h, 00Ah, 000h, 000h
        xor    al, al
loc_3_0465:
        ret
loc_3_0466:
        db  0E8h, 09Bh, 003h, 0FDh, 0FFh
        jmp    SHORT loc_3_0441
loc_3_046D:
        mov    al, 4
        ret
loc_3_0470:
        xor    ah, ah
        db  08Bh, 014h, 085h, 078h, 003h, 000h, 000h
        db  081h, 0FAh, 027h, 00Ah, 000h, 000h
        je     SHORT loc_3_046D
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_0494:
        cmp    word ptr [ebx + 92h], 0cd02h
        je     NEAR PTR loc_3_0405
        mov    dword ptr [ebx + 0a0h], 1fh
        cmp    word ptr [edi + 6ah], 18h
        jne    SHORT loc_3_04BE
        mov    dword ptr [ebx + 0a0h], 15h
loc_3_04BE:
        mov    al, 1
        ret
loc_3_04C1:
        test   byte ptr [esi + 47eh], 4
        je     NEAR PTR loc_3_07B3
        mov    ecx, dword ptr [ebx + 98h]
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_3_0518
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 0ch]
        cmp    eax, edx
        ja     SHORT loc_3_054C
        xchg   al, ah
        rol    eax, 10h
        xchg   al, ah
        xor    al, al
        xchg   dl, dh
        rol    edx, 10h
        xchg   dl, dh
        mov    byte ptr [edi + 32h], al
        mov    byte ptr [edi + 39h], al
        mov    dword ptr [edi + 35h], edx
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 30h], 47h
        mov    byte ptr [edi + 0ah], 0ah
        mov    byte ptr [edi + 39h], al
loc_3_050B:
        xor    eax, eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [edi + 0ch], 0
        ret
loc_3_0518:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, dword ptr [ecx + 0ch]
        ja     SHORT loc_3_054C
        mov    eax, dword ptr [ecx + 0ch]
        mov    edx, dword ptr [ecx + 8]
        sub    eax, edx
        xchg   al, ah
        rol    eax, 10h
        xchg   al, ah
        mov    dword ptr [edi + 36h], eax
        xchg   dl, dh
        rol    edx, 10h
        xchg   dl, dh
        mov    dword ptr [edi + 32h], edx
        mov    byte ptr [edi + 30h], 0a5h
        mov    byte ptr [edi + 0ah], 0ch
        mov    word ptr [edi + 3ah], 0
        jmp    SHORT loc_3_050B
loc_3_054C:
        mov    dword ptr [ebx + 0a0h], 16h
        mov    word ptr [ebx + 6ah], 10h
        mov    al, 1
        ret
loc_3_055F:
        adc    byte ptr cs:[edi], dl
        pop    ss
sub_3_0563:
        mov    byte ptr [edi + 30h], 42h
        mov    byte ptr [edi + 0ah], 0ah
        mov    word ptr [ebx + 0eh], 1eh
        mov    word ptr [ebx + 0ch], 1eh
        rol    al, 1
        mov    byte ptr [edi + 33h], ah
        mov    byte ptr [edi + 31h], al
        movzx  eax, ah
        db  00Fh, 0B6h, 080h, 05Fh, 005h, 000h, 000h
        xchg   al, ah
        mov    word ptr [edi + 37h], ax
        xchg   al, ah
        mov    dword ptr [edi + 0ch], 248h
        mov    cl, 1
        call   sub_3_05BC
        or     eax, eax
        mov    al, 1
        je     SHORT loc_3_05B7
        xor    eax, eax
        mov    word ptr [edi + 34h], ax
        mov    byte ptr [edi + 39h], al
        mov    byte ptr [edi + 36h], al
        mov    byte ptr [edi + 32h], 40h
        xor    al, al
loc_3_05B7:
        ret
sub_3_05B8:
        push   edx
        push   edi
        jmp    SHORT loc_3_05F3
sub_3_05BC:
        push   edx
        push   edi
        mov    dword ptr [ebx + 7ch], eax
        cmp    eax, 8
        ja     SHORT loc_3_05CB
        mov    eax, 8
loc_3_05CB:
        sub    esp, 10h
        mov    edi, esp
        mov    word ptr [edi], 3
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0ah], 9
        mov    byte ptr [edi + 0ch], cl
        push   edi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        mov    edi, dword ptr [edi + 6]
        add    esp, 14h
        or     edi, edi
        je     SHORT loc_3_0624
loc_3_05F3:
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ebx + 80h], edi
        mov    dword ptr [eax + 18h], edi
        and    dword ptr [ebx + 6ch], 0fffffffdh
        or     dword ptr [ebx + 6ch], 200h
        mov    byte ptr [ebx + 0b0h], 0
        push   edi
        push   ebx
        db  0FFh, 015h, 068h, 00Fh, 000h, 000h
        add    esp, 4
        pop    edi
        or     eax, eax
loc_3_061F:
        mov    eax, edi
        pop    edi
        pop    edx
        ret
loc_3_0624:
        mov    word ptr [ebx + 6ah], 1dh
        mov    dword ptr [ebx + 0a0h], 8
        jmp    SHORT loc_3_061F
loc_3_0636:
        mov    byte ptr [edi + 30h], 1
        mov    byte ptr [edi + 0ah], 6
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        xor    al, al
        ret
loc_3_0646:
        mov    edx, dword ptr [ebx + 98h]
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 4], 1
        jne    SHORT loc_3_0661
        xchg   ch, cl
        rol    ecx, 10h
        xchg   ch, cl
        db  0E8h, 03Bh, 009h, 0FDh, 0FFh
loc_3_0661:
        mov    byte ptr [edi + 30h], 2bh
        mov    byte ptr [edi + 0ah], 0ah
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [edi + 32h], ecx
        xor    eax, eax
        mov    dword ptr [edi + 36h], eax
        ret
loc_3_0679:
        test   byte ptr [esi + 22h], 8
        jne    SHORT loc_3_0689
        mov    ax, 100h
        call   sub_3_0563
        ret
loc_3_0689:
        mov    edx, 0f97h
        mov    ecx, dword ptr [ebx + 98h]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_06AA:
        mov    edx, dword ptr [ebx + 98h]
        cmp    dword ptr [edx + 4], 1
        ja     SHORT loc_3_06D9
        mov    dword ptr [edx + 8], 924h
        je     SHORT loc_3_06C6
        mov    dword ptr [edx + 8], 800h
loc_3_06C6:
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
loc_3_06D6:
        mov    al, 1
        ret
loc_3_06D9:
        mov    dword ptr [ebx + 0a0h], 16h
        mov    word ptr [ebx + 6ah], 10h
        jmp    SHORT loc_3_06D6
loc_3_06EB:
        mov    edx, dword ptr [ebx + 98h]
        mov    dword ptr [edx + 4], 96h
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_070B:
        mov    edx, dword ptr [ebx + 98h]
        mov    dword ptr [edx + 8], 96h
        mov    dword ptr [edx + 0ch], 96h
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_0732:
        mov    dword ptr [edx + 4], 3
        je     SHORT loc_3_074E
loc_3_073B:
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_074E:
        cmp    dword ptr [edx + 8], 96h
        je     SHORT loc_3_073B
        mov    dword ptr [ebx + 0a0h], 57h
        mov    word ptr [ebx + 6ah], 10h
        mov    al, 1
        ret
loc_3_076A:
        mov    edx, dword ptr [ebx + 98h]
        mov    eax, dword ptr [edx + 4]
        mov    ah, 1
        call   sub_3_0563
        ret
loc_3_077B:
        mov    edx, dword ptr [ebx + 98h]
        xor    ax, ax
        call   sub_3_0563
        ret
loc_3_078A:
        mov    edx, dword ptr [ebx + 98h]
        xor    ax, ax
        call   sub_3_0563
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [edi + 36h], dl
        ret
loc_3_079F:
        mov    ax, 101h
        call   sub_3_0563
        ret
loc_3_07A9:
        mov    ax, 101h
        call   sub_3_0563
        ret
loc_3_07B3:
        mov    al, 1ch
        mov    byte ptr [edi + 34h], al
        mov    cl, 1
        call   sub_3_05BC
        or     eax, eax
        mov    al, 1
        je     SHORT loc_3_07F3
        mov    byte ptr [edi + 31h], 0
        mov    byte ptr [edi + 32h], 0eh
        cmp    word ptr [ebx + 92h], 0cd09h
        je     SHORT loc_3_07DC
        mov    byte ptr [edi + 32h], 4eh
loc_3_07DC:
        mov    byte ptr [edi + 30h], 1ah
        mov    byte ptr [edi + 0ah], 6
        xor    al, al
        mov    byte ptr [edi + 35h], al
        mov    byte ptr [edi + 33h], al
        mov    dword ptr [edi + 0ch], 248h
loc_3_07F3:
        ret
loc_3_07F4:
        mov    edx, dword ptr [ebx + 98h]
        movzx  ecx, word ptr [esi + 474h]
        mov    word ptr [edx + 10h], cx
        movzx  ecx, byte ptr [edx + 12h]
        cmp    ecx, 0ffh
        jne    SHORT loc_3_0818
        movzx  ecx, byte ptr [esi + 475h]
loc_3_0818:
        mov    byte ptr [edx + 13h], cl
        cmp    cl, byte ptr [esi + 474h]
        jb     SHORT loc_3_0894
        je     NEAR PTR loc_3_08E5
        cmp    cl, byte ptr [esi + 475h]
        ja     SHORT loc_3_0894
        je     NEAR PTR loc_3_08CD
        jmp    NEAR PTR loc_3_08CB
loc_3_083C:
        movzx  eax, byte ptr [esi + 136h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 14h], 0
        mov    al, byte ptr [esi + 474h]
        mov    byte ptr [edx + 13h], al
loc_3_0856:
        movzx  eax, byte ptr [esi + 137h]
        mov    dword ptr [edx + 8], eax
        mov    al, 0aah
        test   byte ptr [esi + 482h], 10h
        je     SHORT loc_3_0877
        movzx  eax, byte ptr [esi + 487h]
        dec    eax
        mov    dword ptr [edx + 8], eax
        inc    eax
loc_3_0877:
        movzx  ecx, byte ptr [esi + 137h]
        inc    cl
        sub    cl, byte ptr [esi + 136h]
loc_3_0886:
        cmp    byte ptr [esi + ecx*8 + 13ah], al
        je     SHORT loc_3_08A7
        sub    cl, 1
        jae    SHORT loc_3_0886
loc_3_0894:
        mov    word ptr [ebx + 6ah], 10h
        mov    dword ptr [ebx + 0a0h], 57h
        mov    al, 1
        ret
loc_3_08A7:
        mov    eax, dword ptr [esi + ecx*8 + 13ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
loc_3_08B5:
        mov    dword ptr [edx + 0ch], eax
        mov    word ptr [ebx + 6ah], 0
        mov    dword ptr [ebx + 0a0h], 0
        mov    al, 1
        ret
loc_3_08CB:
        jmp    SHORT loc_3_0921
loc_3_08CD:
        movzx  ecx, byte ptr [esi + 133h]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [esi + 48ch]
        mov    dword ptr [edx + 14h], eax
        jmp    NEAR PTR loc_3_0856
loc_3_08E5:
        test   byte ptr [esi + 482h], 2
        je     NEAR PTR loc_3_083C
        movzx  eax, byte ptr [esi + 136h]
        mov    dword ptr [edx + 4], eax
        xor    eax, eax
        mov    dword ptr [edx + 14h], eax
        movzx  ecx, byte ptr [esi + 486h]
        cmp    cl, 0ffh
        jne    SHORT loc_3_0916
        movzx  ecx, byte ptr [esi + 133h]
        dec    cl
loc_3_0916:
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [esi + 488h]
        jmp    SHORT loc_3_08B5
loc_3_0921:
        mov    edx, dword ptr [ebx + 98h]
        movzx  ecx, byte ptr [edx + 13h]
        mov    byte ptr [edi + 36h], cl
        mov    eax, 8
        mov    word ptr [edi + 37h], ax
        xchg   al, ah
        mov    dword ptr [ebx + 7ch], eax
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 0ah], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0ch], 8
        push   edx
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        mov    eax, dword ptr [edx + 6]
        add    esp, 14h
        mov    edx, eax
        or     eax, eax
        mov    al, 1
        je     SHORT loc_3_0995
        push   edi
        mov    edi, edx
        call   sub_3_05B8
        pop    edi
        mov    byte ptr [edi + 30h], 43h
        mov    byte ptr [edi + 0ah], 0ah
        mov    al, byte ptr [esi + 7dh]
        shl    al, 5
        or     al, 2
        mov    byte ptr [edi + 31h], al
        xor    eax, eax
        mov    dword ptr [edi + 32h], eax
        mov    byte ptr [edi + 39h], 80h
        mov    dword ptr [edi + 0ch], 248h
loc_3_0995:
        ret
loc_3_0996:
        mov    edx, dword ptr [ebx + 98h]
        mov    eax, dword ptr [edx + 4]
        test   eax, 0ffffff00h
        jne    SHORT loc_3_09E2
        cmp    al, byte ptr [esi + 137h]
        ja     SHORT loc_3_09E2
        cmp    al, byte ptr [esi + 136h]
        jb     SHORT loc_3_09E2
        test   byte ptr [esi + 482h], 10h
        je     SHORT loc_3_09C7
        cmp    al, byte ptr [esi + 487h]
        jae    SHORT loc_3_09E2
loc_3_09C7:
        movzx  ecx, byte ptr [esi + 137h]
        sub    cl, byte ptr [esi + 136h]
loc_3_09D4:
        cmp    byte ptr [esi + ecx*8 + 13ah], al
        je     SHORT loc_3_09F5
        sub    cl, 1
        jae    SHORT loc_3_09D4
loc_3_09E2:
        mov    word ptr [ebx + 6ah], 10h
        mov    dword ptr [ebx + 0a0h], 57h
        mov    al, 1
        ret
loc_3_09F5:
        mov    eax, dword ptr [esi + ecx*8 + 13ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [edx + 8], eax
        movzx  eax, byte ptr [esi + ecx*8 + 139h]
        rol    al, 4
        mov    dword ptr [edx + 0ch], eax
        mov    word ptr [ebx + 6ah], 0
        mov    dword ptr [ebx + 0a0h], 0
        mov    al, 1
        ret
loc_3_0A27:
        and    byte ptr [ebx + 6dh], 7fh
        mov    al, 4
        ret
loc_3_0A2E:
        test   byte ptr [esi + 47eh], 1
        je     SHORT loc_3_0A41
        mov    ax, 100h
        call   sub_3_0563
        ret
loc_3_0A41:
        xor    eax, eax
        and    byte ptr [esi + 47eh], 0fdh
        mov    word ptr [ebx + 6ah], ax
        mov    dword ptr [ebx + 0a0h], eax
        mov    al, 1
        ret
loc_3_0A57:
        test   byte ptr [esi + 47eh], 2
        je     SHORT loc_3_0A95
        mov    ecx, dword ptr [esi + 47ah]
        db  0E8h, 05Eh, 005h, 0FDh, 0FFh
        mov    byte ptr [edi + 32h], 0
        mov    byte ptr [edi + 39h], 0
        mov    dword ptr [edi + 35h], ecx
        mov    ecx, dword ptr [esi + 476h]
        db  0E8h, 048h, 005h, 0FDh, 0FFh
        mov    dword ptr [edi + 32h], ecx
        mov    byte ptr [edi + 30h], 47h
        mov    byte ptr [edi + 0ah], 0ah
        mov    word ptr [edi + 3ah], 0
        xor    al, al
        ret
loc_3_0A95:
        mov    word ptr [ebx + 6ah], 20h
        mov    dword ptr [ebx + 0a0h], 15h
        mov    al, 1
        ret
loc_3_0AA8:
        mov    eax, dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 4]
        add    dword ptr [ebx + 14h], 8
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [edi + 44h], 0
        cmp    byte ptr [edi + 3], 1
        ja     SHORT loc_3_0B16
loc_3_0AC2:
        cmp    byte ptr [edi + 30h], 0
        je     SHORT loc_3_0AE0
        movzx  eax, word ptr [ebx + 92h]
        xor    ah, ah
        cmp    eax, 19h
        ja     SHORT loc_3_0B40
        db  0FFh, 014h, 085h, 08Bh, 00Bh, 000h, 000h
        ret
loc_3_0AE0:
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0B0E
        mov    word ptr [ebx + 0ch], 0fh
        mov    word ptr [ebx + 0eh], 0fh
        mov    dword ptr [edi + 0ch], 0
        mov    byte ptr [edi + 31h], 0
        movzx  eax, word ptr [ebx + 92h]
        xor    ah, ah
        db  0FFh, 014h, 085h, 078h, 003h, 000h, 000h
        ret
loc_3_0B0E:
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0B16:
        test   byte ptr [edi + 3], 80h
        jne    SHORT loc_3_0B53
        cmp    byte ptr [edi + 3], 4
        jne    SHORT loc_3_0B32
        cmp    byte ptr [edi + 4], 2
        cmp    byte ptr [edi + 4], 8
        je     SHORT loc_3_0B32
        cmp    byte ptr [edi + 4], 18h
        je     SHORT loc_3_0B3A
loc_3_0B32:
        cmp    byte ptr [edi + 4ch], 5
        jb     SHORT loc_3_0B3A
        jmp    SHORT loc_3_0AC2
loc_3_0B3A:
        inc    byte ptr [edi + 4ch]
        mov    al, 1
        ret
loc_3_0B40:
        mov    word ptr [ebx + 6ah], 16h
        mov    dword ptr [ebx + 0a0h], 1fh
        mov    al, 1
        ret
loc_3_0B53:
        mov    ecx, dword ptr [edi + 1ch]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0fh
        cmp    al, 0
        je     SHORT loc_3_0B32
        cmp    al, 5
        jne    SHORT loc_3_0B69
        jmp    NEAR PTR loc_3_0AC2
loc_3_0B69:
        cmp    al, 1
        jne    SHORT loc_3_0B76
        mov    byte ptr [edi + 3], 1
        jmp    NEAR PTR loc_3_0AC2
loc_3_0B76:
        cmp    al, 3
        je     SHORT loc_3_0B32
        cmp    al, 4
        je     SHORT loc_3_0B32
        cmp    al, 0bh
        je     SHORT loc_3_0B32
        cmp    al, 2
        jne    SHORT loc_3_0B86
loc_3_0B86:
        jmp    NEAR PTR loc_3_0AC2
sub_3_0B8B:
        pop    ebp
        or     al, 0
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        jo     SHORT loc_3_0BA5
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h, 07Eh, 00Dh
        add    byte ptr [eax], al
        stosb
        db  00Dh, 000h, 000h, 0D4h, 00Dh
loc_3_0BA5:
        add    byte ptr [eax], al
        db  0FEh, 00Dh, 000h, 000h, 0A2h, 00Eh
        add    byte ptr [eax], al
        and    dword ptr [edi], ecx
        add    byte ptr [eax], al
        mov    cl, byte ptr [edi]
        add    byte ptr [eax], al
        push   es
        adc    dword ptr [eax], eax
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h, 065h, 00Ch
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        or     al, 0
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h, 05Dh, 00Ch
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h
sub_3_0BEF:
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0C06
        mov    word ptr [ebx + 6ah], 0
        mov    dword ptr [ebx + 0a0h], 0
        ret
loc_3_0C06:
        cmp    byte ptr [edi + 4], 8
        je     SHORT loc_3_0C3B
        cmp    byte ptr [edi + 4], 18h
        je     SHORT loc_3_0C4C
        mov    eax, dword ptr [edi + 1ch]
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_3_0C3B
        movzx  edx, byte ptr [eax + 2]
        and    dl, 0fh
        db  00Fh, 0B7h, 004h, 055h, 05Eh, 012h, 000h, 000h
        movzx  ecx, ah
        movzx  eax, al
        mov    word ptr [ebx + 6ah], ax
        mov    dword ptr [ebx + 0a0h], ecx
        ret
loc_3_0C3B:
        mov    word ptr [ebx + 6ah], 21h
        mov    dword ptr [ebx + 0a0h], 15h
        ret
loc_3_0C4C:
        mov    word ptr [ebx + 6ah], 1bh
        mov    dword ptr [ebx + 0a0h], 1fh
        ret
loc_3_0C5D:
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0C65:
        cmp    byte ptr [edi + 30h], 1ah
        je     SHORT loc_3_0CDC
        cmp    byte ptr [edi + 30h], 15h
        je     SHORT loc_3_0CE6
        cmp    byte ptr [edi + 3], 1
        ja     SHORT loc_3_0CD4
        and    byte ptr [esi + 47eh], 0fdh
        or     byte ptr [esi + 47eh], 1
        cmp    byte ptr [edi + 30h], 47h
        je     SHORT loc_3_0CC0
        mov    edx, dword ptr [edi + 32h]
        xchg   dl, dh
        rol    edx, 10h
        xchg   dl, dh
        mov    ecx, dword ptr [edi + 36h]
        xchg   cl, ch
        rol    ecx, 10h
        xchg   cl, ch
        add    ecx, edx
loc_3_0CA1:
        mov    dword ptr [esi + 476h], edx
        mov    dword ptr [esi + 47ah], ecx
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        mov    al, 1
        ret
loc_3_0CC0:
        mov    ecx, dword ptr [edi + 32h]
        db  0E8h, 0D4h, 002h, 0FDh, 0FFh
        mov    edx, ecx
        mov    ecx, dword ptr [edi + 35h]
        db  0E8h, 0CAh, 002h, 0FDh, 0FFh
        jmp    SHORT loc_3_0CA1
loc_3_0CD4:
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0CDC:
        cmp    byte ptr [edi + 3], 1
        je     NEAR PTR loc_3_0FBA
loc_3_0CE6:
        or     byte ptr [esi + 47eh], 4
        and    byte ptr [edi + 31h], 0e0h
        jmp    NEAR PTR loc_3_04C1
loc_3_0CF6:
        mov    eax, dword ptr [ebx + 80h]
        and    byte ptr [esi + 47eh], 0fch
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0D71
        cmp    byte ptr [edi + 30h], 42h
        jne    SHORT loc_3_0D52
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edi + 32h], ecx
        xchg   cl, ch
        rol    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [esi + 476h], ecx
        cmp    byte ptr [eax + 1], 11h
        jne    SHORT loc_3_0D52
        xor    ecx, ecx
        mov    dword ptr [edi + 36h], ecx
        mov    byte ptr [edi + 30h], 2bh
        mov    byte ptr [edi + 0ah], 0ah
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [edi + 0ch], 0
        and    byte ptr [edi + 31h], 0e0h
        mov    word ptr [ebx + 0ch], 0fh
        mov    word ptr [ebx + 0eh], 0fh
        xor    al, al
        ret
loc_3_0D52:
        or     byte ptr [esi + 47eh], 2
        call   sub_3_1244
        mov    word ptr [ebx + 6ah], 0
        mov    dword ptr [ebx + 0a0h], 0
        mov    al, 1
        ret
loc_3_0D71:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0D7E:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0D9D
        mov    edx, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebx + 98h]
        xchg   dl, dh
        rol    edx, 10h
        xchg   dl, dh
        mov    dword ptr [ecx + 8], edx
loc_3_0D9D:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0DAA:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_0E7D
        test   byte ptr [eax + 10h], 80h
        je     NEAR PTR loc_3_0E7D
        push   edi
        push   esi
        mov    ecx, dword ptr [ebx + 98h]
        lea    esi, [eax + 11h]
        lea    edi, [ecx + 4]
        jmp    SHORT loc_3_0E53
loc_3_0DD4:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_0E7D
        test   byte ptr [eax + 1fh], 80h
        je     NEAR PTR loc_3_0E8A
        push   edi
        push   esi
        mov    ecx, dword ptr [ebx + 98h]
        lea    esi, [eax + 20h]
        lea    edi, [ecx + 8]
        jmp    SHORT loc_3_0E53
loc_3_0DFE:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0E7D
        movzx  edx, byte ptr [eax + 5]
        cmp    dl, 40h
        jae    SHORT loc_3_0E8A
        and    dl, 0f0h
        or     dl, 10h
        mov    byte ptr [eax + 5], dl
        cmp    byte ptr [eax + 1], 11h
        je     SHORT loc_3_0E29
        and    byte ptr [esi + 47eh], 0feh
loc_3_0E29:
        push   edi
        push   esi
        push   eax
        mov    ecx, dword ptr [ebx + 98h]
        lea    esi, [eax + 6]
        lea    edi, [ecx + 5]
        mov    dword ptr [edi + 7], 0
        movsw
        lodsd
        mov    edx, eax
        lodsd
        ror    eax, 8
        stosd
        mov    eax, edx
        dec    edi
        stosd
        lea    edi, [ecx + 4]
        pop    eax
        jmp    SHORT loc_3_0E70
loc_3_0E53:
        push   edi
        push   eax
        inc    edi
        mov    dword ptr [esi + 8], 0
        mov    ecx, 7
        xor    eax, eax
loc_3_0E64:
        lodsb
        rol    eax, 0ch
        lodsb
        or     al, ah
        stosb
        loop   loc_3_0E64
        pop    eax
        pop    edi
loc_3_0E70:
        movzx  ecx, byte ptr [eax + 5]
        rol    ecx, 4
        or     cl, ch
        mov    byte ptr [edi], cl
        pop    esi
        pop    edi
loc_3_0E7D:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0E8A:
        call   sub_3_1244
        mov    word ptr [ebx + 6ah], 10h
        mov    dword ptr [ebx + 0a0h], 0dh
        mov    al, 1
        ret
loc_3_0EA2:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0EC1
        mov    ecx, dword ptr [ebx + 98h]
        cmp    byte ptr [eax + 1], 11h
        jne    SHORT loc_3_0EFA
        mov    dword ptr [ecx + 4], 0
loc_3_0EC1:
        call   sub_3_1244
        mov    edx, ecx
        mov    ecx, dword ptr [esi + 476h]
        db  0E8h, 0F6h, 000h, 0FDh, 0FFh
        xchg   cl, ch
        rol    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 47ah]
        db  0E8h, 0E1h, 000h, 0FDh, 0FFh
        xchg   cl, ch
        rol    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [edx + 0ch], ecx
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0EFA:
        and    byte ptr [esi + 47eh], 0feh
        test   dword ptr [esi + 47eh], 2
        jne    SHORT loc_3_0F16
        mov    dword ptr [ecx + 4], 2
        jmp    SHORT loc_3_0EC1
loc_3_0F16:
        mov    dword ptr [ecx + 4], 1
        mov    edx, ecx
        jmp    SHORT loc_3_0EC1
loc_3_0F21:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_0F5C
        mov    edx, dword ptr [ebx + 98h]
        cmp    byte ptr [eax + 3], 8
        jne    SHORT loc_3_0F72
        cmp    byte ptr [eax + 0ch], 0eh
        jne    SHORT loc_3_0F72
        push   edi
        push   esi
        push   eax
        lea    esi, [eax + 14h]
        lea    edi, [edx + 4]
        mov    ecx, 4
loc_3_0F4D:
        lodsw
        or     al, al
        je     SHORT loc_3_0F69
        dec    al
loc_3_0F55:
        stosw
        loop   loc_3_0F4D
        pop    eax
        pop    esi
        pop    edi
loc_3_0F5C:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_0F69:
        mov    eax, 4
        sub    eax, ecx
        jmp    SHORT loc_3_0F55
loc_3_0F72:
        mov    word ptr [ebx + 6ah], 10h
        mov    dword ptr [ebx + 0a0h], 1fh
        call   sub_3_1244
        mov    al, 1
        ret
loc_3_0F8A:
        cmp    byte ptr [edi + 30h], 15h
        je     NEAR PTR loc_3_1038
        mov    edx, dword ptr [ebx + 98h]
        push   esi
        push   edi
        lea    edi, [esi + 494h]
        lea    esi, [edx + 4]
        mov    ecx, 4
loc_3_0FAA:
        lodsw
        inc    al
        or     ah, ah
        jne    SHORT loc_3_0FB4
        xor    eax, eax
loc_3_0FB4:
        stosw
        loop   loc_3_0FAA
        pop    edi
        pop    esi
loc_3_0FBA:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_104B
        cmp    byte ptr [eax + 3], 8
        jne    NEAR PTR loc_3_1058
        mov    byte ptr [eax + 0ch], 0eh
        mov    byte ptr [eax + 0dh], 0eh
        mov    byte ptr [eax + 0eh], 4
        mov    dword ptr [eax + 4], 0
        mov    ecx, dword ptr [esi + 490h]
        xchg   cl, ch
        ror    ecx, 10h
        xchg   cl, ch
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 494h]
        and    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esi + 498h]
        and    dword ptr [eax + 18h], ecx
        mov    dword ptr [edi + 32h], 0
        mov    byte ptr [eax], 0
        mov    edx, 1ch
        mov    byte ptr [edi + 34h], dl
        mov    byte ptr [edi + 31h], 10h
        mov    byte ptr [edi + 30h], 15h
        mov    byte ptr [edi + 0ah], 6
        mov    dword ptr [edi + 0ch], 288h
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [ebx + 7ch], edx
        xor    al, al
        ret
loc_3_1038:
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_104B
        or     byte ptr [esi + 47eh], 4
loc_3_104B:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_1058:
        mov    word ptr [ebx + 6ah], 10h
        mov    dword ptr [ebx + 0a0h], 1fh
        call   sub_3_1244
        mov    al, 1
        ret
loc_3_1070:
        mov    ecx, dword ptr [ebx + 98h]
        mov    eax, dword ptr [ebx + 80h]
        cmp    byte ptr [edi + 3], 1
        jne    SHORT loc_3_10CD
        mov    edx, 4390h
        cmp    byte ptr [eax + 1], 11h
        je     SHORT loc_3_10A6
loc_3_108D:
        and    edx, 0ffffbfffh
        and    byte ptr [esi + 47eh], 0feh
        test   byte ptr [esi + 482h], 1
        jne    SHORT loc_3_10A6
        and    edx, 0ffffffefh
loc_3_10A6:
        or     edx, 2
        cmp    byte ptr [esi + 79h], 0
        jne    SHORT loc_3_10B2
        and    edx, 0fffffffdh
loc_3_10B2:
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ebx + 0a0h], 0
        mov    word ptr [ebx + 6ah], 0
        call   sub_3_1244
        mov    al, 1
        ret
loc_3_10CD:
        mov    edx, dword ptr [edi + 1ch]
        movzx  edx, byte ptr [edx + 2]
        and    dl, 0fh
        cmp    dl, 5
        mov    edx, 0b91h
        jne    SHORT loc_3_108D
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret
        db  0C7h, 083h, 0A0h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 066h, 0C7h, 043h, 06Ah, 016h, 000h
        db  0E8h, 041h, 001h, 000h, 000h, 0B0h, 001h, 0C3h
loc_3_1106:
        cmp    byte ptr [edi + 3], 1
        jne    NEAR PTR loc_3_1224
        mov    eax, dword ptr [ebx + 80h]
        lea    edx, [eax + 4]
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        add    eax, ecx
        movzx  ecx, byte ptr [edi + 36h]
        mov    ch, byte ptr [esi + 475h]
        push   ebx
        push   esi
        mov    esi, dword ptr [ebx + 98h]
loc_3_1132:
        cmp    byte ptr [edx + 3], 0a0h
        je     SHORT loc_3_1167
loc_3_1138:
        add    edx, 0bh
        cmp    edx, eax
        jl     SHORT loc_3_1132
loc_3_113F:
        pop    esi
        pop    ebx
        mov    word ptr [ebx + 6ah], 14h
        mov    dword ptr [ebx + 0a0h], 0bh
loc_3_1151:
        mov    edx, dword ptr [ebx + 80h]
        push   esi
        db  0BEh, 02Ch, 012h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        mov    al, 1
        ret
loc_3_1167:
        and    byte ptr [edx + 1], 0f0h
        cmp    byte ptr [edx + 1], 10h
        jne    SHORT loc_3_1138
        cmp    byte ptr [edx], cl
        jne    SHORT loc_3_113F
        movzx  ebx, byte ptr [edx + 8]
        mov    dword ptr [esi + 4], ebx
        add    edx, 0bh
        cmp    byte ptr [edx + 3], 0a1h
        jne    SHORT loc_3_113F
        and    byte ptr [edx + 1], 0f0h
        cmp    byte ptr [edx + 1], 10h
        jne    SHORT loc_3_113F
        movzx  ebx, byte ptr [edx + 8]
        mov    dword ptr [esi + 8], ebx
        add    edx, 0bh
        cmp    byte ptr [edx + 3], 0a2h
        jne    SHORT loc_3_113F
        and    byte ptr [edx + 1], 0f0h
        cmp    byte ptr [edx + 1], 10h
        jne    SHORT loc_3_113F
        mov    ebx, dword ptr [edx + 7]
        movzx  bx, bh
        rol    ebx, 10h
        xchg   bh, bl
        mov    dword ptr [esi + 0ch], ebx
        mov    ebx, dword ptr [esi + 4]
loc_3_11BB:
        cmp    byte ptr [edx + 2], bl
        je     SHORT loc_3_11CC
loc_3_11C0:
        add    edx, 0bh
        cmp    edx, eax
        jle    SHORT loc_3_11BB
        jmp    NEAR PTR loc_3_113F
loc_3_11CC:
        and    byte ptr [edx + 1], 0f0h
        cmp    byte ptr [edx + 1], 10h
        jne    SHORT loc_3_11C0
        cmp    byte ptr [edx + 3], 99h
        jae    SHORT loc_3_11C0
        cmp    byte ptr [edx], cl
        jne    NEAR PTR loc_3_113F
        mov    byte ptr [esi + 13h], cl
        mov    ecx, dword ptr [edx + 7]
        db  0E8h, 0ADh, 0FDh, 0FCh, 0FFh
        mov    dword ptr [esi + 14h], ecx
        pop    esi
        pop    ebx
        movzx  ecx, byte ptr [edi + 36h]
        cmp    cl, byte ptr [esi + 475h]
        jne    SHORT loc_3_120F
        mov    ecx, dword ptr [esi + 48ch]
        mov    edx, dword ptr [ebx + 98h]
        mov    dword ptr [edx + 14h], ecx
loc_3_120F:
        mov    word ptr [ebx + 6ah], 0
        mov    dword ptr [ebx + 0a0h], 0
        jmp    NEAR PTR loc_3_1151
loc_3_1224:
        call   sub_3_0BEF
        mov    al, 1
        ret
loc_3_122C:
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 0ch
        ret
sub_3_1244:
        push   esi
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], eax
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 054h, 00Fh, 000h, 000h
        add    esp, 0ch
        pop    esi
        ret
        db  000h, 000h, 001h, 000h, 020h, 015h, 014h, 01Bh, 01Bh, 01Fh, 010h, 057h, 019h, 015h, 01Fh, 013h
        db  010h, 057h, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh
        db  023h, 043h
_PTEXT ends
        end
