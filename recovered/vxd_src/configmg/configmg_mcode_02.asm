; configmg_mcode_02.asm -- CONFIGMG object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 054h, 000h
        db  060h, 000h, 069h, 000h, 072h, 000h, 089h, 000h, 0DAh, 001h, 006h, 002h, 0A7h, 002h, 0CAh, 002h
        db  071h, 003h, 09Ch, 003h, 030h, 004h, 054h, 004h, 05Eh, 004h, 011h, 005h, 026h, 005h, 0DFh, 005h
        db  002h, 006h, 077h, 006h, 09Ah, 007h, 0BDh, 009h, 08Eh, 00Ah, 0CCh, 0CCh
loc_2_003C:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        imul   esi, dword ptr [ebx + 20h], 62616e75h
        insb
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        je     NEAR PTR loc_2_00BA
        jb     SHORT loc_2_00C4
        imul   ebp, dword ptr [esi + 65h], 756f7920h
        db  072h, 020h
        push   77647261h
        popad
        db  072h, 065h
        and    byte ptr [eax + 72h], dh
        outsd
        imul   bp, word ptr [ebp + 2eh], 2020h
        dec    ecx
        jae    SHORT loc_2_0096
        and    ah, byte ptr [74202273h]
        push   75632065h
        db  072h, 072h
        outsb
        db  074h, 020h
        db  070h, 072h
        outsd
        imul   bp, word ptr [ebp + 3fh], 4400h
        outsd
        arpl   word ptr [ebx + 20h], bp
        inc    ebx
loc_2_0096:
        push   65676e61h
        add    byte ptr [ebp + 6eh], dl
        outsd
        arpl   word ptr [ebx + 65h], bp
        add    byte ptr fs:[edi + ebp*2 + 63h], al
        imul   esp, dword ptr [eax], 25h
        insb
        add    byte ptr fs:[edi + 72h], cl
        imul   esp, dword ptr [edi + 69h], 206c616eh
        inc    ebx
        outsd
        outsb
loc_2_00BA:
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_012B
        outsd
        outsb
loc_2_00C4:
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [edi + ebp*2 + 65h], ah
        jae    SHORT loc_2_00F2
        outsb
        outsd
        db  074h, 020h
        push   20657661h
        popad
        and    byte ptr [eax + 61h], ch
        db  072h, 064h
        ja     SHORT loc_2_0144
        db  072h, 065h
        and    byte ptr [eax + 72h], dh
        outsd
        imul   bp, word ptr [ebp + 20h], 7263h
        popad
loc_2_00F2:
        db  074h, 065h
        and    byte ptr fs:[esi + 6fh], ah
        db  072h, 020h
        db  079h, 06Fh
        jne    SHORT loc_2_0170
        and    byte ptr [ebp + 6eh], dh
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_017C
        outsd
        outsb
        and    byte ptr cs:[ecx + 66h], cl
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_013E
        jne    SHORT loc_2_018E
        outsd
        arpl   word ptr [ebx + 20h], bp
        bound  esp, qword ptr [ebp + 66h]
        outsd
        jb     SHORT loc_2_0190
loc_2_012B:
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebp + 74h], ah
        arpl   word ptr gs:[ebx + esi*2 + 20h], si
        db  074h, 068h
loc_2_013E:
        and    byte ptr gs:[eax + 61h], ch
        jb     SHORT loc_2_01A8
loc_2_0144:
        ja     SHORT loc_2_01A7
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 6eh], dh
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ebx + 74h], dh
        popad
        je     SHORT loc_2_01C3
        sub    al, 20h
        db  079h, 06Fh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_2_018E
        insd
        popad
loc_2_0170:
        jns    SHORT loc_2_0192
        db  073h, 074h
        outsd
        jo     SHORT loc_2_0197
        db  072h, 065h
        db  073h, 070h
        outsd
loc_2_017C:
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 6f54202eh
        and    byte ptr [eax + 61h], ch
        db  076h, 065h
        and    byte ptr [edi + 69h], dl
        outsb
loc_2_018E:
        outsd
loc_2_0190:
        ja     SHORT loc_2_0205
loc_2_0192:
        and    byte ptr [ebx + 72h], ah
        popad
loc_2_0197:
        je     SHORT loc_2_01FE
        and    byte ptr [ecx + 20h], ah
        push   77647261h
        popad
        db  072h, 065h
        and    byte ptr [eax + 72h], dh
loc_2_01A7:
        outsd
loc_2_01A8:
        imul   bp, word ptr [ebp + 2ch], 7320h
        push   64207475h
        outsd
        db  077h, 06Eh
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edx + 65h], dh
        db  073h, 074h
        popad
        jb     SHORT loc_2_0237
loc_2_01C3:
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_2_01F2
        imul   ebp, dword ptr [esi + 20h], 20656874h
        jne    SHORT loc_2_0249
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ebx + 74h], dh
        popad
        je     SHORT loc_2_024C
        and    byte ptr cs:[edi + ebp*2 + 20h], al
        db  079h, 06Fh
        jne    SHORT loc_2_0210
        db  077h, 061h
loc_2_01F2:
        outsb
        db  074h, 020h
        je     SHORT loc_2_0266
        and    byte ptr [ebx + 6fh], ah
        outsb
        je     SHORT loc_2_0266
        outsb
loc_2_01FE:
        db  075h, 065h
        and    byte ptr [edi + 69h], dh
        db  074h, 068h
loc_2_0205:
        outsd
        db  075h, 074h
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_2_0281
        popad
        db  072h, 074h
loc_2_0210:
        imul   ebp, dword ptr [esi + 67h], 5700203fh
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        dec    eax
        popad
        db  073h, 020h
        dec    esi
        outsd
        db  074h, 020h
        push   ebx
        outsb
        and    byte ptr [ebp + 6eh], dl
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ebx + 6fh], al
        outsb
loc_2_0237:
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_02A8
        outsd
        outsb
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_02BC
loc_2_0249:
        and    byte ptr [ebx + 61h], ah
loc_2_024C:
        outsb
        outsb
        outsd
        db  074h, 020h
        db  064h, 065h, 074h, 065h
        arpl   word ptr [eax + 61h], si
        outsb
        and    byte ptr fs:[ecx + 6eh], ch
        jae    SHORT loc_2_02D4
        popad
        insb
        insb
        and    byte ptr [ecx + 6fh], bh
loc_2_0266:
        jne    SHORT loc_2_02DA
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 68772073h
        imul   ebp, dword ptr [ebp + 20h], 69207469h
        db  073h, 020h
        db  072h, 075h
        outsb
        outsb
loc_2_0281:
        imul   ebp, dword ptr [esi + 67h], 206e6920h
        jae    SHORT loc_2_02EB
        and    byte ptr gs:[ebp + 6fh], ch
        and    byte ptr cs:[edi + ebp*2 + 20h], dl
        db  064h, 065h, 074h, 065h
        arpl   word ptr [eax + 61h], si
        outsb
        and    byte ptr fs:[ecx + 6eh], ch
        db  073h, 074h
        popad
        insb
        insb
loc_2_02A8:
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_031F
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 72202c73h
        jae    NEAR PTR loc_2_032F
        popad
loc_2_02BC:
        jb     SHORT loc_2_0332
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_0339
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edi + ebp*2 + 6eh], ah
        daa
        db  074h, 020h
        arpl   word ptr [eax + 6fh], bp
loc_2_02D4:
        outsd
        db  073h, 065h
        and    byte ptr [ebx + 61h], dh
loc_2_02DA:
        and    byte ptr gs:[ebp + 6fh], ch
        add    byte ptr cs:[ebx + 61h], al
        outsb
        outsb
        outsd
        db  074h, 020h
        inc    esp
loc_2_02EB:
        db  065h, 074h, 065h
        arpl   word ptr [eax + 44h], si
        jbe    NEAR PTR loc_2_035E
        arpl   word ptr [ebp + 73h], sp
        and    byte ptr [ecx + 6eh], cl
        and    byte ptr [ebx + 61h], dl
        and    byte ptr gs:[ebp + 6fh], cl
        add    byte ptr gs:[edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_0380
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 66h], 206d726fh
loc_2_031F:
        popad
        insb
        insb
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [eax + 72h], dh
        outsd
        db  067h, 072h, 061h
        insd
loc_2_032F:
        db  073h, 020h
        popad
loc_2_0332:
        bound  ebp, qword ptr [edi + 75h]
        db  074h, 020h
        db  074h, 068h
loc_2_0339:
        imul   esi, dword ptr [ebx + 20h], 666e6f63h
        imul   esp, dword ptr [edi + 75h], 69746172h
        outsd
        outsb
        and    byte ptr [ebx + 68h], ah
        popad
        outsb
        and    byte ptr cs:[bp + si + 65h], al
        outsw
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
loc_2_035E:
        outsb
        db  075h, 069h
        outsb
        sub    al, 20h
        jae    SHORT loc_2_03C8
        db  076h, 065h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        db  075h, 06Eh
        db  073h, 061h
        jbe    SHORT loc_2_03D9
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
loc_2_0380:
        outsb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_03FB
        and    byte ptr [eax + 72h], dh
        outsd
        jb     NEAR PTR loc_2_03F1
        insd
        jae    SHORT loc_2_03C1
        and    byte ptr [edi + ebp*2 + 20h], al
        jns    SHORT loc_2_0408
        db  075h, 020h
        db  077h, 061h
        outsb
        db  074h, 020h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        jne    SHORT loc_2_0410
        aas
        add    byte ptr [ebx + 68h], al
        popad
        outsb
        imul   ebp, dword ptr [bp + 67h], 6e6f4320h
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
loc_2_03C1:
        outsd
        outsb
        and    byte ptr [ebp + 61h], cl
        db  079h, 020h
loc_2_03C8:
        inc    ecx
        arpl   word ptr gs:[eax + 50h], si
        db  072h, 06Fh
        jb     NEAR PTR loc_2_0436
        insd
        jae    SHORT loc_2_03D8
loc_2_03D8:
        dec    ecx
loc_2_03D9:
        je     SHORT loc_2_03FB
        imul   esi, dword ptr [ebx + 20h], 6f636572h
        insd
        insd
        outsb
        and    byte ptr fs:[eax + ebp*2 + 61h], dh
        je     SHORT loc_2_040F
        db  079h, 06Fh
loc_2_03F1:
        db  075h, 020h
        arpl   word ptr [edi + ebp*2 + 73h], bp
        and    byte ptr gs:[eax + 72h], dh
loc_2_03FB:
        outsd
        db  067h, 072h, 061h
        insd
        db  073h, 020h
        bound  esp, qword ptr [ebp + 66h]
        outsd
        db  072h, 065h
loc_2_0408:
        and    byte ptr [edx + 65h], dh
        insd
        outsd
        db  076h, 069h
loc_2_040F:
        outsb
loc_2_0410:
        and    byte ptr [bx + 72h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ecx + ebp*2 + 6eh]
        and    byte ptr [si + 68h], dh
        imul   esi, dword ptr [ebx + 20h], 69766564h
        arpl   word ptr [ebp + 20h], sp
        sub    byte ptr [202e2973h], ah
        inc    esp
        outsd
        and    byte ptr [ecx + 6fh], bh
loc_2_0436:
        jne    SHORT loc_2_0458
        db  077h, 061h
        outsb
        je     SHORT loc_2_045D
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        db  075h, 065h
        and    byte ptr [edx + 65h], dh
        insd
        outsd
        db  076h, 069h
        outsb
        and    byte ptr [bx + 72h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
loc_2_0458:
        popad
        bound  ebp, qword ptr [ecx + ebp*2 + 6eh]
loc_2_045D:
        and    byte ptr [si + 68h], dh
        imul   esi, dword ptr [ebx + 20h], 69766564h
        arpl   word ptr [ebp + 3fh], sp
        add    byte ptr [edx + 65h], dl
        insd
        outsd
        db  076h, 069h
        outsb
        and    byte ptr [si + 65h], al
        jbe    SHORT loc_2_04E2
        arpl   word ptr [ebp + 20h], sp
        dec    ebp
        popad
        db  079h, 020h
        inc    ecx
        arpl   word ptr gs:[eax + 50h], si
        db  072h, 06Fh
        db  067h, 072h, 061h
        insd
        jae    SHORT loc_2_0490
loc_2_0490:
        daa
        db  075h, 06Eh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        daa
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_0514
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_04CC
        arpl   word ptr [eax + 61h], bp
        outsb
        and    byte ptr gs:[si + 68h], dh
        and    byte ptr gs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        db  073h, 065h
loc_2_04CC:
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 6fh], ah
        jne    SHORT loc_2_0545
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        jb     SHORT loc_2_0546
        insd
loc_2_04E2:
        outsd
        jbe    SHORT loc_2_054A
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 20h]
        popad
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 6f54202eh
        and    byte ptr [ebp + 6eh], dh
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  079h, 06Fh
        db  075h, 072h
        and    byte ptr [ecx + 70h], ch
        db  074h, 06Fh
        db  070h, 020h
        arpl   word ptr [edi + 6dh], bp
loc_2_0514:
        jo     SHORT loc_2_058B
        je     SHORT loc_2_057D
        jb     SHORT loc_2_0546
        and    byte ptr [ebx + 68h], dh
        db  075h, 074h
        and    byte ptr [edi + ebp*2 + 77h], ah
        outsb
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        sub    al, 20h
        popad
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [ebp + 6eh], dh
        outsd
        arpl   word ptr [ebx + 20h], bp
        jns    SHORT loc_2_05AF
        jne    SHORT loc_2_05B4
        and    byte ptr [ebx + 6fh], ah
loc_2_0545:
        insd
loc_2_0546:
        db  070h, 075h
        je     SHORT loc_2_05AF
loc_2_054A:
        db  072h, 02Eh
        add    byte ptr [edi + ebp*2 + 63h], al
        imul   esp, dword ptr [eax], 2fh
        and    byte ptr [ebp + 6eh], dl
        outsd
        arpl   word ptr [ebx + 20h], bp
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 6e695700h
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0594
        arpl   word ptr [eax + 61h], bp
        outsb
        and    byte ptr gs:[si + 68h], dh
loc_2_057D:
        and    byte ptr gs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
loc_2_058B:
        outsb
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        db  073h, 065h
loc_2_0594:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        popad
        bound  ebp, qword ptr [ebp + 20h]
        outsd
        db  072h, 020h
        outsd
        arpl   word ptr [ebx + 69h], bp
        outsb
        and    byte ptr [bp + di + 74h], dh
loc_2_05AF:
        popad
        db  074h, 069h
        outsd
        outsb
loc_2_05B4:
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 65687420h
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        db  065h, 073h, 074h
        and    byte ptr cs:[edi + ebp*2 + 20h], dl
        jne    SHORT loc_2_063C
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  079h, 06Fh
        db  075h, 072h
        and    byte ptr [ecx + 70h], ch
        je     SHORT loc_2_064C
        db  070h, 020h
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_2_0659
        je     SHORT loc_2_064B
        jb     SHORT loc_2_0614
        and    byte ptr [ebx + 68h], dh
        db  075h, 074h
        and    byte ptr [edi + ebp*2 + 77h], ah
        outsb
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_066D
        sub    al, 20h
        popad
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [ebp + 6eh], dh
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  079h, 06Fh
        jne    SHORT loc_2_0682
        and    byte ptr [ebx + 6fh], ah
        insd
loc_2_0614:
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_2_0648
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [eax + 61h], cl
        db  073h, 020h
        dec    esi
        outsd
        je     SHORT loc_2_064B
        inc    esi
        imul   ebp, dword ptr [esi + 69h], 64656873h
        and    byte ptr [ebp + 74h], al
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
loc_2_063C:
        outsb
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [eax + 61h], ch
loc_2_0648:
        jae    SHORT loc_2_066A
        outsb
loc_2_064B:
        outsd
loc_2_064C:
        db  074h, 020h
        jae    SHORT loc_2_06C5
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_2_06C8
        db  066h, 075h, 06Ch
        insb
loc_2_0659:
        db  079h, 020h
        jb     SHORT loc_2_06C2
        db  072h, 075h
        outsb
        and    byte ptr [ebp + 74h], ah
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
loc_2_066A:
        and    byte ptr [ecx + 6eh], ch
loc_2_066D:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], ah
        insb
        insb
        outsd
        db  077h, 069h
        outsb
        and    byte ptr [bx + si + 61h], ch
        db  072h, 064h
        ja     SHORT loc_2_06E3
loc_2_0682:
        db  072h, 065h
        and    byte ptr [eax + 72h], dh
        outsd
        imul   bp, word ptr [ebp + 73h], 7320h
        imul   ebp, dword ptr [esi + 63h], 68742065h
        and    byte ptr gs:[ecx + 73h], ch
        je     SHORT loc_2_06BD
        db  074h, 069h
        insd
        and    byte ptr gs:[ecx + 6fh], bh
        jne    SHORT loc_2_06C6
        db  072h, 061h
        outsb
        and    byte ptr [ebx + 65h], dl
        je     SHORT loc_2_0723
        jo     SHORT loc_2_06EA
        or     cl, byte ptr [edx]
        add    byte ptr [edx], cl
        dec    ecx
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_06DB
        db  075h, 06Eh
loc_2_06BD:
        outsd
        arpl   word ptr [ebx + 20h], bp
loc_2_06C2:
        bound  esp, qword ptr [ebp + 66h]
loc_2_06C5:
        outsd
loc_2_06C6:
        db  072h, 065h
loc_2_06C8:
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [edx + 65h], dh
        je     NEAR PTR loc_2_073C
        arpl   word ptr [ebx + esi*2 + 20h], si
loc_2_06DB:
        db  074h, 068h
        and    byte ptr gs:[eax + 61h], ch
        db  072h, 064h
loc_2_06E3:
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
loc_2_06EA:
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [eax + 61h], ch
        jb     SHORT loc_2_075F
        db  077h, 061h
        jb     SHORT loc_2_0764
        and    byte ptr [eax + 72h], dh
        outsd
        imul   bp, word ptr [ebp + 73h], 202ch
        jns    SHORT loc_2_077B
        jne    SHORT loc_2_0780
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0789
        je     SHORT loc_2_077B
        jb     SHORT loc_2_0738
        insd
        popad
        jns    SHORT loc_2_073C
        db  073h, 074h
        outsd
        db  070h, 020h
        jb     SHORT loc_2_0788
loc_2_0723:
        db  073h, 070h
        outsd
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 6f54202eh
        and    byte ptr [eax + 61h], ch
        jbe    SHORT loc_2_0799
        and    byte ptr [edi + 69h], dl
        outsb
loc_2_0738:
        outsd
        db  077h, 073h
loc_2_073C:
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [ebp + 74h], ah
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 20h], ah
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebx + 20h], 64726168h
        ja     SHORT loc_2_07C0
loc_2_075F:
        jb     SHORT loc_2_07C6
        and    byte ptr [eax + 72h], dh
loc_2_0764:
        outsd
        imul   bp, word ptr [ebp + 2ch], 7320h
        push   64207475h
        outsd
        db  077h, 06Eh
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edx + 65h], dh
loc_2_077B:
        db  073h, 074h
        popad
        jb     SHORT loc_2_07F4
loc_2_0780:
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
loc_2_0788:
        insd
loc_2_0789:
        db  070h, 075h
        db  074h, 065h
        db  072h, 020h
        imul   ebp, dword ptr [esi + 20h], 74616874h
        and    byte ptr [eax + 61h], ch
loc_2_0799:
        db  072h, 064h
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [eax + 72h], dh
        outsd
        imul   bp, word ptr [ebp + 2eh], 4420h
        outsd
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_07D0
        db  077h, 061h
        outsb
        db  074h, 020h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        db  075h, 065h
loc_2_07C0:
        and    byte ptr [edi + 69h], dh
        je     SHORT loc_2_082D
        outsd
loc_2_07C6:
        db  075h, 074h
        and    byte ptr [edx + 65h], dh
        db  073h, 074h
        popad
        db  072h, 074h
loc_2_07D0:
        imul   ebp, dword ptr [esi + 67h], 5900203fh
        outsd
        db  075h, 072h
        and    byte ptr [ebp + 75h], ch
        insb
        je     SHORT loc_2_0849
        sub    eax, 636e7566h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 73252820h
loc_2_07F4:
        sub    dword ptr [eax], esp
        push   73207361h
        outsd
        insd
        and    byte ptr gs:[ebx + 68h], ah
        imul   ebp, dword ptr [esp + 20h], 69766564h
        arpl   word ptr [ebp + 73h], sp
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 2d323320h
        bound  ebp, qword ptr [ecx + 74h]
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 073h
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edi + 74h], ch
        push   20737265h
loc_2_082D:
        db  075h, 073h
        imul   ebp, dword ptr [esi + 67h], 6d6f6320h
        db  070h, 061h
        je     SHORT loc_2_08A3
        bound  ebp, qword ptr [ecx + 6ch]
        imul   esi, dword ptr [ecx + edi*2 + 2dh], 65646f6dh
        and    byte ptr [edx + esi*2 + 69h], ah
loc_2_0849:
        db  076h, 065h
        jb     SHORT loc_2_08C0
        and    byte ptr cs:[eax + ebp*2 + 69h], dl
        jae    SHORT loc_2_0874
        arpl   word ptr [edi + 6eh], bp
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_08C8
        outsd
        outsb
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        db  073h, 075h
        db  070h, 070h
        outsd
        jb     SHORT loc_2_08E4
        sub    al, 20h
loc_2_0874:
        db  073h, 06Fh
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        je     SHORT loc_2_08E8
        jb     SHORT loc_2_08A5
        push   62207361h
        outsb
        and    byte ptr [eax + 61h], ch
        insb
        db  074h, 065h
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  070h, 072h
        jbe    NEAR PTR loc_2_0902
        outsb
        je     SHORT loc_2_08C0
        arpl   word ptr [edi + 72h], bp
loc_2_08A3:
        db  072h, 075h
loc_2_08A5:
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 0d0a0d2eh
        or     al, byte ptr [ecx + 66h]
        db  074h, 065h
        jb     SHORT loc_2_08D5
        db  079h, 06Fh
        db  075h, 020h
        jb     SHORT loc_2_0920
        jae    SHORT loc_2_0931
        popad
        db  072h, 074h
loc_2_08C0:
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
loc_2_08C8:
        insd
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_2_08FB
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
loc_2_08D5:
        db  077h, 073h
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[ebx + 6fh], ah
        insd
loc_2_08E4:
        db  070h, 061h
        db  074h, 069h
loc_2_08E8:
        bound  ebp, qword ptr [ecx + 6ch]
        imul   esi, dword ptr [ecx + edi*2 + 2dh], 65646f6dh
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 073h
loc_2_08FB:
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_2_0920
        popad
loc_2_0902:
        arpl   word ptr [eax + 20h], bp
        arpl   word ptr [eax + 69h], bp
        insb
        and    byte ptr fs:[ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 74746120h
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  074h, 068h
loc_2_0920:
        imul   esi, dword ptr [ebx + 20h], 746c756dh
        imul   ebp, dword ptr [636e7566h], 6e6f6974h
loc_2_0931:
        and    byte ptr [ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 6649202eh
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_0961
        db  077h, 061h
        outsb
        db  074h, 020h
        db  074h, 06Fh
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[ebx], dh
        xor    ch, byte ptr [20746962h]
        jb     NEAR PTR loc_2_09C0
        db  076h, 065h
        jb     SHORT loc_2_09CE
        sub    al, 20h
        jns    SHORT loc_2_09CE
        db  075h, 020h
loc_2_0961:
        insd
        popad
        jns    SHORT loc_2_0985
        bound  esp, qword ptr [ebp + 20h]
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edi + 62h], ch
        db  074h, 061h
        imul   ebp, dword ptr [esi + 20h], 75206e61h
        jo     SHORT loc_2_09E1
        popad
        db  074h, 065h
        and    byte ptr fs:[edx + esi*2 + 69h], ah
loc_2_0985:
        db  076h, 065h
        db  072h, 020h
        outsw
        db  072h, 020h
        db  074h, 068h
        and    byte ptr gs:[ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 61687420h
        db  074h, 020h
        arpl   word ptr [ecx + 75h], sp
        db  073h, 065h
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ebx + 6fh], ah
        outsb
        je     SHORT loc_2_0A19
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        and    byte ptr [bx + di + 6fh], bh
loc_2_09C0:
        db  075h, 072h
        and    byte ptr [eax + 61h], ch
        db  072h, 064h
        db  077h, 061h
        jb     SHORT loc_2_0A30
        and    byte ptr [ebp + 61h], ch
loc_2_09CE:
        outsb
        db  075h, 066h
        popad
        arpl   word ptr [ebp + esi*2 + 72h], si
        jb     NEAR PTR loc_2_0A05
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
loc_2_09E1:
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_0A57
        and    byte ptr gs:[ebp + 76h], ah
        imul   esp, dword ptr [ebx + 65h], 6f43002eh
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        je     SHORT loc_2_0A6D
        outsd
loc_2_0A05:
        outsb
        and    byte ptr [ebp + 61h], cl
        outsb
        popad
        db  067h, 065h, 072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        je     SHORT loc_2_0A36
        insb
        outsd
        popad
loc_2_0A19:
        and    byte ptr fs:[edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        jae    SHORT loc_2_0A87
        and    byte ptr [edi + 6eh], ch
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], ah
loc_2_0A30:
        insb
        insb
        outsd
        db  077h, 069h
        outsb
loc_2_0A36:
        and    byte ptr [di], cl
        or     ah, byte ptr [esi + 69h]
        insb
        db  065h, 073h, 020h
        imul   esi, dword ptr [ebx + 20h], 68746965h
        db  065h, 072h, 020h
        outsb
        outsd
        je     SHORT loc_2_0A6E
        jo     SHORT loc_2_0AC2
        jae    NEAR PTR loc_2_0AB8
        outsb
        je     SHORT loc_2_0A76
        outsd
loc_2_0A57:
        db  072h, 020h
        push   61207361h
        outsb
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_2_0AC5
        insb
        imul   esp, dword ptr [eax + 76h], 69737265h
loc_2_0A6D:
        outsd
loc_2_0A6E:
        outsb
        and    byte ptr [esi + 75h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
loc_2_0A76:
        cmp    ah, byte ptr [eax]
        or     eax, 4d4d560ah
        push   esi
        pop    eax
        inc    esp
        sub    al, 20h
        push   ebx
        dec    eax
        inc    ebp
        dec    esp
loc_2_0A87:
        dec    esp
        push   esi
        pop    eax
        inc    esp
        sub    al, 20h
        push   esi
        push   esp
        inc    esp
        push   esi
        pop    eax
        inc    esp
        sub    al, 20h
        push   esi
        pop    eax
        inc    esp
        dec    esp
        inc    esp
        push   edx
        push   esi
        pop    eax
        inc    esp
        sub    al, 20h
        push   esi
        push   eax
        dec    ecx
        inc    ebx
        inc    esp
        push   esi
        pop    eax
        inc    esp
        or     eax, 540a0d0ah
        db  072h, 079h
        and    byte ptr [edx + 75h], dh
        outsb
loc_2_0AB8:
        outsb
        imul   ebp, dword ptr [esi + 67h], 74655320h
        db  075h, 070h
loc_2_0AC2:
        and    byte ptr [ecx + 67h], ah
loc_2_0AC5:
        popad
        db  069h, 06Eh, 02Eh, 000h
_LMSGTABLE ends
        end
