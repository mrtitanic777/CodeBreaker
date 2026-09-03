; vflatd_lcode_01.asm -- VFLATD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VFLATD_Control:
        clc
        ret
sub_1_0002:
        db  066h, 083h, 03Dh, 002h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_007B
        db  0A1h, 0F8h, 002h, 000h, 000h
        db  08Ah, 00Dh, 04Ah, 001h, 000h, 000h
        shr    eax, cl
        db  0A3h, 04Ch, 001h, 000h, 000h
        shl    eax, cl
        db  00Fh, 0B7h, 00Dh, 002h, 003h, 000h, 000h
        lsl    edx, ecx
        shr    edx, 0ch
        push   0
        push   80h
        push   92h
        push   edx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        db  00Fh, 0B7h, 00Dh, 002h, 003h, 000h, 000h
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        db  083h, 03Dh, 024h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_007B
        db  08Bh, 015h, 0FFh, 001h, 000h, 000h
        db  08Bh, 00Dh, 004h, 003h, 000h, 000h
        shr    ecx, 0ch
loc_1_0072:
        mov    byte ptr [edx], 3
        add    edx, 4
        dec    ecx
        jne    SHORT loc_1_0072
loc_1_007B:
        ret
VFLATD_Service_01:
        db  089h, 035h, 020h, 003h, 000h, 000h
sub_1_0082:
        db  066h, 083h, 03Dh, 002h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_00BE
        db  083h, 03Dh, 01Ch, 003h, 000h, 000h, 000h
        jne    SHORT loc_1_00BE
        pushad
        call   2
        db  0BEh, 0DCh, 000h, 000h, 000h
        db  0CDh, 020h, 0E9h, 000h, 001h, 000h
        db  0BEh, 05Ah, 001h, 000h, 000h
        db  0B8h, 030h, 001h, 000h, 000h
        sub    eax, esi
        mov    ah, al
        mov    al, 0ebh
        xchg   dword ptr [esi], eax
        db  0A3h, 01Ch, 003h, 000h, 000h
        popad
loc_1_00BE:
        ret
loc_1_00BF:
        cmp    edx, 3c4h
        je     SHORT loc_1_00CD
loc_1_00C7:
        db  0FFh, 025h, 018h, 003h, 000h, 000h
loc_1_00CD:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_00C7
        call   sub_1_0082
        jmp    SHORT loc_1_00C7
loc_1_00DC:
        mov    eax, dword ptr [edi]
        db  03Bh, 005h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_0130
        xor    eax, eax
        db  087h, 005h, 01Ch, 003h, 000h, 000h
        db  0A3h, 05Ah, 001h, 000h, 000h
        db  0BEh, 0DCh, 000h, 000h, 000h
        db  0CDh, 020h, 0EAh, 000h, 001h, 000h
        db  08Bh, 035h, 020h, 003h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0113
        call   esi
        xor    esi, esi
        db  089h, 035h, 020h, 003h, 000h, 000h
        ret
loc_1_0113:
        mov    edx, 3c4h
        mov    ecx, 0
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        clc
        ret
loc_1_0130:
        stc
        ret
        db  0B8h, 000h, 010h, 000h, 000h, 00Fh, 022h, 0D0h, 05Ah
loc_1_013B:
        pop    eax
loc_1_013C:
        ljmp   0:0
        add    byte ptr [eax + 0fh], dl
        and    al, dl
        shr    eax, 10h
        sub    eax, 12345678h
        jl     SHORT loc_1_013B
        cmp    eax, 12345678h
        jge    SHORT loc_1_013B
        push   edx
loc_1_015A:
        mov    edx, 12345678h
        sub    edx, eax
loc_1_0161:
        db  036h, 089h, 015h, 04Ch, 001h, 000h, 000h
loc_1_0168:
        shl    edx, 8
        mov    byte ptr ss:[12345678h], dl
        shr    edx, 8
loc_1_0175:
        mov    byte ptr ss:[12345678h], dl
        db  036h, 088h, 035h
loc_1_017F:
        js     SHORT loc_1_01D7
        xor    al, 12h
        mov    edx, es
        mov    es, edx
        mov    edx, ds
        mov    ds, edx
loc_1_018B:
        pop    edx
        pop    eax
        add    esp, 4
loc_1_0190:
        iretd
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_01D7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01F3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        mov    edx, ss
        mov    ds, dx
        db  0BAh
loc_1_01FF:
        db  078h, 056h
        xor    al, 12h
        mov    byte ptr [edx], 3
        mov    byte ptr [edx + 4], 3
        mov    byte ptr [edx + 8], 3
        mov    byte ptr [edx + 0ch], 3
        mov    byte ptr [edx + 10h], 3
        mov    byte ptr [edx + 14h], 3
        mov    byte ptr [edx + 18h], 3
        mov    byte ptr [edx + 1ch], 3
        mov    byte ptr [edx + 20h], 3
        mov    byte ptr [edx + 24h], 3
        mov    byte ptr [edx + 28h], 3
        mov    byte ptr [edx + 2ch], 3
        mov    byte ptr [edx + 30h], 3
        mov    byte ptr [edx + 34h], 3
        mov    byte ptr [edx + 38h], 3
        mov    byte ptr [edx + 3ch], 3
        mov    edx, eax
        shl    edx, 6
        add    edx, 12345678h
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 4], 7
        mov    byte ptr [edx + 8], 7
        mov    byte ptr [edx + 0ch], 7
        mov    byte ptr [edx + 10h], 7
        mov    byte ptr [edx + 14h], 7
        mov    byte ptr [edx + 18h], 7
        mov    byte ptr [edx + 1ch], 7
        mov    byte ptr [edx + 20h], 7
        mov    byte ptr [edx + 24h], 7
        mov    byte ptr [edx + 28h], 7
        mov    byte ptr [edx + 2ch], 7
        mov    byte ptr [edx + 30h], 7
        mov    byte ptr [edx + 34h], 7
        mov    byte ptr [edx + 38h], 7
        mov    byte ptr [edx + 3ch], 7
        db  089h, 015h, 0FFh, 001h, 000h, 000h
        mov    edx, 12345678h
        mov    cr3, edx
        pop    ds
        jmp    NEAR PTR loc_1_018B
        db  000h, 000h, 000h, 000h, 07Ch, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 01Fh, 001h, 002h, 001h, 000h, 000h, 056h, 046h, 04Ch, 041h
        db  054h, 044h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 002h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_031C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0320:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0324:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0328:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
