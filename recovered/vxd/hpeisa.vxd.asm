; HPEISA -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 6843 instructions recovered by recursive descent

Declare_Virtual_Device HPEISA, 3, 10, HPEISA_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x87a  R-X
loc_1_0000:
        mov    al, byte ptr [0x668]          ; -> obj1:0x0668
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x668]              ; -> obj1:0x0668
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_4_1B20
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
HPEISA_Control:
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
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
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
sub_1_0596:
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
        db  'HPEISA  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  HPEISA_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 04Dh, 075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h
        db  04Ch, 069h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0688:
        dec    ebp
        popal  
        js     loc_1_06F5
        insd   dword ptr es:[edi], dx
        jne    loc_1_06FC
        dec    ebp
        jne    0x6fe
        je     0x6fd
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_06E5
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    esi
        je     0x71b
        outsd  dx, dword ptr [esi]
        jb     0x712
        inc    ecx
        jb     loc_1_0711
        jae    loc_1_0721
        add    byte ptr [eax], al
        dec    esi
        je     loc_1_072B
        outsd  dx, dword ptr [esi]
        jb     0x722
        inc    ecx
        jb     loc_1_0721
        jae    0x731
        add    byte ptr [eax], al
loc_1_06C0:
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
loc_1_06E5:
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
loc_1_06F5:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
loc_1_06FC:
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
loc_1_0711:
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
loc_1_0721:
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
loc_1_072B:
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_073C:
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
loc_1_0798:
        dec    eax
        push   eax
        xor    dh, byte ptr [edi]
        xor    dh, byte ptr [eax + edi]
        inc    ecx
        and    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        and    byte ptr [ecx + eax*2 + 0x4e], cl
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_0823
        jb     0x7e1
        xor    esi, dword ptr [edx]
        and    byte ptr [eax + edx*2], dl
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 045h, 049h, 053h, 041h, 000h, 000h
loc_1_07C4:
        dec    eax
        push   eax
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
loc_1_07CC:
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [ebp + 0x72], ah
        jb     loc_1_0843
        jb     loc_1_07F6
        jb     loc_1_083D
        jae    loc_1_084E
        popal  
        jb     loc_1_0851
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + esi*2 + 0x72], 0x65
loc_1_07E8:
        or     byte ptr [eax], al
        or     dword ptr [eax], eax
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_07F4:
        inc    esi
        popal  
loc_1_07F6:
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     loc_1_0865
        jae    loc_1_0876
        popal  
        jb     0x879
        add    byte ptr cs:[eax], al
loc_1_0808:
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jb     0x879
        jae    0x88a
        popal  
        jb     0x88d
        add    byte ptr cs:[eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
loc_1_0823:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0831:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_083D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0843:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_084E:
        add    byte ptr [eax], al
        db  000h
loc_1_0851:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0865:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0876:
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
        mov    ebx, dword ptr [0x81c]        ; -> obj1:0x081C
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
        mov    eax, dword ptr [0x81c]        ; -> obj1:0x081C
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x3c70  R-X
        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0020:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        test   ecx, ecx
        jne    loc_4_004C
        mov    eax, dword ptr [0]            ; -> obj1:0x07B8
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
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        mov    ecx, dword ptr [edx + 0x201]
        lea    eax, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   eax
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
        mov    esi, dword ptr [ebp + 8]
        push   edx
        push   0x12
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_05A8
loc_4_0111:
        mov    di, word ptr [ebp + 0xc]
        add    esp, 0x54
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax], di
        je     loc_4_012E
        push   eax
        call   sub_4_27D0
        mov    eax, dword ptr [esi]
        mov    word ptr [eax], di
        or     dword ptr [esi + 0x47], 2
loc_4_012E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 068h, 0E8h, 003h, 000h, 000h, 0E8h
        dd  sub_1_04D0               ; fixup
        db  0FFh, 005h
        dd  obj1:0x0872              ; fixup
        db  0A1h
        dd  obj1:0x0872              ; fixup
        db  0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h
sub_4_0170:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x4b]
        push   ecx
        call   sub_1_0596
loc_4_0210:
        dec    eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0220:
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
        db  08Dh, 064h, 024h, 000h
sub_4_0270:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        in     ax, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0280:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    al, byte ptr [esp + 0x10]
        out    dx, al
        pop    ebp
        ret    0xc
sub_4_0290:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ax, word ptr [esp + 0x10]
        mov    ebp, esp
        out    dx, ax
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3810
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_02D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 0xb
        jne    loc_4_0300
        push   esi
        call   sub_4_1960
        test   al, al
        je     loc_4_0300
        mov    dword ptr [esi + 0xb3], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3630
        push   esi
        call   sub_4_1990
loc_4_0300:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0310:
        push   ebp
        mov    eax, 0x15
        mov    ebp, esp
        sub    esp, 0x16c
        mov    word ptr [ebp - 0x2c], ax
        push   ebx
        mov    word ptr [ebp - 0x2a], ax
        push   esi
        mov    dword ptr [ebp - 0x28], 0     ; -> loc_1_0688
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0x10]
        mov    esi, dword ptr [edi + 0x18]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x1c], ecx
        push   ebx
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 0x10], 0
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_0554
loc_4_0360:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_0374
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0374:
        lea    eax, [ebp - 0x16c]
        lea    ecx, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        call   sub_1_0486
loc_4_0389:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0430
        cmp    dword ptr [ebp - 0x16c], 0x4018f022
        jne    loc_4_0430
        mov    bl, byte ptr [ebp - 0xba]
        and    ebx, 0xf
        test   byte ptr [ebp - 0xba], 0x20
        jne    loc_4_03BC
        mov    dword ptr [ebp - 0xc], 1
loc_4_03BC:
        mov    al, byte ptr [ebp - 0xac]
        push   0
        and    eax, 7
        lea    ecx, [ebp - 0x2c]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 4]
        shl    eax, 0xc
        add    eax, 0xc00
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   ecx
        call   sub_1_055A
loc_4_03ED:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_03F9
        mov    eax, dword ptr [ebp - 0x24]
        mov    esi, dword ptr [eax + 4]
loc_4_03F9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0560
loc_4_0402:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [edi + 0xc], ebx
        mov    dword ptr [edi + 0x10], eax
        xor    eax, eax
        mov    dword ptr [edi + 0x14], ecx
        mov    dword ptr [edi + 0x18], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0430:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0560
loc_4_0439:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0450:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x1c
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   ecx
        mov    dword ptr [ebp - 0xc], eax
        push   edx
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [ebp - 4], 0x14
        push   eax
        call   sub_4_0310
        test   eax, eax
        je     loc_4_0495
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0495:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x205], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x21d], edx
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x219], eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx + 0x211], edx
        jne    loc_4_04CB
        mov    dword ptr [ecx + 0x20], 1
        jmp    loc_4_04D8
loc_4_04CB:
        cmp    dword ptr [ebp - 0xc], 1
        jne    loc_4_04D8
        mov    dword ptr [ecx + 0x20], 2
loc_4_04D8:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x209], eax
        mov    dword ptr [ecx + 0x20d], edx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0500:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     loc_4_0521
        mov    eax, 0xc0010002
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0521:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x10]
        test   ecx, ecx
        jne    loc_4_053B
        push   edx
        push   esi
        push   edi
        call   sub_4_0950
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_053B:
        cmp    ecx, 1
        jne    loc_4_0573
        mov    ecx, dword ptr [edi + 0x47]
        test   ecx, ecx
        je     loc_4_0563
        test   cl, 2
        je     loc_4_0551
        mov    eax, 0xc0000001
loc_4_0551:
        test   cl, 1
        je     loc_4_0578
        mov    eax, 0xc001000d
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0563:
        push   edx
        push   esi
        push   edi
        call   sub_4_09A0
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0573:
        mov    eax, 0x10001
loc_4_0578:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0580:
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
        ja     loc_4_05BF
        je     loc_4_07A5
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_4_05E0
        jmp    dword ptr [eax*4]             ; -> obj4:0x08F8
loc_4_05BF:
        cmp    eax, 0x1010102
        je     loc_4_07C4
        cmp    eax, 0x1010103
        je     loc_4_07E3
        cmp    eax, 0x1010104
        je     loc_4_0893
loc_4_05E0:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_4_08A9
loc_4_05EC:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_0601
        mov    eax, 0                        ; -> loc_1_073C
        mov    edi, 0x5c
        jmp    loc_4_08AF
loc_4_0601:
        mov    eax, 0                        ; -> loc_1_06C0
        mov    edi, 0x7c
        jmp    loc_4_08AF
loc_4_0610:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x47]
        test   al, 2
        je     loc_4_0623
        mov    dword ptr [ebp - 0x18], 4
        jmp    loc_4_0635
loc_4_0623:
        mov    dword ptr [ebp - 0x18], 2
        test   al, 1
        jne    loc_4_0635
        mov    dword ptr [ebp - 0x18], 0
loc_4_0635:
        lea    eax, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_4_08AF
loc_4_0642:
        lea    eax, [ebp - 0x1c]
        mov    edi, 4
        jmp    loc_4_08AF
loc_4_064F:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_4_08AF
loc_4_0663:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_4_08AF
loc_4_0677:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_4_08AF
loc_4_068B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_08AF
loc_4_06A4:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_08AF
loc_4_06BD:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_08AF
loc_4_06D1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_08AF
loc_4_06EA:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 4
        call   sub_1_05BC
loc_4_0701:
        mov    byte ptr [ebp - 0x21], 0
        lea    eax, [ebp - 0x24]
        jmp    loc_4_08AF
loc_4_070D:
        mov    eax, 0                        ; -> loc_1_0798
        mov    edi, 0x21
        jmp    loc_4_08AF
loc_4_071C:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_072D
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7b]
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_4_0736
loc_4_072D:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [eax + 8]
loc_4_0736:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_4_08AF
loc_4_0746:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_0754
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x7f]
        jmp    loc_4_075A
loc_4_0754:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 0x21]
loc_4_075A:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_08AF
loc_4_076A:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_08AF
loc_4_077D:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_08AF
loc_4_0791:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_08AF
loc_4_07A5:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x6d
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_05BC
loc_4_07BC:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_08AF
loc_4_07C4:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x67
        lea    eax, [ebp - 0x24]
        push   edi
        push   eax
        mov    edi, 6
        call   sub_1_05BC
loc_4_07DB:
        lea    eax, [ebp - 0x24]
        jmp    loc_4_08AF
loc_4_07E3:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_0839
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x7b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0810
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_08A9
loc_4_0810:
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
loc_4_0826:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0xc], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_08A9
loc_4_0839:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1d]
        mov    ecx, dword ptr [edi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0660
loc_4_084C:
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_4_0867
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_4_08A9
loc_4_0867:
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
loc_4_0884:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_08A9
loc_4_0893:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x20d]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    loc_4_08AF
loc_4_08A9:
        mov    eax, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [ebp - 0x14]
loc_4_08AF:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_08EA
        cmp    esi, 0x1010103
        je     loc_4_08EA
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_4_08DA
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
loc_4_08DA:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05BC
loc_4_08E5:
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_4_08EA:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        ; ---- dispatch table (19 entries) ----
        dd  loc_4_05EC
        dd  loc_4_0610
        dd  loc_4_0642
        dd  loc_4_0642
        dd  loc_4_064F
        dd  loc_4_0663
        dd  loc_4_0677
        dd  loc_4_068B
        dd  loc_4_06A4
        dd  loc_4_06BD
        dd  loc_4_06D1
        dd  loc_4_06EA
        dd  loc_4_070D
        dd  loc_4_071C
        dd  loc_4_0746
        dd  loc_4_076A
        dd  loc_4_077D
        dd  loc_4_05E0
        dd  loc_4_0791
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0950:
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
        call   sub_4_0580
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_09A0:
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
        ja     loc_4_09DD
        je     loc_4_0AAF
        cmp    eax, 0x1010e
        je     loc_4_09EF
        cmp    eax, 0x1010f
        je     loc_4_0A29
        cmp    eax, 0x10112
        je     loc_4_0A9B
loc_4_09DD:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0AEA
loc_4_09EF:
        cmp    edi, 4
        je     loc_4_0A06
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0AEA
loc_4_0A06:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_05BC
loc_4_0A12:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0B10
        jmp    loc_4_0AE8
loc_4_0A29:
        cmp    edi, 4
        je     loc_4_0A40
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0AEA
loc_4_0A40:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_05BC
loc_4_0A4C:
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_4_0A94
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x7f]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_4_0A6E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x7f], eax
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0AEA
loc_4_0A6E:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x21]
        jne    loc_4_0A8C
        mov    eax, dword ptr [ecx + 0x21]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_4_0A8C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x21], eax
        call   sub_4_0D60
        jmp    loc_4_0AEA
loc_4_0A8C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21], eax
        jmp    loc_4_0AEA
loc_4_0A94:
        mov    ebx, 0xc0000001
        jmp    loc_4_0AEA
loc_4_0A9B:
        cmp    edi, 4
        je     loc_4_0AEA
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0AEA
loc_4_0AAF:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_4_0ACD
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_4_0AEA
loc_4_0ACD:
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
        call   sub_4_0B70
loc_4_0AE8:
        mov    ebx, eax
loc_4_0AEA:
        test   ebx, ebx
        jne    loc_4_0AF8
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_4_0AF8:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B10:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0B52
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0B48
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
loc_4_0B41:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0B48:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0B52:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B70:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 0x47]
        test   eax, eax
        jne    loc_4_0BB6
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 4], 0
        jne    loc_4_0BAC
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
loc_4_0BA5:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0BAC:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_4_0BB6:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1000c
        sub    eax, 0x3fffffff
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_0BD0:
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
        call   sub_4_0580
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_4_0CA5
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_4_0C35
        je     loc_4_0C74
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_4_0C43
        jmp    dword ptr [eax*4]             ; -> obj4:0x0CC0
loc_4_0C35:
        cmp    eax, 0x1020102
        je     loc_4_0C7C
        cmp    eax, 0x1020103
        je     loc_4_0C84
loc_4_0C43:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_4_0C8D
loc_4_0C4C:
        mov    eax, dword ptr [ebx + 0x83]
        jmp    loc_4_0C8A
loc_4_0C54:
        mov    eax, dword ptr [ebx + 0x87]
        jmp    loc_4_0C8A
loc_4_0C5C:
        mov    eax, dword ptr [ebx + 0x8b]
        jmp    loc_4_0C8A
loc_4_0C64:
        mov    eax, dword ptr [ebx + 0x9b]
        jmp    loc_4_0C8A
loc_4_0C6C:
        mov    eax, dword ptr [ebx + 0x9f]
        jmp    loc_4_0C8A
loc_4_0C74:
        mov    eax, dword ptr [ebx + 0x97]
        jmp    loc_4_0C8A
loc_4_0C7C:
        mov    eax, dword ptr [ebx + 0x8f]
        jmp    loc_4_0C8A
loc_4_0C84:
        mov    eax, dword ptr [ebx + 0x93]
loc_4_0C8A:
        mov    dword ptr [ebp - 4], eax
loc_4_0C8D:
        cmp    edi, 4
        jb     loc_4_0CA5
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_05BC
loc_4_0CA1:
        add    dword ptr [ebp - 0xc], 4
loc_4_0CA5:
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
        dd  loc_4_0C4C
        dd  loc_4_0C54
        dd  loc_4_0C5C
        dd  loc_4_0C64
        dd  loc_4_0C6C
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0CE0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [ecx + 0x14]
        cmp    edx, 0x10113
        ja     loc_4_0D08
        je     loc_4_0D3F
        cmp    edx, 0x10101
        jb     loc_4_0D3A
        cmp    edx, 0x10111
        jbe    loc_4_0D3F
        jmp    loc_4_0D3A
loc_4_0D08:
        cmp    edx, 0x1010104
        ja     loc_4_0D2A
        cmp    edx, 0x1010101
        jae    loc_4_0D3F
        cmp    edx, 0x20101
        jb     loc_4_0D3A
        cmp    edx, 0x20105
        jbe    loc_4_0D3F
        jmp    loc_4_0D3A
loc_4_0D2A:
        cmp    edx, 0x1020101
        jb     loc_4_0D3A
        cmp    edx, 0x1020103
        jbe    loc_4_0D3F
loc_4_0D3A:
        mov    eax, 0xc0010017
loc_4_0D3F:
        test   eax, eax
        jne    loc_4_0D4D
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0BD0
loc_4_0D4D:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D60:
        push   ebp
        mov    ecx, 0xee
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x1c5]
        test   edx, edx
        je     loc_4_0D86
loc_4_0D76:
        mov    esi, dword ptr [edx + 0x21]
        cmp    ecx, esi
        jae    loc_4_0D7F
        mov    ecx, esi
loc_4_0D7F:
        mov    edx, dword ptr [edx + 0x11]
        test   edx, edx
        jne    loc_4_0D76
loc_4_0D86:
        pop    esi
        mov    dword ptr [eax + 0x7f], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0D90:
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
loc_4_0DB7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     loc_4_0DCE
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DCE:
        mov    eax, dword ptr [esi + 0x47]
        test   eax, eax
        je     loc_4_0DF9
        test   al, 0xa
        je     loc_4_0DE7
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DE7:
        test   al, 1
        je     loc_4_0DF9
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0DF9:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_0E20
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E20:
        push   esi
        call   sub_4_1960
        test   al, al
        jne    loc_4_0E4A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x46], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E4A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_0524
loc_4_0E5B:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2C80
        test   eax, eax
        je     loc_4_0EA9
        jle    loc_4_0E97
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FB0
        push   esi
        call   sub_4_1990
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0E97:
        test   eax, eax
        jge    loc_4_0EA9
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0EA9:
        xor    edi, edi
        xor    eax, eax
loc_4_0EAD:
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_4_0EC7
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], cl
        jb     loc_4_0EAD
loc_4_0EC7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        je     loc_4_0F2B
loc_4_0ECF:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_0EDC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_0EED:
        xor    ecx, ecx
        cmp    edi, 6
        jae    loc_4_0F0E
loc_4_0EF4:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_0F0E
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [edi + esi + 0xb6], al
        jb     loc_4_0EF4
loc_4_0F0E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2D40
        test   eax, eax
        jl     loc_4_0F45
        inc    ebx
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, ebx
        ja     loc_4_0ECF
loc_4_0F2B:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        test   eax, eax
        jge    loc_4_0F53
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F45:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F53:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0330
loc_4_0F63:
        test   al, al
        je     loc_4_0F89
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_0FF0
        push   esi
        call   sub_4_1990
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0F89:
        push   esi
        inc    dword ptr [esi + 0x83]
        call   sub_4_1990
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_16E0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0FB0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_0FDC
        pop    ebp
        mov    dword ptr [eax + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
loc_4_0FDC:
        mov    ecx, dword ptr [eax + 0x28]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x28], edx
        inc    dword ptr [eax + 0x2c]
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_0FF0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x38], 0
        jne    loc_4_1019
        pop    ebp
        mov    dword ptr [eax + 0x38], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
loc_4_1019:
        mov    ecx, dword ptr [eax + 0x34]
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [eax + 0x34], edx
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1030:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x24], 0
        mov    eax, dword ptr [ecx + 0x24]
        test   eax, eax
        jne    loc_4_1055
        pop    ebp
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [ecx + 0x28], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
loc_4_1055:
        pop    ebp
        mov    dword ptr [edx + 0x24], eax
        mov    dword ptr [ecx + 0x24], edx
        inc    dword ptr [ecx + 0x2c]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1070:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    al, byte ptr [ebp + 0xc]
        push   esi
        test   al, al
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xb3], 0
        je     loc_4_10C8
        test   al, 1
        je     loc_4_1094
        inc    dword ptr [esi + 0x30]
loc_4_1094:
        test   al, 2
        je     loc_4_10A1
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x93]
loc_4_10A1:
        test   al, 4
        je     loc_4_10AE
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_10AE:
        test   al, 8
        je     loc_4_10BB
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_10BB:
        test   al, 0x10
        je     loc_4_10C8
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [esi + 0x8b]
loc_4_10C8:
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_4_10DC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_10DC:
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
loc_4_10FC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_110D:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2C80
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_1157
        push   edi
        push   esi
        call   sub_4_1030
        test   ebx, ebx
        jge    loc_4_1149
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
loc_4_1149:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1157:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_115B:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_1175
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_115B
loc_4_1175:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 0x14]
        dec    eax
        je     loc_4_11E2
loc_4_1182:
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_118F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_0524
loc_4_11A0:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    loc_4_11C1
loc_4_11A7:
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_4_11C1
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [ebx + esi + 0xb6], al
        jb     loc_4_11A7
loc_4_11C1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2D40
        cmp    eax, -2
        je     loc_4_1204
        mov    eax, dword ptr [ebp - 0x14]
        inc    dword ptr [ebp - 0x10]
        dec    eax
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_4_1182
loc_4_11E2:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2D70
        cmp    eax, -2
        jne    loc_4_1219
        push   edi
        push   esi
        call   sub_4_1030
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1204:
        push   edi
        push   esi
        call   sub_4_1030
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1219:
        lea    eax, [esi + 0xb7]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_1_0330
loc_4_1229:
        test   al, al
        je     loc_4_1249
        push   edi
        mov    eax, dword ptr [edi + 0x28]
        push   eax
        push   esi
        call   sub_4_0FF0
        push   esi
        call   sub_4_3A70
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1249:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_044A
loc_4_125E:
        push   esi
        call   sub_4_16E0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_1270:
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
loc_4_1295:
        test   edx, edx
        je     loc_4_12A4
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_4_1295
        mov    dword ptr [ecx], edx
loc_4_12A4:
        cmp    dword ptr [ecx], -1
        jne    loc_4_12B7
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_12B7:
        mov    eax, dword ptr [0]            ; -> obj1:0x07B8
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x25
        push   ecx
        call   sub_1_04D6
loc_4_12CA:
        test   eax, eax
        je     loc_4_12DC
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_12DC:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x1d
        mov    esi, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x7b]
        push   eax
        push   ecx
        call   sub_2_0170
loc_4_12F7:
        test   al, al
        jne    loc_4_1316
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x25
        push   eax
        call   sub_1_04DC
loc_4_1308:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_1316:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_1338
        mov    eax, dword ptr [esi + 0x1cd]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1cd], ecx
        jmp    loc_4_134F
loc_4_1338:
        mov    eax, dword ptr [esi + 0x1c5]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0x11], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x1c5], ecx
loc_4_134F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xd], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x1c1]
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
        mov    edx, dword ptr [esi + 0x7f]
        mov    dword ptr [eax + 0x21], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [esi + 0x1c9], 0
        jne    loc_4_13A2
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
loc_4_13A2:
        mov    eax, ebx
        pop    edi
        inc    dword ptr [esi + 0x1c9]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_13C0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        xor    eax, eax
loc_4_13CF:
        mov    dl, byte ptr [esi + eax + 0x67]
        mov    ecx, dword ptr [esi + 0x73]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     loc_4_13CF
        mov    edi, dword ptr [esi + 0x73]
        add    edi, 6
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_4_1407
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x10]
loc_4_13F2:
        xor    ecx, ecx
loc_4_13F4:
        mov    dl, byte ptr [eax + ecx]
        inc    edi
        inc    ecx
        mov    byte ptr [edi - 1], dl
        cmp    ecx, 6
        jb     loc_4_13F4
        add    eax, 6
        dec    ebx
        jne    loc_4_13F2
loc_4_1407:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x77], eax
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_1420
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x44], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_1420:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1AE0
        push   eax
        call   sub_1_04CA
loc_4_142F:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_146E
        mov    eax, 0xc0000001
        pop    edi
        or     dword ptr [esi + 0x47], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_146E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  090h
loc_4_1480:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [eax + 0xd]
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0xffffffd0
        je     loc_4_14A0
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_14A0:
        test   al, 1
        je     loc_4_14A9
        mov    ecx, 1
loc_4_14A9:
        test   al, 2
        je     loc_4_14B0
        or     ecx, 4
loc_4_14B0:
        test   al, 4
        je     loc_4_14B7
        or     ecx, 8
loc_4_14B7:
        test   al, 8
        je     loc_4_14BE
        or     ecx, 2
loc_4_14BE:
        test   al, 0x20
        je     loc_4_14C5
        or     ecx, 0x10
loc_4_14C5:
        test   dword ptr [esi + 0x1bd], ecx
        je     loc_4_14D7
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_4_14D7:
        mov    dword ptr [esi + 0x1b9], ecx
        cmp    byte ptr [esi + 0x40], 0
        je     loc_4_14EE
        mov    byte ptr [esi + 0x43], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_4_14EE:
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1AE0
        push   eax
        call   sub_1_04CA
loc_4_14FD:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_1526
        or     dword ptr [esi + 0x47], 2
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_4_1526:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1540:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xd]
        push   1
        add    ecx, 0x1d1
        push   ecx
        mov    edx, dword ptr [ecx + 0x1d]
        mov    dword ptr [eax + 0x15], edx
        mov    dword ptr [ecx + 0x1d], eax
        call   sub_1_04B2
loc_4_1561:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1570:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        je     loc_4_15A0
loc_4_1581:
        mov    eax, dword ptr [esi + 0x1ee]
        push   ecx
        mov    edx, dword ptr [eax + 0x15]
        mov    dword ptr [esi + 0x1ee], edx
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 0x1ee]
        test   ecx, ecx
        jne    loc_4_1581
loc_4_15A0:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x47], 8
        jne    loc_4_15E6
        push   esi
        call   sub_4_1960
        test   al, al
        je     loc_4_15D5
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3420
        push   esi
        call   sub_4_1990
loc_4_15D5:
        push   0x3e8
        add    esi, 0x1dd
        push   esi
        call   sub_1_04B2
loc_4_15E6:
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_4_15F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 4], 0
        mov    esi, dword ptr [edi + 0xd]
        je     loc_4_160E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_160E:
        inc    dword ptr [edi]
        mov    eax, dword ptr [esi + 0x1c5]
        cmp    edi, eax
        jne    loc_4_1625
        mov    eax, dword ptr [edi + 0x11]
        mov    dword ptr [esi + 0x1c5], eax
        jmp    loc_4_1638
loc_4_1625:
        cmp    dword ptr [eax + 0x11], edi
        je     loc_4_1632
loc_4_162A:
        mov    eax, dword ptr [eax + 0x11]
        cmp    dword ptr [eax + 0x11], edi
        jne    loc_4_162A
loc_4_1632:
        mov    ecx, dword ptr [edi + 0x11]
        mov    dword ptr [eax + 0x11], ecx
loc_4_1638:
        push   0
        mov    eax, dword ptr [edi + 0x1d]
        push   eax
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_2_0240
loc_4_1647:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_165F
        cmp    ebx, 0x103
        je     loc_4_1673
        cmp    ebx, 0xc001000e
        je     loc_4_167C
        jmp    loc_4_1685
loc_4_165F:
        mov    eax, dword ptr [edi]
        sub    eax, 2
        mov    dword ptr [edi], eax
        je     loc_4_1687
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
        jmp    loc_4_1687
loc_4_1673:
        mov    byte ptr [edi + 4], 1
        sub    dword ptr [edi], 2
        jmp    loc_4_1687
loc_4_167C:
        mov    byte ptr [edi + 4], 1
        mov    ebx, 0x103
loc_4_1685:
        dec    dword ptr [edi]
loc_4_1687:
        cmp    dword ptr [edi], 0
        jne    loc_4_16CB
        mov    eax, dword ptr [edi + 0x21]
        cmp    dword ptr [esi + 0x7f], eax
        jne    loc_4_169A
        push   esi
        call   sub_4_0D60
loc_4_169A:
        cmp    dword ptr [esi + 0x1c5], 0
        jne    loc_4_16B2
        push   esi
        lea    eax, [esi + 8]
        push   0                             ; -> loc_4_1AE0
        push   eax
        call   sub_1_04CA
loc_4_16B2:
        push   0
        dec    dword ptr [esi + 0x1c9]
        push   0x25
        push   edi
        call   sub_1_04DC
loc_4_16C2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_16CB:
        mov    ebx, 0x103
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_16E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xd]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    loc_4_1733
        mov    eax, dword ptr [esi + 0x21]
        cmp    dword ptr [edi + 0x7f], eax
        jne    loc_4_1700
        push   edi
        call   sub_4_0D60
loc_4_1700:
        cmp    dword ptr [edi + 0x1c5], 0
        jne    loc_4_1718
        push   edi
        lea    eax, [edi + 8]
        push   0                             ; -> loc_4_1AE0
        push   eax
        call   sub_1_04CA
loc_4_1718:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_0444
loc_4_1723:
        push   0
        push   0x25
        push   esi
        call   sub_1_04DC
loc_4_172D:
        dec    dword ptr [edi + 0x1c9]
loc_4_1733:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1740:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xd]
        cmp    byte ptr [eax + 4], 0
        je     loc_4_175D
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_175D:
        test   byte ptr [esi + 0x47], 1
        je     loc_4_176E
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_176E:
        push   0x40010004
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 0x1ea], eax
        call   sub_4_1900
        or     dword ptr [esi + 0x47], 1
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        je     loc_4_17B7
loc_4_178C:
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
loc_4_17AA:
        push   edi
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        jne    loc_4_178C
loc_4_17B7:
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        je     loc_4_17E3
loc_4_17BE:
        mov    eax, dword ptr [ecx + 0x24]
        push   0xc001000c
        push   ecx
        mov    dword ptr [esi + 0x38], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_044A
loc_4_17D6:
        push   edi
        call   sub_4_16E0
        mov    ecx, dword ptr [esi + 0x38]
        test   ecx, ecx
        jne    loc_4_17BE
loc_4_17E3:
        mov    byte ptr [esi + 0x1ff], 1
        push   esi
        call   sub_4_1960
        test   al, al
        je     loc_4_180B
        push   esi
        call   sub_4_1820
        push   esi
        call   sub_4_1990
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_180B:
        mov    eax, 0x103
        pop    edi
        mov    byte ptr [esi + 0x1ff], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1820:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x1ea]
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 0x1ff], 0
        mov    byte ptr [esi + 0x42], 1
        and    dword ptr [esi + 0x47], 0xfffffffd
        push   eax
        call   sub_4_27D0
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_1870
        or     dword ptr [esi + 0x47], 2
loc_4_1870:
        push   0x40010005
        push   esi
        call   sub_4_1900
        mov    eax, dword ptr [esi + 0x47]
        and    eax, 0xfffffffe
        mov    dword ptr [esi + 0x47], eax
        test   al, 2
        je     loc_4_188F
        push   0x80010004
        jmp    loc_4_1891
loc_4_188F:
        push   0
loc_4_1891:
        mov    eax, dword ptr [edi + 5]
        push   eax
        call   sub_1_0456
loc_4_189A:
        push   edi
        call   sub_4_16E0
        push   esi
        call   sub_4_18B0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_18B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        je     loc_4_18F6
loc_4_18C1:
        mov    eax, dword ptr [esi + 0x1cd]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0x11]
        mov    eax, dword ptr [esi + 0x1c5]
        mov    dword ptr [esi + 0x1cd], edx
        mov    dword ptr [ecx + 0x11], eax
        mov    dword ptr [esi + 0x1c5], ecx
        mov    ecx, dword ptr [ecx + 5]
        push   ecx
        call   sub_1_043E
loc_4_18EC:
        mov    ecx, dword ptr [esi + 0x1cd]
        test   ecx, ecx
        jne    loc_4_18C1
loc_4_18F6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1900:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x1c5]
        test   edi, edi
        je     loc_4_194B
        mov    esi, dword ptr [ebp + 0xc]
loc_4_191A:
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
loc_4_1936:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        call   sub_1_0474
loc_4_193F:
        push   edi
        call   sub_4_16E0
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_4_191A
loc_4_194B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1960:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0x40], 0
        je     loc_4_1973
        xor    al, al
        pop    ebp
        ret    4
loc_4_1973:
        mov    byte ptr [eax + 0x40], 1
        push   eax
        push   0                             ; -> loc_4_1AE0
        add    eax, 8
        push   eax
        call   sub_1_04CA
loc_4_1986:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1990:
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
        mov    al, byte ptr [esi + 0x1ff]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x43]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x44]
        or     ecx, eax
        je     loc_4_1ABE
loc_4_19CD:
        cmp    byte ptr [esi + 0x1ff], 0
        je     loc_4_19DE
        mov    bl, 1
        push   esi
        call   sub_4_1820
loc_4_19DE:
        cmp    byte ptr [esi + 0x44], 0
        je     loc_4_1A21
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 0x77]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_29A0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 0x1b9]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_1A1F
        or     dword ptr [esi + 0x47], 2
loc_4_1A1F:
        mov    bl, 1
loc_4_1A21:
        cmp    byte ptr [esi + 0x43], 0
        je     loc_4_1A4C
        mov    byte ptr [esi + 0x43], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi + 0x1b9]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_1A4A
        or     dword ptr [esi + 0x47], 2
loc_4_1A4A:
        mov    bl, 1
loc_4_1A4C:
        cmp    byte ptr [esi + 0x46], 0
        je     loc_4_1A6A
        mov    byte ptr [esi + 0x46], 0
loc_4_1A56:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_1A68
        push   0
        push   esi
        call   sub_4_1070
        test   eax, eax
        je     loc_4_1A56
loc_4_1A68:
        mov    bl, 1
loc_4_1A6A:
        cmp    dword ptr [esi + 0x38], 0
        je     loc_4_1A78
        push   esi
        mov    bl, 1
        call   sub_4_3A70
loc_4_1A78:
        cmp    byte ptr [esi + 0x41], 0
        je     loc_4_1A8B
        mov    byte ptr [esi + 0x41], 0
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_1_0290
loc_4_1A8B:
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
        mov    al, byte ptr [esi + 0x1ff]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 0x43]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 0x44]
        or     ecx, eax
        jne    loc_4_19CD
loc_4_1ABE:
        mov    byte ptr [esi + 0x40], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3840
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1AE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3810
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 031h, 01Dh, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1B20:
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
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_07C4
        mov    dword ptr [ebp - 0x14], 0x4018f022
        mov    word ptr [ebp - 0x18], 1
        push   ecx
        push   eax
        push   0xc                           ; -> obj1:0x082C
        call   sub_1_041A
loc_4_1B61:
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [0x46], edx         ; -> obj1:0x0866
        mov    dword ptr [0x4a], ecx         ; -> obj1:0x086A
        mov    dword ptr [4], ebx            ; -> obj1:0x0824
        mov    dword ptr [0x52], ebx         ; -> obj1:0x0872
        call   sub_4_2330
        mov    dword ptr [0x4e], eax         ; -> obj1:0x086E
        mov    byte ptr [0x10], 3            ; -> obj1:0x0830
        mov    byte ptr [0x11], bl           ; -> loc_1_0831
        mov    dword ptr [0x12], ebx         ; -> obj1:0x0832
        mov    dword ptr [0x16], 0           ; -> obj1:0x0836
        mov    dword ptr [0x1a], 0           ; -> obj1:0x083A
        mov    dword ptr [0x1e], 0           ; -> obj1:0x083E
        mov    dword ptr [0x22], 0           ; -> obj1:0x0842
        mov    dword ptr [0x26], 0           ; -> obj1:0x0846
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x2a], 0           ; -> obj1:0x084A
        mov    dword ptr [0x2e], 0           ; -> loc_1_084E
        mov    dword ptr [0x32], 0           ; -> obj1:0x0852
        mov    dword ptr [0x36], 0           ; -> obj1:0x0856
        mov    dword ptr [0x3a], 0           ; -> obj1:0x085A
        push   0x36
        mov    dword ptr [0x3e], edx         ; -> obj1:0x085E
        push   0x10                          ; -> obj1:0x0830
        lea    edx, [ebp - 4]
        push   0                             ; -> obj1:0x0820
        mov    dword ptr [0x42], ecx         ; -> obj1:0x0862
        mov    eax, dword ptr [0xc]          ; -> obj1:0x082C
        push   eax
        push   8                             ; -> obj1:0x0828
        push   edx
        call   sub_1_0426
loc_4_1C34:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1C61
        cmp    dword ptr [4], ebx            ; -> obj1:0x0824
        jbe    loc_4_1C4A
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1C4A:
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj1:0x082C
        push   eax
        call   sub_1_0420
loc_4_1C57:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1C61:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_1C70:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x07B8
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   0x225
        push   ecx
        call   sub_1_04D6
loc_4_1C8E:
        test   eax, eax
        je     loc_4_1C9F
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1C9F:
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_0450
        test   eax, eax
        je     loc_4_1CD1
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x225
        push   eax
        call   sub_1_04DC
loc_4_1CC4:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1CD1:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1D10
        test   eax, eax
        jne    loc_4_1CEA
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1CEA:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x225
        push   eax
        call   sub_1_04DC
loc_4_1CFA:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_21C0
        push   esi
        call   sub_4_20B0
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_2018
        lea    edi, [ebp - 0x48]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x48], 0
        mov    eax, dword ptr [esi + 0x209]
        cmp    eax, -1
        je     loc_4_1D56
        mov    dword ptr [ebp - 0x48], eax
loc_4_1D56:
        mov    edi, 2
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 0x205]
        mov    dword ptr [ebp - 0x3e], edi
        mov    dword ptr [ebp - 0x3a], ebx
        mov    dword ptr [ebp - 0x2e], eax
        lea    ecx, [ebp - 0x48]
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [8]            ; -> obj1:0x0828
        push   eax
        lea    edx, [esi + 0x201]
        mov    dword ptr [ebp - 0x36], 0x600
        mov    dword ptr [ebp - 0x32], 1
        mov    dword ptr [ebp - 0x2a], 0xff
        push   esi
        push   ecx
        push   edx
        call   sub_1_0432
loc_4_1DA1:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_4_2012
        mov    eax, dword ptr [esi + 0x209]
        cmp    eax, -1
        je     loc_4_1E1E
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
        mov    dword ptr [ebp - 0x1d], edi
        mov    dword ptr [ebp - 0x15], eax
        mov    eax, dword ptr [esi + 0x201]
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 0x20], 1
        mov    dword ptr [ebp - 0x19], 3
        push   eax
        push   edx
        push   ecx
        call   sub_1_0584
loc_4_1DF7:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_1E1E
        mov    eax, dword ptr [esi + 0x209]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_05A8
loc_4_1E16:
        add    esp, 0x10
        jmp    loc_4_2006
loc_4_1E1E:
        mov    ecx, dword ptr [esi + 0x205]
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [esi + 0x209]
        push   edx
        mov    ebx, dword ptr [esi + 0x21d]
        push   ebx
        lea    eax, [ecx + 0xff]
        push   ebx
        mov    edx, dword ptr [esi + 0x73]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esi + 0x211]
        mov    eax, dword ptr [esi]
        push   ecx
        push   edx
        push   esi
        push   eax
        call   sub_4_2490
        test   eax, eax
        jge    loc_4_1E87
        mov    eax, dword ptr [esi + 0x205]
        mov    ecx, dword ptr [esi + 0x211]
        push   eax
        mov    edx, dword ptr [esi + 0x201]
        push   ecx
        push   2
        push   0xaa000004
        push   edx
        call   sub_1_05A8
loc_4_1E78:
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0x14
        jmp    loc_4_1FF4
loc_4_1E87:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x20], eax
loc_4_1E98:
        lea    eax, [esi + ecx]
        mov    edx, dword ptr [esi + 0x73]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 0x6d], dl
        mov    byte ptr [eax + 0x67], dl
        jb     loc_4_1E98
        lea    ebx, [esi + 0x7b]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 0x67]
        push   eax
        mov    edx, dword ptr [esi + 0x20d]
        push   ecx
        push   0                             ; -> loc_4_1540
        push   0                             ; -> loc_4_1480
        push   0                             ; -> loc_4_13C0
        push   edx
        call   sub_2_0000
loc_4_1ED5:
        test   al, al
        jne    loc_4_1EE5
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_4_1FF4
loc_4_1EE5:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_29A0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2A50
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_33E0
        mov    edi, eax
        mov    ecx, dword ptr [0]            ; -> obj1:0x07B8
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        call   sub_1_04D6
loc_4_1F14:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_1F27
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_4_1FEC
loc_4_1F27:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3400
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   edi
        push   ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0xa3], edx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 0xa7], eax
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0xab], edx
        call   sub_1_04DC
loc_4_1F59:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_37F0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 0x1bd], eax
        mov    dword ptr [esi + 0x1b9], 0
        call   sub_4_2A80
        test   eax, eax
        jge    loc_4_1F89
        or     dword ptr [esi + 0x47], 2
        jmp    loc_4_1FEC
loc_4_1F89:
        lea    edx, [esi + 8]
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [esi + 0x211]
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [esi + 0x201]
        push   ecx
        push   0                             ; -> loc_4_02D0
        push   esi
        push   0                             ; -> loc_4_02B0
        push   eax
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        call   sub_1_04BE
loc_4_1FC1:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_2024
        mov    eax, dword ptr [esi + 0x211]
        mov    ecx, dword ptr [esi + 0x201]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_05A8
loc_4_1FE1:
        add    esp, 0x10
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_28B0
loc_4_1FEC:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_4_1FF4:
        cmp    dword ptr [esi + 0x209], -1
        je     loc_4_2006
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_059C
loc_4_2006:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0438
loc_4_2012:
        push   esi
        call   sub_4_2180
loc_4_2018:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_2024:
        mov    dword ptr [esi + 0x1c1], 0    ; -> obj1:0x0820
        mov    byte ptr [esi + 0x45], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x1c9], ecx
        mov    dword ptr [esi + 0x1c5], ecx
        mov    dword ptr [esi + 0x1cd], ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x0820
        mov    dword ptr [esi + 0x221], eax
        lea    eax, [esi + 0x1dd]
        mov    dword ptr [0], esi            ; -> obj1:0x0820
        mov    dword ptr [esi + 0x1d1], ecx
        mov    dword ptr [esi + 0x1d9], esi
        push   0x3e8
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 0x1e5], esi
        mov    dword ptr [esi + 0x1d5], 0    ; -> loc_4_1570
        mov    dword ptr [esi + 0x1e1], 0    ; -> loc_4_15B0
        call   sub_1_04B2
loc_4_2090:
        mov    ecx, dword ptr [esi + 0x1c1]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_20B0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x07B8
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [0x4e]         ; -> obj1:0x086E
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        call   sub_1_04D6
loc_4_20D1:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_20E6
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_20E6:
        lea    ebx, [esi + 0x73]
        mov    eax, dword ptr [0]            ; -> obj1:0x07B8
        push   eax
        mov    ecx, dword ptr [esi + 0x20d]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        call   sub_1_04D6
loc_4_2104:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_2155
        mov    edx, dword ptr [esi + 0x20d]
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
loc_4_2138:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_2173
        push   0
        mov    eax, dword ptr [esi + 0x20d]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_04DC
loc_4_2155:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x086E
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_04DC
loc_4_2165:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2173:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_2180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x4f]
        push   eax
        call   sub_1_050C
loc_4_2190:
        push   3
        mov    eax, dword ptr [0x4e]         ; -> obj1:0x086E
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_04DC
loc_4_21A0:
        push   0
        mov    ecx, dword ptr [esi + 0x20d]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 0x73]
        add    eax, eax
        push   eax
        push   ecx
        call   sub_1_04DC
loc_4_21B8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_21C0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1c9], ecx
        mov    byte ptr [eax + 0x40], cl
        mov    byte ptr [eax + 0x44], cl
        mov    byte ptr [eax + 0x1fe], cl
        mov    byte ptr [eax + 0x45], cl
        mov    byte ptr [eax + 0x1e9], cl
        mov    dword ptr [eax + 0x47], ecx
        mov    dword ptr [eax + 0x1b9], ecx
        mov    dword ptr [eax + 0x77], ecx
        mov    byte ptr [eax + 0x43], cl
        mov    byte ptr [eax + 0x1ff], cl
        mov    byte ptr [eax + 0x200], cl
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    byte ptr [eax + 0x46], cl
        mov    dword ptr [eax + 0x34], ecx
        pop    ebp
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x7f], 0xee
        mov    dword ptr [eax + 0x3c], ecx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2230:
        push   ebp
        mov    eax, dword ptr [8]            ; -> obj1:0x0828
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_042C
loc_4_2245:
        push   0
        mov    ecx, dword ptr [0xc]          ; -> obj1:0x082C
        push   ecx
        call   sub_1_0420
loc_4_2253:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2260:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x4c4b40
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0x47], 8
        lea    edi, [esi + 8]
        call   sub_1_04D0
loc_4_2279:
        push   esi
        push   0                             ; -> loc_4_2310
        push   edi
        call   sub_1_04CA
loc_4_2285:
        push   edi
        call   sub_1_04C4
loc_4_228B:
        cmp    esi, dword ptr [0]            ; -> obj1:0x0820
        jne    loc_4_22A0
        mov    eax, dword ptr [esi + 0x221]
        mov    dword ptr [0], eax            ; -> obj1:0x0820
        jmp    loc_4_22CE
loc_4_22A0:
        mov    ecx, dword ptr [0]            ; -> obj1:0x0820
        cmp    dword ptr [ecx + 0x221], esi
        je     loc_4_22BC
loc_4_22AE:
        mov    ecx, dword ptr [ecx + 0x221]
        cmp    dword ptr [ecx + 0x221], esi
        jne    loc_4_22AE
loc_4_22BC:
        mov    eax, dword ptr [ecx + 0x221]
        mov    edx, dword ptr [eax + 0x221]
        mov    dword ptr [ecx + 0x221], edx
loc_4_22CE:
        cmp    dword ptr [esi + 0x209], -1
        je     loc_4_22E0
        mov    eax, dword ptr [esi + 0x4b]
        push   eax
        call   sub_1_059C
loc_4_22E0:
        mov    eax, dword ptr [esi + 0x7b]
        push   eax
        call   sub_2_0120
loc_4_22E9:
        mov    eax, dword ptr [esi + 0x201]
        push   eax
        call   sub_1_0438
loc_4_22F5:
        push   esi
        call   sub_4_2180
        push   0
        push   0x225
        push   esi
        call   sub_1_04DC
loc_4_2308:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_28B0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_27D0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_2330:
        mov    eax, 0x110
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2340:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 4]
        add    eax, edi
        push   eax
        push   ecx
        call   sub_4_0290
        push   edi
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0270
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_4_2370:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bx, word ptr [esi + 2]
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_4_238F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_238F:
        mov    di, word ptr [ebp + 0x10]
        xor    ecx, ecx
        and    di, 1
        mov    cl, byte ptr [esi + 0xb]
        push   edi
        mov    edx, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0170
        test   eax, eax
        je     loc_4_23BC
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_23BC:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ebx
        push   ecx
        xor    ebx, ebx
        call   sub_4_0290
loc_4_23CC:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0xb]
        push   eax
        push   ecx
        call   sub_4_0200
        and    eax, 0xffff
        cmp    eax, 0xffff
        je     loc_4_23F0
        inc    ebx
        cmp    ebx, 0x61a8
        jb     loc_4_23CC
loc_4_23F0:
        cmp    ebx, 0x61a8
        jae    loc_4_2436
        test   di, di
        jne    loc_4_241E
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   0x61
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
loc_4_241E:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0xb]
        push   eax
        push   ecx
        call   sub_4_0220
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_2436:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    al, byte ptr [esi + 0xb]
        push   eax
        push   ecx
        call   sub_4_0220
        push   esi
        call   sub_4_27D0
        push   esi
        call   sub_4_2A50
        mov    ecx, dword ptr [esi + 0x11]
        push   ecx
        push   esi
        call   sub_4_2A80
        test   eax, eax
        je     loc_4_2475
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0
        push   0                             ; -> loc_1_07CC
        push   1
        push   eax
        call   sub_4_0080
loc_4_2475:
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2490:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    word ptr [ebx], 0
        mov    edi, dword ptr [ebp + 0x18]
        mov    word ptr [ebx + 2], di
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 4], eax
        mov    eax, edi
        mov    dword ptr [ebx + 0x17], esi
        and    eax, 0xffff0fff
        cmp    eax, 0xc00
        jne    loc_4_27B3
        mov    eax, dword ptr [ebp + 0x1c]
        sub    eax, edi
        cmp    eax, 0xff
        jb     loc_4_27B3
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    dword ptr [ebp + 0x14], 0xd
        mov    dword ptr [eax], 0            ; -> obj6:0x0054
        jb     loc_4_24F3
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_24F3:
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    byte ptr [ecx], 0             ; -> obj6:0x0000
        jne    loc_4_250D
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_250D:
        cmp    dword ptr [ebp + 0x28], 7
        mov    cl, byte ptr [ebp + 0x14]
        mov    byte ptr [ebx + 0xc], cl
        ja     loc_4_279C
        mov    eax, dword ptr [ebp + 0x28]
        mov    al, byte ptr [eax]            ; -> obj6:0x0010
        mov    byte ptr [ebp - 4], al
        test   al, al
        je     loc_4_279C
        push   edi
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0260
        and    al, 0x81
        mov    ecx, dword ptr [ebp + 0x14]
        or     al, byte ptr [ecx]            ; -> obj6:0x0000
        or     al, byte ptr [ebp - 4]
        or     al, 1
        mov    byte ptr [ebx + 9], al
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   0
        lea    ecx, [edi + 0x80]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        cmp    al, 0x22
        jne    loc_4_26B2
        lea    eax, [edi + 0x81]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 0xf0
        jne    loc_4_26B2
        lea    eax, [edi + 0x82]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        cmp    al, 0x18
        jne    loc_4_26B2
        lea    eax, [edi + 0x83]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        call   sub_4_0260
        mov    byte ptr [ebp - 4], al
        and    al, 0xf0
        cmp    al, 0x40
        jne    loc_4_26B2
        push   0
        lea    eax, [edi + 0x20]
        push   eax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0280
        lea    ecx, [edi + 0x80]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 0x80]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 0x80]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        lea    ecx, [edi + 0x80]
        mov    edx, dword ptr [ebx + 4]
        push   ecx
        push   edx
        call   sub_4_0260
        mov    cl, byte ptr [ebx + 9]
        lea    edx, [edi + 0x20]
        push   ecx
        mov    eax, dword ptr [ebx + 4]
        push   edx
        push   eax
        call   sub_4_0280
        mov    byte ptr [ebx + 0x10f], 0
        mov    al, byte ptr [ebx + 9]
        lea    ecx, [edi + 0x20]
        and    al, 0xf1
        mov    edx, dword ptr [ebx + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_4_0280
        push   0x1f
        push   0xbe
        push   edi
        push   ebx
        call   sub_4_2340
        mov    dword ptr [ebp - 0xc], 0
        mov    word ptr [ebp - 6], 0
        mov    dword ptr [ebp - 0x10], esi
loc_4_2654:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebx + 4]
        add    eax, edi
        add    eax, 0x85
        push   eax
        push   ecx
        call   sub_4_0260
        mov    ecx, dword ptr [ebp - 0x10]
        inc    dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 0x10]
        mov    byte ptr [ecx], al
        and    ax, 0xff
        add    word ptr [ebp - 6], ax
        cmp    dword ptr [ebp - 0xc], 6
        jb     loc_4_2654
        lea    eax, [edi + 0x8b]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        add    edi, 0x8c
        push   ecx
        call   sub_4_0260
        and    ax, 0xff
        push   edi
        add    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        call   sub_4_0260
        and    byte ptr [ebp - 4], 0xf
        mov    byte ptr [ebx + 8], al
        jmp    loc_4_26B6
loc_4_26B2:
        mov    byte ptr [ebp - 4], 0xff
loc_4_26B6:
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0010
        cmp    byte ptr [ebp - 4], 0xff
        jne    loc_4_26DC
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj6:0x0018
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_26DC:
        cmp    byte ptr [ebp - 6], 0xff
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx + 0xa], al
        je     loc_4_270D
        mov    edi, 0                        ; -> loc_1_07E8
        mov    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_4_270D
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj6:0x0018
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_270D:
        mov    al, byte ptr [ebp + 0x28]
        push   0xc1c
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    byte ptr [ebx + 0xb], al
        call   sub_4_0020
        test   eax, eax
        jne    loc_4_273E
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj6:0x00B4
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_273E:
        add    eax, 3
        and    eax, 0xfffffffc
        mov    edx, eax
        mov    dword ptr [ebx + 0xd], eax
        lea    ecx, [eax + 0x5f4]
        and    edx, 0xfffff000
        and    ecx, 0xfffff000
        cmp    edx, ecx
        je     loc_4_2762
        mov    dword ptr [ebx + 0xd], ecx
loc_4_2762:
        mov    dword ptr [ebx + 0xeb], 0x100
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        mov    dword ptr [ebx + 0xe3], 0x2800
        mov    dword ptr [ebx + 0xe7], 0xd800
        pop    esi
        mov    dword ptr [eax], 0            ; -> obj6:0x00E0
        mov    word ptr [ebx], 2
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_279C:
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj6:0x0074
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
loc_4_27B3:
        mov    eax, dword ptr [ebp + 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0            ; -> obj6:0x0034
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        ret    0x28
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_27D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    dword ptr [esi + 0x37], 0
        mov    word ptr [esi], 2
        cmp    byte ptr [esi + 0x10f], 0
        je     loc_4_2801
        mov    al, byte ptr [esi + 9]
        and    eax, 0xfe
        jmp    loc_4_280C
loc_4_2801:
        lea    eax, [esi + 9]
        mov    al, byte ptr [esi + 9]
        and    eax, 0xf0
loc_4_280C:
        push   eax
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [edi + 0x20]
        lea    ebx, [edi + 0x80]
        push   eax
        push   ecx
        call   sub_4_0280
        push   ebx
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0260
        cmp    byte ptr [esi + 0x10f], 0
        mov    al, byte ptr [esi + 9]
        jne    loc_4_2856
        and    al, 0xf1
loc_4_2856:
        or     al, 1
        mov    ecx, dword ptr [esi + 4]
        and    eax, 0xff
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        push   ecx
        call   sub_4_0280
        push   edi
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_4_0270
        push   0x21
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        lea    ecx, [edi + 0xa0]
        mov    edx, dword ptr [esi + 4]
        push   ecx
        push   edx
        call   sub_4_0270
        push   esi
        call   sub_4_3810
        push   0x1f
        push   0xbe
        push   edi
        push   esi
        call   sub_4_2340
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_28B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    dword ptr [esi + 0x37], 0
        lea    eax, [edi + 0xa0]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 4
        push   0x21
        push   0xa0
        mov    byte ptr [ebp - 1], al
        push   edi
        mov    ebx, 0x63f
        push   esi
        call   sub_4_2340
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3810
loc_4_28FC:
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0270
        dec    ebx
        jne    loc_4_28FC
        xor    ebx, ebx
        push   ebx
        push   0xb4
        push   edi
        push   esi
        call   sub_4_2340
        push   ebx
        push   0xb6
        push   edi
        push   esi
        call   sub_4_2340
        push   2
        push   0xba
        push   edi
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0xae]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 0xa
        je     loc_4_2985
        push   0xa
        push   0xae
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        mov    dword ptr [esi + 0x33], ebx
        mov    al, byte ptr [esi + 0x40]
        mov    cl, byte ptr [esi + eax*4 + 0x44]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 0x40], cl
        mov    byte ptr [eax + 0x44], bl
        mov    word ptr [eax + 0x45], bx
        mov    al, byte ptr [esi + 0x3f]
        test   al, al
        je     loc_4_297A
        dec    al
        mov    byte ptr [esi + 0x3f], al
loc_4_297A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2985:
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_29A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    word ptr [esi + 0x15], di
        mov    ebx, dword ptr [ebp + 0xc]
        lea    eax, [esi + 0x1b]
        cmp    ebx, edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        je     loc_4_29D3
loc_4_29BE:
        mov    eax, dword ptr [esi + 0x17]
        add    eax, edi
        add    edi, 6
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_29E0
        dec    ebx
        jne    loc_4_29BE
loc_4_29D3:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_29E0:
        push   ebp
        mov    ecx, 0xffffffff
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
loc_4_29F8:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    eax, 8
loc_4_2A00:
        mov    edi, ecx
        mov    bl, dl
        shr    dl, 1
        and    edi, 0x80000000
        shr    edi, 0x1f
        and    ebx, 1
        xor    edi, ebx
        add    ecx, ecx
        test   edi, edi
        je     loc_4_2A23
        xor    ecx, 0x4c11db6
        or     ecx, edi
loc_4_2A23:
        dec    eax
        jne    loc_4_2A00
        dec    dword ptr [ebp - 4]
        jne    loc_4_29F8
        shr    ecx, 0x1a
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 0x1b], bl
        inc    word ptr [eax + 0x15]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_2A50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x33], eax
        mov    dword ptr [edi + 0x37], eax
        mov    byte ptr [edi + 0x3f], al
        mov    byte ptr [edi + 0x40], al
        mov    byte ptr [edi + 0x41], al
        mov    byte ptr [edi + 0x42], al
        add    edi, 0x43
        mov    ecx, 0x28
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   al, 2
        mov    dword ptr [esi + 0x37], ebx
        mov    dword ptr [esi + 0x11], eax
        je     loc_4_2A9D
        mov    bl, 4
loc_4_2A9D:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        jne    loc_4_2AAE
        test   byte ptr [ebp + 0xc], 8
        je     loc_4_2AB1
loc_4_2AAE:
        or     bl, 8
loc_4_2AB1:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x10
        mov    dword ptr [ebp - 0xc], eax
        je     loc_4_2ABF
        or     bl, 0x1c
loc_4_2ABF:
        test   byte ptr [ebp + 0xc], 0x20
        je     loc_4_2AC8
        or     bl, 3
loc_4_2AC8:
        xor    edi, edi
        push   0x21
        mov    di, word ptr [esi + 2]
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        push   esi
        call   sub_4_3810
        push   0x49
        push   0xbc
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0xb4
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0xb6
        push   edi
        push   esi
        call   sub_4_2340
        mov    al, bl
        and    eax, 0xffff00ff
        push   eax
        push   0xb8
        push   edi
        push   esi
        call   sub_4_2340
        push   2
        push   0xba
        push   edi
        push   esi
        call   sub_4_2340
        push   0x28
        push   0xa2
        push   edi
        push   esi
        call   sub_4_2340
        push   0x28
        push   0xa6
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0xa4
        push   edi
        push   esi
        call   sub_4_2340
        push   0xff
        push   0xae
        push   edi
        push   esi
        call   sub_4_2340
        push   0x1f
        push   0xbe
        push   edi
        push   esi
        call   sub_4_2340
        push   0x61
        push   0xa0
        push   edi
        xor    ebx, ebx
        push   esi
        call   sub_4_2340
        mov    eax, dword ptr [esi + 0x17]
        mov    dword ptr [ebp - 4], eax
loc_4_2B8D:
        lea    ecx, [ebx + 0xa2]
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 2
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [ebp - 4]
        cmp    ebx, 0xc
        jl     loc_4_2B8D
        test   byte ptr [ebp + 0xc], 8
        jne    loc_4_2BF1
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_2BF1
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_2C0E
        lea    eax, [esi + 0x1b]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], eax
loc_4_2BCB:
        lea    ecx, [ebx + 0xb0]
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 2
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [ebp - 4]
        cmp    ebx, 0x10
        jl     loc_4_2BCB
        jmp    loc_4_2C0E
loc_4_2BF1:
        xor    ebx, ebx
loc_4_2BF3:
        lea    eax, [ebx + 0xb0]
        push   0xffff
        push   eax
        add    ebx, 2
        push   edi
        push   esi
        call   sub_4_2340
        cmp    ebx, 0x10
        jl     loc_4_2BF3
loc_4_2C0E:
        push   0x29
        push   0xae
        push   edi
        push   esi
        call   sub_4_2340
        mov    dword ptr [esi + 0x23], 0x29
        push   2
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0xba
        push   edi
        push   esi
        add    edi, 0xa0
        call   sub_4_2340
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_0270
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_2C63
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2C63:
        mov    word ptr [esi], 8
        cmp    byte ptr [esi + 0x3f], 0
        je     loc_4_2C74
        push   esi
        call   sub_4_2E40
loc_4_2C74:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_4_2C80:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [edx], 8
        push   esi
        push   edi
        jne    loc_4_2C9B
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_2C9B:
        mov    esi, dword ptr [ebp + 0x14]
        xor    eax, eax
        add    esi, 0xff
        mov    al, byte ptr [edx + 0x41]
        shr    esi, 8
        xor    ebx, ebx
        cmp    byte ptr [edx + 0x3f], 0
        mov    bl, byte ptr [edx + eax*4 + 0x44]
        je     loc_4_2D04
        xor    eax, eax
        inc    dword ptr [edx + 0x107]
        mov    al, byte ptr [edx + 0x40]
        cmp    eax, ebx
        jae    loc_4_2CEC
        mov    ecx, 0x28
        sub    ecx, ebx
        cmp    ecx, esi
        jae    loc_4_2D0C
        cmp    esi, eax
        jbe    loc_4_2CE8
        mov    eax, 1
        pop    edi
        pop    esi
        inc    dword ptr [edx + 0x103]
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_2CE8:
        xor    ebx, ebx
        jmp    loc_4_2D0C
loc_4_2CEC:
        sub    eax, ebx
        cmp    eax, esi
        jae    loc_4_2D0C
        mov    eax, 1
        pop    edi
        pop    esi
        inc    dword ptr [edx + 0x103]
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_2D04:
        xor    ebx, ebx
        mov    byte ptr [edx + 0x41], bl
        mov    byte ptr [edx + 0x40], bl
loc_4_2D0C:
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, eax
        mov    byte ptr [edx + 0x42], bl
        shr    ecx, 2
        mov    edi, dword ptr [edx + 0xd]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        mov    dword ptr [edx + 0x3b], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2D40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [eax + 0xd]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, dword ptr [eax + 0x3b]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        add    dword ptr [eax + 0x3b], edx
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_4_2D70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x3b]
        cmp    ebx, 0x3c
        jae    loc_4_2DA2
        mov    edx, 0x3c
        mov    edi, dword ptr [esi + 0xd]
        sub    edx, ebx
        add    edi, ebx
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        mov    ebx, 0x3c
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_4_2DA2:
        lea    eax, [ebx + 3]
        mov    ecx, dword ptr [esi + 0xd]
        and    eax, 0xfffffffc
        push   eax
        mov    al, byte ptr [esi + 0x42]
        and    eax, 0xffff00ff
        shl    ax, 8
        push   eax
        push   ecx
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     loc_4_2DD0
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2DD0:
        mov    cl, byte ptr [esi + 0x42]
        lea    eax, [ebx + 0xff]
        shr    eax, 8
        add    al, cl
        cmp    al, 0x28
        jb     loc_4_2DE4
        xor    al, al
loc_4_2DE4:
        and    ecx, 0xff
        mov    byte ptr [esi + ecx*4 + 0x44], al
        xor    eax, eax
        mov    al, byte ptr [esi + 0x42]
        mov    word ptr [esi + eax*4 + 0x45], bx
        mov    dword ptr [esi + 0x3b], 0
        mov    al, byte ptr [esi + 0x3f]
        test   al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 0x3f], cl
        jne    loc_4_2E1A
        mov    al, byte ptr [esi + 0x42]
        push   esi
        mov    byte ptr [esi + 0x40], al
        call   sub_4_2E40
        jmp    loc_4_2E26
loc_4_2E1A:
        mov    cl, byte ptr [esi + 0x42]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x41]
        mov    byte ptr [esi + eax*4 + 0x44], cl
loc_4_2E26:
        mov    al, byte ptr [esi + 0x42]
        pop    edi
        mov    byte ptr [esi + 0x41], al
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 2]
        mov    al, byte ptr [esi + 0x40]
        and    eax, 0xffff00ff
        push   eax
        push   0xa8
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x40]
        mov    cx, word ptr [esi + eax*4 + 0x45]
        mov    dl, cl
        mov    dword ptr [ebp - 4], ecx
        and    edx, 0xffff00ff
        push   edx
        push   0xaa
        push   edi
        push   esi
        call   sub_4_2340
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0xffff00ff
        push   ecx
        push   0xac
        push   edi
        push   esi
        call   sub_4_2340
        push   0x26
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        xor    eax, eax
        pop    edi
        mov    dword ptr [esi + 0x37], 2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_2EC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        push   0x62
        and    edi, 0xffff
        push   0xa0
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0xae]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        mov    bl, al
        push   0x22
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        add    esi, 7
        and    esi, 0xfffffffc
        cmp    esi, 0xf8
        ja     loc_4_2F2F
        mov    esi, 0xf8
loc_4_2F2F:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, 0x10000
        mov    dword ptr [ebx + 0x2f], esi
        mov    eax, dword ptr [ebx + 0xd]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 0x23]
        shl    ax, 8
        mov    ecx, eax
        and    ecx, 0xffff
        sub    edi, ecx
        cmp    edi, esi
        jae    loc_4_2F7E
        push   edi
        or     al, 1
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   ebx
        call   sub_4_2370
        test   eax, eax
        je     loc_4_2F75
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_2F75:
        mov    ax, 0x2800
        sub    esi, edi
        add    dword ptr [ebp - 4], edi
loc_4_2F7E:
        push   esi
        or     al, 1
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   ebx
        call   sub_4_2370
        test   eax, eax
        je     loc_4_2F9E
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_2F9E:
        mov    eax, dword ptr [ebx + 0xd]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0x27], ecx
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebx + 0xd]
        add    esi, 4
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    cl, byte ptr [ebx + 0x27]
        test   cl, 0x48
        je     loc_4_306C
        xor    esi, esi
        push   0xff
        push   0xb4
        inc    dword ptr [ebx + 0xfb]
        mov    si, word ptr [ebx + 2]
        push   esi
        push   ebx
        call   sub_4_2340
        push   0xff
        push   0xb6
        push   esi
        push   ebx
        call   sub_4_2340
        push   0x28
        push   0xa6
        push   esi
        push   ebx
        call   sub_4_2340
        push   0x61
        push   0xa0
        push   esi
        push   ebx
        call   sub_4_2340
        push   0x29
        push   0xae
        push   esi
        push   ebx
        call   sub_4_2340
        mov    dword ptr [ebx + 0x23], 0x29
        push   0x22
        push   0xa0
        push   esi
        push   ebx
        call   sub_4_2340
        push   5
        push   0xae
        push   esi
        push   ebx
        add    esi, 0xa0
        call   sub_4_2340
        push   esi
        mov    eax, dword ptr [ebx + 4]
        push   eax
        call   sub_4_0270
        and    al, 0xc2
        pop    edi
        sub    al, 2
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    eax, eax
        mov    esp, ebp
        and    eax, 0xfffffffe
        pop    ebp
        dec    eax
        ret    0x10
loc_4_306C:
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x28]
        sub    eax, dword ptr [ebx + 0x23]
        lea    edx, [eax - 1]
        cmp    edx, 0xd8
        jbe    loc_4_3085
        lea    edx, [eax + 0xd7]
loc_4_3085:
        mov    al, byte ptr [ebx + 0x29]
        cmp    al, 0xfc
        jbe    loc_4_308D
        dec    edx
loc_4_308D:
        shl    edx, 8
        and    eax, 0xff
        add    eax, edx
        mov    dword ptr [ebx + 0x2b], eax
        sub    eax, 4
        test   cl, 1
        jne    loc_4_30F6
        mov    dl, cl
        and    dl, 4
        jne    loc_4_30B9
        test   cl, 2
        jne    loc_4_30B9
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x201
        jmp    loc_4_30E1
loc_4_30B9:
        and    cl, 6
        cmp    cl, 6
        jne    loc_4_30CC
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x206
        jmp    loc_4_30E1
loc_4_30CC:
        mov    ecx, dword ptr [ebp + 0x14]
        test   dl, dl
        je     loc_4_30DB
        mov    dword ptr [ecx], 0x204
        jmp    loc_4_30E1
loc_4_30DB:
        mov    dword ptr [ecx], 0x202
loc_4_30E1:
        test   byte ptr [ebx + 0x11], 0x20
        jne    loc_4_324A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_30F6:
        cmp    eax, 0x3c
        jb     loc_4_3239
        cmp    eax, 0x5ea
        ja     loc_4_3239
        mov    edx, dword ptr [ebx + 0x17]
        mov    ecx, dword ptr [ebp + 0xc]
        test   byte ptr [ecx], 1
        jne    loc_4_3182
        mov    ecx, 6
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_315F
        mov    ecx, dword ptr [ebx + 0x11]
        test   cl, 1
        je     loc_4_313D
        mov    ecx, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_313D:
        test   cl, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x100
        je     loc_4_3154
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3154:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_315F:
        mov    ecx, dword ptr [ebp + 0x14]
        test   byte ptr [ebx + 0x11], 0x10
        mov    dword ptr [ecx], 0x104
        je     loc_4_3177
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3177:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3182:
        mov    esi, 0                        ; -> obj1:0x07EC
        mov    ecx, 6
        mov    edi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_31CF
        mov    ecx, dword ptr [ebx + 0x11]
        test   cl, 2
        je     loc_4_31AD
        mov    ecx, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_31AD:
        test   cl, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x101
        je     loc_4_31C4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_31C4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_31CF:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [ebx + 0x15], 0
        jbe    loc_4_3202
loc_4_31DD:
        add    edx, 6
        mov    ecx, 6
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_31F4
        test   byte ptr [ebx + 0x11], 4
        jne    loc_4_321E
loc_4_31F4:
        xor    ecx, ecx
        inc    dword ptr [ebp - 4]
        mov    cx, word ptr [ebx + 0x15]
        cmp    ecx, dword ptr [ebp - 4]
        ja     loc_4_31DD
loc_4_3202:
        mov    ecx, dword ptr [ebx + 0x11]
        test   cl, 0x18
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x102
        jne    loc_4_3230
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_321E:
        mov    ecx, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 2
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3230:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3239:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0x208
        test   byte ptr [ebx + 0x11], 0x20
        jne    loc_4_324A
        xor    eax, eax
loc_4_324A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3260:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 0x14]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    eax, ebx
        mov    ecx, dword ptr [esi + 0x2f]
        add    eax, 4
        cmp    eax, ecx
        ja     loc_4_32A4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xd]
        mov    edi, dword ptr [ebp + 0x10]
        lea    esi, [eax + ecx + 4]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_32A4:
        cmp    byte ptr [ebp + 0x18], 0
        jne    loc_4_3370
        mov    eax, dword ptr [esi + 0xd]
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 0x23]
        shl    eax, 8
        lea    edx, [eax + ecx]
        cmp    edx, 0x10000
        jb     loc_4_32CD
        sub    edx, 0xd800
loc_4_32CD:
        mov    eax, 0x10000
        mov    edi, dword ptr [esi + 0x2b]
        sub    eax, edx
        cmp    edi, 0x5f2
        mov    dword ptr [ebp - 4], eax
        jbe    loc_4_32E7
        mov    edi, 0x5f2
loc_4_32E7:
        sub    edi, ecx
        add    edi, 3
        and    edi, 0xfffffffc
        add    ecx, edi
        cmp    edi, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x2f], ecx
        jbe    loc_4_3329
        mov    eax, dword ptr [ebp - 4]
        or     dl, 1
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     loc_4_331D
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_331D:
        mov    dx, 0x2800
        mov    eax, dword ptr [ebp - 4]
        sub    edi, eax
        add    dword ptr [ebp - 8], eax
loc_4_3329:
        push   edi
        or     dl, 1
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     loc_4_334A
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_334A:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xd]
        mov    edi, dword ptr [ebp + 0x10]
        lea    esi, [eax + ecx + 4]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_4_3365:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_3370:
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [esi + 0x23]
        shl    ax, 8
        mov    ecx, dword ptr [ebp + 0xc]
        lea    ecx, [eax + ecx + 4]
        mov    eax, 0x10000
        mov    edx, ecx
        and    edx, 0xffff
        sub    eax, edx
        mov    dword ptr [ebp - 4], eax
        cmp    ebx, eax
        jbe    loc_4_33BF
        push   eax
        or     cl, 1
        push   ecx
        push   edi
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     loc_4_33B5
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_33B5:
        mov    cx, 0x2800
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
loc_4_33BF:
        push   ebx
        or     cl, 1
        push   ecx
        push   edi
        push   esi
        call   sub_4_2370
        test   eax, eax
        je     loc_4_3365
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_33E0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0xef
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0x2c
        pop    edi
        pop    ebp
        ret    4
sub_4_3400:
        push   ebp
        mov    ecx, 0xb
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0xe3
        mov    edi, dword ptr [ebp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_3420:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     loc_4_34C8
        mov    eax, dword ptr [esi + 0x37]
        test   eax, eax
        jle    loc_4_34C8
        dec    eax
        mov    dword ptr [esi + 0x37], eax
        test   eax, eax
        jg     loc_4_34C8
        push   esi
        call   sub_4_27D0
        xor    eax, eax
        inc    dword ptr [esi + 0x10b]
        mov    dword ptr [esi + 0x33], 0
        mov    al, byte ptr [esi + 0x40]
        mov    cl, byte ptr [esi + eax*4 + 0x44]
        lea    eax, [esi + eax*4]
        mov    byte ptr [esi + 0x40], cl
        mov    byte ptr [eax + 0x44], 0
        mov    word ptr [eax + 0x45], 0
        mov    al, byte ptr [esi + 0x3f]
        test   al, al
        je     loc_4_347F
        dec    al
        mov    byte ptr [esi + 0x3f], al
loc_4_347F:
        mov    eax, dword ptr [esi + 0x11]
        push   eax
        push   esi
        call   sub_4_2A80
        test   eax, eax
        je     loc_4_34A8
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0
        push   0xa
        push   0                             ; -> loc_1_07F4
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebp
        ret    4
loc_4_34A8:
        push   0x10
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jge    loc_4_34C8
loc_4_34B8:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jl     loc_4_34B8
loc_4_34C8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_34D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   esi
        inc    dword ptr [esi + 0xf7]
        mov    di, word ptr [esi + 2]
        call   sub_4_28B0
        push   2
        push   0xba
        push   edi
        push   esi
        call   sub_4_2340
        push   0x22
        push   0xa0
        push   edi
        push   esi
        call   sub_4_2340
        lea    eax, [edi + 0xa0]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 0xc2
        cmp    al, 2
        je     loc_4_353A
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   0xc
        push   0                             ; -> loc_1_0808
        push   1
        push   eax
        call   sub_4_0080
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_353A:
        mov    word ptr [esi], 8
        xor    ebx, ebx
loc_4_3541:
        push   edi
        push   esi
        call   sub_4_2EC0
        and    eax, 0xff
        cmp    eax, dword ptr [esi + 0x23]
        je     loc_4_35E7
        cmp    dword ptr [esi + 0x11], 0
        mov    eax, 2
        je     loc_4_356A
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_4_3870
loc_4_356A:
        cmp    eax, 1
        jne    loc_4_3594
        mov    al, byte ptr [esi + 0x28]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x23], ecx
        cmp    al, 0x28
        jae    loc_4_3581
        mov    al, 0xff
loc_4_3581:
        and    ax, 0xff
        push   eax
        push   0xa6
        push   edi
        push   esi
        call   sub_4_2340
        jmp    loc_4_35DA
loc_4_3594:
        cmp    eax, 2
        jne    loc_4_35D6
        push   edi
        push   esi
        call   sub_4_2EC0
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [esi + 0x23], ecx
        cmp    al, 0x28
        jae    loc_4_35AF
        mov    al, 0xff
loc_4_35AF:
        and    ax, 0xff
        push   eax
        push   0xa6
        push   edi
        push   esi
        call   sub_4_2340
        push   5
        push   0xae
        push   edi
        push   esi
        call   sub_4_2340
        inc    dword ptr [esi + 0xff]
        jmp    loc_4_35DA
loc_4_35D6:
        test   eax, eax
        jl     loc_4_3626
loc_4_35DA:
        inc    ebx
        cmp    ebx, 0xd9
        jb     loc_4_3541
loc_4_35E7:
        push   0x10
        push   0xae
        push   edi
        push   esi
        call   sub_4_2340
        push   0
        push   0xba
        push   edi
        push   esi
        call   sub_4_2340
        cmp    byte ptr [esi + 0x3f], 0
        je     loc_4_3616
        push   esi
        call   sub_4_2E40
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_3616:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_4_1070
        cmp    eax, -1
        jl     loc_4_3616
loc_4_3626:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_3630:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   0
        mov    bx, word ptr [edi + 2]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
loc_4_3652:
        lea    eax, [ebx + 0xae]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 0x10
        jne    loc_4_37C6
        push   5
        push   0xae
        push   ebx
        push   edi
        call   sub_4_2340
        push   ebx
        push   edi
        call   sub_4_2EC0
        and    eax, 0xff
        cmp    eax, dword ptr [edi + 0x23]
        je     loc_4_371B
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 1
        cmp    dword ptr [edi + 0x11], 0
        mov    eax, 2
        je     loc_4_36A6
        push   esi
        call   sub_4_3870
loc_4_36A6:
        cmp    eax, 1
        jne    loc_4_36D0
        mov    al, byte ptr [edi + 0x28]
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [edi + 0x23], ecx
        cmp    al, 0x28
        jae    loc_4_36BD
        mov    al, 0xff
loc_4_36BD:
        and    ax, 0xff
        push   eax
        push   0xa6
        push   ebx
        push   edi
        call   sub_4_2340
        jmp    loc_4_371F
loc_4_36D0:
        cmp    eax, 2
        jne    loc_4_3712
        push   ebx
        push   edi
        call   sub_4_2EC0
        xor    ecx, ecx
        mov    cl, al
        dec    al
        mov    dword ptr [edi + 0x23], ecx
        cmp    al, 0x28
        jae    loc_4_36EB
        mov    al, 0xff
loc_4_36EB:
        and    ax, 0xff
        push   eax
        push   0xa6
        push   ebx
        push   edi
        call   sub_4_2340
        push   5
        push   0xae
        push   ebx
        push   edi
        call   sub_4_2340
        inc    dword ptr [edi + 0xff]
        jmp    loc_4_371B
loc_4_3712:
        test   eax, eax
        jge    loc_4_371F
        jmp    loc_4_37D1
loc_4_371B:
        mov    byte ptr [ebp - 1], 1
loc_4_371F:
        lea    eax, [ebx + 0xae]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        test   al, 0xa
        je     loc_4_37B9
        mov    dword ptr [edi + 0x37], 0
        mov    dword ptr [edi + 0x33], 0
        push   0xa
        push   0xae
        push   ebx
        push   edi
        call   sub_4_2340
        lea    eax, [ebx + 0xa8]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        push   ecx
        call   sub_4_0270
        and    al, 0xf
        xor    edx, edx
        mov    dl, byte ptr [edi + 0x40]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], 1
        mov    al, byte ptr [edi + edx*4 + 0x44]
        lea    ecx, [edi + edx*4]
        mov    byte ptr [edi + 0x40], al
        mov    byte ptr [ecx + 0x44], 0
        mov    word ptr [ecx + 0x45], 0
        mov    al, byte ptr [edi + 0x3f]
        test   al, al
        je     loc_4_3799
        dec    al
        mov    byte ptr [edi + 0x3f], al
        je     loc_4_3799
        push   edi
        call   sub_4_2E40
loc_4_3799:
        mov    al, byte ptr [ebp - 3]
        push   eax
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jge    loc_4_37B5
loc_4_37A8:
        push   0
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jl     loc_4_37A8
loc_4_37B5:
        xor    al, al
        jmp    loc_4_37BB
loc_4_37B9:
        mov    al, 1
loc_4_37BB:
        test   byte ptr [ebp - 1], al
        je     loc_4_3652
        jmp    loc_4_37D1
loc_4_37C6:
        push   esi
        push   edi
        call   sub_4_34D0
        mov    byte ptr [ebp - 2], 1
loc_4_37D1:
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_4_0010
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 2]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_37F0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_4_3803
        mov    eax, 0x3f
        pop    ebp
        ret    8
loc_4_3803:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0x11]
        ret    8
        db  08Dh, 049h, 000h
sub_4_3810:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 2]
        mov    byte ptr [eax + 0x10f], 0
        mov    dl, byte ptr [eax + 9]
        add    ecx, 0x20
        and    dl, 0xf1
        mov    eax, dword ptr [eax + 4]
        push   edx
        push   ecx
        push   eax
        call   sub_4_0280
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_3840:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 2]
        mov    byte ptr [eax + 0x10f], 1
        mov    dl, byte ptr [eax + 9]
        add    ecx, 0x20
        push   edx
        mov    eax, dword ptr [eax + 4]
        push   ecx
        push   eax
        call   sub_4_0280
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3870:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xb3]
        inc    eax
        mov    dword ptr [esi + 0xb3], eax
        cmp    eax, 0xa
        jbe    loc_4_38AD
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_4_38AD
        push   0
        push   esi
        call   sub_4_1070
        cmp    eax, -1
        jne    loc_4_38AD
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38AD:
        lea    edi, [esi + 0xbd]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0xfc
        push   edi
        push   ecx
        call   sub_4_2F10
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     loc_4_3913
        add    eax, 3
        cmp    eax, 3
        ja     loc_4_3913
        jmp    dword ptr [eax*4]             ; -> obj4:0x3968
loc_4_38DB:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38E5:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38EF:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_38FC:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_3C20
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3913:
        sub    dword ptr [ebp - 4], 0xe
        mov    ecx, dword ptr [esi + 0x7f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     loc_4_3923
        mov    ecx, eax
loc_4_3923:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x1c], esi
        push   eax
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0xc], 0
        push   ecx
        inc    dword ptr [esi + 0x87]
        lea    ecx, [esi + 0xcb]
        lea    edx, [ebp - 0x1c]
        mov    byte ptr [esi + 0x41], 1
        push   ecx
        mov    eax, dword ptr [esi + 0x7b]
        push   0xe
        push   edi
        push   edi
        push   edx
        push   eax
        call   sub_1_00B0
loc_4_3957:
        cmp    dword ptr [ebp - 0x18], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
        ; ---- dispatch table (4 entries) ----
        dd  loc_4_38DB
        dd  loc_4_38E5
        dd  loc_4_38EF
        dd  loc_4_38FC
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_3980:
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
        je     loc_4_39CB
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        add    edx, 0xe
        mov    ecx, dword ptr [ebp + 0x14]
        push   edx
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_054E
loc_4_39C0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_39CB:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [ebp + 0x10]
        lea    edi, [ecx + edx]
        cmp    edi, eax
        jbe    loc_4_39E2
        sub    eax, edx
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_4_39E5
loc_4_39E2:
        mov    dword ptr [ebp - 0x18], ecx
loc_4_39E5:
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
loc_4_3A01:
        test   edi, edi
        je     loc_4_3A54
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        call   sub_1_0524
loc_4_3A18:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    ebx, eax
        jb     loc_4_3A21
        mov    ebx, eax
loc_4_3A21:
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_4_3260
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     loc_4_3A54
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_0548
loc_4_3A4E:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_3A01
loc_4_3A54:
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
sub_4_3A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        je     loc_4_3A92
loc_4_3A7E:
        mov    ecx, dword ptr [eax + 0x24]
        push   eax
        push   esi
        mov    dword ptr [esi + 0x38], ecx
        call   sub_4_3AA0
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        jne    loc_4_3A7E
loc_4_3A92:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3AA0:
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
loc_4_3ABC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 0x7f]
        add    ebx, 0xe
        cmp    ebx, eax
        jb     loc_4_3ACE
        mov    ebx, eax
loc_4_3ACE:
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
loc_4_3AE8:
        lea    ecx, [ebp - 0x14]
        lea    edx, [ebp - 0x10]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        lea    ecx, [esi + 0xbd]
        mov    dword ptr [ebp - 0x10], 0
        push   eax
        push   ebx
        push   ecx
        call   sub_4_3B70
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [esi + 0xcb]
        push   eax
        mov    dword ptr [ebp - 0x28], esi
        push   edx
        mov    dword ptr [ebp - 0x20], ecx
        push   0xe
        lea    ecx, [esi + 0xbd]
        push   ecx
        lea    edx, [ebp - 0x28]
        push   ecx
        mov    dword ptr [ebp - 0x18], edi
        push   edx
        mov    ecx, dword ptr [esi + 0x7b]
        push   ecx
        call   sub_1_00B0
loc_4_3B37:
        push   0
        inc    dword ptr [esi + 0x83]
        mov    byte ptr [esi + 0x41], 1
        mov    esi, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        call   sub_1_044A
loc_4_3B50:
        push   esi
        call   sub_4_16E0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3B70:
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
loc_4_3B96:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     loc_4_3C14
loc_4_3BA3:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        lea    edx, [ecx + ebx]
        cmp    edx, dword ptr [ebp + 0xc]
        jae    loc_4_3BEE
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_05BC
loc_4_3BC0:
        mov    ecx, dword ptr [ebp + 0x18]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_0548
loc_4_3BCE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0524
loc_4_3BE4:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    loc_4_3BA3
loc_4_3BEE:
        cmp    dword ptr [esi], 0
        je     loc_4_3C14
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
loc_4_3C0D:
        mov    ecx, dword ptr [ebp + 0x18]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_3C14:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_4_3C20:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   eax, 0x208
        jne    loc_4_3C6C
        test   eax, 0x206
        je     loc_4_3C48
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        inc    dword ptr [eax + 0x9b]
        ret    8
loc_4_3C48:
        test   eax, 0x204
        je     loc_4_3C5C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 0x97]
        ret    8
loc_4_3C5C:
        test   eax, 0x202
        je     loc_4_3C6C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x9b]
loc_4_3C6C:
        pop    ebp
        ret    8
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x59  RW-
        db  02Dh, 03Fh, 063h, 06Fh, 06Dh, 06Dh, 065h, 06Eh, 074h, 03Ah, 022h, 05Fh, 048h, 050h, 05Fh, 020h
        db  068h, 070h, 065h, 069h, 073h, 061h, 02Eh, 063h, 028h, 054h, 068h, 075h, 020h, 053h, 065h, 070h
        db  020h, 031h, 035h, 020h, 031h, 037h, 03Ah, 035h, 037h, 03Ah, 032h, 038h, 020h, 031h, 039h, 039h
        db  034h, 029h, 020h, 043h, 02Eh, 030h, 02Eh, 036h, 020h, 063h, 06Fh, 06Dh, 070h, 06Ch, 069h, 065h
        db  064h, 028h, 053h, 065h, 070h, 020h, 032h, 036h, 020h, 031h, 039h, 039h, 034h, 020h, 031h, 038h
        db  03Ah, 035h, 036h, 03Ah, 030h, 032h, 029h, 022h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0xeb  RW-
        db  000h, 000h, 000h, 002h, 004h, 006h, 000h, 008h, 000h, 000h, 00Ah, 00Ch, 00Eh, 000h, 000h, 000h
        db  010h, 020h, 000h, 040h, 000h, 060h, 070h, 030h, 04Eh, 04Fh, 054h, 049h, 043h, 045h, 03Ah, 020h
        db  041h, 064h, 061h, 070h, 074h, 065h, 072h, 020h, 06Eh, 06Fh, 074h, 020h, 070h, 072h, 065h, 073h
        db  065h, 06Eh, 074h, 000h, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 049h, 06Eh, 076h, 061h, 06Ch
        db  069h, 064h, 020h, 049h, 02Fh, 04Fh, 020h, 061h, 064h, 064h, 072h, 065h, 073h, 073h, 028h, 065h
        db  073h, 029h, 000h, 000h, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 049h, 06Eh, 076h, 061h, 06Ch
        db  069h, 064h, 020h, 049h, 06Eh, 074h, 065h, 072h, 072h, 075h, 070h, 074h, 020h, 056h, 065h, 063h
        db  074h, 06Fh, 072h, 000h, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 049h, 06Eh, 076h, 061h, 06Ch
        db  069h, 064h, 020h, 044h, 04Dh, 041h, 020h, 043h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 000h, 000h
        db  045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh, 074h, 020h, 061h, 06Ch
        db  06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h, 04Dh, 041h, 020h, 043h, 068h, 061h, 06Eh, 06Eh
        db  065h, 06Ch, 000h, 000h, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh
        db  074h, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h, 04Dh, 041h, 020h, 062h
        db  075h, 066h, 066h, 065h, 072h, 020h, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 000h, 000h, 000h, 000h
        db  048h, 050h, 020h, 045h, 049h, 053h, 041h, 02Dh, 033h, 032h, 032h
VxD__LDATA ENDS
