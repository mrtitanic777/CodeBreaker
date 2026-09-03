; s3v_lcode_01.asm -- S3V object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

S3MINI_Control:
        cmp    eax, 24h
        jae    SHORT loc_1_000C
        db  0FFh, 024h, 085h, 00Eh, 000h, 000h, 000h
loc_1_000C:
        clc
        ret
        db  000h, 000h, 000h, 000h, 01Bh, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 059h, 001h, 000h, 000h, 00Ch, 000h, 000h, 000h, 0B1h, 001h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 0EDh, 001h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 01Bh, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 09Eh, 000h, 000h, 000h
S3MINI_W32_DEVICEIOCONTROL:
        pushad
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, -1
        je     SHORT loc_1_00B8
        cmp    ecx, 0
        je     SHORT loc_1_00BA
        cmp    ecx, 0ah
        je     SHORT loc_1_00BC
        cmp    ecx, 0bh
        je     SHORT loc_1_00D8
        jae    SHORT loc_1_0110
loc_1_00B8:
        jmp    SHORT loc_1_011D
loc_1_00BA:
        jmp    SHORT loc_1_011D
loc_1_00BC:
        mov    edx, dword ptr [esi + 10h]
        db  089h, 015h, 052h, 004h, 000h, 000h
        mov    edx, dword ptr [esi + 18h]
        db  089h, 015h, 056h, 004h, 000h, 000h
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_0131
        jmp    SHORT loc_1_011D
loc_1_00D8:
        mov    edx, dword ptr [esi + 10h]
        mov    ebx, dword ptr [edx]
        shr    ebx, 0ch
        mov    ecx, 1
        db  0B8h, 05Ah, 004h, 000h, 000h
        push   0
        push   eax
        push   ecx
        push   ebx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0110
        db  0BBh, 05Ah, 004h, 000h, 000h
        mov    edx, dword ptr [esi + 18h]
        mov    ebx, dword ptr [ebx]
        and    ebx, 0fffff000h
        mov    dword ptr [edx], ebx
        jmp    SHORT loc_1_011D
loc_1_0110:
        db  0C7h, 005h, 04Eh, 004h, 000h, 000h, 032h, 000h, 000h, 000h
        stc
        jmp    SHORT loc_1_012A
loc_1_011D:
        db  0C7h, 005h, 04Eh, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_1_012A
loc_1_012A:
        popad
        db  0A1h, 04Eh, 004h, 000h, 000h
        ret
sub_1_0131:
        pushad
        db  08Bh, 015h, 052h, 004h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        push   0
        push   edx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0157
        db  08Bh, 015h, 056h, 004h, 000h, 000h
        mov    dword ptr [edx], eax
loc_1_0157:
        popad
        ret
S3MINI_SYSTEM_EXIT:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_1_0171
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        jne    SHORT loc_1_01B0
loc_1_0171:
        db  083h, 03Dh, 0B8h, 003h, 000h, 000h, 000h
        je     SHORT loc_1_01B0
        db  0A1h, 0BCh, 003h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        db  0C7h, 005h, 0BCh, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0B8h, 003h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        db  0C7h, 005h, 0B8h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_01B0:
        ret
S3MINI_CREATE_VM:
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_1_01EC
        db  0C6h, 005h, 039h, 004h, 000h, 000h, 001h
        db  0FEh, 005h, 037h, 004h, 000h, 000h
        db  0A1h, 0A0h, 003h, 000h, 000h
        db  003h, 005h, 0ACh, 003h, 000h, 000h
        mov    edx, dword ptr [eax]
        test   edx, 0fh
        je     SHORT loc_1_01E2
        db  089h, 015h, 03Ch, 004h, 000h, 000h
loc_1_01E2:
        mov    edx, 4022h
        call   sub_1_0254
loc_1_01EC:
        ret
S3MINI_VM_INIT:
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_1_0216
        db  0CDh, 020h, 005h, 000h, 017h, 000h
        test   eax, 4
        jne    SHORT loc_1_0216
        mov    edx, 4021h
        call   sub_1_0254
        db  0C6h, 005h, 037h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_0216
loc_1_0216:
        ret
sub_1_0217:
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], dx
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_1_0254:
        pushad
        db  03Bh, 01Dh, 078h, 003h, 000h, 000h
        je     SHORT loc_1_0277
        mov    eax, 400000h
        db  08Bh, 01Dh, 078h, 003h, 000h, 000h
        xor    ecx, ecx
        db  0BEh, 017h, 002h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_027C
loc_1_0277:
        call   sub_1_0217
loc_1_027C:
        popad
        ret
loc_1_027E:
        db  03Bh, 01Dh, 078h, 003h, 000h, 000h
        je     SHORT loc_1_02DD
        cmp    edx, 9ae8h
        je     SHORT loc_1_0296
        cmp    edx, 9ae9h
        jne    SHORT loc_1_02DD
loc_1_0296:
        cmp    ecx, 0
        je     SHORT loc_1_02BE
        cmp    ecx, 8
        jne    SHORT loc_1_02DD
        mov    ecx, 0ffffh
loc_1_02A5:
        in     ax, dx
        cmp    ax, -1
        je     SHORT loc_1_02B9
        test   ah, 2
        je     SHORT loc_1_02DA
        db  0E8h, 0EBh, 009h, 0FFh, 0FFh
        loop   loc_1_02A5
loc_1_02B9:
        and    ah, 0fdh
        jmp    SHORT loc_1_02DA
loc_1_02BE:
        cmp    dl, 0e9h
        jne    SHORT loc_1_02DD
        mov    ecx, 0ffffh
loc_1_02C8:
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_1_02D8
        test   al, 2
        je     SHORT loc_1_02DA
        db  0E8h, 0CCh, 009h, 0FFh, 0FFh
        loop   loc_1_02C8
loc_1_02D8:
        and    al, 0fdh
loc_1_02DA:
        stc
        jmp    SHORT loc_1_02DE
loc_1_02DD:
        clc
loc_1_02DE:
        ret
loc_1_02DF:
        cmp    dx, 4f00h
        je     SHORT loc_1_02FC
        cmp    dx, 4f02h
        jne    SHORT loc_1_0321
        db  03Bh, 01Dh, 078h, 003h, 000h, 000h
        je     SHORT loc_1_0321
        db  0E8h, 0AAh, 002h, 0FFh, 0FFh
        jmp    SHORT loc_1_0321
loc_1_02FC:
        db  03Bh, 01Dh, 078h, 003h, 000h, 000h
        jne    SHORT loc_1_0321
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_1_0321:
        ret
        db  000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 053h, 033h, 04Dh, 049h
        db  04Eh, 049h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_03B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_042A:
        add    byte ptr [eax], al
loc_1_042C:
        add    byte ptr [eax], al
loc_1_042E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0434:
        add    byte ptr [eax], al
loc_1_0436:
        add    byte ptr [eax], al
loc_1_0438:
        add    byte ptr [eax], al
loc_1_043A:
        add    byte ptr [eax], al
loc_1_043C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0440:
        add    byte ptr [eax], al
loc_1_0442:
        add    bh, bh
loc_1_0444:
        add    byte ptr [eax], al
loc_1_0446:
        add    bh, bh
loc_1_0448:
        add    byte ptr [eax], al
loc_1_044A:
        add    byte ptr [eax], al
loc_1_044C:
        add    byte ptr [eax], al
loc_1_044E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0452:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0456:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_045A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0462:
        inc    ebp
        dec    esi
        push   ebp
        dec    ebp
        pop    esp
        push   eax
        inc    ebx
        dec    ecx
        pop    esp
        push   esi
        inc    ebp
        dec    esi
        pop    edi
        cmp    byte ptr [eax], dh
        cmp    byte ptr [esi], dh
        inc    esp
        inc    ebp
        push   esi
        pop    edi
        xor    byte ptr [eax + edi], dh
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
