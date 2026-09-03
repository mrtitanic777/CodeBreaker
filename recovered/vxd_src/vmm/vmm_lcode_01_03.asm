; vmm_lcode_01_03.asm -- VMM object 1 segment _LTEXT  (part 3/4)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_947B:
        push   eax
        push   ecx
        push   edx
        add    dword ptr [edi - 8], eax
        call   sub_1_8FD1
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_948A:
        push   eax
        push   ecx
        push   edi
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        mov    edi, dword ptr [ebx - 1ch]
loc_1_9496:
        db  0CDh, 020h, 068h, 001h, 001h, 000h
        mov    edi, dword ptr [edi + 40h]
        or     edi, edi
        jne    SHORT loc_1_9496
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        jne    SHORT loc_1_94B3
        pushfd
        cli
        call   sub_1_8ACC
        popfd
loc_1_94B3:
        pop    edi
        pop    ecx
        pop    eax
        ret
loc_1_94B7:
        push   eax
        push   ecx
        push   edi
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        mov    ecx, edi
loc_1_94C2:
        db  0CDh, 020h, 068h, 001h, 001h, 000h
        mov    edi, dword ptr [edi - 4]
        cmp    ecx, edi
        jne    SHORT loc_1_94C2
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        jne    SHORT loc_1_94DF
        pushfd
        cli
        call   sub_1_8ACC
        popfd
loc_1_94DF:
        pop    edi
        pop    ecx
        pop    eax
        ret
loc_1_94E3:
        push   eax
        push   ecx
        mov    ecx, edi
loc_1_94E7:
        mov    eax, dword ptr [ecx - 4]
        cmp    edi, eax
        je     SHORT loc_1_94F2
        mov    ecx, eax
        jmp    SHORT loc_1_94E7
loc_1_94F2:
        mov    eax, dword ptr [edi - 4]
        mov    dword ptr [ecx - 4], eax
        mov    dword ptr [edi - 4], edi
        pop    ecx
        pop    eax
        ret
loc_1_94FE:
        push   ecx
        db  0CDh, 020h, 06Ch, 001h, 001h, 000h
        mov    ecx, edi
        xchg   dword ptr [eax - 4], ecx
        mov    dword ptr [edi - 4], ecx
        pop    ecx
        ret
loc_1_950F:
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_9531
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_9525
        test   byte ptr [ebx + 1], 2
        je     SHORT loc_1_9531
loc_1_9525:
        push   eax
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        call   sub_1_947B
        pop    eax
loc_1_9531:
        ret
Get_Time_Slice_Priority:
        mov    esi, 64h
        mov    ecx, esi
        xor    edx, edx
        mov    dl, 32h
        mov    eax, dword ptr [ebx]
        test   eax, 40000h
        je     SHORT loc_1_954A
        mov    cx, 2710h
loc_1_954A:
        and    eax, 203h
        ret
        db  08Bh, 047h, 014h, 0BEh, 064h, 000h, 000h, 000h, 033h, 0D2h, 0B2h, 032h, 08Bh, 0CAh, 08Bh, 000h
        db  0A9h, 000h, 000h, 004h, 000h, 074h, 002h, 0B1h, 064h, 025h, 003h, 002h, 000h, 000h, 0C3h, 0E3h
        db  036h, 083h, 0F9h, 064h, 077h, 031h, 085h, 0D2h, 074h, 02Dh, 081h, 0FAh, 010h, 027h, 000h, 000h
        db  077h, 025h, 0A9h, 0FCh, 0FDh, 0FFh, 0FFh, 075h, 01Eh, 083h, 0E0h, 0FEh, 060h, 083h, 0F9h, 064h
        db  075h, 005h, 00Dh, 000h, 000h, 004h, 000h, 081h, 023h, 0FCh, 0FDh, 0FBh, 0FFh, 009h, 003h, 0E8h
        db  012h, 0F9h, 0FFh, 0FFh, 061h, 0F8h, 0C3h, 0F9h, 0C3h
Set_Time_Slice_Priority:
        and    eax, 202h
        jecxz  loc_1_962F
        or     edx, edx
        je     SHORT loc_1_962F
        cmp    edx, 2710h
        ja     SHORT loc_1_962F
        cmp    ecx, 2710h
        ja     SHORT loc_1_962F
        pushad
        cmp    ecx, 2710h
        jne    SHORT loc_1_95D2
        or     eax, 40000h
loc_1_95D2:
        mov    ecx, dword ptr [ebx]
        mov    edx, eax
        and    ecx, 200h
        and    edx, 200h
        xor    ecx, edx
        je     SHORT loc_1_961F
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_961F
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_961F
        push   eax
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        test   eax, 200h
        je     SHORT loc_1_9611
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 06Dh, 001h, 001h, 000h
        jmp    SHORT loc_1_961E
loc_1_9611:
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        neg    eax
        db  0CDh, 020h, 06Dh, 001h, 001h, 000h
loc_1_961E:
        pop    eax
loc_1_961F:
        and    dword ptr [ebx], 0fffbfdfch
        or     dword ptr [ebx], eax
        call   sub_1_8EB6
        popad
        clc
        ret
loc_1_962F:
        stc
        ret
Get_Execution_Focus:
        db  08Bh, 01Dh, 044h, 0F7h, 000h, 000h
        ret
sub_1_9638:
        push   esi
        push   edi
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     NEAR PTR loc_1_96F9
        call   sub_1_90A9
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0FFh, 005h, 078h, 0F7h, 000h, 000h
        db  089h, 01Dh, 048h, 0F7h, 000h, 000h
        push   ebx
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
        mov    esi, ebx
loc_1_966B:
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_9694
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_9687
        test   byte ptr [ebx + 1], 2
        je     SHORT loc_1_9694
loc_1_9687:
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        neg    eax
        db  0CDh, 020h, 06Dh, 001h, 001h, 000h
loc_1_9694:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, esi
        jne    SHORT loc_1_966B
        db  08Fh, 005h, 044h, 0F7h, 000h, 000h
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
        mov    esi, ebx
loc_1_96AC:
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_96D3
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  039h, 01Dh, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_96C8
        test   byte ptr [ebx + 1], 2
        je     SHORT loc_1_96D3
loc_1_96C8:
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 06Dh, 001h, 001h, 000h
loc_1_96D3:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, esi
        jne    SHORT loc_1_96AC
        call   sub_1_8EB6
        db  0FFh, 00Dh, 078h, 0F7h, 000h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        call   sub_1_90BE
loc_1_96F9:
        pop    edi
        pop    esi
        ret
Set_Execution_Focus:
        push   edi
        db  08Bh, 03Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 03Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_971C
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_971C
        stc
        pop    edi
        ret
loc_1_971C:
        pushad
        call   sub_1_9638
        popad
        clc
        pop    edi
        ret
Get_Time_Slice_Granularity:
        db  0A1h, 050h, 0F7h, 000h, 000h
        ret
Set_Time_Slice_Granularity:
        test   eax, eax
        je     SHORT loc_1_973C
        cmp    eax, 0fah
        jg     SHORT loc_1_973C
        db  0A3h, 050h, 0F7h, 000h, 000h
loc_1_973C:
        ret
Get_Time_Slice_Info:
        xor    ecx, ecx
        mov    eax, ecx
        inc    eax
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        ret
Call_When_Idle:
        pushad
        mov    edi, esi
        db  08Bh, 035h, 05Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_9763
        mov    dword ptr [eax], edi
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        clc
loc_1_9763:
        popad
        ret
loc_1_9765:
        pushad
        db  08Bh, 03Dh, 05Ch, 0F7h, 000h, 000h
        call   sub_1_871A
        popad
        ret
sub_1_9773:
        push   ebx
        push   edi
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    ebp, dword ptr [edi + 10h]
        db  08Bh, 00Dh, 05Ch, 0F7h, 000h, 000h
        mov    ecx, dword ptr [ecx]
loc_1_978C:
        jecxz  loc_1_9799
        pushad
        call   dword ptr [ecx]
        popad
        jae    SHORT loc_1_9799
        mov    ecx, dword ptr [ecx - 4]
        jmp    SHORT loc_1_978C
loc_1_9799:
        pop    edi
        pop    ebx
        ret
loc_1_979C:
        mov    ebx, edx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_97B7
        mov    dword ptr [ebx - 28h], 0
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        or     byte ptr [ebx + 1], 40h
loc_1_97B7:
        ret
sub_1_97B8:
        sti
        push   esi
        push   edx
        push   ecx
        push   eax
        or     eax, eax
        jne    SHORT loc_1_97EF
        db  08Bh, 015h, 00Ch, 0F7h, 000h, 000h
        cmp    dword ptr [edx - 64h], eax
        jne    SHORT loc_1_97EE
        push   edi
        push   edx
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        call   sub_1_9065
        or     dword ptr [edi], 1000000h
        cli
        call   sub_1_8ACC
        pop    edx
        or     byte ptr [edx + 1], 40h
        sti
        pop    edi
        jmp    SHORT loc_1_9858
loc_1_97EE:
        inc    eax
loc_1_97EF:
        sub    eax, 7fffffffh
        cdq
        and    eax, edx
        add    eax, 7fffffffh
        db  08Bh, 015h, 00Ch, 0F7h, 000h, 000h
        cli
        or     byte ptr [edx + 1], 80h
        and    byte ptr [edx + 1], 0bfh
        mov    ecx, eax
        db  003h, 00Dh, 020h, 0DAh, 000h, 000h
        mov    esi, ecx
        cmp    dword ptr [edx - 28h], 0
        je     SHORT loc_1_982B
        sub    esi, dword ptr [edx - 24h]
        jns    SHORT loc_1_984D
        xor    esi, esi
        xchg   dword ptr [edx - 28h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_982B:
        sti
        db  0BEh, 09Ch, 097h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        cli
        test   byte ptr [edx + 1], 80h
        jne    SHORT loc_1_9847
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        sti
        jmp    SHORT loc_1_9858
loc_1_9847:
        mov    dword ptr [edx - 28h], esi
        mov    dword ptr [edx - 24h], ecx
loc_1_984D:
        push   15h
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_9858:
        test   byte ptr [edx + 1], 40h
        sti
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        ret
loc_1_9862:
        pushad
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [ebx - 28h], esi
        test   esi, esi
        je     SHORT loc_1_9878
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_9878:
        btr    dword ptr [ebx], 0fh
        jae    SHORT loc_1_9886
        push   ebx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_1_9886:
        db  00Fh, 0BAh, 035h, 06Ch, 0F7h, 000h, 000h, 000h
        jae    SHORT loc_1_98B5
        cli
        xor    esi, esi
        db  087h, 035h, 068h, 0F7h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_98A3
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_98A3:
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        db  0A1h, 060h, 0F7h, 000h, 000h
        sti
        db  0CDh, 020h, 067h, 001h, 001h, 000h
loc_1_98B5:
        popad
        ret
loc_1_98B7:
        pushad
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  03Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        je     SHORT loc_1_98CE
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        jmp    SHORT loc_1_992B
loc_1_98CE:
        db  00Fh, 0BAh, 02Dh, 06Ch, 0F7h, 000h, 000h, 000h
        jb     SHORT loc_1_992B
        mov    eax, dword ptr [edi - 24h]
        db  0A3h, 060h, 0F7h, 000h, 000h
        mov    eax, 4
        db  0CDh, 020h, 067h, 001h, 001h, 000h
loc_1_98EB:
        sti
        mov    eax, dword ptr [edi - 20h]
        cmp    eax, dword ptr [edi + 3ch]
        je     SHORT loc_1_98FC
        xor    eax, eax
        inc    eax
        call   sub_1_97B8
loc_1_98FC:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        call   sub_1_905F
        cli
        call   sub_1_8ACC
        sti
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, dword ptr [edi - 8]
        cmp    eax, 4
        jbe    SHORT loc_1_992D
        cmp    dword ptr [edi - 24h], 4
        jbe    SHORT loc_1_98EB
loc_1_992B:
        popad
        ret
loc_1_992D:
        db  083h, 03Dh, 008h, 0F7h, 000h, 000h, 000h
        jne    SHORT loc_1_9943
        call   sub_1_9773
        cmp    dword ptr [edi - 24h], 4
        ja     SHORT loc_1_992B
        jecxz  loc_1_9986
loc_1_9943:
        db  0A1h, 064h, 0F7h, 000h, 000h
        db  0BEh, 0ACh, 092h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 068h, 0F7h, 000h, 000h
        mov    eax, dword ptr [edi - 24h]
        neg    eax
        mov    ecx, eax
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        call   sub_1_905F
        xor    esi, esi
        db  087h, 035h, 068h, 0F7h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_1_98EB
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_98EB
loc_1_9986:
        db  08Bh, 00Dh, 044h, 0F7h, 000h, 000h
        jecxz  loc_1_9943
        cmp    ebx, ecx
        je     SHORT loc_1_9943
        test   byte ptr [ecx + 1], 80h
        je     SHORT loc_1_9943
        mov    ebx, ecx
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_98EB
loc_1_99A5:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        and    byte ptr [ebx + 1ch], 0bfh
        test   byte ptr [ebx + 1], 80h
        jne    SHORT Release_Time_Slice
        ret
        db  08Bh, 0C0h
Release_Time_Slice:
        sti
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  039h, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_99FD
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        db  0A1h, 088h, 0F7h, 000h, 000h
        jecxz  loc_1_99FF
        db  039h, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_99FF
        bts    word ptr [ebx + 1ch], 6
        jb     SHORT loc_1_9A05
        or     byte ptr [ebx + 1], 80h
        db  0BEh, 0A5h, 099h, 000h, 000h
        xor    eax, eax
        mov    ecx, 6ah
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_9A05
loc_1_99FD:
        xor    eax, eax
loc_1_99FF:
        db  0CDh, 020h, 061h, 001h, 001h, 000h
loc_1_9A05:
        popad
        ret
Wake_Up_VM:
        push   edi
        push   esi
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_9A31
        xor    esi, esi
        xchg   dword ptr [ebx - 28h], esi
        test   esi, esi
        je     SHORT loc_1_9A23
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_9A23:
        btr    dword ptr [ebx], 0fh
        jae    SHORT loc_1_9A31
        push   ebx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_1_9A31:
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        ret
        db  090h
Adjust_Execution_Time:
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        call   sub_1_9A48
        pop    edi
        ret
        db  08Bh, 0C0h
sub_1_9A48:
        push   eax
        push   ecx
        push   edx
        or     eax, eax
        js     SHORT loc_1_9A7F
        je     SHORT loc_1_9A97
        db  08Bh, 00Dh, 050h, 0F7h, 000h, 000h
        sub    eax, ecx
        cdq
        and    eax, edx
        add    eax, ecx
        add    eax, ecx
        mov    dword ptr [edi + 78h], eax
        xor    eax, eax
        mov    edx, dword ptr [edi + 14h]
        db  03Bh, 015h, 044h, 0F7h, 000h, 000h
        je     SHORT loc_1_9A97
        db  0A1h, 04Ch, 0F7h, 000h, 000h
        mov    ecx, eax
        db  0CDh, 020h, 062h, 001h, 001h, 000h
        jmp    SHORT loc_1_9A97
loc_1_9A7F:
        db  08Bh, 00Dh, 050h, 0F7h, 000h, 000h
        neg    ecx
        sub    eax, ecx
        cdq
        not    edx
        and    eax, edx
        add    eax, ecx
        neg    ecx
        add    eax, ecx
        mov    dword ptr [edi + 78h], eax
loc_1_9A97:
        mov    eax, dword ptr [edi + 14h]
        test   byte ptr [eax + 1], 80h
        je     SHORT loc_1_9AAA
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        pop    ebx
loc_1_9AAA:
        pop    edx
        pop    ecx
        pop    eax
        ret
No_Fail_Resume_VM:
        bts    dword ptr [ebx], 11h
        jae    SHORT loc_1_9ABA
        db  0CDh, 020h, 02Ch, 080h, 001h, 000h
loc_1_9ABA:
        pushad
        push   6
        push   dword ptr [ebx - 20h]
        call   sub_1_8F05
        popad
        ret
        db  000h
Out_Debug_String:
        ret
_Trace_Out_Service:
        ret    4
Queue_Debug_String:
        ret    8
        db  000h
sub_1_9AD0:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_9AD8:
        db  0CDh, 020h, 012h, 080h, 017h, 000h
        add    byte ptr [eax], al
loc_1_9AE0:
        db  0CDh, 020h, 00Fh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_9AE8:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_9AF0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_9AF8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
loc_1_9B08:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_9B11:
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B38:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_1_9B48:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B50:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_1_9B60:
        add    byte ptr [eax], al
loc_1_9B62:
        add    byte ptr [eax], al
loc_1_9B64:
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_1_9B68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B90:
        inc    dword ptr [edx]
loc_1_9B92:
        pushfd
        wait
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B98:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9B9C:
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 010h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 054h, 030h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 020h, 002h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        xor    byte ptr [edx], al
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        inc    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        push   eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        mov    gs, word ptr [eax]
        db  000h, 060h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 084h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 06Ch, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 08Ch, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0A0h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0A8h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0B0h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0B8h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 078h, 002h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 094h, 030h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        pushfd
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        movsb
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        lodsb
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        mov    ah, 30h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        mov    esp, 30h
        out    dx, al
        sub    byte ptr [eax], al
        les    esi, ptr [eax]
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        int3
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        aam    30h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        fdiv   qword ptr [eax]
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        in     al, 30h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        in     al, dx
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        hlt
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        cld
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        add    al, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        or     al, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        adc    al, 31h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 01Ch, 031h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        and    al, 31h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 02Ch, 031h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        xor    al, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        cmp    al, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        inc    esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        dec    esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        push   esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        pop    esp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        xor    dword ptr fs:[eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        insb
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        je     SHORT loc_1_9D2F
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        jl     SHORT loc_1_9D37
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        test   byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        db  08Ch, 031h, 000h, 000h, 000h, 0EEh, 028h, 000h, 004h, 00Bh, 000h, 000h, 000h, 0EEh, 028h, 000h
        db  09Ch, 031h, 000h, 000h, 000h, 0EEh, 028h, 000h, 0A4h, 031h, 000h
loc_1_9D2F:
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        lodsb
        xor    dword ptr [eax], eax
loc_1_9D37:
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        mov    ah, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        mov    esp, 31h
        out    dx, al
        sub    byte ptr [eax], al
        les    esi, ptr [ecx]
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        int3
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        aam    31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        fdiv   qword ptr [ecx]
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        in     al, 31h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        in     al, dx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        hlt
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        cld
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    dx, al
        sub    byte ptr [eax], al
        add    al, 32h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        or     al, 32h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        adc    al, 32h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 01Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 024h, 032h
        add    byte ptr [eax], al
        add    dh, ch
        sub    byte ptr [eax], al
        sub    al, 32h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 034h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 03Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 044h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 04Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 054h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 05Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 064h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 06Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 074h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 07Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 084h, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 08Ch, 032h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 000h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 010h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 020h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 030h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 040h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 050h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 060h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 070h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 080h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 090h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0A0h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0B0h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0C0h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0D0h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0E0h, 001h
        add    byte ptr [eax], al
        db  000h, 08Eh, 028h, 000h, 0F0h, 001h
        add    byte ptr [eax], al
        add    byte ptr [esi + 28h], cl
        add    byte ptr [eax], al
loc_1_9EA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9EA4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9EB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9F02:
        add    byte ptr [eax], al
loc_1_9F04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [ecx]
        add    byte ptr [eax], al
loc_1_BF08:
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 070h, 009h
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 01Bh
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 023h
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 02Bh
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 033h
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 03Bh
        adc    dword ptr [eax], eax
        db  000h, 043h, 011h
        add    byte ptr [eax], al
        dec    ebx
        adc    dword ptr [eax], eax
        db  000h, 053h, 011h
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 080h, 016h, 000h, 000h, 088h
        push   ss
        add    byte ptr [eax], al
        xchg   esp, eax
        push   ss
        add    byte ptr [eax], al
        pushfd
        push   ss
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 003h
        sbb    eax, dword ptr [eax]
        db  000h, 049h, 01Bh
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 010h
        db  015h, 000h, 000h, 02Ch, 015h
        add    byte ptr [eax], al
        sbb    al, 15h
        add    byte ptr [eax], al
        test   byte ptr [ebx], bl
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0D4h
        db  02Dh, 000h, 000h, 080h, 002h
        add    byte ptr [eax], al
        sbb    byte ptr [esi], ch
        add    byte ptr [eax], al
        and    al, 2
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0A0h, 02Dh, 000h, 000h, 038h
        add    al, byte ptr [eax]
        db  000h, 0E0h
        db  02Dh, 000h, 000h, 024h, 02Eh
        add    byte ptr [eax], al
        pop    ebx
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 063h, 011h
        add    byte ptr [eax], al
        imul   edx, dword ptr [ecx], 0
        db  000h, 020h
        or     al, byte ptr [eax]
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 004h, 01Ah
        add    byte ptr [eax], al
loc_1_C2F4:
        cli
        sbb    al, byte ptr [eax]
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0FBh
        adc    byte ptr [eax], al
        db  000h, 0A8h, 015h, 000h, 000h, 0C4h
        db  015h, 000h, 000h, 0B4h, 015h
        add    byte ptr [eax], al
loc_1_C314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fcom   qword ptr [eax]
        add    byte ptr [eax], al
        fcom   qword ptr [eax]
        add    byte ptr [eax], al
        sti
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 081h, 011h, 000h, 000h, 081h
        adc    dword ptr [eax], eax
        db  000h, 0FBh
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_C340:
        sbb    cl, 0
        db  000h
sub_1_C344:
        mov    dh, 14h
        add    byte ptr [eax], al
        mov    dh, 14h
        add    byte ptr [eax], al
        mov    dh, 14h
        add    byte ptr [eax], al
        db  025h, 014h, 000h, 000h, 07Eh
        or     al, byte ptr [eax]
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 040h, 000h
        add    byte ptr [eax], al
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 0ACh, 000h, 000h, 000h, 0ACh, 000h
        add    byte ptr [eax], al
        lodsb
        add    byte ptr [eax], al
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 019h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 030h
        add    byte ptr [eax], al
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 050h, 003h
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0
        add    byte ptr [eax], al
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 044h, 00Ah, 000h
        db  000h, 061h, 00Ah
        add    byte ptr [eax], al
sub_1_C410:
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0B6h, 000h, 000h, 000h, 0D0h
        add    byte ptr [eax], al
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 02Fh
        add    dword ptr [eax], eax
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0A7h, 014h, 000h, 000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 0B6h, 014h, 000h, 000h, 0B6h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h
sub_1_C4DC:
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 098h, 00Ch, 000h, 000h, 016h
        db  00Dh, 000h, 000h, 01Ah, 00Dh
        add    byte ptr [eax], al
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0FAh
        or     al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0DAh
        or     al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 0DCh
        or     al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 07Dh, 00Dh
        add    byte ptr [eax], al
        and    edx, dword ptr [eax + eax]
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        db  000h, 023h
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [26h], ah
        daa
        sub    byte ptr [ecx], ch
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 2f2eh], ch
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        and    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
loc_1_C5DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_C5E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_C6E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   ecx, eax
        adc    al, byte ptr [eax]
        db  000h, 097h, 012h, 000h, 000h, 008h
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0DDh
        adc    al, byte ptr [eax]
        db  000h, 0DFh
        adc    al, byte ptr [eax]
        db  000h, 008h
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    bh, bh
        inc    byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    bh, bh
        inc    byte ptr [eax + eax]
        add    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        db  0FFh, 0FEh, 006h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 008h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 00Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 00Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 00Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 010h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 012h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 014h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 016h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 018h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 01Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 01Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 01Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 020h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 022h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 024h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 026h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 028h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 02Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 02Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 02Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 030h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 032h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 034h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 036h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 038h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 03Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 03Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 03Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 040h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 042h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 044h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 046h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 048h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 04Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 04Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 04Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 050h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 052h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 054h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 056h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 058h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 05Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 05Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 05Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 060h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 062h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 064h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 066h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 068h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 06Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 06Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 06Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 070h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 072h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 074h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 076h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 078h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 07Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 07Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 07Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 080h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 082h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 084h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 086h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 088h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 08Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 08Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 08Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 090h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 092h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 094h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 096h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 098h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 09Ah, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 09Ch, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 09Eh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0A2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0A6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0AAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0ACh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0AEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0B2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0B6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0BAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0BCh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0BEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0C2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0C6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0CAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0CCh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0CEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0D2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0D6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0DAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0DCh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0DEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0E2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0E6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0EAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0ECh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0EEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F0h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0F2h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F4h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0F6h, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F8h, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0FAh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0FCh, 000h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0FEh, 000h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 000h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 002h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 004h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 006h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 008h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 00Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 00Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 00Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 010h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 012h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 014h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 016h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 018h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 01Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 01Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 01Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 020h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 022h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 024h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 026h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 028h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 02Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 02Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 02Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 030h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 032h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 034h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 036h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 038h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 03Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 03Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 03Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 040h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 042h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 044h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 046h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 048h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 04Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 04Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 04Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 050h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 052h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 054h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 056h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 058h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 05Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 05Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 05Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 060h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 062h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 064h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 066h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 068h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 06Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 06Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 06Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 070h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 072h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 074h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 076h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 078h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 07Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 07Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 07Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 080h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 082h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 084h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 086h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 088h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 08Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 08Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 08Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 090h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 092h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 094h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 096h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 098h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 09Ah, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 09Ch, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 09Eh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0A2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0A6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0A8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0AAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0ACh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0AEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0B2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0B6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0B8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0BAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0BCh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0BEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0C2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0C6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0C8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0CAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0CCh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0CEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0D2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0D6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0D8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0DAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0DCh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0DEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0E2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0E6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0E8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0EAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0ECh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0EEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F0h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0F2h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F4h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0F6h, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0F8h, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0FAh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 0FCh, 001h, 000h, 000h, 03Bh, 000h
        db  0FFh, 0FEh, 0FEh, 001h, 000h, 000h, 03Bh, 000h, 0FFh, 0FEh, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h
        db  000h, 000h, 03Ch, 015h, 000h, 000h, 03Ch, 015h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D310:
        call   sub_1_D315
_LTEXT ends
        end
