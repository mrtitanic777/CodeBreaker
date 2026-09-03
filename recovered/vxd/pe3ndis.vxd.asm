; PE3NDIS -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7619 instructions recovered by recursive descent

Declare_Virtual_Device PE3NDIS, 3, 10, PE3NDIS_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5b6c  R-X
loc_1_0000:
        mov    al, byte ptr [0x668]          ; -> obj1:0x0668
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x668]              ; -> obj1:0x0668
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_1B70
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
PE3NDIS_Control:
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
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0444:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_044A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_0456:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_045C:
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
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_048C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
sub_1_049E:
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_04B2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_04B8:
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
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
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
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
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
        db  'PE3NDIS '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  PE3NDIS_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    edi, [esi + 0x20]
        push   edi
        call   dword ptr [esi + 0x7c]
        cmp    al, 0x72
        je     loc_1_06E0
        push   0x72
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   6
        push   edi
        call   dword ptr [esi + 0x7c]
        cmp    al, 0x72
        je     loc_1_06E0
        push   esi
        call   sub_1_09F0
        push   0x72
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   6
        push   edi
        call   dword ptr [esi + 0x7c]
        cmp    al, 0x72
        je     loc_1_06E0
        mov    byte ptr [esi + 0x76b], 1
        push   edi
        call   dword ptr [esi + 0x5c]
        push   6
        push   3
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06E0:
        mov    byte ptr [esi + 0x76b], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x4e]
        mov    byte ptr [ecx + 0x6e1], al
        mov    al, byte ptr [ecx + 0x4f]
        mov    byte ptr [ecx + 0x6e2], al
        mov    al, byte ptr [ecx + 0x50]
        mov    byte ptr [ecx + 0x6e3], al
        mov    al, byte ptr [ecx + 0x45]
        mov    byte ptr [ecx + 0x6e4], al
        mov    al, byte ptr [ecx + 0x44]
        mov    byte ptr [ecx + 0x6e5], al
        mov    al, byte ptr [ecx + 0x43]
        mov    byte ptr [ecx + 0x6e6], al
        cmp    byte ptr [ecx + 0x6db], 0
        jne    loc_1_07AA
        cmp    byte ptr [ecx + 0x6dc], 0
        jne    loc_1_07AA
        cmp    byte ptr [ecx + 0x6dd], 0
        jne    loc_1_07AA
        cmp    byte ptr [ecx + 0x6de], 0
        jne    loc_1_07AA
        cmp    byte ptr [ecx + 0x6df], 0
        jne    loc_1_07AA
        cmp    byte ptr [ecx + 0x6e0], 0
        jne    loc_1_07AA
        mov    al, byte ptr [ecx + 0x6e1]
        mov    byte ptr [ecx + 0x6db], al
        mov    al, byte ptr [ecx + 0x6e2]
        mov    byte ptr [ecx + 0x6dc], al
        mov    al, byte ptr [ecx + 0x6e3]
        mov    byte ptr [ecx + 0x6dd], al
        mov    al, byte ptr [ecx + 0x6e4]
        mov    byte ptr [ecx + 0x6de], al
        mov    al, byte ptr [ecx + 0x6e5]
        mov    byte ptr [ecx + 0x6df], al
        mov    al, byte ptr [ecx + 0x6e6]
        mov    byte ptr [ecx + 0x6e0], al
loc_1_07AA:
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_07B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0xf2
        lea    edi, [esi + 0x20]
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   0xb0
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
        push   0x83
        push   0
        push   edi
        call   dword ptr [esi + 0x70]
        push   0xc1
        push   1
        push   edi
        call   dword ptr [esi + 0x70]
        push   0x83
        push   2
        push   edi
        call   dword ptr [esi + 0x70]
        push   0xc1
        push   3
        push   edi
        call   dword ptr [esi + 0x70]
        push   6
        push   4
        push   edi
        call   dword ptr [esi + 0x70]
        mov    al, byte ptr [esi + 0xd0]
        push   eax
        push   5
        sub    bl, bl
        push   edi
        call   dword ptr [esi + 0x70]
        push   0x20
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
loc_1_0820:
        xor    eax, eax
        mov    al, bl
        mov    al, byte ptr [esi + eax + 0x6db]
        push   eax
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 0x70]
        inc    bl
        cmp    bl, 6
        jb     loc_1_0820
        push   0xb4
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
        sub    bl, bl
        mov    eax, dword ptr [esi + 0x708]
        mov    eax, dword ptr [eax + 0xc]
        and    eax, 4
        mov    dword ptr [ebp - 4], eax
loc_1_0858:
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 0xff
        jne    loc_1_0873
        xor    eax, eax
        mov    al, bl
        mov    al, byte ptr [esi + eax + 0x70c]
        and    eax, 0xff
loc_1_0873:
        push   eax
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 0x70]
        inc    bl
        cmp    bl, 8
        jb     loc_1_0858
        push   0xb8
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
        push   7
        push   0xb
        push   edi
        call   dword ptr [esi + 0x70]
        push   0
        push   0xd
        push   edi
        call   dword ptr [esi + 0x70]
        push   0x72
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   edi
        call   dword ptr [esi + 0x58]
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x108
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   4
        lea    eax, [ebx + 0x20]
        push   4
        push   eax
        call   dword ptr [ebx + 0x70]
        push   3
        lea    eax, [ebx + 0x20]
        push   5
        push   eax
        call   dword ptr [ebx + 0x70]
        push   0
        lea    eax, [ebx + 0x20]
        push   8
        push   eax
        call   dword ptr [ebx + 0x70]
        push   1
        lea    eax, [ebx + 0x20]
        push   8
        push   eax
        call   dword ptr [ebx + 0x70]
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_3750
        push   0x100
        push   0x4cc0                        ; -> obj1:0x4CC0
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x74]
        push   0x81
        push   0xa
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x70]
        sub    esi, esi
loc_1_0925:
        push   0
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x7c]
        test   al, 0x80
        jne    loc_1_093B
        inc    esi
        cmp    esi, 0x100
        jb     loc_1_0925
loc_1_093B:
        mov    byte ptr [ebp - 1], 1
        cmp    esi, 0x100
        jae    loc_1_09BB
        push   0xff
        push   0
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x70]
        push   0xff
        push   1
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x70]
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_3720
        push   4
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x80]
        cmp    word ptr [ebp - 6], 0x100
        jne    loc_1_09BB
        push   0x100
        lea    eax, [ebp - 0x108]
        push   eax
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x80]
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_3770
        mov    esi, 0x4cc0                   ; -> obj1:0x4CC0
        lea    edi, [ebp - 0x108]
        mov    ecx, 0x40
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_09BB
        mov    byte ptr [ebp - 1], 0
loc_1_09BB:
        mov    al, byte ptr [ebx + 0xd0]
        push   eax
        push   5
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x70]
        push   6
        push   4
        lea    eax, [ebx + 0x20]
        push   eax
        call   dword ptr [ebx + 0x70]
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_07B0
        or     al, al
        jne    loc_1_0A20
        push   1
        push   4
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_1_0A20:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0x2a]
        or     bl, bl
        je     loc_1_0A4A
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x5c]
loc_1_0A4A:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_0AA5
loc_1_0A66:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0A80
        jmp    loc_1_0A92
        db  090h
loc_1_0A80:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x74]
        add    edi, dword ptr [ebp - 8]
loc_1_0A92:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0A66
loc_1_0AA5:
        or     bl, bl
        je     loc_1_0AB0
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x58]
loc_1_0AB0:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_0B0B
loc_1_0AD2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0AE0:
        mov    ecx, eax
        and    ecx, 0x80000000
        cmp    ecx, 1
        sbb    edi, edi
        inc    edi
        mov    cl, bl
        and    ecx, 1
        xor    edi, ecx
        add    eax, eax
        shr    bl, 1
        or     edi, edi
        je     loc_1_0B02
        xor    eax, 0x4c11db7
loc_1_0B02:
        dec    esi
        jne    loc_1_0AE0
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_1_0AD2
loc_1_0B0B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B20:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0AC0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0B35:
        add    ecx, ecx
        test   eax, edx
        je     loc_1_0B3E
        or     ecx, 1
loc_1_0B3E:
        add    edx, edx
        cmp    edx, 0x40
        jb     loc_1_0B35
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 0xc]
        shr    edx, 3
        mov    byte ptr [eax], dl
        mov    dl, 1
        and    cl, 7
        mov    eax, dword ptr [ebp + 0x10]
        shl    dl, cl
        mov    byte ptr [eax], dl
        pop    ebp
        ret    0xc
        db  090h
sub_1_0B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
        lea    esp, [esp]
loc_1_0B80:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x70c], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_0B80
        push   0x4c60                        ; -> obj1:0x4C60
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_0BE7
        sub    edi, edi
loc_1_0BB5:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x4c60]         ; -> obj1:0x4C60
        push   eax
        call   sub_1_0B20
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x70c], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_0BB5
loc_1_0BE7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0BF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x20]
        mov    al, byte ptr [esi + 0xd0]
        push   eax
        push   5
        push   edi
        call   dword ptr [esi + 0x70]
        cmp    byte ptr [esi + 0x2a], 0
        je     loc_1_0C12
        push   edi
        call   dword ptr [esi + 0x60]
loc_1_0C12:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0C20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0xf2
        lea    edi, [esi + 0x20]
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   0xb4
        sub    bl, bl
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
loc_1_0C44:
        push   0xff
        mov    al, bl
        add    al, 8
        push   eax
        push   edi
        call   dword ptr [esi + 0x70]
        inc    bl
        cmp    bl, 8
        jb     loc_1_0C44
        push   0x72
        push   6
        push   edi
        call   dword ptr [esi + 0x70]
        push   0xb8
        push   7
        push   edi
        call   dword ptr [esi + 0x70]
        cmp    byte ptr [esi + 0x2a], 0
        je     loc_1_0C76
        push   edi
        call   dword ptr [esi + 0x60]
loc_1_0C76:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   0xf2
        lea    esi, [edi + 0x20]
        push   6
        push   esi
        call   dword ptr [edi + 0x70]
        push   0xb4
        sub    ebx, ebx
        push   7
        push   esi
        call   dword ptr [edi + 0x70]
loc_1_0CA4:
        mov    al, byte ptr [edi + ebx + 0x70c]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        push   esi
        call   dword ptr [edi + 0x70]
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0CA4
        push   0x72
        push   6
        push   esi
        call   dword ptr [edi + 0x70]
        push   0xb8
        push   7
        push   esi
        call   dword ptr [edi + 0x70]
        cmp    byte ptr [edi + 0x2a], 0
        je     loc_1_0CD7
        push   esi
        call   dword ptr [edi + 0x60]
loc_1_0CD7:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, dword ptr [esi]
        or     al, 0x60
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_0D05:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_0D05
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 0x33]
        or     al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_0D1D:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_0D1D
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        sar    al, 3
        and    al, 0x47
        mov    byte ptr [ebp - 3], al
        mov    ecx, 1
loc_1_0D44:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_0D44
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xb8
        mov    al, byte ptr [ebp - 4]
        or     al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 3], al
        mov    cl, byte ptr [edi + 0x33]
        xor    cl, al
        mov    byte ptr [ebp - 4], cl
        or     ebx, ebx
        je     loc_1_0DE9
        mov    esi, dword ptr [ebp + 0xc]
loc_1_0D94:
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, 1
loc_1_0DA2:
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_0DA2
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        sar    al, 3
        and    al, 0x47
        mov    byte ptr [ebp - 1], al
        mov    ecx, 1
loc_1_0DC9:
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_0DC9
        mov    edx, dword ptr [edi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    byte ptr [ebp - 2], 0xb8
        mov    al, byte ptr [ebp - 1]
        or     al, byte ptr [ebp - 2]
        dec    ebx
        mov    byte ptr [esi], al
        inc    esi
        or     ebx, ebx
        jne    loc_1_0D94
loc_1_0DE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [eax + 8]
        mov    cl, byte ptr [eax + 0x33]
        xor    cl, bl
        or     esi, esi
        je     loc_1_0E26
        mov    edx, dword ptr [eax]
        add    dx, 2
loc_1_0E1D:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    loc_1_0E1D
loc_1_0E26:
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_0E30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0x36], 0x68
        mov    dword ptr [esi + 0x5c], 0xce0 ; -> loc_1_0CE0
        mov    dword ptr [esi + 0x60], 0xd70 ; -> loc_1_0D70
        mov    dword ptr [esi + 0x64], 0xe00 ; -> loc_1_0E00
        mov    dword ptr [esi + 0xf], 1
        call   sub_1_3E30
        push   0
        push   0
        push   esi
        call   sub_1_39B0
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0E70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x2a], 0
        jne    loc_1_0E82
        sub    al, al
        pop    ebp
        ret    4
loc_1_0E82:
        lea    ecx, [eax + 0x20]
        push   ecx
        call   dword ptr [eax + 0x5c]
        mov    al, 1
        pop    ebp
        ret    4
        db  090h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_105A
        mov    byte ptr [esi + 0x730], 1
        inc    dword ptr [esi + 0xac]
        mov    dword ptr [ebp - 8], 0
        lea    edi, [esi + 0x20]
loc_1_0EC0:
        push   0
        push   edi
        call   dword ptr [esi + 0x7c]
        mov    byte ptr [ebp - 1], al
        push   1
        push   edi
        call   dword ptr [esi + 0x7c]
        mov    bl, al
        or     bl, bl
        jne    loc_1_0EDF
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0F9B
loc_1_0EDF:
        cmp    bl, 0xff
        je     loc_1_0EEF
        cmp    byte ptr [ebp - 1], 0xff
        je     loc_1_0EEF
        cmp    bl, byte ptr [ebp - 1]
        jne    loc_1_0EFD
loc_1_0EEF:
        push   esi
        call   sub_1_0670
        or     al, al
        je     loc_1_0F9B
loc_1_0EFD:
        test   bl, 0x81
        je     loc_1_0F19
        test   bl, 1
        je     loc_1_0F0D
        inc    dword ptr [esi + 0x6fb]
loc_1_0F0D:
        push   esi
        call   sub_1_1070
        push   esi
        call   sub_1_1990
loc_1_0F19:
        test   bl, 0x40
        je     loc_1_0F28
        and    bl, 0x7e
        push   ebx
        push   1
        push   edi
        call   dword ptr [esi + 0x70]
loc_1_0F28:
        test   byte ptr [ebp - 1], 0x83
        jne    loc_1_0F37
        cmp    byte ptr [esi + 0xca], 0
        je     loc_1_0F8E
loc_1_0F37:
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   0
        push   edi
        call   dword ptr [esi + 0x70]
        mov    dword ptr [esi + 0xb0], 0
        cmp    byte ptr [esi + 0xca], 0
        je     loc_1_0F64
        mov    byte ptr [esi + 0xcb], 2
        mov    byte ptr [esi + 0xca], 0
        jmp    loc_1_0F6D
loc_1_0F64:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0xcb], al
loc_1_0F6D:
        mov    byte ptr [esi + 0x769], 0
        cmp    dword ptr [esi + 0xc6], 0
        push   esi
        je     loc_1_0F85
        call   sub_1_1260
        jmp    loc_1_0F8E
loc_1_0F85:
        call   sub_1_0670
        or     al, al
        je     loc_1_0F9B
loc_1_0F8E:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 8
        jl     loc_1_0EC0
loc_1_0F9B:
        cmp    dword ptr [esi + 0x718], 0
        je     loc_1_0FBC
loc_1_0FA4:
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_0FBC
        push   esi
        call   sub_1_1070
        cmp    dword ptr [esi + 0x718], 0
        jne    loc_1_0FA4
loc_1_0FBC:
        cmp    byte ptr [esi + 0x76b], 0
        je     loc_1_0FCB
        push   esi
        call   sub_1_1360
loc_1_0FCB:
        mov    byte ptr [esi + 0x730], 0
        cmp    byte ptr [esi + 0x76b], 0
        jne    loc_1_0FDF
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_0FDF:
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_1020
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_1009
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_1009
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_1020
loc_1_1009:
        push   1
        lea    eax, [esi + 0x731]
        push   eax
        call   sub_1_04B2
        jmp    loc_1_1049
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1020:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_1049
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_1049
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_1049
        push   edi
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_1049:
        push   0x7d0
        add    esi, 0x75d
        push   esi
        call   sub_1_04B2
loc_1_105A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1070:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_1084:
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_11DC
        lea    edi, [esi + 0x20]
        push   0x81
        push   1
        push   edi
        call   dword ptr [esi + 0x70]
        push   5
        push   edi
        call   dword ptr [esi + 0x7c]
        test   al, 0x40
        jne    loc_1_11DC
        inc    dword ptr [esi + 0xb0]
        mov    dword ptr [esi + 0x720], 0
        push   edi
        call   sub_1_3720
        push   4
        lea    eax, [esi + 0x6d3]
        push   eax
        push   edi
        call   dword ptr [esi + 0x80]
        mov    ax, word ptr [esi + 0x6d5]
        mov    word ptr [esi + 0xd1], ax
        mov    ebx, dword ptr [esi + 0x714]
        add    bx, 0xe
        cmp    ax, bx
        jae    loc_1_10F3
        mov    bx, ax
loc_1_10F3:
        mov    byte ptr [ebp - 1], 1
        test   byte ptr [esi + 0xcc], 0x28
        jne    loc_1_1150
        push   6
        lea    eax, [esi + 0xd3]
        push   eax
        push   edi
        call   dword ptr [esi + 0x80]
        sub    word ptr [esi + 0xd1], 6
        sub    bx, 6
        cmp    dword ptr [esi + 0xd5], -1
        jne    loc_1_1140
        cmp    byte ptr [esi + 0xd3], 0xff
        jne    loc_1_1140
        cmp    byte ptr [esi + 0xd4], 0xff
        jne    loc_1_1140
        mov    byte ptr [ebp - 1], 0
        jmp    loc_1_116D
        db  08Dh, 049h, 000h
loc_1_1140:
        mov    eax, ebx
        and    eax, 0xffff
        push   eax
        lea    eax, [esi + 0xd9]
        jmp    loc_1_115E
loc_1_1150:
        mov    eax, ebx
        and    eax, 0xffff
        push   eax
        lea    eax, [esi + 0xd3]
loc_1_115E:
        push   eax
        push   edi
        call   dword ptr [esi + 0x80]
        sub    word ptr [esi + 0xd1], bx
loc_1_116D:
        push   edi
        call   sub_1_3770
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_118E
        push   esi
        call   sub_1_1590
        cmp    eax, 3
        je     loc_1_118E
        inc    dword ptr [esi + 0x6eb]
        mov    byte ptr [ebp - 2], 1
loc_1_118E:
        mov    ax, word ptr [esi + 0xd1]
        or     ax, ax
        je     loc_1_11C1
        cmp    ax, 8
        ja     loc_1_11B9
        mov    ebx, eax
        and    ebx, 0xffff
        or     ebx, ebx
        jle    loc_1_11C1
loc_1_11AC:
        push   8
        push   edi
        call   dword ptr [esi + 0x7c]
        dec    ebx
        or     ebx, ebx
        jg     loc_1_11AC
        jmp    loc_1_11C1
loc_1_11B9:
        push   4
        push   0xe
        push   edi
        call   dword ptr [esi + 0x70]
loc_1_11C1:
        cmp    dword ptr [esi + 0xb0], 5
        jbe    loc_1_1084
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 0xb0], 0
loc_1_11DC:
        cmp    dword ptr [esi + 0x718], 0
        je     loc_1_1227
loc_1_11E5:
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_1230
        mov    edi, dword ptr [esi + 0x718]
        push   edi
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x718], eax
        mov    dword ptr [esi + 0x720], edi
        mov    ebx, dword ptr [edi + 0x28]
        push   esi
        call   sub_1_13F0
        push   0
        mov    eax, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x718], 0
        jne    loc_1_11E5
loc_1_1227:
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_1240
loc_1_1230:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1240:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_1252
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_1_0290
loc_1_1252:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1260:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xc6]
        or     edi, edi
        jne    loc_1_1291
        push   2
        push   6
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1291:
        mov    al, byte ptr [esi + 0xcb]
        test   al, 0x80
        je     loc_1_12D1
        test   al, 4
        je     loc_1_12C9
        push   4
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x7c]
        and    eax, 0xff
        and    eax, 0xfffffff0
        cmp    eax, 0x10
        jne    loc_1_12C3
        inc    dword ptr [esi + 0x6f3]
        inc    dword ptr [esi + 0x6e7]
        jmp    loc_1_12D7
loc_1_12C3:
        inc    dword ptr [esi + 0x6f7]
loc_1_12C9:
        inc    dword ptr [esi + 0x6e7]
        jmp    loc_1_12D7
loc_1_12D1:
        inc    dword ptr [esi + 0x6ef]
loc_1_12D7:
        cmp    byte ptr [edi + 0x2c], 0
        jne    loc_1_1300
        mov    al, byte ptr [esi + 0xcb]
        and    al, 0x80
        cmp    al, 1
        mov    ebx, dword ptr [edi + 0x28]
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 0x1c]
        jmp    loc_1_1327
loc_1_1300:
        cmp    dword ptr [esi + 0x718], 0
        jne    loc_1_1311
        mov    dword ptr [esi + 0x718], edi
        jmp    loc_1_131A
loc_1_1311:
        mov    eax, dword ptr [esi + 0x71c]
        mov    dword ptr [eax + 0x24], edi
loc_1_131A:
        mov    dword ptr [esi + 0x71c], edi
        mov    dword ptr [edi + 0x24], 0
loc_1_1327:
        mov    dword ptr [esi + 0xc6], 0
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_1349
        push   2
        push   esi
        call   sub_1_3440
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1349:
        push   esi
        call   sub_1_1990
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xc6]
        or     ecx, ecx
        je     loc_1_1396
        mov    dword ptr [esi + 0xc6], 0
        inc    dword ptr [esi + 0x6ef]
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc0000001
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x1c]
loc_1_1396:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_13D5
        nop    
loc_1_13A0:
        mov    edx, dword ptr [esi + 0xbe]
        push   0xc0000001
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0xbe], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x6ef]
        cmp    dword ptr [esi + 0xbe], 0
        jne    loc_1_13A0
loc_1_13D5:
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_13E6
        push   2
        push   esi
        call   sub_1_3440
loc_1_13E6:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_13F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_1420
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1420:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x714]
        add    edi, 0xe
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_1434
        mov    edi, dword ptr [ebp - 4]
loc_1_1434:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0xd3]
        push   eax
        call   sub_1_14B0
        cmp    edi, 0xe
        jae    loc_1_1470
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0xd3]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1470:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0xe1]
        push   eax
        push   0xe
        lea    eax, [esi + 0xd3]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 0x10]
        je     loc_1_1504
loc_1_14D5:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     loc_1_1504
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_14D5
loc_1_1504:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1515
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1515:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_1524:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_1533
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_1536
loc_1_1533:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1536:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05BC
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_1572
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_157D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        jmp    loc_1_1524
loc_1_1572:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_157D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1590:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 0x6d5]
        cmp    eax, 0x5ea
        jbe    loc_1_15B0
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_15B0:
        mov    dword ptr [edx + 0x6d7], eax
        mov    ecx, dword ptr [edx + 0x714]
        add    ecx, 0xe
        cmp    ecx, eax
        jbe    loc_1_15C5
        mov    ecx, eax
loc_1_15C5:
        cmp    ecx, 0xe
        jae    loc_1_15EC
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0xd3]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x708]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_15EC:
        sub    eax, 0xe
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    eax, [edx + 0xe1]
        push   eax
        push   0xe
        lea    eax, [edx + 0xd3]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x708]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x720], 0
        je     loc_1_16C7
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, edi
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_169A
loc_1_165A:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x720]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_14B0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_169A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_165A
loc_1_169A:
        cmp    edi, ebx
        jbe    loc_1_16B5
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_16B5:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    ebx, edi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_16C7:
        cmp    word ptr [esi + 0xd1], 0
        jbe    loc_1_1708
        lea    ebx, [esi + 0x20]
        push   ebx
        call   sub_1_3720
        xor    eax, eax
        mov    ax, word ptr [esi + 0xd1]
        push   eax
        mov    eax, dword ptr [esi + 0x714]
        add    eax, esi
        add    eax, 0xe1
        push   eax
        push   ebx
        call   dword ptr [esi + 0x80]
        mov    word ptr [esi + 0xd1], 0
        push   ebx
        call   sub_1_3770
loc_1_1708:
        mov    ecx, dword ptr [esi + 0x6d7]
        mov    eax, dword ptr [ebp + 0x14]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    loc_1_173A
        cmp    edi, ecx
        jbe    loc_1_1733
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1733:
        sub    ecx, edi
        mov    dword ptr [ebp - 0x18], ecx
        jmp    loc_1_173D
loc_1_173A:
        mov    dword ptr [ebp - 0x18], eax
loc_1_173D:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0500
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_17C1
loc_1_1770:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     loc_1_177D
        mov    ebx, dword ptr [ebp - 8]
loc_1_177D:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 0xd3
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05BC
        add    edi, ebx
        sub    dword ptr [ebp - 0xc], ebx
        je     loc_1_17C1
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_17C1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1770
loc_1_17C1:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax + 8]
        lea    ebx, [edi + 0x24]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_1810
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1810:
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_1827
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1827:
        cmp    byte ptr [esi + 0x76b], 0
        je     loc_1_1848
        push   esi
        call   sub_1_0670
        or     al, al
        jne    loc_1_1848
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1848:
        mov    eax, dword ptr [ebp + 8]
        push   0
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [esi + 0xac]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0500
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0524
        mov    ecx, dword ptr [esi + 0x6dd]
        lea    edx, [esi + 0x6db]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    loc_1_18A0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_18D7
        db  08Dh, 049h, 000h
loc_1_18A0:
        jbe    loc_1_18B0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_18D7
        db  005h, 000h, 000h, 000h, 000h
loc_1_18B0:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    loc_1_18C7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_18D7
loc_1_18C7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_18D7
        mov    dword ptr [ebp - 4], 0
loc_1_18D7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_18F2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_1_0330
        mov    byte ptr [ebx + 8], al
        jmp    loc_1_18F6
loc_1_18F2:
        mov    byte ptr [ebx + 8], 0
loc_1_18F6:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_1910
        mov    eax, dword ptr [esi + 0xc2]
        mov    dword ptr [eax + 0x24], edi
        jmp    loc_1_1916
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1910:
        mov    dword ptr [esi + 0xbe], edi
loc_1_1916:
        mov    dword ptr [esi + 0xc2], edi
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_1956
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_1946
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_1946
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_1956
loc_1_1946:
        push   1
        add    esi, 0x731
        push   esi
        call   sub_1_04B2
        jmp    loc_1_1982
loc_1_1956:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_1982
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_1982
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_1982
        lea    edi, [esi + 0x20]
        push   edi
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_1982:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_1990:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x2a]
        or     al, al
        mov    byte ptr [ebp - 1], al
        je     loc_1_19AD
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x5c]
loc_1_19AD:
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_1AAF
        lea    ebx, [ebx]
loc_1_19C0:
        mov    edi, dword ptr [esi + 0xbe]
        or     edi, edi
        je     loc_1_1AAF
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0xbe], eax
        mov    dword ptr [esi + 0xc6], edi
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0500
        mov    ebx, dword ptr [ebp - 8]
        cmp    ebx, 0x3c
        jae    loc_1_19FA
        mov    ebx, 0x3c
loc_1_19FA:
        push   0xb8
        push   7
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x70]
        push   ebx
        push   8
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x70]
        shr    ebx, 8
        push   ebx
        push   8
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x70]
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_3750
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_0A30
        or     al, al
        jne    loc_1_1A57
        mov    ebx, dword ptr [edi + 0x28]
        push   0xc0000001
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [esi + 0xc6], 0
        jmp    loc_1_1AA2
loc_1_1A57:
        cmp    dword ptr [ebp - 8], 0x3c
        jae    loc_1_1A72
        mov    eax, 0x3c
        sub    eax, dword ptr [ebp - 8]
        push   eax
        push   0x4dc0                        ; -> loc_1_4DC0
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x74]
loc_1_1A72:
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_3840
        mov    al, byte ptr [esi + 0xd0]
        and    al, 3
        cmp    al, 3
        jne    loc_1_1A8B
        mov    byte ptr [edi + 0x2c], 0
loc_1_1A8B:
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_1AC5
        push   0x81
        push   0xa
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x70]
loc_1_1AA2:
        cmp    dword ptr [esi + 0xc6], 0
        je     loc_1_19C0
loc_1_1AAF:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1ABC
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x58]
loc_1_1ABC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1AC5:
        mov    ebx, dword ptr [edi + 0x28]
        push   0
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0xc6], 0
        push   3
        push   esi
        call   sub_1_3440
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1ABC
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x58]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1B00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x769], 0
        je     loc_1_1B45
        cmp    dword ptr [esi + 0xc6], 0
        je     loc_1_1B55
        mov    byte ptr [esi + 0xca], 1
        mov    byte ptr [esi + 0x769], 0
        push   7
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x10
        lea    eax, [esi + 0x20]
        push   eax
        call   dword ptr [esi + 0x5c]
        jmp    loc_1_1B55
loc_1_1B45:
        cmp    dword ptr [esi + 0xc6], 0
        je     loc_1_1B55
        mov    byte ptr [esi + 0x769], 1
loc_1_1B55:
        push   0x7d0
        add    esi, 0x75d
        push   esi
        call   sub_1_04B2
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    word ptr [ebp - 0x10], 8
        mov    word ptr [ebp - 0xe], 8
        mov    dword ptr [ebp - 0xc], 0x4f3c ; -> obj1:0x4F3C
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_1_041A
        mov    dword ptr [0x4e46], esi       ; -> obj1:0x4E46
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x4e04], eax       ; -> obj1:0x4E04
        mov    byte ptr [0x4e4a], 0          ; -> obj1:0x4E4A
        mov    dword ptr [0x4e3e], 0         ; -> obj1:0x4E3E
        mov    byte ptr [0x4e08], 3          ; -> obj1:0x4E08
        mov    byte ptr [0x4e09], 0          ; -> obj1:0x4E09
        mov    dword ptr [0x4e0a], 0         ; -> obj1:0x4E0A
        mov    dword ptr [0x4e0e], 0x22f0    ; -> obj1:0x4E0E
        mov    dword ptr [0x4e12], 0x2510    ; -> obj1:0x4E12
        mov    dword ptr [0x4e16], 0x17e0    ; -> obj1:0x4E16
        mov    dword ptr [0x4e1a], 0x1620    ; -> obj1:0x4E1A
        mov    dword ptr [0x4e1e], 0x30f0    ; -> obj1:0x4E1E
        mov    dword ptr [0x4e22], 0x2680    ; -> obj1:0x4E22
        mov    dword ptr [0x4e26], 0x2f10    ; -> obj1:0x4E26
        mov    dword ptr [0x4e2a], 0x3000    ; -> obj1:0x4E2A
        mov    dword ptr [0x4e2e], 0x1c90    ; -> obj1:0x4E2E
        mov    dword ptr [0x4e32], 0x3030    ; -> obj1:0x4E32
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x4e36], eax       ; -> obj1:0x4E36
        mov    dword ptr [0x4e3a], ecx       ; -> obj1:0x4E3A
        push   0x36
        push   0x4e08                        ; -> obj1:0x4E08
        push   0x4e00                        ; -> obj1:0x4E00
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x4e00                        ; -> obj1:0x4E00
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1C82
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1C82:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x3c], 0xa
        mov    word ptr [ebp - 0x3a], 0xa
        mov    dword ptr [ebp - 0x38], 0x4f50 ; -> obj1:0x4F50
        mov    word ptr [ebp - 0x34], 4
        mov    word ptr [ebp - 0x32], 4
        mov    dword ptr [ebp - 0x30], 0x4f60 ; -> obj1:0x4F60
        mov    word ptr [ebp - 0x2c], 0x15
        sub    esi, esi
        mov    word ptr [ebp - 0x2a], 0x15
        mov    dword ptr [ebp - 0x28], 0x4f7c ; -> loc_1_4F7C
        mov    word ptr [ebp - 0x44], 0xe
        mov    word ptr [ebp - 0x42], 0xe
        sub    bl, bl
        mov    dword ptr [ebp - 0x40], 0x4fd4 ; -> loc_1_4FD4
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 0x18], 0x10
        mov    eax, dword ptr [0x4dfc]       ; -> obj1:0x4DFC
        push   eax
        lea    eax, [ebp - 0xc]
        push   esi
        push   0x76c
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        cmp    eax, esi
        je     loc_1_1D19
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1D19:
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, eax
        mov    ecx, 0x1db
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1D5A
        push   0
        push   0x76c
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_04DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1D5A:
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1D7D
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, byte ptr [eax + 4]
loc_1_1D7D:
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1DAB
        mov    byte ptr [ebp - 1], 1
        mov    esi, 0xc1020001
        mov    edi, dword ptr [ebp - 0x24]
        jmp    loc_1_1E47
loc_1_1DAB:
        mov    eax, dword ptr [ebp - 0x10]
        push   1
        mov    edi, dword ptr [eax + 4]
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1DD9
        mov    byte ptr [ebp - 1], 1
        mov    esi, 0xc1020002
        jmp    loc_1_1E47
loc_1_1DD9:
        mov    eax, dword ptr [ebp - 0x10]
        push   0
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 2], al
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1E08
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x18], ecx
loc_1_1E08:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1E47
        cmp    dword ptr [ebp - 0x20], 6
        jne    loc_1_1E47
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, edx
        add    ecx, 0x6db
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1E47:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0560
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xbd], bl
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0xb4], edi
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx + 0xb8], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0xb9], ecx
        push   esi
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1EC0
        or     eax, eax
        je     loc_1_1EB2
        push   0
        push   0x76c
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_04DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1EB2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x4dfc]       ; -> obj1:0x4DFC
        push   eax
        lea    eax, [ebp - 0xc]
        push   0
        push   0x26
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_2260
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xa], 1
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [edi + 0xb4]
        mov    dword ptr [ecx + 0x1a], eax
        mov    eax, dword ptr [ebp - 0xc]
        lea    esi, [edi + 4]
        mov    dword ptr [eax + 0x1e], 3
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ecx + 0x22], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x16], 1
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        lea    ebx, [edi + 0x20]
        push   edi
        mov    eax, dword ptr [0x4e00]       ; -> obj1:0x4E00
        push   eax
        push   esi
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebx], eax
        push   0
        push   0x26
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_04DC
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2260
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_1F85
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
        jmp    loc_1_2250
loc_1_1F85:
        mov    dword ptr [edi + 0x73d], 0x3b20 ; -> loc_1_3B20
        mov    dword ptr [edi + 0x731], 0
        mov    eax, dword ptr [edi + 0x73d]
        mov    dword ptr [edi + 0x735], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [edi + 0x739], edi
        push   eax
        push   ebx
        call   sub_1_3890
        or     al, al
        je     loc_1_2044
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 2
        je     loc_1_1FE2
        cmp    eax, 3
        je     loc_1_1FF0
        cmp    eax, 4
        je     loc_1_2000
        cmp    eax, 5
        je     loc_1_2010
        cmp    eax, 6
        je     loc_1_2020
        mov    dword ptr [ebp - 8], 0xc1020008
        jmp    loc_1_2027
loc_1_1FE2:
        mov    dword ptr [ebp - 8], 0xc1020003
        jmp    loc_1_2027
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FF0:
        mov    dword ptr [ebp - 8], 0xc1020006
        jmp    loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2000:
        mov    dword ptr [ebp - 8], 0xc1020005
        jmp    loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
        mov    dword ptr [ebp - 8], 0xc1020004
        jmp    loc_1_2027
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2020:
        mov    dword ptr [ebp - 8], 0xc1020007
loc_1_2027:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2250
loc_1_2044:
        cmp    byte ptr [edi + 0xbd], 0
        je     loc_1_20C3
        mov    eax, dword ptr [edi + 0x2f]
        test   al, 8
        je     loc_1_2060
        mov    eax, 0x41020009
        jmp    loc_1_20B5
        db  090h
loc_1_2060:
        test   al, 4
        je     loc_1_206B
        mov    eax, 0x4102000a
        jmp    loc_1_20B5
loc_1_206B:
        test   ah, 4
        je     loc_1_2077
        mov    eax, 0x4102000b
        jmp    loc_1_20B5
loc_1_2077:
        test   ah, 8
        je     loc_1_2083
        mov    eax, 0x4102000c
        jmp    loc_1_20B5
loc_1_2083:
        test   ah, 1
        je     loc_1_2090
        mov    eax, 0x4102000d
        jmp    loc_1_20B5
        db  090h
loc_1_2090:
        test   ah, 2
        je     loc_1_209C
        mov    eax, 0x4102000e
        jmp    loc_1_20B5
loc_1_209C:
        test   al, 2
        je     loc_1_20A7
        mov    eax, 0x4102000f
        jmp    loc_1_20B5
loc_1_20A7:
        test   al, 1
        mov    eax, 0x41020010
        jne    loc_1_20B5
        mov    eax, 0x41020011
loc_1_20B5:
        push   0
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
loc_1_20C3:
        mov    byte ptr [edi + 0xd0], 0
        mov    byte ptr [edi + 0x730], 0
        mov    dword ptr [edi + 0xc6], 0
        mov    byte ptr [edi + 0x769], 0
        push   edi
        call   sub_1_06F0
        lea    eax, [edi + 0x708]
        push   eax
        push   edi
        lea    eax, [edi + 0x6db]
        push   eax
        push   0x3650                        ; -> loc_1_3650
        push   0x35d0                        ; -> loc_1_35D0
        push   0x3550                        ; -> loc_1_3550
        mov    eax, dword ptr [edi + 0xb9]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_1_2140
        push   3
        push   2
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2250
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2140:
        mov    dword ptr [edi + 0x9c], 0x4e00 ; -> obj1:0x4E00
        mov    eax, dword ptr [0x4e3e]       ; -> obj1:0x4E3E
        mov    dword ptr [edi + 0xa0], eax
        mov    dword ptr [0x4e3e], edi       ; -> obj1:0x4E3E
        push   edi
        call   sub_1_07B0
        or     al, al
        jne    loc_1_2184
        push   1
        push   2
        push   2
        push   0xaa000004
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_21F4
loc_1_2184:
        push   edi
        call   sub_1_08C0
        or     al, al
        je     loc_1_21B0
        push   0
        push   0xc1020007
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_21F4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_21B0:
        movsx  eax, byte ptr [edi + 0xb8]
        push   1
        push   0
        push   eax
        push   eax
        push   0xe90                         ; -> loc_1_0E90
        push   edi
        push   0xe70                         ; -> loc_1_0E70
        mov    eax, dword ptr [esi]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04BE
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2270
        push   0
        push   0xaa000005
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
loc_1_21F4:
        cmp    dword ptr [0x4e3e], edi       ; -> obj1:0x4E3E
        jne    loc_1_2210
        mov    eax, dword ptr [edi + 0xa0]
        mov    dword ptr [0x4e3e], eax       ; -> obj1:0x4E3E
        jmp    loc_1_2240
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2210:
        mov    ecx, dword ptr [0x4e3e]       ; -> obj1:0x4E3E
        cmp    dword ptr [ecx + 0xa0], edi
        je     loc_1_222E
        mov    edi, edi
loc_1_2220:
        mov    ecx, dword ptr [ecx + 0xa0]
        cmp    dword ptr [ecx + 0xa0], edi
        jne    loc_1_2220
loc_1_222E:
        mov    eax, dword ptr [ecx + 0xa0]
        mov    eax, dword ptr [eax + 0xa0]
        mov    dword ptr [ecx + 0xa0], eax
loc_1_2240:
        push   ebx
        call   dword ptr [edi + 0x5c]
        mov    eax, dword ptr [edi + 0x708]
        push   eax
        call   sub_2_0120
loc_1_2250:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0438
loc_1_2260:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_1_2270:
        mov    dword ptr [edi + 0x741], 0
        mov    dword ptr [edi + 0x745], 0xe90 ; -> loc_1_0E90
        mov    dword ptr [edi + 0x749], edi
        mov    dword ptr [edi + 0x74d], 0
        lea    eax, [edi + 0x75d]
        mov    dword ptr [edi + 0x751], 0xe90 ; -> loc_1_0E90
        mov    dword ptr [edi + 0x755], edi
        mov    dword ptr [edi + 0x759], 0x1b00 ; -> loc_1_1B00
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [edi + 0x759]
        mov    dword ptr [edi + 0x761], ecx
        mov    dword ptr [edi + 0x765], edi
        push   0x7d0
        push   eax
        call   sub_1_04B2
        mov    byte ptr [edi + 0x76a], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_22F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0xffffffff
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x18]
        lea    eax, [eax + edx*4]
loc_1_2313:
        or     edx, edx
        je     loc_1_2322
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_2313
        mov    dword ptr [ecx], edx
loc_1_2322:
        cmp    dword ptr [ecx], -1
        jne    loc_1_2334
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2334:
        mov    eax, dword ptr [0x4dfc]       ; -> obj1:0x4DFC
        push   eax
        push   0
        push   0x71
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D6
        or     eax, eax
        je     loc_1_2372
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2372:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0xac]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0xa4], eax
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_2486
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_2_0170
        or     al, al
        je     loc_1_2486
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x9c]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xee
        mov    dword ptr [esi + 0x714], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_2450
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_2437
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_2437
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_2450
loc_1_2437:
        push   1
        add    esi, 0x731
        push   esi
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_1_2450:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_247C
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_247C
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_247C
        lea    edi, [esi + 0x20]
        push   edi
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_247C:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2486:
        dec    dword ptr [esi + 0xac]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xa4], eax
        push   0x71
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_24D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0xa4]
        cmp    esi, edx
        je     loc_1_24F3
loc_1_24E3:
        mov    eax, dword ptr [esi + 0x18]
        cmp    edx, eax
        jae    loc_1_24EC
        mov    edx, eax
loc_1_24EC:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_24E3
loc_1_24F3:
        or     edx, edx
        jne    loc_1_24FC
        mov    edx, 0xee
loc_1_24FC:
        mov    dword ptr [ecx + 0x714], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2510:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_2533
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2533:
        inc    dword ptr [esi + 0xac]
        inc    dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [esi + 0xa4]
        cmp    ecx, ebx
        jne    loc_1_2551
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0xa4], eax
        jmp    loc_1_2567
loc_1_2551:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_255E
loc_1_2556:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_2556
loc_1_255E:
        lea    eax, [ebx + 0xc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_2567:
        push   0
        mov    eax, dword ptr [ebx + 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_2_0240
        mov    edi, eax
        or     edi, edi
        jne    loc_1_25A0
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_2596
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_25C6
loc_1_2596:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_25C6
        db  08Bh, 0FFh
loc_1_25A0:
        cmp    edi, 0x103
        jne    loc_1_25B2
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_25C6
loc_1_25B2:
        cmp    edi, 0xc001000e
        jne    loc_1_25C3
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_25C3:
        dec    dword ptr [ebx + 0x1c]
loc_1_25C6:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_25F0
        mov    eax, dword ptr [esi + 0x714]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_25DD
        push   esi
        call   sub_1_24D0
loc_1_25DD:
        push   0
        push   0x71
        push   ebx
        call   sub_1_04DC
        jmp    loc_1_2604
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_25F0:
        mov    eax, dword ptr [esi + 0xa8]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0xa8], ebx
        mov    edi, 0x103
loc_1_2604:
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_2647
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_262E
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_262E
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_2647
loc_1_262E:
        push   1
        add    esi, 0x731
        push   esi
        call   sub_1_04B2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2647:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_2673
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_2673
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_2673
        lea    ebx, [esi + 0x20]
        push   ebx
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   ebx
        call   dword ptr [esi + 0x58]
loc_1_2673:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2680:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [edi + 0xac]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_26A2
        mov    esi, 0xc0010002
        jmp    loc_1_26E5
loc_1_26A2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        jne    loc_1_26B8
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    loc_1_26E5
loc_1_26B8:
        cmp    ecx, 1
        jne    loc_1_26E0
        cmp    byte ptr [edi + 0x6ff], 0
        je     loc_1_26CD
        mov    esi, 0xc001000d
        jmp    loc_1_26E5
loc_1_26CD:
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B80
        mov    esi, eax
        jmp    loc_1_26E5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_26E0:
        mov    esi, 0x10001
loc_1_26E5:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0xac]
        dec    eax
        mov    dword ptr [edi + 0xac], eax
        jne    loc_1_2730
        cmp    byte ptr [edi + 0x6ff], 0
        jne    loc_1_2712
        cmp    dword ptr [edi + 0x724], 0
        jne    loc_1_2712
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_1_2730
loc_1_2712:
        push   1
        add    edi, 0x731
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2730:
        cmp    dword ptr [edi + 0xbe], 0
        je     loc_1_275C
        cmp    dword ptr [edi + 0xc6], 0
        jne    loc_1_275C
        cmp    byte ptr [edi + 0x730], 0
        jne    loc_1_275C
        lea    ebx, [edi + 0x20]
        push   ebx
        call   dword ptr [edi + 0x5c]
        push   edi
        call   sub_1_1990
        push   ebx
        call   dword ptr [edi + 0x58]
loc_1_275C:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2770:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        lea    esi, [ebp - 8]
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, 4
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010101
        ja     loc_1_27C0
        je     loc_1_2970
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_27E1
        shl    eax, 2
        jmp    dword ptr [eax + 0x2adc]      ; -> obj1:0x2ADC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27C0:
        cmp    eax, 0x1010102
        je     loc_1_2992
        cmp    eax, 0x1010103
        je     loc_1_29B4
        cmp    eax, 0x1010104
        je     loc_1_2A86
loc_1_27E1:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2A92
        db  08Dh, 049h, 000h
loc_1_27F0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2805
        mov    esi, 0x4ec8                   ; -> loc_1_4EC8
        mov    edi, 0x5c
        jmp    loc_1_2A92
loc_1_2805:
        mov    esi, 0x4e4c                   ; -> loc_1_4E4C
        mov    edi, 0x7c
        jmp    loc_1_2A92
loc_1_2814:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x6ff], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_1_282E
        mov    dword ptr [ebp - 0x14], 0
loc_1_282E:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2A92
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2A92
        db  08Dh, 049h, 000h
loc_1_2850:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2860:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2870:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_2880:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0xc6], 1
        sbb    eax, eax
        and    eax, 0x5ee
        jmp    loc_1_2A8F
loc_1_2896:
        mov    dword ptr [ebp - 8], 0x2000
        jmp    loc_1_2A92
loc_1_28A2:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_2A92
        db  08Bh, 0FFh
loc_1_28B0:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2A92
        db  08Dh, 064h, 024h, 000h
loc_1_28C0:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x6e1
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    loc_1_2A92
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_28F0:
        mov    esi, 0x4fe4                   ; -> loc_1_4FE4
        mov    edi, 0x23
        jmp    loc_1_2A92
        db  090h
loc_1_2900:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2917
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x708]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_2A8F
loc_1_2917:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_1_2A8F
loc_1_2925:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2939
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x714]
        jmp    loc_1_2A8F
loc_1_2939:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        jmp    loc_1_2A8F
loc_1_2944:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_1_2A92
loc_1_2957:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2A92
loc_1_2963:
        mov    dword ptr [ebp - 8], 7
        jmp    loc_1_2A92
        db  090h
loc_1_2970:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x6e1
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2A92
loc_1_2992:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x6db
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2A92
loc_1_29B4:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_2A21
        mov    eax, dword ptr [ebx + 0x708]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_29E9
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2A92
loc_1_29E9:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebx + 0x708]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_2A92
        jmp    loc_1_2A92
loc_1_2A21:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x708]
        push   eax
        call   sub_2_0660
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2A54
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2A92
loc_1_2A54:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x708]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_2A92
loc_1_2A86:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xb9]
loc_1_2A8F:
        mov    dword ptr [ebp - 8], eax
loc_1_2A92:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2ACE
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_2ACE
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_2ABE
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_2ABE:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05BC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_2ACE:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        dd  loc_1_27F0               ; fixup
        dd  loc_1_2814               ; fixup
        dd  loc_1_2840               ; fixup
        dd  loc_1_2840               ; fixup
        dd  loc_1_2850               ; fixup
        dd  loc_1_2860               ; fixup
        dd  loc_1_2870               ; fixup
        dd  loc_1_2880               ; fixup
        dd  loc_1_2896               ; fixup
        dd  loc_1_28A2               ; fixup
        dd  loc_1_28B0               ; fixup
        dd  loc_1_28C0               ; fixup
        dd  loc_1_28F0               ; fixup
        dd  loc_1_2900               ; fixup
        dd  loc_1_2925               ; fixup
        dd  loc_1_2944               ; fixup
        dd  loc_1_2957               ; fixup
        dd  loc_1_27E1               ; fixup
        dd  loc_1_2963               ; fixup
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2770
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2B80:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010e
        je     loc_1_2BD7
        cmp    eax, 0x1010f
        je     loc_1_2C38
        cmp    eax, 0x10112
        je     loc_1_2CC0
        cmp    eax, 0x1010103
        je     loc_1_2CC4
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D05
loc_1_2BD7:
        cmp    edi, 4
        je     loc_1_2BF4
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 4
        jmp    loc_1_2D05
loc_1_2BF4:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_2C21
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D05
loc_1_2C21:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2D20
        jmp    loc_1_2D03
loc_1_2C38:
        cmp    edi, 4
        je     loc_1_2C55
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D05
loc_1_2C55:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_2CB8
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x714]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_2C90
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x714], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_2D05
        db  08Dh, 064h, 024h, 000h
loc_1_2C90:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_1_2CB0
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_2CB0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        push   edx
        call   sub_1_24D0
        jmp    loc_1_2D05
        db  08Bh, 0FFh
loc_1_2CB0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_2D05
loc_1_2CB8:
        mov    ebx, 0xc0010014
        jmp    loc_1_2D05
        db  090h
loc_1_2CC0:
        sub    ebx, ebx
        jmp    loc_1_2D05
loc_1_2CC4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_1_2CE8
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D05
loc_1_2CE8:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2D80
loc_1_2D03:
        mov    ebx, eax
loc_1_2D05:
        or     ebx, ebx
        jne    loc_1_2D13
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_2D13:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2D20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6ff], 0
        jne    loc_1_2D6A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_2D60
        inc    dword ptr [esi + 0x1c]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x708]
        push   eax
        call   sub_2_05E0
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_1_2D60:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_2D6A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6ff], 0
        jne    loc_1_2DD0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_2DC2
        inc    dword ptr [esi + 0x1c]
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x708]
        push   eax
        call   sub_2_0300
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_2DC2:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_1_2DD0:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_2770
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_1_2EF6
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2E66
        je     loc_1_2EB0
        cmp    eax, 0x20101
        je     loc_1_2E82
        cmp    eax, 0x20102
        je     loc_1_2E90
        cmp    eax, 0x20103
        je     loc_1_2E98
        cmp    eax, 0x20104
        je     loc_1_2EA0
        mov    edi, 0xc0010017
        jmp    loc_1_2ED9
loc_1_2E66:
        cmp    eax, 0x1020101
        je     loc_1_2EB8
        cmp    eax, 0x1020102
        je     loc_1_2EC1
        cmp    eax, 0x1020103
        je     loc_1_2ED0
        mov    edi, 0xc0010017
        jmp    loc_1_2ED9
loc_1_2E82:
        mov    eax, dword ptr [ebx + 0x6e7]
        jmp    loc_1_2ED6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2E90:
        mov    eax, dword ptr [ebx + 0x6eb]
        jmp    loc_1_2ED6
loc_1_2E98:
        mov    eax, dword ptr [ebx + 0x6ef]
        jmp    loc_1_2ED6
loc_1_2EA0:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2ED9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EB0:
        mov    eax, dword ptr [ebx + 0x6fb]
        jmp    loc_1_2ED6
loc_1_2EB8:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2ED9
loc_1_2EC1:
        mov    eax, dword ptr [ebx + 0x6f3]
        jmp    loc_1_2ED6
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2ED0:
        mov    eax, dword ptr [ebx + 0x6f7]
loc_1_2ED6:
        mov    dword ptr [ebp - 4], eax
loc_1_2ED9:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_2EF6
        or     edi, edi
        jne    loc_1_2EF6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05BC
        add    dword ptr [ebp - 8], 4
loc_1_2EF6:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2F43
        cmp    eax, 0x20101
        jae    loc_1_2F64
        cmp    eax, 0x10101
        jb     loc_1_2F5F
        cmp    eax, 0x10111
        jbe    loc_1_2F64
        cmp    eax, 0x10113
        je     loc_1_2F64
        jmp    loc_1_2F5F
loc_1_2F43:
        cmp    eax, 0x1010101
        jb     loc_1_2F5F
        cmp    eax, 0x1010104
        jbe    loc_1_2F64
        cmp    eax, 0x1020101
        jb     loc_1_2F5F
        cmp    eax, 0x1020103
        jbe    loc_1_2F64
loc_1_2F5F:
        mov    esi, 0xc0010017
loc_1_2F64:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0xac]
        or     esi, esi
        jne    loc_1_2F7A
        push   ecx
        push   edi
        call   sub_1_2DE0
        mov    esi, eax
loc_1_2F7A:
        mov    eax, dword ptr [edi + 0xac]
        dec    eax
        mov    dword ptr [edi + 0xac], eax
        jne    loc_1_2FC0
        cmp    byte ptr [edi + 0x6ff], 0
        jne    loc_1_2FA4
        cmp    dword ptr [edi + 0x724], 0
        jne    loc_1_2FA4
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_1_2FC0
loc_1_2FA4:
        push   1
        add    edi, 0x731
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_2FC0:
        cmp    dword ptr [edi + 0xbe], 0
        je     loc_1_2FEC
        cmp    dword ptr [edi + 0xc6], 0
        jne    loc_1_2FEC
        cmp    byte ptr [edi + 0x730], 0
        jne    loc_1_2FEC
        lea    ebx, [edi + 0x20]
        push   ebx
        call   dword ptr [edi + 0x5c]
        push   edi
        call   sub_1_1990
        push   ebx
        call   dword ptr [edi + 0x58]
loc_1_2FEC:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x4e00]       ; -> obj1:0x4E00
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [0x4e04]       ; -> obj1:0x4E04
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3030:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x76a], 1
        cmp    dword ptr [0x4e3e], esi       ; -> obj1:0x4E3E
        jne    loc_1_3057
        mov    eax, dword ptr [esi + 0xa0]
        mov    dword ptr [0x4e3e], eax       ; -> obj1:0x4E3E
        jmp    loc_1_3085
loc_1_3057:
        mov    ecx, dword ptr [0x4e3e]       ; -> obj1:0x4E3E
        cmp    dword ptr [ecx + 0xa0], esi
        je     loc_1_3073
loc_1_3065:
        mov    ecx, dword ptr [ecx + 0xa0]
        cmp    dword ptr [ecx + 0xa0], esi
        jne    loc_1_3065
loc_1_3073:
        mov    eax, dword ptr [ecx + 0xa0]
        mov    eax, dword ptr [eax + 0xa0]
        mov    dword ptr [ecx + 0xa0], eax
loc_1_3085:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x75d]
        push   eax
        call   sub_1_04B8
        mov    edi, 0x61a8
        lea    ebx, [ebx]
loc_1_30A0:
        push   0x64
        call   sub_1_04D0
        dec    edi
        jne    loc_1_30A0
        mov    eax, dword ptr [esi + 0x708]
        push   eax
        call   sub_2_0120
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_04C4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
        push   0
        push   0x76c
        push   esi
        call   sub_1_04DC
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_30F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_3113
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3113:
        inc    dword ptr [esi + 0x1c]
        inc    dword ptr [edi + 0xac]
        cmp    byte ptr [edi + 0x6ff], 0
        je     loc_1_3140
        dec    dword ptr [esi + 0x1c]
        dec    dword ptr [edi + 0xac]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_3140:
        mov    ebx, dword ptr [edi + 0xa4]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_1_3173
loc_1_3151:
        inc    dword ptr [ebx + 0x1c]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010004
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 0xc]
        dec    dword ptr [ebx + 0x1c]
        mov    ebx, eax
        or     eax, eax
        jne    loc_1_3151
loc_1_3173:
        mov    byte ptr [edi + 0x6ff], 1
        mov    dword ptr [edi + 0x700], 0
        mov    dword ptr [edi + 0x704], esi
        push   edi
        call   sub_1_3220
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 0x1c]
        mov    eax, dword ptr [edi + 0xac]
        dec    eax
        mov    dword ptr [edi + 0xac], eax
        jne    loc_1_31E0
        cmp    byte ptr [edi + 0x6ff], 0
        jne    loc_1_31C0
        cmp    dword ptr [edi + 0x724], 0
        jne    loc_1_31C0
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_1_31E0
loc_1_31C0:
        push   1
        add    edi, 0x731
        push   edi
        call   sub_1_04B2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_31E0:
        cmp    dword ptr [edi + 0xbe], 0
        je     loc_1_320C
        cmp    dword ptr [edi + 0xc6], 0
        jne    loc_1_320C
        cmp    byte ptr [edi + 0x730], 0
        jne    loc_1_320C
        lea    esi, [edi + 0x20]
        push   esi
        call   dword ptr [edi + 0x5c]
        push   edi
        call   sub_1_1990
        push   esi
        call   dword ptr [edi + 0x58]
loc_1_320C:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3220:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0xac]
        cmp    dword ptr [esi + 0x724], 0
        je     loc_1_326F
loc_1_3237:
        mov    ecx, dword ptr [esi + 0x724]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x724], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_1_3263
        cmp    eax, 8
        je     loc_1_3263
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_3263:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_3237
loc_1_326F:
        cmp    dword ptr [esi + 0xc6], 0
        je     loc_1_3293
        mov    dword ptr [esi + 0x700], 2
        dec    dword ptr [esi + 0xac]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3293:
        dec    dword ptr [esi + 0xac]
        push   esi
        call   sub_1_32B0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xac]
        inc    ecx
        mov    dword ptr [eax + 0xac], ecx
        cmp    dword ptr [eax + 0xc6], 0
        je     loc_1_32E5
        mov    dword ptr [eax + 0x700], 3
        dec    dword ptr [eax + 0xac]
        mov    eax, 0x103
        pop    ebp
        ret    4
loc_1_32E5:
        dec    ecx
        mov    dword ptr [eax + 0xac], ecx
        push   eax
        call   sub_1_3300
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3300:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0xac]
        mov    ecx, dword ptr [esi + 0xc6]
        or     ecx, ecx
        je     loc_1_333A
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0xc6], 0
loc_1_333A:
        mov    edx, dword ptr [esi + 0xbe]
        or     edx, edx
        je     loc_1_336E
loc_1_3344:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0xbe], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0xbe]
        or     edx, edx
        jne    loc_1_3344
loc_1_336E:
        mov    edx, dword ptr [esi + 0x718]
        or     edx, edx
        je     loc_1_33A2
loc_1_3378:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x718], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x718]
        or     edx, edx
        jne    loc_1_3378
loc_1_33A2:
        cmp    byte ptr [esi + 0x730], 0
        je     loc_1_33C3
loc_1_33AB:
        mov    edi, 0x64
loc_1_33B0:
        push   0x64
        call   sub_1_04D0
        dec    edi
        jne    loc_1_33B0
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_33AB
loc_1_33C3:
        push   esi
        call   sub_1_09F0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x6ff], 0
        mov    edi, dword ptr [esi + 0xa4]
        or     edi, edi
        je     loc_1_3426
        sub    ebx, ebx
loc_1_33E8:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_33FF
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_33FF:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0474
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        cmp    eax, ebx
        jne    loc_1_33E8
loc_1_3426:
        dec    dword ptr [esi + 0xac]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3440:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x700], eax
        jne    loc_1_3543
        cmp    eax, 1
        je     loc_1_34D0
        cmp    eax, 2
        je     loc_1_34D8
        cmp    eax, 3
        je     loc_1_34E0
        mov    eax, dword ptr [ebp - 4]
loc_1_346C:
        cmp    eax, 0x103
        je     loc_1_348B
        push   eax
        mov    eax, dword ptr [esi + 0x704]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 0x704]
        dec    dword ptr [eax + 0x1c]
loc_1_348B:
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_3517
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_34B5
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_34B5
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_3517
loc_1_34B5:
        push   1
        add    esi, 0x731
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_34D0:
        push   esi
        call   sub_1_3220
        jmp    loc_1_346C
loc_1_34D8:
        push   esi
        call   sub_1_32B0
        jmp    loc_1_346C
loc_1_34E0:
        mov    eax, dword ptr [esi + 0xac]
        inc    eax
        mov    dword ptr [esi + 0xac], eax
        cmp    dword ptr [esi + 0xc6], 0
        je     loc_1_3505
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3505:
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        push   esi
        call   sub_1_3300
        jmp    loc_1_346C
loc_1_3517:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_3543
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_3543
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_3543
        lea    edi, [esi + 0x20]
        push   edi
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_3543:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3550:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_3563
        lea    edx, [eax + 0x49]
loc_1_3563:
        cmp    byte ptr [ecx + 0x6ff], 0
        je     loc_1_3575
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_3575:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x724], 0
        jne    loc_1_35B0
        mov    dword ptr [ecx + 0x728], edx
        mov    dword ptr [ecx + 0x724], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_35B0:
        mov    eax, dword ptr [ecx + 0x728]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x728], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_35E3
        lea    edx, [eax + 0x21]
loc_1_35E3:
        cmp    byte ptr [ecx + 0x6ff], 0
        je     loc_1_35F5
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_35F5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x724], 0
        jne    loc_1_3630
        mov    dword ptr [ecx + 0x728], edx
        mov    dword ptr [ecx + 0x724], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_3630:
        mov    eax, dword ptr [ecx + 0x728]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x728], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3650:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x1c]
        ret    4
        db  08Dh, 049h, 000h
sub_1_3660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0xa], 1
        call   sub_1_4210
        mov    al, 1
        cmp    dword ptr [esi + 4], 0
        je     loc_1_367B
        mov    al, 3
loc_1_367B:
        push   eax
        push   0x10
        push   esi
        call   dword ptr [esi + 0x50]
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3690:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0xa], 0
        call   sub_1_4240
        sub    al, al
        cmp    dword ptr [esi + 4], 0
        je     loc_1_36AB
        mov    al, 2
loc_1_36AB:
        push   eax
        push   0x10
        push   esi
        call   dword ptr [esi + 0x50]
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_36C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3690
        push   esi
        call   sub_1_3660
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, 1
        cmp    dword ptr [eax + 4], 0
        je     loc_1_36F0
        mov    cl, 3
loc_1_36F0:
        push   ecx
        push   0x10
        push   eax
        call   dword ptr [eax + 0x50]
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_3700:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3710:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3720:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x36]
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
        test   byte ptr [esi + 0xf], 6
        je     loc_1_3749
        push   0
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    al, 1
        test   byte ptr [esi + 0x33], al
        je     loc_1_3744
        mov    al, 5
loc_1_3744:
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
loc_1_3749:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3750:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 0x37]
        push   ecx
        push   eax
        call   dword ptr [eax + 0x4c]
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3770:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xf], 6
        je     loc_1_3829
        mov    bl, byte ptr [esi + 9]
        mov    cl, byte ptr [esi + 0x33]
        and    cl, bl
        or     cl, 8
        or     cl, byte ptr [esi + 8]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    al, byte ptr [esi + 0x34]
        and    al, bl
        mov    byte ptr [esi + 9], al
        mov    edi, 1
loc_1_37A9:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    edi
        jne    loc_1_37A9
        and    cl, byte ptr [esi + 0x34]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_37C0:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_37C0
        mov    dx, word ptr [esi]
        mov    al, 0x58
        out    dx, al
        mov    ecx, 1
loc_1_37D2:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_37D2
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_37E7:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_37E7
        push   0
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    al, 2
        test   byte ptr [esi + 0x33], 1
        je     loc_1_37FE
        mov    al, 6
loc_1_37FE:
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    edi, dword ptr [esi]
        add    edi, 2
        mov    cl, byte ptr [esi + 8]
        or     cl, byte ptr [esi + 0x33]
        mov    dx, di
        mov    al, cl
        out    dx, al
        mov    ecx, 3
loc_1_3819:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_3819
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
loc_1_3829:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3840:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xf], 0xc
        jne    loc_1_3886
        mov    cl, byte ptr [esi + 8]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    bl, byte ptr [esi + 0x34]
        and    bl, cl
        mov    edi, dword ptr [esi]
        mov    byte ptr [esi + 8], bl
        lea    ecx, [edi + 2]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    dx, di
        mov    al, 0x18
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [esi + 0x33]
        or     al, bl
        out    dx, al
        mov    al, bl
        out    dx, al
loc_1_3886:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0x36], 0xe8
        mov    byte ptr [esi + 0x37], 0xc8
        call   sub_1_41A0
        or     eax, eax
        jne    loc_1_38C2
        mov    eax, dword ptr [ebp + 0xc]
        pop    edi
        mov    dword ptr [eax], 2
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_38C2:
        cmp    eax, 0x113
        je     loc_1_38E0
        mov    eax, dword ptr [ebp + 0xc]
        pop    edi
        mov    dword ptr [eax], 3
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_38E0:
        mov    eax, dword ptr [esi + 0xb]
        test   ah, 0x40
        je     loc_1_38ED
        test   ah, 0x80
        je     loc_1_38F5
loc_1_38ED:
        mov    byte ptr [esi + 0x33], 2
        mov    byte ptr [esi + 0x34], 0xfd
loc_1_38F5:
        sub    ebx, ebx
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [ebx]
loc_1_3900:
        mov    edx, dword ptr [esi + 0xb]
        mov    edi, 0x80
        mov    cl, bl
        sar    edi, cl
        test   edx, edi
        je     loc_1_3927
        mov    eax, dword ptr [esi + 0xf]
        and    eax, edx
        push   eax
        push   esi
        call   sub_1_5008
        and    eax, 0xff
        or     eax, eax
        je     loc_1_3945
loc_1_3927:
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_3900
        or     eax, eax
        je     loc_1_3945
        mov    eax, dword ptr [ebp + 0xc]
        pop    edi
        mov    dword ptr [eax], 5
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3945:
        mov    dword ptr [esi + 0x38], 0x3660 ; -> sub_1_3660
        mov    dword ptr [esi + 0x3c], 0x3690 ; -> sub_1_3690
        mov    dword ptr [esi + 0x40], 0x36c0 ; -> loc_1_36C0
        mov    dword ptr [esi + 0x44], 0x3700 ; -> loc_1_3700
        mov    dword ptr [esi + 0x48], 0x3710 ; -> loc_1_3710
        mov    dword ptr [esi + 0x70], 0x3a90 ; -> loc_1_3A90
        mov    dword ptr [esi + 0x6c], 0x3ab0 ; -> loc_1_3AB0
        mov    dword ptr [esi + 0x74], 0x3af0 ; -> loc_1_3AF0
        mov    dword ptr [esi + 0x78], 0x3ad0 ; -> loc_1_3AD0
        push   esi
        call   sub_1_3A30
        or     al, al
        je     loc_1_39A2
        mov    eax, dword ptr [ebp + 0xc]
        pop    edi
        mov    dword ptr [eax], 4
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39A2:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_39B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        add    dx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 1
        out    dx, al
        mov    al, cl
        out    dx, al
        push   0x98
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    al, byte ptr [ebp + 0xc]
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    al, byte ptr [ebp + 0x10]
        test   byte ptr [esi + 0x33], 2
        jne    loc_1_39E7
        or     al, 4
loc_1_39E7:
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    eax, dword ptr [esi + 0xf]
        and    eax, 8
        cmp    eax, 1
        cmc    
        sbb    al, al
        and    al, 3
        push   eax
        push   esi
        call   dword ptr [esi + 0x4c]
        mov    ecx, dword ptr [esi]
        mov    dx, cx
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 2]
        mov    al, byte ptr [esi + 8]
        or     al, byte ptr [esi + 0x33]
        out    dx, al
        mov    ecx, 1
loc_1_3A16:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_3A16
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_3A30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x13]
        push   eax
        push   esi
        call   sub_1_3FA0
        or     ax, ax
        je     loc_1_3A50
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A50:
        cmp    byte ptr [esi + 0x2e], 0
        je     loc_1_3A60
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A60:
        cmp    byte ptr [esi + 0x2f], 0x80
        je     loc_1_3A70
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_3A70:
        cmp    byte ptr [esi + 0x30], 0xc7
        je     loc_1_3A7D
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_3A7D:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   4
        push   0x11
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 0x50]
        or     byte ptr [esi + 0x35], 4
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   0x11
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 0x50]
        mov    byte ptr [esi + 0x35], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3AD0:
        push   ebp
        mov    ebp, esp
        push   0x11
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 0x5c]
        mov    cl, al
        and    cl, 1
        mov    al, cl
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0x35]
        or     bl, byte ptr [ebp + 0xc]
        push   ebx
        push   0x11
        push   esi
        call   dword ptr [esi + 0x50]
        mov    al, bl
        or     al, 2
        push   eax
        push   0x11
        push   esi
        call   dword ptr [esi + 0x50]
        push   ebx
        push   0x11
        push   esi
        call   dword ptr [esi + 0x50]
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_3B20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0xac]
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_3BE4
        cmp    dword ptr [esi + 0x72c], 0
        jne    loc_1_3BE4
        lea    esp, [esp]
loc_1_3B50:
        mov    edi, dword ptr [esi + 0x724]
        or     edi, edi
        je     loc_1_3BBC
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x724], eax
        cmp    dword ptr [esi + 0x728], edi
        jne    loc_1_3B74
        mov    dword ptr [esi + 0x728], 0
loc_1_3B74:
        mov    dword ptr [esi + 0x72c], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_3B90
        cmp    eax, 0xa
        je     loc_1_3B90
        push   esi
        call   sub_1_3CC0
        jmp    loc_1_3B96
        db  090h
loc_1_3B90:
        push   esi
        call   sub_1_3D40
loc_1_3B96:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_3BB0
        cmp    ecx, 4
        je     loc_1_3BB0
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3BB0:
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_3B50
loc_1_3BBC:
        mov    dword ptr [esi + 0x72c], 0
        cmp    byte ptr [esi + 0x6ff], 0
        je     loc_1_3BE4
        dec    dword ptr [esi + 0xac]
        push   1
        push   esi
        call   sub_1_3440
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3BE4:
        mov    edi, dword ptr [esi + 0xa8]
        or     edi, edi
        je     loc_1_3C48
        sub    ebx, ebx
loc_1_3BF0:
        cmp    dword ptr [edi + 0x1c], 0
        jbe    loc_1_3C00
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_3C44
        db  08Dh, 049h, 000h
loc_1_3C00:
        mov    eax, dword ptr [esi + 0x714]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_3C11
        push   esi
        call   sub_1_24D0
loc_1_3C11:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_3C30
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_3C36
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3C30:
        mov    dword ptr [esi + 0xa8], eax
loc_1_3C36:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x71
        push   eax
        call   sub_1_04DC
loc_1_3C44:
        or     edi, edi
        jne    loc_1_3BF0
loc_1_3C48:
        mov    eax, dword ptr [esi + 0xac]
        dec    eax
        mov    dword ptr [esi + 0xac], eax
        jne    loc_1_3C87
        cmp    byte ptr [esi + 0x6ff], 0
        jne    loc_1_3C72
        cmp    dword ptr [esi + 0x724], 0
        jne    loc_1_3C72
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_1_3C87
loc_1_3C72:
        push   1
        add    esi, 0x731
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3C87:
        cmp    dword ptr [esi + 0xbe], 0
        je     loc_1_3CB3
        cmp    dword ptr [esi + 0xc6], 0
        jne    loc_1_3CB3
        cmp    byte ptr [esi + 0x730], 0
        jne    loc_1_3CB3
        lea    edi, [esi + 0x20]
        push   edi
        call   dword ptr [esi + 0x5c]
        push   esi
        call   sub_1_1990
        push   edi
        call   dword ptr [esi + 0x58]
loc_1_3CB3:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x708]
        mov    eax, dword ptr [eax + 0xc]
        test   al, 0x24
        mov    dword ptr [esi + 0xcc], eax
        je     loc_1_3CF0
        push   0xc20                         ; -> loc_1_0C20
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_04CA
        jmp    loc_1_3CF5
        db  005h, 000h, 000h, 000h, 000h
loc_1_3CF0:
        call   sub_1_3D40
loc_1_3CF5:
        mov    eax, dword ptr [esi + 0xcc]
        test   al, 0x20
        je     loc_1_3D08
        mov    byte ptr [esi + 0xd0], 3
        jmp    loc_1_3D27
loc_1_3D08:
        test   al, 6
        je     loc_1_3D15
        mov    byte ptr [esi + 0xd0], 2
        jmp    loc_1_3D27
loc_1_3D15:
        test   al, 9
        mov    byte ptr [esi + 0xd0], 1
        jne    loc_1_3D27
        mov    byte ptr [esi + 0xd0], 0
loc_1_3D27:
        push   esi
        push   0xbf0                         ; -> loc_1_0BF0
        add    esi, 8
        push   esi
        call   sub_1_04CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3D40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0B60
        push   esi
        push   0xc80                         ; -> loc_1_0C80
        add    esi, 8
        push   esi
        call   sub_1_04CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3D70:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx]
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [ecx + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3D90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        or     al, 0x40
        mov    esi, dword ptr [edi]
        mov    dx, si
        out    dx, al
        lea    ecx, [esi + 2]
        mov    bl, byte ptr [edi + 8]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        mov    al, byte ptr [edi + 0x33]
        or     al, bl
        mov    dx, cx
        out    dx, al
        mov    al, byte ptr [edi + 0x34]
        and    al, bl
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        or     esi, esi
        mov    cl, byte ptr [eax + 8]
        je     loc_1_3E1E
        mov    edi, dword ptr [eax]
        mov    al, byte ptr [eax + 0x33]
        mov    byte ptr [ebp - 8], al
        lea    eax, [edi + 2]
        mov    word ptr [ebp - 4], ax
        mov    ebx, dword ptr [ebp + 0xc]
loc_1_3E08:
        mov    dx, di
        mov    al, byte ptr [ebx]
        out    dx, al
        xor    cl, byte ptr [ebp - 8]
        mov    dx, word ptr [ebp - 4]
        mov    al, cl
        out    dx, al
        dec    esi
        inc    ebx
        or     esi, esi
        jne    loc_1_3E08
loc_1_3E1E:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    byte ptr [eax + 8], cl
        sub    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_3E30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x50], 0x3d90 ; -> loc_1_3D90
        mov    dword ptr [eax + 0x54], 0x3de0 ; -> loc_1_3DE0
        mov    dword ptr [eax + 0x4c], 0x3d70 ; -> loc_1_3D70
        ret    4
        db  090h
loc_1_3E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, dword ptr [esi]
        or     al, 0x60
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_3E75:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_3E75
        mov    ecx, dword ptr [esi]
        mov    al, byte ptr [esi + 9]
        lea    edx, [ecx + 2]
        or     al, byte ptr [esi + 0x33]
        out    dx, al
        test   byte ptr [esi + 0xc], 2
        je     loc_1_3E94
        mov    dx, cx
        mov    al, 0xff
        out    dx, al
loc_1_3E94:
        mov    ecx, 1
loc_1_3E99:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_3E99
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 9]
        test   byte ptr [esi + 0xc], 2
        je     loc_1_3EDB
        mov    dx, word ptr [esi]
        mov    al, 0xff
        out    dx, al
loc_1_3EDB:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_3F0E
        mov    dword ptr [ebp - 8], eax
loc_1_3EE8:
        xor    cl, byte ptr [esi + 0x33]
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, cl
        out    dx, al
        mov    ebx, 1
loc_1_3EF8:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        dec    ebx
        jne    loc_1_3EF8
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        dec    dword ptr [ebp - 8]
        jne    loc_1_3EE8
loc_1_3F0E:
        mov    byte ptr [esi + 9], cl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_3F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        mov    bl, byte ptr [ecx + 9]
        je     loc_1_3F50
loc_1_3F36:
        xor    bl, byte ptr [ecx + 0x33]
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, bl
        out    dx, al
        mov    edi, 1
loc_1_3F46:
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        dec    edi
        jne    loc_1_3F46
        dec    esi
        jne    loc_1_3F36
loc_1_3F50:
        mov    byte ptr [ecx + 9], bl
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3F60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [esi + 0x5c], 0x3e50 ; -> loc_1_3E50
        mov    dword ptr [esi + 0x60], 0x3ec0 ; -> loc_1_3EC0
        mov    dword ptr [esi + 0x64], 0x3f20 ; -> loc_1_3F20
        mov    dword ptr [esi + 0xf], 2
        call   sub_1_3E30
        push   2
        push   0
        push   esi
        call   sub_1_39B0
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 0xf
        mov    esi, dword ptr [ebp + 8]
loc_1_3FBF:
        push   esi
        call   sub_1_4110
        mov    eax, dword ptr [ebp - 8]
        or     eax, 0x80
        push   eax
        push   esi
        call   sub_1_4140
        sub    di, di
        mov    ebx, 0x10
loc_1_3FDC:
        push   0
        push   esi
        call   dword ptr [esi + 0x74]
        add    di, di
        push   esi
        call   dword ptr [esi + 0x78]
        and    ax, 0xff
        or     di, ax
        dec    ebx
        jne    loc_1_3FDC
        mov    eax, dword ptr [ebp - 0xc]
        add    word ptr [ebp - 2], di
        mov    word ptr [eax], di
        add    dword ptr [ebp - 0xc], 2
        push   0
        push   esi
        call   dword ptr [esi + 0x74]
        dec    dword ptr [ebp - 8]
        jns    loc_1_3FBF
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_1_4035:
        push   0x30
        push   esi
        call   sub_1_40E0
        push   0x20
        push   esi
        call   sub_1_40E0
        push   esi
        call   sub_1_40B0
        push   esi
        call   sub_1_4110
        push   0x40
        push   esi
        call   sub_1_40E0
        mov    eax, dword ptr [ebp + 0xc]
        xor    ebx, ebx
        mov    bl, byte ptr [eax + edi*2]
        add    dword ptr [ebp - 4], ebx
        cmp    edi, 0xf
        jne    loc_1_406C
        sub    ebx, dword ptr [ebp - 4]
loc_1_406C:
        mov    eax, ebx
        shr    eax, 8
        push   eax
        push   esi
        call   sub_1_4140
        push   ebx
        push   esi
        call   sub_1_4140
        push   esi
        call   sub_1_4180
        push   esi
        call   sub_1_40B0
        inc    edi
        cmp    edi, 0x10
        jl     loc_1_4035
        push   0
        push   esi
        call   sub_1_40E0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x1388
        call   sub_1_04D0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4110
        push   esi
        call   sub_1_4180
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4110
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_1_4140
        push   esi
        call   sub_1_4180
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 0x6c]
        push   0
        push   esi
        call   dword ptr [esi + 0x74]
        push   esi
        call   dword ptr [esi + 0x70]
        push   0
        push   esi
        call   dword ptr [esi + 0x74]
        push   1
        push   esi
        call   dword ptr [esi + 0x74]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [ebp - 1], al
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
loc_1_4153:
        mov    al, byte ptr [ebp - 1]
        mov    cl, 7
        sub    cl, bl
        shr    al, cl
        and    eax, 1
        push   eax
        push   esi
        call   dword ptr [esi + 0x74]
        inc    ebx
        cmp    ebx, 8
        jl     loc_1_4153
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   dword ptr [esi + 0x6c]
        push   0
        push   esi
        call   dword ptr [esi + 0x74]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 4
        mov    byte ptr [edi + 8], 4
        mov    ecx, dword ptr [edi]
        mov    byte ptr [edi + 9], 0x24
        lea    edx, [ecx + 2]
        out    dx, al
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   0x61a8
        call   sub_1_04D0
        mov    dx, word ptr [edi]
        mov    al, 0xff
        out    dx, al
        push   1
        call   sub_1_04D0
        mov    esi, 0x5028                   ; -> loc_1_5028
        cmp    dword ptr [0x5028], 0         ; -> loc_1_5028
        je     loc_1_4207
loc_1_41E3:
        push   edi
        call   dword ptr [esi + 4]
        or     al, al
        je     loc_1_4203
        mov    edx, dword ptr [edi]
        add    edx, 2
        mov    al, 0xc
        add    esi, 0x10
        out    dx, al
        cmp    dword ptr [esi], 0
        jne    loc_1_41E3
        mov    eax, dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4203:
        push   edi
        call   dword ptr [esi + 8]
loc_1_4207:
        mov    eax, dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_4210:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        or     byte ptr [ecx + 8], 0x10
        or     byte ptr [ecx + 9], 0x10
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 8]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4240:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        and    byte ptr [eax + 8], 0xef
        and    byte ptr [eax + 9], 0xef
        mov    edx, dword ptr [eax]
        add    edx, 2
        mov    al, byte ptr [eax + 8]
        out    dx, al
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4260:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4670
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4480
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_45B0
        or     al, al
        je     loc_1_4292
        mov    al, 1
        pop    esi
        pop    ebp
        ret    0xc
loc_1_4292:
        push   0x7f
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4540
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_45B0
        cmp    al, 0x7c
        je     loc_1_42B1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    0xc
loc_1_42B1:
        sub    al, al
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_42C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_43C0
        or     dword ptr [esi + 0xb], eax
        or     dword ptr [esi + 0xb], 1
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_4430
        or     al, al
        jne    loc_1_42E9
        or     dword ptr [esi + 0xb], 0x2000
loc_1_42E9:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_42F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0x24
        lea    ecx, [esi + 2]
        mov    dx, cx
        out    dx, al
        mov    edi, 1
        lea    edx, [esi + 1]
loc_1_430C:
        in     al, dx
        dec    edi
        jne    loc_1_430C
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, cx
        mov    al, 0xc
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4330:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0xe4
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        mov    ecx, 1
        lea    edx, [edi + 1]
loc_1_434C:
        in     al, dx
        dec    ecx
        jne    loc_1_434C
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, si
        mov    al, 0xc
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4370:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, si
        mov    al, 0xff
        out    dx, al
        mov    ecx, 1
        lea    edi, [esi + 1]
loc_1_438A:
        mov    dx, di
        in     al, dx
        dec    ecx
        jne    loc_1_438A
        lea    ecx, [esi + 2]
        mov    dx, cx
        mov    al, 4
        out    dx, al
        mov    ebx, 1
        nop    
loc_1_43A0:
        mov    dx, di
        in     al, dx
        dec    ebx
        jne    loc_1_43A0
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, cx
        mov    al, 0xc
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_43C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_44E0
        push   0x7f
        push   0
        push   edi
        call   sub_1_4540
        mov    dx, di
        mov    al, 1
        out    dx, al
        lea    edx, [edi + 2]
        mov    al, 0xc
        out    dx, al
        push   edi
        call   sub_1_42F0
        cmp    al, 0x7d
        jne    loc_1_43F7
        mov    esi, 2
        jmp    loc_1_441C
loc_1_43F7:
        push   edi
        call   sub_1_4330
        cmp    al, 0x7d
        jne    loc_1_4408
        mov    esi, 0x102
        jmp    loc_1_441C
loc_1_4408:
        push   edi
        call   sub_1_4370
        cmp    al, 0x7d
        mov    esi, 0x202
        je     loc_1_441C
        mov    esi, 1
loc_1_441C:
        push   0
        push   edi
        call   sub_1_4480
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_4430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    bl, 0x20
        mov    esi, dword ptr [ebp + 8]
loc_1_443D:
        push   ebx
        push   esi
        call   sub_1_4480
        cmp    edi, 0x7f
        jge    loc_1_4476
loc_1_4449:
        push   edi
        push   ebx
        push   esi
        call   sub_1_4540
        push   esi
        call   sub_1_45B0
        cmp    al, 0xf8
        je     loc_1_4461
        inc    edi
        cmp    edi, 0x7f
        jl     loc_1_4449
loc_1_4461:
        cmp    edi, 0x7f
        jge    loc_1_4476
        inc    bl
        cmp    bl, 0x2e
        jb     loc_1_443D
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4476:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_1_4480:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0xf
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        push   0x64
        call   sub_1_04D0
        mov    bl, byte ptr [ebp + 0xc]
        mov    cl, bl
        or     cl, 0x10
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   0x64
        call   sub_1_04D0
        mov    dx, si
        mov    al, 4
        out    dx, al
        and    ebx, 0xf
        xor    edx, edx
        mov    dl, byte ptr [ebx + 0x5b54]   ; -> loc_1_5B54
        add    edx, edi
        mov    al, byte ptr [ebx + 0x5b44]   ; -> loc_1_5B44
        out    dx, al
        push   0x64
        call   sub_1_04D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_44E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0xf
        lea    esi, [edi + 2]
        mov    dx, si
        out    dx, al
        push   0x64
        call   sub_1_04D0
        mov    bl, byte ptr [ebp + 0xc]
        mov    cl, bl
        or     cl, 0xd0
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   0x64
        call   sub_1_04D0
        mov    dx, si
        mov    al, 0xc
        out    dx, al
        push   0x64
        call   sub_1_04D0
        and    ebx, 0xf
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 0x5b54]   ; -> loc_1_5B54
        add    ecx, edi
        mov    dx, cx
        mov    al, byte ptr [ebx + 0x5b44]   ; -> loc_1_5B44
        out    dx, al
        push   0x64
        call   sub_1_04D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
sub_1_4540:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    al, byte ptr [ebp + 0xc]
        and    eax, 0xf
        xor    edx, edx
        mov    cl, byte ptr [eax + 0x5b34]   ; -> loc_1_5B34
        mov    bl, byte ptr [eax + 0x5b44]   ; -> loc_1_5B44
        mov    dl, byte ptr [eax + 0x5b54]   ; -> loc_1_5B54
        add    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_4582
        mov    esi, 0x3e
loc_1_456D:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    loc_1_456D
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_4582:
        mov    esi, 0x1f
loc_1_4587:
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    esi
        jne    loc_1_4587
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_45B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [ecx + 1]
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    al, 0x78
        shr    al, 3
        mov    byte ptr [ebp - 2], al
        mov    dx, cx
        mov    al, 0x80
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0xf8
        add    al, al
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        or     al, byte ptr [ebp - 1]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_45F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0xa
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        or     al, 0xb
        out    dx, al
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        mov    al, 0x38
        out    dx, al
        push   0xa
        call   sub_1_04D0
        mov    ecx, dword ptr [esi]
        add    ecx, 2
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   0xa
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        sub    al, al
        out    dx, al
        push   0xa
        call   sub_1_04D0
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_465F
        mov    edi, 4
loc_1_4642:
        mov    dx, word ptr [esi]
        mov    al, 1
        out    dx, al
        push   0xa
        call   sub_1_04D0
        mov    dx, word ptr [esi]
        sub    al, al
        out    dx, al
        push   0xa
        call   sub_1_04D0
        dec    edi
        jne    loc_1_4642
loc_1_465F:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4670:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    edx, dword ptr [esi]
        out    dx, al
        add    edx, 2
        out    dx, al
        push   6
        call   sub_1_04D0
        mov    ecx, dword ptr [esi]
        add    ecx, 2
        mov    dx, cx
        mov    al, byte ptr [esi + 8]
        out    dx, al
        push   0x1f4
        call   sub_1_04D0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_46DE
        mov    edx, dword ptr [esi]
        add    dx, 2
loc_1_46AB:
        mov    eax, dword ptr [ebp + 0xc]
        sub    ecx, ecx
        movzx  edi, byte ptr [eax]
loc_1_46B3:
        mov    eax, edi
        shl    eax, cl
        and    eax, 0x80
        cmp    eax, 1
        sbb    bl, bl
        and    bl, 2
        or     bl, byte ptr [esi + 8]
        mov    al, bl
        out    dx, al
        or     al, 8
        out    dx, al
        mov    al, bl
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jl     loc_1_46B3
        inc    dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0x10]
        jne    loc_1_46AB
loc_1_46DE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_46F0:
        push   ebp
        mov    ebp, esp
        push   0xadc
        push   0x5058                        ; -> loc_1_5058
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4260
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4710:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
        push   0xa5
        push   0x13
        push   esi
        call   sub_1_4A40
        push   0x13
        push   esi
        call   sub_1_4AA0
        sub    al, 0xa5
        cmp    al, 1
        sbb    al, al
        inc    al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4750:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4B20
        push   0
        push   0
        push   esi
        call   sub_1_4A40
        push   esi
        call   sub_1_4780
        or     dword ptr [esi + 0xb], eax
        or     dword ptr [esi + 0xb], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4780:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   2
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
        push   0xa5
        push   0x13
        push   esi
        call   sub_1_4A40
        push   esi
        call   sub_1_47F0
        cmp    al, 0xa5
        jne    loc_1_47B4
        mov    eax, 2
        pop    esi
        pop    ebp
        ret    4
loc_1_47B4:
        push   esi
        call   sub_1_4850
        cmp    al, 0xa5
        jne    loc_1_47C8
        mov    eax, 0x102
        pop    esi
        pop    ebp
        ret    4
loc_1_47C8:
        push   esi
        call   sub_1_48A0
        cmp    al, 0xa5
        jne    loc_1_47E0
        mov    eax, 0x202
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_47E0:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_47F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0x73
        mov    edx, dword ptr [esi]
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
loc_1_4812:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4812
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 9]
        or     al, byte ptr [0x5b64]         ; -> loc_1_5B64
        out    dx, al
        mov    ecx, 1
loc_1_482D:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_482D
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4850:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, 1
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 9]
        or     al, byte ptr [0x5b64]         ; -> loc_1_5B64
        or     al, 0xc0
        out    dx, al
loc_1_4870:
        mov    edx, dword ptr [ecx]
        inc    edx
        in     al, dx
        dec    esi
        jne    loc_1_4870
        mov    dx, word ptr [ecx]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx]
        add    edx, 2
        mov    al, byte ptr [ecx + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 2]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0xff
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_48B5:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_48B5
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 9]
        or     al, byte ptr [0x5b64]         ; -> loc_1_5B64
        out    dx, al
        mov    ecx, 1
loc_1_48D0:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_48D0
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    al, byte ptr [ebp - 3]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 02Bh
        db  0C0h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4910:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0x50
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_4925:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4925
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        or     al, byte ptr [0x5b64]         ; -> loc_1_5B64
        out    dx, al
        mov    ecx, 1
loc_1_4940:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4940
        mov    edx, dword ptr [esi]
        add    dx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        out    dx, al
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    ecx, 1
        mov    edi, edi
loc_1_4960:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4960
        cmp    byte ptr [ebp + 0xc], 1
        cmc    
        sbb    al, al
        and    al, 3
        mov    dx, word ptr [esi]
        out    dx, al
        mov    ecx, 1
loc_1_4979:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4979
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        or     al, byte ptr [0x5b64]         ; -> loc_1_5B64
        out    dx, al
        mov    ecx, 1
loc_1_4994:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4994
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_49B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, 0x58
        mov    esi, dword ptr [edi]
        mov    dx, si
        out    dx, al
        lea    ecx, [esi + 2]
        mov    bl, byte ptr [edi + 8]
        or     bl, 8
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
        test   byte ptr [ebp + 0x10], 4
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    al, byte ptr [edi + 8]
        out    dx, al
        mov    byte ptr [0x5b64], 1          ; -> loc_1_5B64
        jne    loc_1_4A1B
        mov    byte ptr [0x5b64], 2          ; -> loc_1_5B64
loc_1_4A1B:
        mov    edx, dword ptr [edi]
        add    dx, 2
        mov    cl, byte ptr [edi + 8]
        mov    al, byte ptr [0x5b64]         ; -> loc_1_5B64
        or     al, cl
        out    dx, al
        mov    al, cl
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4A40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        or     al, 0x80
        mov    edi, dword ptr [esi]
        mov    dx, di
        out    dx, al
        lea    ecx, [edi + 2]
        mov    bl, byte ptr [esi + 8]
        mov    al, bl
        or     al, 8
        mov    dx, cx
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    dx, di
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        mov    al, byte ptr [0x5b64]         ; -> loc_1_5B64
        or     al, bl
        mov    dx, cx
        out    dx, al
        mov    ecx, 1
loc_1_4A80:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4A80
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        mov    edx, dword ptr [esi]
        or     al, 0x60
        out    dx, al
        add    edx, 2
        mov    cl, byte ptr [esi + 8]
        mov    al, cl
        or     al, 8
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, byte ptr [0x5b64]         ; -> loc_1_5B64
        or     al, cl
        out    dx, al
        mov    ecx, 1
loc_1_4ACD:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4ACD
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    edx, 2
        mov    al, byte ptr [esi + 8]
        out    dx, al
        mov    ecx, 1
loc_1_4AE9:
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        dec    ecx
        jne    loc_1_4AE9
        mov    al, byte ptr [ebp - 2]
        sar    al, 3
        and    al, 0x47
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0xb8
        mov    al, byte ptr [ebp - 3]
        or     al, byte ptr [ebp - 2]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        push   4
        mov    di, 0xfc
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_49B0
loc_1_4B38:
        mov    al, byte ptr [0x5b64]         ; -> loc_1_5B64
        push   eax
        push   edi
        push   esi
        call   sub_1_4BC0
        or     al, al
        je     loc_1_4B4D
        dec    di
        jne    loc_1_4B38
loc_1_4B4D:
        or     di, di
        je     loc_1_4B85
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_49B0
        mov    di, 0xfc
loc_1_4B62:
        mov    al, byte ptr [0x5b64]         ; -> loc_1_5B64
        push   eax
        push   edi
        push   esi
        call   sub_1_4BC0
        or     al, al
        je     loc_1_4B77
        dec    di
        jne    loc_1_4B62
loc_1_4B77:
        or     di, di
        je     loc_1_4B85
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4B85:
        cmp    byte ptr [0x5b64], 1          ; -> loc_1_5B64
        jne    loc_1_4BA0
        or     dword ptr [esi + 0xb], 0x4000
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_4BA0:
        cmp    byte ptr [0x5b64], 2          ; -> loc_1_5B64
        jne    loc_1_4BB0
        or     dword ptr [esi + 0xb], 0x8000
loc_1_4BB0:
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x90
        push   0x14
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4A40
        push   0x90
        push   0x16
        push   esi
        call   sub_1_4A40
        push   0x90
        push   0x18
        push   esi
        call   sub_1_4A40
        mov    ecx, dword ptr [esi]
        add    cx, 2
        mov    bl, byte ptr [esi + 8]
        mov    dx, cx
        mov    al, bl
        out    dx, al
        mov    ax, word ptr [ebp + 0xc]
        shr    ax, 1
        mov    edi, eax
        and    edi, 0xffff
        or     edi, edi
        jle    loc_1_4C2F
        mov    al, byte ptr [ebp + 0x10]
        xor    al, bl
        mov    byte ptr [ebp - 4], al
loc_1_4C20:
        mov    dx, cx
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    al, bl
        out    dx, al
        dec    edi
        or     edi, edi
        jg     loc_1_4C20
loc_1_4C2F:
        push   0x16
        push   esi
        call   sub_1_4AA0
        and    eax, 0xff
        mov    cx, word ptr [ebp + 0xc]
        and    ecx, 0xfe
        sub    eax, ecx
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 008h, 009h, 00Ah
        db  00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 010h, 011h, 012h, 013h, 014h, 015h, 016h, 017h, 018h, 019h, 01Ah
        db  01Bh, 01Ch, 01Dh, 01Eh, 01Fh, 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h, 028h, 029h, 02Ah
        db  02Bh, 02Ch, 02Dh, 02Eh, 02Fh, 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h, 038h, 039h, 03Ah
        db  03Bh, 03Ch, 03Dh, 03Eh, 03Fh, 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h, 048h, 049h, 04Ah
        db  04Bh, 04Ch, 04Dh, 04Eh, 04Fh, 050h, 051h, 052h, 053h, 054h, 055h, 056h, 057h, 058h, 059h, 05Ah
        db  05Bh, 05Ch, 05Dh, 05Eh, 05Fh, 060h, 061h, 062h, 063h, 064h, 065h, 066h, 067h, 068h, 069h, 06Ah
        db  06Bh, 06Ch, 06Dh, 06Eh, 06Fh, 070h, 071h, 072h, 073h, 074h, 075h, 076h, 077h, 078h, 079h, 07Ah
        db  07Bh, 07Ch, 07Dh, 07Eh, 07Fh, 080h, 081h, 082h, 083h, 084h, 085h, 086h, 087h, 088h, 089h, 08Ah
        db  08Bh, 08Ch, 08Dh, 08Eh, 08Fh, 090h, 091h, 092h, 093h, 094h, 095h, 096h, 097h, 098h, 099h, 09Ah
        db  09Bh, 09Ch, 09Dh, 09Eh, 09Fh, 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h, 0A8h, 0A9h, 0AAh
        db  0ABh, 0ACh, 0ADh, 0AEh, 0AFh, 0B0h, 0B1h, 0B2h, 0B3h, 0B4h, 0B5h, 0B6h, 0B7h, 0B8h, 0B9h, 0BAh
        db  0BBh, 0BCh, 0BDh, 0BEh, 0BFh, 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h, 0C8h, 0C9h, 0CAh
        db  0CBh, 0CCh, 0CDh, 0CEh, 0CFh, 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h, 0D8h, 0D9h, 0DAh
        db  0DBh, 0DCh, 0DDh, 0DEh, 0DFh, 0E0h, 0E1h, 0E2h, 0E3h, 0E4h, 0E5h, 0E6h, 0E7h, 0E8h, 0E9h, 0EAh
        db  0EBh, 0ECh, 0EDh, 0EEh, 0EFh, 0F0h, 0F1h, 0F2h, 0F3h, 0F4h, 0F5h, 0F6h, 0F7h, 0F8h, 0F9h, 0FAh
        db  0FBh, 0FCh, 0FDh, 0FEh, 0FFh
loc_1_4DC0:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4E4C:
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
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
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
loc_1_4EC8:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        js     loc_1_4F2C
        add    byte ptr [eax], al
loc_1_4F2C:
        js     0x4f31
        add    byte ptr [eax], al
        mov    esp, 0x50000003
        inc    ebp
        xor    ecx, dword ptr [esi + 0x44]
        dec    ecx
        push   ebx
        add    byte ptr [eax + 0x45], dl
        xor    ecx, dword ptr [esi + 0x44]
        dec    ecx
        push   ebx
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        add    byte ptr [ebp + 0x61], cl
        js     0x4fd1
        insd   dword ptr es:[edi], dx
        jne    loc_1_4FD8
        dec    ebp
        jne    0x4fda
        je     loc_1_4FD9
        arpl   word ptr [ecx + 0x73], sp
        je     0x4fc1
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_4F7C:
        dec    ebp
        popal  
        js     0x4fe9
        insd   dword ptr es:[edi], dx
        jne    loc_1_4FF0
        dec    ebp
        jne    loc_1_4FF2
        je     0x4ff1
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_4FD9
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    esi
        je     0x500f
        outsd  dx, dword ptr [esi]
        jb     0x5006
        inc    ecx
        jb     0x5005
        jae    0x5015
        add    byte ptr [eax], al
        dec    esi
        je     0x501f
        outsd  dx, dword ptr [esi]
        jb     loc_1_5016
        inc    ecx
        jb     0x5015
        jae    0x5025
        add    byte ptr [eax], al
        inc    edx
        jne    loc_1_502A
        push   esp
        jns    loc_1_502A
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_5014
        jns    loc_1_5032
        add    byte ptr gs:[esi + 0x65], dl
        jb     loc_1_502A
        outsd  dx, dword ptr [esi]
        jae    0x5030
        push   ebx
        je     loc_1_502F
        je     loc_1_5045
        jae    loc_1_4FD2
loc_1_4FD2:
        add    byte ptr [eax], al
loc_1_4FD4:
        push   esi
        jb     0x503a
loc_1_4FD8:
        outsd  dx, dword ptr [esi]
loc_1_4FD9:
        jae    loc_1_5040
        push   ebx
        je     0x503f
        je     loc_1_5055
        jae    loc_1_4FE2
loc_1_4FE2:
        add    byte ptr [eax], al
loc_1_4FE4:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x50206d6f
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
loc_1_4FF0:
        je     loc_1_5012
loc_1_4FF2:
        inc    ebp
        je     loc_1_505D
        jb     loc_1_5066
        je     0x501b
        dec    ecx
        dec    ecx
        dec    ecx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x5078
        jb     0x5035
        db  000h
sub_1_5008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5012:
        add    byte ptr [eax], al
loc_1_5014:
        add    byte ptr [eax], al
loc_1_5016:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushal 
        aas    
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
loc_1_5028:
        adc    eax, dword ptr [ecx]
loc_1_502A:
        add    byte ptr [eax], al
        adc    byte ptr [edi], al
loc_1_502F:
        add    byte ptr [eax + 0x47], dl     ; -> loc_1_4750
loc_1_5032:
        add    byte ptr [eax], al
        adc    byte ptr [ecx], cl
        add    byte ptr [edx], dl
        add    dword ptr [eax], eax
        add    al, dh                        ; -> loc_1_46F0
        inc    esi
        add    byte ptr [eax], al
loc_1_5040:
        rol    byte ptr [edx], 0
        dd  loc_1_45F0               ; fixup
loc_1_5045:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_5055:
        add    byte ptr [eax], al
        db  000h
loc_1_5058:
        aas    
        enter  0x5b01, 0x33
loc_1_505D:
        iretd  
        db  0F7h, 0FFh, 0F7h, 0F7h, 0F7h, 07Bh, 0FBh, 0FBh
loc_1_5066:
        sti    
        jnp    loc_1_5066
        jmp    loc_1_5066
        db  0DBh, 01Bh, 01Bh, 09Dh, 0CDh, 0EDh, 0C5h, 08Dh, 0EFh, 0D8h, 08Fh, 08Fh, 08Fh, 08Fh, 08Bh, 0C5h
        db  0C7h, 0C4h, 0C7h, 046h, 0FCh, 0EFh, 0CFh, 0CFh, 0CFh, 0CFh, 0DFh, 0FFh, 0E7h, 0FFh, 0CFh, 0EFh
        db  0CFh, 07Ah, 07Ah, 07Ah, 07Ah, 07Bh, 0BDh, 0BDh, 0BDh, 0BFh, 03Eh, 0FDh, 07Fh, 0BFh, 0BFh, 0BFh
        db  0BDh, 0BEh, 0DFh, 0D7h, 0CFh, 0FFh, 0BFh, 0DEh, 04Fh, 05Fh, 05Fh, 05Fh, 05Dh, 0AFh, 0AFh, 0AFh
        db  0ADh, 0EEh, 0FCh, 03Dh, 0F5h, 0F5h, 0FDh, 0FDh, 0FAh, 0FEh, 0FEh, 0FEh, 0FEh, 0EFh, 0DFh, 0BEh
        db  0AEh, 0AEh, 0ACh, 09Fh, 0D7h, 0D6h, 056h, 0F7h, 055h, 0FCh, 07Fh, 0EFh, 0FFh, 0FFh, 0FFh, 0DFh
        db  0FFh, 0EFh, 0EFh, 0BBh, 0AFh, 0DEh, 03Fh, 03Fh, 03Fh, 03Fh, 03Ch, 09Eh, 01Dh, 09Fh, 09Fh, 09Fh
        db  0FDh, 0FDh, 06Fh, 06Fh, 07Fh, 07Fh, 07Fh, 0BFh, 0BFh, 0BFh, 0BFh, 0BDh, 0DFh, 0BFh, 0BFh, 0BFh
        db  0BFh, 0BFh, 0DFh, 0DFh, 0DFh, 0DEh, 07Eh, 0FDh, 0FFh, 0FFh, 07Fh, 09Fh, 09Fh, 0AFh, 0FFh, 0EDh
        db  0D7h, 0E5h, 07Fh, 0C1h, 0CAh, 0CBh, 0CBh, 0FAh, 0FBh, 079h, 0FEh, 07Fh, 0FFh, 0FAh, 01Ch, 0FFh
        db  0FDh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0EFh, 0DFh, 0EFh, 0EFh, 0EFh, 0EFh, 0CFh
        db  0F7h, 0F7h, 0F5h, 0F7h, 0E7h, 0FDh, 0FFh, 0FDh, 0FDh, 0FDh, 0FFh, 0FEh, 0FFh, 0FFh, 0FFh, 0EFh
        db  0FBh, 0DBh, 0DFh, 0DBh, 0DBh, 0DFh, 0DBh, 0EDh, 0EFh, 0EDh, 0EFh, 0EFh, 03Ch, 0FEh, 0BEh, 0FEh
        db  0FEh, 0BEh, 0FFh, 07Eh, 05Fh, 07Eh, 06Fh, 07Fh, 0CFh, 0CFh, 0DBh, 0C9h, 0DFh, 0DBh, 0FCh, 0FFh
        db  0FEh, 0FFh, 0E7h, 0DDh, 02Eh, 05Dh, 05Fh, 0FDh, 05Dh, 06Eh, 037h, 0BEh, 0AFh, 0BFh, 0B1h, 0D5h
        db  0FFh, 0FFh, 0FCh, 0FFh, 0FDh, 0FEh, 07Fh, 0FFh, 0FFh, 0F6h, 0BDh, 03Fh, 0FFh, 0FFh, 0DFh, 0FFh
        db  0DFh, 0EFh, 0FFh, 0FFh, 0AFh, 0F3h, 0DEh, 06Fh, 06Fh, 06Eh, 06Fh, 06Fh, 0B7h, 037h, 0B3h, 036h
        db  0B3h, 0FDh, 07Fh, 0BFh, 07Fh, 0FFh, 07Fh, 05Fh, 0FFh, 0BFh, 0FFh, 0B7h, 0FBh, 0D5h, 0F7h, 0F7h
        db  0F4h, 0F7h, 0F7h, 0FCh, 0FFh, 0FCh, 07Bh, 0FAh, 0BDh, 07Fh, 0FFh, 0FFh, 0DFh, 0FEh, 07Fh, 0EFh
        db  0FFh, 0EFh, 05Fh, 0EBh, 0DDh, 0F7h, 0CEh, 0F7h, 0C3h, 0F5h, 0FBh, 0E7h, 07Bh, 0E3h, 07Ah, 0DCh
        db  0FCh, 0FFh, 0FFh, 0AFh, 0BFh, 0BEh, 0D7h, 0FEh, 0D7h, 0DEh, 0BDh, 0CDh, 0E7h, 0E6h, 0E7h, 0E5h
        db  0EFh, 0F7h, 076h, 0F7h, 073h, 0F3h, 0FDh, 0BCh, 0EFh, 0FFh, 0EFh, 0FFh, 0DFh, 0F7h, 0FFh, 0F7h
        db  0FEh, 0F3h, 0DFh, 0FFh, 0F6h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0FFh, 0EEh, 0BDh, 07Ch, 0DCh
        db  0FEh, 0FEh, 0FEh, 05Fh, 07Eh, 07Fh, 07Fh, 07Eh, 07Bh, 0D5h, 0DEh, 0D5h, 0DFh, 0DCh, 0DFh, 0FFh
        db  0FFh, 07Fh, 0FFh, 0EEh, 0DDh, 0AFh, 0EDh, 09Fh, 0FDh, 0FDh, 0D7h, 0FFh, 0FFh, 0DEh, 0EFh, 0F5h
        db  0C1h, 0CFh, 0CAh, 0CBh, 0DBh, 0FCh, 0E7h, 0E7h, 0FFh, 0E7h, 067h, 0BDh, 0DFh, 0FDh, 0FDh, 0FEh
        db  0FFh, 0EFh, 0EEh, 0FFh, 0B7h, 0E7h, 0F7h, 0DFh, 0EDh, 0EFh, 0EFh, 0EFh, 0CEh, 0E5h, 0E7h, 0E7h
        db  0E5h, 0E7h, 0FDh, 07Dh, 0FFh, 0EDh, 0FDh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0FFh, 0FBh, 0DFh, 0FFh
        db  0DBh, 0DBh, 0DBh, 0DFh, 0EFh, 06Fh, 0EFh, 0EFh, 06Fh, 0BCh, 0FEh, 0DEh, 0BEh, 0FEh, 0FEh, 0FFh
        db  07Fh, 077h, 07Fh, 07Eh, 07Fh, 0CFh, 0DFh, 0CBh, 0DBh, 0DBh, 0DFh, 0ECh, 0EFh, 0EFh, 0EFh, 06Ch
        db  0DDh, 03Fh, 07Eh, 05Dh, 05Ch, 07Dh, 07Eh, 02Eh, 0BEh, 0BEh, 0AFh, 0B9h, 0D5h, 0EFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FEh, 0FEh, 0FFh, 0FFh, 07Fh, 09Dh, 07Fh, 0EFh, 09Fh, 0FFh, 0BFh, 08Fh, 0DFh, 0DFh
        db  0F7h, 0FFh, 0FBh, 0DFh, 06Dh, 06Fh, 06Fh, 06Fh, 06Fh, 0B7h, 0B7h, 033h, 0B7h, 0B7h, 0FDh, 07Fh
        db  07Fh, 0BFh, 01Fh, 0EFh, 07Fh, 0CFh, 0BFh, 0FFh, 0BFh, 0BBh, 0D7h, 0F7h, 0F7h, 0F7h, 0F5h, 0F7h
        db  0FBh, 0FBh, 0FFh, 0FBh, 0FBh, 0BDh, 01Fh, 0FFh, 0FFh, 07Eh, 07Fh, 07Fh, 0BFh, 0BFh, 0FFh, 0BFh
        db  0BBh, 0DDh, 0CEh, 0FFh, 0CFh, 0F7h, 0C7h, 0FBh, 0E9h, 0FBh, 0E3h, 062h, 0DCh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0BEh, 0FFh, 07Fh, 06Fh, 07Fh, 07Fh, 07Dh, 0CFh, 0E6h, 0E7h, 0EFh, 0EFh, 0EFh, 0F6h, 0E7h
        db  0F7h, 0F7h, 0F7h, 0FDh, 0BFh, 0FCh, 0FDh, 0FFh, 0FDh, 0FEh, 0FEh, 0EEh, 0FEh, 0FEh, 0FBh, 0DFh
        db  0FEh, 0FFh, 0FFh, 0FFh, 0DFh, 0EFh, 0EEh, 0EFh, 0EFh, 06Fh, 07Dh, 0FCh, 0FCh, 0FEh, 04Eh, 0FCh
        db  0FEh, 076h, 06Eh, 07Eh, 07Eh, 07Bh, 0DDh, 0FDh, 0DFh, 0CEh, 0DEh, 0DDh, 0EEh, 06Eh, 0EFh, 06Ah
        db  0EEh, 0DDh, 0BDh, 09Fh, 0FDh, 0CDh, 0EDh, 0FEh, 0DEh, 0DEh, 0F6h, 0CEh, 0EDh, 0DFh, 0C9h, 0FFh
        db  0CFh, 0FFh, 0CFh, 0FEh, 066h, 0FEh, 067h, 0FEh, 01Dh, 09Fh, 0FDh, 07Dh, 08Dh, 0DDh, 0FFh, 0B6h
        db  0DEh, 0EFh, 0D6h, 0ABh, 0DFh, 0EFh, 0EDh, 0EFh, 0CBh, 0CFh, 0F7h, 0E7h, 0F7h, 0F7h, 0E7h, 07Dh
        db  0FFh, 0FFh, 0FFh, 0AFh, 0BFh, 0FFh, 0FFh, 0F7h, 0DEh, 0E7h, 0FFh, 0DFh, 0DFh, 0DFh, 0D9h, 0DBh
        db  0DFh, 0EFh, 07Eh, 0EFh, 06Eh, 0EDh, 0FCh, 0DCh, 0FCh, 0CCh, 0FEh, 0CEh, 0DFh, 05Fh, 06Eh, 047h
        db  07Fh, 05Fh, 0CFh, 0FDh, 0FFh, 0FFh, 0CEh, 0CFh, 0ECh, 0EFh, 0FFh, 067h, 0EFh, 0DDh, 07Fh, 05Eh
        db  07Fh, 07Eh, 07Fh, 07Eh, 0BFh, 0AFh, 0BFh, 0F6h, 0A9h, 0D7h, 0FDh, 0FDh, 0FFh, 0FEh, 0F7h, 0FEh
        db  0F7h, 0FFh, 07Fh, 0FFh, 0BDh, 03Fh, 0FFh, 0FFh, 0FFh, 0FFh, 08Fh, 0FFh, 0FFh, 0DFh, 0FFh, 0F3h
        db  0DFh, 066h, 06Ch, 06Fh, 06Eh, 06Fh, 0B3h, 0B7h, 0B7h, 0B7h, 0B3h, 0FDh, 07Fh, 09Fh, 0CFh, 07Fh
        db  0EFh, 06Fh, 0CFh, 0BFh, 0BFh, 0FFh, 0FBh, 0D6h, 0F7h, 0FDh, 0F7h, 0FFh, 0F7h, 0FEh, 0FBh, 0FFh
        db  0FFh, 0FDh, 0BDh, 05Fh, 07Fh, 0DFh, 07Eh, 0FFh, 07Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0EBh, 0DFh, 0F7h
        db  0F7h, 0C6h, 0D7h, 0FCh, 0E3h, 063h, 063h, 07Bh, 0FBh, 0DCh, 0FDh, 05Ch, 0FEh, 0FFh, 0BDh, 0CFh
        db  0BFh, 07Fh, 07Fh, 07Fh, 0BDh, 0CFh, 0EFh, 0EFh, 0EEh, 0CFh, 0EFh, 0F7h, 0F3h, 0F7h, 077h, 0F7h
        db  07Dh, 0FDh, 0DDh, 0FFh, 0FDh, 0ECh, 0FEh, 0FFh, 0FFh, 0FEh, 07Eh, 07Bh, 0DBh, 0FFh, 0FCh, 0FEh
        db  0F7h, 0FFh, 0EFh, 07Fh, 07Fh, 07Fh, 0FFh, 07Dh, 0FEh, 0DEh, 0FCh, 0FEh, 0DEh, 0DEh, 07Eh, 07Eh
        db  07Eh, 07Eh, 07Bh, 0DFh, 0EFh, 0EFh, 0FDh, 0CFh, 0FFh, 0EEh, 0EAh, 0EEh, 0EFh, 06Fh, 07Ch, 0DFh
        db  0DFh, 0AFh, 09Fh, 0DDh, 0FEh, 0F6h, 0CEh, 0CFh, 0FEh, 0FBh, 0C7h, 0C6h, 0FFh, 0CFh, 0DFh, 0CFh
        db  0FFh, 0E3h, 0FEh, 06Fh, 0E7h, 09Dh, 0DFh, 0EDh, 0CFh, 0FEh, 0CFh, 0EFh, 0EEh, 0F6h, 0F7h, 07Fh
        db  0F3h, 0DBh, 0EAh, 0CBh, 0EAh, 0CBh, 0EBh, 0E5h, 0E7h, 0E7h, 0E7h, 0F7h, 0FDh, 07Dh, 0FFh, 0FDh
        db  0FFh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0FFh, 0DFh, 0FFh, 0DAh, 0DFh, 0DAh, 0DFh, 0EDh, 0EFh
        db  0FFh, 07Fh, 0EFh, 0FCh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FFh, 037h, 07Fh, 05Fh, 067h, 04Fh, 0CFh
        db  0DFh, 0CEh, 0EFh, 0DEh, 0EDh, 0E7h, 0FFh, 0ECh, 0EFh, 067h, 0DDh, 07Fh, 0FFh, 05Fh, 0FCh, 07Fh
        db  0DFh, 02Eh, 0BFh, 0BFh, 0BFh, 0A9h, 0D5h, 0FFh, 0FEh, 0FFh, 0FEh, 0FDh, 0FEh, 0FFh, 0FEh, 0FFh
        db  07Fh, 0BDh, 07Fh, 0DFh, 0FFh, 0CFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F3h, 0DEh, 06Fh, 06Fh
        db  06Fh, 06Fh, 06Fh, 0B7h, 033h, 0B3h, 0B7h, 0B7h, 0FDh, 07Fh, 0FFh, 05Fh, 0EFh, 0FFh, 09Fh, 0CFh
        db  0FFh, 0CFh, 0BFh, 0BBh, 0D7h, 0F7h, 0F7h, 0F5h, 0F7h, 0F7h, 0FBh, 0FBh, 0FAh, 0FBh, 0F9h, 0BDh
        db  04Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0FFh, 0FFh, 0FBh, 0DFh, 0DFh, 0C7h, 0F4h, 0D7h
        db  0D7h, 0FBh, 0FBh, 0E3h, 0E3h, 063h, 0DCh, 0EFh, 0FEh, 0FEh, 0FEh, 0FFh, 05Fh, 07Eh, 0BFh, 0AFh
        db  07Fh, 07Dh, 0CFh, 0C7h, 0E7h, 0E5h, 0C7h, 0C7h, 0F2h, 0F3h, 0F3h, 0F3h, 073h, 0FDh, 0FDh, 0FFh
        db  0DDh, 0FDh, 0DDh, 05Eh, 0FEh, 0FEh, 0FFh, 0FFh, 0FBh, 0DFh, 0FFh, 0FFh, 0FFh, 0DFh, 0FFh, 0EFh
        db  0FFh, 0EEh, 07Fh, 07Fh, 07Dh, 0FCh, 0ECh, 05Eh, 0FCh, 0ECh, 0DEh, 077h, 07Eh, 07Eh, 07Eh, 07Fh
        db  0DFh, 0ECh, 0DFh, 0EEh, 0DFh, 0E7h, 0EFh, 076h, 0EFh, 0EEh, 0EFh, 0FCh, 0FFh, 0BFh, 0DFh, 0EDh
        db  0FFh, 0FFh, 0E7h, 0FEh, 0D6h, 0CFh, 0EFh, 0C3h, 0FFh, 0FFh, 0FDh, 0FFh, 0CDh, 087h, 0E5h, 06Eh
        db  0EFh, 0FFh, 09Ch, 0BFh, 07Dh, 0EDh, 0EFh, 0FDh, 069h, 0FEh, 0FFh, 07Fh, 046h, 0E3h, 0D3h, 0CDh
        db  0CFh, 0CFh, 0CFh, 0CFh, 087h, 0E7h, 0E5h, 0E7h, 0E7h, 0FDh, 0FFh, 0FFh, 0DFh, 0FFh, 0DFh, 0F9h
        db  0FFh, 0F7h, 0FFh, 0C7h, 0FFh, 0DFh, 0DFh, 0DFh, 0DFh, 0FFh, 0DEh, 09Fh, 0EDh, 0EFh, 0ECh, 0EFh
        db  0FCh, 0FEh, 0FEh, 0FEh, 0BEh, 0FEh, 0B9h, 07Fh, 047h, 07Fh, 07Fh, 07Fh, 0CFh, 0CBh, 0DDh, 0D9h
        db  0DDh, 0D9h, 08Fh, 0E5h, 0FFh, 0EFh, 0FFh, 0DDh, 05Fh, 0FDh, 07Dh, 07Eh, 05Dh, 079h, 02Fh, 0AEh
        db  0BEh, 0B6h, 0B1h, 0D7h, 0F7h, 0FDh, 0FDh, 0FDh, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0BDh, 07Fh
        db  0BFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0CFh, 0FFh, 0FFh, 0BBh, 0DEh, 06Fh, 06Fh, 067h, 06Eh, 067h
        db  0F7h, 0B7h, 0B7h, 0B7h, 0B7h, 0FDh, 07Fh, 0FFh, 01Fh, 0DFh, 08Fh, 09Fh, 0DFh, 0AFh, 0BFh, 0BFh
        db  0BBh, 0D7h, 0F7h, 0FFh, 0FFh, 0FFh, 0F7h, 0FBh, 0F9h, 0FBh, 0FBh, 0FFh, 0BDh, 07Fh, 07Fh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0F7h, 0FFh, 0F7h, 0EBh, 0DDh, 0F3h, 0C2h, 0F3h, 0D6h, 0F7h, 08Bh, 0E3h
        db  0E3h, 0E3h, 0FAh, 0DCh, 0ECh, 0FEh, 0FEh, 0FEh, 0CFh, 079h, 07Fh, 07Fh, 07Fh, 06Fh, 07Dh, 0C7h
        db  0EFh, 0EFh, 0EFh, 0CDh, 0E5h, 082h, 0F3h, 0F3h, 0F3h, 0F7h, 0BDh, 0BDh, 0FDh, 0FDh, 0DDh, 0FCh
        db  0F8h, 0FFh, 0EEh, 0FEh, 07Eh, 0F3h, 0DFh, 0DFh, 0DFh, 0DFh, 0DFh, 0FFh, 09Bh, 0FFh, 0FFh, 0FEh
        db  07Fh, 0FDh, 07Ch, 0DCh, 0FCh, 0FCh, 0FEh, 0F8h, 076h, 077h, 07Eh, 07Fh, 07Fh, 0CDh, 0DFh, 0DDh
        db  0FFh, 0DEh, 0FFh, 086h, 06Eh, 076h, 0EFh, 0F6h, 0FCh, 0EFh, 0BDh, 0DDh, 05Dh, 0FDh, 0F8h, 0DFh
        db  0FFh, 0FEh, 0F7h, 0F5h, 0C7h, 0CEh, 0DEh, 0FFh, 0FEh, 0CBh, 0E2h, 0FFh, 0EEh, 0FFh, 07Fh, 0BDh
        db  0DFh, 0FEh, 05Fh, 07Dh, 0FFh, 0DFh, 0DEh, 0FFh, 01Eh, 0E7h, 0EBh, 0DBh, 0E7h, 0CBh, 0CFh, 0C3h
        db  0EFh, 0F7h, 0F6h, 0F5h, 0F5h, 0E7h, 0BDh, 07Dh, 0FFh, 07Fh, 0FFh, 0FCh, 07Eh, 0E7h, 0FEh, 0FFh
        db  0FFh, 0F7h, 0DFh, 0DDh, 0FDh, 070h, 0DFh, 0DCh, 0ECh, 0EBh, 0FFh, 0FDh, 06Fh, 0FCh, 0BEh, 0FEh
        db  0F6h, 0FCh, 07Eh, 0EFh, 07Fh, 067h, 07Fh, 07Fh, 04Fh, 0CEh, 0ECh, 0FFh, 05Fh, 0FEh, 0CFh, 0EFh
        db  0EFh, 0FFh, 0EFh, 077h, 0DDh, 07Fh, 0FEh, 077h, 05Fh, 07Fh, 0FEh, 0F6h, 0BFh, 0B7h, 0BFh, 0E9h
        db  0D5h, 0FFh, 0FFh, 0CFh, 0FFh, 0FEh, 0FFh, 0FFh, 0FBh, 0FFh, 07Fh, 0BDh, 07Fh, 0FFh, 0F7h, 0EEh
        db  0FFh, 0FFh, 0DFh, 0FFh, 0C7h, 0FFh, 0F3h, 0DFh, 06Dh, 06Fh, 0E5h, 06Fh, 06Fh, 0B7h, 0B2h, 033h
        db  0B7h, 0B7h, 0FDh, 07Eh, 09Eh, 0FFh, 0FFh, 03Fh, 0FFh, 0FFh, 0F7h, 0FFh, 0BFh, 0FBh, 0D6h, 0F6h
        db  0F7h, 072h, 0FFh, 0F7h, 0FBh, 0FEh, 0FBh, 0FFh, 0F9h, 03Dh, 04Fh, 07Eh, 07Dh, 07Fh, 0BFh, 05Fh
        db  0BFh, 0E7h, 0BFh, 07Fh, 0BBh, 0DFh, 0C6h, 0C7h, 077h, 0D7h, 0F3h, 0FEh, 0EBh, 0FFh, 0E3h, 0FBh
        db  0DCh, 0FEh, 0DFh, 0B4h, 0FEh, 0DEh, 0DEh, 0FFh, 0AFh, 0FFh, 0DFh, 07Dh, 0CFh, 0EDh, 0EDh, 06Dh
        db  0CFh, 0EFh, 0F7h, 0E7h, 0F6h, 0F7h, 0F7h, 0FDh, 0FFh, 0FFh, 0F5h, 0FDh, 05Dh, 0EEh, 07Eh, 0FEh
        db  077h, 0FEh, 0FBh, 0DBh, 0FDh, 0FFh, 0DFh, 0FDh, 0FFh, 0FFh, 0FEh, 07Fh, 0FFh, 0EFh, 03Dh, 05Ch
        db  0FEh, 0FCh, 0CCh, 05Eh, 0FEh, 076h, 07Fh, 067h, 07Eh, 07Bh, 0D7h, 0FFh, 0FFh, 0FFh, 0CDh, 0CFh
        db  0EEh, 067h, 0FFh, 06Eh, 0FFh, 05Ch, 09Fh, 0FDh, 0FFh, 07Dh, 0BFh, 0F6h, 0FEh, 0FEh, 0E6h, 0FFh
        db  0EDh, 0C3h, 007h, 04Eh, 0BAh, 07Dh, 0CDh, 0FFh, 0C2h, 067h, 0FFh, 0FEh, 09Ch, 0F3h, 08Bh, 0F9h
        db  07Bh, 07Dh, 0DEh, 0FCh, 0FFh, 0FEh, 0DEh, 0FBh, 0DBh, 00Fh, 05Bh, 057h, 05Fh, 01Fh, 0B4h, 0F7h
        db  077h, 0E7h, 0E7h, 0FDh, 0F2h, 0FAh, 077h, 0FBh, 0E3h, 07Ah, 0FCh, 07Eh, 0FFh, 0C7h, 0FFh, 0DDh
        db  00Ah, 08Bh, 08Ch, 08Eh, 00Bh, 0AFh, 08Dh, 06Dh, 0EAh, 0EFh, 0FCh, 0F2h, 0F7h, 0F9h, 0F7h, 0F3h
        db  0FBh, 07Bh, 07Fh, 07Fh, 077h, 07Fh, 0CFh, 03Eh, 09Fh, 09Fh, 09Fh, 0DDh, 0EFh, 09Fh, 06Eh, 0E7h
        db  0EFh, 0DDh, 071h, 074h, 07Ah, 074h, 07Ch, 07Eh, 0F8h, 0BEh, 0EFh, 0B6h, 0B9h, 0D6h, 0FFh, 05Eh
        db  07Dh, 04Fh, 05Fh, 0AFh, 0EFh, 0FFh, 0FFh, 0FFh, 0BDh, 03Eh, 0BFh, 0FFh, 07Fh, 0FFh, 0FBh, 0BBh
        db  0FFh, 0F7h, 0FFh, 0F3h, 0DFh, 0E7h, 06Fh, 06Fh, 06Fh, 06Dh, 0F7h, 0F7h, 0B7h, 0B7h, 037h, 0FDh
        db  07Fh, 0DFh, 0AFh, 0FFh, 0BFh, 0FFh, 0DFh, 0BFh, 0DFh, 0A7h, 0BBh, 0D6h, 0F4h, 0B7h, 0B7h, 075h
        db  0B5h, 0BAh, 0BFh, 0FFh, 0FDh, 0FDh, 0BDh, 05Fh, 077h, 0FDh, 0E3h, 0F6h, 0FAh, 03Eh, 0FFh, 0FFh
        db  0F7h, 0EBh, 0DFh, 037h, 057h, 0F6h, 0C7h, 0D7h, 0EBh, 08Bh, 06Fh, 0E3h, 0E3h, 0DCh, 0F1h, 07Ah
        db  0FDh, 0BFh, 0BFh, 05Fh, 0D9h, 0BFh, 0FFh, 07Fh, 07Dh, 0CFh, 03Fh, 057h, 037h, 037h, 017h, 0A7h
        db  0AFh, 0E6h, 0F7h, 0F7h, 0FDh, 0F1h, 0F9h, 0E1h, 0F1h, 0F1h, 0FAh, 0F8h, 0FEh, 0FEh, 06Eh, 0FBh
        db  0DBh, 03Fh, 0FFh, 0EFh, 0FFh, 0FFh, 0A7h, 0C7h, 0FBh, 0FFh, 0FFh, 0FDh, 0F3h, 0DFh, 0CFh, 0DFh
        db  0EFh, 0EBh, 06Fh, 0FEh, 076h, 077h, 07Fh, 0DFh, 02Dh, 007h, 00Fh, 00Fh, 007h, 0EFh, 0DEh, 0E6h
        db  0EEh, 067h, 0FCh, 0D3h, 0F1h, 0F1h, 0F1h, 0D1h, 0D6h, 09Ch, 0FEh, 0DFh, 0FEh, 0FDh, 0CDh, 0F5h
        db  04Fh, 04Fh, 07Eh, 05Eh, 05Eh, 09Dh, 0EAh, 0E7h, 0EEh, 01Dh, 0FFh, 0F7h, 07Fh, 0E7h, 0B4h, 0BCh
        db  0DDh, 0FFh, 07Eh, 0FFh, 06Bh, 0D7h, 0FFh, 0EFh, 0C7h, 05Bh, 05Fh, 0FFh, 087h, 067h, 0C7h, 0E7h
        db  07Dh, 0FFh, 0FFh, 0F7h, 0E7h, 0F7h, 0FCh, 0E9h, 05Fh, 0FDh, 0FFh, 0FFh, 0DFh, 01Fh, 0DBh, 01Fh
        db  05Fh, 05Bh, 09Fh, 09Ch, 0CDh, 04Bh, 0EFh, 0FCh, 0F1h, 0FCh, 0F4h, 0F7h, 0F7h, 07Bh, 0E9h, 077h
        db  07Dh, 07Fh, 07Fh, 0CFh, 01Dh, 05Fh, 059h, 05Bh, 05Ch, 08Eh, 0DFh, 0FFh, 06Fh, 0EEh, 0DDh, 053h
        db  0E6h, 073h, 0F4h, 074h, 079h, 0F9h, 0B4h, 0FEh, 0AEh, 0B1h, 0D7h, 04Ch, 0BFh, 05Eh, 05Dh, 05Fh
        db  0AFh, 05Fh, 0AFh, 0AFh, 0F6h, 0BDh, 037h, 0C7h, 0FAh, 0F6h, 0F5h, 0FAh, 0FBh, 0FBh, 0CBh, 0FFh
        db  0FBh, 0DFh, 06Eh, 06Fh, 06Dh, 06Fh, 0CFh, 0E7h, 0B3h, 0F7h, 0F7h, 0B7h, 0FDh, 07Fh, 0FFh, 0EFh
        db  0FFh, 0EFh, 0EFh, 0FFh, 0FFh, 0EFh, 0BFh, 0BBh, 0D6h, 0B5h, 07Eh, 077h, 076h, 074h, 0BBh, 03Fh
        db  0BFh, 0BBh, 0FBh, 0BDh, 073h, 0E5h, 0DDh, 0FDh, 0F5h, 0FAh, 0FAh, 07Ah, 0FAh, 0B7h, 0ABh, 0DDh
        db  0F7h, 007h, 005h, 0B4h, 057h, 0BAh, 0EBh, 0C3h, 083h, 0F8h, 0DCh, 0ECh, 0D3h, 072h, 0F4h, 0E4h
        db  0F8h, 0BFh, 0CBh, 069h, 06Eh, 07Dh, 0CDh, 0F7h, 06Fh, 0E7h, 077h, 057h, 0B3h, 067h, 0B7h, 0DFh
        db  0F7h, 0FDh, 0FDh, 0E8h, 0FDh, 0F9h, 0F5h, 0FAh, 0BEh, 0F4h, 0FCh, 07Eh, 0F3h, 0DFh, 03Fh, 0FFh
        db  0FDh, 09Dh, 04Fh, 0EFh, 0CFh, 06Fh, 0DEh, 06Bh, 0FDh, 0F3h, 0FCh, 0DCh, 075h, 0F5h, 0DFh, 07Fh
        db  072h, 075h, 0FEh, 07Bh, 0DFh, 0EFh, 0BDh, 0CFh, 05Eh, 05Dh, 0DAh, 0BFh, 0BFh, 00Fh, 0EEh, 0DCh
        db  0BFh, 0C7h, 0FDh, 0D9h, 0F5h, 0F7h, 0F1h, 0FEh, 0F9h, 0F6h, 0F5h, 0D7h, 0FEh, 0DEh, 075h, 07Ch
        db  0FBh, 04Eh, 027h, 026h, 0DBh, 07Fh, 09Ch, 0DFh, 0FEh, 055h, 0F7h, 0FDh, 0FDh, 03Dh, 0DCh, 0FDh
        db  0CFh, 0E3h, 0DFh, 0EFh, 0EFh, 08Fh, 0CFh, 0C5h, 0C7h, 0CFh, 0CFh, 07Fh, 067h, 0FDh, 0FFh, 0FFh
        db  076h, 0EEh, 0DFh, 0DFh, 0BBh, 0FBh, 0FFh, 0D7h, 0FFh, 0DBh, 0DBh, 0FFh, 03Bh, 01Bh, 0F7h, 0AFh
        db  0BDh, 0AAh, 0CCh, 0EFh, 0FCh, 0FEh, 07Eh, 072h, 070h, 0FEh, 0D9h, 07Dh, 0FDh, 0FDh, 0FFh, 07Fh
        db  0CAh, 0CEh, 0FFh, 05Fh, 07Dh, 01Fh, 095h, 0CEh, 0CFh, 08Fh, 0EDh, 0DDh, 07Fh, 07Eh, 077h, 076h
        db  073h, 0F9h, 0BBh, 0FAh, 0B8h, 0BEh, 0B9h, 0D7h, 0FFh, 0FFh, 05Fh, 04Fh, 0DFh, 0AFh, 0AFh, 027h
        db  0BEh, 0FFh, 0BDh, 0FFh, 0FFh, 0F7h, 0E7h, 0E6h, 0BBh, 0FBh, 0FBh, 0F5h, 0DFh, 0DBh, 0CFh, 0EEh
        db  06Dh, 0E6h, 06Fh, 06Fh, 0F3h, 0B7h, 0B3h, 0B7h, 0B7h, 0FDh, 07Fh, 07Eh, 0FFh, 0FFh, 0DFh, 0AFh
        db  0FFh, 0DFh, 0E7h, 0BFh, 0BBh, 0D7h, 0F1h, 0F5h, 077h, 0B7h, 075h, 0BFh, 05Bh, 0DBh, 0BBh, 0FBh
        db  0BDh, 07Fh, 07Fh, 065h, 07Dh, 073h, 07Ah, 0FBh, 0FBh, 0FAh, 077h, 0ABh, 0DFh, 0DFh, 0FFh, 046h
        db  083h, 045h, 09Bh, 01Bh, 09Bh, 0C3h, 0FBh, 0DCh, 0FFh, 0FFh, 0D7h, 075h, 092h, 0FAh, 079h, 079h
        db  07Dh, 0DEh, 07Dh, 0CDh, 0DFh, 0EFh, 07Fh, 06Fh, 06Fh, 0B7h, 0DAh, 0DBh, 05Bh, 0F7h, 0FDh, 07Dh
        db  0FCh, 0E9h, 0F9h, 07Fh, 0EAh, 0EAh, 0B2h, 0FDh, 0FEh, 0FBh, 0DFh, 0F4h, 0FEh, 0FFh, 0DFh, 0FFh
        db  0FFh, 0E7h, 06Fh, 0DFh, 06Fh, 0FDh, 0FDh, 0FEh, 0FFh, 0EEh, 0F6h, 0FFh, 07Eh, 0FEh, 0EDh, 0F6h
        db  07Bh, 0DDh, 0CFh, 0FFh, 06Fh, 0DDh, 01Fh, 0F6h, 0CFh, 0CFh, 0CFh, 0EFh, 05Dh, 0BFh, 0FDh, 0F5h
        db  0EDh, 075h, 0BCh, 0F2h, 0BAh, 0FCh, 0FEh, 0EDh, 0D7h, 003h, 00Dh, 07Dh, 0CFh, 0CAh, 08Fh, 09Fh
        db  0ADh, 0FEh, 0FFh, 09Dh, 051h, 0D1h, 0F5h, 0FDh, 06Dh, 0F9h, 058h, 0FDh, 07Fh, 0CEh, 0BBh, 0DFh
        db  0FFh, 0CFh, 05Fh, 0DFh, 0DFh, 0EFh, 085h, 0E5h, 0E7h, 0E7h, 07Dh, 0FDh, 0FDh, 0F4h, 0FDh, 0FCh
        db  0FFh, 0F5h, 0F7h, 07Fh, 0FFh, 0BFh, 0DEh, 0DFh, 07Bh, 05Bh, 01Fh, 01Bh, 0FFh, 08Dh, 04Fh, 0EDh
        db  0EFh, 0FCh, 0FDh, 0B6h, 0F7h, 0A1h, 0B3h, 0FFh, 0F8h, 05Bh, 06Fh, 057h, 07Fh, 0CFh, 03Eh, 07Fh
        db  05Fh, 03Fh, 03Fh, 09Fh, 08Fh, 017h, 0EFh, 0FFh, 0DDh, 072h, 066h, 065h, 072h, 070h, 069h, 0B1h
        db  0F9h, 0BEh, 0BEh, 0B9h, 0D5h, 0BFh, 0D6h, 057h, 05Fh, 057h, 0ABh, 0E7h, 0ABh, 0FFh, 0FFh, 0BDh
        db  0F7h, 0F7h, 0EBh, 0FBh, 0FBh, 0FBh, 0FBh, 0FBh, 0F7h, 0FFh, 0DBh, 0CFh, 06Eh, 06Fh, 06Eh, 06Fh
        db  06Fh, 0B3h, 0B7h, 0F3h, 0B7h, 0B3h, 0FDh, 07Fh, 0FFh, 0FFh, 07Fh, 0FFh, 0FFh, 0F7h, 0F7h, 0FFh
        db  037h, 0FBh, 0D7h, 0F7h, 0B3h, 077h, 072h, 073h, 0DBh, 0BFh, 0BBh, 0FBh, 0FFh, 0BCh, 0F5h, 0FBh
        db  0FFh, 0F6h, 0F3h, 0EAh, 0FAh, 0FAh, 037h, 0B7h, 0FBh, 0DFh, 003h, 07Fh, 043h, 01Fh, 017h, 0DBh
        db  02Bh, 08Bh, 0E3h, 078h, 0DCh, 0F3h, 077h, 0F7h, 073h, 0F1h, 0BBh, 0BDh, 079h, 0DFh, 076h, 0ADh
        db  0CDh, 066h, 077h, 0A7h, 0C7h, 0D6h, 0BBh, 087h, 0AFh, 0F7h, 0F7h, 0FDh, 0D5h, 0F5h, 0F8h, 0FDh
        db  0FDh, 0FDh, 0F8h, 07Dh, 0F6h, 07Eh, 0EBh, 0DFh, 07Fh, 05Fh, 07Fh, 0AFh, 0DFh, 0DFh, 09Fh, 0FFh
        db  0FFh, 0FEh, 0FDh, 074h, 0F7h, 0F4h, 0F6h, 0FFh, 0FAh, 0F8h, 07Eh, 0FEh, 077h, 07Bh, 0CFh, 0CEh
        db  07Eh, 05Fh, 04Eh, 01Fh, 0AFh, 09Fh, 0DFh, 06Fh, 0F7h, 0FCh, 07Dh, 0F7h, 0B7h, 0F9h, 0F1h, 0B4h
        db  0F9h, 0FBh, 0FEh, 0FFh, 0FBh, 0DDh, 07Eh, 0FAh, 07Eh, 07Eh, 0FAh, 0BFh, 03Fh, 02Fh, 03Bh, 07Fh
        db  0FDh, 066h, 0F7h, 0E7h, 0F7h, 0E7h, 0FBh, 033h, 07Bh, 07Bh, 07Bh, 077h, 0CFh, 06Eh, 0A7h, 0AFh
        db  0CFh, 0AFh, 0A7h, 097h, 037h, 037h, 036h, 0FDh, 072h, 0EAh, 0E2h, 0EAh, 0EAh, 0EDh, 0B1h, 0D5h
        db  0F1h, 075h, 077h, 0DEh, 0BFh, 09Fh, 09Eh, 09Fh, 09Eh, 0EFh, 0EFh, 0FFh, 0DFh, 0DEh, 07Dh, 068h
        db  0F0h, 0F0h, 0F0h, 0F0h, 0F8h, 078h, 07Ch, 07Ch, 07Ch, 07Fh, 0DFh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh
        db  0FFh, 07Fh, 0FFh, 0FFh, 07Fh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_5B34:
        add    dword ptr [edx], eax
        add    al, 0xc
        adc    byte ptr [eax], ah
        inc    eax
        or     byte ptr [0xe0d0e07], 0xe
        add    dword ptr [eax], eax
loc_1_5B44:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x60c0c06], cl
        add    byte ptr [eax], al
loc_1_5B54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, 0
loc_1_5B64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x5b68]       ; -> obj1:0x5B68
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
        mov    eax, dword ptr [0x5b68]       ; -> obj1:0x5B68
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
