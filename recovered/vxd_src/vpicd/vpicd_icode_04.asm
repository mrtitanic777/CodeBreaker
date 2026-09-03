; vpicd_icode_04.asm -- VPICD object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_4_0000:
        db  066h, 0A1h, 0FAh, 026h, 000h, 000h
        db  068h, 028h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03F3
        db  066h, 0A1h, 012h, 027h, 000h, 000h
        db  068h, 040h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03F3
        db  066h, 0A1h, 0FCh, 026h, 000h, 000h
        db  068h, 058h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03F3
        db  066h, 0A1h, 00Eh, 027h, 000h, 000h
        db  068h, 070h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03F3
        db  066h, 0A1h, 014h, 027h, 000h, 000h
        db  068h, 088h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    NEAR PTR sub_4_03F3
        db  000h, 000h, 00Dh, 00Ah, 02Ah, 02Ah, 02Ah, 056h, 050h, 049h, 043h, 044h, 020h, 044h, 045h, 042h
        db  055h, 047h, 020h, 049h, 04Eh, 046h, 04Fh, 052h, 04Dh, 041h, 054h, 04Fh, 04Eh, 020h, 02Ah, 02Ah
        db  02Ah, 00Dh, 00Ah, 00Dh, 00Ah, 05Bh, 031h, 05Dh, 020h, 047h, 06Ch, 06Fh, 062h, 061h, 06Ch, 020h
        db  050h, 049h, 043h, 020h, 069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh
        db  00Ah, 05Bh, 032h, 05Dh, 020h, 050h, 065h, 072h, 02Dh, 056h, 04Dh, 020h, 050h, 049h, 043h, 020h
        db  069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh, 00Ah, 05Bh, 033h, 05Dh
        db  020h, 049h, 052h, 051h, 020h, 068h, 061h, 06Eh, 064h, 06Ch, 065h, 072h, 020h, 069h, 06Eh, 066h
        db  06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 00Dh, 00Ah, 00Dh, 00Ah, 03Eh, 000h, 00Dh, 00Ah
        db  00Dh, 00Ah, 000h, 0FCh, 064h, 0ACh, 03Ch, 020h, 074h, 0FAh, 084h, 0C0h, 074h, 004h, 03Ch, 03Bh
        db  075h, 024h, 068h, 070h, 000h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0C4h
        db  000h, 001h, 000h, 074h, 030h, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 068h, 0EEh, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 02Ch, 031h, 072h, 01Bh, 074h, 01Ah, 048h, 00Fh, 084h, 0FAh
        db  000h, 000h, 000h, 048h, 00Fh, 084h, 024h, 002h, 000h, 000h, 068h, 0A0h, 004h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 0C3h, 0E8h, 093h, 002h, 000h, 000h, 068h, 0BEh, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h, 032h, 01Eh, 000h, 000h, 0E8h, 094h, 002h, 000h
        db  000h, 068h, 0D6h, 004h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h, 030h, 01Eh
        db  000h, 000h, 0E8h, 07Eh, 002h, 000h, 000h, 068h, 0EEh, 004h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0B0h, 00Ah, 0E6h, 0A0h, 0EBh, 000h, 0E4h, 0A0h, 08Ah, 0E0h, 0B0h, 00Ah, 0E6h, 020h
        db  0EBh, 000h, 0E4h, 020h, 0E8h, 05Ch, 002h, 000h, 000h, 033h, 0C0h, 033h, 0DBh, 033h, 0D2h, 0BEh
        db  068h, 01Eh, 000h, 000h, 033h, 0C9h, 083h, 07Eh, 008h, 000h, 075h, 003h, 00Fh, 0ABh, 0CBh, 0F6h
        db  046h, 006h, 004h, 074h, 003h, 00Fh, 0ABh, 0C8h, 0F6h, 046h, 006h, 002h, 074h, 000h, 083h, 0C6h
        db  040h, 041h, 083h, 0F9h, 00Fh, 076h, 0DFh, 068h, 006h, 005h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0E8h, 01Eh, 002h, 000h, 000h, 068h, 01Eh, 005h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 08Bh, 0C3h, 0E8h, 00Ch, 002h, 000h, 000h, 068h, 036h, 005h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 08Bh, 0C2h, 0E8h, 0FAh, 001h, 000h, 000h, 0E8h, 002h, 0FEh, 0FFh, 0FFh
        db  068h, 04Eh, 005h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 066h, 0A1h, 036h, 01Eh, 000h
        db  000h, 0E8h, 0DFh, 001h, 000h, 000h, 068h, 066h, 005h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 066h, 0A1h, 038h, 01Eh, 000h, 000h, 0E8h, 0C9h, 001h, 000h, 000h, 0C3h, 0CDh, 020h, 003h
        db  000h, 001h, 000h, 0EBh, 022h
loc_4_0233:
        db  068h, 07Eh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     NEAR PTR loc_4_0350
        db  068h, 0ECh, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        add    edi, ebx
        mov    eax, dword ptr [ebx + 0ch]
        db  068h, 0ABh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        movzx  ecx, word ptr [edi + 16h]
        jecxz  loc_4_027C
        db  068h, 0DAh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_027C:
        mov    ecx, dword ptr [edi]
        jecxz  loc_4_028B
        db  068h, 0FFh, 005h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_028B:
        cmp    dword ptr [edi + 4], 0
        je     SHORT loc_4_029C
        db  068h, 017h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_029C:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_4_02AD
        db  068h, 037h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_02AD:
        mov    ax, word ptr [edi + 14h]
        cmp    ax, 7008h
        je     SHORT loc_4_02CD
        db  068h, 05Ch, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 090h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_02CD:
        db  068h, 0EEh, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03DC
        db  068h, 0C4h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    ax, word ptr [edi + 12h]
VPICD_Post_Sys_Critical_Init:
        call   sub_4_03F3
        xor    ecx, ecx
        xor    eax, eax
loc_4_02F5:
        cmp    word ptr [edi + ecx*2 + 60h], 0
        je     SHORT loc_4_0300
        bts    eax, ecx
loc_4_0300:
        inc    ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_4_02F5
        db  068h, 0DCh, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        call   sub_4_03F3
        db  068h, 0F4h, 006h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    ax, word ptr [edi + 10h]
        call   sub_4_03F3
        db  068h, 00Ch, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    ax, word ptr [edi + 0ch]
        call   sub_4_03F3
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_4_0233
loc_4_0350:
        db  068h, 0EEh, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        ret
        db  0BEh, 068h, 01Eh, 000h, 000h, 033h, 0EDh, 08Bh, 07Eh, 018h, 0EBh, 066h, 081h, 07Fh, 010h, 092h
        db  004h, 000h, 000h, 074h, 05Ah, 08Bh, 0C5h, 0CDh, 020h, 0C6h, 000h, 001h, 000h, 08Bh, 04Fh, 00Ch
        db  08Bh, 057h, 024h, 068h, 024h, 007h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 04Fh
        db  010h, 08Bh, 057h, 014h, 068h, 04Eh, 007h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh
        db  04Fh, 018h, 08Bh, 057h, 01Ch, 068h, 07Ah, 007h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  081h, 07Fh, 028h, 053h, 049h, 052h, 051h, 074h, 00Bh, 068h, 0AAh, 007h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 068h, 0EEh, 000h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh
        db  07Fh, 008h, 085h, 0FFh, 075h, 096h, 083h, 0C6h, 040h, 045h, 083h, 0FDh, 00Fh, 076h, 088h, 0C3h
sub_4_03DC:
        db  068h, 0C4h, 007h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 00Eh, 008h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        ret
sub_4_03F3:
        push   ecx
        xor    ecx, ecx
loc_4_03F6:
        bt     eax, ecx
        jb     SHORT loc_4_0408
        db  068h, 058h, 008h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_4_0413
loc_4_0408:
        db  068h, 05Ch, 008h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_4_0413:
        inc    ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_4_03F6
        db  068h, 060h, 008h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        pop    ecx
        ret
        db  000h, 000h, 041h, 06Ch, 06Ch, 06Fh, 063h, 027h, 064h, 020h, 062h, 079h, 020h, 061h, 072h, 062h
        db  069h, 074h, 072h, 061h, 074h, 06Fh, 072h, 03Ah, 020h, 000h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 050h, 072h
loc_4_044A:
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        push   61657261h
        bound  ebp, qword ptr [ebp + 3ah]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    esp
        jbe    NEAR PTR loc_4_04E1
        insb
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx + 65h], dl
        jae    SHORT loc_4_04F9
        db  072h, 076h
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        sub    ch, byte ptr [edx]
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 56h], 44434950h
        and    byte ptr [ebx + 75h], dh
        bound  esp, qword ptr [ebx + 6fh]
        insd
        insd
        popad
        outsb
        or     eax, 6850000ah
        db  079h, 073h
        imul   esp, dword ptr [ebx + 61h], 20796c6ch
        imul   ebp, dword ptr [esi + 20h], 76726573h
        imul   esp, dword ptr [ebx + 65h], 5000203ah
        push   63697379h
loc_4_04DC:
        popad
        insb
        insb
        db  079h, 020h
loc_4_04E1:
        insd
        popad
        jae    SHORT loc_4_0550
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax + 68h], dl
        jns    SHORT loc_4_0565
        imul   esp, dword ptr [ebx + 61h], 20796c6ch
loc_4_04F9:
        jb     SHORT loc_4_0560
        jno    SHORT loc_4_0572
        db  065h, 073h, 074h
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], al
        push   esi
        imul   esi, dword ptr [edx + 74h], 696c6175h
        jp     SHORT loc_4_0575
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edi + 6ch], al
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        insb
        db  079h, 020h
        outsd
        db  077h, 06Eh
        cmp    ah, byte ptr fs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_4_0536:
        push   ebx
        push   64657261h
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0544:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0548:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        dec    ebp
        popad
loc_4_0550:
        db  073h, 06Bh
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [edx + 6fh], ah
        outsd
        je     SHORT loc_4_057D
        db  074h, 069h
        insd
loc_4_0560:
        cmp    ah, byte ptr gs:[eax]
        and    byte ptr [eax], ah
loc_4_0565:
        add    byte ptr [ecx + 75h], al
        db  074h, 06Fh
        and    byte ptr [ebp + 61h], ch
        db  073h, 06Bh
        and    byte ptr [ecx + 74h], ah
loc_4_0572:
        and    byte ptr [ecx + 6eh], ch
loc_4_0575:
        db  073h, 074h
        and    byte ptr [ebx + 77h], dh
        popad
        db  070h, 03Ah
loc_4_057D:
        add    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
loc_4_0591:
        outsb
        db  074h, 069h
        outsb
        db  075h, 065h
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebx + 45h], bl
        db  073h, 063h
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  071h, 075h
        imul   esi, dword ptr [edx + edi + 20h], 72695600h
        db  074h, 075h
        popad
        insb
        and    byte ptr [ecx + 6eh], ch
        db  074h, 065h
        db  072h
_ITEXT ends
        end
