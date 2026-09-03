; ati_pcode_06.asm -- ATI object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

ATI_SYS_VM_TERMINATE:
        call   sub_6_01C5
        clc
        ret
loc_6_0007:
        push   eax
        push   edx
        mov    edx, 42eeh
        xor    eax, eax
        out    dx, ax
        pop    edx
        pop    eax
        db  080h, 03Dh, 09Ch, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_6_0061
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_0069
        push   eax
        push   edx
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0b2h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 09Ch, 001h, 000h, 000h
        dec    edx
        db  08Ah, 025h, 09Ah, 001h, 000h, 000h
        mov    al, 0b2h
        out    dx, ax
        mov    al, 0aeh
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 09Dh, 001h, 000h, 000h
        dec    edx
        db  08Ah, 025h, 09Bh, 001h, 000h, 000h
        mov    al, 0aeh
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_6_0061:
        and    al, 0fh
        db  002h, 005h, 0A1h, 001h, 000h, 000h
loc_6_0069:
        ret
loc_6_006A:
        db  080h, 03Dh, 09Ch, 001h, 000h, 000h, 0FFh
        je     SHORT loc_6_00C7
        push   eax
        push   edx
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0aeh
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
        mov    al, 0b2h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 015h, 09Ah, 001h, 000h, 000h
        db  08Ah, 035h, 09Bh, 001h, 000h, 000h
        cmp    ax, dx
        jne    SHORT loc_6_00B5
        mov    edx, 1ceh
        mov    al, 0b2h
        db  08Ah, 025h, 09Ch, 001h, 000h, 000h
        out    dx, ax
        mov    al, 0aeh
        db  08Ah, 025h, 09Dh, 001h, 000h, 000h
        out    dx, ax
loc_6_00B5:
        db  0C6h, 005h, 09Ch, 001h, 000h, 000h, 0FFh
        mov    edx, 1ceh
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_6_00C7:
        ret
loc_6_00C8:
        push   eax
        push   edx
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_010A
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0b2h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 09Fh, 001h, 000h, 000h
        dec    edx
        mov    al, 0b2h
        db  08Ah, 025h, 09Ah, 001h, 000h, 000h
        out    dx, ax
        mov    al, 0aeh
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 09Eh, 001h, 000h, 000h
        dec    edx
        mov    al, 0aeh
        db  08Ah, 025h, 09Bh, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_010A:
        pop    edx
        pop    eax
        ret
loc_6_010D:
        push   eax
        push   edx
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_0139
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0b2h
        db  08Ah, 025h, 09Fh, 001h, 000h, 000h
        out    dx, ax
        mov    al, 0aeh
        db  08Ah, 025h, 09Eh, 001h, 000h, 000h
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_0139:
        pop    edx
        pop    eax
        ret
loc_6_013C:
        db  0C6h, 005h, 0A3h, 001h, 000h, 000h, 0FFh
        mov    edx, 1ceh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 1cfh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    SHORT loc_6_0167
        call   sub_6_0B28
loc_6_0167:
        call   sub_6_01C5
        ret
loc_6_016D:
        mov    edx, 1ceh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 1cfh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    SHORT loc_6_0191
        call   sub_6_07DD
loc_6_0191:
        db  0C6h, 005h, 0A3h, 001h, 000h, 000h, 000h
        ret
loc_6_0199:
        db  0C6h, 005h, 0A3h, 001h, 000h, 000h, 0FFh
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_01BC
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0e04h
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_6_01BC:
        ret
loc_6_01BD:
        db  0C6h, 005h, 0A3h, 001h, 000h, 000h, 000h
        ret
sub_6_01C5:
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_0243
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 008h
        je     SHORT loc_6_0238
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    SHORT loc_6_0243
        push   ebx
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    edx, 68h
        db  066h, 08Bh, 00Dh, 098h, 001h, 000h, 000h
        mov    dword ptr [ebp + 1ch], 0
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        pop    ebx
        jmp    SHORT loc_6_0243
loc_6_0238:
        mov    edx, 4aeeh
        in     ax, dx
        and    al, 0feh
        out    dx, ax
loc_6_0243:
        ret
loc_6_0244:
        mov    edx, 1ceh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 1cfh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    SHORT loc_6_0268
        call   sub_6_07DD
loc_6_0268:
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    NEAR PTR loc_6_0516
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_6_0516
        mov    edx, 76eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 76efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 86e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ee9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8eeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8eefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 92e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 9aeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 9aefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aaeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aaefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aeeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aeefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b2eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b2efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0baeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0baefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bee9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0beeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0beefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0c2eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0c2efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0caeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0caefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0ceeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0ceefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0d2eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0d2efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0d6eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0d6efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0daeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0daefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0deeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0deefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2e9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e6eeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e6efh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0feeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0feefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_6_0516:
        ret
loc_6_0517:
        db  083h, 03Dh, 08Ch, 001h, 000h, 000h, 000h
        jg     SHORT loc_6_052E
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    SHORT loc_6_052E
        call   sub_6_0B28
loc_6_052E:
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    NEAR PTR loc_6_07DC
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_6_07DC
        mov    edx, 76eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 76efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 86e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ee9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8eeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8eefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 92e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 9aeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 9aefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aaeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aaefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aeeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aeefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b2eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b2efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0baeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0baefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bee9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0beeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0beefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0c2eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0c2efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0caeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0caefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0ceeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0ceefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0d2eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0d2efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0d6eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0d6efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0daeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0daefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0deeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0deefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2e9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e6eeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e6efh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0feeeh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0feefh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_6_07DC:
        ret
sub_6_07DD:
        xor    edx, edx
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_07F8
        add    dx, 4eech
        jmp    SHORT loc_6_07FD
loc_6_07F8:
        add    dx, 38ch
loc_6_07FD:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_081B
        add    dx, 52ech
        jmp    SHORT loc_6_0820
loc_6_081B:
        add    dx, 39ch
loc_6_0820:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_083E
        add    dx, 56ech
        jmp    SHORT loc_6_0843
loc_6_083E:
        add    dx, 3a0h
loc_6_0843:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0861
        add    dx, 5aech
        jmp    SHORT loc_6_0866
loc_6_0861:
        add    dx, 3a4h
loc_6_0866:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0884
        add    dx, 66ech
        jmp    SHORT loc_6_0889
loc_6_0884:
        add    dx, 3bch
loc_6_0889:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_08A7
        add    dx, 6aech
        jmp    SHORT loc_6_08AC
loc_6_08A7:
        add    dx, 3c8h
loc_6_08AC:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_08CA
        add    dx, 6ech
        jmp    SHORT loc_6_08CF
loc_6_08CA:
        add    dx, 2f0h
loc_6_08CF:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_08ED
        add    dx, 0aech
        jmp    SHORT loc_6_08F2
loc_6_08ED:
        add    dx, 2f4h
loc_6_08F2:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0910
        add    dx, 0eech
        jmp    SHORT loc_6_0915
loc_6_0910:
        add    dx, 2f8h
loc_6_0915:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0933
        add    dx, 12ech
        jmp    SHORT loc_6_0938
loc_6_0933:
        add    dx, 2fch
loc_6_0938:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0956
        add    dx, 16ech
        jmp    SHORT loc_6_095B
loc_6_0956:
        add    dx, 300h
loc_6_095B:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0979
        add    dx, 1aech
        jmp    SHORT loc_6_097E
loc_6_0979:
        add    dx, 304h
loc_6_097E:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_099C
        add    dx, 1eech
        jmp    SHORT loc_6_09A1
loc_6_099C:
        add    dx, 308h
loc_6_09A1:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_09BF
        add    dx, 22ech
        jmp    SHORT loc_6_09C4
loc_6_09BF:
        add    dx, 32ch
loc_6_09C4:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_09E2
        add    dx, 26ech
        jmp    SHORT loc_6_09E7
loc_6_09E2:
        add    dx, 330h
loc_6_09E7:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0A05
        add    dx, 2aech
        jmp    SHORT loc_6_0A0A
loc_6_0A05:
        add    dx, 334h
loc_6_0A0A:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0A28
        add    dx, 2eech
        jmp    SHORT loc_6_0A2D
loc_6_0A28:
        add    dx, 34ch
loc_6_0A2D:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0A4B
        add    dx, 32ech
        jmp    SHORT loc_6_0A50
loc_6_0A4B:
        add    dx, 350h
loc_6_0A50:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0A6E
        add    dx, 36ech
        jmp    SHORT loc_6_0A73
loc_6_0A6E:
        add    dx, 354h
loc_6_0A73:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0A91
        add    dx, 3aech
        jmp    SHORT loc_6_0A96
loc_6_0A91:
        add    dx, 358h
loc_6_0A96:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0AB4
        add    dx, 3eech
        jmp    SHORT loc_6_0AB9
loc_6_0AB4:
        add    dx, 35ch
loc_6_0AB9:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0AD7
        add    dx, 4aech
        jmp    SHORT loc_6_0ADC
loc_6_0AD7:
        add    dx, 37ch
loc_6_0ADC:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0AFA
        add    dx, 5eech
        jmp    SHORT loc_6_0AFF
loc_6_0AFA:
        add    dx, 3ach
loc_6_0AFF:
        call   sub_6_0E73
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0B1D
        add    dx, 62ech
        jmp    SHORT loc_6_0B22
loc_6_0B1D:
        add    dx, 3b0h
loc_6_0B22:
        call   sub_6_0E73
        ret
sub_6_0B28:
        xor    edx, edx
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0B43
        add    dx, 4eech
        jmp    SHORT loc_6_0B48
loc_6_0B43:
        add    dx, 38ch
loc_6_0B48:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0B66
        add    dx, 52ech
        jmp    SHORT loc_6_0B6B
loc_6_0B66:
        add    dx, 39ch
loc_6_0B6B:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0B89
        add    dx, 56ech
        jmp    SHORT loc_6_0B8E
loc_6_0B89:
        add    dx, 3a0h
loc_6_0B8E:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0BAC
        add    dx, 5aech
        jmp    SHORT loc_6_0BB1
loc_6_0BAC:
        add    dx, 3a4h
loc_6_0BB1:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0BCF
        add    dx, 66ech
        jmp    SHORT loc_6_0BD4
loc_6_0BCF:
        add    dx, 3bch
loc_6_0BD4:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0BF2
        add    dx, 6aech
        jmp    SHORT loc_6_0BF7
loc_6_0BF2:
        add    dx, 3c8h
loc_6_0BF7:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0C15
        add    dx, 6ech
        jmp    SHORT loc_6_0C1A
loc_6_0C15:
        add    dx, 2f0h
loc_6_0C1A:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0C38
        add    dx, 0aech
        jmp    SHORT loc_6_0C3D
loc_6_0C38:
        add    dx, 2f4h
loc_6_0C3D:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0C5B
        add    dx, 0eech
        jmp    SHORT loc_6_0C60
loc_6_0C5B:
        add    dx, 2f8h
loc_6_0C60:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0C7E
        add    dx, 12ech
        jmp    SHORT loc_6_0C83
loc_6_0C7E:
        add    dx, 2fch
loc_6_0C83:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0CA1
        add    dx, 16ech
        jmp    SHORT loc_6_0CA6
loc_6_0CA1:
        add    dx, 300h
loc_6_0CA6:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0CC4
        add    dx, 1aech
        jmp    SHORT loc_6_0CC9
loc_6_0CC4:
        add    dx, 304h
loc_6_0CC9:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0CE7
        add    dx, 1eech
        jmp    SHORT loc_6_0CEC
loc_6_0CE7:
        add    dx, 308h
loc_6_0CEC:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0D0A
        add    dx, 22ech
        jmp    SHORT loc_6_0D0F
loc_6_0D0A:
        add    dx, 32ch
loc_6_0D0F:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0D2D
        add    dx, 26ech
        jmp    SHORT loc_6_0D32
loc_6_0D2D:
        add    dx, 330h
loc_6_0D32:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0D50
        add    dx, 2aech
        jmp    SHORT loc_6_0D55
loc_6_0D50:
        add    dx, 334h
loc_6_0D55:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0D73
        add    dx, 2eech
        jmp    SHORT loc_6_0D78
loc_6_0D73:
        add    dx, 34ch
loc_6_0D78:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0D96
        add    dx, 32ech
        jmp    SHORT loc_6_0D9B
loc_6_0D96:
        add    dx, 350h
loc_6_0D9B:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0DB9
        add    dx, 36ech
        jmp    SHORT loc_6_0DBE
loc_6_0DB9:
        add    dx, 354h
loc_6_0DBE:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0DDC
        add    dx, 3aech
        jmp    SHORT loc_6_0DE1
loc_6_0DDC:
        add    dx, 358h
loc_6_0DE1:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0DFF
        add    dx, 3eech
        jmp    SHORT loc_6_0E04
loc_6_0DFF:
        add    dx, 35ch
loc_6_0E04:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0E22
        add    dx, 4aech
        jmp    SHORT loc_6_0E27
loc_6_0E22:
        add    dx, 37ch
loc_6_0E27:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0E45
        add    dx, 5eech
        jmp    SHORT loc_6_0E4A
loc_6_0E45:
        add    dx, 3ach
loc_6_0E4A:
        call   sub_6_0E8F
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_6_0E68
        add    dx, 62ech
        jmp    SHORT loc_6_0E6D
loc_6_0E68:
        add    dx, 3b0h
loc_6_0E6D:
        call   sub_6_0E8F
        ret
sub_6_0E73:
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
sub_6_0E8F:
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        ret
loc_6_0EAB:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 080h, 001h, 000h, 000h
        jne    SHORT loc_6_0EBF
        cmp    ebx, edi
        jne    SHORT loc_6_0EFE
loc_6_0EBF:
        cmp    ecx, 4
        je     SHORT loc_6_0EF8
        cmp    ecx, 0
        je     SHORT loc_6_0EE5
        cmp    ecx, 0ch
        je     SHORT loc_6_0EF1
        cmp    ecx, 8
        je     SHORT loc_6_0EDE
        cmp    ecx, 14h
        je     SHORT loc_6_0EEB
        in     eax, dx
        jmp    NEAR PTR loc_6_0F83
loc_6_0EDE:
        in     ax, dx
        jmp    NEAR PTR loc_6_0F83
loc_6_0EE5:
        in     al, dx
        jmp    NEAR PTR loc_6_0F83
loc_6_0EEB:
        out    dx, eax
        jmp    NEAR PTR loc_6_0F83
loc_6_0EF1:
        out    dx, ax
        jmp    NEAR PTR loc_6_0F83
loc_6_0EF8:
        out    dx, al
        jmp    NEAR PTR loc_6_0F83
loc_6_0EFE:
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    SHORT loc_6_0F15
        cmp    dx, 1ceh
        je     SHORT loc_6_0F15
        cmp    dx, 1cfh
        jne    SHORT loc_6_0F83
loc_6_0F15:
        mov    edi, ebx
        db  003h, 03Dh, 088h, 001h, 000h, 000h
        cmp    ecx, 4
        je     SHORT loc_6_0F6B
        cmp    ecx, 0ch
        je     SHORT loc_6_0F5C
        cmp    dx, 1ceh
        jne    SHORT loc_6_0F33
        mov    al, byte ptr [edi + 40h]
        jmp    SHORT loc_6_0F83
loc_6_0F33:
        movzx  eax, byte ptr [edi + 40h]
        cmp    al, 0b6h
        je     SHORT loc_6_0F4F
        cmp    al, 0b0h
        je     SHORT loc_6_0F4F
        cmp    al, 0bah
        je     SHORT loc_6_0F4F
        cmp    al, 0bbh
        je     SHORT loc_6_0F4F
        cmp    al, 0a8h
        je     SHORT loc_6_0F4F
        cmp    al, 0a9h
        jne    SHORT loc_6_0F55
loc_6_0F4F:
        dec    edx
        out    dx, al
        inc    edx
        in     al, dx
        jmp    SHORT loc_6_0F83
loc_6_0F55:
        sub    al, 80h
        mov    al, byte ptr [eax + edi]
        jmp    SHORT loc_6_0F83
loc_6_0F5C:
        mov    byte ptr [edi + 40h], al
        sub    al, 80h
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    SHORT loc_6_0F83
loc_6_0F6B:
        cmp    edx, 1ceh
        jne    SHORT loc_6_0F78
        mov    byte ptr [edi + 40h], al
        jmp    SHORT loc_6_0F83
loc_6_0F78:
        mov    cl, al
        movzx  eax, byte ptr [edi + 40h]
        sub    al, 80h
        mov    byte ptr [eax + edi], cl
loc_6_0F83:
        pop    esi
        pop    edi
        ret
loc_6_0F86:
        push   edx
        db  03Bh, 005h, 080h, 001h, 000h, 000h
        jne    SHORT loc_6_0FC9
        db  080h, 03Dh, 0A0h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_0FC9
        mov    ecx, 1000h
loc_6_0F9D:
        mov    dx, 9aeeh
        in     ax, dx
        test   ax, 0ffffh
        jne    SHORT loc_6_0FC1
        mov    dx, 62eeh
        in     ax, dx
        test   ax, 2000h
        jne    SHORT loc_6_0FC1
        mov    dx, 9ae8h
        in     ax, dx
        test   ax, 200h
        je     SHORT loc_6_0FC9
loc_6_0FC1:
        call   sub_6_0FCB
        dec    ecx
        jne    SHORT loc_6_0F9D
loc_6_0FC9:
        pop    edx
        ret
sub_6_0FCB:
        push   ecx
        mov    ecx, 3e8h
loc_6_0FD1:
        push   ecx
        pop    ecx
        loop   loc_6_0FD1
        pop    ecx
        ret
loc_6_0FD7:
        push   edi
        push   esi
        mov    edi, ebx
        db  003h, 03Dh, 088h, 001h, 000h, 000h
        mov    word ptr [edi + 45h], ax
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 008h
        je     SHORT loc_6_0FF7
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    SHORT loc_6_100E
loc_6_0FF7:
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_6_100E
        and    al, 7fh
        cmp    al, 13h
        ja     SHORT loc_6_100E
        pushad
        call   sub_6_01C5
        popad
loc_6_100E:
        pop    esi
        pop    edi
        ret
loc_6_1011:
        db  08Bh, 00Dh, 084h, 001h, 000h, 000h
        stc
        ret
loc_6_1019:
        mov    edx, 10000h
        xor    eax, eax
        stc
        ret
loc_6_1022:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 088h, 001h, 000h, 000h
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0beh
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 3eh], al
        and    al, 0f7h
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_6_104D:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 088h, 001h, 000h, 000h
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0beh
        mov    ah, byte ptr [esi + 3eh]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_6_1071:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    ch, al
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    ax, cx
        and    ax, 70fh
        shl    al, 1
        shl    ah, 5
        or     ah, al
        mov    al, 0b2h
        out    dx, ax
        mov    ax, cx
        and    ax, 1830h
        shr    al, 4
        shr    ah, 1
        or     ah, al
        mov    al, 0aeh
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f0h
        or     al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc
        ret
loc_6_10B7:
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0b2h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0aeh
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        and    ax, 1e03h
        shr    ah, 1
        shl    al, 4
        or     al, ah
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_6_10E6:
        mov    edx, 1ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0b2h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0aeh
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        and    ax, 0e00ch
        shr    ah, 5
        shl    al, 1
        or     al, ah
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
_PTEXT ends
        end
