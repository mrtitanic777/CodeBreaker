; mswebsp_icode_02.asm -- MSWEBSP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  083h, 0ECh, 008h, 0B8h, 00Ch, 002h, 000h, 000h, 0BAh, 07Ch, 001h, 000h, 000h, 08Bh, 008h, 089h
        db  00Ah, 08Bh, 040h, 004h, 089h, 042h, 004h, 08Dh, 044h, 024h, 000h, 050h, 068h, 014h, 002h, 000h
        db  000h, 068h, 002h, 000h, 000h, 080h, 0E8h, 02Dh, 001h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h
        db  075h, 032h, 0C7h, 044h, 024h, 004h, 001h, 000h, 000h, 000h, 08Dh, 044h, 024h, 004h, 06Ah, 004h
        db  050h, 06Ah, 004h, 08Bh, 044h, 024h, 00Ch, 068h, 028h, 002h, 000h, 000h, 050h, 0E8h, 016h, 001h
        db  000h, 000h, 08Bh, 044h, 024h, 014h, 083h, 0C4h, 014h, 050h, 0E8h, 001h, 001h, 000h, 000h, 083h
        db  0C4h, 004h, 02Bh, 0C0h, 083h, 0C4h, 008h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0070:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   4
        db  068h, 0CCh, 001h, 000h, 000h
        db  068h, 078h, 001h, 000h, 000h
        push   0
        db  068h, 068h, 002h, 000h, 000h
        call   sub_2_0170
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_0099
        sub    eax, eax
loc_2_0099:
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
SECPROV_INIT_COMPLETE:
        db  0C7h, 005h, 014h, 001h, 000h, 000h, 050h, 000h, 000h, 000h
        db  0C7h, 005h, 018h, 001h, 000h, 000h, 0A0h, 000h, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     SHORT loc_2_00F2
        db  0A3h, 008h, 001h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 091h, 004h
        jb     SHORT loc_2_00F0
        call   0
        or     ax, ax
        jne    SHORT loc_2_00EE
        call   sub_2_0070
        or     eax, eax
        jne    SHORT loc_2_00F9
        jmp    SHORT loc_2_00EA
        db  0C6h, 005h, 025h, 001h, 000h, 000h, 000h
loc_2_00EA:
        clc
loc_2_00EB:
        ret
        db  0EBh, 00Bh
loc_2_00EE:
        jmp    SHORT loc_2_00F9
loc_2_00F0:
        jmp    SHORT loc_2_00F9
loc_2_00F2:
        jmp    SHORT loc_2_00F9
        db  0EBh, 003h, 0F8h, 0EBh, 0F1h
loc_2_00F9:
        stc
        jmp    SHORT loc_2_00EB
        db  083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh
        db  020h, 082h, 000h, 001h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 066h, 0C7h
        db  045h, 01Ch, 000h, 0EAh, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0CDh
        db  020h, 084h, 000h, 001h, 000h, 066h, 08Bh, 045h, 010h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h
        db  08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 066h, 083h
        db  0F8h, 001h, 072h, 006h, 0CDh, 020h, 000h, 000h, 08Eh, 004h, 0C3h, 000h, 0CDh, 020h, 048h, 081h
        db  001h, 000h, 000h, 000h, 0CDh, 020h, 049h, 081h, 001h, 000h, 000h, 000h, 0CDh, 020h, 04Eh, 081h
        db  001h, 000h, 000h, 000h
sub_2_0170:
        db  0CDh, 020h, 076h, 081h, 001h, 000h
        add    byte ptr [eax], al
loc_2_0178:
        lock add eax, dword ptr [eax]
        db  000h, 0F0h
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 6ch], dl
        popad
        db  074h, 066h
        outsd
        db  072h, 06Dh
        pop    edi
        push   esp
        jns    SHORT loc_2_020C
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6fh], al
        outsb
        je     SHORT loc_2_0206
        imul   ebp, dword ptr [esi + 65h], 72h
loc_2_01AC:
        inc    ecx
        jb     NEAR PTR loc_2_0216
        db  073h, 073h
        pop    edi
        push   ebx
        jb     NEAR PTR loc_2_022E
        jb     NEAR PTR loc_2_01BB
loc_2_01BB:
        add    byte ptr [ecx + 64h], al
        db  064h, 072h, 065h
        db  073h, 073h
        pop    edi
        inc    edx
        outsd
        outsd
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_01CC:
        nop
        add    dword ptr [eax], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        pop    eax
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  000h, 0A0h, 001h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 060h, 001h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        lodsb
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 068h, 001h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    esp, 1
        add    byte ptr [eax], al
        db  000h, 070h, 001h
loc_2_0206:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_020C:
        cmp    al, 4ch
        outsd
        arpl   word ptr [ecx + 6ch], sp
        add    byte ptr ds:[ebx + 65h], dl
loc_2_0216:
        arpl   word ptr [ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 5ch], 766f7250h
        imul   esp, dword ptr [ebp + 72h], 50000000h
        insb
        popad
        db  074h, 066h
        outsd
loc_2_022E:
        db  072h, 06Dh
        pop    edi
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6fh], al
        outsb
        db  074h, 061h
        imul   ebp, dword ptr [esi + 65h], 72h
        push   edi
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   edi
        push   ebx
        pop    edi
        push   eax
        inc    ecx
        push   ebx
        push   ebx
        push   esp
        dec    eax
        push   edx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        pop    edi
        inc    edx
        outsd
        outsd
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0268:
        push   ebx
        arpl   word ptr gs:[ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 5ch], 766f7250h
        imul   esp, dword ptr [ebp + 72h], 4e000000h
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        add    byte ptr [esi + 57h], cl
        xor    eax, dword ptr [ecx + 63h]
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 053h
        db  065h, 072h, 076h
        db  065h, 072h, 000h
_ITEXT ends
        end
