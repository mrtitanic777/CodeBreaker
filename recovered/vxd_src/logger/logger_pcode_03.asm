; logger_pcode_03.asm -- LOGGER object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, 0d500h
        mov    bx, 2
        mov    cx, 0
        mov    dl, 11h
        mov    dh, 0
        mov    esi, dword ptr [ebp - 8]
        call   sub_3_070A
        jae    NEAR PTR loc_3_0033
        xor    eax, eax
loc_3_0033:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_3_005B
        mov    eax, 0d800h
        mov    ebx, dword ptr [ebp - 4]
        call   sub_3_070A
        jae    NEAR PTR loc_3_0058
        mov    eax, 0ffffffffh
loc_3_0058:
        mov    dword ptr [ebp - 0ch], eax
loc_3_005B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 104h], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_3_0077
loc_3_0077:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
LOGGER_Service_03:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_3_0096
        mov    dword ptr [ebp + 0ch], 1000h
loc_3_0096:
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 119h
        push   eax
        db  0E8h, 05Ah, 0FFh, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_3_00BD
        sub    eax, eax
        jmp    NEAR PTR loc_3_019F
loc_3_00BD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10ch], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 1]
        cmp    eax, 3ah
        je     NEAR PTR loc_3_0114
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        cmp    eax, 5ch
        je     NEAR PTR loc_3_0114
        call   sub_3_0740
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    edx, edi
        mov    edi, dword ptr [ebp - 4]
        add    edi, 4
        mov    esi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_0114:
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, edi
        mov    ebx, ecx
        mov    edi, dword ptr [ebp - 4]
        add    edi, 4
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_3_0184
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 104h], 0
        jae    NEAR PTR loc_3_017F
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_017F:
        jmp    NEAR PTR loc_3_0197
loc_3_0184:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_0197:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_3_019F
loc_3_019F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_3_01A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 114h], 4
        je     NEAR PTR loc_3_01C5
        jmp    NEAR PTR loc_3_0303
loc_3_01C5:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 114h], 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 118h
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], 0
loc_3_01E9:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 104h]
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 108h]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, 0d601h
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 1ch]
        mov    esi, dword ptr [ebp - 14h]
        call   sub_3_070A
        jae    NEAR PTR loc_3_0228
        mov    eax, 0ffffffffh
loc_3_0228:
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jb     NEAR PTR loc_3_0241
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ecx + 104h], eax
loc_3_0241:
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 108h], ecx
        jne    NEAR PTR loc_3_01E9
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 108h], 0
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 110h]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 0
        jne    NEAR PTR loc_3_02EF
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 114h], 2
        je     NEAR PTR loc_3_02CC
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 114h], 1
        jne    NEAR PTR loc_3_02CC
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_3_02B6
        mov    eax, 0d700h
        mov    ebx, dword ptr [ebp - 8]
        call   sub_3_070A
loc_3_02B6:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 114h], 0fffffffdh
loc_3_02CC:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 114h], 1
        je     NEAR PTR loc_3_02EF
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0440
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_3_02EF:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_3_0303
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 114h], 0fffffffbh
loc_3_0303:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
LOGGER_Service_01:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 104h], 40000h
        jbe    NEAR PTR loc_3_033F
        jmp    NEAR PTR loc_3_043B
loc_3_033F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10ch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 108h]
        cmp    eax, 100h
        jae    NEAR PTR loc_3_0361
        jmp    NEAR PTR loc_3_043B
loc_3_0361:
        xor    eax, eax
        db  066h, 0A1h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        add    eax, dword ptr [ebp + 8]
        add    eax, 118h
        push   eax
        call   sub_3_053D
        add    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 108h], eax
        lea    eax, [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        add    eax, dword ptr [ebp + 8]
        add    eax, 118h
        push   eax
        db  0E8h, 017h, 0FCh, 0FCh, 0FFh
        add    esp, 30h
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 108h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10ch]
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 108h]
        ja     NEAR PTR loc_3_043B
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 110h]
        mov    edx, dword ptr [ebp + 8]
        db  08Dh, 035h, 0A4h, 001h, 000h, 000h
        mov    ecx, 40h
        call   sub_3_06F4
loc_3_043B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_0440:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_3_0474
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_3_0474
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0d700h
        mov    ebx, dword ptr [ebp - 4]
        call   sub_3_070A
loc_3_0474:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_3_048C
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 07Fh, 0FBh, 0FCh, 0FFh
        add    esp, 8
loc_3_048C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
LOGGER_Service_02:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_3_04DC
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 114h], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 0
        je     NEAR PTR loc_3_04C0
        jmp    NEAR PTR loc_3_04DC
loc_3_04C0:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 110h]
        mov    edx, dword ptr [ebp + 8]
        db  08Dh, 035h, 0A4h, 001h, 000h, 000h
        mov    ecx, 40h
        call   sub_3_06F4
loc_3_04DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
LOGGER_Service_04:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_3_0538
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_3_0538
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 114h], 2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 0
        je     NEAR PTR loc_3_051C
        jmp    NEAR PTR loc_3_0538
loc_3_051C:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 110h]
        mov    edx, dword ptr [ebp + 8]
        db  08Dh, 035h, 0A4h, 001h, 000h, 000h
        mov    ecx, 40h
        call   sub_3_06F4
loc_3_0538:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_053D:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_3_0710
        add    esp, 4
        mov    dword ptr [ebp - 2ch], eax
        mov    ecx, 15180h
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 1ch], eax
        mov    ecx, 15180h
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 2ch], edx
        mov    ecx, 3ch
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 14h], dx
        mov    ecx, 3ch
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 2ch], eax
        mov    ecx, 3ch
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 4], dx
        mov    ecx, 3ch
        mov    eax, dword ptr [ebp - 2ch]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 30h], ax
        mov    ecx, 5b5h
        mov    eax, dword ptr [ebp - 1ch]
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 28h], ax
        mov    ecx, 5b5h
        mov    eax, dword ptr [ebp - 1ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 20h], edx
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        shl    eax, 2
        add    eax, 7bch
        mov    word ptr [ebp - 34h], ax
        db  0C7h, 005h, 084h, 000h, 000h, 000h, 01Dh, 000h, 000h, 000h
        mov    word ptr [ebp - 18h], 16eh
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        js     NEAR PTR loc_3_0640
        dec    word ptr [ebp - 18h]
loc_3_0611:
        cmp    dword ptr [ebp - 20h], 0
        jl     NEAR PTR loc_3_0636
        inc    word ptr [ebp - 34h]
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_3_0611
loc_3_0636:
        db  0C7h, 005h, 084h, 000h, 000h, 000h, 01Ch, 000h, 000h, 000h
loc_3_0640:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_3_065A
loc_3_0657:
        inc    dword ptr [ebp - 0ch]
loc_3_065A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 20h]
        db  039h, 00Ch, 085h, 080h, 000h, 000h, 000h
        jg     NEAR PTR loc_3_0683
        sub    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        db  02Bh, 004h, 08Dh, 080h, 000h, 000h, 000h
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_3_0657
loc_3_0683:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 02Ch, 0F9h, 0FCh, 0FFh
        add    esp, 24h
        jmp    NEAR PTR loc_3_06EC
loc_3_06EC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_3_06F4:
        mov    eax, 0
        mov    ebx, 0
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
        db  0CDh, 020h, 07Eh, 081h, 001h, 000h
sub_3_070A:
        db  0CDh, 020h, 032h, 080h, 040h, 000h
sub_3_0710:
        push   ebp
        mov    ebp, esp
        db  0CDh, 020h, 008h, 000h, 005h, 000h
        imul   edx, edx, 15180h
        push   edx
        mov    ecx, 3e8h
        xor    edx, edx
        div    ecx
        push   eax
        mov    eax, edx
        xor    edx, edx
        mov    ecx, 0ah
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], eax
        pop    eax
        pop    edx
        add    eax, edx
        leave
        ret
sub_3_0740:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
LOGGER_Control:
        clc
        ret
LOGGER_Service_00:
        mov    eax, 1
        clc
        ret
        db  023h, 04Ch, 04Fh
_PTEXT ends
        end
