; v86mmgr_pcode_08.asm -- V86MMGR object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

V86MMGR_SYS_VM_TERMINATE:
        db  0A1h, 098h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_8_0027
        mov    ecx, 692h
        add    ecx, eax
        dec    ecx
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
loc_8_0027:
        call   sub_8_004D
        ret
        db  000h, 000h, 000h
sub_8_0030:
        add    ecx, eax
        dec    ecx
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        ret
sub_8_004D:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        or     esi, esi
        je     SHORT loc_8_0090
        db  0A1h, 0B4h, 012h, 000h, 000h
        db  06Bh, 00Dh, 0B8h, 012h, 000h, 000h, 010h
        call   sub_8_0030
        jb     SHORT loc_8_008C
        db  0A1h, 0BCh, 012h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        shl    ecx, 1
        call   sub_8_0030
        jb     SHORT loc_8_008C
        mov    eax, dword ptr [esi + ebx + 60h]
        push   20h
        pop    ecx
        call   sub_8_0030
        jae    SHORT loc_8_0090
loc_8_008C:
        mov    dword ptr [esi + ebx + 60h], eax
loc_8_0090:
        clc
        ret
V86MMGR_SYSTEM_EXIT:
        pushad
        xor    al, al
        db  086h, 005h, 027h, 013h, 000h, 000h
        or     al, al
        je     SHORT loc_8_00AA
        db  0A1h, 098h, 032h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_8_00AA:
        call   sub_8_004D
        test   eax, eax
        je     NEAR PTR loc_8_019E
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  080h, 03Dh, 021h, 013h, 000h, 000h, 000h
        je     SHORT loc_8_0103
        db  0BEh, 0C0h, 012h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 020h, 013h, 000h, 000h
        jecxz  loc_8_0103
loc_8_00E2:
        movzx  eax, word ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        mov    byte ptr [ebp + 1dh], 11h
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 001h, 005h, 0F8h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        add    esi, 6
        loop   loc_8_00E2
loc_8_0103:
        db  083h, 03Dh, 07Ch, 012h, 000h, 000h, 000h
        jne    SHORT loc_8_013D
        db  08Bh, 035h, 0BCh, 012h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_8_013D
loc_8_011A:
        cld
        lodsw
        inc    ax
        je     SHORT loc_8_013B
        dec    ax
        mov    word ptr [ebp + 14h], ax
        mov    byte ptr [ebp + 1dh], 0ah
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0C4h, 004h, 0F8h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
loc_8_013B:
        loop   loc_8_011A
loc_8_013D:
        db  08Bh, 035h, 0B4h, 012h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 012h, 000h, 000h
        jecxz  loc_8_018D
loc_8_014B:
        test   byte ptr [esi + 0ch], 1
        je     SHORT loc_8_0188
        push   ecx
        movzx  ecx, word ptr [esi + 0ah]
        jecxz  loc_8_0181
loc_8_0158:
        mov    ax, word ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        mov    byte ptr [ebp + 1dh], 0dh
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 08Bh, 004h, 0F8h, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 1
        jne    SHORT loc_8_0181
        dec    word ptr [esi + 0ah]
        loop   loc_8_0158
loc_8_0181:
        mov    word ptr [esi + 0ch], 0
        pop    ecx
loc_8_0188:
        add    esi, 10h
        loop   loc_8_014B
loc_8_018D:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
loc_8_019E:
        popad
        clc
        ret
sub_8_01A1:
        pushfd
        db  0A1h, 0A0h, 012h, 000h, 000h
        or     eax, eax
        je     SHORT loc_8_01CA
        db  083h, 03Dh, 022h, 013h, 000h, 000h, 000h
        je     SHORT loc_8_01CA
        cli
        xor    ecx, ecx
        db  087h, 00Dh, 022h, 013h, 000h, 000h
        mov    dword ptr [eax], ecx
        xor    cl, cl
        db  086h, 00Dh, 026h, 013h, 000h, 000h
        mov    byte ptr [eax + 4], cl
loc_8_01CA:
        db  08Bh, 015h, 030h, 013h, 000h, 000h
        inc    edx
        je     SHORT loc_8_020B
        dec    edx
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        jne    SHORT loc_8_01E6
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 040h
        je     SHORT loc_8_0202
loc_8_01E6:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        or     esi, esi
        je     SHORT loc_8_0202
        mov    edi, dword ptr [esi + ebx + 60h]
        or     edi, edi
        je     SHORT loc_8_0202
        movzx  edx, word ptr [edi + 16h]
loc_8_0202:
        xor    ecx, ecx
        dec    ecx
        db  0CDh, 020h, 0BCh, 000h, 001h, 000h
loc_8_020B:
        popfd
        clc
        ret
        db  000h, 000h
_PTEXT ends
        end
