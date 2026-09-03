; vmcpd_icode_02.asm -- VMCPD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_2_0010
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_2_0010:
        db  0A3h, 070h, 008h, 000h, 000h
        mov    ecx, 74h
        mov    eax, 1
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  089h, 035h, 074h, 008h, 000h, 000h
        ret
loc_2_002C:
        mov    eax, cr0
        and    eax, 1eh
        db  08Bh, 00Dh, 040h, 008h, 000h, 000h
        and    ecx, 4
        or     eax, ecx
        db  0A3h, 068h, 008h, 000h, 000h
        mov    eax, cr0
        and    eax, 0fffffff1h
        mov    cr0, eax
        mov    ax, word ptr [410h]
        db  066h, 0A3h, 010h, 000h, 000h, 000h
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_2_00A2
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 010h, 000h, 000h, 000h
        jne    SHORT loc_2_0074
        call   sub_2_00FF
loc_2_0074:
        mov    ecx, 0ah
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        jne    SHORT loc_2_0088
        and    ecx, 0fffffffdh
loc_2_0088:
        mov    eax, cr0
        or     eax, ecx
        mov    cr0, eax
        db  0BFh, 054h, 001h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_2_00F6
        db  0A3h, 06Ch, 008h, 000h, 000h
loc_2_00A2:
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  0C7h, 005h, 064h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 7
        db  0BEh, 08Ah, 005h, 000h, 000h
        db  0CDh, 020h, 080h, 000h, 001h, 000h
        db  0BEh, 0D7h, 005h, 000h, 000h
        db  0CDh, 020h, 07Fh, 000h, 001h, 000h
        mov    eax, 7
        db  0BEh, 0A0h, 005h, 000h, 000h
        db  0CDh, 020h, 081h, 000h, 001h, 000h
        db  0BEh, 047h, 003h, 000h, 000h
        db  0CDh, 020h, 004h, 001h, 001h, 000h
        db  083h, 00Dh, 040h, 008h, 000h, 000h, 010h
        clc
        ret
loc_2_00F6:
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_2_00FF:
        push   esi
        push   edi
        mov    ax, word ptr [410h]
        db  066h, 0A3h, 010h, 000h, 000h, 000h
        sub    eax, eax
        db  0BEh, 074h, 001h, 000h, 000h
        db  0BFh, 084h, 001h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_2_0147
        cmp    eax, 2
        je     SHORT loc_2_0139
        cmp    eax, 1
        jne    SHORT loc_2_0147
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 008h, 000h, 000h, 000h
        je     SHORT loc_2_0147
loc_2_0139:
        db  083h, 025h, 040h, 008h, 000h, 000h, 0FDh
        and    byte ptr [410h], 0fdh
loc_2_0147:
        db  083h, 00Dh, 040h, 008h, 000h, 000h, 010h
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h
loc_2_0154:
        db  00Dh, 000h, 000h, 000h, 0D0h
        push   es
        add    byte ptr [eax], al
        dec    esp
        pop    es
        add    byte ptr [eax], al
        push   ebx
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0174:
        dec    ebp
        popad
        db  074h, 068h
        inc    ebx
        outsd
        db  070h, 072h
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 06Fh
        jb     SHORT loc_2_0184
loc_2_0184:
        inc    esi
        push   eax
        push   ebp
        inc    esi
        insb
        popad
        db  067h, 073h, 000h
_ITEXT ends
        end
