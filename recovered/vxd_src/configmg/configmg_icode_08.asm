; configmg_icode_08.asm -- CONFIGMG object 8 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_8_0000:
        inc    ebx
        dec    edi
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        dec    ebp
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_000C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_0010:
        dec    eax
        push   esp
        push   edx
        inc    ebp
        inc    ebp
        pop    esp
        push   edx
        dec    edi
        dec    edi
        push   esp
        pop    esp
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_8_0020:
        db  068h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   2
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0033:
        add    esp, 10h
        ret
sub_8_0037:
        push   0
        push   16h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0040:
        add    esp, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0049:
        ret
sub_8_004A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 0ah], ax
        cmp    word ptr [ebp - 0ah], 400h
        jae    SHORT loc_8_006C
        call   sub_8_0037
loc_8_006C:
        db  0CDh, 020h, 000h, 000h, 027h, 000h
        mov    word ptr [ebp - 8], ax
        cmp    word ptr [ebp - 8], 100h
        jae    SHORT loc_8_0083
        call   sub_8_0037
loc_8_0083:
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6], 400h
        jae    SHORT loc_8_009A
        call   sub_8_0037
loc_8_009A:
        db  0CDh, 020h, 000h, 000h, 005h, 000h
        xchg   word ptr [ebp - 4], ax
        xchg   dword ptr [ebp - 10h], ebx
        xchg   dword ptr [ebp - 10h], ecx
        cmp    word ptr [ebp - 4], 310h
        jae    SHORT loc_8_00B7
        call   sub_8_0037
loc_8_00B7:
        db  0CDh, 020h, 000h, 000h, 003h, 000h
        xchg   word ptr [ebp - 2], ax
        xchg   dword ptr [ebp - 10h], ebx
        xchg   dword ptr [ebp - 10h], ecx
        cmp    word ptr [ebp - 2], 400h
        jae    SHORT loc_8_00D4
        call   sub_8_0037
loc_8_00D4:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_00E1:
        mov    edx, esi
        mov    ecx, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_00EA:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_8_00FA
        xor    eax, eax
        jmp    NEAR PTR loc_8_0180
loc_8_00FA:
        xor    esi, esi
loc_8_00FC:
        push   0
        xor    edx, edx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_010C:
        test   eax, eax
        je     SHORT loc_8_0132
        inc    esi
        cmp    esi, 7
        jb     SHORT loc_8_00FC
        push   201h
        push   24h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0122:
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_8_0136
        xor    eax, eax
        jmp    SHORT loc_8_0180
loc_8_0132:
        xor    eax, eax
        jmp    SHORT loc_8_0180
loc_8_0136:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   14h
        mov    dword ptr [eax + 1ch], ecx
        push   5
        mov    dword ptr [eax + 8], 1
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        add    eax, 24h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_8_0330
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_8_0180
        push   14h
        push   4
        call   sub_8_0330
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_8_0180:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_8_0185:
        call   sub_8_0344
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0194:
        test   al, 4
        je     SHORT loc_8_01A1
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 008h
        jmp    SHORT loc_8_01AC
loc_8_01A1:
        test   al, 1
        je     SHORT loc_8_01AC
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 010h
loc_8_01AC:
        db  0A1h, 02Ch, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_01BB:
        test   eax, eax
        je     SHORT loc_8_01C6
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 020h
loc_8_01C6:
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_01CB:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_8_01D8
        xor    eax, eax
        jmp    SHORT loc_8_01F5
loc_8_01D8:
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_01DD:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_8_01F5
        call   sub_8_0020
        mov    eax, 1
loc_8_01F5:
        ret
loc_8_01F6:
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 002h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0203:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 010h
        jne    SHORT loc_8_0211
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0211:
        xor    esi, esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0224:
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   90000000h
        push   808h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_023C:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_024A:
        add    esp, 10h
        pop    esi
        ret
loc_8_024F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_025B:
        push   0
        xor    edx, edx
        push   esi
        mov    ecx, 13h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_026B:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 13h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_027B:
        mov    eax, 1
        pop    esi
        ret
sub_8_0282:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        and    al, 3
        sub    esp, 4
        cmp    al, 3
        je     SHORT loc_8_0297
        xor    eax, eax
        jmp    SHORT loc_8_0312
loc_8_0297:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_8_02B3
        push   0
        push   1
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_8_0360
        add    esp, 10h
loc_8_02B3:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, 2
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 001h
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_02C9:
        db  0CDh, 020h, 000h, 000h, 026h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        jb     SHORT loc_8_02F8
        push   0c0000000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_8_0358
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_8_02F8
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 010h
loc_8_02F8:
        push   40000000h
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_8_0350
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_8_0312:
        mov    esp, ebp
        pop    ebp
        ret
sub_8_0316:
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 002h
        mov    ecx, 1ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_8_0327:
        mov    eax, 1
        ret
        db  0CCh, 0CCh, 0CCh
sub_8_0330:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_8_0344:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_8_0350:
        db  0CDh, 020h, 011h, 080h, 017h, 000h
        int3
        int3
sub_8_0358:
        db  0CDh, 020h, 00Ah, 080h, 026h, 000h
        int3
        int3
sub_8_0360:
        db  0CDh, 020h, 0D2h, 080h, 001h, 000h
_ITEXT ends
        end
