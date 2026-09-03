; IRLAPFRM -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 2834 instructions recovered by recursive descent

Declare_Virtual_Device IRLAPFRM, 1, 0, IRLAPFRM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 030000002h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2678  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0070:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        xor    edx, edx
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IRLAPFRM'                  ; DDB_Name
        dd  030000002h                  ; DDB_Init_Order
        dd  IRLAPFRM_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00E0:
        inc    ebx
        dec    edi
        dec    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx - 0x64dcedef], cl
loc_1_0167:
        xor    ah, byte ptr [esi + eax*2]
        lodsd  eax, dword ptr [esi]
        push   edi
        mov    edi, 0xc18c4874
        popfd  
        pop    edx
        scasd  eax, dword ptr es:[edi]
        sar    dword ptr [esi - 0x241a3594], cl
        jle    loc_1_0167
        idiv   eax
        adc    dword ptr [eax], 0x33930108
        sbb    ah, byte ptr [edx]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   esi
        sub    al, 0x47
        mov    bh, 0x75
        leave  
        pushfd 
        inc    eax
        db  08Dh, 0DBh, 0BFh, 052h, 0AEh, 0EDh, 0DAh, 064h, 0CBh, 0FFh, 0F9h, 076h, 0E8h, 002h, 021h, 08Bh
        db  030h, 010h, 002h, 099h, 013h, 026h, 067h, 0AFh, 076h, 034h, 044h, 0BDh, 055h, 04Ah, 0ADh, 0C3h
        db  0BCh, 058h, 08Eh, 0D1h, 09Fh, 06Eh, 0EBh, 0E7h, 0FAh, 07Ch, 0C8h, 0F5h, 0D9h, 083h, 031h, 00Ah
        db  020h, 091h, 012h, 018h, 003h, 0A7h, 077h, 02Eh, 066h, 0B5h, 054h, 03Ch, 045h, 0CBh, 0BDh, 042h
        db  0ACh, 0D9h, 09Eh, 050h, 08Fh, 0EFh, 0FBh, 066h, 0EAh, 0FDh, 0D8h, 074h, 0C9h, 004h, 042h, 08Dh
        db  053h, 016h, 061h, 09Fh, 070h, 020h, 004h, 0A9h, 015h, 032h, 027h, 0BBh, 036h, 04Ch, 0CEh, 0C5h
        db  0DFh, 05Eh, 0EDh, 0D7h, 0FCh, 068h, 088h, 0E1h, 099h, 07Ah, 0ABh, 0F3h, 0BAh, 085h, 052h, 00Ch
        db  043h, 097h, 071h, 01Eh, 060h, 0A1h, 014h, 028h, 005h, 0B3h, 037h, 03Ah, 026h, 0CDh, 0DEh, 044h
        db  0CFh, 0DFh, 0FDh, 056h, 0ECh, 0E9h, 098h, 060h, 089h, 0FBh, 0BBh, 072h, 0AAh, 006h, 063h, 08Fh
        db  072h, 014h, 040h, 09Dh, 051h, 022h, 025h, 0ABh, 034h, 030h, 006h, 0B9h, 017h, 04Eh, 0EFh, 0C7h
        db  0FEh, 05Ch, 0CCh, 0D5h, 0DDh, 06Ah, 0A9h, 0E3h, 0B8h, 078h, 08Ah, 0F1h, 09Bh, 087h, 073h, 00Eh
        db  062h, 095h, 050h, 01Ch, 041h, 0A3h, 035h, 02Ah, 024h, 0B1h, 016h, 038h, 007h, 0CFh, 0FFh, 046h
        db  0EEh, 0DDh, 0DCh, 054h, 0CDh, 0EBh, 0B9h, 062h, 0A8h, 0F9h, 09Ah, 070h, 08Bh, 008h, 084h, 081h
        db  095h, 01Ah, 0A7h, 093h, 0B6h, 02Ch, 0C2h, 0A5h, 0D3h, 03Eh, 0E1h, 0B7h, 0F0h, 040h, 008h, 0C9h
        db  019h, 052h, 02Bh, 0DBh, 03Ah, 064h, 04Eh, 0EDh, 05Fh, 076h, 06Dh, 0FFh, 07Ch, 089h, 094h, 000h
        db  085h, 09Bh, 0B7h, 012h, 0A6h, 0ADh, 0D2h, 024h, 0C3h, 0BFh, 0F1h, 036h, 0E0h, 0C1h, 018h, 048h
        db  009h, 0D3h, 03Bh, 05Ah, 02Ah, 0E5h, 05Eh, 06Ch, 04Fh, 0F7h, 07Dh, 07Eh, 06Ch, 00Ah, 0A5h, 083h
        db  0B4h, 018h, 086h, 091h, 097h, 02Eh, 0E3h, 0A7h, 0F2h, 03Ch, 0C0h, 0B5h, 0D1h, 042h, 029h, 0CBh
        db  038h, 050h, 00Ah, 0D9h, 01Bh, 066h, 06Fh, 0EFh, 07Eh, 074h, 04Ch, 0FDh, 05Dh, 08Bh, 0B5h, 002h
        db  0A4h, 099h, 096h, 010h, 087h, 0AFh, 0F3h, 026h, 0E2h, 0BDh, 0D0h, 034h, 0C1h, 0C3h, 039h, 04Ah
        db  028h, 0D1h, 01Ah, 058h, 00Bh, 0E7h, 07Fh, 06Eh, 06Eh, 0F5h, 05Ch, 07Ch, 04Dh, 00Ch, 0C6h, 085h
        db  0D7h, 01Eh, 0E5h, 097h, 0F4h, 028h, 080h, 0A1h, 091h, 03Ah, 0A3h, 0B3h, 0B2h, 044h, 04Ah, 0CDh
        db  05Bh, 056h, 069h, 0DFh, 078h, 060h, 00Ch, 0E9h, 01Dh, 072h, 02Fh, 0FBh, 03Eh, 08Dh, 0D6h, 004h
        db  0C7h, 09Fh, 0F5h, 016h, 0E4h, 0A9h, 090h, 020h, 081h, 0BBh, 0B3h, 032h, 0A2h, 0C5h, 05Ah, 04Ch
        db  04Bh, 0D7h, 079h, 05Eh, 068h, 0E1h, 01Ch, 068h, 00Dh, 0F3h, 03Fh, 07Ah, 02Eh, 00Eh, 0E7h, 087h
        db  0F6h, 01Ch, 0C4h, 095h, 0D5h, 02Ah, 0A1h, 0A3h, 0B0h, 038h, 082h, 0B1h, 093h, 046h, 06Bh, 0CFh
        db  07Ah, 054h, 048h, 0DDh, 059h, 062h, 02Dh, 0EBh, 03Ch, 070h, 00Eh, 0F9h, 01Fh, 08Fh, 0F7h, 006h
        db  0E6h, 09Dh, 0D4h, 014h, 0C5h, 0ABh, 0B1h, 022h, 0A0h, 0B9h, 092h, 030h, 083h, 0C7h, 07Bh, 04Eh
        db  06Ah, 0D5h, 058h, 05Ch, 049h, 0E3h, 03Dh, 06Ah, 02Ch, 0F1h, 01Eh, 078h, 00Fh
loc_1_0360:
        aam    0x30
        add    byte ptr [eax], al
        push   0x18
        add    byte ptr [eax], al
        loop   loc_1_036E
        add    byte ptr [eax], al
        jno    loc_1_0370
loc_1_036E:
        add    byte ptr [eax], al
loc_1_0370:
        jge    loc_1_0372
loc_1_0372:
        add    byte ptr [eax], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0380:
        inc    edx
        add    byte ptr [eax], al            ; -> loc_1_0510
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0568
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_1_1560
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E30
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_097D
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_099D
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1598
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_05C5
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_07F2
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0EFF
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_092B
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_09BE
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_095D
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0943
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1E60
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_2591
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x3f000000], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        aas    
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        sbb    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
loc_1_0400:
        xor    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
        add    byte ptr [edx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    eax, 1
        add    byte ptr [eax], al
        adc    al, 0xa
        add    al, byte ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [esp + eax], dl
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        sbb    eax, 0x10306
        add    byte ptr [eax], al
        jae    0x46c
        or     al, 6
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0450:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0454:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_045C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0460:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0464:
        push   esp
        js     0x4ab
        popal  
        je     0x4cb
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x70530000
        add    byte ptr fs:[eax], al
        add    byte ptr [edi + ebp*2 + 0x6e], al
        insb   byte ptr es:[di], dx
        push   esp
        jns    0x4f1
        add    byte ptr gs:[eax], al
loc_1_0484:
        dec    ebp
        imul   ebp, dword ptr [esi + 0x54], 0x546e7275
        imul   ebp, dword ptr [ebp + 0x65], 0x64644100
        imul   esi, dword ptr [ecx + ebp*2 + 0x6f], 0x426c616e
        dec    edi
        inc    esi
        jae    loc_1_049F
loc_1_049F:
        add    byte ptr [eax + 0x6f], dl
        jb     loc_1_0518
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
IRLAPFRM_Control:
        cmp    eax, 0x1b
        je     loc_1_04CA
        cmp    eax, 0x1c
        jne    loc_1_04C3
        call   sub_1_0B72
        cmp    eax, 1
        ret    
loc_1_04C3:
        cmp    eax, 0x1a
        je     loc_1_04EB
        clc    
        ret    
loc_1_04CA:
        VxDCall  Dev_04A2.Service_0000h          ; cd200000a204
        jb     loc_1_04DD
        call   sub_2_0000
loc_1_04D7:
        or     eax, eax
        je     loc_1_04DD
        clc    
        ret    
loc_1_04DD:
        stc    
        ret    
sub_1_04DF:
        VxDCall  Dev_04A2.Service_8001h          ; cd200180a204
sub_1_04E5:
        VxDCall  Dev_04A2.Service_8025h          ; cd202580a204
loc_1_04EB:
        push   bp
        push   edx
        push   ebx
        push   esi
        call   sub_1_0BAE
        pop    esi
        pop    ebx
        pop    edx
        pop    bp
        clc    
        ret    
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h, 0C3h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0510:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh
loc_1_0518:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        test   byte ptr [esi], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_1_054F
        call   sub_1_1EC6
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x24], eax
        xor    eax, eax
        jmp    loc_1_0563
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_054F:
        mov    eax, 4
        jmp    loc_1_0563
        db  0B8h, 00Eh, 000h, 000h, 000h, 0E9h, 000h, 000h, 000h, 000h
loc_1_0563:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0568:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        jne    loc_1_0585
        mov    eax, 0xe
        jmp    loc_1_05C0
loc_1_0585:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x24], 0
        je     loc_1_05B6
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x24], 0
        mov    dword ptr [0], 0              ; -> obj1:0x00DC
        xor    eax, eax
        jmp    loc_1_05C0
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_05B6:
        mov    eax, 8
        jmp    loc_1_05C0
loc_1_05C0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_05C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x54], 1
        call   sub_1_254F
        mov    dword ptr [ebp - 0x50], eax
        cmp    dword ptr [ebp - 0x50], 0
        je     loc_1_05EF
        mov    eax, dword ptr [ebp - 0x50]
        jmp    loc_1_07ED
loc_1_05EF:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_07E3
        push   -1
        push   0                             ; -> loc_1_00E0
        call   sub_1_25D4
        add    esp, 8
        mov    dword ptr [ebp - 0x4c], eax
        cmp    dword ptr [ebp - 0x4c], -1
        jne    loc_1_0629
        mov    eax, 5
        jmp    loc_1_07ED
        db  0E9h, 014h, 000h, 000h, 000h
loc_1_0629:
        cmp    dword ptr [ebp - 0x4c], -2
        jne    loc_1_063D
        mov    eax, 0xb
        jmp    loc_1_07ED
loc_1_063D:
        mov    eax, dword ptr [ebp - 0x4c]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x34], eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x0060
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25CC
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_0677
        jmp    loc_1_07A4
loc_1_0677:
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25C4
        add    esp, 8
        test   eax, eax
        jne    loc_1_0694
        jmp    loc_1_07A4
loc_1_0694:
        mov    dword ptr [ebp - 0x2c], 0x2580
        mov    byte ptr [ebp - 0x1a], 8
        mov    byte ptr [ebp - 0x19], 0
        mov    dword ptr [ebp - 0x28], 1
        mov    byte ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x48], 0x57
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25BC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_06D6
        jmp    loc_1_07A4
loc_1_06D6:
        cmp    dword ptr [0], 0              ; -> sub_1_0450
        je     loc_1_073E
        mov    eax, 0                        ; -> obj1:0x03C8
        add    eax, 4
        lea    ecx, [ebp - 0x44]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_0450
        add    esp, 8
        test   eax, eax
        jne    loc_1_0723
        mov    dword ptr [ebp - 0x54], 0x36
        jmp    loc_1_07A4
loc_1_0723:
        lea    eax, [ebp - 0x44]
        mov    ecx, 0                        ; -> obj1:0x03C8
        add    ecx, 4
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
loc_1_073E:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x44
        push   eax
        push   0x85
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0765
        jmp    loc_1_07A4
loc_1_0765:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x44], 0x85
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc2], 0
        push   0
        push   0                             ; -> loc_1_0AE7
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_079D
        jmp    loc_1_07A4
loc_1_079D:
        xor    eax, eax
        jmp    loc_1_07ED
loc_1_07A4:
        cmp    dword ptr [0], 0              ; -> sub_1_0458
        je     loc_1_07BE
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_0458
        add    esp, 4
loc_1_07BE:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_25A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x54]
        jmp    loc_1_07ED
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_07E3:
        mov    eax, 4
        jmp    loc_1_07ED
loc_1_07ED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_07F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_091C
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x14
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x1c
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x1c], eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25AC
        add    esp, 0xc
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25FC
        add    esp, 0xc
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        cmp    dword ptr [0], 0              ; -> sub_1_0458
        je     loc_1_08C4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0458
        add    esp, 4
loc_1_08C4:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25A4
        add    esp, 4
        test   eax, eax
        jne    loc_1_08EE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_25EC
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 4], 1
loc_1_08EE:
        call   sub_1_1D30
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x48]
        push   eax
        call   sub_1_25DC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x4c]
        push   eax
        call   sub_1_25DC
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0926
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_091C:
        mov    eax, 6
        jmp    loc_1_0926
loc_1_0926:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_092B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x2c]
        jmp    loc_1_093E
loc_1_093E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0943:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x90
        jmp    loc_1_0958
loc_1_0958:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_095D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        jmp    loc_1_0978
loc_1_0978:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_097D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        jmp    loc_1_0998
loc_1_0998:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_099D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [ecx + 0xc4], al
        xor    eax, eax
        jmp    loc_1_09B9
loc_1_09B9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_09BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> sub_1_0454
        je     loc_1_0AAF
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x44
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0A08
        jmp    loc_1_0AD8
loc_1_0A08:
        push   0
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0A2A
        jmp    loc_1_0AD8
loc_1_0A2A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_0454
        add    esp, 8
        test   eax, eax
        jne    loc_1_0A4D
        jmp    loc_1_0AD8
loc_1_0A4D:
        push   0
        push   0                             ; -> loc_1_0AE7
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0A72
        jmp    loc_1_0AD8
loc_1_0A72:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x44
        push   eax
        push   0x85
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0A9E
        jmp    loc_1_0AD8
loc_1_0A9E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x44], eax
        jmp    loc_1_0AD1
loc_1_0AAF:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_230E
        add    esp, 8
        test   eax, eax
        jne    loc_1_0AD1
        jmp    loc_1_0AD8
loc_1_0AD1:
        xor    eax, eax
        jmp    loc_1_0AE2
loc_1_0AD8:
        mov    eax, 1
        jmp    loc_1_0AE2
loc_1_0AE2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0AE7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0B55
        test   byte ptr [ebp + 0x14], 4
        je     loc_1_0B50
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x48], 0
        jne    loc_1_0B50
        lea    eax, [ebp - 4]
        push   eax
        push   4
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_261C
        add    esp, 0xc
        push   0x30c001
        push   0                             ; -> loc_1_135E
        call   sub_1_2604
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x48], eax
loc_1_0B50:
        jmp    loc_1_0B6D
loc_1_0B55:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1693
        add    esp, 0x10
loc_1_0B6D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0B72:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        je     loc_1_0B9F
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        push   eax
        call   sub_1_2624
        add    esp, 8
        mov    dword ptr [0], 0              ; -> obj1:0x00D8
loc_1_0B9F:
        mov    eax, 1
        jmp    loc_1_0BA9
loc_1_0BA9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0BAE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0BC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_0BC6:
        cmp    dword ptr [ebp + 0x10], 0
        jbe    loc_1_0C14
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0xffff
        xor    eax, ecx
        and    eax, 0xff
        mov    ax, word ptr [eax*2]          ; -> obj1:0x0160
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0xffff
        sar    ecx, 8
        xor    eax, ecx
        mov    word ptr [ebp + 8], ax
        inc    dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0x10]
        jmp    loc_1_0BC6
loc_1_0C14:
        mov    ax, word ptr [ebp + 8]
        jmp    loc_1_0C1D
loc_1_0C1D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0C48:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_0C64
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C48
loc_1_0C64:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_0C6C
loc_1_0C6C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0C71:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0CA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0CD2
loc_1_0CD2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0CD7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0D54:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        je     loc_1_0D83
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_1_0D76
        mov    eax, 1
        jmp    loc_1_0D8A
loc_1_0D76:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0D54
loc_1_0D83:
        xor    eax, eax
        jmp    loc_1_0D8A
loc_1_0D8A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    loc_1_0DC0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_0DC7
loc_1_0DC0:
        xor    eax, eax
        jmp    loc_1_0E20
loc_1_0DC7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0DDC
loc_1_0DD4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_0DDC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_0E16
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    loc_1_0E0A
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_0E11
loc_1_0E0A:
        xor    eax, eax
        jmp    loc_1_0E20
loc_1_0E11:
        jmp    loc_1_0DD4
loc_1_0E16:
        mov    eax, 1
        jmp    loc_1_0E20
loc_1_0E20:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_0E52
        mov    eax, 6
        jmp    loc_1_0EFA
loc_1_0E52:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x28], 2
        je     loc_1_0E85
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x44], 0x85
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x28], 2
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 5
loc_1_0E85:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x2c], 5
        jne    loc_1_0ED8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0FBB
        add    esp, 4
        test   eax, eax
        je     loc_1_0EB2
        mov    eax, 1
        jmp    loc_1_0EFA
loc_1_0EB2:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        and    dword ptr [eax + 0x44], 0xfffffffb
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc2], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x30], eax
        jmp    loc_1_0EF0
loc_1_0ED8:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x14
        push   eax
        call   sub_1_0C71
        add    esp, 8
loc_1_0EF0:
        mov    eax, 2
        jmp    loc_1_0EFA
loc_1_0EFA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0EFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x40], ecx
        jne    loc_1_0F48
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x30]
        push   eax
        push   9
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 8]
        add    esp, 8
        mov    byte ptr [0x3f], 4            ; -> obj1:0x003F
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x30], 0     ; -> obj1:0x0000
loc_1_0F48:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], eax
loc_1_0F53:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x14
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_0FAF
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0x35
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x40], ecx
        jne    loc_1_0FA4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   9
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 8]
        add    esp, 8
loc_1_0FA4:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0F53
loc_1_0FAF:
        xor    eax, eax
        jmp    loc_1_0FB6
loc_1_0FB6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0FBB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x40], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x3d]
        and    eax, 0xff
        dec    eax
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_0FEE
loc_1_0FEB:
        dec    dword ptr [ebp - 0x10]
loc_1_0FEE:
        cmp    dword ptr [ebp - 0x10], 0
        jbe    loc_1_101A
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x3c]
        mov    byte ptr [eax + ecx], 0xff
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x40]
        jmp    loc_1_0FEB
loc_1_101A:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x3c]
        mov    byte ptr [eax + ecx], 0xc0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x40]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x45
        push   eax
        push   0xffff
        call   sub_1_0BC0
        add    esp, 0xc
        mov    word ptr [ebp - 0x1c], ax
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_106A
loc_1_1067:
        inc    dword ptr [ebp - 0x10]
loc_1_106A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_1096
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x45]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    loc_1_1067
loc_1_1096:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_119F
loc_1_10A9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_0BC0
        add    esp, 0xc
        mov    word ptr [ebp - 0x1c], ax
        jmp    loc_1_10E4
loc_1_10DE:
        dec    dword ptr [ebp - 0x10]
        inc    dword ptr [ebp - 4]
loc_1_10E4:
        cmp    dword ptr [ebp - 0x10], 0
        jbe    loc_1_1101
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    loc_1_10DE
loc_1_1101:
        jmp    loc_1_11B8
loc_1_1106:
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_1115
loc_1_1112:
        inc    dword ptr [ebp - 0x18]
loc_1_1115:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x18]
        jbe    loc_1_1195
        mov    eax, dword ptr [ebp - 0x18]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 5]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x18]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 9]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_0BC0
        add    esp, 0xc
        mov    word ptr [ebp - 0x1c], ax
        jmp    loc_1_1173
loc_1_116D:
        dec    dword ptr [ebp - 0x10]
        inc    dword ptr [ebp - 4]
loc_1_1173:
        cmp    dword ptr [ebp - 0x10], 0
        jbe    loc_1_1190
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_1463
        add    esp, 4
        jmp    loc_1_116D
loc_1_1190:
        jmp    loc_1_1112
loc_1_1195:
        jmp    loc_1_11B8
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_119F:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_10A9
        cmp    dword ptr [ebp - 0x20], 1
        je     loc_1_1106
        jmp    loc_1_11B8
loc_1_11B8:
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        not    eax
        mov    word ptr [ebp - 0x1c], ax
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xff
        push   eax
        call   sub_1_1463
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        shr    eax, 8
        and    eax, 0xff
        push   eax
        call   sub_1_1463
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x3c]
        mov    byte ptr [eax + ecx], 0xc1
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x40]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x3e]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_123E
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x3c]
        mov    byte ptr [eax + ecx], 0xff
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x40]
loc_1_123E:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12DD
        add    esp, 4
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x3c]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_262C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_1296
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x40], ecx
        je     loc_1_12B4
loc_1_1296:
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25FC
        add    esp, 0xc
        mov    dword ptr [ebp - 0x14], 1
loc_1_12B4:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x7c
        push   eax
        call   sub_1_1DB4
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xac]
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_12D8
loc_1_12D8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_12DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x4f]
        and    eax, 0xff
        cmp    eax, 7
        jge    loc_1_1359
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x4f]
        and    eax, 0xff
        mov    eax, dword ptr [eax*4]        ; -> loc_1_0360
        mov    dword ptr [ebp - 0xc], eax
        call   sub_1_0070
        add    eax, dword ptr [ebp - 0xc]
        adc    edx, 0
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], edx
loc_1_1320:
        call   sub_1_0070
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_134F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jne    loc_1_1354
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x10], eax
        jb     loc_1_1354
loc_1_134F:
        jmp    loc_1_1359
loc_1_1354:
        jmp    loc_1_1320
loc_1_1359:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_135E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x48], 0
        call   sub_1_137A
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_137A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_13AA
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_13AF
loc_1_13AA:
        jmp    loc_1_145E
loc_1_13AF:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x7c
        push   eax
        call   sub_1_1E15
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 6
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x30]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 8]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        add    ecx, 0x14
        cmp    dword ptr [eax + 0x14], ecx
        jne    loc_1_1412
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x30], 0
        call   sub_1_1647
        jmp    loc_1_145E
loc_1_1412:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x14
        push   eax
        call   sub_1_0CA4
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x35
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x30], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x30]
        push   eax
        call   sub_1_0FBB
        add    esp, 4
        test   eax, eax
        je     loc_1_145E
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xb4]
        call   sub_1_137A
loc_1_145E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1463:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0xc0
        je     loc_1_1497
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0xc1
        je     loc_1_1497
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0x7d
        jne    loc_1_14D7
loc_1_1497:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x40]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x3c]
        mov    byte ptr [eax + ecx], 0x7d
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        xor    al, 0x20
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x40]
        mov    edx, dword ptr [0]            ; -> obj1:0x00D8
        mov    edx, dword ptr [edx + 0x3c]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    dword ptr [eax + 0x40], 2
        jmp    loc_1_14F7
loc_1_14D7:
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0x40]
        mov    edx, dword ptr [0]            ; -> obj1:0x00D8
        mov    edx, dword ptr [edx + 0x3c]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x40]
loc_1_14F7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_14FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_154E
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25F4
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25FC
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xb0]
        call   sub_1_137A
loc_1_154E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_1560:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x1c
        push   eax
        call   sub_1_0C71
        add    esp, 8
        call   sub_1_19AC
        xor    eax, eax
        jmp    loc_1_1593
loc_1_1593:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1598:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_15BA
        mov    eax, 6
        jmp    loc_1_1642
loc_1_15BA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_160E
loc_1_15C5:
        call   sub_1_1647
        jmp    loc_1_1631
loc_1_15CF:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x44], 0x85
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc2], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 0
        jmp    loc_1_1631
        db  0E9h, 02Fh, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_160E:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_15CF
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_15C5
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_1638
        jmp    loc_1_1631
loc_1_1631:
        xor    eax, eax
        jmp    loc_1_1642
loc_1_1638:
        mov    eax, 1
        jmp    loc_1_1642
loc_1_1642:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1647:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc2], 0
        call   sub_1_19AC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x28], 1
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 3
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc3], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x44], 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1693:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_16C5
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc1], 1
        jmp    loc_1_16D1
loc_1_16C5:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc1], 0
loc_1_16D1:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc4], 1
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xa8]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [eax + 0xc3], 1
        test   byte ptr [ebp + 0x14], 0x80
        je     loc_1_1759
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25FC
        add    esp, 0xc
        test   byte ptr [ebp - 4], 8
        je     loc_1_172A
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xa0]
loc_1_172A:
        test   byte ptr [ebp - 4], 2
        je     loc_1_173F
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xa4]
loc_1_173F:
        lea    eax, [ebp - 4]
        push   eax
        push   0x80
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_261C
        add    esp, 0xc
loc_1_1759:
        test   byte ptr [ebp + 0x14], 1
        jne    loc_1_177B
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc1]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_179F
loc_1_177B:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x4c], 0
        jne    loc_1_179F
        push   0
        push   0                             ; -> loc_1_17A4
        call   sub_1_2604
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x4c], eax
loc_1_179F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_17A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0x30], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_17CF
        jmp    loc_1_19A7
loc_1_17CF:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc1]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_17F7
        push   0x17
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 0xc]
        add    esp, 4
loc_1_17F7:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_1821
        lea    eax, [ebp - 0x2c]
        push   eax
        push   0x28
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_2634
        add    esp, 0x10
loc_1_1821:
        cmp    dword ptr [ebp - 0x2c], 0
        jne    loc_1_186A
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_1851
        lea    eax, [ebp - 0x30]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x34]
        push   eax
        call   sub_1_25FC
        add    esp, 0xc
loc_1_1851:
        cmp    dword ptr [ebp - 0x30], 0
        jne    loc_1_1865
        jmp    loc_1_198F
        db  0E9h, 005h, 000h, 000h, 000h
loc_1_1865:
        jmp    loc_1_17F7
loc_1_186A:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0xa8]
        mov    dword ptr [ebp - 0x34], 0
        jmp    loc_1_1884
loc_1_1881:
        inc    dword ptr [ebp - 0x34]
loc_1_1884:
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    dword ptr [ebp - 0x34], eax
        jae    loc_1_198A
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [ebp + eax - 0x28]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [ecx + 0xc0], al
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        call   dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_1_1985
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   0xffff
        call   sub_1_0BC0
        add    esp, 0xc
        and    eax, 0xffff
        cmp    eax, 0xf0b8
        jne    loc_1_197A
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 8]
        sub    eax, 2
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x94]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_1936
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 4]
        add    esp, 8
        test   eax, eax
        jne    loc_1_1936
        jmp    loc_1_19A7
loc_1_1936:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax], 0
        call   sub_1_19AC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x34], 0
        je     loc_1_1975
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax], 0
        jne    loc_1_1975
        push   0
        push   0xa
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x24]
        call   dword ptr [eax + 4]
        add    esp, 8
loc_1_1975:
        jmp    loc_1_1985
loc_1_197A:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x90]
loc_1_1985:
        jmp    loc_1_1881
loc_1_198A:
        jmp    loc_1_17F7
loc_1_198F:
        pushfd 
        cli    
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x4c], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        and    dword ptr [eax + 0x44], 0xfffffffe
        popfd  
loc_1_19A7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_19AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax], 0
        jne    loc_1_1A4B
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        add    ecx, 0x1c
        cmp    dword ptr [eax + 0x1c], ecx
        je     loc_1_1A3F
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x1c
        push   eax
        call   sub_1_0CA4
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xc
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A6E
        jmp    loc_1_1A4B
loc_1_1A3F:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A50
loc_1_1A4B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1A50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 2
        xor    eax, eax
        jmp    loc_1_1A69
loc_1_1A69:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1A6E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        and    eax, 0xff
        cmp    eax, 0xc0
        jne    loc_1_1AA7
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1ABF
loc_1_1AA7:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 2
        xor    eax, eax
        jmp    loc_1_1ABA
loc_1_1ABA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1ABF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1B51
        db  0E9h, 095h, 000h, 000h, 000h
loc_1_1AE5:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A6E
        jmp    loc_1_1B7A
loc_1_1AF6:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1B86
        jmp    loc_1_1B7A
loc_1_1B07:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [0]            ; -> obj1:0x00D8
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 8]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1BFE
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 1
        jmp    loc_1_1B7A
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1B51:
        cmp    dword ptr [ebp - 4], 0x7d
        je     loc_1_1AF6
        cmp    dword ptr [ebp - 4], 0xc0
        je     loc_1_1B7A
        cmp    dword ptr [ebp - 4], 0xc1
        je     loc_1_1AE5
        jmp    loc_1_1B07
loc_1_1B7A:
        xor    eax, eax
        jmp    loc_1_1B81
loc_1_1B81:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1B86:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        and    eax, 0xff
        cmp    eax, 0xc1
        jne    loc_1_1BCF
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x9c]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A6E
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 2
        jmp    loc_1_1BF2
loc_1_1BCF:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        and    eax, 0xff
        xor    al, 0x20
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    byte ptr [ecx + 0xc0], al
        call   sub_1_1CB3
loc_1_1BF2:
        xor    eax, eax
        jmp    loc_1_1BF9
loc_1_1BF9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1BFE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1C7E
loc_1_1C1F:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1ABF
        jmp    loc_1_1CA7
loc_1_1C3C:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A6E
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x2c], 3
        mov    eax, 1
        jmp    loc_1_1CAE
loc_1_1C5E:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1B86
        jmp    loc_1_1CA7
loc_1_1C6F:
        call   sub_1_1CB3
        jmp    loc_1_1CA7
        db  0E9h, 029h, 000h, 000h, 000h
loc_1_1C7E:
        cmp    dword ptr [ebp - 4], 0x7d
        je     loc_1_1C5E
        cmp    dword ptr [ebp - 4], 0xc0
        je     loc_1_1C1F
        cmp    dword ptr [ebp - 4], 0xc1
        je     loc_1_1C3C
        jmp    loc_1_1C6F
loc_1_1CA7:
        xor    eax, eax
        jmp    loc_1_1CAE
loc_1_1CAE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1CB3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [eax + 8], ecx
        jb     loc_1_1CEC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 0x98]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1A6E
        jmp    loc_1_1D20
loc_1_1CEC:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    al, byte ptr [eax + 0xc0]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [0]            ; -> obj1:0x00D8
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        inc    dword ptr [eax + 8]
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_1BFE
loc_1_1D20:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_1D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_1D39:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        add    ecx, 0x74
        cmp    dword ptr [eax + 0x74], ecx
        je     loc_1_1D75
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xc
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1E15
        add    esp, 4
        jmp    loc_1_1D39
loc_1_1D75:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1D7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        call   dword ptr [eax + 4]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1DB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E15
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        push   0                             ; -> loc_1_1D7A
        call   sub_1_263C
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1E10
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x74
        push   eax
        call   sub_1_0C71
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        jmp    loc_1_1E10
loc_1_1E10:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1E15:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 0
        je     loc_1_1E4D
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc
        push   eax
        call   sub_1_0CD7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_2658
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
loc_1_1E4D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_1_1E60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        jne    loc_1_1EB2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2390
        add    esp, 4
        call   sub_1_2001
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0x0058
        add    eax, 0xc5
        push   eax
        call   sub_1_2668
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj1:0x00D8
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        jne    loc_1_1EB2
        xor    eax, eax
        jmp    loc_1_1EC1
loc_1_1EB2:
        call   sub_1_1EC6
        mov    eax, 0                        ; -> obj1:0x03C8
        jmp    loc_1_1EC1
loc_1_1EC1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1EC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        je     loc_1_1FFC
        cmp    dword ptr [0], 0              ; -> obj1:0x00DC
        jne    loc_1_1FFC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0058
        add    eax, 0xc5
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_219D
        add    esp, 8
        add    dword ptr [ebp - 4], 0xc5
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x14
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x1c
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x74
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    eax, dword ptr [eax + 0x78]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x74], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x80], 0     ; -> loc_1_14FC
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [eax + 0x7c], 0x3e8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0060
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x3c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x005C
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0050
        add    eax, 4
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x58], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x54], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0x00D8
        mov    dword ptr [ecx + 0x70], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        add    eax, 0x50
        push   eax
        call   sub_1_1560
        add    esp, 4
        mov    dword ptr [0], 1              ; -> obj1:0x00DC
loc_1_1FFC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2001:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x10]         ; -> obj1:0x03D8
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    ax, word ptr [eax*2]          ; -> obj1:0x03E8
        and    eax, 0xffff
        mov    dword ptr [0], eax            ; -> obj1:0x0050
        mov    eax, dword ptr [0x14]         ; -> obj1:0x03DC
        push   eax
        call   sub_1_213D
        add    esp, 4
        inc    eax
        mov    byte ptr [0], al              ; -> obj1:0x0054
        mov    eax, dword ptr [4]            ; -> obj1:0x03CC
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [8]            ; -> obj1:0x03D0
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx*8]    ; -> obj1:0x0408
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0xc]          ; -> obj1:0x03D4
        push   eax
        call   sub_1_213D
        add    esp, 4
        mov    al, byte ptr [eax]            ; -> obj1:0x03F8
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]            ; -> loc_1_0400
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0050
        shl    eax, 4
        mov    ecx, 0x64
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [0]            ; -> obj1:0x0050
        add    ecx, eax
        add    ecx, dword ptr [ebp - 0xc]
        add    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0xe
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj1:0x0054
        imul   ecx, eax
        mov    dword ptr [0], ecx            ; -> obj1:0x0060
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj1:0x0054
        cmp    eax, 2
        jge    loc_1_20F3
        mov    eax, dword ptr [0]            ; -> obj1:0x0050
        add    dword ptr [0], eax            ; -> obj1:0x0060
loc_1_20F3:
        mov    eax, dword ptr [0]            ; -> obj1:0x0060
        cmp    eax, 0x1a80
        jb     loc_1_2108
        mov    eax, 0x1a80
loc_1_2108:
        mov    dword ptr [0], eax            ; -> obj1:0x0060
        xor    eax, eax
        mov    ax, word ptr [0x1c]           ; -> obj1:0x03E4
        add    eax, eax
        add    eax, 0x3e
        mov    dword ptr [0], eax            ; -> obj1:0x005C
        mov    eax, dword ptr [0]            ; -> obj1:0x005C
        add    eax, dword ptr [0]            ; -> obj1:0x0060
        add    eax, dword ptr [0]            ; -> obj1:0x0050
        add    eax, 4
        mov    dword ptr [0], eax            ; -> obj1:0x0058
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_213D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        and    dword ptr [ebp + 8], 0xffff
        jne    loc_1_215A
        xor    eax, eax
        jmp    loc_1_2198
loc_1_215A:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2169
loc_1_2166:
        inc    dword ptr [ebp - 4]
loc_1_2169:
        cmp    dword ptr [ebp - 4], 0xf
        jae    loc_1_218B
        shl    dword ptr [ebp + 8], 1
        test   byte ptr [ebp + 0xa], 1
        je     loc_1_2186
        jmp    loc_1_218B
loc_1_2186:
        jmp    loc_1_2166
loc_1_218B:
        mov    eax, 0xf
        sub    eax, dword ptr [ebp - 4]
        jmp    loc_1_2198
loc_1_2198:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_219D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_21B4
loc_1_21B1:
        dec    dword ptr [ebp + 0xc]
loc_1_21B4:
        cmp    dword ptr [ebp + 0xc], 0
        jbe    loc_1_21CC
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 4]
        jmp    loc_1_21B1
loc_1_21CC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_21E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_21E6:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_1_2228
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     loc_1_2228
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, byte ptr [ecx]
        cmp    eax, ecx
        je     loc_1_221D
        xor    eax, eax
        jmp    loc_1_2252
loc_1_221D:
        inc    dword ptr [ebp + 8]
        inc    dword ptr [ebp + 0xc]
        jmp    loc_1_21E6
loc_1_2228:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, byte ptr [ecx]
        cmp    eax, ecx
        jne    loc_1_224B
        mov    eax, 1
        jmp    loc_1_2252
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_224B:
        xor    eax, eax
        jmp    loc_1_2252
loc_1_2252:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2257:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_226F
loc_1_226C:
        inc    dword ptr [ebp - 4]
loc_1_226F:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [0], eax            ; -> loc_1_045C
        jbe    loc_1_22EB
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        mov    ecx, dword ptr [0]            ; -> loc_1_0460
        mov    eax, dword ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21E0
        add    esp, 8
        test   eax, eax
        je     loc_1_22E6
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        mov    ecx, dword ptr [0]            ; -> loc_1_0460
        mov    eax, dword ptr [eax + ecx + 4]
        mov    dword ptr [0], eax            ; -> sub_1_0450
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        mov    ecx, dword ptr [0]            ; -> loc_1_0460
        mov    eax, dword ptr [eax + ecx + 8]
        mov    dword ptr [0], eax            ; -> sub_1_0454
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        mov    ecx, dword ptr [0]            ; -> loc_1_0460
        mov    eax, dword ptr [eax + ecx + 0xc]
        mov    dword ptr [0], eax            ; -> sub_1_0458
        jmp    loc_1_2309
loc_1_22E6:
        jmp    loc_1_226C
loc_1_22EB:
        mov    dword ptr [0], 0              ; -> sub_1_0450
        mov    dword ptr [0], 0              ; -> sub_1_0454
        mov    dword ptr [0], 0              ; -> sub_1_0458
loc_1_2309:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_230E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25C4
        add    esp, 8
        test   eax, eax
        jne    loc_1_2336
        xor    eax, eax
        jmp    loc_1_237C
loc_1_2336:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x2c], eax
        mov    byte ptr [ebp - 0x1a], 8
        mov    byte ptr [ebp - 0x19], 0
        mov    byte ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x34], 0x55
        mov    eax, dword ptr [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_25BC
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_2372
        xor    eax, eax
        jmp    loc_1_237C
loc_1_2372:
        mov    eax, 1
        jmp    loc_1_237C
loc_1_237C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_2390:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x24], 4
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> loc_1_0464
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_23F4
        cmp    dword ptr [ebp - 0x28], 4
        jne    loc_1_23F4
        cmp    dword ptr [ebp - 0x2c], 0
        jbe    loc_1_23F4
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    eax, 0x800
        jb     loc_1_23EE
        mov    eax, 0x800
loc_1_23EE:
        mov    word ptr [0x1c], ax           ; -> obj1:0x03E4
loc_1_23F4:
        mov    dword ptr [ebp - 0x24], 4
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> obj1:0x0470
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_2441
        cmp    dword ptr [ebp - 0x28], 4
        jne    loc_1_2441
        cmp    dword ptr [ebp - 0x2c], 0
        jbe    loc_1_2441
        mov    eax, dword ptr [ebp - 0x2c]
        and    eax, 0x3f
        mov    dword ptr [4], eax            ; -> obj1:0x03CC
loc_1_2441:
        mov    dword ptr [ebp - 0x24], 0x20
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> obj1:0x0478
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_2485
        cmp    dword ptr [ebp - 0x28], 1
        jne    loc_1_2485
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_2257
        add    esp, 4
loc_1_2485:
        mov    dword ptr [ebp - 0x24], 4
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> loc_1_0484
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_24D4
        cmp    dword ptr [ebp - 0x28], 4
        jne    loc_1_24D4
        cmp    dword ptr [ebp - 0x2c], 0
        jbe    loc_1_24D4
        mov    eax, dword ptr [ebp - 0x2c]
        and    eax, 0xff
        mov    dword ptr [8], eax            ; -> obj1:0x03D0
loc_1_24D4:
        mov    dword ptr [ebp - 0x24], 4
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> obj1:0x0490
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_2523
        cmp    dword ptr [ebp - 0x28], 4
        jne    loc_1_2523
        cmp    dword ptr [ebp - 0x2c], 0
        jbe    loc_1_2523
        mov    eax, dword ptr [ebp - 0x2c]
        and    eax, 0xff
        mov    dword ptr [0xc], eax          ; -> obj1:0x03D4
loc_1_2523:
        mov    dword ptr [ebp - 0x24], 0x80
        lea    eax, [ebp - 0x24]
        push   eax
        push   0                             ; -> loc_1_00E0
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        push   0                             ; -> obj1:0x04A0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2670
        add    esp, 0x18
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_254F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x00D8
        jne    loc_1_256C
        mov    eax, 0xe
        jmp    loc_1_258C
loc_1_256C:
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_1_2585
        mov    eax, 8
        jmp    loc_1_258C
loc_1_2585:
        xor    eax, eax
        jmp    loc_1_258C
loc_1_258C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2591:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        jmp    loc_1_259E
loc_1_259E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  0CCh
sub_1_25A4:
        VxDCall  BIOS.Service_8009h              ; cd2009802b00
        int3   
        int3   
sub_1_25AC:
        VxDCall  BIOS.Service_8014h              ; cd2014802b00
        int3   
        int3   
sub_1_25B4:
        VxDCall  BIOS.Service_8010h              ; cd2010802b00
        int3   
        int3   
sub_1_25BC:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        int3   
        int3   
sub_1_25C4:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        int3   
        int3   
sub_1_25CC:
        VxDCall  BIOS.Service_8007h              ; cd2007802b00
        int3   
        int3   
sub_1_25D4:
        VxDCall  BIOS.Service_8004h              ; cd2004802b00
        int3   
        int3   
sub_1_25DC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        pop    esi
        ret    4
        db  0CCh
sub_1_25EC:
        VxDCall  BIOS.Service_8015h              ; cd2015802b00
        int3   
        int3   
sub_1_25F4:
        VxDCall  BIOS.Service_800Fh              ; cd200f802b00
        int3   
        int3   
sub_1_25FC:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        int3   
        int3   
sub_1_2604:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_1_261C:
        VxDCall  BIOS.Service_8011h              ; cd2011802b00
        int3   
        int3   
sub_1_2624:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_1_262C:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        int3   
        int3   
sub_1_2634:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        int3   
        int3   
sub_1_263C:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  0CCh, 0CCh, 0CCh
sub_1_2658:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  0CCh
sub_1_2668:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_1_2670:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x42  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_0380
        call   sub_1_04DF
loc_2_0013:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0035
        push   0                             ; -> loc_1_045C
        push   0                             ; -> loc_1_0460
        call   sub_1_04E5
loc_2_0032:
        add    esp, 8
loc_2_0035:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_003D
loc_2_003D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
VxD__LTEXT ENDS
