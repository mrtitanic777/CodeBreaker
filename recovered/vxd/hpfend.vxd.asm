; HPFEND -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 8027 instructions recovered by recursive descent

Declare_Virtual_Device HPFEND, 3, 10, HPFEND_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xd46  R-X
loc_1_0000:
        mov    al, byte ptr [0x668]          ; -> obj1:0x0668
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x668]              ; -> obj1:0x0668
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_4_1D30
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
HPFEND_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0038
        jmp    dword ptr [eax*4 + 0x3a]      ; -> obj1:0x003A
loc_1_0038:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0000
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0000
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_00B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00E0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_0180
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_0120
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_0115
        cmp    al, 0xff
        jne    loc_1_0115
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_0115
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_0187
loc_1_0115:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0187
        db  08Bh, 0FFh
loc_1_0120:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_0170
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0146
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_014D
loc_1_0146:
        mov    dword ptr [ebp - 0x10], 1
loc_1_014D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0160
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_018A
        db  08Dh, 064h, 024h, 000h
loc_1_0160:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0170:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0180:
        mov    dword ptr [ebp - 4], 0x20
loc_1_0187:
        mov    esi, dword ptr [ebp + 8]
loc_1_018A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0281
loc_1_0195:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_01ED
        test   al, 2
        je     loc_1_01D9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_1_01D9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_01ED
loc_1_01D9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_01E4
        test   bl, 4
        jne    loc_1_01ED
loc_1_01E4:
        test   bl, 0x20
        je     loc_1_0276
loc_1_01ED:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_0462
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0276
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_022F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_022F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0240
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0246
        db  08Bh, 0FFh
loc_1_0240:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0246:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0255
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
loc_1_0255:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04DC
        mov    edi, ebx
        jmp    loc_1_0279
loc_1_0276:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0279:
        or     edi, edi
        jne    loc_1_0195
loc_1_0281:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_02A0
        pop    ebp
        ret    4
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_031E
loc_1_02B0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0317
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0317
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_02DC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_02DC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_02F0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_02F6
        db  005h, 000h, 000h, 000h, 000h
loc_1_02F0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_02F6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04DC
        mov    edi, ebx
        jmp    loc_1_031A
loc_1_0317:
        mov    edi, dword ptr [edi + 0x10]
loc_1_031A:
        or     edi, edi
        jne    loc_1_02B0
loc_1_031E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0330:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_0345
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0345:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_0381
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0372
        cmp    bl, 0xff
        jne    loc_1_0372
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_0372
        test   al, 8
        je     loc_1_036B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_036B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0372:
        test   al, 4
        jne    loc_1_037A
        test   al, 2
        je     loc_1_0399
loc_1_037A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0381:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0399
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_0399
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0399:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_03C9:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        cld    
        rep outsd dx, dword ptr [esi]
        pop    esi
        ret    0xc
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch
        db  05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_0406:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        cld    
        rep insd dword ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_041A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0420:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0426:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_042C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0432:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0438:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_043E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0444:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_044A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_0456:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0462:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0468:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_046E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_0474:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_0486:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_04B2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_04BE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_04C4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_04CA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_04D0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_04D6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_04DC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0500:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_0506:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_050C:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_0512:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_051E:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0524:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0548:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_054E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0554:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_055A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0560:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
sub_1_0584:
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
sub_1_058A:
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
sub_1_0590:
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
loc_1_0596:
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
sub_1_059C:
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
sub_1_05A2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_05A8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_05BC:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
sub_1_05DC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_05FC
        or     ecx, ecx
        je     loc_1_0612
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0610
loc_1_05FC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0610
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0610:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0612:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'HPFEND  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  HPFEND_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 04Dh, 075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0680:
        dec    ebp
        popal  
        js     0x6d1
        jne    loc_1_06F2
        je     0x6f1
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_068D
loc_1_068D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], ch
        popal  
        jb     0x6fc
        jae    0x70c
        add    byte ptr [eax], al
        db  000h
loc_1_069C:
        imul   ebp, dword ptr [edi + 0x61], 0x65726464
        jae    0x718
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], ch
        je     0x711
        jb     0x720
        jne    0x720
        je     loc_1_06B2
loc_1_06B2:
        add    byte ptr [eax], al
loc_1_06B4:
        imul   ebp, dword ptr [esi + 0x74], 0x75727265
        jo     0x731
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    loc_1_0712
        jne    0x733
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    loc_1_071E
        jne    0x73f
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0757
        dec    ebp
        popal  
        jo     0x752
        inc    edx
        popal  
        jae    loc_1_074D
        inc    ecx
        jb     0x752
        jae    0x762
        add    byte ptr [ebp + 0x65], cl
loc_1_06F2:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_076F
        dec    ebp
        popal  
        jo     0x76a
        inc    edx
        popal  
        jae    loc_1_0765
        inc    ecx
        jb     0x76a
        jae    0x77a
        add    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_0781
        jb     0x764
        jns    0x782
loc_1_0712:
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_078D
        jb     0x770
        jns    0x78e
loc_1_071E:
        add    byte ptr gs:[eax + 0x72], dl
        imul   ebp, dword ptr [edi + 0x72], 0x797469
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x72], dl
        imul   ebp, dword ptr [edi + 0x72], 0x797469
        add    byte ptr [eax], al
        db  000h
loc_1_0738:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
loc_1_074D:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
loc_1_0757:
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
loc_1_0765:
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
loc_1_076F:
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
loc_1_0781:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
loc_1_078D:
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_07B4:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_0810:
        dec    eax
        push   eax
        and    byte ptr [ecx], dh
        xor    byte ptr [edi], ch
        xor    dword ptr [eax], esi
        xor    byte ptr [esi + 0x47], dl
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_0899
        jb     loc_1_0856
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 046h, 045h, 04Eh, 044h, 000h, 000h
loc_1_0838:
        dec    eax
        push   eax
        inc    esi
        inc    ebp
        dec    esi
        inc    esp
        add    byte ptr [eax], al
loc_1_0840:
        dec    eax
        push   eax
        das    
        push   esi
        inc    edi
        and    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
loc_1_084C:
        dec    eax
        push   eax
        das    
        push   esi
        inc    edi
        and    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
loc_1_0856:
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x8d6
        cmp    eax, 0x3f3f7830
        aas    
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [eax + 0x50], cl
        das    
        push   esi
        inc    edi
        and    byte ptr [eax], ah
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
loc_1_0874:
        dec    eax
        push   eax
        das    
        push   esi
        inc    edi
        and    byte ptr [eax], ah
        dec    ecx
        push   ebx
        inc    ecx
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x8fe
        cmp    eax, 0x3f3f7830
        aas    
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [esi + 0x4f], cl
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0899:
        popal  
        jo     loc_1_0911
        jb     loc_1_08C0
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_08C4
        jo     loc_1_0918
        jae    0x90e
        outsb  dx, byte ptr [esi]
        je     loc_1_08AC
loc_1_08AC:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_1_0918
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x49], 0x61204f2f
loc_1_08C0:
        jb     0x929
loc_1_08C4:
        jae    0x939
        sub    byte ptr [ebp + 0x73], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_1_0938
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x69], 0x7265746e
        jb     loc_1_0957
        jo     0x958
        and    byte ptr [esi + 0x65], dh
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     loc_1_095C
        jb     0x96b
        jne    0x96b
        je     0x91d
        jbe    loc_1_0964
        arpl   word ptr [edi + ebp*2 + 0x72], si
        and    byte ptr [ebp + 0x69], ch
        jae    loc_1_0975
        popal  
        je     0x96e
        push   0
loc_1_0910:
        inc    ebp
loc_1_0911:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
loc_1_0918:
        popal  
        jo     0x990
        jb     0x93f
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x943
        jae    0x99a
        jo     loc_1_0997
        outsd  dx, dword ptr [esi]
        jb     0x99e
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0930:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
loc_1_0938:
        je     0x9a9
        jb     0x9a1
        and    byte ptr fs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     loc_1_09B2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x6f], ah
        jb     loc_1_09C2
        jne    loc_1_09C2
        je     0x9b9
        add    byte ptr fs:[eax], al
loc_1_0957:
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
loc_1_095C:
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebp
        inc    ebp
        push   eax
        push   edx
        dec    edi
loc_1_0964:
        dec    ebp
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0 ; -> loc_1_0840
loc_1_0975:
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_084C
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0868
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0874
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0890
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_08AC
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x08CC
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x08EC
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0910
        add    byte ptr [eax], al
loc_1_0997:
        add    byte ptr [eax], al            ; -> loc_1_0930
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0958
        add    byte ptr [eax], al
        db  000h
loc_1_09A0:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        popal  
        bound  esp, qword ptr [ebx + 0x64]
        add    byte ptr gs:[eax], al
loc_1_09B2:
        add    byte ptr [eax], al
        push   ebp
        jae    0xa20
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + si], bh
        xor    byte ptr [edx], dh
        xor    esp, dword ptr cs:[eax]
        jo     0xa31
loc_1_09C2:
        jb     loc_1_0A38
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebp
        jae    0xa34
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + si], bh
        xor    byte ptr [edx], dh
        xor    dword ptr cs:[edx], esi
        and    byte ptr [eax + 0x6f], dh
        jb     0xa4d
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax], bh
        xor    byte ptr [edx], dh
        xor    esp, dword ptr cs:[eax]
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        db  064h, 065h, 000h
loc_1_09E8:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xa60
        and    byte ptr [eax + 0x65], dh
        jb     0xa5c
        outsd  dx, dword ptr [esi]
        jb     0xa66
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [eax + 0x6f], dh
        jb     0xa76
        and    byte ptr [ecx + 0x75], ah
        je     0xa76
        sub    eax, 0x736e6573
        add    byte ptr gs:[eax], al
        add    byte ptr [ecx + 0x75], al
        je     0xa83
        sub    eax, 0x736e6573
        and    byte ptr gs:[ecx + eax*2 + 0x4e], cl
        and    byte ptr [eax + 0x6f], dh
        jb     0xa97
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x6556202e
        jb     loc_1_0A99
        jns    loc_1_0A53
        arpl   word ptr [ecx + 0x62], sp
        insb   byte ptr es:[edi], dx
        db  069h
loc_1_0A38:
        outsb  dx, byte ptr [esi]
        db  067h, 02Eh, 000h
loc_1_0A3C:
        push   esp
        jb     0xaa0
        imul   ebp, dword ptr [esi + 0x69], 0x6620676e
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x6854202e
        and    byte ptr gs:[ebx + 0x61], ah
loc_1_0A53:
        bound  ebp, qword ptr [ebp + 0x20]
        insd   dword ptr es:[edi], dx
        popal  
        jns    0xa7b
        bound  esp, qword ptr [ebp + 0x20]
        bound  esp, qword ptr [ecx + 0x64]
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        arpl   word ptr [edi + 0x6e], bp
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ebp + 0x64], si
        add    byte ptr cs:[eax], al
loc_1_0A74:
        push   esp
        jb     0xad8
        imul   ebp, dword ptr [esi + 0x69], 0x6620676e
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x6854202e
        and    byte ptr gs:[ebx + 0x61], ah
        bound  ebp, qword ptr [ebp + 0x20]
        push   0x74207361
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        and    byte ptr [ebp + 0x75], ch
loc_1_0A99:
        arpl   word ptr [eax + 0x20], bp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [ebx + 0x65], 0x5400002e
        jb     0xb08
        imul   ebp, dword ptr [esi + 0x69], 0x6620676e
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x65756420
        and    byte ptr [edi + ebp*2 + 0x20], dh
        ja     loc_1_0B2F
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x72], ah
        popal  
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[ecx + edi*2 + 0x70], dh
        and    byte ptr gs:[edi + 0x72], ch
        and    byte ptr [ebx + 0x65], dh
        arpl   word ptr [ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x20], 0x6c6f6976
        popal  
        je     loc_1_0B48
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[eax], al
loc_1_0AE4:
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    loc_1_0B5D
        jo     loc_1_0B5A
        outsd  dx, dword ptr [esi]
        jb     0xb61
        and    byte ptr fs:[eax + 0x72], dh
        sub    eax, 0x656c6572
        popal  
        jae    loc_1_0B60
        and    byte ptr [eax + 0x50], cl
        and    byte ptr [ecx + 0x64], ah
        popal  
        jo     0xb78
        jb     loc_1_0B27
        outsw  dx, word ptr [esi]
        jne    loc_1_0B79
        and    byte ptr cs:[eax + 0x6c], dl
        popal  
        jae    loc_1_0B79
        and    byte ptr [edx + 0x65], dh
        je     0xb8e
        jb     0xb89
        and    byte ptr [edi + ebp*2 + 0x20], dh
        dec    eax
        push   eax
        add    byte ptr cs:[eax], al
loc_1_0B24:
        dec    ebp
        insd   dword ptr es:[edi], dx
loc_1_0B27:
        outsd  dx, dword ptr [esi]
        jb     0xba3
        and    byte ptr [ebp + 0x61], ch
        jo     loc_1_0B4F
loc_1_0B2F:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xb53
        arpl   word ptr [edi + 0x6e], bp
        imul   sp, word ptr [edi + 0x75], 0x6572
        cmp    esp, dword ptr fs:[eax]
        jne    0xbb4
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
loc_1_0B48:
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    loc_1_0BB5
loc_1_0B4F:
        jb     0xbc5
        add    byte ptr [eax], al
loc_1_0B54:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0BD3
loc_1_0B5A:
        and    byte ptr [ebp + 0x61], ch
loc_1_0B5D:
        jo     0xb7f
        popal  
loc_1_0B60:
        jb     loc_1_0BC9
        jae    loc_1_0BD9
        and    byte ptr [edi + ebp*2 + 0x65], ah
        jae    loc_1_0B8C
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xb90
        insd   dword ptr es:[edi], dx
        popal  
        je     loc_1_0BD7
        push   0x61646120
loc_1_0B79:
        jo     loc_1_0BEF
        jb     0xbb9
        and    byte ptr [ebp + 0x73], dh
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
loc_1_0B8C:
        outsb  dx, byte ptr [esi]
        jae    0xbf5
        jb     loc_1_0C05
        add    byte ptr [eax], al
loc_1_0B94:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xc13
        and    byte ptr [ebp + 0x61], ch
        jo     0xbbf
        jb     loc_1_0C02
        outsb  dx, byte ptr [esi]
        and    byte ptr gs:[di + 0x75], ch
        jae    0xc1d
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [eax], bh
        xor    dword ptr [ecx], edi
        xor    ah, byte ptr [eax]
        bound  edi, qword ptr [ecx + 0x74]
loc_1_0BB5:
        jae    0xbf3
        and    byte ptr [ebp + 0x73], dh
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0xc2f
loc_1_0BC9:
        jb     0xc3f
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD0:
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
loc_1_0BD3:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xbf7
loc_1_0BD7:
        insd   dword ptr es:[edi], dx
        popal  
loc_1_0BD9:
        jo     0xbfb
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0C5A
        and    byte ptr [ecx + 0x6e], ch
        je     0xc55
        and    byte ptr [ebx + 0x65], ch
        jb     0xc59
        insb   byte ptr es:[edi], dx
        cmp    esp, dword ptr [eax]
loc_1_0BEF:
        jne    loc_1_0C64
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    loc_1_0C65
        jb     0xc75
loc_1_0C02:
        add    byte ptr [eax], al
loc_1_0C04:
        dec    ebp
loc_1_0C05:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0C83
        and    byte ptr [ebp + 0x61], ch
        jo     0xc2f
        arpl   word ptr [edi + 0x6e], bp
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x7375203b
        imul   ebp, dword ptr [esi + 0x67], 0x4f2f4920
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0xc8f
        jb     loc_1_0C9F
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C30:
        inc    esi
        inc    ecx
        push   esp
        inc    ecx
        dec    esp
        cmp    ah, byte ptr [eax]
        inc    edx
        popal  
        and    byte ptr fs:[eax + 0x2f], cl
        push   edi
        and    byte ptr [ecx + 0x6e], ch
        imul   esi, dword ptr [eax + 0x63], 0x65646f
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x61], al
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     0xcbd
        jae    loc_1_0CCE
loc_1_0C5A:
        popal  
        jb     0xcd1
        add    byte ptr cs:[eax], al
loc_1_0C60:
        inc    ebp
        push   edx
        push   edx
        dec    edi
loc_1_0C64:
        push   edx
loc_1_0C65:
        cmp    ah, byte ptr [eax]
        dec    ecx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x52], cl
        push   ecx
        and    byte ptr [eax], ch
        xor    byte ptr [eax + 0x2e], bh
        sub    dword ptr cs:[eax], eax
loc_1_0C80:
        inc    ebp
        push   edx
        push   edx
loc_1_0C83:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [ecx + 0x64], ah
        jb     loc_1_0CFD
        jae    loc_1_0D0D
        and    byte ptr [eax], ch
        xor    byte ptr [eax + 0x2e], bh
loc_1_0C9F:
        sub    dword ptr cs:[eax], eax
loc_1_0CA8:
        dec    esi
        dec    edi
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popal  
        jo     0xd29
        jb     loc_1_0CD8
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_1_0CDC
        jo     loc_1_0D30
        jae    loc_1_0D26
        outsb  dx, byte ptr [esi]
        je     loc_1_0CC4
loc_1_0CC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CCC:
        add    byte ptr [eax], al
loc_1_0CCE:
        add    byte ptr [eax], al
loc_1_0CD0:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
loc_1_0CD8:
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
loc_1_0CDC:
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0CE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_0CFD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D0D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0D26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D2E:
        add    byte ptr [eax], al
loc_1_0D30:
        add    byte ptr [eax], al
loc_1_0D32:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D36:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D3A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D3E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0xce8]        ; -> obj1:0x0CE8
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_04D6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_004E
        mov    esi, 2
loc_2_004E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04D6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_04D6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_00B1
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_00B1:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x18]
        add    ecx, 0x2c
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], dl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_04DC
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_04DC
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_04DC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0189:
        mov    eax, dword ptr [0xce8]        ; -> obj1:0x0CE8
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D6
        or     eax, eax
        je     loc_2_01A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_01A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_01B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_01C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_01B0
        jmp    loc_2_01D2
loc_2_01C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_01D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_01E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_01E8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_05E0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0269
        cmp    ebx, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    ebx, eax
loc_2_0280:
        or     ebx, ebx
        je     loc_2_028C
        cmp    ebx, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04DC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    ebx, 0xc001000e
loc_2_02EB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0300:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_05DC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_05DC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0384
        sub    eax, eax
        nop    
loc_2_0360:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_0360
loc_2_0384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_03EF
loc_2_0391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_03E4
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_05DC
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_05DC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_03E7
loc_2_03E4:
        inc    dword ptr [ebp - 4]
loc_2_03E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0391
loc_2_03EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_04DA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_2_043A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_04C8
loc_2_043A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0514
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        call   sub_1_05DC
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_05DC
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_04C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_0565
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_0552
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_0552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0559
loc_2_0514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05DC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05DC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0552:
        mov    dword ptr [ebp - 8], 1
loc_2_0559:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_04F0
loc_2_0565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_05D0
loc_2_056B:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_05D2
        cmp    edi, 0x103
        je     loc_2_05D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05DC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05DC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_05D0:
        sub    edi, edi
loc_2_05D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        mov    dword ptr [esi + 0xc], 0
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_0651
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0651:
        sub    eax, eax
loc_2_0653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0660:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_0692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_0687:
        test   dword ptr [esi], edx
        je     loc_2_068C
        inc    eax
loc_2_068C:
        add    esi, 4
        dec    edi
        jne    loc_2_0687
loc_2_0692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [edi + 0x24], 0
        jbe    loc_2_071C
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_1_05DC
        add    ebx, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [edi + 0x24], eax
        ja     loc_2_06E0
loc_2_071C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0736:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0760:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_0790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0790:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05DC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_07C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     loc_2_0859
        dec    esi
        cmp    esi, edx
        jb     loc_2_0859
        nop    
loc_2_07E0:
        cmp    edx, ecx
        jb     loc_2_0859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jbe    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0837
        mov    dword ptr [ebp - 4], 0
loc_2_0837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0869
        jle    loc_2_0848
        or     edx, edx
        je     loc_2_0859
        lea    esi, [edx - 1]
        jmp    loc_2_084B
loc_2_0848:
        lea    ecx, [edx + 1]
loc_2_084B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_07E0
loc_2_0859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0869:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x46a0  R-X
        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0020:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        jne    loc_4_004C
        mov    eax, dword ptr [0]            ; -> obj1:0x082C
        mov    ecx, dword ptr [ebp + 0x10]
        push   eax
        lea    edx, [ebp - 4]
        push   3
        push   ecx
        push   edx
        call   sub_1_04D6
loc_4_0043:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_004C:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        mov    eax, dword ptr [edx + 0x205]
        lea    ecx, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_05A2
loc_4_0068:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0080:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0xf
        sub    esp, 0x3c
        lea    edx, [ebp - 0x3c]
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 0x3c]
        rep stosd dword ptr es:[edi], eax
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        cmp    byte ptr [eax], cl
        je     loc_4_00B3
loc_4_00A1:
        cmp    ecx, 0x3c
        jae    loc_4_00B3
        mov    bl, byte ptr [eax + ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax + ecx], 0
        jne    loc_4_00A1
loc_4_00B3:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x18]
        push   ecx
        mov    eax, dword ptr [ebp - 0x1c]
        push   edx
        mov    ecx, dword ptr [ebp - 0x20]
        push   eax
        mov    edx, dword ptr [ebp - 0x24]
        push   ecx
        mov    eax, dword ptr [ebp - 0x28]
        push   edx
        mov    ecx, dword ptr [ebp - 0x2c]
        push   eax
        mov    edx, dword ptr [ebp - 0x30]
        push   ecx
        mov    eax, dword ptr [ebp - 0x34]
        push   edx
        mov    ecx, dword ptr [ebp - 0x38]
        push   eax
        mov    edx, dword ptr [ebp - 0x3c]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   0x12
        push   0xaa000003
        mov    ecx, dword ptr [eax + 0x205]
        push   ecx
        call   sub_1_05A8
loc_4_0111:
        add    esp, 0x54
        int3   
        mov    si, word ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], si
        je     loc_4_0135
        push   eax
        call   sub_4_2FD0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        mov    word ptr [ecx], si
        or     dword ptr [eax + 0x47], 2
loc_4_0135:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 068h, 0E8h, 003h, 000h, 000h, 0E8h
        dd  sub_1_04D0               ; fixup
        db  0FFh, 005h
        dd  loc_1_0D3E               ; fixup
        db  0A1h
        dd  loc_1_0D3E               ; fixup
        db  0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        push   esi
        mov    ebx, dword ptr [ebp + 0x14]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x18]
        push   ebx
        mov    edi, dword ptr [esi + 0x4b]
        and    ecx, 0xffff
        push   eax
        lea    edx, [esi + 0x53]
        mov    dword ptr [esi + 0x63], ebx
        mov    dword ptr [esi + 0x5f], ecx
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 0x4f]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0512
loc_4_01A7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_01BB
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_01BB:
        mov    ax, word ptr [ebp + 0x18]
        dec    ax
        mov    ecx, dword ptr [esi + 0x53]
        cmp    ax, 1
        sbb    al, al
        lea    edx, [ebp - 4]
        inc    al
        push   eax
        push   ebx
        push   0
        push   ecx
        push   edi
        push   edx
        call   sub_1_058A
loc_4_01DB:
        mov    eax, 0
        pop    edi
        cmp    dword ptr [ebp - 4], 1
        adc    eax, -1
        pop    esi
        and    eax, 0xfffffffe
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 048h, 04Bh, 051h, 0E8h
        dd  loc_1_0596               ; fixup
        db  048h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x5f]
        mov    ecx, dword ptr [esi + 0x63]
        dec    eax
        mov    edx, dword ptr [esi + 0x53]
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [esi + 0x4b]
        push   edx
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0590
loc_4_024D:
        mov    ecx, dword ptr [esi + 0x53]
        push   ecx
        call   sub_1_051E
loc_4_0256:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0260:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 066h, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h
        db  000h, 08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h
        dd  sub_1_0406               ; fixup
        db  05Dh, 0C2h, 010h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 00Ch
        db  08Bh, 0ECh, 08Ah, 044h, 024h, 010h, 0EEh, 05Dh, 0C2h, 00Ch, 000h, 055h, 08Bh, 054h, 024h, 00Ch
        db  066h, 08Bh, 044h, 024h, 010h, 08Bh, 0ECh, 066h, 0EFh, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 00Ch
        db  08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 0EFh, 05Dh, 0C2h, 00Ch, 000h, 055h, 08Bh, 044h, 024h, 014h
        db  08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh, 055h, 00Ch, 051h, 052h, 0E8h
        dd  sub_1_03C9               ; fixup
        db  05Dh, 0C2h, 010h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0310:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3FF0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0330:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 0xb
        jne    loc_4_0360
        push   esi
        call   sub_4_1B80
        test   al, al
        je     loc_4_0360
        mov    dword ptr [esi + 0xb7], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3E40
        push   esi
        call   sub_4_1BB0
loc_4_0360:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0370:
        push   ebp
        mov    eax, 0xd
        mov    ebp, esp
        sub    esp, 0x1a0
        mov    word ptr [ebp - 0x40], ax
        push   ebx
        mov    word ptr [ebp - 0x3e], ax
        push   esi
        push   edi
        mov    eax, 0xa
        mov    dword ptr [ebp - 0x3c], 0     ; -> loc_1_0680
        mov    dword ptr [ebp - 0x4c], 0     ; -> loc_1_069C
        mov    dword ptr [ebp - 0x44], 0     ; -> loc_1_06B4
        mov    word ptr [ebp - 0x50], ax
        mov    word ptr [ebp - 0x4e], ax
        mov    dword ptr [ebp - 0x5c], 0     ; -> obj1:0x06CC
        mov    word ptr [ebp - 0x48], ax
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x06F0
        mov    word ptr [ebp - 0x46], ax
        mov    word ptr [ebp - 0x60], ax
        mov    word ptr [ebp - 0x5e], ax
        mov    eax, 0x18
        mov    dword ptr [ebp - 0x54], 0     ; -> obj1:0x0714
        mov    word ptr [ebp - 0x38], ax
        mov    word ptr [ebp - 0x36], ax
        mov    eax, 0xc
        mov    word ptr [ebp - 0x58], ax
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x56], ax
        mov    ecx, dword ptr [edi + 4]
        mov    eax, 9
        mov    dword ptr [ebp - 0x1c], ecx
        mov    word ptr [ebp - 0x30], ax
        mov    edx, dword ptr [edi + 0x10]
        mov    word ptr [ebp - 0x2e], ax
        mov    ecx, dword ptr [edi + 0x14]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 0x14], edx
        xor    esi, esi
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        mov    dword ptr [ebp - 0x28], ecx
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x24], esi
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x072C
        push   eax
        push   ecx
        call   sub_1_0554
loc_4_0435:
        cmp    dword ptr [ebp - 4], esi
        je     loc_4_0448
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0448:
        push   1
        lea    eax, [ebp - 0x60]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_045F:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_046E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x18], ecx
loc_4_046E:
        push   1
        lea    eax, [ebp - 0x58]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_0485:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_4_04EC
        push   1
        lea    eax, [ebp - 0x50]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_04B0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 0x48]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 0x20], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_055A
loc_4_04DA:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax + 4]
        jmp    loc_4_0563
loc_4_04EC:
        cmp    eax, 1
        jne    loc_4_060F
        lea    eax, [ebp - 0x1a0]
        lea    ecx, [ebp - 0x24]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        call   sub_1_0486
loc_4_050A:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_060F
        cmp    dword ptr [ebp - 0x1a0], 0x2019f022
        je     loc_4_053C
        cmp    dword ptr [ebp - 0x1a0], 0x80f1f022
        je     loc_4_053C
        cmp    dword ptr [ebp - 0x1a0], 0x4019f022
        jne    loc_4_060F
loc_4_053C:
        mov    bl, byte ptr [ebp - 0xee]
        and    ebx, 0xf
        test   byte ptr [ebp - 0xee], 0x20
        jne    loc_4_0555
        mov    dword ptr [ebp - 0x14], 1
loc_4_0555:
        mov    eax, dword ptr [ebp - 0x24]
        shl    eax, 0xc
        add    eax, 0xc00
        mov    dword ptr [ebp - 0x20], eax
loc_4_0563:
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_057A:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0589
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], ecx
loc_4_0589:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_05A0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_05AF
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], ecx
loc_4_05AF:
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055A
loc_4_05C6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_05D2
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4]
loc_4_05D2:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0560
loc_4_05DB:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 0x28]
        mov    dword ptr [edi + 8], edx
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [edi + 0xc], ebx
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], ecx
        xor    eax, eax
        mov    dword ptr [edi + 0x18], edx
        mov    dword ptr [edi + 0x1c], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_060F:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0560
loc_4_0618:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0630:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x20
        mov    dword ptr [ebp - 0x20], eax
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x10], eax
        push   ecx
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0xc], 0xffffffff
        mov    dword ptr [ebp - 8], 0x14
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_4_0370
        test   eax, eax
        je     loc_4_0678
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0678:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    dword ptr [ecx + 0x209], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x225], edx
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x221], eax
        cmp    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ecx + 0x219], edx
        jne    loc_4_06AE
        mov    dword ptr [ecx + 0x20], 1
        jmp    loc_4_06BB
loc_4_06AE:
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_4_06BB
        mov    dword ptr [ecx + 0x20], 2
loc_4_06BB:
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x20d], eax
        mov    dword ptr [ecx + 0x211], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x215], eax
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_4_06E0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     loc_4_0701
        mov    eax, 0xc0010002
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0701:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x10]
        test   ecx, ecx
        jne    loc_4_071B
        push   edx
        push   esi
        push   edi
        call   sub_4_0B40
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_071B:
        cmp    ecx, 1
        jne    loc_4_0753
        mov    ecx, dword ptr [edi + 0x47]
        test   ecx, ecx
        je     loc_4_0743
        test   cl, 2
        je     loc_4_0731
        mov    eax, 0xc0000001
loc_4_0731:
        test   cl, 1
        je     loc_4_0758
        mov    eax, 0xc001000d
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0743:
        push   edx
        push   esi
        push   edi
        call   sub_4_0B90
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0753:
        mov    eax, 0x10001
loc_4_0758:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0760:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x24
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x1c], eax
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], eax
        cmp    esi, 0x1010101
        mov    eax, esi
        ja     loc_4_079F
        je     loc_4_0998
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_4_07C0
        jmp    dword ptr [eax*4]             ; -> obj4:0x0AEC
loc_4_079F:
        cmp    eax, 0x1010102
        je     loc_4_09B7
        cmp    eax, 0x1010103
        je     loc_4_09D6
        cmp    eax, 0x1010104
        je     loc_4_0A86
loc_4_07C0:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_4_0A9C
loc_4_07CC:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_07E1
        mov    eax, 0                        ; -> loc_1_07B4
        mov    edi, 0x5c
        jmp    loc_4_0AA2
loc_4_07E1:
        mov    eax, 0                        ; -> loc_1_0738
        mov    edi, 0x7c
        jmp    loc_4_0AA2
loc_4_07F0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x47]
        test   al, 2
        je     loc_4_0803
        mov    dword ptr [ebp - 0x18], 4
        jmp    loc_4_0815
loc_4_0803:
        mov    dword ptr [ebp - 0x18], 2
        test   al, 1
        jne    loc_4_0815
        mov    dword ptr [ebp - 0x18], 0
loc_4_0815:
        lea    eax, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_4_0AA2
loc_4_0822:
        lea    eax, [ebp - 0x1c]
        mov    edi, 4
        jmp    loc_4_0AA2
loc_4_082F:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_4_0AA2
loc_4_0843:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_4_0AA2
loc_4_0857:
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0x186a0
        cmp    dword ptr [edi + 0xaf], 0xa
        je     loc_4_0871
        mov    dword ptr [ebp - 8], 0xf4240
loc_4_0871:
        lea    eax, [ebp - 8]
        mov    edi, 4
        jmp    loc_4_0AA2
loc_4_087E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_0AA2
loc_4_0897:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_0AA2
loc_4_08B0:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_0AA2
loc_4_08C4:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_0AA2
loc_4_08DD:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 4
        call   sub_1_05BC
loc_4_08F4:
        mov    byte ptr [ebp - 0x21], 0
        lea    eax, [ebp - 0x24]
        jmp    loc_4_0AA2
loc_4_0900:
        mov    eax, 0                        ; -> loc_1_0810
        mov    edi, 0x17
        jmp    loc_4_0AA2
loc_4_090F:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_0920
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7b]
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_4_0929
loc_4_0920:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [eax + 8]
loc_4_0929:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_4_0AA2
loc_4_0939:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_0947
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7f]
        jmp    loc_4_094D
loc_4_0947:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x21]
loc_4_094D:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_0AA2
loc_4_095D:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_0AA2
loc_4_0970:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_0AA2
loc_4_0984:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_0AA2
loc_4_0998:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_05BC
loc_4_09AF:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_0AA2
loc_4_09B7:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x67
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_05BC
loc_4_09CE:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_0AA2
loc_4_09D6:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_0A2C
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x7b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0A03
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0A9C
loc_4_0A03:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
loc_4_0A19:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0xc], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0A9C
loc_4_0A2C:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0660
loc_4_0A3F:
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0A5A
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_0A9C
loc_4_0A5A:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebx + 0x1d]
        push   edx
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_2_06A0
loc_4_0A77:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_0A9C
loc_4_0A86:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x211]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_0AA2
loc_4_0A9C:
        mov    eax, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [ebp - 0x14]
loc_4_0AA2:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_0ADD
        cmp    esi, 0x1010103
        je     loc_4_0ADD
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_4_0ACD
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    ecx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_4_0ACD:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05BC
loc_4_0AD8:
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_4_0ADD:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (19 entries) ----
        dd  loc_4_07CC
        dd  loc_4_07F0
        dd  loc_4_0822
        dd  loc_4_0822
        dd  loc_4_082F
        dd  loc_4_0843
        dd  loc_4_0857
        dd  loc_4_087E
        dd  loc_4_0897
        dd  loc_4_08B0
        dd  loc_4_08C4
        dd  loc_4_08DD
        dd  loc_4_0900
        dd  loc_4_090F
        dd  loc_4_0939
        dd  loc_4_095D
        dd  loc_4_0970
        dd  loc_4_07C0
        dd  loc_4_0984
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0B40:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x18]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_4_0760
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_4_0BCD
        je     loc_4_0C9F
        cmp    eax, 0x1010e
        je     loc_4_0BDF
        cmp    eax, 0x1010f
        je     loc_4_0C19
        cmp    eax, 0x10112
        je     loc_4_0C8B
loc_4_0BCD:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0CDA
loc_4_0BDF:
        cmp    edi, 4
        je     loc_4_0BF6
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0CDA
loc_4_0BF6:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_05BC
loc_4_0C02:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0D00
        jmp    loc_4_0CD8
loc_4_0C19:
        cmp    edi, 4
        je     loc_4_0C30
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0CDA
loc_4_0C30:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_05BC
loc_4_0C3C:
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_4_0C84
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x7f]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_4_0C5E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x7f], eax
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0CDA
loc_4_0C5E:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x21]
        jne    loc_4_0C7C
        mov    eax, dword ptr [ecx + 0x21]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_4_0C7C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x21], eax
        call   sub_4_0F50
        jmp    loc_4_0CDA
loc_4_0C7C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0CDA
loc_4_0C84:
        mov    ebx, 0xc0000001
        jmp    loc_4_0CDA
loc_4_0C8B:
        cmp    edi, 4
        je     loc_4_0CDA
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0CDA
loc_4_0C9F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_4_0CBD
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0CDA
loc_4_0CBD:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        call   sub_4_0D60
loc_4_0CD8:
        mov    ebx, eax
loc_4_0CDA:
        test   ebx, ebx
        jne    loc_4_0CE8
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_4_0CE8:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D00:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0D42
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0D38
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 0x1d]
        push   eax
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 0x7b]
        push   ecx
        call   sub_2_05E0
loc_4_0D31:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0D38:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0D42:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D60:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0DA6
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0D9C
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [esi + 0x1d]
        push   edx
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 0x7b]
        push   ecx
        call   sub_2_0300
loc_4_0D95:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0D9C:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0DA6:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_0DC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        mov    dword ptr [ebp - 0x10], ecx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0xc]
        lea    ebx, [ebp - 0x10]
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        push   ebx
        mov    edx, dword ptr [esi + 0x14]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        push   1
        push   edx
        push   ecx
        push   ebx
        call   sub_4_0760
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_4_0E95
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_4_0E25
        je     loc_4_0E64
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_4_0E33
        jmp    dword ptr [eax*4]             ; -> obj4:0x0EB0
loc_4_0E25:
        cmp    eax, 0x1020102
        je     loc_4_0E6C
        cmp    eax, 0x1020103
        je     loc_4_0E74
loc_4_0E33:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_4_0E7D
loc_4_0E3C:
        mov    eax, dword ptr [ebx + 0x83]
        jmp    loc_4_0E7A
loc_4_0E44:
        mov    eax, dword ptr [ebx + 0x87]
        jmp    loc_4_0E7A
loc_4_0E4C:
        mov    eax, dword ptr [ebx + 0x8b]
        jmp    loc_4_0E7A
loc_4_0E54:
        mov    eax, dword ptr [ebx + 0x9b]
        jmp    loc_4_0E7A
loc_4_0E5C:
        mov    eax, dword ptr [ebx + 0x9f]
        jmp    loc_4_0E7A
loc_4_0E64:
        mov    eax, dword ptr [ebx + 0x97]
        jmp    loc_4_0E7A
loc_4_0E6C:
        mov    eax, dword ptr [ebx + 0x8f]
        jmp    loc_4_0E7A
loc_4_0E74:
        mov    eax, dword ptr [ebx + 0x93]
loc_4_0E7A:
        mov    dword ptr [ebp - 4], eax
loc_4_0E7D:
        cmp    edi, 4
        jb     loc_4_0E95
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_05BC
loc_4_0E91:
        add    dword ptr [ebp - 0xc], 4
loc_4_0E95:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        pop    edi
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_4_0E3C
        dd  loc_4_0E44
        dd  loc_4_0E4C
        dd  loc_4_0E54
        dd  loc_4_0E5C
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0ED0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [edx + 0x14]
        cmp    ecx, 0x10113
        ja     loc_4_0EF8
        je     loc_4_0F2F
        cmp    ecx, 0x10101
        jb     loc_4_0F2A
        cmp    ecx, 0x10111
        jbe    loc_4_0F2F
        jmp    loc_4_0F2A
loc_4_0EF8:
        cmp    ecx, 0x1010104
        ja     loc_4_0F1A
        cmp    ecx, 0x1010101
        jae    loc_4_0F2F
        cmp    ecx, 0x20101
        jb     loc_4_0F2A
        cmp    ecx, 0x20105
        jbe    loc_4_0F2F
        jmp    loc_4_0F2A
loc_4_0F1A:
        cmp    ecx, 0x1020101
        jb     loc_4_0F2A
        cmp    ecx, 0x1020103
        jbe    loc_4_0F2F
loc_4_0F2A:
        mov    eax, 0xc0010017
loc_4_0F2F:
        test   eax, eax
        jne    loc_4_0F3D
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0DC0
loc_4_0F3D:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0F50:
        push   ebp
        mov    ecx, 0xee
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x1c9]
        test   edx, edx
        je     loc_4_0F76
loc_4_0F66:
        mov    esi, dword ptr [edx + 0x21]
        cmp    ecx, esi
        jae    loc_4_0F6F
        mov    ecx, esi
loc_4_0F6F:
        mov    edx, dword ptr [edx + 0x11]
        test   edx, edx
        jne    loc_4_0F66
loc_4_0F76:
        pop    esi
        mov    dword ptr [eax + 0x7f], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0F80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        lea    ecx, [ebp - 0x14]
        push   esi
        lea    edx, [ebp - 0x10]
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   edx
        push   0
        push   ecx
        call   sub_1_0500
loc_4_0FA7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     loc_4_0FBE
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FBE:
        mov    eax, dword ptr [esi + 0x47]
        test   eax, eax
        je     loc_4_0FE9
        test   al, 0xa
        je     loc_4_0FD7
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FD7:
        test   al, 1
        je     loc_4_0FE9
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FE9:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_1010
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1010:
        push   esi
        call   sub_4_1B80
        test   al, al
        jne    loc_4_103A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x46], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_103A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_0524
loc_4_104B:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3690
        test   eax, eax
        je     loc_4_1099
        jle    loc_4_1087
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        push   esi
        call   sub_4_1BB0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1087:
        test   eax, eax
        jge    loc_4_1099
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1099:
        xor    edi, edi
        xor    eax, eax
loc_4_109D:
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_4_10B7
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [esi + edi + 0xba], cl
        jb     loc_4_109D
loc_4_10B7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        je     loc_4_111B
loc_4_10BF:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_10CC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_10DD:
        xor    ecx, ecx
        cmp    edi, 6
        jae    loc_4_10FE
loc_4_10E4:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_10FE
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [esi + edi + 0xba], al
        jb     loc_4_10E4
loc_4_10FE:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3820
        test   eax, eax
        jl     loc_4_1135
        inc    ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, ebx
        ja     loc_4_10BF
loc_4_111B:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_38E0
        test   eax, eax
        jge    loc_4_1143
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1135:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1143:
        lea    eax, [esi + 0xbb]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0330
loc_4_1153:
        test   al, al
        je     loc_4_1179
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11E0
        push   esi
        call   sub_4_1BB0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1179:
        push   esi
        inc    dword ptr [esi + 0x83]
        call   sub_4_1BB0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_18F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_11A0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_11CC
        pop    ebp
        mov    dword ptr [eax + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
loc_4_11CC:
        mov    ecx, dword ptr [eax + 0x28]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_11E0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x38], 0
        jne    loc_4_1209
        pop    ebp
        mov    dword ptr [eax + 0x38], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
loc_4_1209:
        mov    ecx, dword ptr [eax + 0x34]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1220:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x24], 0
        mov    edx, dword ptr [eax + 0x24]
        test   edx, edx
        jne    loc_4_1245
        pop    ebp
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x28], ecx
        inc    dword ptr [eax + 0x2c]
        ret    8
loc_4_1245:
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x24], ecx
        inc    dword ptr [eax + 0x2c]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1260:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    al, byte ptr [ebp + 0xc]
        push   esi
        test   al, al
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xb7], 0
        je     loc_4_12B8
        test   al, 1
        je     loc_4_1284
        inc    dword ptr [esi + 0x30]
loc_4_1284:
        test   al, 2
        je     loc_4_1291
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x93]
loc_4_1291:
        test   al, 4
        je     loc_4_129E
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_129E:
        test   al, 8
        je     loc_4_12AB
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_12AB:
        test   al, 0x10
        je     loc_4_12B8
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_12B8:
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_4_12CC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12CC:
        mov    edi, dword ptr [esi + 0x24]
        lea    ecx, [ebp - 0x18]
        push   ecx
        lea    edx, [ebp - 0xc]
        mov    eax, dword ptr [edi + 0x24]
        push   edx
        mov    dword ptr [esi + 0x24], eax
        lea    eax, [ebp - 0x14]
        push   eax
        dec    dword ptr [esi + 0x2c]
        push   0
        push   edi
        call   sub_1_0500
loc_4_12EC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_12FD:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3690
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_1347
        push   edi
        push   esi
        call   sub_4_1220
        test   ebx, ebx
        jge    loc_4_1339
        add    ebx, 2
        pop    edi
        pop    esi
        cmp    ebx, 1
        sbb    eax, eax
        pop    ebx
        and    eax, 0xfffffffc
        mov    esp, ebp
        add    eax, 2
        pop    ebp
        ret    8
loc_4_1339:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1347:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_134B:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_1365
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [esi + ebx + 0xba], al
        jb     loc_4_134B
loc_4_1365:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 0x14]
        dec    eax
        je     loc_4_13D2
loc_4_1372:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_137F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_1390:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    loc_4_13B1
loc_4_1397:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_13B1
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [esi + ebx + 0xba], al
        jb     loc_4_1397
loc_4_13B1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3820
        cmp    eax, -2
        je     loc_4_13F4
        mov    eax, dword ptr [ebp - 0x14]
        inc    dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_4_1372
loc_4_13D2:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_38E0
        cmp    eax, -2
        jne    loc_4_1409
        push   edi
        push   esi
        call   sub_4_1220
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_13F4:
        push   edi
        push   esi
        call   sub_4_1220
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1409:
        lea    eax, [esi + 0xbb]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0330
loc_4_1419:
        test   al, al
        je     loc_4_1439
        push   edi
        mov    eax, dword ptr [edi + 0x28]
        push   eax
        push   esi
        call   sub_4_11E0
        push   esi
        call   sub_4_44A0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1439:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_044A
loc_4_144E:
        push   esi
        call   sub_4_18F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_1460:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_4_1485:
        test   edx, edx
        je     loc_4_1494
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_4_1485
        mov    dword ptr [ecx], edx
loc_4_1494:
        cmp    dword ptr [ecx], -1
        jne    loc_4_14A7
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_14A7:
        mov    eax, dword ptr [0]            ; -> obj1:0x082C
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x25
        push   ecx
        call   sub_1_04D6
loc_4_14BA:
        test   eax, eax
        je     loc_4_14CC
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_14CC:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x1d
        mov    ebx, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0170
loc_4_14E7:
        test   al, al
        jne    loc_4_1506
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x25
        push   eax
        call   sub_1_04DC
loc_4_14F8:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_1506:
        test   byte ptr [ebx + 0x47], 1
        je     loc_4_1528
        mov    eax, dword ptr [ebx + 0x1d1]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, 0x103
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x1d1], ecx
        jmp    loc_4_153F
loc_4_1528:
        mov    eax, dword ptr [ebx + 0x1c9]
        mov    ecx, dword ptr [ebp - 4]
        xor    esi, esi
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x1c9], ecx
loc_4_153F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xd], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0x1c5]
        mov    dword ptr [eax + 0x19], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 5], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 9], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0x7f]
        mov    dword ptr [eax + 0x21], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [ebx + 0x1cd], 0
        jne    loc_4_1592
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_4_4010
loc_4_1592:
        mov    eax, esi
        pop    edi
        pop    esi
        inc    dword ptr [ebx + 0x1cd]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15B0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        xor    eax, eax
loc_4_15C2:
        mov    dl, byte ptr [esi + eax + 0x67]
        mov    ecx, dword ptr [esi + 0x73]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     loc_4_15C2
        mov    edi, dword ptr [esi + 0x73]
        add    edi, 6
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_4_1604
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x14]
loc_4_15EA:
        xor    ebx, ebx
loc_4_15EC:
        lea    edx, [ecx + ebx]
        inc    edi
        inc    ebx
        mov    dl, byte ptr [edx + eax]
        mov    byte ptr [edi - 1], dl
        cmp    ebx, 6
        jb     loc_4_15EC
        add    ecx, 6
        dec    dword ptr [ebp - 4]
        jne    loc_4_15EA
loc_4_1604:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x77], eax
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_161F
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x44], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_161F:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1CF0
        push   eax
        call   sub_1_04CA
loc_4_162E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 0x215]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 0x1bd]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_1676
        mov    eax, 0xc0000001
        pop    edi
        or     dword ptr [esi + 0x47], 2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_1676:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1690:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [eax + 0xd]
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0xffffffd0
        je     loc_4_16B0
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_16B0:
        test   al, 1
        je     loc_4_16B9
        mov    ecx, 1
loc_4_16B9:
        test   al, 2
        je     loc_4_16C0
        or     ecx, 4
loc_4_16C0:
        test   al, 4
        je     loc_4_16C7
        or     ecx, 8
loc_4_16C7:
        test   al, 8
        je     loc_4_16CE
        or     ecx, 2
loc_4_16CE:
        test   al, 0x20
        je     loc_4_16D5
        or     ecx, 0x10
loc_4_16D5:
        test   dword ptr [esi + 0x1c1], ecx
        je     loc_4_16E7
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_16E7:
        mov    dword ptr [esi + 0x1bd], ecx
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_16FE
        mov    byte ptr [esi + 0x43], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_4_16FE:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1CF0
        push   eax
        call   sub_1_04CA
loc_4_170D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 0x1bd]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_1736
        or     dword ptr [esi + 0x47], 2
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1736:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1750:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xd]
        push   1
        add    ecx, 0x1d5
        push   ecx
        mov    edx, dword ptr [ecx + 0x1d]
        mov    dword ptr [eax + 0x15], edx
        mov    dword ptr [ecx + 0x1d], eax
        call   sub_1_04B2
loc_4_1771:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x1f2]
        test   ecx, ecx
        je     loc_4_17B0
loc_4_1791:
        mov    eax, dword ptr [esi + 0x1f2]
        push   ecx
        mov    edx, dword ptr [eax + 0x15]
        mov    dword ptr [esi + 0x1f2], edx
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 0x1f2]
        test   ecx, ecx
        jne    loc_4_1791
loc_4_17B0:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 8
        jne    loc_4_17F6
        push   esi
        call   sub_4_1B80
        test   al, al
        je     loc_4_17E5
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3D90
        push   esi
        call   sub_4_1BB0
loc_4_17E5:
        push   0x3e8
        add    esi, 0x1e1
        push   esi
        call   sub_1_04B2
loc_4_17F6:
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_4_1800:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4], 0
        mov    edi, dword ptr [esi + 0xd]
        je     loc_4_181E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_181E:
        inc    dword ptr [esi]
        mov    eax, dword ptr [edi + 0x1c9]
        cmp    esi, eax
        jne    loc_4_1835
        mov    eax, dword ptr [esi + 0x11]
        mov    dword ptr [edi + 0x1c9], eax
        jmp    loc_4_1848
loc_4_1835:
        cmp    dword ptr [eax + 0x11], esi
        je     loc_4_1842
loc_4_183A:
        mov    eax, dword ptr [eax + 0x11]
        cmp    dword ptr [eax + 0x11], esi
        jne    loc_4_183A
loc_4_1842:
        mov    ecx, dword ptr [esi + 0x11]
        mov    dword ptr [eax + 0x11], ecx
loc_4_1848:
        push   0
        mov    eax, dword ptr [esi + 0x1d]
        push   eax
        mov    ecx, dword ptr [edi + 0x7b]
        push   ecx
        call   sub_2_0240
loc_4_1857:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_186F
        cmp    ebx, 0x103
        je     loc_4_1883
        cmp    ebx, 0xc001000e
        je     loc_4_188C
        jmp    loc_4_1895
loc_4_186F:
        mov    eax, dword ptr [esi]
        sub    eax, 2
        mov    dword ptr [esi], eax
        je     loc_4_1897
        mov    byte ptr [esi + 4], 1
        mov    ebx, 0x103
        jmp    loc_4_1897
loc_4_1883:
        mov    byte ptr [esi + 4], 1
        sub    dword ptr [esi], 2
        jmp    loc_4_1897
loc_4_188C:
        mov    byte ptr [esi + 4], 1
        mov    ebx, 0x103
loc_4_1895:
        dec    dword ptr [esi]
loc_4_1897:
        cmp    dword ptr [esi], 0
        jne    loc_4_18DB
        mov    eax, dword ptr [edi + 0x7f]
        cmp    dword ptr [esi + 0x21], eax
        jne    loc_4_18AA
        push   edi
        call   sub_4_0F50
loc_4_18AA:
        cmp    dword ptr [edi + 0x1c9], 0
        jne    loc_4_18C2
        push   edi
        lea    eax, [edi + 8]
        push   0                             ; -> loc_4_1CF0
        push   eax
        call   sub_1_04CA
loc_4_18C2:
        push   0
        dec    dword ptr [edi + 0x1cd]
        push   0x25
        push   esi
        call   sub_1_04DC
loc_4_18D2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_18DB:
        mov    ebx, 0x103
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_18F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    loc_4_1943
        mov    eax, dword ptr [esi + 0x21]
        cmp    dword ptr [edi + 0x7f], eax
        jne    loc_4_1910
        push   edi
        call   sub_4_0F50
loc_4_1910:
        cmp    dword ptr [edi + 0x1c9], 0
        jne    loc_4_1928
        push   edi
        lea    eax, [edi + 8]
        push   0                             ; -> loc_4_1CF0
        push   eax
        call   sub_1_04CA
loc_4_1928:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0444
loc_4_1933:
        push   0
        push   0x25
        push   esi
        call   sub_1_04DC
loc_4_193D:
        dec    dword ptr [edi + 0x1cd]
loc_4_1943:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1950:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        cmp    byte ptr [eax + 4], 0
        je     loc_4_196D
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_196D:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_197E
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_197E:
        push   0x40010004
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 0x1ee], eax
        call   sub_4_1B20
        or     dword ptr [esi + 0x47], 1
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        je     loc_4_19C7
loc_4_199C:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x24], eax
        dec    dword ptr [esi + 0x2c]
        inc    dword ptr [esi + 0x30]
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_044A
loc_4_19BA:
        push   edi
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        jne    loc_4_199C
loc_4_19C7:
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        je     loc_4_19F3
loc_4_19CE:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x38], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_044A
loc_4_19E6:
        push   edi
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        jne    loc_4_19CE
loc_4_19F3:
        mov    byte ptr [esi + 0x203], 1
        push   esi
        call   sub_4_1B80
        test   al, al
        je     loc_4_1A1B
        push   esi
        call   sub_4_1A30
        push   esi
        call   sub_4_1BB0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1A1B:
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x203], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1A30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x1ee]
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 0x203], 0
        mov    byte ptr [esi + 0x42], 1
        and    dword ptr [esi + 0x47], 0xfffffffd
        push   eax
        call   sub_4_2FD0
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 0x215]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 0x1bd]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_1A87
        or     dword ptr [esi + 0x47], 2
loc_4_1A87:
        push   0x40010005
        push   esi
        call   sub_4_1B20
        mov    eax, dword ptr [esi + 0x47]
        and    eax, 0xfffffffe
        mov    dword ptr [esi + 0x47], eax
        test   al, 2
        je     loc_4_1AA6
        push   0x80010004
        jmp    loc_4_1AA8
loc_4_1AA6:
        push   0
loc_4_1AA8:
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0456
loc_4_1AB1:
        push   edi
        call   sub_4_18F0
        push   esi
        call   sub_4_1AD0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1AD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x1d1]
        test   ecx, ecx
        je     loc_4_1B16
loc_4_1AE1:
        mov    eax, dword ptr [esi + 0x1d1]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0x11]
        mov    eax, dword ptr [esi + 0x1c9]
        mov    dword ptr [esi + 0x1d1], edx
        mov    dword ptr [ecx + 0x11], eax
        mov    dword ptr [esi + 0x1c9], ecx
        mov    ecx, dword ptr [ecx + 5]
        push   ecx
        call   sub_1_043E
loc_4_1B0C:
        mov    ecx, dword ptr [esi + 0x1d1]
        test   ecx, ecx
        jne    loc_4_1AE1
loc_4_1B16:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1B20:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x1c9]
        test   edi, edi
        je     loc_4_1B6B
        mov    esi, dword ptr [ebp + 0xc]
loc_4_1B3A:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi]
        mov    ebx, dword ptr [edi + 0x11]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_046E
loc_4_1B56:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0474
loc_4_1B5F:
        push   edi
        call   sub_4_18F0
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_4_1B3A
loc_4_1B6B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0x40], 0
        je     loc_4_1B93
        xor    al, al
        pop    ebp
        ret    4
loc_4_1B93:
        mov    byte ptr [eax + 0x40], 1
        push   eax
        push   0                             ; -> loc_4_1CF0
        add    eax, 8
        push   eax
        call   sub_1_04CA
loc_4_1BA6:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x38], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 0x46]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x41]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x43]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x44]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x203]
        or     ecx, eax
        je     loc_4_1CDC
loc_4_1BED:
        cmp    byte ptr [esi + 0x203], 0
        je     loc_4_1BFE
        mov    bl, 1
        push   esi
        call   sub_4_1A30
loc_4_1BFE:
        cmp    byte ptr [esi + 0x44], 0
        je     loc_4_1C48
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 0x215]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 0x1bd]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_1C46
        or     dword ptr [esi + 0x47], 2
loc_4_1C46:
        mov    bl, 1
loc_4_1C48:
        cmp    byte ptr [esi + 0x43], 0
        je     loc_4_1C73
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 0x1bd]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_1C71
        or     dword ptr [esi + 0x47], 2
loc_4_1C71:
        mov    bl, 1
loc_4_1C73:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_1C88
        mov    eax, dword ptr [esi]
        mov    bl, 1
        mov    byte ptr [esi + 0x46], 0
        push   eax
        push   esi
        call   sub_4_3E40
loc_4_1C88:
        cmp    dword ptr [esi + 0x38], 0
        je     loc_4_1C96
        push   esi
        mov    bl, 1
        call   sub_4_44A0
loc_4_1C96:
        cmp    byte ptr [esi + 0x41], 0
        je     loc_4_1CA9
        mov    byte ptr [esi + 0x41], 0
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_1_0290
loc_4_1CA9:
        cmp    dword ptr [esi + 0x38], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 0x46]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x41]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x43]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x44]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x203]
        or     ecx, eax
        jne    loc_4_1BED
loc_4_1CDC:
        mov    byte ptr [esi + 0x40], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_1CF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3FF0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0F1h, 022h, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D30:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 0x18
        mov    word ptr [ebp - 0xc], ax
        push   ebx
        mov    word ptr [ebp - 0xa], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x16], bx
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_0838
        mov    dword ptr [ebp - 0x14], 0x4019f022
        mov    word ptr [ebp - 0x18], 1
        push   ecx
        push   eax
        push   0xc                           ; -> obj1:0x0CF8
        call   sub_1_041A
loc_4_1D71:
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [0x46], edx         ; -> loc_1_0D32
        mov    dword ptr [0x4a], ecx         ; -> loc_1_0D36
        mov    dword ptr [4], ebx            ; -> obj1:0x0CF0
        mov    dword ptr [0x52], ebx         ; -> loc_1_0D3E
        call   sub_4_2560
        mov    dword ptr [0x4e], eax         ; -> loc_1_0D3A
        mov    byte ptr [0x10], 3            ; -> obj1:0x0CFC
        mov    byte ptr [0x11], bl           ; -> loc_1_0CFD
        mov    dword ptr [0x12], ebx         ; -> obj1:0x0CFE
        mov    dword ptr [0x16], 0           ; -> obj1:0x0D02
        mov    dword ptr [0x1a], 0           ; -> obj1:0x0D06
        mov    dword ptr [0x1e], 0           ; -> obj1:0x0D0A
        mov    dword ptr [0x22], 0           ; -> obj1:0x0D0E
        mov    dword ptr [0x26], 0           ; -> obj1:0x0D12
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x2a], 0           ; -> obj1:0x0D16
        mov    dword ptr [0x2e], 0           ; -> obj1:0x0D1A
        mov    dword ptr [0x32], 0           ; -> obj1:0x0D1E
        mov    dword ptr [0x36], 0           ; -> obj1:0x0D22
        mov    dword ptr [0x3a], 0           ; -> loc_1_0D26
        push   0x36
        mov    dword ptr [0x3e], edx         ; -> loc_1_0D2A
        push   0x10                          ; -> obj1:0x0CFC
        lea    edx, [ebp - 4]
        push   0                             ; -> obj1:0x0CEC
        mov    dword ptr [0x42], ecx         ; -> loc_1_0D2E
        mov    eax, dword ptr [0xc]          ; -> obj1:0x0CF8
        push   eax
        push   8                             ; -> obj1:0x0CF4
        push   edx
        call   sub_1_0426
loc_4_1E44:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1E71
        cmp    dword ptr [4], ebx            ; -> obj1:0x0CF0
        jbe    loc_4_1E5A
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1E5A:
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj1:0x0CF8
        push   eax
        call   sub_1_0420
loc_4_1E67:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1E71:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_1E80:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x082C
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   0x22d
        push   ecx
        call   sub_1_04D6
loc_4_1E9E:
        test   eax, eax
        je     loc_4_1EAF
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1EAF:
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_0630
        test   eax, eax
        je     loc_4_1EE1
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x22d
        push   eax
        call   sub_1_04DC
loc_4_1ED4:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1EE1:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1F20
        test   eax, eax
        jne    loc_4_1EFA
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1EFA:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x22d
        push   eax
        call   sub_1_04DC
loc_4_1F0A:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x64
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_23F0
        push   esi
        call   sub_4_22E0
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_2252
        lea    edi, [ebp - 0x64]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x64], 0
        mov    eax, dword ptr [esi + 0x20d]
        cmp    eax, -1
        je     loc_4_1F66
        mov    dword ptr [ebp - 0x64], eax
loc_4_1F66:
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0x209]
        mov    dword ptr [ebp - 0x56], ebx
        mov    dword ptr [ebp - 0x4a], eax
        mov    dword ptr [ebp - 0x5a], 1
        lea    ecx, [ebp - 0x64]
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [8]            ; -> obj1:0x0CF4
        push   eax
        lea    edx, [esi + 0x205]
        mov    dword ptr [ebp - 0x52], 0x600
        mov    dword ptr [ebp - 0x4e], 1
        mov    dword ptr [ebp - 0x46], 0x40
        push   esi
        push   ecx
        push   edx
        call   sub_1_0432
loc_4_1FB0:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_4_224C
        mov    eax, dword ptr [esi + 0x20d]
        cmp    eax, -1
        je     loc_4_2031
        lea    ecx, [ebp - 0x20]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [ecx + 0xc], ebx
        push   0x600
        mov    word ptr [ecx + 0x10], bx
        push   ecx
        mov    byte ptr [ecx + 0x12], bl
        lea    edx, [esi + 0x4b]
        mov    byte ptr [ebp - 0x20], 1
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x15], eax
        mov    dword ptr [ebp - 0x1d], 2
        mov    dword ptr [ebp - 0x19], 3
        mov    eax, dword ptr [esi + 0x205]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0584
loc_4_200A:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_2031
        mov    eax, dword ptr [esi + 0x20d]
        mov    ecx, dword ptr [esi + 0x205]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_05A8
loc_4_2029:
        add    esp, 0x10
        jmp    loc_4_2240
loc_4_2031:
        lea    eax, [ebp - 0x3c]
        mov    ecx, dword ptr [esi + 0x219]
        push   eax
        mov    edx, dword ptr [esi + 0x209]
        push   ecx
        push   edx
        call   sub_4_4090
        mov    ecx, dword ptr [esi + 0x209]
        mov    edx, dword ptr [esi + 0x20d]
        mov    ebx, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   edx
        push   ebx
        mov    eax, dword ptr [ebp - 0x2c]
        push   eax
        lea    edx, [ecx + 0x40]
        push   edx
        mov    eax, dword ptr [esi + 0x73]
        push   ecx
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 0x219]
        push   ecx
        push   eax
        push   esi
        push   edx
        call   sub_4_2EC0
        test   eax, eax
        jge    loc_4_20B1
        mov    eax, dword ptr [esi + 0x209]
        mov    ecx, dword ptr [esi + 0x219]
        push   eax
        mov    edx, dword ptr [esi + 0x205]
        push   ecx
        push   2
        push   0xaa000004
        push   edx
        call   sub_1_05A8
loc_4_20A2:
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0x14
        jmp    loc_4_222E
loc_4_20B1:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x20], eax
loc_4_20C2:
        lea    eax, [ecx + esi]
        mov    edx, dword ptr [esi + 0x73]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 0x6d], dl
        mov    byte ptr [eax + 0x67], dl
        jb     loc_4_20C2
        lea    ebx, [esi + 0x7b]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 0x67]
        push   eax
        mov    edx, dword ptr [esi + 0x211]
        push   ecx
        push   0                             ; -> loc_4_1750
        push   0                             ; -> loc_4_1690
        push   0                             ; -> loc_4_15B0
        push   edx
        call   sub_2_0000
loc_4_20FF:
        test   al, al
        jne    loc_4_210F
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_4_222E
loc_4_210F:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3120
        mov    eax, dword ptr [esi + 0x215]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3D50
        mov    edi, eax
        mov    ecx, dword ptr [0]            ; -> obj1:0x082C
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        call   sub_1_04D6
loc_4_2145:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_2158
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_4_2226
loc_4_2158:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3D70
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0xa3], edx
        push   0
        mov    eax, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    dword ptr [esi + 0xa7], eax
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0xab], edx
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0xaf], eax
        call   sub_1_04DC
loc_4_2193:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3FD0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 0x1c1], eax
        mov    dword ptr [esi + 0x1bd], 0
        call   sub_4_3390
        test   eax, eax
        jge    loc_4_21C3
        or     dword ptr [esi + 0x47], 2
        jmp    loc_4_2226
loc_4_21C3:
        lea    edx, [esi + 8]
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [esi + 0x219]
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [esi + 0x205]
        push   ecx
        push   0                             ; -> loc_4_0330
        push   esi
        push   0                             ; -> loc_4_0310
        push   eax
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        call   sub_1_04BE
loc_4_21FB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_225E
        mov    eax, dword ptr [esi + 0x219]
        mov    ecx, dword ptr [esi + 0x205]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_05A8
loc_4_221B:
        add    esp, 0x10
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3040
loc_4_2226:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_4_222E:
        cmp    dword ptr [esi + 0x20d], -1
        je     loc_4_2240
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_059C
loc_4_2240:
        mov    eax, dword ptr [esi + 0x205]
        push   eax
        call   sub_1_0438
loc_4_224C:
        push   esi
        call   sub_4_23B0
loc_4_2252:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_225E:
        mov    dword ptr [esi + 0x1c5], 0    ; -> obj1:0x0CEC
        mov    byte ptr [esi + 0x45], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x1cd], ecx
        mov    dword ptr [esi + 0x1c9], ecx
        mov    dword ptr [esi + 0x1d1], ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x0CEC
        mov    dword ptr [esi + 0x229], eax
        lea    eax, [esi + 0x1e1]
        mov    dword ptr [0], esi            ; -> obj1:0x0CEC
        mov    dword ptr [esi + 0x1d5], ecx
        mov    dword ptr [esi + 0x1dd], esi
        push   0x3e8
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 0x1e9], esi
        mov    dword ptr [esi + 0x1d9], 0    ; -> loc_4_1780
        mov    dword ptr [esi + 0x1e5], 0    ; -> loc_4_17C0
        call   sub_1_04B2
loc_4_22CA:
        mov    ecx, dword ptr [esi + 0x1c5]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_4_22E0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x082C
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [0x4e]         ; -> loc_1_0D3A
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        call   sub_1_04D6
loc_4_2301:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_2316
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2316:
        lea    ebx, [esi + 0x73]
        mov    eax, dword ptr [0]            ; -> obj1:0x082C
        push   eax
        mov    ecx, dword ptr [esi + 0x211]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        call   sub_1_04D6
loc_4_2334:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_2385
        mov    edx, dword ptr [esi + 0x211]
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        lea    edx, [edx + edx*2 + 3]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        push   2
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    ecx, [ebp - 4]
        lea    eax, [esi + 0x4f]
        push   eax
        push   ecx
        call   sub_1_0506
loc_4_2368:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_23A3
        push   0
        mov    eax, dword ptr [esi + 0x211]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_04DC
loc_4_2385:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> loc_1_0D3A
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_04DC
loc_4_2395:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_23A3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_23B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x4f]
        push   eax
        call   sub_1_050C
loc_4_23C0:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> loc_1_0D3A
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_04DC
loc_4_23D0:
        push   0
        mov    ecx, dword ptr [esi + 0x211]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 0x73]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_04DC
loc_4_23E8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_23F0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1cd], ecx
        mov    byte ptr [eax + 0x40], cl
        mov    byte ptr [eax + 0x44], cl
        mov    byte ptr [eax + 0x202], cl
        mov    byte ptr [eax + 0x45], cl
        mov    byte ptr [eax + 0x41], cl
        mov    byte ptr [eax + 0x1ed], cl
        mov    dword ptr [eax + 0x47], ecx
        mov    dword ptr [eax + 0x1bd], ecx
        mov    dword ptr [eax + 0x77], ecx
        mov    byte ptr [eax + 0x43], cl
        mov    byte ptr [eax + 0x203], cl
        mov    byte ptr [eax + 0x204], cl
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    byte ptr [eax + 0x46], cl
        pop    ebp
        mov    dword ptr [eax + 0x34], ecx
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x3c], ecx
        mov    dword ptr [eax + 0x7f], 0xee
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2460:
        push   ebp
        mov    eax, dword ptr [8]            ; -> obj1:0x0CF4
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_042C
loc_4_2475:
        push   0
        mov    ecx, dword ptr [0xc]          ; -> obj1:0x0CF8
        push   ecx
        call   sub_1_0420
loc_4_2483:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2490:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x4c4b40
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0x47], 8
        lea    edi, [esi + 8]
        call   sub_1_04D0
loc_4_24A9:
        push   esi
        push   0                             ; -> loc_4_2540
        push   edi
        call   sub_1_04CA
loc_4_24B5:
        push   edi
        call   sub_1_04C4
loc_4_24BB:
        cmp    esi, dword ptr [0]            ; -> obj1:0x0CEC
        jne    loc_4_24D0
        mov    eax, dword ptr [esi + 0x229]
        mov    dword ptr [0], eax            ; -> obj1:0x0CEC
        jmp    loc_4_24FE
loc_4_24D0:
        mov    ecx, dword ptr [0]            ; -> obj1:0x0CEC
        cmp    dword ptr [ecx + 0x229], esi
        je     loc_4_24EC
loc_4_24DE:
        mov    ecx, dword ptr [ecx + 0x229]
        cmp    dword ptr [ecx + 0x229], esi
        jne    loc_4_24DE
loc_4_24EC:
        mov    eax, dword ptr [ecx + 0x229]
        mov    edx, dword ptr [eax + 0x229]
        mov    dword ptr [ecx + 0x229], edx
loc_4_24FE:
        cmp    dword ptr [esi + 0x20d], -1
        je     loc_4_2510
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_059C
loc_4_2510:
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_2_0120
loc_4_2519:
        mov    eax, dword ptr [esi + 0x205]
        push   eax
        call   sub_1_0438
loc_4_2525:
        push   esi
        call   sub_4_23B0
        push   0
        push   0x22d
        push   esi
        call   sub_1_04DC
loc_4_2538:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2540:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2FD0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_2560:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        test   eax, eax
        jne    loc_4_257A
        mov    dword ptr [0], 1              ; -> obj6:0x0000
loc_4_257A:
        mov    eax, 0x186
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2590:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        mov    esi, dword ptr [ecx + 4]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 0x50
        jne    loc_4_275C
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 0x48
        jne    loc_4_275C
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        mov    eax, dword ptr [ecx + 0x104]
        and    al, 0xf0
        cmp    al, 0x50
        jne    loc_4_275C
        lea    edx, [esi + 3]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 0x53
        jne    loc_4_275C
        mov    edx, dword ptr [ecx + 0xc]
        mov    eax, 6
        out    dx, ax
        mov    esi, dword ptr [ecx + 0x44]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 8
        jne    loc_4_274F
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, bl
        jne    loc_4_274F
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 9
        jne    loc_4_274F
        mov    esi, dword ptr [ecx + 0x24]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 0x22
        jne    loc_4_2742
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        cmp    al, 0xf0
        jne    loc_4_2742
        lea    edx, [esi + 2]
        in     al, dx
        lea    edx, [esi + 3]
        mov    byte ptr [ecx + 0x104], al
        mov    byte ptr [ebp - 1], al
        in     al, dx
        cmp    byte ptr [ebp - 1], 0xf1
        mov    byte ptr [ecx + 0x104], al
        jne    loc_4_26AB
        cmp    al, 0x80
        je     loc_4_2685
        cmp    byte ptr [ebp - 1], 0xf1
        jne    loc_4_26AB
        cmp    al, 0x50
        jne    loc_4_26AB
loc_4_2685:
        push   0
        mov    eax, dword ptr [ecx + 0x108]
        push   0
        push   0xa
        push   0                             ; -> loc_1_0AE4
        push   1
        push   eax
        call   sub_4_0080
        mov    ax, 0xff02
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_26AB:
        cmp    byte ptr [ebp - 1], 0x19
        jne    loc_4_2735
        cmp    al, 0x40
        je     loc_4_26BD
        cmp    al, 0x50
        jne    loc_4_2735
loc_4_26BD:
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0xc]
loc_4_26C2:
        mov    edx, dword ptr [ecx + 0x44]
        inc    esi
        add    edx, edi
        inc    edi
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        mov    byte ptr [esi - 1], al
        and    ax, 0xff
        add    bx, ax
        cmp    edi, 6
        jl     loc_4_26C2
        mov    edx, dword ptr [ecx + 0x5c]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        and    ax, 0xff
        add    bx, ax
        cmp    bl, 0xff
        jne    loc_4_2725
        xor    bx, bx
        xor    edi, edi
        mov    esi, dword ptr [ecx + 0x24]
loc_4_26FD:
        lea    edx, [edi + esi]
        inc    edi
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        and    ax, 0xff
        add    bx, ax
        cmp    edi, 4
        jl     loc_4_26FD
        mov    edx, dword ptr [ecx + 0x34]
        in     al, dx
        mov    byte ptr [ecx + 0x104], al
        and    ax, 0xff
        add    bx, ax
loc_4_2725:
        mov    ax, bx
        pop    edi
        and    ax, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2735:
        mov    ax, 0xff02
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2742:
        mov    ax, 0xff02
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_274F:
        mov    ax, 0xff01
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_275C:
        mov    ax, 0xff00
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2770:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        test   ebx, ebx
        jne    loc_4_278E
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_278E:
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x14]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x40
        jne    loc_4_27D0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        push   eax
        mov    dx, word ptr [esi]
        push   ebx
        push   ecx
        mov    eax, dword ptr [esi + 0x108]
        push   0                             ; -> loc_1_0B24
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_27D0:
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    cx, ax
        mov    edx, dword ptr [esi + 0x24]
        mov    word ptr [esi + 0x104], ax
        and    ecx, 0xff
        in     ax, dx
        shl    ecx, 0x10
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        and    eax, 0xff00
        or     eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        cmp    ebx, eax
        je     loc_4_283E
        mov    dx, word ptr [esi]
        push   ebx
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 0x108]
        push   ecx
        push   0                             ; -> loc_1_0B54
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_283E:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 0x1fff
        jae    loc_4_2879
        mov    cx, word ptr [esi]
        push   eax
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        mov    edx, dword ptr [esi + 0x108]
        push   eax
        push   0                             ; -> loc_1_0B94
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_2879:
        push   0x2000
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [esi + 0x108]
        push   ecx
        call   sub_4_0020
        mov    edi, eax
        mov    dword ptr [esi + 0x114], eax
        test   edi, edi
        jne    loc_4_28C9
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [esi]
        push   eax
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x108]
        mov    cx, word ptr [esi + 2]
        push   ecx
        push   0                             ; -> loc_1_0BD0
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_28C9:
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 2
        out    dx, ax
        mov    ebx, dword ptr [esi + 0x3c]
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, 0x2000
        mov    ecx, dword ptr [esi + 0x104]
        mov    edx, dword ptr [esi + 0xc]
        shr    cl, 5
        shl    eax, cl
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [esi + 0x14e], eax
        mov    eax, 5
        mov    ecx, dword ptr [ebp - 0x14]
        out    dx, ax
        shr    ecx, 4
        mov    edx, dword ptr [esi + 0x34]
        dec    cx
        xor    eax, eax
        out    dx, ax
        mov    ax, cx
        mov    edx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 2
        out    dx, ax
        mov    dword ptr [ebp - 4], 0xc
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xfffffc00
        mov    dword ptr [ebp - 0x18], eax
        je     loc_4_29AF
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0xa
        mov    dword ptr [ebp - 8], eax
loc_4_2950:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 0x10]
        cmp    eax, 1
        jne    loc_4_2962
        mov    dword ptr [ebp - 4], 0x1c
loc_4_2962:
        mov    cx, 0x400
        mov    ebx, 0x400
        sub    cx, word ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x44]
        mov    eax, ebx
        out    dx, eax
        mov    ax, cx
        mov    edx, dword ptr [esi + 0x34]
        out    dx, ax
        xor    ecx, ecx
        test   ebx, 0xfffffffc
        jbe    loc_4_2998
        shr    ebx, 2
        mov    edx, dword ptr [esi + 0x44]
loc_4_2990:
        mov    eax, ecx
        inc    ecx
        out    dx, eax
        cmp    ecx, ebx
        jb     loc_4_2990
loc_4_2998:
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, 3
        out    dx, ax
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_4_2950
        jmp    loc_4_29B2
loc_4_29AF:
        mov    ecx, dword ptr [ebp - 4]
loc_4_29B2:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x8000
        out    dx, ax
        mov    eax, 2
        out    dx, ax
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 7
        out    dx, ax
        mov    edx, dword ptr [esi + 0x6c]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x8100
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2000
        mov    dword ptr [ebp - 4], 0xc
        out    dx, ax
        mov    eax, dword ptr [edi + 4]
        xor    ebx, ebx
        add    eax, dword ptr [edi]
        inc    eax
        cmp    dword ptr [ebp - 0x18], ebx
        mov    dword ptr [esi + 0x104], eax
        je     loc_4_2A7C
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0xa
        mov    dword ptr [ebp - 8], eax
loc_4_2A19:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, ebx
        cmp    eax, 1
        jne    loc_4_2A2A
        mov    dword ptr [ebp - 4], 0x1c
loc_4_2A2A:
        mov    ecx, 3
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    ecx, ebx
        mov    eax, dword ptr [edi + edx*4]
        and    ecx, 1
        add    eax, dword ptr [edi + ecx*4]
        add    eax, dword ptr [edi]
        jne    loc_4_2A4A
        mov    dword ptr [esi + 0x104], ebx
loc_4_2A4A:
        xor    ecx, ecx
        mov    eax, 0x400
        sub    eax, dword ptr [ebp - 4]
        test   eax, 0xfffffffc
        jbe    loc_4_2A6F
loc_4_2A5B:
        mov    edx, ecx
        and    edx, 1
        cmp    dword ptr [edi + edx*4], ecx
        jne    loc_4_2A6F
        inc    ecx
        mov    edx, eax
        shr    edx, 2
        cmp    edx, ecx
        ja     loc_4_2A5B
loc_4_2A6F:
        shr    eax, 2
        cmp    eax, ecx
        ja     loc_4_2A7C
        inc    ebx
        cmp    ebx, dword ptr [ebp - 8]
        jb     loc_4_2A19
loc_4_2A7C:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2100
        out    dx, ax
        mov    eax, 0x8000
        out    dx, ax
        mov    eax, 2
        out    dx, ax
        mov    eax, 3
        out    dx, ax
        mov    eax, 0x400
        sub    eax, dword ptr [ebp - 4]
        shr    eax, 2
        cmp    eax, ecx
        jbe    loc_4_2AD6
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   ecx
        mov    cx, word ptr [esi]
        push   eax
        push   0                             ; -> loc_1_0C04
        mov    edx, dword ptr [esi + 0x108]
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_2AD6:
        mov    ecx, dword ptr [0xc]          ; -> obj1:0x0980
        mov    eax, 3
        shr    dword ptr [ebp - 0xc], 8
        add    ecx, 0x16
loc_4_2AE8:
        shr    dword ptr [ebp - 0xc], 4
        mov    edx, dword ptr [ebp - 0xc]
        dec    ecx
        and    edx, 0xf
        dec    eax
        mov    dl, byte ptr [edx]            ; -> loc_1_09A0
        mov    byte ptr [ecx + 1], dl
        jne    loc_4_2AE8
        mov    eax, 3
        pop    edi
        or     byte ptr [esi + 0x14c], 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    ecx, dword ptr [ebp + 0x14]
        test   cl, 0x3f
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x14c], 2
        jne    loc_4_2B57
        cmp    ecx, 0x100
        jb     loc_4_2B57
        cmp    ecx, 0x3c0
        ja     loc_4_2B57
        mov    eax, dword ptr [ebp + 0x18]
        sub    eax, ecx
        cmp    eax, 0x1f
        jae    loc_4_2B96
loc_4_2B57:
        mov    eax, ecx
        and    eax, 0xffff0fff
        cmp    eax, 0xc00
        jne    loc_4_2EA3
        mov    eax, dword ptr [ebp + 0x18]
        sub    eax, ecx
        cmp    eax, 0x3f
        jb     loc_4_2EA3
        lea    ecx, [esi + 4]
        mov    eax, 0x40
        mov    word ptr [esi + 0x14c], 1
        add    word ptr [esi + 2], 0x38
loc_4_2B8D:
        add    dword ptr [ecx], 0x38
        add    ecx, 4
        dec    eax
        jne    loc_4_2B8D
loc_4_2B96:
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   esi
        call   sub_4_2590
        cmp    ax, 0xff
        je     loc_4_2BBE
        mov    cx, 0xff
        pop    edi
        cmp    cx, ax
        pop    esi
        sbb    eax, eax
        pop    ebx
        and    eax, 0xfffffffb
        mov    esp, ebp
        add    eax, 9
        pop    ebp
        ret    0x1c
loc_4_2BBE:
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, 6
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x38]
        in     al, dx
        mov    edx, dword ptr [esi + 0x14]
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [esi + 0x10c], al
        mov    eax, 0xc
        out    dx, ax
        mov    eax, 0x303
        out    dx, ax
        mov    edx, ecx
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0x40]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xfb
        out    dx, al
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 0x34]
        xor    ebx, ebx
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xcf
        out    dx, al
loc_4_2C25:
        mov    edx, dword ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xc0
        cmp    al, 0xc0
        je     loc_4_2C52
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        inc    ebx
        call   sub_4_0260
        cmp    ebx, 0xa28
        jb     loc_4_2C25
loc_4_2C52:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 0x24]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    ebx, 0x7d0
        mov    eax, dword ptr [esi + 0x104]
        and    ax, 0xfffe
        out    dx, ax
        or     al, 1
        out    dx, ax
loc_4_2C87:
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    loc_4_2C87
        mov    edx, dword ptr [esi + 0x1c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x40
        je     loc_4_2CB6
        mov    eax, 0xa
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_2CB6:
        push   edi
        push   esi
        call   sub_4_2590
        cmp    ax, 0xff
        je     loc_4_2CD1
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_2CD1:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0xd00c
        out    dx, ax
        mov    eax, 0x2300
        mov    ecx, dword ptr [ebp + 0x1c]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, 0x86
        out    dx, ax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        push   ecx
        push   esi
        call   sub_4_2770
        test   byte ptr [esi + 0x14c], 1
        mov    dword ptr [ebp - 4], eax
        je     loc_4_2D12
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
loc_4_2D12:
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    ebx, dword ptr [esi + 0x24]
        mov    eax, 0xffff
        mov    edx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        mov    eax, 0xfefe
        out    dx, ax
        mov    eax, 0x700
        xor    ecx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 1
        out    dx, ax
loc_4_2D42:
        lea    edx, [ebx + ecx]
        mov    al, byte ptr [edi]
        inc    ecx
        inc    edi
        out    dx, al
        cmp    ecx, 6
        jb     loc_4_2D42
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 0x44]
loc_4_2D54:
        lea    edx, [edi + ecx]
        xor    al, al
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     loc_4_2D54
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    ecx, 3
        jb     loc_4_2E95
        cmp    ecx, 8
        je     loc_4_2E95
        cmp    ecx, 0xd
        je     loc_4_2E95
        cmp    ecx, 0xe
        je     loc_4_2E95
        cmp    ecx, 0xf
        ja     loc_4_2E95
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0x38]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    eax, 0xf
        cmp    eax, ecx
        je     loc_4_2DBF
        mov    eax, 7
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_2DBF:
        mov    edx, dword ptr [esi + 0x3c]
        mov    edi, 0x2000
        in     al, dx
        mov    edx, dword ptr [esi + 0xc]
        mov    byte ptr [esi + 0x104], al
        mov    ecx, dword ptr [esi + 0x104]
        mov    eax, 5
        out    dx, ax
        shr    cl, 5
        shl    edi, cl
        mov    ecx, edi
        mov    edx, edi
        shr    ecx, 2
        mov    dword ptr [esi + 0x14e], edi
        shr    edx, 1
        add    edx, ecx
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0xc], edx
        shr    dword ptr [ebp - 0xc], 4
        mov    ax, word ptr [ebp - 0xc]
        mov    edx, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0x15a], 0x100
        out    dx, ax
        mov    eax, edi
        mov    edx, dword ptr [esi + 0x3c]
        shr    eax, 4
        mov    dword ptr [esi + 0x152], ecx
        mov    dword ptr [esi + 0x15e], 0xa
        dec    ax
        out    dx, ax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 4
        mov    dword ptr [esi + 0x156], edx
        mov    edx, dword ptr [esi + 0xc]
        out    dx, ax
        mov    edx, ebx
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    edx, dword ptr [esi + 0x38]
        in     al, dx
        mov    ecx, dword ptr [esi + 0x104]
        mov    word ptr [esi + 0x182], cx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0x85
        out    dx, al
        mov    edx, dword ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xf0
        pop    edi
        out    dx, al
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 3
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_2E95:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_2EA3:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2EC0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0x61
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [esi], ax
        mov    ebx, dword ptr [ebp + 0x18]
        mov    word ptr [esi + 2], bx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x108], ecx
        mov    byte ptr [esi + 0x185], al
        mov    dword ptr [esi + 0x11e], edi
        mov    dword ptr [edi], eax
        mov    word ptr [edi + 4], ax
loc_4_2EFE:
        lea    ecx, [ebx + eax]
        inc    eax
        cmp    eax, 0x40
        mov    dword ptr [esi + eax*4], ecx
        jl     loc_4_2EFE
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 0x24]
        mov    ecx, dword ptr [ebp + 0x20]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   edx
        push   ebx
        push   eax
        push   edi
        push   esi
        call   sub_4_2B20
        mov    edi, eax
        mov    ecx, dword ptr [esi + 0x108]
        push   ecx
        call   sub_4_0010
        cmp    edi, 4
        jge    loc_4_2F99
        test   edi, edi
        jl     loc_4_2F99
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 0x24]
        mov    eax, 0xffff
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        mov    eax, 0xfefe
        mov    word ptr [esi], 2
        mov    ecx, dword ptr [ebp + 0x2c]
        out    dx, ax
        mov    eax, 0x700
        out    dx, ax
        mov    eax, dword ptr [edi*4]        ; -> obj1:0x0974
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    edx, dword ptr [esi + 0xc]
        pop    edi
        out    dx, ax
        mov    ax, word ptr [esi + 0x14c]
        pop    esi
        and    ax, 2
        pop    ebx
        cmp    ax, 1
        pop    ebp
        sbb    eax, eax
        neg    eax
        ret    0x28
loc_4_2F99:
        cmp    edi, 0xb
        jae    loc_4_2FB0
        test   edi, edi
        jl     loc_4_2FB0
        mov    eax, dword ptr [edi*4]        ; -> obj1:0x0974
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [ecx], eax
        jmp    loc_4_2FB9
loc_4_2FB0:
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [ecx], 0            ; -> loc_1_0C30
loc_4_2FB9:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x28
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2FD0:
        push   ebp
        mov    eax, 0xe
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0x61
        mov    dword ptr [esi + 0x13e], 0
        mov    word ptr [esi], 2
        mov    edx, dword ptr [esi + 0x14]
        out    dx, ax
        mov    eax, 0x2300
        out    dx, ax
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        push   0x61
        mov    ecx, dword ptr [esi + 0x108]
        push   ecx
        call   sub_4_0260
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 3
        out    dx, ax
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        push   esi
        call   sub_4_3FF0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3040:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, 4
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    dword ptr [esi + 0x13e], edi
        mov    edx, dword ptr [esi + 0xc]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xcf
        mov    edx, dword ptr [esi + 0x34]
        out    dx, al
loc_4_3070:
        mov    edx, dword ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0xc0
        cmp    al, 0xc0
        je     loc_4_309D
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        inc    edi
        call   sub_4_0260
        cmp    edi, 0xa28
        jl     loc_4_3070
loc_4_309D:
        cmp    byte ptr [esi + 0x10e], 0x64
        jne    loc_4_3104
        mov    edx, dword ptr [esi + 0x2c]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        and    eax, 0xffe7
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
loc_4_30C4:
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x62
        je     loc_4_30EB
        inc    edi
        cmp    edi, 0xfa0
        jl     loc_4_30C4
loc_4_30EB:
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        or     al, 8
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
loc_4_3104:
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3FF0
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_3120:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    word ptr [esi + 0x11c], di
        mov    ebx, dword ptr [ebp + 0xc]
        lea    eax, [esi + 0x122]
        cmp    ebx, edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        je     loc_4_315C
loc_4_3144:
        mov    eax, dword ptr [esi + 0x11e]
        add    eax, edi
        add    edi, 6
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_3170
        dec    ebx
        jne    loc_4_3144
loc_4_315C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, 0xffffffff
        mov    edi, dword ptr [ebp + 0xc]
loc_4_3188:
        mov    dl, byte ptr [edi]
        inc    edi
        mov    ecx, 8
loc_4_3190:
        mov    eax, esi
        mov    bl, dl
        shr    dl, 1
        and    eax, 0x80000000
        shr    eax, 0x1f
        and    ebx, 1
        xor    eax, ebx
        add    esi, esi
        test   eax, eax
        je     loc_4_31B2
        xor    esi, 0x4c11db6
        or     esi, eax
loc_4_31B2:
        dec    ecx
        jne    loc_4_3190
        dec    dword ptr [ebp - 4]
        jne    loc_4_3188
        shr    esi, 0x1a
        mov    ecx, 0x3f
        sub    ecx, esi
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 0x122], bl
        inc    word ptr [eax + 0x11c]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_31F0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x13e], eax
        mov    byte ptr [esi + 0x146], al
        mov    eax, 4
        mov    edx, dword ptr [esi + 0xc]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x80
        mov    ax, word ptr [esi + 0x182]
        je     loc_4_3296
        test   al, 0xc0
        je     loc_4_326B
        test   al, 0xc
        je     loc_4_323D
        lea    edi, [esi + 0x10e]
        mov    byte ptr [edi], 0xa
        jmp    loc_4_32DB
loc_4_323D:
        mov    edx, dword ptr [esi + 0x2c]
        lea    edi, [esi + 0x10e]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x20
        je     loc_4_325B
        mov    byte ptr [edi], 0x64
        jmp    loc_4_32DB
loc_4_325B:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        push   0                             ; -> obj1:0x0A10
        jmp    loc_4_32CD
loc_4_326B:
        mov    edx, dword ptr [esi + 0x2c]
        lea    edi, [esi + 0x10e]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x20
        je     loc_4_3286
        mov    byte ptr [edi], 0x64
        jmp    loc_4_32DB
loc_4_3286:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        push   0                             ; -> obj1:0x0A10
        jmp    loc_4_32CD
loc_4_3296:
        test   al, 0xc0
        je     loc_4_32B9
        lea    edi, [esi + 0x10e]
        test   al, 0xc
        je     loc_4_32A9
        mov    byte ptr [edi], 0xa
        jmp    loc_4_32DB
loc_4_32A9:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        push   0                             ; -> obj1:0x0A10
        jmp    loc_4_32CD
loc_4_32B9:
        lea    edi, [esi + 0x10e]
        push   1
        push   0
        push   0
        push   0                             ; -> loc_1_09E8
        mov    byte ptr [edi], 0
loc_4_32CD:
        push   1
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0080
loc_4_32DB:
        mov    al, byte ptr [edi]
        cmp    al, 0xa
        jne    loc_4_3313
        mov    dword ptr [esi + 0x15e], 0xa
        mov    dword ptr [esi + 0x132], 0x158
        mov    dword ptr [esi + 0x136], 0x10
        mov    word ptr [esi + 0x118], 0x3c
        mov    word ptr [esi + 0x11a], 0x5ea
        jmp    loc_4_3376
loc_4_3313:
        cmp    al, 0x64
        jne    loc_4_336C
        mov    dword ptr [esi + 0x15e], 0x64
        mov    dword ptr [esi + 0x132], 0x7f48
        mov    dword ptr [esi + 0x136], 0x200
        mov    word ptr [esi + 0x118], 0x3c
        mov    word ptr [esi + 0x11a], 0x5ea
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 1
        sbb    al, al
        inc    al
        mov    byte ptr [esi + 0x185], al
        cmp    byte ptr [edi], 0x64
        jne    loc_4_3376
        test   al, al
        je     loc_4_3376
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, 0x81
        out    dx, ax
        jmp    loc_4_3376
loc_4_336C:
        mov    dword ptr [esi + 0x15e], 0
loc_4_3376:
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3390:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp - 1], al
        test   edi, 2
        mov    dword ptr [esi + 0x13e], eax
        mov    dword ptr [esi + 0x110], edi
        je     loc_4_33BC
        mov    byte ptr [ebp - 1], 2
loc_4_33BC:
        mov    eax, edi
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        jne    loc_4_33CE
        test   edi, 8
        je     loc_4_33D2
loc_4_33CE:
        or     byte ptr [ebp - 1], 4
loc_4_33D2:
        mov    eax, edi
        and    eax, 0x10
        mov    dword ptr [ebp - 0xc], eax
        je     loc_4_33E0
        or     byte ptr [ebp - 1], 7
loc_4_33E0:
        test   edi, 0x20
        je     loc_4_33EC
        or     byte ptr [ebp - 1], 8
loc_4_33EC:
        push   esi
        call   sub_4_3FF0
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 0x24]
        mov    eax, 0xffff
        mov    ecx, dword ptr [esi + 0x11e]
        xor    ebx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 1
        out    dx, ax
loc_4_3415:
        mov    al, byte ptr [ecx]
        mov    edx, dword ptr [esi + 0x24]
        add    edx, ebx
        inc    ecx
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     loc_4_3415
        test   edi, 8
        jne    loc_4_3452
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_3452
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_3463
        lea    edi, [esi + 0x122]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x44]
loc_4_3443:
        lea    edx, [ecx + ebx]
        mov    al, byte ptr [edi]
        inc    ebx
        inc    edi
        out    dx, al
        cmp    ebx, 8
        jl     loc_4_3443
        jmp    loc_4_3463
loc_4_3452:
        xor    edi, edi
        mov    ecx, dword ptr [esi + 0x44]
loc_4_3457:
        lea    edx, [ecx + edi]
        mov    al, 0xff
        out    dx, al
        inc    edi
        cmp    edi, 8
        jl     loc_4_3457
loc_4_3463:
        cmp    byte ptr [esi + 0x10e], 0x64
        jne    loc_4_3618
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        or     al, 1
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
loc_4_3495:
        mov    edx, dword ptr [esi + 0x2c]
        xor    ebx, ebx
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        and    eax, 0xffe7
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
loc_4_34B3:
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x62
        je     loc_4_34DA
        inc    ebx
        cmp    ebx, 0xfa0
        jl     loc_4_34B3
loc_4_34DA:
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        mov    edx, dword ptr [esi + 0x2c]
        or     al, 0x10
        and    eax, 0xffff
        out    dx, ax
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0x104]
        mov    edx, dword ptr [esi + 0x2c]
        or     al, 8
        out    dx, ax
loc_4_3510:
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        and    al, 0x62
        cmp    al, 0x62
        je     loc_4_353F
        inc    ebx
        cmp    ebx, 0x1312d00
        jl     loc_4_3510
loc_4_353F:
        cmp    ebx, 0x1312d00
        jl     loc_4_3551
        inc    edi
        cmp    edi, 5
        jl     loc_4_3495
loc_4_3551:
        cmp    edi, 5
        jl     loc_4_3618
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x20
        jne    loc_4_3577
        push   1
        push   0
        push   0
        push   0                             ; -> loc_1_0A3C
        jmp    loc_4_3597
loc_4_3577:
        test   al, 2
        jne    loc_4_3588
        push   1
        push   0
        push   0
        push   0                             ; -> loc_1_0A74
        jmp    loc_4_3597
loc_4_3588:
        test   al, 0x40
        jne    loc_4_35A5
        push   1
        push   0
        push   0
        push   0                             ; -> obj1:0x0AA4
loc_4_3597:
        push   1
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0080
loc_4_35A5:
        mov    edx, dword ptr [esi + 0x2c]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        and    eax, 0xffe7
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
loc_4_35C3:
        push   0x61
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        test   al, 0x62
        je     loc_4_35EA
        inc    edi
        cmp    edi, 0xfa0
        jl     loc_4_35C3
loc_4_35EA:
        mov    edx, dword ptr [esi + 0x2c]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        mov    eax, dword ptr [esi + 0x104]
        or     al, 8
        mov    edx, dword ptr [esi + 0x2c]
        out    dx, ax
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3618:
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        mov    edx, dword ptr [esi + 0x34]
        or     al, byte ptr [ebp - 1]
        or     al, 0x30
        out    dx, al
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    eax, dword ptr [esi + 0x104]
        sub    al, 0x30
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        test   al, 0x30
        je     loc_4_3668
        mov    edx, dword ptr [esi + 0xc]
        xor    eax, eax
        out    dx, ax
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3668:
        mov    word ptr [esi], 8
        xor    eax, eax
        mov    edx, dword ptr [esi + 0xc]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x200
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        jne    loc_4_36AF
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_36AF:
        mov    edx, dword ptr [esi + 0x54]
        in     eax, dx
        mov    dword ptr [esi + 0x104], eax
        test   eax, 0x80000000
        jne    loc_4_3707
        and    eax, 0x7ffffffe
        inc    dword ptr [esi + 0x17a]
        shr    eax, 1
        cmp    eax, dword ptr [ebp + 0x14]
        jae    loc_4_3707
        mov    eax, 0x11
        inc    dword ptr [esi + 0x176]
        mov    byte ptr [esi + 0x14a], 1
        mov    dword ptr [esi + 0x13e], 2
        mov    edx, dword ptr [esi + 0x24]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        pop    esi
        out    dx, ax
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3707:
        mov    dl, byte ptr [esi + 0x146]
        cmp    dl, 0xff
        lea    eax, [edx + 1]
        mov    byte ptr [esi + 0x146], al
        jne    loc_4_376D
        mov    edx, dword ptr [esi + 0x68]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        cmp    al, 0xff
        mov    byte ptr [esi + 0x146], al
        jne    loc_4_3763
        mov    eax, 0x11
        inc    dword ptr [esi + 0x176]
        mov    dword ptr [esi + 0x13e], 2
        mov    byte ptr [esi + 0x14a], 1
        mov    edx, dword ptr [esi + 0x24]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x2c]
        pop    esi
        out    dx, ax
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3763:
        test   al, al
        je     loc_4_376D
        inc    dword ptr [esi + 0x17a]
loc_4_376D:
        mov    ecx, dword ptr [esi + 0x44]
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, ecx
        cmp    eax, 0x3c
        ja     loc_4_377F
        mov    eax, 0x3c
loc_4_377F:
        out    dx, eax
        mov    edx, dword ptr [esi + 0x34]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ax, di
        test   byte ptr [esi + 0x14c], 4
        out    dx, ax
        je     loc_4_37C0
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2000
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x114]
        out    dx, ax
        mov    eax, edi
        and    eax, 0xfffffffc
        push   eax
        push   ebx
        push   ecx
        call   sub_1_05BC
loc_4_37B4:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2100
        out    dx, ax
        jmp    loc_4_37D0
loc_4_37C0:
        mov    eax, edi
        mov    ebx, dword ptr [ebp + 0xc]
        shr    eax, 2
        push   eax
        push   ebx
        push   ecx
        call   sub_1_03C9
loc_4_37D0:
        mov    eax, edi
        and    eax, 0xfffffffc
        add    eax, ebx
        test   edi, 2
        je     loc_4_37F8
        mov    cx, word ptr [eax]
        add    eax, 2
        test   edi, 1
        mov    word ptr [ebp - 4], cx
        je     loc_4_3805
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2], al
        jmp    loc_4_3805
loc_4_37F8:
        test   edi, 1
        je     loc_4_380C
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
loc_4_3805:
        mov    edx, dword ptr [esi + 0x44]
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
loc_4_380C:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [esi + 0x142], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_4_3820:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [esi + 0x34]
        mov    ax, di
        test   byte ptr [esi + 0x14c], 4
        out    dx, ax
        je     loc_4_386C
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2000
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x114]
        out    dx, ax
        mov    eax, edi
        and    eax, 0xfffffffc
        push   eax
        push   ebx
        push   ecx
        call   sub_1_05BC
loc_4_3860:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2100
        out    dx, ax
        jmp    loc_4_387F
loc_4_386C:
        mov    eax, edi
        mov    ebx, dword ptr [ebp + 0xc]
        shr    eax, 2
        mov    ecx, dword ptr [esi + 0x44]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_03C9
loc_4_387F:
        mov    eax, edi
        and    eax, 0xfffffffc
        add    eax, ebx
        test   edi, 2
        je     loc_4_38B7
        mov    cx, word ptr [eax]
        add    eax, 2
        test   edi, 1
        mov    word ptr [ebp - 4], cx
        je     loc_4_38A5
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2], al
loc_4_38A5:
        mov    edx, dword ptr [esi + 0x44]
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_38B7:
        test   edi, 1
        je     loc_4_38CB
        mov    al, byte ptr [eax]
        mov    edx, dword ptr [esi + 0x44]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
loc_4_38CB:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_38E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x142]
        cmp    ecx, 0x3c
        jae    loc_4_3918
        mov    eax, 0x3c
        mov    edx, dword ptr [esi + 0x34]
        sub    eax, ecx
        out    dx, ax
        lea    ecx, [eax + 3]
        shr    ecx, 2
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_4_3918
        mov    edx, dword ptr [esi + 0x44]
loc_4_390E:
        xor    eax, eax
        out    dx, eax
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        jne    loc_4_390E
loc_4_3918:
        mov    dword ptr [esi + 0x142], 0
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3940:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 0x44]
        mov    ecx, dword ptr [ebp + 0x10]
        in     eax, dx
        mov    dword ptr [ebx + 0x104], eax
        mov    dword ptr [ebp - 8], eax
        and    eax, 0xfff
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebx + 0x12a], eax
        cmp    eax, ecx
        jb     loc_4_3970
        mov    eax, ecx
loc_4_3970:
        mov    dword ptr [ebx + 0x12e], eax
        xor    eax, eax
        mov    edx, dword ptr [ebx + 0x3c]
        out    dx, ax
        mov    edx, dword ptr [ebx + 4]
        in     ax, dx
        mov    word ptr [ebx + 0x104], ax
        mov    edx, dword ptr [ebx + 4]
        in     ax, dx
        mov    word ptr [ebx + 0x104], ax
        test   byte ptr [ebx + 0x14c], 4
        je     loc_4_39CE
        mov    edx, dword ptr [ebx + 0x14]
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, dword ptr [ebx + 0x12e]
        mov    ecx, dword ptr [ebx + 0x114]
        and    eax, 0xfffffffc
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   edx
        call   sub_1_05BC
loc_4_39C2:
        mov    edx, dword ptr [ebx + 0x14]
        mov    eax, 0x2100
        out    dx, ax
        jmp    loc_4_39E5
loc_4_39CE:
        mov    eax, dword ptr [ebx + 0x12e]
        mov    edx, dword ptr [ebp + 0xc]
        shr    eax, 2
        mov    ecx, dword ptr [ebx + 0x44]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0406
loc_4_39E5:
        mov    eax, dword ptr [ebx + 0x12e]
        mov    ecx, eax
        and    ecx, 0xfffffffc
        add    ecx, dword ptr [ebp + 0xc]
        test   al, 2
        je     loc_4_3A13
        mov    edx, dword ptr [ebx + 0x44]
        inc    ecx
        in     al, dx
        inc    ecx
        mov    byte ptr [ebx + 0x104], al
        mov    byte ptr [ecx - 2], al
        mov    edx, dword ptr [ebx + 0x44]
        in     al, dx
        mov    byte ptr [ebx + 0x104], al
        mov    byte ptr [ecx - 1], al
loc_4_3A13:
        test   byte ptr [ebx + 0x12e], 1
        je     loc_4_3A28
        mov    edx, dword ptr [ebx + 0x44]
        in     al, dx
        mov    byte ptr [ebx + 0x104], al
        mov    byte ptr [ecx], al
loc_4_3A28:
        shr    dword ptr [ebp - 8], 0x10
        mov    ecx, dword ptr [ebx + 0x110]
        test   cl, 0x20
        je     loc_4_3AD6
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [ebx + 0x132], eax
        je     loc_4_3AD6
        mov    edx, eax
        and    edx, 8
        jne    loc_4_3A84
        test   dword ptr [ebx + 0x136], eax
        jne    loc_4_3A84
        test   al, 0x40
        mov    eax, dword ptr [ebp + 0x14]
        je     loc_4_3A72
        pop    edi
        mov    dword ptr [eax], 0x208
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3A72:
        pop    edi
        mov    dword ptr [eax], 0x201
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3A84:
        mov    eax, dword ptr [ebx + 0x136]
        mov    ecx, dword ptr [ebp - 8]
        or     eax, 8
        and    ecx, eax
        cmp    ecx, eax
        jne    loc_4_3AAB
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0x206
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3AAB:
        mov    eax, dword ptr [ebp + 0x14]
        test   edx, edx
        je     loc_4_3AC4
        pop    edi
        mov    dword ptr [eax], 0x202
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3AC4:
        pop    edi
        mov    dword ptr [eax], 0x204
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3AD6:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x11a]
        cmp    eax, dword ptr [ebx + 0x12a]
        jae    loc_4_3B10
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0x208
        test   byte ptr [ebx + 0x110], 0x20
        je     loc_4_3B05
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B05:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B10:
        mov    eax, dword ptr [ebx + 0x11e]
        mov    edx, dword ptr [ebp + 0xc]
        test   byte ptr [edx], 1
        jne    loc_4_3B8B
        mov    eax, dword ptr [ebp - 8]
        and    al, 3
        cmp    al, 1
        jne    loc_4_3B66
        test   cl, 1
        je     loc_4_3B41
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B41:
        mov    eax, dword ptr [ebp + 0x14]
        test   cl, 0x10
        mov    dword ptr [eax], 0x100
        je     loc_4_3B5B
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B5B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B66:
        mov    eax, dword ptr [ebp + 0x14]
        test   cl, 0x10
        mov    dword ptr [eax], 0x104
        je     loc_4_3B80
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B80:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3B8B:
        mov    edx, dword ptr [ebp - 8]
        and    dl, 3
        cmp    dl, 3
        jne    loc_4_3BD5
        test   cl, 2
        je     loc_4_3BB0
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3BB0:
        mov    eax, dword ptr [ebp + 0x14]
        test   cl, 0x10
        mov    dword ptr [eax], 0x101
        je     loc_4_3BCA
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3BCA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3BD5:
        xor    edx, edx
        cmp    word ptr [ebx + 0x11c], dx
        jbe    loc_4_3C08
loc_4_3BE0:
        add    eax, 6
        mov    ecx, 6
        mov    esi, eax
        mov    edi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_3BFA
        test   byte ptr [ebx + 0x110], 4
        jne    loc_4_3C26
loc_4_3BFA:
        inc    edx
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x11c]
        cmp    ecx, edx
        jg     loc_4_3BE0
loc_4_3C08:
        mov    eax, dword ptr [ebx + 0x110]
        test   al, 0x18
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0x102
        jne    loc_4_3C3B
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3C26:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3C3B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3C50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        test   edi, edi
        jne    loc_4_3C66
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_4_3C66:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ebx, dword ptr [ebp + 0x10]
        test   bl, 1
        mov    edx, dword ptr [esi + 0x3c]
        out    dx, ax
        mov    edx, dword ptr [esi + 4]
        in     ax, dx
        mov    word ptr [esi + 0x104], ax
        je     loc_4_3C99
        mov    edx, dword ptr [esi + 0x44]
        inc    ebx
        in     al, dx
        dec    edi
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [ebx - 1], al
loc_4_3C99:
        test   bl, 2
        je     loc_4_3CBD
        mov    edx, dword ptr [esi + 0x44]
        inc    ebx
        in     al, dx
        inc    ebx
        mov    byte ptr [esi + 0x104], al
        sub    edi, 2
        mov    byte ptr [ebx - 2], al
        mov    edx, dword ptr [esi + 0x44]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [ebx - 1], al
loc_4_3CBD:
        test   byte ptr [esi + 0x14c], 4
        je     loc_4_3CEF
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, edi
        mov    ecx, dword ptr [esi + 0x114]
        and    eax, 0xfffffffc
        push   eax
        push   ecx
        push   ebx
        call   sub_1_05BC
loc_4_3CE3:
        mov    edx, dword ptr [esi + 0x14]
        mov    eax, 0x2100
        out    dx, ax
        jmp    loc_4_3CFF
loc_4_3CEF:
        mov    eax, edi
        mov    ecx, dword ptr [esi + 0x44]
        shr    eax, 2
        push   eax
        push   ebx
        push   ecx
        call   sub_1_0406
loc_4_3CFF:
        mov    eax, edi
        and    eax, 0xfffffffc
        add    ebx, eax
        test   edi, 2
        je     loc_4_3D2A
        mov    edx, dword ptr [esi + 0x44]
        inc    ebx
        in     al, dx
        inc    ebx
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [ebx - 2], al
        mov    edx, dword ptr [esi + 0x44]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [ebx - 1], al
loc_4_3D2A:
        test   edi, 1
        je     loc_4_3D3E
        mov    edx, dword ptr [esi + 0x44]
        in     al, dx
        mov    byte ptr [esi + 0x104], al
        mov    byte ptr [ebx], al
loc_4_3D3E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3D50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x162
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0x30
        pop    edi
        pop    ebp
        ret    4
sub_4_3D70:
        push   ebp
        mov    ecx, 0xc
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x152
        mov    edi, dword ptr [ebp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_3D90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     loc_4_3E12
        mov    eax, dword ptr [esi + 0x13e]
        test   eax, eax
        jle    loc_4_3E12
        dec    eax
        mov    dword ptr [esi + 0x13e], eax
        test   eax, eax
        jg     loc_4_3E12
        push   esi
        call   sub_4_2FD0
        inc    dword ptr [esi + 0x17e]
        mov    eax, dword ptr [esi + 0x110]
        push   eax
        push   esi
        call   sub_4_3390
        test   eax, eax
        je     loc_4_3DEC
        push   0
        mov    eax, dword ptr [esi + 0x108]
        push   0
        push   0xa
        push   0                             ; -> obj1:0x0C4C
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebp
        ret    4
loc_4_3DEC:
        push   0x10
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_1260
        cmp    eax, -1
        jge    loc_4_3E12
loc_4_3DFF:
        push   0
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_4_1260
        cmp    eax, -1
        jl     loc_4_3DFF
loc_4_3E12:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh
        db  0FFh, 080h, 06Ah, 001h, 000h, 000h, 066h, 0C7h, 000h, 008h, 000h, 05Dh, 0C2h, 008h, 000h, 08Dh
        db  049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x108]
        push   eax
        call   0
        mov    edx, dword ptr [edi + 0x64]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 3], 0
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        mov    bl, al
loc_4_3E71:
        test   bl, bl
        je     loc_4_3F11
loc_4_3E79:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        cmp    dword ptr [edi + 0x110], 0
        mov    eax, 2
        je     loc_4_3E95
        push   esi
        call   sub_4_4290
loc_4_3E95:
        cmp    eax, 1
        jne    loc_4_3ED5
        mov    edx, dword ptr [edi + 0x1c]
        mov    eax, 5
        out    dx, ax
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        mov    edx, dword ptr [edi + 0x1c]
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        dec    bl
        mov    edx, dword ptr [edi + 0x1c]
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        mov    edx, dword ptr [edi + 0x1c]
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        jmp    loc_4_3F15
loc_4_3ED5:
        cmp    eax, 2
        je     loc_4_3EE3
        test   eax, eax
        jge    loc_4_3F15
        jmp    loc_4_3F96
loc_4_3EE3:
        mov    edx, dword ptr [edi + 0x64]
        in     ax, dx
        mov    word ptr [edi + 0x104], ax
        test   al, al
        je     loc_4_3EFF
        mov    edx, dword ptr [edi + 0x1c]
        mov    eax, 5
        out    dx, ax
        jmp    loc_4_3EE3
loc_4_3EFF:
        mov    edx, dword ptr [edi + 0x24]
        mov    eax, 0x700
        xor    bl, bl
        inc    dword ptr [edi + 0x172]
        out    dx, ax
loc_4_3F11:
        mov    byte ptr [ebp - 2], 1
loc_4_3F15:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_3F64
        mov    dword ptr [edi + 0x13e], 0
        mov    byte ptr [edi + 0x14a], 0
loc_4_3F2C:
        push   0
        push   esi
        call   sub_4_1260
        mov    byte ptr [edi + 0x14b], al
        and    eax, 0xff
        cmp    eax, -1
        jl     loc_4_3F2C
        cmp    byte ptr [edi + 0x14b], 0
        jne    loc_4_3F5C
        xor    cl, cl
        mov    eax, 1
        mov    byte ptr [ebp - 1], al
        mov    byte ptr [ebp - 3], al
        jmp    loc_4_3F6C
loc_4_3F5C:
        mov    byte ptr [ebp - 1], 0
        mov    cl, 1
        jmp    loc_4_3F6C
loc_4_3F64:
        mov    ecx, 1
        mov    byte ptr [ebp - 1], cl
loc_4_3F6C:
        test   bl, bl
        jne    loc_4_3F85
        mov    edx, dword ptr [edi + 0x64]
        in     ax, dx
        mov    bx, ax
        mov    word ptr [edi + 0x104], ax
        mov    byte ptr [edi + 0x146], bh
loc_4_3F85:
        test   byte ptr [ebp - 2], cl
        je     loc_4_3E71
        test   bl, bl
        jne    loc_4_3E79
loc_4_3F96:
        mov    edx, dword ptr [edi + 0x24]
        mov    eax, 0x700
        cmp    byte ptr [edi + 0x14a], 0
        out    dx, ax
        jne    loc_4_3FB3
        mov    edx, dword ptr [edi + 0x2c]
        mov    eax, 0x10
        out    dx, ax
loc_4_3FB3:
        mov    eax, dword ptr [edi + 0x108]
        push   eax
        call   sub_4_0010
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 3]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_3FD0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_4_3FE3
        mov    eax, 0x3f
        pop    ebp
        ret    8
loc_4_3FE3:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0x110]
        ret    8
sub_4_3FF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 0x184], 0
        mov    edx, dword ptr [eax + 0x14]
        pop    ebp
        mov    eax, 4
        out    dx, ax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_4010:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 0x184], 1
        mov    edx, dword ptr [eax + 0x14]
        pop    ebp
        mov    eax, 5
        out    dx, ax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_4030:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x12a]
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4050:
        push   ebp
        mov    edx, 8
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 7
loc_4_4064:
        mov    esi, eax
        mov    edi, dword ptr [0]            ; -> loc_1_0CE4
        sar    eax, 4
        and    esi, 0xf
        dec    ecx
        mov    bl, byte ptr [esi + edi]
        dec    edx
        mov    byte ptr [ecx + 1], bl
        jne    loc_4_4064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4090:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x108
        push   ebx
        mov    ecx, dword ptr [ebp + 0x10]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ecx + 0x14], esi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x10], esi
        mov    dword ptr [ecx], esi
        cmp    eax, 3
        mov    dword ptr [ecx + 0x18], 0xffffffff
        mov    dword ptr [ecx + 4], 0xffffffff
        jl     loc_4_4264
        cmp    eax, 0xf
        jg     loc_4_4264
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    ebx, edx
        and    ebx, 0x3f
        jne    loc_4_40EE
        cmp    edx, 0x100
        jl     loc_4_40EE
        cmp    edx, 0x3c0
        jle    loc_4_410D
loc_4_40EE:
        mov    eax, edx
        and    eax, 0xffff0fff
        cmp    eax, 0xc00
        jne    loc_4_4247
        test   ebx, ebx
        jne    loc_4_4247
        mov    edi, 0x38
loc_4_410D:
        lea    eax, [edx + 0x3f]
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0xc], eax
        xor    eax, eax
loc_4_4118:
        lea    ebx, [edx + eax]
        inc    eax
        add    ebx, edi
        cmp    eax, 0x40
        mov    dword ptr [ebp + eax*4 - 0x10c], ebx
        jl     loc_4_4118
        mov    edx, dword ptr [ebp - 0x108]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 0x50
        jne    loc_4_4223
        mov    edx, dword ptr [ebp - 0x108]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 0x48
        jne    loc_4_4223
        mov    edx, dword ptr [ebp - 0x108]
        add    edx, 2
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 8]
        and    al, 0xf0
        cmp    al, 0x50
        jne    loc_4_4223
        mov    edx, dword ptr [ebp - 0x108]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 0x53
        jne    loc_4_4223
        mov    edx, dword ptr [ebp - 0x100]
        mov    eax, 6
        out    dx, ax
        xor    edi, edi
loc_4_418D:
        mov    edx, dword ptr [ebp - 0xc8]
        add    edx, edi
        inc    edi
        in     al, dx
        mov    byte ptr [ebp - 8], al
        and    eax, 0xff
        add    esi, eax
        cmp    edi, 6
        jl     loc_4_418D
        mov    edx, dword ptr [ebp - 0xb0]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        and    eax, 0xff
        add    eax, esi
        cmp    al, 0xff
        jne    loc_4_4219
        mov    dword ptr [ebp - 4], 1
        mov    edx, dword ptr [ebp - 0xf8]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        test   al, 0x40
        je     loc_4_4219
        mov    edx, dword ptr [ebp - 0x100]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0xe0]
        in     ax, dx
        mov    si, ax
        mov    word ptr [ebp - 8], ax
        and    esi, 0xff
        shl    esi, 0x10
        mov    edx, dword ptr [ebp - 0xe8]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff00
        or     esi, eax
        mov    dword ptr [ecx + 0x10], esi
        add    esi, 0x1fff
        mov    dword ptr [ecx + 0x14], esi
loc_4_4219:
        mov    edx, dword ptr [ebp - 0x100]
        xor    eax, eax
        out    dx, ax
loc_4_4223:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_4237
        mov    eax, dword ptr [8]            ; -> loc_1_0CCC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_4237:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_4247:
        mov    eax, dword ptr [4]            ; -> loc_1_0CC8
        add    eax, 0x1e
        push   eax
        push   edx
        call   sub_4_4050
        mov    eax, dword ptr [4]            ; -> loc_1_0CC8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_4264:
        mov    eax, dword ptr [0]            ; -> loc_1_0CC4
        mov    edx, dword ptr [ebp + 8]
        add    eax, 0x16
        push   eax
        push   edx
        call   sub_4_4050
        mov    eax, dword ptr [0]            ; -> loc_1_0CC4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4290:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xb7]
        inc    eax
        mov    dword ptr [esi + 0xb7], eax
        cmp    eax, 0xa
        jbe    loc_4_42CD
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_42CD
        push   0
        push   esi
        call   sub_4_1260
        cmp    eax, -1
        jne    loc_4_42CD
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_42CD:
        lea    edi, [esi + 0xc1]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0xfc
        push   edi
        push   ecx
        call   sub_4_3940
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     loc_4_433F
        add    eax, 3
        cmp    eax, 3
        ja     loc_4_433F
        jmp    dword ptr [eax*4]             ; -> obj4:0x4394
loc_4_42FB:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_4305:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_430F:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_431C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_4650
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_4030
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_433F:
        sub    dword ptr [ebp - 4], 0xe
        mov    ecx, dword ptr [esi + 0x7f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     loc_4_434F
        mov    ecx, eax
loc_4_434F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x20], esi
        push   eax
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x10], 0
        push   ecx
        inc    dword ptr [esi + 0x87]
        lea    ecx, [esi + 0xcf]
        lea    edx, [ebp - 0x20]
        mov    byte ptr [esi + 0x41], 1
        push   ecx
        mov    eax, dword ptr [esi + 0x7b]
        push   0xe
        push   edi
        push   edi
        push   edx
        push   eax
        call   sub_1_00B0
loc_4_4383:
        cmp    dword ptr [ebp - 0x1c], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
        ; ---- dispatch table (4 entries) ----
        dd  loc_4_42FB
        dd  loc_4_4305
        dd  loc_4_430F
        dd  loc_4_431C
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 0x1c]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [esi + 4], 0
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_4_43FB
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        add    edx, 0xe
        mov    ecx, dword ptr [ebp + 0x18]
        push   edx
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        push   ecx
        call   sub_1_054E
loc_4_43F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_43FB:
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp + 0x14]
        lea    edi, [edx + eax]
        cmp    edi, ecx
        jbe    loc_4_4412
        sub    ecx, edx
        mov    dword ptr [ebp - 0x18], ecx
        jmp    loc_4_4415
loc_4_4412:
        mov    dword ptr [ebp - 0x18], eax
loc_4_4415:
        mov    edi, dword ptr [ebp - 0x18]
        add    edx, 0xe
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    dword ptr [ebp - 8], edx
        push   0
        push   ecx
        call   sub_1_0500
loc_4_4431:
        test   edi, edi
        je     loc_4_4484
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        call   sub_1_0524
loc_4_4448:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    ebx, eax
        jb     loc_4_4451
        mov    ebx, eax
loc_4_4451:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_4_3C50
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     loc_4_4484
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_447E:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_4431
loc_4_4484:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, edi
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_44A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        je     loc_4_44C2
loc_4_44AE:
        mov    ecx, dword ptr [eax + 0x24]
        push   eax
        push   esi
        mov    dword ptr [esi + 0x38], ecx
        call   sub_4_44D0
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        jne    loc_4_44AE
loc_4_44C2:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_44D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0500
loc_4_44EC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 0x7f]
        add    ebx, 0xe
        cmp    ebx, eax
        jb     loc_4_44FE
        mov    ebx, eax
loc_4_44FE:
        lea    eax, [ebx - 0xe]
        push   0
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 4], 0xe
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_0500
loc_4_4518:
        lea    ecx, [ebp - 0x14]
        lea    edx, [ebp - 0x10]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        lea    ecx, [esi + 0xc1]
        mov    dword ptr [ebp - 0x10], 0
        push   eax
        push   ebx
        push   ecx
        call   sub_4_45A0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [esi + 0xcf]
        push   eax
        mov    dword ptr [ebp - 0x28], esi
        push   edx
        mov    dword ptr [ebp - 0x20], ecx
        push   0xe
        lea    ecx, [esi + 0xc1]
        push   ecx
        lea    edx, [ebp - 0x28]
        push   ecx
        mov    dword ptr [ebp - 0x18], edi
        push   edx
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_1_00B0
loc_4_4567:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    byte ptr [esi + 0x41], 1
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_044A
loc_4_4580:
        push   esi
        call   sub_4_18F0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_45A0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        lea    edx, [ebp - 4]
        push   ecx
        mov    esi, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0524
loc_4_45C6:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     loc_4_4644
loc_4_45D3:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        lea    edx, [ebx + ecx]
        cmp    edx, dword ptr [ebp + 0xc]
        jae    loc_4_461E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_05BC
loc_4_45F0:
        mov    ecx, dword ptr [ebp + 0x18]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_0548
loc_4_45FE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0524
loc_4_4614:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    loc_4_45D3
loc_4_461E:
        cmp    dword ptr [esi], 0
        je     loc_4_4644
        mov    eax, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [eax]
        sub    esi, eax
        add    ecx, dword ptr [ebp - 4]
        push   esi
        add    eax, dword ptr [ebp + 8]
        push   ecx
        push   eax
        call   sub_1_05BC
loc_4_463D:
        mov    ecx, dword ptr [ebp + 0x18]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_4644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_4650:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   eax, 0x208
        jne    loc_4_469C
        test   eax, 0x206
        je     loc_4_4678
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        inc    dword ptr [eax + 0x9b]
        ret    8
loc_4_4678:
        test   eax, 0x204
        je     loc_4_468C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        ret    8
loc_4_468C:
        test   eax, 0x202
        je     loc_4_469C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x9b]
loc_4_469C:
        pop    ebp
        ret    8
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0xb3  RW-
        db  02Dh, 03Fh, 063h, 06Fh, 06Dh, 06Dh, 065h, 06Eh, 074h, 03Ah, 022h, 05Fh, 048h, 050h, 05Fh, 020h
        db  068h, 070h, 076h, 067h, 02Eh, 063h, 028h, 04Dh, 06Fh, 06Eh, 020h, 041h, 075h, 067h, 020h, 032h
        db  032h, 020h, 031h, 038h, 03Ah, 031h, 037h, 03Ah, 033h, 038h, 020h, 031h, 039h, 039h, 034h, 029h
        db  020h, 041h, 02Eh, 030h, 02Eh, 032h, 020h, 063h, 06Fh, 06Dh, 070h, 06Ch, 069h, 065h, 064h, 028h
        db  053h, 065h, 070h, 020h, 032h, 038h, 020h, 031h, 039h, 039h, 034h, 020h, 031h, 031h, 03Ah, 032h
        db  034h, 03Ah, 032h, 034h, 029h, 022h, 020h, 000h, 02Dh, 03Fh, 063h, 06Fh, 06Dh, 06Dh, 065h, 06Eh
        db  074h, 03Ah, 022h, 05Fh, 048h, 050h, 05Fh, 020h, 068h, 070h, 076h, 067h, 069h, 06Eh, 066h, 06Fh
        db  02Eh, 063h, 028h, 057h, 065h, 064h, 020h, 04Ah, 075h, 06Eh, 020h, 032h, 032h, 020h, 031h, 031h
        db  03Ah, 034h, 033h, 03Ah, 031h, 032h, 020h, 031h, 039h, 039h, 034h, 029h, 020h, 041h, 02Eh, 030h
        db  02Eh, 032h, 020h, 063h, 06Fh, 06Dh, 070h, 06Ch, 069h, 065h, 064h, 028h, 053h, 065h, 070h, 020h
        db  032h, 038h, 020h, 031h, 039h, 039h, 034h, 020h, 031h, 031h, 03Ah, 032h, 034h, 03Ah, 034h, 035h
        db  029h, 022h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x4  RW-
        db  04Ch, 001h, 004h, 032h
VxD__LDATA ENDS
