; mssp_icode_02.asm -- MSSP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

SECPROV_DEVICE_INIT:
        call   sub_2_0235
        or     eax, eax
        je     SHORT loc_2_000A
        stc
loc_2_000A:
        ret
SECPROV_INIT_COMPLETE:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jae    SHORT loc_2_0015
        jmp    SHORT loc_2_004F
loc_2_0015:
        db  0CDh, 020h, 000h, 000h, 091h, 004h
        jae    SHORT loc_2_001F
        jmp    SHORT loc_2_004F
loc_2_001F:
        db  0CDh, 020h, 000h, 000h, 080h, 004h
        jae    SHORT loc_2_0029
        jmp    SHORT loc_2_004F
loc_2_0029:
        db  0CDh, 020h, 000h, 000h, 081h, 004h
        jae    SHORT loc_2_0033
        jmp    SHORT loc_2_004F
loc_2_0033:
        db  066h, 083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        je     SHORT loc_2_0046
        call   sub_2_0299
        or     eax, eax
        jne    SHORT loc_2_004F
loc_2_0046:
        call   sub_2_0173
        or     eax, eax
        je     SHORT loc_2_0055
loc_2_004F:
        call   sub_2_024A
        stc
loc_2_0055:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0060:
        sub    esp, 0ch
        push   esi
        lea    eax, [esp + 8]
        push   eax
        db  068h, 088h, 003h, 000h, 000h
        push   80000002h
        call   sub_2_02B0
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_016E
        mov    dword ptr [esp + 0ch], 4
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 8]
        db  068h, 04Ch, 002h, 000h, 000h
        push   eax
        push   0
        db  068h, 09Ch, 003h, 000h, 000h
        push   dword ptr [esp + 1ch]
        call   sub_2_02C0
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_2_0160
        cmp    dword ptr [esp + 4], 4
        je     SHORT loc_2_00D4
        cmp    dword ptr [esp + 4], 3
        jne    NEAR PTR loc_2_0157
        cmp    dword ptr [esp + 0ch], 4
        jne    NEAR PTR loc_2_0157
loc_2_00D4:
        db  0A1h, 04Ch, 002h, 000h, 000h
        cmp    eax, 1
        jb     SHORT loc_2_015B
        cmp    eax, 2
        jbe    SHORT loc_2_00E5
        jmp    SHORT loc_2_015B
loc_2_00E5:
        db  0A1h, 04Ch, 002h, 000h, 000h
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        db  0A3h, 048h, 002h, 000h, 000h
        db  068h, 000h, 009h, 000h, 000h
        lea    eax, [esp + 8]
        db  068h, 054h, 002h, 000h, 000h
        push   eax
        push   0
        db  068h, 0ACh, 003h, 000h, 000h
        push   dword ptr [esp + 1ch]
        call   sub_2_02C0
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_0160
        cmp    dword ptr [esp + 4], 1
        jne    SHORT loc_2_0157
        db  068h, 008h, 00Ah, 000h, 000h
        lea    eax, [esp + 8]
        db  068h, 004h, 009h, 000h, 000h
        push   eax
        push   0
        db  068h, 0B8h, 003h, 000h, 000h
        push   dword ptr [esp + 1ch]
        call   sub_2_02C0
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_0160
        cmp    dword ptr [esp + 4], 1
        je     SHORT loc_2_0160
loc_2_0157:
        or     esi, esi
        jne    SHORT loc_2_0160
loc_2_015B:
        mov    esi, 0ffffffffh
loc_2_0160:
        push   dword ptr [esp + 8]
        call   sub_2_02B8
        add    esp, 4
        mov    eax, esi
loc_2_016E:
        pop    esi
        add    esp, 0ch
        ret
sub_2_0173:
        sub    esp, 4
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        je     SHORT loc_2_0197
        db  0A1h, 050h, 002h, 000h, 000h
        lea    ecx, [eax + 10h]
        db  089h, 00Dh, 01Ch, 00Ah, 000h, 000h
        add    eax, 8
        db  0A3h, 020h, 00Ah, 000h, 000h
        jmp    SHORT loc_2_01AB
loc_2_0197:
        db  0C7h, 005h, 01Ch, 00Ah, 000h, 000h, 054h, 002h, 000h, 000h
        db  0C7h, 005h, 020h, 00Ah, 000h, 000h, 000h, 009h, 000h, 000h
loc_2_01AB:
        lea    eax, [esp]
        push   eax
        push   4
        db  068h, 044h, 003h, 000h, 000h
        db  068h, 0F0h, 002h, 000h, 000h
        push   0
        db  068h, 0C8h, 003h, 000h, 000h
        call   sub_2_02C8
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_01D1
        sub    eax, eax
loc_2_01D1:
        add    esp, 4
        ret
sub_2_01D5:
        push   esi
        sub    esi, esi
        push   esi
        push   esi
        call   sub_2_02E0
        add    esp, 8
        db  0A3h, 020h, 000h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_2_01F2
        mov    eax, 8
        jmp    SHORT loc_2_0233
loc_2_01F2:
        push   200h
        push   78h
        call   sub_2_02D0
        add    esp, 8
        db  0A3h, 050h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_2_0211
        mov    esi, 8
        jmp    SHORT loc_2_021B
loc_2_0211:
        db  0C7h, 005h, 054h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
loc_2_021B:
        or     esi, esi
        je     SHORT loc_2_0231
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_0231
        push   eax
        call   sub_2_02E8
        add    esp, 4
loc_2_0231:
        mov    eax, esi
loc_2_0233:
        pop    esi
        ret
sub_2_0235:
        call   sub_2_0060
        or     eax, eax
        jne    SHORT loc_2_0249
        call   sub_2_01D5
        or     eax, eax
        jne    SHORT loc_2_0249
        sub    eax, eax
loc_2_0249:
        ret
sub_2_024A:
        db  083h, 03Dh, 084h, 003h, 000h, 000h, 000h
        je     SHORT loc_2_025E
        db  0FFh, 035h, 084h, 003h, 000h, 000h
        db  0E8h, 09Dh, 00Eh, 0FEh, 0FFh
loc_2_025E:
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_027A
        push   eax
        call   sub_2_02E8
        add    esp, 4
        db  0C7h, 005h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_027A:
        db  0A1h, 050h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_0298
        push   0
        push   eax
        call   sub_2_02D8
        add    esp, 8
        db  0C7h, 005h, 050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0298:
        ret
sub_2_0299:
        db  068h, 084h, 003h, 000h, 000h
        push   1
        push   0
        db  068h, 054h, 002h, 000h, 000h
        db  0E8h, 0A8h, 00Fh, 0FEh, 0FFh
        ret
        db  000h, 000h, 000h
sub_2_02B0:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02B8:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02C0:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02C8:
        db  0CDh, 020h, 076h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02D0:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02D8:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02E0:
        db  0CDh, 020h, 001h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_02E8:
        db  0CDh, 020h, 002h, 081h, 001h, 000h
        add    byte ptr [eax], al
loc_2_02F0:
        db  0A2h, 017h, 000h, 000h, 0A2h
        pop    ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0308:
        push   eax
        insb
        popad
        db  074h, 066h
        outsd
        db  072h, 06Dh
        pop    edi
        push   esp
        jns    SHORT loc_2_0384
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6fh], al
        outsb
        je     SHORT loc_2_037E
        imul   ebp, dword ptr [esi + 65h], 72h
loc_2_0324:
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_2_039E
        pop    edi
        push   ebx
        jb     NEAR PTR loc_2_03A6
        jb     NEAR PTR loc_2_0333
loc_2_0333:
        add    byte ptr [ecx + 64h], al
        jb     NEAR PTR loc_2_039E
        jae    SHORT loc_2_03AE
        pop    edi
        inc    edx
        outsd
        outsd
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0344:
        or     byte ptr [ebx], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     al, 0ah
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        and    al, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        xor    al, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0ah
loc_2_037E:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_0384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0388:
        push   ebx
        arpl   word ptr gs:[ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 5ch], 766f7250h
        imul   esp, dword ptr [ebp + 72h], 50000000h
        insb
loc_2_039E:
        popad
        db  074h, 066h
        outsd
        db  072h, 06Dh
        pop    edi
        push   esp
loc_2_03A6:
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6fh], al
loc_2_03AE:
        outsb
        db  074h, 061h
        imul   ebp, dword ptr [esi + 65h], 72h
loc_2_03B8:
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        pop    edi
        inc    edx
        outsd
        outsd
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_03C8:
        push   ebx
        arpl   word ptr gs:[ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 5ch], 766f7250h
        db  069h, 064h, 065h, 072h, 000h, 000h, 000h
_ITEXT ends
        end
