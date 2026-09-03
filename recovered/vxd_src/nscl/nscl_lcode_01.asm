; nscl_lcode_01.asm -- NSCL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

NSCL_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 23h
        je     NEAR PTR loc_1_0568
        cmp    eax, 4
        je     SHORT loc_1_0024
        cmp    eax, 0bh
        je     SHORT loc_1_006C
        clc
        ret
NSCL_Service_00:
        mov    ax, 100h
        clc
        ret
loc_1_0024:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_0035
        call   sub_1_2EDC
loc_1_0035:
        ret
NSCL_Service_02:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_006B
        db  083h, 025h, 020h, 039h, 000h, 000h, 0DFh
        call   sub_1_016D
        call   sub_1_0351
        db  081h, 025h, 020h, 039h, 000h, 000h, 0DDh, 0DBh, 0FFh, 0FFh
        db  083h, 03Dh, 06Ch, 039h, 000h, 000h, 000h
        je     SHORT loc_1_006B
        call   sub_1_0F51
loc_1_006B:
        ret
loc_1_006C:
        push   ebx
        call   sub_1_2E45
        pop    ebx
        ret
sub_1_0074:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        leave
        ret
sub_1_008C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_009C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    eax, 4
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, 0ah
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_00BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_00D6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_00E6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_00FA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_010E:
        push   ebx
        mov    ebx, 0ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        ret
sub_1_011C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, 0eh
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], dx
        pop    esi
        pop    ebx
        leave
        ret
sub_1_013F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 3
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0156:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 6
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        leave
        ret
sub_1_016D:
        push   ebx
        push   esi
        db  083h, 025h, 020h, 039h, 000h, 000h, 0BFh
        db  0BEh, 06Ch, 039h, 000h, 000h
        mov    ebx, 9
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cmp    ecx, 4
        jb     SHORT loc_1_0192
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 040h
loc_1_0192:
        imul   ecx, ecx, 6eh
        add    ecx, 0fah
        db  089h, 00Dh, 078h, 039h, 000h, 000h
        pop    esi
        pop    ebx
        ret
sub_1_01A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        or     eax, eax
        jne    SHORT loc_1_01C7
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, cx
        mov    dword ptr [esi], ecx
loc_1_01C7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_01CC:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_01D3:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
sub_1_01D9:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
sub_1_01DF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 0ch]
        push   1
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [ebx*2]
        push   eax
        push   edi
        push   edi
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        push   1
        push   ebx
        add    ebx, ebx
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 095h, 002h, 000h, 000h, 0A3h, 002h, 000h, 000h, 0B4h, 002h, 000h, 000h, 0A5h
        db  002h, 000h, 000h, 0F5h, 002h, 000h, 000h, 0FEh, 002h, 000h, 000h, 017h, 003h, 000h, 000h, 037h
        db  003h, 000h, 000h, 0C1h, 002h, 000h, 000h, 0DAh, 002h, 000h, 000h, 0E9h, 002h, 000h, 000h
NSCL_Service_01:
        push   0
        call   sub_1_025F
        add    esp, 4
        ret
sub_1_025F:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_027F
loc_1_026B:
        movzx  eax, al
        cmp    eax, 0bh
        jae    SHORT loc_1_028E
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        db  0FFh, 024h, 085h, 028h, 002h, 000h, 000h
loc_1_027F:
        push   eax
        push   ecx
        push   edx
        call   sub_1_06F6
        or     eax, eax
        pop    edx
        pop    ecx
        pop    eax
        je     SHORT loc_1_026B
loc_1_028E:
        stc
        mov    eax, 0ffffffffh
        ret
loc_1_0295:
        push   ebp
        push   edi
        push   ecx
        push   esi
        call   sub_1_08B7
        add    esp, 10h
        pop    ebp
        ret
loc_1_02A3:
        xor    ebx, ebx
loc_1_02A5:
        push   ebp
        push   ebx
        push   edx
        push   ecx
        push   esi
        call   sub_1_09B1
        add    esp, 14h
        pop    ebp
        ret
loc_1_02B4:
        push   ebp
        push   ecx
        push   esi
        call   sub_1_0BAB
        add    esp, 0ch
        pop    ebp
        ret
loc_1_02C1:
        push   ebp
        push   ecx
        push   edi
        mov    eax, dword ptr [esi + 2]
        push   eax
        mov    ax, word ptr [esi]
        push   eax
        add    esi, 6
        push   esi
        call   sub_1_0C7E
        add    esp, 18h
        pop    ebp
        ret
loc_1_02DA:
        push   ebp
        push   ecx
        push   edi
        push   dword ptr [esi]
        call   sub_1_0EC2
        add    esp, 10h
        pop    ebp
        ret
loc_1_02E9:
        push   ebp
        push   esi
        call   sub_1_0D9A
        add    esp, 8
        pop    ebp
        ret
loc_1_02F5:
        push   ebx
        call   sub_1_1076
        pop    ebx
        pop    ebp
        ret
loc_1_02FE:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 24h]
        movzx  ecx, word ptr [edi + 2]
        xchg   ch, cl
        lea    esi, [esi + 1ch]
        call   sub_1_03AB
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0317:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0FDh, 0FFh, 0FFh
        mov    ebx, 0eh
        mov    dx, 5505h
        db  0BEh, 0ABh, 003h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebp
        ret
loc_1_0337:
        push   ecx
        db  0CDh, 020h, 000h, 000h, 092h, 004h
        pop    ecx
        cmc
        sbb    eax, eax
        and    ecx, eax
        db  089h, 00Dh, 024h, 039h, 000h, 000h
        call   sub_1_0351
        pop    ebp
        ret
sub_1_0351:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0FEh, 0FFh, 0FFh
        xor    eax, eax
        db  039h, 005h, 024h, 039h, 000h, 000h
        je     SHORT loc_1_037C
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_1_037C
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 001h, 000h, 000h
        ret
loc_1_037C:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0FDh, 0FFh, 0FFh
        ret
loc_1_0387:
        cmp    byte ptr [eax + 9], 0
        jne    SHORT loc_1_03A0
        mov    edi, dword ptr [eax + 24h]
        movzx  ecx, word ptr [edi + 2]
        xchg   ch, cl
        lea    esi, [eax + 1ch]
        push   eax
        call   sub_1_03AB
        pop    eax
loc_1_03A0:
        mov    edx, eax
        db  087h, 015h, 09Ch, 039h, 000h, 000h
        mov    dword ptr [eax], edx
        ret
sub_1_03AB:
        lea    ebx, [edi + 1eh]
        cmp    byte ptr [edi + 5], 14h
        jne    SHORT loc_1_03B7
        add    ebx, 20h
loc_1_03B7:
        cmp    word ptr [edi + 1ch], 5505h
        jne    SHORT loc_1_03DF
        cmp    dword ptr [ebx], 53575242h
        jne    SHORT loc_1_03DF
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    ecx, eax
        jb     SHORT loc_1_0401
        push   esi
        push   ecx
        push   ebx
        push   edi
        call   sub_1_1F80
        add    esp, 10h
        jmp    SHORT loc_1_0455
loc_1_03DF:
        cmp    word ptr [edi + 1ch], 5204h
        jne    SHORT loc_1_0433
        cmp    word ptr [edi + 1eh], 400h
        je     SHORT loc_1_03F7
        cmp    word ptr [edi + 1eh], 200h
        jne    SHORT loc_1_0433
loc_1_03F7:
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    ecx, eax
        jae    SHORT loc_1_041E
loc_1_0401:
        db  08Bh, 035h, 09Ch, 039h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0458
        mov    edx, dword ptr [esi]
        db  089h, 015h, 09Ch, 039h, 000h, 000h
        xor    ebx, ebx
        dec    ebx
        db  0C7h, 046h, 004h, 087h, 003h, 000h, 000h
        ret
loc_1_041E:
        add    edi, 20h
        sub    ecx, 20h
        shr    ecx, 6
        push   ecx
        push   edi
        call   sub_1_26A0
        add    esp, 8
        jmp    SHORT loc_1_0455
loc_1_0433:
        cmp    dword ptr [ebx], 53575242h
        je     SHORT loc_1_0455
        movzx  eax, word ptr [edi + 2]
        xchg   ah, al
        cmp    eax, 28h
        jb     SHORT loc_1_0455
        cmp    ecx, eax
        jb     SHORT loc_1_0401
        push   ecx
        push   ebx
        push   edi
        call   sub_1_37E7
        add    esp, 0ch
loc_1_0455:
        xor    esi, esi
        ret
loc_1_0458:
        xor    esi, esi
        ret
sub_1_045B:
        db  0A1h, 09Ch, 039h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_046D
        mov    edx, dword ptr [eax]
        db  089h, 015h, 09Ch, 039h, 000h, 000h
        ret
loc_1_046D:
        stc
        ret
sub_1_046F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 28h], 440h
        mov    edx, eax
        db  087h, 015h, 09Ch, 039h, 000h, 000h
        mov    dword ptr [eax], edx
        leave
        ret
loc_1_0487:
        mov    dword ptr [edx + 10h], 0
        push   edx
        call   sub_1_114B
        pop    edx
        ret
loc_1_0496:
        mov    dword ptr [edx + 14h], 0
        push   edx
        call   sub_1_114B
        pop    edx
        ret
loc_1_04A5:
        xor    eax, eax
        mov    dword ptr [edx + 10h], eax
        push   eax
        push   eax
        push   edx
        call   sub_1_0FBB
        add    esp, 0ch
        ret
loc_1_04B6:
        mov    dword ptr [edx + 14h], 0
        push   edx
        mov    byte ptr [edx + 8], 0
        call   dword ptr [edx + 24h]
        pop    edx
        ret
loc_1_04C7:
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 002h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 092h, 004h
        ret
NSCL_V86_API:
        push   ebx
        movzx  esi, word ptr [ebp + 4]
        movzx  edi, word ptr [ebp]
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        add    esi, eax
        add    edi, eax
        movzx  eax, word ptr [ebp + 1ch]
        movzx  ebx, word ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        movzx  edx, word ptr [ebp + 14h]
        cmp    al, 0bh
        jae    SHORT loc_1_050E
        call   sub_1_025F
        pop    ebx
loc_1_0509:
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_050E:
        mov    ax, 0ffffh
        jmp    SHORT loc_1_0509
NSCL_PM_API:
        push   ebx
        movzx  eax, word ptr [ebp + 3ch]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        movzx  eax, word ptr [ebp + 38h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  edi, word ptr [ebp]
        add    edi, eax
        movzx  eax, word ptr [ebp + 1ch]
        movzx  ebx, word ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        movzx  edx, word ptr [ebp + 14h]
        cmp    al, 4
        jae    SHORT loc_1_0562
        call   sub_1_025F
        pop    ebx
loc_1_055D:
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_0562:
        mov    ax, 0ffffh
        jmp    SHORT loc_1_055D
loc_1_0568:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_057D
        call   sub_1_06F6
        or     eax, eax
        jne    SHORT loc_1_05A9
loc_1_057D:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_05A6
        mov    eax, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 1ch]
        call   sub_1_05AF
        jb     SHORT loc_1_05A9
        push   eax
        push   eax
        push   esi
        call   sub_1_0746
        add    esp, 8
        mov    ebx, eax
        pop    eax
        call   sub_1_0601
        mov    eax, ebx
        ret
loc_1_05A6:
        xor    eax, eax
        ret
loc_1_05A9:
        mov    eax, 0ffffffffh
        ret
sub_1_05AF:
        or     eax, eax
        je     SHORT loc_1_05F7
        push   ebx
        push   edi
        push   esi
        push   eax
        push   ecx
        mov    ecx, eax
        and    ecx, 0fffh
        add    ebx, ecx
        shr    eax, 0ch
        add    ebx, 0fffh
        shr    ebx, 0ch
        pop    ecx
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_05FD
        cmp    eax, 0fffffbadh
        je     SHORT loc_1_05F8
        pop    ebx
        and    ebx, 0fffh
        add    eax, ebx
        clc
loc_1_05F4:
        pop    esi
        pop    edi
        pop    ebx
loc_1_05F7:
        ret
loc_1_05F8:
        int3
        stc
        pop    ebx
        jmp    SHORT loc_1_05F4
loc_1_05FD:
        stc
        pop    ebx
        jmp    SHORT loc_1_05F4
sub_1_0601:
        or     eax, eax
        je     SHORT loc_1_0638
        push   ebx
        push   edi
        push   esi
        push   ecx
        mov    ecx, eax
        and    ecx, 0fffh
        add    ebx, ecx
        shr    eax, 0ch
        add    ebx, 0fffh
        shr    ebx, 0ch
        pop    ecx
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0639
        clc
loc_1_0635:
        pop    esi
        pop    edi
        pop    ebx
loc_1_0638:
        ret
loc_1_0639:
        jmp    SHORT loc_1_0635
        db  000h, 01Eh, 000h, 000h, 000h, 054h, 002h, 000h, 000h, 036h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 094h, 004h, 001h, 000h, 000h, 000h, 04Eh, 053h, 043h, 04Ch
        db  020h, 020h, 020h, 020h, 06Eh, 060h, 000h, 054h, 000h, 000h, 000h, 000h, 0D8h, 004h, 000h, 000h
        db  014h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 006h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06C0:
        push   esi
        db  0A1h, 0B8h, 039h, 000h, 000h
        imul   eax, eax, 41c64e6dh
        add    eax, 3039h
        db  0A3h, 0B8h, 039h, 000h, 000h
        jne    SHORT loc_1_06DD
        mov    eax, 1
loc_1_06DD:
        db  0A3h, 0B8h, 039h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        sub    esi, ecx
        inc    esi
        sub    edx, edx
        div    esi
        lea    eax, [ecx + edx]
        pop    esi
        ret
sub_1_06F6:
        sub    esp, 4
        mov    word ptr [esp + 2], 5505h
        call   sub_1_01CC
        db  0A3h, 0B8h, 039h, 000h, 000h
        push   0
        push   0
        lea    eax, [esp + 0ah]
        db  068h, 0ABh, 003h, 000h, 000h
        push   eax
        call   sub_1_011C
        add    esp, 10h
        or     eax, eax
        mov    eax, 0ffffffffh
        jne    SHORT loc_1_0742
        call   sub_1_016D
        db  0A1h, 020h, 039h, 000h, 000h
        or     eax, 1
        db  0A3h, 020h, 039h, 000h, 000h
        call   sub_1_0F51
        sub    eax, eax
loc_1_0742:
        add    esp, 4
        ret
sub_1_0746:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        cmp    eax, 5
        ja     SHORT loc_1_075A
        db  0FFh, 024h, 085h, 0FFh, 007h, 000h, 000h
loc_1_075A:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_07FE
loc_1_0764:
        mov    ecx, dword ptr [ecx + 10h]
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    ax, word ptr [ecx]
        add    ecx, 2
        push   eax
        push   ecx
        call   sub_1_08B7
        add    esp, 10h
        jmp    SHORT loc_1_07FE
loc_1_0780:
        mov    edx, dword ptr [ecx + 10h]
        push   0
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [ecx + 1ch]
        add    edx, 6
        push   eax
        push   dword ptr [edx - 4]
        mov    ax, word ptr [edx - 6]
        push   eax
        push   edx
        call   sub_1_0C7E
        add    esp, 18h
        jmp    SHORT loc_1_07FE
loc_1_07A3:
        push   0
        push   dword ptr [ecx + 1ch]
        mov    eax, dword ptr [ecx + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [eax]
        call   sub_1_0EC2
        add    esp, 10h
        jmp    SHORT loc_1_07FE
loc_1_07BB:
        mov    ecx, dword ptr [ecx + 10h]
        push   0
        push   0
        mov    ax, word ptr [ecx]
        add    ecx, 4
        push   eax
        mov    ax, word ptr [ecx - 2]
        push   eax
        push   ecx
        call   sub_1_09B1
        add    esp, 14h
        jmp    SHORT loc_1_07FE
loc_1_07D9:
        mov    eax, dword ptr [ecx + 10h]
        push   0
        mov    cx, word ptr [eax + 2]
        push   ecx
        add    eax, 4
        push   eax
        call   sub_1_0BAB
        add    esp, 0ch
        jmp    SHORT loc_1_07FE
loc_1_07F1:
        push   0
        push   dword ptr [ecx + 10h]
        call   sub_1_0D9A
        add    esp, 8
loc_1_07FE:
        ret
        db  064h, 007h, 000h, 000h, 080h, 007h, 000h, 000h, 0A3h, 007h, 000h, 000h, 0BBh, 007h, 000h, 000h
        db  0D9h, 007h, 000h, 000h, 0F1h, 007h, 000h, 000h
sub_1_0817:
        db  066h, 083h, 03Dh, 098h, 039h, 000h, 000h, 000h
        jne    SHORT loc_1_084A
        push   0
        push   0
        db  068h, 0ABh, 003h, 000h, 000h
        db  068h, 098h, 039h, 000h, 000h
        call   sub_1_011C
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_1_084A
        mov    edx, dword ptr [esp + 4]
        mov    byte ptr [edx + 8], 0
        mov    byte ptr [edx + 0bh], 5
        jmp    SHORT loc_1_08B6
loc_1_084A:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [edx + 8], 0ffh
        mov    byte ptr [edx + 9], al
        movzx  eax, al
        mov    byte ptr [edx + 0ah], 0
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 20h], ecx
        mov    dword ptr [edx + 10h], 0
        mov    dword ptr [edx + 14h], 0
        db  08Bh, 004h, 085h, 0B8h, 039h, 000h, 000h
        mov    dword ptr [edx + 18h], eax
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [ecx + 22h], 1
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        db  0A1h, 080h, 039h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0C7h, 042h, 004h, 080h, 039h, 000h, 000h
        db  089h, 015h, 080h, 039h, 000h, 000h
        mov    eax, dword ptr [edx]
        push   edx
        mov    dword ptr [eax + 4], edx
        db  0FFh, 005h, 07Ch, 039h, 000h, 000h
        call   dword ptr [edx + 18h]
        add    esp, 4
loc_1_08B6:
        ret
sub_1_08B7:
        sub    esp, 160h
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [esp + 174h]
        or     ax, ax
        jne    SHORT loc_1_08D7
        mov    eax, 8
        jmp    NEAR PTR loc_1_09A7
loc_1_08D7:
        mov    dword ptr [esp + 5ch], 0
        mov    dword ptr [esp + 70h], 0
        mov    word ptr [esp + 74h], ax
        mov    dword ptr [esp + 68h], 0
        mov    ecx, 0ffffffffh
        mov    eax, dword ptr [esp + 17ch]
        mov    dword ptr [esp + 44h], eax
        mov    edx, dword ptr [esp + 170h]
        mov    edi, edx
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 2fh
        jbe    SHORT loc_1_0923
        mov    eax, 4
        jmp    NEAR PTR loc_1_09A7
loc_1_0923:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 76h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 0c4h]
        push   2
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        call   sub_1_0817
        add    esp, 10h
        cmp    byte ptr [esp + 40h], 0ffh
        jne    SHORT loc_1_0983
        mov    ebx, 0ffh
loc_1_096E:
        lea    eax, [esp + 38h]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 40h], bl
        je     SHORT loc_1_096E
loc_1_0983:
        cmp    byte ptr [esp + 43h], 0
        jne    SHORT loc_1_09A2
        mov    edi, dword ptr [esp + 178h]
        or     edi, edi
        je     SHORT loc_1_09A2
        lea    esi, [esp + 74h]
        mov    ecx, 13h
        rep movsd
        movsw
loc_1_09A2:
        movzx  eax, byte ptr [esp + 43h]
loc_1_09A7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 160h
        ret
sub_1_09B1:
        sub    esp, 168h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 010h
        je     SHORT loc_1_09CE
        mov    eax, 6
        jmp    NEAR PTR loc_1_0B3D
loc_1_09CE:
        push   44h
        call   sub_1_00E6
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_09EA
        mov    eax, 5
        jmp    NEAR PTR loc_1_0B3D
loc_1_09EA:
        mov    ebp, dword ptr [esp + 188h]
        or     ebp, ebp
        jne    SHORT loc_1_0A12
        lea    ebx, [esp + 44h]
        lea    edx, [esp + 18h]
        lea    eax, [esp + 0d0h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 68h], 0
        jmp    SHORT loc_1_0A63
loc_1_0A12:
        push   15ah
        call   sub_1_00E6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0A3B
        push   dword ptr [esp + 10h]
        call   sub_1_00FA
        add    esp, 4
        mov    eax, 5
        jmp    NEAR PTR loc_1_0B3D
loc_1_0A3B:
        lea    eax, [ebx + 0b4h]
        lea    edx, [ebx + 8ah]
        mov    dword ptr [esp + 14h], eax
        mov    byte ptr [ebp + 8], 0ffh
        mov    dword ptr [ebp], ebx
        db  0C7h, 043h, 024h, 048h, 00Bh, 000h, 000h
        mov    dword ptr [ebx + 28h], ebp
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp + 0ch], eax
loc_1_0A63:
        mov    ax, word ptr [esp + 180h]
        mov    word ptr [ebx + 3ch], ax
        mov    edi, dword ptr [esp + 17ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 30h
        jbe    SHORT loc_1_0A91
        mov    eax, 4
        jmp    NEAR PTR loc_1_0B3D
loc_1_0A91:
        mov    edi, dword ptr [esp + 17ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 3eh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 06Ch, 039h, 000h, 000h
        lea    edi, [ebx + 6eh]
        movsd
        movsd
        movsw
        mov    si, word ptr [esp + 184h]
        mov    word ptr [ebx + 78h], si
        mov    eax, dword ptr [esp + 18ch]
        push   3
        mov    dword ptr [ebx + 0ch], eax
        push   dword ptr [esp + 18h]
        push   edx
        push   ebx
        call   sub_1_0817
        add    esp, 10h
        or     ebp, ebp
        je     SHORT loc_1_0AF2
        sub    eax, eax
        jmp    SHORT loc_1_0B3D
loc_1_0AF2:
        cmp    byte ptr [ebx + 8], 0ffh
        jne    SHORT loc_1_0B09
loc_1_0AF8:
        push   1
        push   ebx
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [ebx + 8], 0ffh
        je     SHORT loc_1_0AF8
loc_1_0B09:
        cmp    byte ptr [ebx + 0bh], 0
        push   dword ptr [esp + 10h]
        jne    SHORT loc_1_0B31
        movzx  eax, si
        push   eax
        movzx  eax, word ptr [esp + 188h]
        push   eax
        push   dword ptr [esp + 188h]
        call   sub_1_282B
        add    esp, 10h
        jmp    SHORT loc_1_0B39
loc_1_0B31:
        call   sub_1_00FA
        add    esp, 4
loc_1_0B39:
        movzx  eax, byte ptr [ebx + 0bh]
loc_1_0B3D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 168h
        ret
loc_1_0B48:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 28h]
        cmp    byte ptr [edi + 0bh], 0
        push   dword ptr [esi + 0ch]
        jne    SHORT loc_1_0B72
        movzx  eax, word ptr [edi + 78h]
        push   eax
        movzx  eax, word ptr [edi + 3ch]
        push   eax
        lea    eax, [edi + 3eh]
        push   eax
        call   sub_1_282B
        add    esp, 10h
        jmp    SHORT loc_1_0B7A
loc_1_0B72:
        call   sub_1_00FA
        add    esp, 4
loc_1_0B7A:
        or     esi, esi
        je     SHORT loc_1_0B8E
        mov    al, byte ptr [edi + 0bh]
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi], 0
loc_1_0B8E:
        push   edi
        call   sub_1_00FA
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_0BA8
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_0BA8
        push   esi
        call   eax
        add    esp, 4
loc_1_0BA8:
        pop    edi
        pop    esi
        ret
sub_1_0BAB:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_1_27D0
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_0BD2
        mov    eax, 1
        jmp    NEAR PTR loc_1_0C76
loc_1_0BD2:
        push   15ah
        call   sub_1_00E6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0BF3
        push   ebp
        call   sub_1_28ED
        add    esp, 4
        jmp    NEAR PTR loc_1_0C74
loc_1_0BF3:
        lea    eax, [ebx + 8ah]
        mov    dword ptr [esp + 10h], eax
        add    eax, 2ah
        mov    dword ptr [esp + 14h], eax
        db  0C7h, 043h, 024h, 0AEh, 00Fh, 000h, 000h
        mov    dword ptr [ebx + 28h], 0
        mov    eax, dword ptr [esp + 24h]
        lea    edi, [ebp + 14h]
        mov    dword ptr [ebx + 0ch], eax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [ebx + 3ch], ax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 3eh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 06Ch, 039h, 000h, 000h
        lea    edi, [ebx + 6eh]
        movsd
        movsd
        movsw
        mov    ax, word ptr [ebp + 0eh]
        mov    word ptr [ebx + 78h], ax
        push   ebp
        call   sub_1_28ED
        add    esp, 4
        push   4
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   ebx
        call   sub_1_0817
        add    esp, 10h
loc_1_0C74:
        sub    eax, eax
loc_1_0C76:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_0C7E:
        sub    esp, 154h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 7ch], 0
        mov    ax, word ptr [esp + 168h]
        mov    word ptr [esp + 94h], ax
        mov    ecx, dword ptr [esp + 170h]
        mov    dword ptr [esp + 8ch], ecx
        mov    eax, dword ptr [esp + 174h]
        mov    dword ptr [esp + 90h], eax
        mov    eax, dword ptr [esp + 16ch]
        mov    dword ptr [esp + 88h], eax
        mov    eax, dword ptr [esp + 178h]
        mov    dword ptr [esp + 64h], eax
        sub    eax, eax
        mov    word ptr [ecx + 4], 0
        mov    dword ptr [ecx], 0
        mov    edx, dword ptr [esp + 164h]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_1_0D04
        mov    eax, 4
        jmp    NEAR PTR loc_1_0D90
loc_1_0D04:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 96h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 0ch]
        push   eax
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [esp + 9ah]
        push   ecx
        push   eax
        call   sub_1_01DF
        add    esp, 0ch
        lea    eax, [esp + 0b8h]
        push   5
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        lea    eax, [esp + 64h]
        push   eax
        call   sub_1_0817
        add    esp, 10h
        cmp    byte ptr [esp + 60h], 0ffh
        jne    SHORT loc_1_0D8B
        mov    ebx, 0ffh
loc_1_0D76:
        lea    eax, [esp + 58h]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 60h], bl
        je     SHORT loc_1_0D76
loc_1_0D8B:
        movzx  eax, byte ptr [esp + 63h]
loc_1_0D90:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 154h
        ret
sub_1_0D9A:
        sub    esp, 154h
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 164h]
        mov    eax, edx
        db  0B9h, 0A8h, 006h, 000h, 000h
loc_1_0DB1:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_1_0DD1
        or     bl, bl
        je     SHORT loc_1_0DCD
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    SHORT loc_1_0DD1
        add    eax, 2
        add    ecx, 2
        or     bl, bl
        jne    SHORT loc_1_0DB1
loc_1_0DCD:
        sub    eax, eax
        jmp    SHORT loc_1_0DD6
loc_1_0DD1:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0DD6:
        or     eax, eax
        jne    SHORT loc_1_0DE1
        sub    eax, eax
        jmp    NEAR PTR loc_1_0EB8
loc_1_0DE1:
        mov    dword ptr [esp + 7ch], 0
        mov    word ptr [esp + 94h], 0
        mov    dword ptr [esp + 8ch], 0
        mov    dword ptr [esp + 90h], 1
        mov    edi, edx
        mov    dword ptr [esp + 88h], 0
        mov    eax, dword ptr [esp + 168h]
        mov    dword ptr [esp + 64h], eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_1_0E39
        mov    eax, 4
        jmp    SHORT loc_1_0EB8
loc_1_0E39:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 96h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 9ah]
        push   4
        push   eax
        call   sub_1_01DF
        add    esp, 0ch
        lea    eax, [esp + 0b8h]
        push   7
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        lea    eax, [esp + 64h]
        push   eax
        call   sub_1_0817
        add    esp, 10h
        cmp    byte ptr [esp + 60h], 0ffh
        jne    SHORT loc_1_0EB3
        mov    ebx, 0ffh
loc_1_0E9E:
        lea    eax, [esp + 58h]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 60h], bl
        je     SHORT loc_1_0E9E
loc_1_0EB3:
        movzx  eax, byte ptr [esp + 63h]
loc_1_0EB8:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 154h
        ret
sub_1_0EC2:
        sub    esp, 134h
        push   ebx
        mov    dword ptr [esp + 54h], 0
        mov    ecx, dword ptr [esp + 140h]
        mov    dword ptr [esp + 64h], ecx
        mov    eax, dword ptr [esp + 144h]
        mov    dword ptr [esp + 68h], eax
        mov    eax, dword ptr [esp + 13ch]
        mov    dword ptr [esp + 60h], eax
        mov    eax, dword ptr [esp + 148h]
        mov    dword ptr [esp + 3ch], eax
        lea    eax, [esp + 90h]
        push   6
        push   eax
        mov    dword ptr [ecx], 0
        lea    eax, [esp + 0ch]
        push   eax
        mov    byte ptr [ecx + 4], 0
        lea    eax, [esp + 3ch]
        push   eax
        call   sub_1_0817
        add    esp, 10h
        cmp    byte ptr [esp + 38h], 0ffh
        jne    SHORT loc_1_0F44
        mov    ebx, 0ffh
loc_1_0F2F:
        lea    eax, [esp + 30h]
        push   1
        push   eax
        call   sub_1_01D3
        add    esp, 8
        cmp    byte ptr [esp + 38h], bl
        je     SHORT loc_1_0F2F
loc_1_0F44:
        movzx  eax, byte ptr [esp + 3bh]
        pop    ebx
        add    esp, 134h
        ret
sub_1_0F51:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 082h, 020h, 000h, 000h
        jne    SHORT loc_1_0F87
        db  0A1h, 088h, 039h, 000h, 000h
        lea    ecx, [eax + 68h]
        add    eax, 3eh
        db  08Bh, 015h, 088h, 039h, 000h, 000h
        push   1
        mov    dword ptr [edx + 24h], 0
        push   ecx
        push   eax
        db  0FFh, 035h, 088h, 039h, 000h, 000h
        call   sub_1_0817
        add    esp, 10h
loc_1_0F87:
        ret
loc_1_0F88:
        db  0C7h, 005h, 034h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 082h, 020h, 000h, 000h
        jne    SHORT loc_1_0FAD
        push   0
        db  068h, 051h, 00Fh, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
loc_1_0FAD:
        ret
loc_1_0FAE:
        push   dword ptr [esp + 4]
        call   sub_1_00FA
        add    esp, 4
        ret
sub_1_0FBB:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 0ah], 0ffh
        je     SHORT loc_1_0FCE
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esi + 0bh], al
loc_1_0FCE:
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_0FE5
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 10h], 0
loc_1_0FE5:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_0FFC
        push   eax
        call   sub_1_00D6
        add    esp, 4
        mov    dword ptr [esi + 14h], 0
loc_1_0FFC:
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_1034
        or     edi, edi
        je     SHORT loc_1_1018
        push   eax
        call   sub_1_0156
        add    esp, 4
        jmp    SHORT loc_1_1034
loc_1_1018:
        mov    byte ptr [esi + 0ah], 0ffh
        push   esi
        db  068h, 0A5h, 004h, 000h, 000h
        push   1f4h
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_1_1073
loc_1_1034:
        or     edi, edi
        jne    SHORT loc_1_105E
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_1_1051
        push   esi
        db  068h, 0B6h, 004h, 000h, 000h
        call   sub_1_009C
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
        jmp    SHORT loc_1_1055
loc_1_1051:
        mov    byte ptr [esi + 8], 0
loc_1_1055:
        push   esi
        call   sub_1_01D9
        add    esp, 4
loc_1_105E:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        db  0FFh, 00Dh, 07Ch, 039h, 000h, 000h
loc_1_1073:
        pop    edi
        pop    esi
        ret
sub_1_1076:
        db  08Bh, 00Dh, 080h, 039h, 000h, 000h
        db  081h, 0F9h, 080h, 039h, 000h, 000h
        je     SHORT loc_1_1098
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
loc_1_108A:
        cmp    ecx, eax
        je     SHORT loc_1_109E
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 080h, 039h, 000h, 000h
        jne    SHORT loc_1_108A
loc_1_1098:
        mov    eax, 0ffffffffh
loc_1_109D:
        ret
loc_1_109E:
        mov    dword ptr [ecx + 28h], 0
        push   0
        push   7
        push   ecx
        call   sub_1_0FBB
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_1_109D
sub_1_10B6:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 080h, 039h, 000h, 000h
        db  081h, 0FEh, 080h, 039h, 000h, 000h
        je     SHORT loc_1_111C
        mov    bl, byte ptr [esp + 10h]
loc_1_10CB:
        mov    edi, dword ptr [esi]
        movzx  eax, byte ptr [esi + 9]
        cmp    eax, 1
        je     SHORT loc_1_10E7
        cmp    eax, 3
        je     SHORT loc_1_10F5
        cmp    eax, 5
        jl     SHORT loc_1_1112
        cmp    eax, 7
        jle    SHORT loc_1_10F5
        jmp    SHORT loc_1_1112
loc_1_10E7:
        push   0
        push   ebx
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_1112
loc_1_10F5:
        cmp    byte ptr [esi + 0ah], 1
        jne    SHORT loc_1_1112
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_1112
        push   esi
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
loc_1_1112:
        mov    esi, edi
        db  081h, 0FFh, 080h, 039h, 000h, 000h
        jne    SHORT loc_1_10CB
loc_1_111C:
        db  081h, 025h, 020h, 039h, 000h, 000h, 07Fh, 0F7h, 0FFh, 0FFh
        db  066h, 0C7h, 005h, 038h, 03Ah, 000h, 000h, 040h, 004h
        db  083h, 03Dh, 08Ch, 039h, 000h, 000h, 000h
        je     SHORT loc_1_1147
        push   0
        db  068h, 0E7h, 02Ah, 000h, 000h
        call   sub_1_009C
        add    esp, 8
loc_1_1147:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_114B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 8], 0ffh
        jne    SHORT loc_1_11AB
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_116D
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 10h], 0
loc_1_116D:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_1184
        push   eax
        call   sub_1_00D6
        add    esp, 4
        mov    dword ptr [esi + 14h], 0
loc_1_1184:
        mov    eax, dword ptr [esi + 20h]
        push   esi
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_1_11A5
        db  068h, 087h, 004h, 000h, 000h
        push   96h
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_1_11AB
loc_1_11A5:
        call   dword ptr [esi + 18h]
        add    esp, 4
loc_1_11AB:
        pop    esi
        ret
loc_1_11AD:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 0ah]
        or     eax, eax
        je     SHORT loc_1_11F2
        cmp    eax, 1
        je     SHORT loc_1_1223
        cmp    eax, 2
        je     NEAR PTR loc_1_131F
        mov    edi, dword ptr [esp + 0ch]
loc_1_11D0:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   edi
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   dword ptr [esi + 20h]
        call   sub_1_013F
        add    esp, 4
        jmp    NEAR PTR loc_1_1363
loc_1_11F2:
        mov    dword ptr [esi + 1ch], 2
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 010h
        jne    SHORT loc_1_125F
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        je     SHORT loc_1_125F
        mov    byte ptr [esi + 0ah], 1
loc_1_1223:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 004h
        jne    NEAR PTR loc_1_12BD
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        je     SHORT loc_1_1276
        db  083h, 03Dh, 038h, 039h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_130A
        push   0
        db  068h, 054h, 026h, 000h, 000h
        call   sub_1_009C
        add    esp, 8
        db  0A3h, 038h, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_130A
loc_1_125F:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 020h
        jne    SHORT loc_1_126D
        call   sub_1_0F51
loc_1_126D:
        mov    byte ptr [esi + 0ah], 2
        jmp    NEAR PTR loc_1_131F
loc_1_1276:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 044h, 03Ah, 000h, 000h
        db  068h, 044h, 039h, 000h, 000h
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_12A2
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0DFh, 0FFh, 0FFh
        call   sub_1_0F51
        jmp    SHORT loc_1_130A
loc_1_12A2:
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 004h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
loc_1_12BD:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_12EB
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        db  08Bh, 03Dh, 02Ch, 039h, 000h, 000h
        push   1
        push   0
        push   esi
        push   0
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        call   sub_1_3233
        add    esp, 18h
        jmp    NEAR PTR loc_1_11D0
loc_1_12EB:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        je     SHORT loc_1_1303
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0DBh, 0FFh, 0FFh
        call   sub_1_0F51
loc_1_1303:
        mov    dword ptr [esi + 1ch], 2
loc_1_130A:
        mov    byte ptr [esi + 0ah], 2
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
loc_1_131F:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_1356
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        db  08Bh, 03Dh, 078h, 039h, 000h, 000h
        push   2
        db  068h, 050h, 039h, 000h, 000h
        push   esi
        push   0
        add    edi, 0fah
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        call   sub_1_3233
        add    esp, 18h
        jmp    NEAR PTR loc_1_11D0
loc_1_1356:
        push   0
        push   1
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_1363:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_1369:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 10h]
        movzx  eax, byte ptr [esi + 0ah]
        or     eax, eax
        je     SHORT loc_1_13B6
        cmp    eax, 1
        je     NEAR PTR loc_1_1428
        cmp    eax, 2
        je     SHORT loc_1_13F1
        cmp    eax, 3
        je     NEAR PTR loc_1_1510
        mov    eax, dword ptr [esp + 8]
loc_1_1394:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   eax
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   dword ptr [esi + 20h]
        call   sub_1_013F
        add    esp, 4
loc_1_13B1:
        pop    esi
        add    esp, 8
        ret
loc_1_13B6:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        jne    SHORT loc_1_13D1
        mov    byte ptr [esi + 0bh], 1
        mov    byte ptr [esi + 0ah], 1
        call   sub_1_0F51
        jmp    SHORT loc_1_13B1
loc_1_13D1:
        mov    byte ptr [esi + 0bh], 0
loc_1_13D5:
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    byte ptr [esi + 0ah], 2
        mov    dword ptr [esi + 1ch], 3
loc_1_13F1:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 004h
        jne    NEAR PTR loc_1_1494
        lea    eax, [esp + 4]
        push   eax
        db  068h, 044h, 03Ah, 000h, 000h
        db  068h, 044h, 039h, 000h, 000h
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_1479
        mov    byte ptr [esi + 0bh], 1
        mov    byte ptr [esi + 0ah], 1
        call   sub_1_0F51
        jmp    SHORT loc_1_13B1
loc_1_1428:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        jne    SHORT loc_1_13D5
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        db  083h, 03Dh, 034h, 039h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_13B1
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 040h
        je     NEAR PTR loc_1_13B1
        push   0
        db  068h, 088h, 00Fh, 000h, 000h
        push   927c0h
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 034h, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_13B1
loc_1_1479:
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 004h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
loc_1_1494:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_14F4
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        je     SHORT loc_1_14CA
        push   1
        push   0
        push   esi
        push   0
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        call   sub_1_3233
        add    esp, 18h
        db  0A1h, 02Ch, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_1394
loc_1_14CA:
        push   2
        db  068h, 050h, 039h, 000h, 000h
        push   esi
        push   0
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        call   sub_1_3233
        add    esp, 18h
        db  0A1h, 078h, 039h, 000h, 000h
        add    eax, 0fah
        jmp    NEAR PTR loc_1_1394
loc_1_14F4:
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    byte ptr [esi + 0ah], 3
        mov    dword ptr [esi + 1ch], 3
loc_1_1510:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_153E
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        push   esi
        mov    ax, word ptr [esi + 78h]
        push   eax
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        push   10h
        call   sub_1_3196
        add    esp, 14h
        db  0A1h, 02Ch, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_1394
loc_1_153E:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DBh, 0FFh, 0FFh
        cmp    byte ptr [esi + 0bh], 0
        jne    SHORT loc_1_1560
        mov    byte ptr [esi + 0ah], 1
        mov    byte ptr [esi + 0bh], 1
        call   sub_1_0F51
        jmp    NEAR PTR loc_1_13B1
loc_1_1560:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_13B1
loc_1_1572:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 18h]
        movzx  eax, byte ptr [ebx + 0ah]
        or     eax, eax
        je     SHORT loc_1_15CE
        cmp    eax, 1
        je     NEAR PTR loc_1_1637
        cmp    eax, 2
        je     NEAR PTR loc_1_17B4
        cmp    eax, 3
        je     NEAR PTR loc_1_1807
        cmp    eax, 5
        je     NEAR PTR loc_1_16BB
        mov    edi, dword ptr [esp + 10h]
loc_1_15AC:
        push   ebx
        db  068h, 087h, 004h, 000h, 000h
        push   edi
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [ebx + 10h], eax
        push   dword ptr [ebx + 20h]
        call   sub_1_013F
        add    esp, 4
        jmp    NEAR PTR loc_1_1841
loc_1_15CE:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        jne    SHORT loc_1_1637
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 020h
        jne    SHORT loc_1_15EC
        mov    byte ptr [ebx + 0ah], 1
        call   sub_1_0F51
loc_1_15EC:
        db  0B8h, 0A8h, 006h, 000h, 000h
        lea    ecx, [ebx + 3eh]
loc_1_15F4:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_1614
        or     dl, dl
        je     SHORT loc_1_1610
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_1614
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_15F4
loc_1_1610:
        sub    eax, eax
        jmp    SHORT loc_1_1619
loc_1_1614:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1619:
        or     eax, eax
        jne    SHORT loc_1_1637
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 020h
        je     NEAR PTR loc_1_1841
        push   0
        push   0
        push   ebx
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_1637:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        je     SHORT loc_1_1642
        jmp    SHORT loc_1_169F
loc_1_1642:
        db  0B8h, 0A8h, 006h, 000h, 000h
        lea    ecx, [ebx + 3eh]
loc_1_164A:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_166A
        or     dl, dl
        je     SHORT loc_1_1666
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_166A
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_164A
loc_1_1666:
        sub    eax, eax
        jmp    SHORT loc_1_166F
loc_1_166A:
        sbb    eax, eax
        sbb    eax, -1
loc_1_166F:
        or     eax, eax
        jne    NEAR PTR loc_1_17A9
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        jne    SHORT loc_1_1689
        push   0
        push   0
        jmp    NEAR PTR loc_1_1838
loc_1_1689:
        lea    edi, [ebx + 4eh]
        db  0BEh, 044h, 03Ah, 000h, 000h
        movsd
        movsw
        db  0BEh, 044h, 039h, 000h, 000h
        lea    edi, [ebx + 54h]
        movsd
        movsd
        movsd
loc_1_169F:
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [ebx + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 0ah], 5
        mov    dword ptr [ebx + 1ch], 3
loc_1_16BB:
        cmp    dword ptr [ebx + 1ch], 0
        jbe    NEAR PTR loc_1_175A
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        je     SHORT loc_1_16EE
        push   1
        push   0
        push   ebx
        lea    eax, [ebx + 3eh]
        push   eax
        mov    ax, word ptr [ebx + 3ch]
        push   eax
        push   0
        call   sub_1_3233
        add    esp, 18h
        db  08Bh, 03Dh, 02Ch, 039h, 000h, 000h
        jmp    SHORT loc_1_1752
loc_1_16EE:
        lea    esi, [ebx + 4eh]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_1724
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        lea    eax, [ebx + 54h]
        push   eax
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_171B
        mov    edi, dword ptr [esp + 0ch]
        imul   edi, edi, 37h
        add    edi, 1f4h
        jmp    SHORT loc_1_173B
loc_1_171B:
        push   0
        push   0
        jmp    NEAR PTR loc_1_1838
loc_1_1724:
        db  083h, 03Dh, 02Ch, 039h, 000h, 000h, 000h
        je     SHORT loc_1_1735
        db  08Bh, 03Dh, 02Ch, 039h, 000h, 000h
        jmp    SHORT loc_1_173B
loc_1_1735:
        db  08Bh, 03Dh, 078h, 039h, 000h, 000h
loc_1_173B:
        push   1
        push   esi
        push   ebx
        lea    eax, [ebx + 3eh]
        push   eax
        mov    ax, word ptr [ebx + 3ch]
        push   eax
        push   0
        call   sub_1_3233
        add    esp, 18h
loc_1_1752:
        dec    dword ptr [ebx + 1ch]
        jmp    NEAR PTR loc_1_15AC
loc_1_175A:
        push   0
        push   0ffh
        push   ebx
        call   sub_1_0FBB
        add    esp, 0ch
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        jne    SHORT loc_1_1795
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        je     NEAR PTR loc_1_1841
        lea    esi, [ebx + 54h]
        db  0BFh, 044h, 039h, 000h, 000h
        mov    ecx, 3
        repe cmpsd
        jne    NEAR PTR loc_1_1841
loc_1_1795:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DBh, 0FFh, 0FFh
        call   sub_1_0F51
        jmp    NEAR PTR loc_1_1841
loc_1_17A9:
        mov    byte ptr [ebx + 0ah], 2
        mov    dword ptr [ebx + 1ch], 3
loc_1_17B4:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        je     SHORT loc_1_17FC
        cmp    dword ptr [ebx + 1ch], 0
        jbe    SHORT loc_1_17E4
        lea    eax, [ebx + 3eh]
        push   eax
        push   5
        push   1
        push   dword ptr [ebx + 20h]
        call   sub_1_1CFD
        add    esp, 10h
        db  08Bh, 03Dh, 02Ch, 039h, 000h, 000h
        dec    dword ptr [ebx + 1ch]
        jmp    NEAR PTR loc_1_15AC
loc_1_17E4:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0FBh, 0FFh, 0FFh
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 020h
        jne    SHORT loc_1_17FC
        call   sub_1_0F51
loc_1_17FC:
        mov    byte ptr [ebx + 0ah], 3
        mov    dword ptr [ebx + 1ch], 3
loc_1_1807:
        cmp    dword ptr [ebx + 1ch], 0
        jbe    SHORT loc_1_1834
        lea    eax, [ebx + 3eh]
        push   eax
        push   0dh
        push   3
        push   dword ptr [ebx + 20h]
        call   sub_1_1CFD
        add    esp, 10h
        db  08Bh, 03Dh, 078h, 039h, 000h, 000h
        add    edi, 1f4h
        dec    dword ptr [ebx + 1ch]
        jmp    NEAR PTR loc_1_15AC
loc_1_1834:
        push   0
        push   3
loc_1_1838:
        push   ebx
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_1841:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_1848:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 0ah]
        or     eax, eax
        je     SHORT loc_1_18A0
        cmp    eax, 1
        je     NEAR PTR loc_1_1933
        cmp    eax, 2
        je     NEAR PTR loc_1_1968
        cmp    eax, 4
        je     SHORT loc_1_18BB
        cmp    eax, 5
        je     NEAR PTR loc_1_1910
        mov    edi, dword ptr [esp + 0ch]
loc_1_187D:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   edi
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   dword ptr [esi + 20h]
        call   sub_1_013F
        add    esp, 4
loc_1_189A:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_18A0:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 022h, 020h, 000h, 000h
        jne    NEAR PTR loc_1_1933
        mov    byte ptr [esi + 0ah], 1
        call   sub_1_0F51
        jmp    SHORT loc_1_189A
loc_1_18BB:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esi + 4eh]
        push   eax
        lea    eax, [esi + 54h]
        push   eax
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_18E3
        push   0
        push   3
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_189A
loc_1_18E3:
        mov    eax, dword ptr [esp + 8]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
        mov    byte ptr [esi + 0ah], 5
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    dword ptr [esi + 1ch], 3
loc_1_1910:
        cmp    dword ptr [esi + 1ch], 0
        jbe    NEAR PTR loc_1_19F8
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        push   1
        je     NEAR PTR loc_1_19C6
        push   0
        jmp    NEAR PTR loc_1_19CA
loc_1_1933:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        je     SHORT loc_1_195D
        mov    byte ptr [esi + 0ah], 5
        mov    dword ptr [esi + 1ch], 3
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        jmp    SHORT loc_1_1910
loc_1_195D:
        mov    byte ptr [esi + 0ah], 2
        mov    dword ptr [esi + 1ch], 4
loc_1_1968:
        db  08Bh, 03Dh, 078h, 039h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, 2
        jbe    SHORT loc_1_1993
        push   300h
        push   7b06h
        push   dword ptr [esi + 20h]
        call   sub_1_1E19
        add    esp, 0ch
        dec    dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_1_187D
loc_1_1993:
        or     eax, eax
        je     SHORT loc_1_19B4
        push   100h
        push   7b06h
        push   dword ptr [esi + 20h]
        call   sub_1_1E19
        add    esp, 0ch
        dec    dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_1_187D
loc_1_19B4:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_189A
loc_1_19C6:
        lea    eax, [esi + 4eh]
        push   eax
loc_1_19CA:
        push   esi
        push   0
        push   0
        push   0
        call   sub_1_3233
        add    esp, 18h
        db  083h, 03Dh, 02Ch, 039h, 000h, 000h, 000h
        je     SHORT loc_1_19EA
        db  08Bh, 03Dh, 02Ch, 039h, 000h, 000h
        jmp    SHORT loc_1_19F0
loc_1_19EA:
        db  08Bh, 03Dh, 078h, 039h, 000h, 000h
loc_1_19F0:
        dec    dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_1_187D
loc_1_19F8:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        je     SHORT loc_1_1A1C
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DBh, 0FFh, 0FFh
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 020h
        jne    SHORT loc_1_1A1C
        call   sub_1_0F51
loc_1_1A1C:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_189A
loc_1_1A2E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, byte ptr [esi + 0ah]
        or     eax, eax
        je     SHORT loc_1_1A42
        cmp    eax, 1
        je     SHORT loc_1_1A6C
        jmp    SHORT loc_1_1A90
loc_1_1A42:
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        jne    SHORT loc_1_1A50
        jmp    SHORT loc_1_1AB4
loc_1_1A50:
        mov    byte ptr [esi + 0ah], 1
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    dword ptr [esi + 1ch], 3
loc_1_1A6C:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_1AB4
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        push   esi
        mov    ax, word ptr [esi + 78h]
        push   eax
        mov    ax, word ptr [esi + 3ch]
        push   eax
        lea    eax, [esi + 3eh]
        push   eax
        push   13h
        call   sub_1_3196
        add    esp, 14h
loc_1_1A90:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        db  0FFh, 035h, 02Ch, 039h, 000h, 000h
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   dword ptr [esi + 20h]
        call   sub_1_013F
        add    esp, 4
        jmp    SHORT loc_1_1AC1
loc_1_1AB4:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_1AC1:
        pop    esi
        ret
loc_1_1AC3:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        movzx  eax, byte ptr [esi + 0ah]
        cmp    eax, 5
        ja     SHORT loc_1_1B1D
        db  0FFh, 024h, 085h, 0E5h, 01Ch, 000h, 000h
loc_1_1ADB:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 080h
        jne    SHORT loc_1_1B3E
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 080h, 000h, 000h, 000h
        mov    byte ptr [esi + 0ah], 1
        mov    dword ptr [esi + 1ch], 4
loc_1_1AF9:
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, 2
        jbe    NEAR PTR loc_1_1BB9
        push   300h
loc_1_1B0A:
        push   5006h
        push   dword ptr [esi + 20h]
        call   sub_1_1E19
        add    esp, 0ch
loc_1_1B1A:
        dec    dword ptr [esi + 1ch]
loc_1_1B1D:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   2eeh
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   dword ptr [esi + 20h]
        call   sub_1_013F
loc_1_1B3B:
        add    esp, 4
loc_1_1B3E:
        pop    esi
        add    esp, 4
        ret
loc_1_1B43:
        cmp    dword ptr [esi + 1ch], 0
        jbe    SHORT loc_1_1BA3
        lea    eax, [esp + 4]
        push   eax
        db  068h, 044h, 03Ah, 000h, 000h
        db  068h, 044h, 039h, 000h, 000h
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_1B7F
        dec    dword ptr [esi + 1ch]
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   12ch
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_1_1B3E
loc_1_1B7F:
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 004h, 000h, 000h
        push   0
        call   sub_1_10B6
        jmp    SHORT loc_1_1B3B
loc_1_1BA3:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DFh, 0FFh, 0FFh
        push   3
        call   sub_1_10B6
        add    esp, 4
        jmp    SHORT loc_1_1C35
loc_1_1BB9:
        or     eax, eax
        je     SHORT loc_1_1BC7
        push   100h
        jmp    NEAR PTR loc_1_1B0A
loc_1_1BC7:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 010h
        je     SHORT loc_1_1BDC
        push   6
        call   sub_1_10B6
        jmp    NEAR PTR loc_1_1B3B
loc_1_1BDC:
        mov    byte ptr [esi + 0ah], 2
        mov    dword ptr [esi + 1ch], 2
loc_1_1BE7:
        cmp    dword ptr [esi + 1ch], 0
        jbe    SHORT loc_1_1C03
        push   0
        push   0dh
        push   2
        push   dword ptr [esi + 20h]
        call   sub_1_1CFD
        add    esp, 10h
        jmp    NEAR PTR loc_1_1B1A
loc_1_1C03:
        mov    byte ptr [esi + 0ah], 3
        mov    dword ptr [esi + 1ch], 2
loc_1_1C0E:
        cmp    dword ptr [esi + 1ch], 0
        jbe    SHORT loc_1_1C2A
        push   0
        push   0dh
        push   3
        push   dword ptr [esi + 20h]
        call   sub_1_1CFD
        add    esp, 10h
        jmp    NEAR PTR loc_1_1B1A
loc_1_1C2A:
        mov    byte ptr [esi + 0ah], 5
        mov    dword ptr [esi + 1ch], 1
loc_1_1C35:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     NEAR PTR loc_1_1CC5
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 040h
        jne    SHORT loc_1_1CAA
        db  083h, 03Dh, 0B0h, 039h, 000h, 000h, 000h
        je     SHORT loc_1_1C6E
        db  0FFh, 035h, 0B0h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        db  0C7h, 005h, 0B0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1C6E:
        call   sub_1_01CC
        db  0A3h, 0A0h, 039h, 000h, 000h
        db  0C7h, 005h, 0A8h, 039h, 000h, 000h, 01Dh, 000h, 000h, 000h
        db  0C7h, 005h, 0A4h, 039h, 000h, 000h, 004h, 000h, 000h, 000h
        call   sub_1_01CC
        add    eax, 3e8h
        db  0A3h, 0B4h, 039h, 000h, 000h
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 040h, 000h, 000h
        call   sub_1_1E95
loc_1_1CAA:
        push   esi
        db  068h, 087h, 004h, 000h, 000h
        push   4e20h
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_1_1B3E
loc_1_1CC5:
        push   3
        call   sub_1_10B6
        add    esp, 4
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 020h
        db  0C7h, 005h, 03Ch, 039h, 000h, 000h, 01Eh, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1B3E
        db  0DBh, 01Ah, 000h, 000h, 0F9h, 01Ah, 000h, 000h, 0E7h, 01Bh, 000h, 000h, 00Eh, 01Ch, 000h, 000h
        db  043h, 01Bh, 000h, 000h, 035h, 01Ch, 000h, 000h
sub_1_1CFD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 4], 0
        db  066h, 0A1h, 098h, 039h, 000h, 000h
        mov    word ptr [ecx + 0ah], ax
        mov    ebp, dword ptr [ecx + 24h]
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 1
        je     SHORT loc_1_1D4E
        cmp    eax, 2
        jb     SHORT loc_1_1D36
        cmp    eax, 3
        jbe    SHORT loc_1_1D66
        cmp    eax, 4
        je     SHORT loc_1_1D84
        cmp    eax, 5
        je     SHORT loc_1_1D9C
loc_1_1D36:
        mov    ebx, dword ptr [esp + 20h]
loc_1_1D3A:
        cmp    eax, 3
        jne    SHORT loc_1_1DB3
        lea    edx, [ebp + 3eh]
        mov    byte ptr [ebp + 5], 14h
        mov    word ptr [ecx + 28h], 66h
        jmp    SHORT loc_1_1DC0
loc_1_1D4E:
        db  0BEh, 044h, 03Ah, 000h, 000h
        lea    edi, [ecx + 1ch]
        movsd
        movsw
        db  0BEh, 044h, 039h, 000h, 000h
        lea    edi, [ebp + 6]
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_1D36
loc_1_1D66:
        db  0BEh, 064h, 039h, 000h, 000h
        lea    edi, [ecx + 1ch]
        movsd
        movsw
        mov    dword ptr [ebp + 6], 0
        db  0BEh, 064h, 039h, 000h, 000h
        lea    edi, [ebp + 0ah]
        movsd
        movsd
        jmp    SHORT loc_1_1D36
loc_1_1D84:
        db  0BEh, 070h, 039h, 000h, 000h
        lea    edi, [ecx + 1ch]
        movsd
        movsw
        db  0BEh, 06Ch, 039h, 000h, 000h
        lea    edi, [ebp + 6]
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_1D36
loc_1_1D9C:
        lea    edi, [ecx + 1ch]
        mov    ebx, dword ptr [esp + 20h]
        mov    esi, ebx
        movsd
        movsw
        lea    esi, [ebx + 6]
        lea    edi, [ebp + 6]
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_1D3A
loc_1_1DB3:
        lea    edx, [ebp + 1eh]
        mov    byte ptr [ebp + 5], 4
        mov    word ptr [ecx + 28h], 46h
loc_1_1DC0:
        mov    dword ptr [edx], 53575242h
        mov    dword ptr [edx + 8], 10006h
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 14h], eax
        cmp    eax, 0dh
        db  08Bh, 00Dh, 024h, 039h, 000h, 000h
        mov    dword ptr [edx + 0ch], ecx
        je     SHORT loc_1_1DE7
        cmp    eax, 5
        jne    SHORT loc_1_1DEF
loc_1_1DE7:
        or     ebx, ebx
        je     SHORT loc_1_1DEF
        mov    edi, ebx
        jmp    SHORT loc_1_1DF4
loc_1_1DEF:
        db  0BFh, 0A8h, 006h, 000h, 000h
loc_1_1DF4:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1E19:
        push   edi
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], 0
        db  066h, 0A1h, 098h, 039h, 000h, 000h
        lea    edi, [ecx + 1ch]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    word ptr [ecx + 28h], 22h
        mov    ecx, dword ptr [ecx + 24h]
        mov    byte ptr [ecx + 5], 4
        lea    edi, [ecx + 0ah]
        mov    dword ptr [ecx + 6], 0
        stosd
        stosw
        mov    word ptr [ecx + 10h], 5204h
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ecx + 1eh], ax
        mov    ax, word ptr [esp + 0ch]
        mov    word ptr [ecx + 20h], ax
        pop    edi
        ret
loc_1_1E6E:
        db  0C7h, 005h, 040h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 020h, 039h, 000h, 000h
        and    eax, 0fffffffbh
        db  0A3h, 020h, 039h, 000h, 000h
        push   0
        db  068h, 0E7h, 02Ah, 000h, 000h
        call   sub_1_009C
        add    esp, 8
        ret
sub_1_1E95:
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 0B0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 0A4h, 039h, 000h, 000h, 000h
        jbe    NEAR PTR loc_1_1F33
        call   sub_1_01CC
        mov    edi, eax
        call   sub_1_045B
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1EC8
        mov    eax, 1f4h
        jmp    SHORT loc_1_1F1C
loc_1_1EC8:
        mov    ebx, dword ptr [esi + 24h]
        push   0
        push   1
        push   3
        push   esi
        call   sub_1_1CFD
        add    esp, 10h
        db  0C7h, 046h, 004h, 06Fh, 004h, 000h, 000h
        mov    word ptr [esi + 0ah], 5505h
        db  0A1h, 0A0h, 039h, 000h, 000h
        mov    dword ptr [ebx + 4eh], eax
        db  0FFh, 00Dh, 0A4h, 039h, 000h, 000h
        push   esi
        call   sub_1_013F
        add    esp, 4
        db  081h, 005h, 0B4h, 039h, 000h, 000h, 0E8h, 003h, 000h, 000h
        db  039h, 03Dh, 0B4h, 039h, 000h, 000h
        mov    eax, 1
        jbe    SHORT loc_1_1F1C
        db  0A1h, 0B4h, 039h, 000h, 000h
        sub    eax, edi
loc_1_1F1C:
        push   0
        db  068h, 095h, 01Eh, 000h, 000h
        push   eax
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 0B0h, 039h, 000h, 000h
        jmp    SHORT loc_1_1F7C
loc_1_1F33:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0BFh, 0FFh, 0FFh
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 001h
        je     SHORT loc_1_1F57
        push   0
        db  068h, 0C7h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        jmp    SHORT loc_1_1F7C
loc_1_1F57:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DFh, 0FFh, 0FFh
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 020h
        db  0C7h, 005h, 03Ch, 039h, 000h, 000h, 01Eh, 000h, 000h, 000h
        push   3
        call   sub_1_10B6
        add    esp, 4
loc_1_1F7C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1F80:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx + 14h], 15h
        je     SHORT loc_1_1FC6
        lea    eax, [ebx + 18h]
        db  0B9h, 0A8h, 006h, 000h, 000h
loc_1_1F99:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_1FB9
        or     dl, dl
        je     SHORT loc_1_1FB5
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_1FB9
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_1F99
loc_1_1FB5:
        sub    eax, eax
        jmp    SHORT loc_1_1FBE
loc_1_1FB9:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1FBE:
        or     eax, eax
        jne    NEAR PTR loc_1_264C
loc_1_1FC6:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1
        je     SHORT loc_1_2025
        cmp    eax, 2
        je     NEAR PTR loc_1_22A7
        cmp    eax, 12h
        je     NEAR PTR loc_1_221F
        cmp    eax, 15h
        je     NEAR PTR loc_1_2272
loc_1_1FE9:
        db  08Bh, 02Dh, 080h, 039h, 000h, 000h
        db  081h, 0FDh, 080h, 039h, 000h, 000h
        je     NEAR PTR loc_1_264C
loc_1_1FFB:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 10h
        je     NEAR PTR loc_1_245A
        cmp    eax, 11h
        je     NEAR PTR loc_1_24FD
        cmp    eax, 15h
        je     NEAR PTR loc_1_2558
        jmp    NEAR PTR loc_1_263C
loc_1_2025:
        mov    esi, dword ptr [esp + 18h]
        db  0BFh, 06Ch, 039h, 000h, 000h
        lea    esi, [esi + 12h]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_2040
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_2040:
        je     NEAR PTR loc_1_264C
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 040h
        je     SHORT loc_1_2065
        cmp    dword ptr [ebx + 0ch], 0
        je     NEAR PTR loc_1_264C
        push   384h
        push   0c8h
        jmp    SHORT loc_1_2094
loc_1_2065:
        call   sub_1_01CC
        db  0A3h, 0A0h, 039h, 000h, 000h
        db  0C7h, 005h, 0A8h, 039h, 000h, 000h, 01Eh, 000h, 000h, 000h
        db  081h, 03Dh, 024h, 039h, 000h, 000h, 000h, 000h, 005h, 000h
        mov    esi, 64h
        jae    SHORT loc_1_209E
        push   0bb8h
        push   320h
loc_1_2094:
        call   sub_1_06C0
        add    esp, 8
        mov    esi, eax
loc_1_209E:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 001h
        jne    SHORT loc_1_20AE
        sub    ecx, ecx
        jmp    NEAR PTR loc_1_2143
loc_1_20AE:
        cmp    dword ptr [ebx + 0ch], 0
        jne    SHORT loc_1_20BE
        mov    ecx, 1
        jmp    NEAR PTR loc_1_2143
loc_1_20BE:
        db  0FFh, 00Dh, 0A8h, 039h, 000h, 000h
        jne    SHORT loc_1_20CA
        sub    ecx, ecx
        jmp    SHORT loc_1_2143
loc_1_20CA:
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, 10006h
        je     SHORT loc_1_20E3
        cmp    eax, 10006h
        mov    ecx, 0
        setb   cl
        jmp    SHORT loc_1_2143
loc_1_20E3:
        mov    eax, dword ptr [ebx + 0ch]
        db  039h, 005h, 024h, 039h, 000h, 000h
        je     SHORT loc_1_20F6
        db  039h, 005h, 024h, 039h, 000h, 000h
        jmp    SHORT loc_1_213B
loc_1_20F6:
        mov    eax, dword ptr [ebx + 10h]
        db  03Bh, 005h, 0A0h, 039h, 000h, 000h
        je     SHORT loc_1_2111
        mov    ecx, 0
        db  03Bh, 005h, 0A0h, 039h, 000h, 000h
        setb   cl
        jmp    SHORT loc_1_2143
loc_1_2111:
        sub    ecx, ecx
        sub    edi, edi
loc_1_2115:
        db  08Ah, 097h, 070h, 039h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [edi + eax + 16h], dl
        jne    SHORT loc_1_212D
        inc    edi
        cmp    edi, 6
        jb     SHORT loc_1_2115
        jmp    SHORT loc_1_2143
loc_1_212D:
        db  08Ah, 08Fh, 070h, 039h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [edi + eax + 16h], cl
loc_1_213B:
        mov    ecx, 0
        seta   cl
loc_1_2143:
        or     ecx, ecx
        jne    SHORT loc_1_2185
        call   sub_1_01CC
        db  0A3h, 0ACh, 039h, 000h, 000h
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0BFh, 0FFh, 0FFh
        db  083h, 03Dh, 0B0h, 039h, 000h, 000h, 000h
        je     NEAR PTR loc_1_264C
        db  0FFh, 035h, 0B0h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        db  0C7h, 005h, 0B0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_264C
loc_1_2185:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 040h
        jne    SHORT loc_1_21C1
        db  083h, 03Dh, 0ACh, 039h, 000h, 000h, 000h
        je     SHORT loc_1_21AD
        db  0A1h, 0A0h, 039h, 000h, 000h
        db  02Bh, 005h, 0ACh, 039h, 000h, 000h
        cmp    eax, 36b0h
        jb     NEAR PTR loc_1_264C
loc_1_21AD:
        db  0C7h, 005h, 0B4h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 040h, 000h, 000h
loc_1_21C1:
        db  0C7h, 005h, 0A4h, 039h, 000h, 000h, 004h, 000h, 000h, 000h
        call   sub_1_01CC
        lea    edi, [esi + eax]
        db  083h, 03Dh, 0B4h, 039h, 000h, 000h, 000h
        je     SHORT loc_1_21E8
        db  03Bh, 03Dh, 0B4h, 039h, 000h, 000h
        jae    NEAR PTR loc_1_264C
loc_1_21E8:
        db  083h, 03Dh, 0B0h, 039h, 000h, 000h, 000h
        je     SHORT loc_1_21FF
        db  0FFh, 035h, 0B0h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
loc_1_21FF:
        db  089h, 03Dh, 0B4h, 039h, 000h, 000h
        push   0
        db  068h, 095h, 01Eh, 000h, 000h
        push   esi
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 0B0h, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_264C
loc_1_221F:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        je     NEAR PTR loc_1_1FE9
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0FBh, 0FFh, 0FFh
        push   1f40h
        push   1f4h
        call   sub_1_06C0
        add    esp, 8
        mov    esi, eax
        db  0FFh, 035h, 034h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        push   0
        db  068h, 088h, 00Fh, 000h, 000h
        push   esi
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 034h, 039h, 000h, 000h
        jmp    NEAR PTR loc_1_1FE9
loc_1_2272:
        lea    eax, [ebx + 18h]
        db  0B9h, 0A8h, 006h, 000h, 000h
loc_1_227A:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_229A
        or     dl, dl
        je     SHORT loc_1_2296
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_229A
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_227A
loc_1_2296:
        sub    eax, eax
        jmp    SHORT loc_1_229F
loc_1_229A:
        sbb    eax, eax
        sbb    eax, -1
loc_1_229F:
        or     eax, eax
        jne    NEAR PTR loc_1_1FE9
loc_1_22A7:
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, 10003h
        jb     NEAR PTR loc_1_264C
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        je     SHORT loc_1_22D3
        cmp    eax, 10006h
        jb     NEAR PTR loc_1_264C
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0DBh, 0FFh, 0FFh
loc_1_22D3:
        db  0C7h, 005h, 028h, 039h, 000h, 000h, 006h, 000h, 000h, 000h
        db  081h, 025h, 020h, 039h, 000h, 000h, 0DFh, 0BFh, 0FFh, 0FFh
        db  0C7h, 005h, 03Ch, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 0B0h, 039h, 000h, 000h, 000h
        je     SHORT loc_1_2312
        db  0FFh, 035h, 0B0h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        db  0C7h, 005h, 0B0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2312:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 002h
        je     SHORT loc_1_2334
        mov    edi, dword ptr [esp + 18h]
        db  0BEh, 044h, 039h, 000h, 000h
        lea    edi, [edi + 12h]
        mov    ecx, 3
        repe cmpsd
        je     NEAR PTR loc_1_23E1
loc_1_2334:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        jne    NEAR PTR loc_1_23E1
        mov    esi, dword ptr [esp + 18h]
        add    esi, 12h
        db  0BFh, 044h, 039h, 000h, 000h
        movsd
        movsd
        movsd
        db  0BFh, 044h, 03Ah, 000h, 000h
        mov    esi, dword ptr [esp + 24h]
        movsd
        movsw
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 002h
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 0FBh, 0FFh, 0FFh
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 080h
        jne    SHORT loc_1_2393
        db  083h, 03Dh, 038h, 039h, 000h, 000h, 000h
        jne    SHORT loc_1_2393
        push   0
        db  068h, 054h, 026h, 000h, 000h
        call   sub_1_009C
        add    esp, 8
        db  0A3h, 038h, 039h, 000h, 000h
loc_1_2393:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 008h
        je     SHORT loc_1_23E1
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 004h
        je     SHORT loc_1_23B3
        db  0FFh, 035h, 040h, 039h, 000h, 000h
        call   sub_1_008C
        add    esp, 4
loc_1_23B3:
        push   1388h
        push   3e8h
        call   sub_1_06C0
        add    esp, 8
        push   0
        db  068h, 06Eh, 01Eh, 000h, 000h
        push   eax
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 040h, 039h, 000h, 000h
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 004h
loc_1_23E1:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 080h
        je     NEAR PTR loc_1_264C
        db  0A1h, 088h, 039h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_240F
        push   eax
        call   sub_1_008C
        db  0A1h, 088h, 039h, 000h, 000h
        add    esp, 4
        mov    dword ptr [eax + 10h], 0
loc_1_240F:
        db  0A1h, 088h, 039h, 000h, 000h
        mov    byte ptr [eax + 0ah], 4
        db  0A1h, 088h, 039h, 000h, 000h
        mov    dword ptr [eax + 1ch], 3
        db  0A1h, 088h, 039h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     SHORT loc_1_2439
        push   eax
        call   sub_1_00D6
        add    esp, 4
loc_1_2439:
        db  0FFh, 035h, 088h, 039h, 000h, 000h
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        db  08Bh, 00Dh, 088h, 039h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_264C
loc_1_245A:
        mov    al, byte ptr [ebp + 9]
        cmp    al, 7
        jne    SHORT loc_1_24A6
        lea    eax, [ebx + 2ah]
        lea    edx, [ebp + 3eh]
loc_1_2467:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_2487
        or     cl, cl
        je     SHORT loc_1_2483
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_2487
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_2467
loc_1_2483:
        sub    eax, eax
        jmp    SHORT loc_1_248C
loc_1_2487:
        sbb    eax, eax
        sbb    eax, -1
loc_1_248C:
        or     eax, eax
        jne    NEAR PTR loc_1_263C
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_263C
loc_1_24A6:
        cmp    al, 5
        jne    NEAR PTR loc_1_263C
        lea    eax, [ebx + 2ah]
        lea    edx, [ebp + 3eh]
loc_1_24B4:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_24D4
        or     cl, cl
        je     SHORT loc_1_24D0
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_24D4
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_24B4
loc_1_24D0:
        sub    eax, eax
        jmp    SHORT loc_1_24D9
loc_1_24D4:
        sbb    eax, eax
        sbb    eax, -1
loc_1_24D9:
        or     eax, eax
        jne    NEAR PTR loc_1_263C
        lea    edi, [ebp + 4eh]
        sub    eax, eax
        stosd
        stosw
        lea    esi, [ebx + 5ah]
        lea    edi, [ebp + 54h]
        movsd
        movsd
        movsd
        mov    word ptr [ebp + 5eh], 5505h
        jmp    NEAR PTR loc_1_25F2
loc_1_24FD:
        cmp    byte ptr [ebp + 9], 5
        jne    SHORT loc_1_253C
        mov    byte ptr [ebp + 0ah], 3
        mov    dword ptr [ebp + 1ch], 2
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_2525
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 10h], 0
loc_1_2525:
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_253C
        push   ebp
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 14h], eax
loc_1_253C:
        cmp    byte ptr [ebp + 9], 7
        jne    NEAR PTR loc_1_263C
        push   0
        push   3
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_263C
loc_1_2558:
        mov    al, byte ptr [ebp + 9]
        cmp    al, 7
        jne    SHORT loc_1_25A4
        lea    eax, [ebx + 18h]
        lea    edx, [ebp + 3eh]
loc_1_2565:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_2585
        or     cl, cl
        je     SHORT loc_1_2581
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_2585
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_2565
loc_1_2581:
        sub    eax, eax
        jmp    SHORT loc_1_258A
loc_1_2585:
        sbb    eax, eax
        sbb    eax, -1
loc_1_258A:
        or     eax, eax
        jne    NEAR PTR loc_1_263C
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_263C
loc_1_25A4:
        cmp    al, 5
        jne    NEAR PTR loc_1_263C
        lea    eax, [ebx + 18h]
        lea    edx, [ebp + 3eh]
loc_1_25B2:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_25D2
        or     cl, cl
        je     SHORT loc_1_25CE
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_25D2
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_25B2
loc_1_25CE:
        sub    eax, eax
        jmp    SHORT loc_1_25D7
loc_1_25D2:
        sbb    eax, eax
        sbb    eax, -1
loc_1_25D7:
        or     eax, eax
        jne    SHORT loc_1_263C
        lea    edi, [ebp + 4eh]
        mov    esi, dword ptr [esp + 24h]
        movsd
        movsw
        mov    esi, dword ptr [esp + 18h]
        add    esi, 12h
        lea    edi, [ebp + 54h]
        movsd
        movsd
        movsd
loc_1_25F2:
        mov    byte ptr [ebp + 0ah], 5
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [ebp + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    dword ptr [ebp + 1ch], 3
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_2625
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 10h], 0
loc_1_2625:
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_263C
        push   ebp
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 14h], eax
loc_1_263C:
        mov    ebp, dword ptr [esp + 10h]
        db  081h, 0FDh, 080h, 039h, 000h, 000h
        jne    NEAR PTR loc_1_1FFB
loc_1_264C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_2654:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        db  068h, 044h, 03Ah, 000h, 000h
        db  068h, 044h, 039h, 000h, 000h
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2692
        mov    eax, dword ptr [esp]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
        db  0A1h, 020h, 039h, 000h, 000h
        or     eax, 400h
        db  0A3h, 020h, 039h, 000h, 000h
loc_1_2692:
        db  0C7h, 005h, 038h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        ret
sub_1_26A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 080h
        je     NEAR PTR loc_1_274D
        db  0A1h, 088h, 039h, 000h, 000h
        mov    ebx, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 0ah], 1
        jne    NEAR PTR loc_1_2751
        cmp    word ptr [ebx], 5006h
        jne    NEAR PTR loc_1_2751
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 020h, 000h, 000h
        lea    esi, [ebx + 32h]
        db  0BFh, 044h, 039h, 000h, 000h
        movsd
        movsd
        movsd
        db  0A1h, 088h, 039h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_2705
        push   eax
        call   sub_1_008C
        db  0A1h, 088h, 039h, 000h, 000h
        add    esp, 4
        mov    dword ptr [eax + 10h], 0
loc_1_2705:
        db  0A1h, 088h, 039h, 000h, 000h
        mov    byte ptr [eax + 0ah], 4
        db  0A1h, 088h, 039h, 000h, 000h
        mov    dword ptr [eax + 1ch], 3
        db  0A1h, 088h, 039h, 000h, 000h
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     SHORT loc_1_272F
        push   eax
        call   sub_1_00D6
        add    esp, 4
loc_1_272F:
        db  0FFh, 035h, 088h, 039h, 000h, 000h
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        db  08Bh, 00Dh, 088h, 039h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ecx + 14h], eax
        jmp    SHORT loc_1_27BF
loc_1_274D:
        mov    ebx, dword ptr [esp + 14h]
loc_1_2751:
        cmp    word ptr [ebx], 7b06h
        jne    SHORT loc_1_27BF
        db  08Bh, 02Dh, 080h, 039h, 000h, 000h
        db  081h, 0FDh, 080h, 039h, 000h, 000h
        je     SHORT loc_1_27BF
loc_1_2766:
        cmp    byte ptr [ebp + 9], 6
        jne    SHORT loc_1_27B4
        lea    esi, [ebx + 32h]
        lea    edi, [ebp + 54h]
        movsd
        movsd
        movsd
        mov    word ptr [ebp + 5eh], 5505h
        mov    byte ptr [ebp + 0ah], 4
        mov    dword ptr [ebp + 1ch], 3
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_279D
        push   eax
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 10h], 0
loc_1_279D:
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_27B4
        push   ebp
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 14h], eax
loc_1_27B4:
        mov    ebp, dword ptr [ebp]
        db  081h, 0FDh, 080h, 039h, 000h, 000h
        jne    SHORT loc_1_2766
loc_1_27BF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 090h, 039h, 000h, 000h
        db  03Dh, 090h, 039h, 000h, 000h
        je     SHORT loc_1_2825
        mov    edx, dword ptr [esp + 10h]
        mov    si, word ptr [esp + 14h]
loc_1_27E8:
        cmp    word ptr [eax + 0ch], si
        jne    SHORT loc_1_281C
        mov    ecx, edx
        lea    edi, [eax + 14h]
loc_1_27F3:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_1_2813
        or     bl, bl
        je     SHORT loc_1_280F
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_1_2813
        add    ecx, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_1_27F3
loc_1_280F:
        sub    ecx, ecx
        jmp    SHORT loc_1_2818
loc_1_2813:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_1_2818:
        or     ecx, ecx
        je     SHORT loc_1_2827
loc_1_281C:
        mov    eax, dword ptr [eax]
        db  03Dh, 090h, 039h, 000h, 000h
        jne    SHORT loc_1_27E8
loc_1_2825:
        sub    eax, eax
loc_1_2827:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_282B:
        push   esi
        push   edi
        mov    ax, word ptr [esp + 10h]
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        mov    word ptr [edx + 0ch], ax
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [edx + 0eh], ax
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [edx + 10h], ecx
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx + 8], 1
        db  0A1h, 090h, 039h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0C7h, 042h, 004h, 090h, 039h, 000h, 000h
        db  0A1h, 090h, 039h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  089h, 015h, 090h, 039h, 000h, 000h
        db  0FFh, 005h, 08Ch, 039h, 000h, 000h
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 008h
        jne    SHORT loc_1_28D6
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 008h
        db  0A1h, 0E8h, 039h, 000h, 000h
        db  0A3h, 0E4h, 039h, 000h, 000h
        push   0
        db  068h, 028h, 029h, 000h, 000h
        push   0ea60h
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 03Ch, 03Ah, 000h, 000h
loc_1_28D6:
        db  0A1h, 0E8h, 039h, 000h, 000h
        db  039h, 005h, 0E4h, 039h, 000h, 000h
        jbe    SHORT loc_1_28E8
        db  0A3h, 0E4h, 039h, 000h, 000h
loc_1_28E8:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_28ED:
        mov    edx, dword ptr [esp + 4]
        db  03Bh, 015h, 0E0h, 039h, 000h, 000h
        jne    SHORT loc_1_2900
        mov    eax, dword ptr [edx]
        db  0A3h, 0E0h, 039h, 000h, 000h
loc_1_2900:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        push   edx
        call   sub_1_00FA
        add    esp, 4
        db  0FFh, 00Dh, 08Ch, 039h, 000h, 000h
        jne    SHORT loc_1_2927
        db  083h, 025h, 020h, 039h, 000h, 000h, 0F7h
loc_1_2927:
        ret
loc_1_2928:
        db  0C7h, 005h, 03Ch, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 03Ch, 039h, 000h, 000h
        dec    eax
        db  0A3h, 03Ch, 039h, 000h, 000h
        db  08Bh, 015h, 020h, 039h, 000h, 000h
        jne    SHORT loc_1_2948
        and    edx, 0ffffffdfh
loc_1_2948:
        db  089h, 015h, 020h, 039h, 000h, 000h
        test   dl, 8
        je     NEAR PTR loc_1_29F7
        db  08Bh, 00Dh, 028h, 039h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_2962
        dec    ecx
loc_1_2962:
        db  0A1h, 0E4h, 039h, 000h, 000h
        dec    eax
        db  0A3h, 0E4h, 039h, 000h, 000h
        jne    SHORT loc_1_29B3
        db  089h, 00Dh, 028h, 039h, 000h, 000h
        db  089h, 015h, 020h, 039h, 000h, 000h
        db  0A1h, 0E8h, 039h, 000h, 000h
        db  0A3h, 0E4h, 039h, 000h, 000h
        db  083h, 03Dh, 0E0h, 039h, 000h, 000h, 000h
        jne    SHORT loc_1_29B3
        db  0A1h, 090h, 039h, 000h, 000h
        db  0A3h, 0E0h, 039h, 000h, 000h
        push   0
        db  068h, 0CCh, 02Bh, 000h, 000h
        call   sub_1_009C
        add    esp, 8
        db  08Bh, 00Dh, 028h, 039h, 000h, 000h
        db  08Bh, 015h, 020h, 039h, 000h, 000h
loc_1_29B3:
        db  089h, 00Dh, 028h, 039h, 000h, 000h
        db  089h, 015h, 020h, 039h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_29DE
        test   dl, 2
        je     SHORT loc_1_29DE
        test   dl, 40h
        jne    SHORT loc_1_29DE
        and    edx, 0fffffbfdh
        db  089h, 015h, 020h, 039h, 000h, 000h
        call   sub_1_0F51
loc_1_29DE:
        push   0
        db  068h, 028h, 029h, 000h, 000h
        push   0ea60h
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 03Ch, 03Ah, 000h, 000h
loc_1_29F7:
        ret
        db  0C3h
loc_1_29F9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 9], 0
        je     SHORT loc_1_2A20
        mov    ecx, dword ptr [esi + 10h]
        push   ecx
        mov    ax, word ptr [ecx + 0eh]
        push   eax
        mov    ax, word ptr [ecx + 0ch]
        add    ecx, 14h
        push   eax
        push   ecx
        call   sub_1_282B
        add    esp, 10h
        jmp    SHORT loc_1_2A2B
loc_1_2A20:
        push   dword ptr [esi + 10h]
        call   sub_1_00FA
        add    esp, 4
loc_1_2A2B:
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 004h
        je     SHORT loc_1_2A81
        db  08Bh, 015h, 0ECh, 039h, 000h, 000h
        db  081h, 0FAh, 0ECh, 039h, 000h, 000h
        je     SHORT loc_1_2A81
loc_1_2A42:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        db  0A1h, 090h, 039h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0C7h, 042h, 004h, 090h, 039h, 000h, 000h
        db  0A1h, 090h, 039h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  089h, 015h, 090h, 039h, 000h, 000h
        db  0FFh, 005h, 08Ch, 039h, 000h, 000h
        db  08Bh, 015h, 0ECh, 039h, 000h, 000h
        db  081h, 0FAh, 0ECh, 039h, 000h, 000h
        jne    SHORT loc_1_2A42
loc_1_2A81:
        db  0A1h, 0ECh, 039h, 000h, 000h
        db  03Dh, 0ECh, 039h, 000h, 000h
        je     SHORT loc_1_2A99
        push   esi
        push   eax
        call   sub_1_2AAE
        add    esp, 8
        jmp    SHORT loc_1_2AAC
loc_1_2A99:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 07Fh, 0FFh, 0FFh
        push   esi
        call   sub_1_00FA
        add    esp, 4
loc_1_2AAC:
        pop    esi
        ret
sub_1_2AAE:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    dword ptr [eax + 10h], edx
        add    edx, 14h
        db  0C7h, 040h, 004h, 0F9h, 029h, 000h, 000h
        movzx  eax, word ptr [edx - 6]
        push   eax
        movzx  eax, word ptr [edx - 8]
        push   eax
        push   edx
        call   sub_1_09B1
        add    esp, 10h
        ret
sub_1_2AE7:
        push   esi
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 004h, 080h, 000h, 000h
        jne    NEAR PTR loc_1_2B87
        db  083h, 03Dh, 08Ch, 039h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2B87
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 080h, 000h, 000h
        push   18h
        call   sub_1_00E6
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2B29
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FFh, 07Fh, 0FFh, 0FFh
        jmp    SHORT loc_1_2B87
loc_1_2B29:
        db  08Bh, 035h, 090h, 039h, 000h, 000h
        db  081h, 0FEh, 090h, 039h, 000h, 000h
        je     SHORT loc_1_2B78
loc_1_2B37:
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], ecx
        mov    edx, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        db  08Bh, 00Dh, 0ECh, 039h, 000h, 000h
        mov    dword ptr [esi], ecx
        db  0C7h, 046h, 004h, 0ECh, 039h, 000h, 000h
        db  08Bh, 00Dh, 0ECh, 039h, 000h, 000h
        mov    dword ptr [ecx + 4], esi
        db  089h, 035h, 0ECh, 039h, 000h, 000h
        db  0FFh, 00Dh, 08Ch, 039h, 000h, 000h
        db  08Bh, 035h, 090h, 039h, 000h, 000h
        db  081h, 0FEh, 090h, 039h, 000h, 000h
        jne    SHORT loc_1_2B37
loc_1_2B78:
        push   eax
        db  0FFh, 035h, 0ECh, 039h, 000h, 000h
        call   sub_1_2AAE
        add    esp, 8
loc_1_2B87:
        pop    esi
        ret
loc_1_2B89:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0bh], 0
        je     SHORT loc_1_2BAC
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        je     SHORT loc_1_2BA7
        call   sub_1_2AE7
        jmp    SHORT loc_1_2BAC
loc_1_2BA7:
        call   sub_1_0F51
loc_1_2BAC:
        push   dword ptr [esi + 20h]
        call   sub_1_046F
        add    esp, 4
        push   esi
        call   sub_1_00FA
        add    esp, 4
        db  0C7h, 005h, 0E0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
loc_1_2BCC:
        sub    esp, 4
        push   esi
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 004h
        jne    SHORT loc_1_2C0F
        lea    eax, [esp + 4]
        push   eax
        db  068h, 044h, 03Ah, 000h, 000h
        db  068h, 044h, 039h, 000h, 000h
        call   sub_1_01A4
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2C40
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 37h
        add    eax, 1f4h
        db  0A3h, 02Ch, 039h, 000h, 000h
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 004h, 000h, 000h
loc_1_2C0F:
        call   sub_1_045B
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_2C5B
        push   3ch
        call   sub_1_00E6
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2C5B
        db  0C7h, 040h, 024h, 089h, 02Bh, 000h, 000h
        push   8
        push   dword ptr [esi + 24h]
        push   esi
        push   eax
        call   sub_1_0817
        add    esp, 10h
        jmp    SHORT loc_1_2C99
loc_1_2C40:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DFh, 0FFh, 0FFh
        db  0C7h, 005h, 0E0h, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_0F51
        jmp    SHORT loc_1_2C99
loc_1_2C5B:
        db  0FFh, 035h, 03Ch, 03Ah, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        push   0
        db  068h, 028h, 029h, 000h, 000h
        push   1f4h
        call   sub_1_0074
        add    esp, 0ch
        db  0A3h, 03Ch, 03Ah, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_2C8F
        push   esi
        call   sub_1_046F
        add    esp, 4
loc_1_2C8F:
        db  0C7h, 005h, 0E4h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_2C99:
        pop    esi
        add    esp, 4
        ret
loc_1_2C9E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esp + 10h], eax
        movzx  eax, byte ptr [ebx + 0ah]
        or     eax, eax
        je     SHORT loc_1_2CCB
        cmp    eax, 1
        je     SHORT loc_1_2CF2
        cmp    eax, 2
        je     NEAR PTR loc_1_2E06
        jmp    NEAR PTR loc_1_2DDB
loc_1_2CCB:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], 0
        db  066h, 08Bh, 00Dh, 098h, 039h, 000h, 000h
        mov    word ptr [eax + 0ah], cx
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [ebx + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
loc_1_2CF2:
        mov    eax, dword ptr [esp + 10h]
        db  0BEh, 044h, 03Ah, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        mov    edi, dword ptr [esp + 10h]
        lea    ebp, [eax + 1eh]
        add    edi, 1ch
        movsd
        movsw
        db  0BEh, 044h, 039h, 000h, 000h
        lea    edi, [eax + 6]
        movsd
        movsd
        movsd
        mov    byte ptr [eax + 5], 4
        mov    al, byte ptr [ebx + 9]
        sub    al, 8
        cmp    al, 1
        sbb    al, al
        add    al, 13h
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], 0
        mov    ax, word ptr [ebx + 2ch]
        mov    word ptr [ebp + 2], ax
        mov    esi, 28h
        mov    word ptr [ebp + 4], 100h
        db  066h, 0A1h, 034h, 03Ah, 000h, 000h
        mov    word ptr [ebp + 6], ax
        db  08Ah, 025h, 038h, 03Ah, 000h, 000h
        add    ebp, 0ah
        db  0A0h, 039h, 03Ah, 000h, 000h
        mov    word ptr [ebp - 2], ax
        db  00Fh, 0B7h, 03Dh, 038h, 03Ah, 000h, 000h
        sub    edi, 28h
        db  081h, 03Dh, 0E0h, 039h, 000h, 000h, 090h, 039h, 000h, 000h
        je     SHORT loc_1_2DC8
loc_1_2D71:
        db  0A1h, 0E0h, 039h, 000h, 000h
        mov    ecx, dword ptr [eax + 10h]
        db  0A1h, 0B8h, 006h, 000h, 000h
        lea    eax, [ecx + eax + 1ah]
        cmp    eax, edi
        ja     SHORT loc_1_2DC8
        db  0A1h, 0E0h, 039h, 000h, 000h
        movzx  eax, word ptr [eax + 0eh]
        push   eax
        db  0A1h, 0E0h, 039h, 000h, 000h
        db  068h, 0A8h, 006h, 000h, 000h
        movzx  eax, word ptr [eax + 0ch]
        push   eax
        db  0A1h, 0E0h, 039h, 000h, 000h
        add    eax, 14h
        push   eax
        push   ebp
        call   sub_1_30C0
        add    esi, eax
        add    ebp, eax
        db  0A1h, 0E0h, 039h, 000h, 000h
        add    esp, 14h
        mov    eax, dword ptr [eax]
        db  0A3h, 0E0h, 039h, 000h, 000h
        db  03Dh, 090h, 039h, 000h, 000h
        jne    SHORT loc_1_2D71
loc_1_2DC8:
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 28h], si
        mov    byte ptr [ebx + 0ah], 2
        mov    dword ptr [ebx + 1ch], 2
loc_1_2DDB:
        push   ebx
        db  068h, 087h, 004h, 000h, 000h
        db  0FFh, 035h, 02Ch, 039h, 000h, 000h
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [ebx + 10h], eax
        push   dword ptr [esp + 10h]
        call   sub_1_013F
        add    esp, 4
loc_1_2DFE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_2E06:
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        je     SHORT loc_1_2E13
        dec    eax
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_2DDB
loc_1_2E13:
        db  066h, 081h, 03Dh, 038h, 03Ah, 000h, 000h, 040h, 002h
        jbe    SHORT loc_1_2E2C
        db  066h, 0C7h, 005h, 038h, 03Ah, 000h, 000h, 040h, 002h
        jmp    NEAR PTR loc_1_2CF2
loc_1_2E2C:
        db  081h, 025h, 020h, 039h, 000h, 000h, 0FDh, 0DBh, 0FFh, 0FFh
        push   0
        push   -1
        push   ebx
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_2DFE
sub_1_2E45:
        push   esi
        push   edi
        db  0A1h, 080h, 039h, 000h, 000h
        db  03Dh, 080h, 039h, 000h, 000h
        je     SHORT loc_1_2E75
        mov    esi, dword ptr [esp + 0ch]
loc_1_2E57:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_2E6B
        push   1
        push   0
        push   eax
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_2E6B:
        mov    eax, edi
        db  081h, 0FFh, 080h, 039h, 000h, 000h
        jne    SHORT loc_1_2E57
loc_1_2E75:
        pop    edi
        pop    esi
        ret
sub_1_2E78:
        push   esi
        push   edi
        push   9a2h
        call   sub_1_00E6
        add    esp, 4
        db  0A3h, 040h, 03Ah, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_2E97
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_2ED9
loc_1_2E97:
        db  08Bh, 015h, 040h, 03Ah, 000h, 000h
        mov    ecx, 2
        mov    esi, 1
        mov    edi, 440h
loc_1_2EAC:
        mov    word ptr [edx + 22h], si
        lea    eax, [edx + 2ah]
        mov    dword ptr [edx + 24h], eax
        mov    word ptr [edx + 28h], di
        db  0A1h, 09Ch, 039h, 000h, 000h
        mov    dword ptr [edx], eax
        db  089h, 015h, 09Ch, 039h, 000h, 000h
        mov    edx, dword ptr [edx + 24h]
        add    edx, 440h
        dec    ecx
        jne    SHORT loc_1_2EAC
        db  089h, 015h, 088h, 039h, 000h, 000h
loc_1_2ED9:
        pop    edi
        pop    esi
        ret
sub_1_2EDC:
        push   esi
        push   edi
        db  0FFh, 035h, 03Ch, 03Ah, 000h, 000h
        call   sub_1_008C
        add    esp, 4
        db  0C7h, 005h, 03Ch, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 020h, 039h, 000h, 000h, 008h
        je     SHORT loc_1_2F72
        db  0F7h, 005h, 020h, 039h, 000h, 000h, 002h, 020h, 000h, 000h
        je     SHORT loc_1_2F72
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 002h
        jne    SHORT loc_1_2F72
        call   sub_1_045B
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_2F72
        push   3ch
        call   sub_1_00E6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_2F69
        mov    dword ptr [esi + 24h], 0
        db  0A1h, 090h, 039h, 000h, 000h
        db  0A3h, 0E0h, 039h, 000h, 000h
        push   9
        push   dword ptr [edi + 24h]
        push   edi
        push   esi
        call   sub_1_0817
        add    esp, 10h
        cmp    byte ptr [esi + 8], 0ffh
        jne    SHORT loc_1_2F60
loc_1_2F55:
        call   sub_1_010E
        cmp    byte ptr [esi + 8], 0ffh
        je     SHORT loc_1_2F55
loc_1_2F60:
        push   esi
        call   sub_1_00FA
        add    esp, 4
loc_1_2F69:
        push   edi
        call   sub_1_046F
        add    esp, 4
loc_1_2F72:
        db  0C7h, 005h, 09Ch, 039h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 040h, 03Ah, 000h, 000h, 000h
        je     SHORT loc_1_2F93
        db  0FFh, 035h, 040h, 03Ah, 000h, 000h
        call   sub_1_00FA
        add    esp, 4
loc_1_2F93:
        db  0C7h, 005h, 040h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        ret
sub_1_2FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        lea    eax, [esi + 1]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 28h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_304A
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 20h], eax
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 1ch], ebx
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_3032
        mov    byte ptr [ebx + esi], 0
loc_1_3032:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_304A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3053:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        db  0BFh, 0A8h, 006h, 000h, 000h
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        push   0fh
        db  068h, 0A8h, 006h, 000h, 000h
        db  068h, 0F4h, 039h, 000h, 000h
        db  068h, 000h, 03Ah, 000h, 000h
        push   80000002h
        call   sub_1_2FA0
        add    esp, 14h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_30B4
        db  0BFh, 0A8h, 006h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [esp + 0ch]
        lea    esi, [ecx - 1]
        push   eax
        push   esi
        db  068h, 0A8h, 006h, 000h, 000h
        call   sub_1_01DF
        add    esp, 0ch
        db  089h, 035h, 0B8h, 006h, 000h, 000h
loc_1_30B4:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  000h, 000h, 000h
sub_1_30C0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    ebx, edx
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_1_3100
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        mov    byte ptr [edx + 1], 1
        movzx  eax, byte ptr [edx]
        lea    edi, [edx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        lea    ebx, [edx + eax + 2]
loc_1_3100:
        mov    ax, word ptr [esp + 18h]
        or     ax, ax
        je     SHORT loc_1_311D
        mov    byte ptr [ebx], 2
        mov    byte ptr [ebx + 1], 2
        mov    word ptr [ebx + 2], ax
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_311D:
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        je     SHORT loc_1_3155
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebx], cl
        mov    byte ptr [ebx + 1], 3
        movzx  eax, byte ptr [ebx]
        lea    edi, [ebx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_3155:
        mov    ax, word ptr [esp + 20h]
        or     ax, ax
        je     SHORT loc_1_318E
        mov    byte ptr [ebx], 10h
        mov    byte ptr [ebx + 1], 4
        mov    byte ptr [ebx + 2], 1
        mov    byte ptr [ebx + 3], 10h
        db  0BEh, 06Ch, 039h, 000h, 000h
        mov    byte ptr [ebx + 4], 6
        mov    byte ptr [ebx + 5], 0ch
        lea    edi, [ebx + 6]
        movsd
        movsd
        movsw
        mov    word ptr [ebx + 10h], ax
        movzx  eax, byte ptr [ebx]
        add    eax, 2
        add    ebx, eax
loc_1_318E:
        mov    eax, ebx
        sub    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3196:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 20h]
        db  0BEh, 044h, 03Ah, 000h, 000h
        mov    edi, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [edi + 24h]
        add    edi, 1ch
        movsd
        movsw
        db  0BEh, 044h, 039h, 000h, 000h
        lea    edi, [ecx + 6]
        movsd
        movsd
        movsd
        mov    byte ptr [ecx + 5], 4
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [ecx + 1eh], al
        mov    byte ptr [ecx + 1fh], 0
        mov    word ptr [ecx + 22h], 100h
        mov    ax, word ptr [ebx + 2ch]
        push   dword ptr [esp + 1ch]
        mov    word ptr [ecx + 20h], ax
        db  066h, 0A1h, 034h, 03Ah, 000h, 000h
        mov    word ptr [ecx + 24h], ax
        add    ecx, 28h
        db  08Ah, 025h, 038h, 03Ah, 000h, 000h
        db  0A0h, 039h, 03Ah, 000h, 000h
        db  068h, 0A8h, 006h, 000h, 000h
        mov    word ptr [ecx - 2], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   ecx
        call   sub_1_30C0
        add    esp, 14h
        mov    ecx, dword ptr [ebx + 20h]
        add    ax, 28h
        mov    dword ptr [ecx + 4], 0
        mov    edx, dword ptr [ebx + 20h]
        db  066h, 08Bh, 00Dh, 098h, 039h, 000h, 000h
        mov    word ptr [edx + 0ah], cx
        mov    ecx, dword ptr [ebx + 20h]
        pop    edi
        mov    word ptr [ecx + 28h], ax
        pop    esi
        pop    ebx
        ret
sub_1_3233:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, 1
        mov    edi, dword ptr [ebx + 20h]
        mov    ebp, dword ptr [edi + 24h]
        jne    SHORT loc_1_325A
        lea    edx, [ebp + 1eh]
        mov    dword ptr [esp + 10h], 2eh
        jmp    SHORT loc_1_3265
loc_1_325A:
        lea    edx, [ebp + 3eh]
        mov    dword ptr [esp + 10h], 4eh
loc_1_3265:
        lea    ecx, [edx + 0ah]
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_1_3281
        db  0BEh, 044h, 03Ah, 000h, 000h
        add    edi, 1ch
        movsd
        movsw
        db  0BEh, 044h, 039h, 000h, 000h
        jmp    SHORT loc_1_3292
loc_1_3281:
        add    edi, 1ch
        mov    esi, dword ptr [esp + 28h]
        movsd
        movsw
        mov    esi, dword ptr [esp + 28h]
        add    esi, 6
loc_1_3292:
        lea    edi, [ebp + 6]
        movsd
        movsd
        movsd
        dec    eax
        cmp    eax, 1
        sbb    al, al
        and    al, 0f0h
        add    al, 14h
        mov    byte ptr [ebp + 5], al
        mov    byte ptr [edx], 14h
        mov    byte ptr [edx + 1], 0
        mov    ax, word ptr [ebx + 2ch]
        mov    word ptr [edx + 2], ax
        mov    word ptr [edx + 4], 100h
        db  066h, 0A1h, 034h, 03Ah, 000h, 000h
        mov    word ptr [edx + 6], ax
        mov    esi, dword ptr [ebx + 38h]
        or     esi, esi
        je     SHORT loc_1_32ED
        lea    eax, [esi + 28h]
        db  00Fh, 0B7h, 03Dh, 038h, 03Ah, 000h, 000h
        cmp    edi, eax
        jbe    SHORT loc_1_32ED
        add    si, 28h
        shl    si, 8
        shr    eax, 8
        or     si, ax
        mov    word ptr [edx + 8], si
        jmp    SHORT loc_1_32FC
loc_1_32ED:
        db  08Ah, 025h, 038h, 03Ah, 000h, 000h
        db  0A0h, 039h, 03Ah, 000h, 000h
        mov    word ptr [edx + 8], ax
loc_1_32FC:
        mov    edi, dword ptr [esp + 18h]
        or     edi, edi
        mov    esi, dword ptr [esp + 20h]
        jne    SHORT loc_1_331B
        or     esi, esi
        jne    SHORT loc_1_331B
        mov    word ptr [ecx], 2000h
        mov    byte ptr [edx], 15h
        mov    ax, word ptr [esp + 1ch]
        jmp    SHORT loc_1_3338
loc_1_331B:
        or     edi, edi
        mov    word ptr [ecx], 3000h
        jne    SHORT loc_1_3329
        mov    word ptr [ecx], 8000h
loc_1_3329:
        mov    ax, word ptr [esp + 1ch]
        or     ax, ax
        jne    SHORT loc_1_3338
        or     word ptr [ecx], 4000h
loc_1_3338:
        mov    edx, dword ptr [ebx + 30h]
        mov    dword ptr [ecx + 2], edx
        push   0
        push   esi
        add    ecx, 6
        push   eax
        push   edi
        push   ecx
        call   sub_1_30C0
        add    esp, 14h
        mov    ecx, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 4], 0
        mov    edx, dword ptr [ebx + 20h]
        db  066h, 08Bh, 00Dh, 098h, 039h, 000h, 000h
        mov    word ptr [edx + 0ah], cx
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    eax, dword ptr [ebx + 20h]
        pop    ebp
        mov    word ptr [eax + 28h], cx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_337C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        movzx  eax, byte ptr [ebx + 24h]
        or     eax, eax
        je     SHORT loc_1_33B9
        cmp    eax, 1
        je     NEAR PTR loc_1_3419
        cmp    eax, 7
        je     SHORT loc_1_3419
        cmp    eax, 9
        je     NEAR PTR loc_1_346B
        push   0
        mov    ebp, dword ptr [esp + 20h]
        push   1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3490
loc_1_33B9:
        cmp    word ptr [ebx + 2ch], 0
        jne    SHORT loc_1_33D3
        push   0
        mov    ebp, dword ptr [esp + 20h]
        push   -1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_33D7
loc_1_33D3:
        mov    ebp, dword ptr [esp + 1ch]
loc_1_33D7:
        lea    edx, [ebp + 3ch]
        movzx  eax, byte ptr [ebx + 2eh]
        lea    esi, [ebx + 30h]
        add    ebx, 2eh
        lea    edi, [edx + 3eh]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebx]
        mov    byte ptr [edx + eax + 3eh], 0
        movzx  eax, byte ptr [ebx]
        lea    esi, [ebx + eax + 8]
        lea    edi, [edx + 32h]
        movsd
        movsd
        movsd
        push   0
        push   0
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_3490
loc_1_3419:
        db  0F6h, 005h, 021h, 039h, 000h, 000h, 020h
        mov    ebp, dword ptr [esp + 1ch]
        jne    SHORT loc_1_345C
        cmp    byte ptr [ebp + 0ah], 1
        jne    SHORT loc_1_345C
        mov    byte ptr [ebp + 0ah], 2
        mov    dword ptr [ebp + 1ch], 3
        push   dword ptr [ebp + 10h]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [ebp + 10h], 0
        push   ebp
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [ebp + 14h], eax
        jmp    SHORT loc_1_3490
loc_1_345C:
        push   0
        push   1
        push   ebp
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_3490
loc_1_346B:
        mov    ebp, dword ptr [esp + 1ch]
        push   dword ptr [ebp + 10h]
        call   sub_1_008C
        add    esp, 4
        push   ebp
        db  068h, 087h, 004h, 000h, 000h
        movzx  eax, word ptr [ebx + 26h]
        push   eax
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [ebp + 10h], eax
loc_1_3490:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3495:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 1eh]
        cmp    al, 94h
        jne    NEAR PTR loc_1_358E
        cmp    byte ptr [esi + 24h], 0
        je     SHORT loc_1_34FE
        mov    edi, dword ptr [esp + 14h]
        mov    byte ptr [edi + 0ah], 3
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [edi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    dword ptr [edi + 1ch], 3
        push   dword ptr [edi + 10h]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [edi + 10h], 0
        cmp    dword ptr [edi + 14h], 0
        jne    NEAR PTR loc_1_35CF
        push   edi
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [edi + 14h], eax
        jmp    NEAR PTR loc_1_35CF
loc_1_34FE:
        movzx  eax, byte ptr [esi + 2eh]
        add    esi, 2eh
        lea    esi, [eax + esi + 2]
        db  0B8h, 070h, 039h, 000h, 000h
        lea    edx, [esi + 0ah]
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    SHORT loc_1_357C
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_1_357C
        db  0A1h, 06Ch, 039h, 000h, 000h
        cmp    eax, dword ptr [esi + 6]
        je     SHORT loc_1_3531
        cmp    dword ptr [esi + 6], 0
        jne    SHORT loc_1_357C
loc_1_3531:
        mov    edi, dword ptr [esp + 14h]
        mov    byte ptr [edi + 0ah], 3
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [edi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        mov    dword ptr [edi + 1ch], 3
        push   dword ptr [edi + 10h]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [edi + 10h], 0
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_35CF
        push   edi
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [edi + 14h], eax
        jmp    SHORT loc_1_35CF
loc_1_357C:
        push   0
        push   2
        push   dword ptr [esp + 1ch]
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_35CF
loc_1_358E:
        cmp    al, 90h
        jne    SHORT loc_1_35CF
        push   0
        cmp    byte ptr [esi + 24h], 1
        cmc
        sbb    eax, eax
        and    eax, 2
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_0FBB
        add    esp, 0ch
        mov    ax, word ptr [esi + 26h]
        mov    ch, al
        mov    cl, ah
        movzx  eax, cx
        cmp    eax, 5
        jbe    SHORT loc_1_35C5
        sub    eax, 5
        db  0A3h, 0E8h, 039h, 000h, 000h
        jmp    SHORT loc_1_35CF
loc_1_35C5:
        db  0C7h, 005h, 0E8h, 039h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_35CF:
        pop    edi
        pop    esi
        ret
sub_1_35D2:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 28h]
        cmp    byte ptr [ecx + 1eh], 94h
        jne    NEAR PTR loc_1_36DD
        cmp    byte ptr [ecx + 24h], 0
        jne    NEAR PTR loc_1_36CD
        mov    eax, dword ptr [esp + 30h]
        lea    edx, [ecx + 2eh]
        movzx  eax, word ptr [eax + 3ch]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 30h]
        mov    bx, word ptr [ecx + 2ch]
        mov    esi, dword ptr [eax + 34h]
        mov    ah, bl
        mov    dword ptr [esp + 14h], esi
        mov    al, bh
        cmp    dword ptr [esp + 18h], 0
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [esi], eax
        je     SHORT loc_1_362E
        mov    ebp, esi
        add    ebp, 6
        mov    ebx, dword ptr [esp + 1ch]
        jmp    SHORT loc_1_3639
loc_1_362E:
        mov    ebx, dword ptr [esp + 14h]
        add    ebx, 6
        mov    ebp, dword ptr [esp + 20h]
loc_1_3639:
        mov    esi, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], 0
        cmp    word ptr [esi + 4], 0
        jbe    SHORT loc_1_36C7
loc_1_364C:
        cmp    dword ptr [esp + 18h], 0
        mov    al, byte ptr [edx]
        jne    SHORT loc_1_3692
        mov    byte ptr [ebx + 2], al
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + 2]
        lea    edi, [ebx + 3]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        add    eax, 2
        add    edx, eax
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebx], ax
        movzx  eax, byte ptr [edx]
        add    eax, 2
        add    edx, eax
        movzx  eax, byte ptr [ebx + 2]
        inc    eax
        imul   eax, eax, 3
        add    ebx, eax
        jmp    SHORT loc_1_36B5
loc_1_3692:
        mov    byte ptr [ebp], al
        inc    ebp
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + 2]
        mov    edi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        add    ebp, eax
        lea    edx, [eax + edx + 2]
loc_1_36B5:
        mov    esi, dword ptr [esp + 14h]
        inc    dword ptr [esp + 10h]
        movzx  eax, word ptr [esi + 4]
        cmp    eax, dword ptr [esp + 10h]
        ja     SHORT loc_1_364C
loc_1_36C7:
        push   0
        push   0
        jmp    SHORT loc_1_36D1
loc_1_36CD:
        push   0
        push   3
loc_1_36D1:
        push   dword ptr [esp + 38h]
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_36DD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_36E5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        cmp    byte ptr [esi + 1eh], 95h
        jne    SHORT loc_1_3771
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_3761
        mov    ecx, dword ptr [esp + 20h]
        lea    eax, [esi + 2eh]
        mov    ebp, dword ptr [ecx + 34h]
        mov    cx, word ptr [esi + 2ch]
        mov    dx, cx
        shl    dl, 8
        or     dl, ch
        mov    byte ptr [ebp + 4], dl
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [ebp], ecx
        lea    edx, [ebp + 5]
        sub    ebx, ebx
        cmp    byte ptr [ebp + 4], bl
        jbe    SHORT loc_1_375B
loc_1_3725:
        mov    cl, byte ptr [eax]
        mov    byte ptr [edx], cl
        movzx  ecx, byte ptr [eax]
        mov    dword ptr [esp + 10h], ecx
        lea    esi, [eax + 2]
        lea    edi, [edx + 1]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [eax]
        add    ecx, 2
        add    eax, ecx
        movzx  ecx, byte ptr [edx]
        inc    ecx
        add    edx, ecx
        inc    ebx
        movzx  ecx, byte ptr [ebp + 4]
        cmp    ecx, ebx
        ja     SHORT loc_1_3725
loc_1_375B:
        push   0
        push   0
        jmp    SHORT loc_1_3765
loc_1_3761:
        push   0
        push   3
loc_1_3765:
        push   dword ptr [esp + 28h]
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_3771:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_3779:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax + 6], 0
        jne    SHORT loc_1_37D5
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 10h]
        call   sub_1_008C
        add    esp, 4
        mov    dword ptr [esi + 10h], 0
        db  081h, 03Dh, 0E0h, 039h, 000h, 000h, 090h, 039h, 000h, 000h
        jne    SHORT loc_1_37AD
        push   0
        push   0
        push   esi
        jmp    SHORT loc_1_37DD
loc_1_37AD:
        mov    byte ptr [esi + 0ah], 1
        db  066h, 0A1h, 030h, 03Ah, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        db  066h, 0FFh, 005h, 030h, 03Ah, 000h, 000h
        push   esi
        db  068h, 096h, 004h, 000h, 000h
        call   sub_1_00BF
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
        jmp    SHORT loc_1_37E5
loc_1_37D5:
        push   0
        push   -1
        push   dword ptr [esp + 14h]
loc_1_37DD:
        call   sub_1_0FBB
        add    esp, 0ch
loc_1_37E5:
        pop    esi
        ret
sub_1_37E7:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    word ptr [edi + 4], 100h
        jne    NEAR PTR loc_1_38F2
        test   byte ptr [edi], 80h
        je     NEAR PTR loc_1_38F2
        db  08Bh, 035h, 080h, 039h, 000h, 000h
        db  081h, 0FEh, 080h, 039h, 000h, 000h
        je     NEAR PTR loc_1_38F2
        mov    ax, word ptr [edi + 2]
loc_1_3818:
        cmp    word ptr [esi + 2ch], ax
        je     SHORT loc_1_382D
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 080h, 039h, 000h, 000h
        jne    SHORT loc_1_3818
        jmp    NEAR PTR loc_1_38F2
loc_1_382D:
        movzx  eax, byte ptr [esi + 9]
        sub    eax, 2
        cmp    eax, 7
        ja     NEAR PTR loc_1_38F2
        db  0FFh, 024h, 085h, 0F5h, 038h, 000h, 000h
loc_1_3844:
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        call   sub_1_337C
        add    esp, 0ch
        jmp    NEAR PTR loc_1_38F2
loc_1_385A:
        cmp    byte ptr [edi + 6], 9
        jne    SHORT loc_1_3888
        push   dword ptr [esi + 10h]
        call   sub_1_008C
        add    esp, 4
        mov    ax, word ptr [edi + 8]
        push   esi
        mov    ch, al
        mov    cl, ah
        db  068h, 087h, 004h, 000h, 000h
        movzx  eax, cx
        push   eax
        call   sub_1_0074
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
loc_1_3888:
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        call   sub_1_3495
        add    esp, 0ch
        jmp    SHORT loc_1_38F2
loc_1_389B:
        push   0
        push   0
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_38F2
loc_1_38AA:
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        call   sub_1_35D2
        add    esp, 0ch
        jmp    SHORT loc_1_38F2
loc_1_38BD:
        push   esi
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        call   sub_1_36E5
        add    esp, 0ch
        jmp    SHORT loc_1_38F2
loc_1_38D0:
        push   0
        cmp    byte ptr [edi + 6], 1
        cmc
        sbb    eax, eax
        and    eax, 3
        push   eax
        push   esi
        call   sub_1_0FBB
        add    esp, 0ch
        jmp    SHORT loc_1_38F2
loc_1_38E8:
        push   esi
        push   edi
        call   sub_1_3779
        add    esp, 8
loc_1_38F2:
        pop    edi
        pop    esi
        ret
        db  044h, 038h, 000h, 000h, 05Ah, 038h, 000h, 000h, 09Bh, 038h, 000h, 000h, 0AAh, 038h, 000h, 000h
        db  0BDh, 038h, 000h, 000h, 0D0h, 038h, 000h, 000h, 0E8h, 038h, 000h, 000h, 0E8h, 038h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 055h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 055h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3980:
        cmp    byte ptr [ecx], 0
        db  000h, 080h, 039h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_3990:
        nop
        cmp    dword ptr [eax], eax
        db  000h, 090h, 039h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C3h
        sbb    al, byte ptr [eax]
        db  000h, 0ADh, 011h, 000h, 000h, 069h
        adc    eax, dword ptr [eax]
        db  000h, 02Eh
        sbb    al, byte ptr [eax]
        db  000h, 072h, 015h
        add    byte ptr [eax], al
        dec    eax
        sbb    byte ptr [eax], al
        db  000h, 072h, 015h
        add    byte ptr [eax], al
        sahf
        sub    al, 0
        db  000h, 09Eh, 02Ch, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_39E8:
        and    ch, bh
        add    byte ptr [eax], al
loc_1_39EC:
        in     al, dx
        cmp    dword ptr [eax], eax
        db  000h, 0ECh
        cmp    dword ptr [eax], eax
        db  000h
loc_1_39F4:
        push   edi
        outsd
        db  072h, 06Bh
        db  067h, 072h, 06Fh
        db  075h, 070h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   esi
        dec    esi
        inc    ebp
        push   esp
        push   ebx
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_3A38:
        inc    eax
loc_1_3A39:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
