; ndis_icode_06.asm -- NDIS object 6 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_6_0000:
        sub    ecx, ecx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    edx, eax
        add    edx, 80h
loc_6_0010:
        inc    ecx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    edx, eax
        ja     SHORT loc_6_0010
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        in     al, 21h
        jmp    SHORT loc_6_0025
loc_6_0025:
        push   eax
        mov    al, 0feh
        out    21h, al
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    edx, eax
loc_6_0032:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    edx, eax
        je     SHORT loc_6_0032
        mov    edx, eax
        mov    ecx, 100000h
        nop
loc_6_0044:
        loop   loc_6_0044
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, edx
        mov    ecx, 3e8h
        mul    ecx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        mov    eax, 100000h
        sub    edx, edx
        div    ecx
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        pop    eax
        out    21h, al
        cld
        ret
sub_6_0070:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 000h, 000h, 000h, 000h
        cld
loc_6_0083:
        lodsb
        or     al, al
        je     SHORT loc_6_008B
        stosb
        jmp    SHORT loc_6_0083
loc_6_008B:
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_0090:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_6_0098
        jmp    SHORT loc_6_0090
loc_6_0098:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh
sub_6_00A0:
        sub    esp, 10h
        lea    eax, [esp + 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_00B7:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_00CA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_011E
loc_6_00CA:
        lea    eax, [esp]
        lea    ecx, [esp + 8]
        mov    edx, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [esp + 4], 4
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_00F2:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_6_0107
        cmp    dword ptr [esp], 4
        jne    SHORT loc_6_0107
        cmp    dword ptr [esp + 8], 3
        je     SHORT loc_6_0111
loc_6_0107:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_0111:
        mov    eax, dword ptr [esp + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_011B:
        add    esp, 4
loc_6_011E:
        lea    eax, [esp]
        lea    ecx, [esp + 0ch]
        lea    edx, [esp + 8]
        push   eax
        mov    dword ptr [esp + 4], 4
        push   ecx
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   80000005h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0146:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_6_015E
        test   byte ptr [esp + 0ch], 4
        je     SHORT loc_6_015E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_015E:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0175:
        add    esp, 8
        xor    eax, eax
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_6_0180:
        call   sub_6_0220
        xor    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_0190:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_01A0:
        add    esp, 8
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_01BB
loc_6_01AC:
        mov    esi, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_01B5:
        mov    eax, esi
        test   esi, esi
        jne    SHORT loc_6_01AC
loc_6_01BB:
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_01EC
        mov    ebx, 1
loc_6_01CA:
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 68h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_01D6:
        cmp    eax, 2
        je     SHORT loc_6_01E0
        cmp    byte ptr [esi + 34h], bl
        jne    SHORT loc_6_01E6
loc_6_01E0:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_01E6:
        mov    esi, edi
        test   edi, edi
        jne    SHORT loc_6_01CA
loc_6_01EC:
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_0213
loc_6_01F6:
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 74h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0202:
        cmp    eax, 2
        jne    SHORT loc_6_020D
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_020D:
        mov    esi, edi
        test   edi, edi
        jne    SHORT loc_6_01F6
loc_6_0213:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_0220:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_ITEXT ends
        end
