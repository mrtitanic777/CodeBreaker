; vtd_lcode_01.asm -- VTD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VTD_Get_Version:
        xor    eax, eax
        mov    ah, 4
        xor    ebx, ebx
        inc    ebx
        push   36h
        pop    ecx
        clc
        ret
VTD_Update_System_Clock:
        pushad
        pushfd
        call   sub_1_0117
        call   sub_1_001C
        popfd
        popad
loc_1_001A:
        ret
        db  090h
sub_1_001C:
        sub    eax, dword ptr [esi + 28h]
        mov    ebx, 4a9h
        cmp    eax, ebx
        jb     SHORT loc_1_001A
        mov    ecx, eax
        xor    edx, edx
        idiv   ebx
        sub    ecx, edx
        add    dword ptr [esi + 28h], ecx
        mov    ecx, eax
        db  0CDh, 020h, 0C0h, 080h, 001h, 000h
VTD_Get_Interrupt_Period:
        push   ecx
        push   edi
        push   36h
        pop    ecx
        db  0BFh, 038h, 005h, 000h, 000h
        xor    eax, eax
        cld
        repe scasb
        neg    ecx
        lea    eax, [ecx + 36h]
        pop    edi
        pop    ecx
        ret
VTD_Begin_Min_Int_Period:
        pushad
        xor    ecx, ecx
        inc    ecx
loc_1_0056:
        cmp    eax, 36h
        ja     SHORT loc_1_0074
        db  0BEh, 0ECh, 004h, 000h, 000h
        add    cl, byte ptr [eax + esi + 4bh]
        cmp    cl, ch
        je     SHORT loc_1_0074
        mov    byte ptr [eax + esi + 4bh], cl
        call   sub_1_01F9
        popad
        clc
        ret
loc_1_0074:
        popad
        stc
        ret
VTD_End_Min_Int_Period:
        pushad
        or     ecx, 0ffffffffh
        jmp    SHORT loc_1_0056
VTD_Disable_Trapping:
        push   edx
        push   40h
        push   43h
loc_1_0082:
        pop    edx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        cmp    edx, 40h
        jne    SHORT loc_1_0082
        db  08Bh, 015h, 0F4h, 004h, 000h, 000h
        inc    dword ptr [edx + ebx + 0ch]
        pop    edx
        ret
VTD_Enable_Trapping:
        push   edx
        db  08Bh, 015h, 0F4h, 004h, 000h, 000h
        dec    dword ptr [edx + ebx + 0ch]
        jl     SHORT loc_1_00BB
        jne    SHORT loc_1_00B9
        push   40h
        push   43h
loc_1_00AD:
        pop    edx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        cmp    edx, 40h
        jne    SHORT loc_1_00AD
loc_1_00B9:
        pop    edx
        ret
loc_1_00BB:
        inc    dword ptr [edx + ebx + 0ch]
        pop    edx
        ret
VTD_Delay:
        push   eax
        push   edx
        pushfd
        call   sub_1_00EE
        popfd
loc_1_00CA:
        mov    edx, eax
        pushfd
        call   sub_1_00EE
        sub    edx, eax
        jae    SHORT loc_1_00E4
        cmp    edx, 0fffffe00h
        jg     SHORT loc_1_00E4
        db  003h, 015h, 008h, 005h, 000h, 000h
loc_1_00E4:
        popfd
        sub    ecx, edx
        jg     SHORT loc_1_00CA
        neg    ecx
        pop    edx
        pop    eax
        ret
sub_1_00EE:
        xor    eax, eax
        cli
        out    43h, al
        jmp    SHORT loc_1_00F5
loc_1_00F5:
        jmp    SHORT loc_1_00F7
loc_1_00F7:
        in     al, 40h
        jmp    SHORT loc_1_00FB
loc_1_00FB:
        jmp    SHORT loc_1_00FD
loc_1_00FD:
        mov    ah, al
        in     al, 40h
        xchg   al, ah
        ret
VTD_Get_Real_Time:
        push   esi
        pushfd
        xor    edx, edx
        call   sub_1_0117
        adc    edx, dword ptr [esi + 4]
        popfd
        pop    esi
        ret
sub_1_0113:
        cli
        mov    ecx, dword ptr [esi + 2ch]
sub_1_0117:
        db  0BEh, 0ECh, 004h, 000h, 000h
        call   sub_1_00EE
        cmp    eax, dword ptr [esi + 24h]
        ja     SHORT loc_1_0131
loc_1_0126:
        mov    dword ptr [esi + 24h], eax
        sub    eax, dword ptr [esi + 18h]
        neg    eax
        add    eax, dword ptr [esi]
        ret
loc_1_0131:
        xor    eax, eax
        jmp    SHORT loc_1_0126
VTD_Control:
        cmp    eax, 2
        ja     SHORT loc_1_013F
        db  0E9h, 0C1h, 0FEh, 0FEh, 0FFh
loc_1_013F:
        cmp    eax, 3
        db  00Fh, 084h, 002h, 0FFh, 0FEh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 0AFh, 0FEh, 0FEh, 0FFh
        cmp    eax, 0dh
        db  00Fh, 084h, 0A6h, 0FEh, 0FEh, 0FFh
        cmp    eax, 0eh
        db  00Fh, 084h, 0B7h, 0FEh, 0FEh, 0FFh
        cmp    eax, 0bh
        db  00Fh, 084h, 094h, 0FEh, 0FEh, 0FFh
        cmp    eax, 6
        db  00Fh, 084h, 046h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0fh
        je     SHORT loc_1_01D4
        db  0F7h, 005h, 080h, 001h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_018F
        cmp    eax, 14h
        db  00Fh, 084h, 071h, 0FEh, 0FEh, 0FFh
loc_1_018F:
        clc
        ret
sub_1_0191:
        cmp    byte ptr [esi + 4ah], 0
        jne    SHORT loc_1_01AB
        inc    byte ptr [esi + 4ah]
        push   0
        push   esi
        db  068h, 0FFh, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
loc_1_01AB:
        ret
loc_1_01AC:
        mov    eax, 80000000h
        cmp    dword ptr [esp + 4], 4
        jne    SHORT loc_1_01D3
        xor    eax, eax
        pushad
        db  0BEh, 0ECh, 004h, 000h, 000h
        mov    ebx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 8]
        add    edi, ebx
        call   sub_1_0221
        call   sub_1_0191
        popad
loc_1_01D3:
        ret
loc_1_01D4:
        test   edx, edx
        je     SHORT loc_1_01DD
        cmp    edx, 0dh
        jne    SHORT loc_1_018F
loc_1_01DD:
        db  0BEh, 0ECh, 004h, 000h, 000h
        cmp    dword ptr [esi + 10h], ebx
        je     SHORT loc_1_018F
        mov    dword ptr [esi + 10h], ebx
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT sub_1_01F9
        push   esi
        mov    edx, esi
        call   sub_1_03F0
        pop    esi
sub_1_01F9:
        call   VTD_Get_Interrupt_Period
        mov    ecx, 10000h
        cmp    eax, 36h
        je     SHORT loc_1_0216
        imul   eax, eax, 4a9h
        bsr    eax, eax
        xor    ecx, ecx
        bts    ecx, eax
loc_1_0216:
        mov    ebx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 8]
        mov    dword ptr [esi + 20h], ecx
        add    edi, ebx
sub_1_0221:
        mov    ecx, dword ptr [edi + 8]
        test   cx, cx
        jne    SHORT loc_1_022E
        mov    ecx, dword ptr [esi + 20h]
        xor    ebx, ebx
loc_1_022E:
        test   eax, eax
        je     SHORT loc_1_0237
        cmp    dword ptr [esi + 1ch], ecx
        je     SHORT loc_1_025C
loc_1_0237:
        sti
        call   sub_1_00EE
        cmp    eax, dword ptr [esi + 30h]
        jb     SHORT loc_1_0237
        add    eax, 0ah
        sub    eax, dword ptr [esi + 1ch]
        cmp    eax, 0ah
        jbe    SHORT loc_1_0237
        mov    al, cl
        out    40h, al
        jmp    SHORT loc_1_0253
loc_1_0253:
        jmp    SHORT loc_1_0255
loc_1_0255:
        mov    al, ch
        out    40h, al
        mov    dword ptr [esi + 1ch], ecx
loc_1_025C:
        mov    dword ptr [esi + 14h], ebx
        sti
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0264:
        cmp    byte ptr [ebp + 1dh], 10h
        jb     SHORT loc_1_026C
loc_1_026A:
        stc
        ret
loc_1_026C:
        cmp    byte ptr [ebp + 1dh], 1
        ja     SHORT loc_1_029B
        jb     SHORT loc_1_0285
        mov    eax, dword ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        db  0A3h, 030h, 005h, 000h, 000h
        clc
        ret
loc_1_0285:
        db  0A1h, 030h, 005h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        mov    byte ptr [ebp + 1ch], 0
        clc
        ret
loc_1_029B:
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jb     SHORT loc_1_026A
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    eax, 1f4h
        db  0BEh, 0C0h, 002h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        stc
        ret
loc_1_02C0:
        db  0CDh, 020h, 020h, 080h, 001h, 000h
        mov    eax, eax
loc_1_02C8:
        xchg   esi, eax
        in     al, 61h
        or     al, 80h
        jmp    SHORT loc_1_02CF
loc_1_02CF:
        jmp    SHORT loc_1_02D1
loc_1_02D1:
        out    61h, al
        xchg   esi, eax
loc_1_02D4:
        push   eax
        mov    esi, edx
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    eax, dword ptr [esi + 18h]
        mov    edi, eax
        add    eax, dword ptr [esi]
        adc    dword ptr [esi + 4], 0
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 18h], edx
        mov    dword ptr [esi + 24h], edx
        call   sub_1_001C
        pop    eax
        mov    ebx, dword ptr [esi + 10h]
        test   di, di
        je     SHORT loc_1_0307
        add    word ptr [esi + 40h], di
        jae    SHORT loc_1_0326
loc_1_0307:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esi + 2ch], ecx
        mov    ecx, dword ptr [esi + 44h]
        inc    ecx
        cmp    ecx, 1800b0h
        jae    SHORT loc_1_032C
loc_1_0318:
        mov    dword ptr [esi + 44h], ecx
        dec    byte ptr [esi + 49h]
        je     SHORT loc_1_0340
loc_1_0320:
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_0326:
        cmp    dword ptr [esi + 14h], ebx
        je     SHORT loc_1_0320
        ret
loc_1_032C:
        inc    byte ptr [esi + 48h]
        mov    ecx, dword ptr [esi - 24h]
        inc    word ptr [ecx]
        pushad
        call   sub_1_0191
        popad
        xor    ecx, ecx
        jmp    SHORT loc_1_0318
loc_1_0340:
        inc    byte ptr [esi + 4bh]
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        jmp    SHORT loc_1_0320
loc_1_034B:
        db  0BEh, 037h, 005h, 000h, 000h
        mov    ecx, 0cf05ea00h
        mov    edi, dword ptr [esi - 0fh]
        cmp    ebx, dword ptr [esi - 3bh]
        jne    SHORT loc_1_036D
loc_1_035D:
        mov    byte ptr [esi], cl
loc_1_035F:
        cmp    byte ptr [edi + 1], cl
        jne    SHORT loc_1_0365
        ret
loc_1_0365:
        mov    byte ptr [edi + 1], cl
        mov    byte ptr [edi + 1dh], ch
        clc
        ret
loc_1_036D:
        cmp    byte ptr [esi], cl
        jne    SHORT loc_1_035D
        xor    eax, eax
        shr    ecx, 10h
        mov    al, 1ch
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        cmp    dword ptr [edi + 13h], eax
        je     SHORT loc_1_035F
        mov    dword ptr [edi + 13h], eax
        jmp    SHORT loc_1_035F
loc_1_0388:
        db  0CDh, 020h, 003h, 080h, 003h, 000h
loc_1_038E:
        jb     SHORT loc_1_0398
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0399
loc_1_0398:
        ret
loc_1_0399:
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        and    cl, 3
        cmp    cl, 2
        jne    SHORT loc_1_0398
        db  0CDh, 020h, 015h, 080h, 003h, 000h
        mov    eax, eax
loc_1_03B0:
        cld
        db  0BEh, 030h, 005h, 000h, 000h
        mov    edi, 46ch
        movsd
        movsb
        mov    word ptr [esi - 1], 900h
        stc
        ret
loc_1_03C5:
        mov    edi, ebx
        xor    ecx, ecx
        mov    esi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 10h]
loc_1_03CF:
        cmp    ebx, eax
        je     SHORT loc_1_03E4
        test   byte ptr [esi + ebx], 4
        je     SHORT loc_1_03E4
        xchg   ecx, eax
        mov    eax, dword ptr [edx + 0ch]
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        xchg   ecx, eax
loc_1_03E4:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, edi
        jne    SHORT loc_1_03CF
        jecxz  loc_1_0402
sub_1_03F0:
        mov    eax, 1f4h
        db  0BEh, 0C5h, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    ecx, esi
loc_1_0402:
        mov    dword ptr [edx + 38h], ecx
        ret
loc_1_0406:
        db  0BAh, 0ECh, 004h, 000h, 000h
        xchg   ebx, eax
        mov    edi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 34h]
        db  0CDh, 020h, 08Dh, 001h, 001h, 000h
        xor    eax, dword ptr [ecx + 20h]
        add    edi, ebx
        xor    eax, dword ptr [ecx + 70h]
        jne    SHORT loc_1_0426
        and    byte ptr [edi], 0fbh
        ret
loc_1_0426:
        or     byte ptr [edi], 4
        cmp    ebx, dword ptr [edx + 10h]
        je     SHORT loc_1_0434
        cmp    dword ptr [edx + 38h], 0
        je     SHORT sub_1_03F0
loc_1_0434:
        ret
VTD_Get_Date_And_Time:
        push   esi
        push   ecx
        db  0BEh, 030h, 005h, 000h, 000h
        pushfd
        cli
        mov    eax, dword ptr [esi]
        xor    edx, edx
        mov    ecx, dword ptr [esi - 4]
        lea    eax, [eax + eax*4]
        lea    ecx, [ecx + ecx*4]
        lea    eax, [eax + eax*4]
        lea    ecx, [ecx + ecx*4]
        shld   edx, eax, 11h
        add    ecx, ecx
        shl    eax, 11h
        mov    esi, dword ptr [esi - 68h]
        add    eax, ecx
        mov    ecx, 0e90bh
        adc    edx, 0
        div    ecx
        mov    dx, word ptr [esi]
        popfd
        pop    ecx
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 03Bh, 000h, 000h, 000h, 052h, 000h, 000h, 000h
        db  077h, 000h, 000h, 000h, 07Dh, 000h, 000h, 000h, 09Ah, 000h, 000h, 000h, 004h, 001h, 000h, 000h
        db  035h, 004h, 000h, 000h, 014h, 001h, 000h, 000h, 0C1h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 005h, 000h, 004h, 000h, 000h, 000h, 056h, 054h, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 014h, 035h, 001h, 000h, 000h, 008h, 000h, 000h, 000h
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  070h, 004h, 000h, 000h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_051C:
        adc    al, 0
        add    byte ptr [eax], al
loc_1_0520:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0528:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_052C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0530:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0536:
        add    byte ptr [eax], al
loc_1_0538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
