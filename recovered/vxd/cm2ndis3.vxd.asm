; CM2NDIS3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5774 instructions recovered by recursive descent

Declare_Virtual_Device CM2NDIS3, 3, 10, CM2NDIS3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3ca0  R-X
loc_1_0000:
        mov    al, byte ptr [0x668]          ; -> obj1:0x0668
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x668]              ; -> obj1:0x0668
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_1A90
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
CM2NDIS3_Control:
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
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_03B4:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
sub_1_03F1:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
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
sub_1_04A4:
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
        db  'CM2NDIS3'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  CM2NDIS3_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        lea    ecx, [eax + 0x3f8]
        cmp    eax, ecx
        jae    loc_1_069C
        mov    bl, byte ptr [ebp + 0x10]
loc_1_0684:
        mov    dl, byte ptr [eax]
        cmp    dl, 0xff
        je     loc_1_069C
        cmp    bl, dl
        je     loc_1_069C
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        lea    eax, [eax + edx*2 + 4]
        cmp    ecx, eax
        ja     loc_1_0684
loc_1_069C:
        cmp    byte ptr [eax], 0xff
        jne    loc_1_06A3
        sub    eax, eax
loc_1_06A3:
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_06B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x51]
        mov    al, 0x62
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 7]
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_06D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0x3938                   ; -> loc_1_3938
        mov    ecx, dword ptr [edx + 0x5d]
        mov    eax, dword ptr [edx + 0x61]
        cmp    dword ptr [edx + 0x3c], 2
        jne    loc_1_06F0
        add    eax, 0xf08
        jmp    loc_1_06F3
loc_1_06F0:
        lea    eax, [ecx + 0x18]
loc_1_06F3:
        cmp    byte ptr [0x3938], 0          ; -> loc_1_3938
        je     loc_1_0714
        lea    esp, [esp]
loc_1_0700:
        mov    bl, byte ptr [esi]
        cmp    bl, 0x20
        je     loc_1_0714
        inc    esi
        cmp    byte ptr [eax], bl
        jne    loc_1_0727
        add    eax, 2
        cmp    byte ptr [esi], 0
        jne    loc_1_0700
loc_1_0714:
        test   byte ptr [edx + 0x45], 0x10
        mov    eax, dword ptr [edx + 0x41]
        je     loc_1_0730
        add    eax, 0x10
        mov    dword ptr [edx + 0x51], eax
        mov    al, 0x41
        jmp    loc_1_073E
loc_1_0727:
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_0730:
        mov    dword ptr [edx + 0x51], eax
        add    eax, 0x10
        mov    dword ptr [edx + 0x41], eax
        mov    dword ptr [edx + 0x49], eax
        mov    al, 0x61
loc_1_073E:
        mov    byte ptr [ecx + 0x800], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x5d]
        mov    ecx, dword ptr [esi + 0x61]
        cmp    dword ptr [esi + 0x3c], 2
        jne    loc_1_07B0
        mov    dx, word ptr [esi + 0x51]
        sub    al, al
        out    dx, al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 2], al
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [ecx + eax], 0x70
        inc    word ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ecx + 3], al
        mov    al, 1
        mov    dx, word ptr [esi + 0x51]
        out    dx, al
        mov    al, 5
        out    dx, al
        mov    byte ptr [esi + 0x6d], 0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_07B0:
        mov    byte ptr [edi + 0x806], 0xd4
        push   esi
        call   sub_1_08D0
        or     al, al
        jne    loc_1_07D0
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_07D0:
        push   0x30d40
        call   sub_1_04D0
        test   byte ptr [edi + 0x806], 1
        jne    loc_1_07F1
        mov    byte ptr [esi + 0x6d], 0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_07F1:
        mov    byte ptr [esi + 0x6d], 1
        mov    byte ptr [edi + 0x806], 0x55
        push   0x30d40
        call   sub_1_04D0
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_0810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x3c], 2
        jne    loc_1_087A
        mov    dx, word ptr [esi + 0x51]
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [esi + 0x61]
        mov    al, byte ptr [eax + 0xf36]
        mov    byte ptr [esi + 0x79], al
        mov    eax, dword ptr [esi + 0x61]
        mov    al, byte ptr [eax + 0xf38]
        mov    byte ptr [esi + 0x7a], al
        mov    eax, dword ptr [esi + 0x61]
        mov    al, byte ptr [eax + 0xf3a]
        mov    byte ptr [esi + 0x7b], al
        mov    eax, dword ptr [esi + 0x61]
        mov    al, byte ptr [eax + 0xf24]
        mov    byte ptr [esi + 0x7c], al
        mov    eax, dword ptr [esi + 0x61]
        pop    edi
        mov    al, byte ptr [eax + 0xf22]
        mov    byte ptr [esi + 0x7d], al
        mov    eax, dword ptr [esi + 0x61]
        mov    al, byte ptr [eax + 0xf20]
        mov    byte ptr [esi + 0x7e], al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_087A:
        mov    ecx, dword ptr [esi + 0x5d]
        sub    edi, edi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, ebx
loc_1_0884:
        cmp    byte ptr [ecx], 0xff
        je     loc_1_08B5
        push   0x89
        push   ecx
        push   esi
        call   sub_1_0670
        cmp    byte ptr [eax + 8], bl
        jne    loc_1_08A6
        cmp    byte ptr [eax + 0xa], 0x80
        jne    loc_1_08A6
        cmp    byte ptr [eax + 0xc], 0xc7
        je     loc_1_08B5
loc_1_08A6:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        lea    ecx, [eax + ecx*2 + 4]
        inc    edi
        cmp    edi, 0xa
        jl     loc_1_0884
loc_1_08B5:
        sub    ecx, ecx
loc_1_08B7:
        mov    dl, byte ptr [eax + ecx*2 + 8]
        mov    byte ptr [esi + ecx + 0x79], dl
        inc    ecx
        cmp    ecx, 6
        jl     loc_1_08B7
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_08D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x3c], 3
        jne    loc_1_08F8
        mov    si, word ptr [ecx + 0x51]
        mov    dx, si
        mov    al, 0x11
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 0x62
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, 4
        out    dx, al
loc_1_08F8:
        mov    byte ptr [ecx + 0x6e], 0x52
        mov    eax, dword ptr [ecx + 0x3c]
        cmp    eax, 1
        je     loc_1_0909
        cmp    eax, 2
        jne    loc_1_0917
loc_1_0909:
        cmp    byte ptr [ecx + 0x6d], 0
        je     loc_1_0917
        lea    edi, [ecx + 0x6f]
        mov    byte ptr [edi], 0x20
        jmp    loc_1_091D
loc_1_0917:
        lea    edi, [ecx + 0x6f]
        mov    byte ptr [edi], 0xa0
loc_1_091D:
        mov    si, word ptr [ecx + 0x49]
        mov    al, byte ptr [ecx + 0x6e]
        lea    edx, [esi + 6]
        or     al, 0x80
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, byte ptr [edi]
        out    dx, al
        lea    edx, [esi + 2]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 3]
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 0x49]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 0x49]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 4]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 5]
        out    dx, al
        sub    bl, bl
        lea    ebx, [ebx]
loc_1_0960:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x7f]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     loc_1_0960
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 0xa7]
        test   al, 0x20
        jne    loc_1_09B2
        test   al, 4
        je     loc_1_09B2
        sub    bl, bl
loc_1_0997:
        mov    dl, bl
        and    edx, 0xffff00ff
        add    dx, si
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_0997
        jmp    loc_1_09D3
loc_1_09B2:
        sub    bl, bl
loc_1_09B4:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x71]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_09B4
loc_1_09D3:
        mov    al, byte ptr [edi]
        or     al, 8
        lea    edx, [esi + 7]
        out    dx, al
        lea    edx, [esi + 0xb]
        mov    al, 7
        out    dx, al
        lea    edx, [esi + 0xc]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 0xd]
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [ecx + 0x6e]
        out    dx, al
        mov    bl, 0xfa
loc_1_09F6:
        push   0x3e8
        call   sub_1_04D0
        dec    bl
        jne    loc_1_09F6
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0xff
        je     loc_1_0A2D
        or     al, al
        jne    loc_1_0A64
loc_1_0A2D:
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        not    al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 2]
        not    eax
        cmp    eax, ecx
        jne    loc_1_0A62
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0A62:
        sub    al, al
loc_1_0A64:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 2
        mov    si, word ptr [ecx + 0x49]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 0x70]
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 0x81
        out    dx, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cx, word ptr [eax + 0x49]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 2
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 5
        out    dx, al
        mov    edi, 0x3930                   ; -> loc_1_3930
        add    eax, 0
loc_1_0AE0:
        mov    ecx, dword ptr [esi + 0x49]
        mov    ax, 0x3d
        lea    edx, [ecx + 8]
        out    dx, ax
        push   0x1f
        mov    eax, dword ptr [edi]
        push   eax
        add    ecx, 8
        push   ecx
        call   sub_1_03B4
        add    edi, 4
        cmp    edi, 0x3938                   ; -> loc_1_3938
        jb     loc_1_0AE0
        push   0x3e8
        call   sub_1_04D0
        mov    ecx, dword ptr [esi + 0x49]
        add    cx, 0xa
        mov    dx, cx
        mov    al, 0x82
        out    dx, al
        push   0x1f4
        call   sub_1_04D0
        sub    edi, edi
        mov    ebx, 0x40
loc_1_0B30:
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 5
        in     al, dx
        test   bl, al
        jne    loc_1_0B54
        push   2
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        inc    edi
        cmp    edi, 2
        jb     loc_1_0B30
loc_1_0B54:
        sub    edi, 2
        cmp    edi, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [esi + 0x49]
        in     al, dx
        mov    dx, word ptr [esi + 0x49]
        out    dx, al
        inc    dx
        mov    si, word ptr [esi + 0x49]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        sub    al, al
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AA0
        push   esi
        call   sub_1_08D0
        push   esi
        call   sub_1_0A70
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_0BFB
loc_1_0BC2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0BD0:
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
        je     loc_1_0BF2
        xor    eax, 0x4c11db7
loc_1_0BF2:
        dec    esi
        jne    loc_1_0BD0
        inc    edx
        cmp    dword ptr [ebp + 0xc], edx
        ja     loc_1_0BC2
loc_1_0BFB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C10:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BB0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0C25:
        add    ecx, ecx
        test   eax, edx
        je     loc_1_0C2E
        or     ecx, 1
loc_1_0C2E:
        add    edx, edx
        cmp    edx, 0x40
        jb     loc_1_0C25
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
sub_1_0C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0C69:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x71], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_0C69
        push   0x3850                        ; -> obj1:0x3850
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_0CC7
        sub    edi, edi
loc_1_0C98:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x3850]         ; -> obj1:0x3850
        push   eax
        call   sub_1_0C10
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x71], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_0C98
loc_1_0CC7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x49]
        mov    al, byte ptr [esi + 0x6e]
        or     al, 0x80
        lea    edx, [edi + 6]
        out    dx, al
        lea    ebx, [edi + 7]
        mov    al, byte ptr [esi + 0x6f]
        or     al, 4
        mov    dx, bx
        out    dx, al
        sub    ecx, ecx
loc_1_0CF4:
        lea    edx, [edi + ecx]
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     loc_1_0CF4
        mov    al, byte ptr [esi + 0x6f]
        or     al, 8
        mov    dx, bx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 0x6e]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x49]
        mov    al, byte ptr [esi + 0x6e]
        or     al, 0x80
        lea    edx, [edi + 6]
        out    dx, al
        lea    ecx, [edi + 7]
        mov    al, byte ptr [esi + 0x6f]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D44:
        lea    edx, [edi + ebx]
        add    dx, 8
        mov    al, byte ptr [esi + ebx + 0x71]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0D44
        mov    al, byte ptr [esi + 0x6f]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 0x6e]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0D70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x49]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_0EE6
        mov    byte ptr [esi + 0x6f2], 1
        inc    dword ptr [esi + 0x8d]
        sub    edi, edi
        sub    ebx, ebx
loc_1_0DBA:
        mov    dx, word ptr [esi + 0x49]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x49]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    loc_1_0DD4
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_0E4C
loc_1_0DD4:
        test   byte ptr [ebp - 2], 0x81
        je     loc_1_0DF2
        test   byte ptr [ebp - 2], 1
        je     loc_1_0DE6
        inc    dword ptr [esi + 0x6c9]
loc_1_0DE6:
        push   esi
        call   sub_1_0EF0
        push   esi
        call   sub_1_1790
loc_1_0DF2:
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_0E00
        cmp    byte ptr [esi + 0xa5], bl
        je     loc_1_0E42
loc_1_0E00:
        mov    dx, word ptr [esi + 0x49]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 0x91], ebx
        cmp    byte ptr [esi + 0xa5], bl
        je     loc_1_0E25
        mov    byte ptr [esi + 0xa6], 2
        mov    byte ptr [esi + 0xa5], bl
        jmp    loc_1_0E2E
loc_1_0E25:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0xa6], al
loc_1_0E2E:
        mov    byte ptr [esi + 0x717], bl
        cmp    dword ptr [esi + 0xa1], ebx
        je     loc_1_0E42
        push   esi
        call   sub_1_1110
loc_1_0E42:
        inc    edi
        cmp    edi, 8
        jl     loc_1_0DBA
loc_1_0E4C:
        cmp    dword ptr [esi + 0x6da], 0
        je     loc_1_0E6D
loc_1_0E55:
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_0E6D
        push   esi
        call   sub_1_0EF0
        cmp    dword ptr [esi + 0x6da], 0
        jne    loc_1_0E55
loc_1_0E6D:
        mov    cx, word ptr [esi + 0x49]
        mov    byte ptr [esi + 0x6f2], 0
        lea    edx, [ecx + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 0x81
        out    dx, al
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_0EC5
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_0EAE
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_0EAE
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_0EC5
loc_1_0EAE:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0EC5:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_0EE6
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_0EE6
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_0EE6
        push   esi
        call   sub_1_1790
loc_1_0EE6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_1_0EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_0F04:
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_108A
        mov    cx, word ptr [esi + 0x49]
        mov    al, 0x81
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 0x40
        jne    loc_1_108A
        inc    dword ptr [esi + 0x91]
        mov    dword ptr [esi + 0x6e2], 0
        push   2
        lea    eax, [esi + 0x6ad]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 0x6af]
        mov    word ptr [esi + 0xab], ax
        cmp    ax, 6
        jb     loc_1_101A
        mov    edi, dword ptr [esi + 0x6d6]
        add    di, 0xe
        cmp    di, ax
        jbe    loc_1_0F76
        mov    di, ax
loc_1_0F76:
        mov    bl, 1
        test   byte ptr [esi + 0xa7], 0x28
        jne    loc_1_0FF0
        push   3
        lea    eax, [esi + 0xad]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0xab], 6
        sub    di, 6
        cmp    dword ptr [esi + 0xaf], -1
        jne    loc_1_0FC0
        cmp    byte ptr [esi + 0xad], 0xff
        jne    loc_1_0FC0
        cmp    byte ptr [esi + 0xae], 0xff
        jne    loc_1_0FC0
        jmp    loc_1_101A
        db  090h
loc_1_0FC0:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0xb3]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0xab], di
        jmp    loc_1_101C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0FF0:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0xad]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0xab], di
        jmp    loc_1_101C
loc_1_101A:
        sub    bl, bl
loc_1_101C:
        or     bl, bl
        je     loc_1_1035
        push   esi
        call   sub_1_13B0
        cmp    eax, 3
        je     loc_1_1035
        inc    dword ptr [esi + 0x6b9]
        mov    byte ptr [ebp - 2], 1
loc_1_1035:
        mov    ax, word ptr [esi + 0xab]
        or     ax, ax
        je     loc_1_106F
        cmp    ax, 8
        ja     loc_1_1065
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    loc_1_106F
loc_1_1065:
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 0xe
        mov    al, 4
        out    dx, al
loc_1_106F:
        cmp    dword ptr [esi + 0x91], 5
        jbe    loc_1_0F04
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 0x91], 0
loc_1_108A:
        cmp    dword ptr [esi + 0x6da], 0
        je     loc_1_10D5
loc_1_1093:
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_10DE
        mov    ebx, dword ptr [esi + 0x6da]
        push   ebx
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x6da], eax
        mov    dword ptr [esi + 0x6e2], ebx
        mov    edi, dword ptr [ebx + 0x28]
        push   esi
        call   sub_1_1210
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x6da], 0
        jne    loc_1_1093
loc_1_10D5:
        cmp    byte ptr [esi + 0x6cd], 0
        je     loc_1_10EA
loc_1_10DE:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_10EA:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_10F9
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0290
loc_1_10F9:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xa1]
        or     ecx, ecx
        jne    loc_1_1144
        push   2
        push   6
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1144:
        mov    al, byte ptr [esi + 0xa6]
        test   al, 0x80
        je     loc_1_1183
        test   al, 4
        je     loc_1_117B
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 4
        in     al, dx
        and    eax, 0xff
        and    eax, 0xfffffff0
        cmp    eax, 0x10
        jne    loc_1_1175
        inc    dword ptr [esi + 0x6c1]
        inc    dword ptr [esi + 0x6b5]
        jmp    loc_1_1189
loc_1_1175:
        inc    dword ptr [esi + 0x6c5]
loc_1_117B:
        inc    dword ptr [esi + 0x6b5]
        jmp    loc_1_1189
loc_1_1183:
        inc    dword ptr [esi + 0x6bd]
loc_1_1189:
        cmp    byte ptr [ecx + 0x2c], 0
        jne    loc_1_11B2
        mov    al, byte ptr [esi + 0xa6]
        and    al, 0x80
        cmp    al, 1
        mov    edi, dword ptr [ecx + 0x28]
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        jmp    loc_1_11D9
loc_1_11B2:
        cmp    dword ptr [esi + 0x6da], 0
        jne    loc_1_11C3
        mov    dword ptr [esi + 0x6da], ecx
        jmp    loc_1_11CC
loc_1_11C3:
        mov    eax, dword ptr [esi + 0x6de]
        mov    dword ptr [eax + 0x24], ecx
loc_1_11CC:
        mov    dword ptr [esi + 0x6de], ecx
        mov    dword ptr [ecx + 0x24], 0
loc_1_11D9:
        mov    dword ptr [esi + 0xa1], 0
        cmp    byte ptr [esi + 0x6cd], 0
        je     loc_1_1200
        push   2
        push   esi
        call   sub_1_3410
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1200:
        push   esi
        call   sub_1_1790
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1210:
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
        jae    loc_1_1240
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1240:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x6d6]
        add    edi, 0xe
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_1254
        mov    edi, dword ptr [ebp - 4]
loc_1_1254:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0xad]
        push   eax
        call   sub_1_12D0
        cmp    edi, 0xe
        jae    loc_1_1290
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0xad]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_1290:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0xbb]
        push   eax
        push   0xe
        lea    eax, [esi + 0xad]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12D0:
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
        je     loc_1_1324
loc_1_12F5:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_1324
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_12F5
loc_1_1324:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1335
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1335:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_1344:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_1353
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_1356
loc_1_1353:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1356:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05BC
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_1392
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_139D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        jmp    loc_1_1344
loc_1_1392:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_139D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 0x6af]
        cmp    eax, 0x5ea
        jbe    loc_1_13D0
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_13D0:
        mov    dword ptr [edx + 0x6b1], eax
        mov    ecx, dword ptr [edx + 0x6d6]
        add    ecx, 0xe
        cmp    ecx, eax
        jbe    loc_1_13E5
        mov    ecx, eax
loc_1_13E5:
        cmp    ecx, 0xe
        jae    loc_1_1409
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0xad]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1409:
        sub    eax, 0xe
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    eax, [edx + 0xbb]
        push   eax
        push   0xe
        lea    eax, [edx + 0xad]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1440:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x6e2], 0
        je     loc_1_14E7
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
        je     loc_1_14BA
loc_1_147A:
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
        mov    eax, dword ptr [esi + 0x6e2]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_12D0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_14BA
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_147A
loc_1_14BA:
        cmp    edi, ebx
        jbe    loc_1_14D5
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_14D5:
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
loc_1_14E7:
        mov    ax, word ptr [esi + 0xab]
        or     ax, ax
        je     loc_1_1522
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [esi + 0x6d6]
        add    eax, esi
        add    eax, 0xbb
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [esi + 0xab], 0
loc_1_1522:
        mov    eax, dword ptr [esi + 0x6b1]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_1557
        cmp    edi, eax
        jbe    loc_1_1550
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
loc_1_1550:
        sub    eax, edi
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_155A
loc_1_1557:
        mov    dword ptr [ebp - 0x18], ecx
loc_1_155A:
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
        je     loc_1_15DB
loc_1_158A:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     loc_1_1597
        mov    ebx, dword ptr [ebp - 8]
loc_1_1597:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 0xad
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05BC
        add    edi, ebx
        sub    dword ptr [ebp - 0xc], ebx
        je     loc_1_15DB
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_15DB
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_158A
loc_1_15DB:
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
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1600:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        add    edi, 0x24
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_1630
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1630:
        cmp    byte ptr [esi + 0x6cd], 0
        je     loc_1_1647
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1647:
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [esi + 0x8d]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], ebx
        lea    eax, [ebp - 0xc]
        push   0
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        call   sub_1_0500
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0524
        mov    ecx, dword ptr [esi + 0x81]
        lea    edx, [esi + 0x7f]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    loc_1_16A0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_16D7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16A0:
        jbe    loc_1_16B0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_16D7
        db  005h, 000h, 000h, 000h, 000h
loc_1_16B0:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    loc_1_16C7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_16D7
loc_1_16C7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_16D7
        mov    dword ptr [ebp - 4], 0
loc_1_16D7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_16F7
        test   byte ptr [ebx + 0x1d], 2
        je     loc_1_16E5
        jmp    loc_1_16F7
loc_1_16E5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    loc_1_16FB
loc_1_16F7:
        mov    byte ptr [edi + 8], 0
loc_1_16FB:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_1712
        mov    eax, dword ptr [esi + 0x9d]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_171B
loc_1_1712:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x99], ecx
loc_1_171B:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x9d], ecx
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_1760
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_174E
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_174E
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_1760
loc_1_174E:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        jmp    loc_1_1781
        db  08Bh, 0FFh
loc_1_1760:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_1781
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_1781
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_1781
        push   esi
        call   sub_1_1790
loc_1_1781:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_1790:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_1936
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
        nop    
loc_1_17B0:
        mov    eax, dword ptr [esi + 0x99]
        cmp    eax, ebx
        je     loc_1_1936
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x99], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0xa1], eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_0500
        cmp    dword ptr [ebp - 0x14], 0x3c
        mov    eax, 0x3c
        jb     loc_1_17F4
        mov    eax, dword ptr [ebp - 0x14]
loc_1_17F4:
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     loc_1_189B
loc_1_1809:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0524
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_1821
        jmp    loc_1_1885
loc_1_1821:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_1853
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        inc    dword ptr [ebp - 0x10]
        shl    ax, 8
        dec    dword ptr [ebp - 0xc]
        or     di, ax
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 8
        cmp    dword ptr [ebp - 0xc], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    loc_1_1853
        jmp    loc_1_1885
loc_1_1853:
        cmp    dword ptr [ebp - 0xc], 1
        jbe    loc_1_186F
        mov    eax, dword ptr [ebp - 0xc]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_186F:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_1885
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_1885:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0548
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_1809
loc_1_189B:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_18B4
        or     di, 0x2000
        mov    edx, dword ptr [esi + 0x49]
        inc    dword ptr [ebp - 0x14]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_18B4:
        cmp    dword ptr [ebp - 0x14], 0x3c
        jae    loc_1_18D6
        mov    eax, 0x3c
        sub    eax, dword ptr [ebp - 0x14]
        shr    eax, 1
        push   eax
        push   0x3940                        ; -> loc_1_3940
        mov    eax, dword ptr [esi + 0x49]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_18D6:
        test   byte ptr [esi + 0xa7], 0x20
        je     loc_1_18E5
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [eax + 0x2c], bl
loc_1_18E5:
        cmp    byte ptr [esi + 0x6cd], bl
        jne    loc_1_1910
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 0xa
        mov    al, 0x81
        out    dx, al
        cmp    dword ptr [esi + 0xa1], ebx
        je     loc_1_17B0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1910:
        mov    eax, dword ptr [ebp - 0x18]
        push   0
        mov    edi, dword ptr [eax + 0x28]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0xa1], 0
        push   3
        push   esi
        call   sub_1_3410
        dec    dword ptr [edi + 0x18]
loc_1_1936:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1940:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x717], 0
        je     loc_1_1990
        cmp    dword ptr [esi + 0xa1], 0
        je     loc_1_19A0
        mov    byte ptr [esi + 0xa5], 1
        mov    byte ptr [esi + 0x717], 0
        push   7
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x10
        push   esi
        call   sub_1_0AA0
        push   esi
        call   sub_1_0A70
        jmp    loc_1_19A0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1990:
        cmp    dword ptr [esi + 0xa1], 0
        je     loc_1_19A0
        mov    byte ptr [esi + 0x717], 1
loc_1_19A0:
        push   0x7d0
        add    esi, 0x6ff
        push   esi
        call   sub_1_04B2
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [esi + 0x51]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi + 0x40], 0
        jne    loc_1_1A33
        or     al, al
        jne    loc_1_1A02
        mov    eax, dword ptr [esi + 0x724]
        shl    eax, 2
        add    esi, 0x718
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        push   eax
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1A02:
        mov    byte ptr [esi + 0x40], 1
        mov    dx, word ptr [esi + 0x51]
        mov    al, 0x10
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 0x40
        je     loc_1_1A33
        mov    edx, dword ptr [esi + 0x51]
        add    dx, 9
        sub    al, al
        out    dx, al
        push   0x32
        call   sub_1_04D0
        mov    ecx, dword ptr [esi + 0x51]
        add    cx, 9
        mov    dx, cx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_1A33:
        mov    edx, dword ptr [esi + 0x51]
        inc    dx
        in     al, dx
        test   al, 8
        je     loc_1_1A59
        push   esi
        call   sub_1_0D70
        or     al, al
        je     loc_1_1A59
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0D90
loc_1_1A59:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1A75
        mov    edx, dword ptr [esi + 0x51]
        add    dx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    byte ptr [esi + 0x40], 0
        mov    dx, word ptr [esi + 0x51]
        mov    al, 0x11
        out    dx, al
loc_1_1A75:
        mov    eax, dword ptr [esi + 0x724]
        push   eax
        add    esi, 0x718
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_1_1A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        mov    word ptr [ebp - 0x14], 9
        mov    word ptr [ebp - 0x12], 9
        mov    dword ptr [ebp - 0x10], 0x3a98 ; -> obj1:0x3A98
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        mov    eax, dword ptr [0x39b0]       ; -> obj1:0x39B0
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_1_1AE1
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1AE1:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x54], 3
        mov    byte ptr [ebp - 0x53], 0
        mov    dword ptr [ebp - 0x52], 0
        mov    dword ptr [ebp - 0x4e], 0x22a0 ; -> loc_1_22A0
        mov    dword ptr [ebp - 0x4a], 0x24d0 ; -> loc_1_24D0
        mov    dword ptr [ebp - 0x46], 0x1600 ; -> loc_1_1600
        mov    dword ptr [ebp - 0x42], 0x1440 ; -> loc_1_1440
        mov    dword ptr [ebp - 0x3e], 0x30d0 ; -> loc_1_30D0
        mov    dword ptr [ebp - 0x3a], 0x2690 ; -> loc_1_2690
        mov    dword ptr [ebp - 0x36], 0x2f40 ; -> loc_1_2F40
        mov    dword ptr [ebp - 0x32], 0x3020 ; -> loc_1_3020
        mov    dword ptr [ebp - 0x2e], 0x1b90 ; -> loc_1_1B90
        mov    dword ptr [ebp - 0x2a], 0x3050 ; -> loc_1_3050
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x26], eax
        lea    eax, [ebp - 0x54]
        mov    dword ptr [ebp - 0x22], ecx
        push   0x36
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1B76
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1B76:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc8
        push   esi
        push   edi
        mov    word ptr [ebp - 0x50], 0xc
        mov    word ptr [ebp - 0x4e], 0xc
        mov    dword ptr [ebp - 0x4c], 0x3ad0 ; -> obj1:0x3AD0
        mov    word ptr [ebp - 0x40], 0x10
        mov    word ptr [ebp - 0x3e], 0x10
        mov    dword ptr [ebp - 0x3c], 0x3b34 ; -> obj1:0x3B34
        mov    word ptr [ebp - 0x48], 9
        mov    word ptr [ebp - 0x46], 9
        mov    dword ptr [ebp - 0x44], 0x3b50 ; -> loc_1_3B50
        mov    word ptr [ebp - 0x38], 0x15
        mov    word ptr [ebp - 0x36], 0x15
        mov    dword ptr [ebp - 0x34], 0x3b74 ; -> loc_1_3B74
        mov    word ptr [ebp - 0x30], 0x10
        mov    word ptr [ebp - 0x2e], 0x10
        mov    dword ptr [ebp - 0x2c], 0x3bb4 ; -> obj1:0x3BB4
        mov    eax, dword ptr [0x39b0]       ; -> obj1:0x39B0
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x728
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_1_1C20
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_1_1C20:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0x1ca
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp - 0xc]
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1DEF
        push   0
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1DE6
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x3c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x3c]
        cmp    eax, 1
        je     loc_1_1C99
        cmp    eax, 2
        je     loc_1_1C99
        cmp    eax, 3
        jne    loc_1_1DE6
loc_1_1C99:
        cmp    eax, 3
        jne    loc_1_1DAC
        mov    word ptr [ebp - 0xc8], 0
        lea    edi, [ebp - 0xc6]
        sub    eax, eax
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0x20
        lea    eax, [ebp - 0xc8]
        push   eax
        push   2
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04A4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1DE6
        mov    ecx, dword ptr [ebp - 0xc4]
        and    ecx, 0xffff
        xor    eax, eax
        mov    ax, word ptr [ebp - 0xc2]
        sub    eax, ecx
        inc    eax
        cmp    eax, 8
        jne    loc_1_1D09
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x45], ecx
loc_1_1D09:
        mov    ecx, dword ptr [ebp - 0xb4]
        and    ecx, 0xffff
        xor    eax, eax
        mov    ax, word ptr [ebp - 0xb2]
        sub    eax, ecx
        inc    eax
        cmp    eax, 0x20
        jne    loc_1_1D2C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x41], ecx
loc_1_1D2C:
        mov    word ptr [ebp - 0xa8], 0
        lea    edi, [ebp - 0xa6]
        sub    eax, eax
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0x20
        lea    eax, [ebp - 0xa8]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04A4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1DE6
        mov    eax, dword ptr [ebp - 0xa0]
        sub    eax, dword ptr [ebp - 0xa4]
        inc    eax
        cmp    eax, 0x1000
        jne    loc_1_1D8C
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0xa4]
        mov    dword ptr [ecx + 0x55], eax
loc_1_1D8C:
        mov    eax, dword ptr [ebp - 0x90]
        sub    eax, dword ptr [ebp - 0x94]
        inc    eax
        cmp    eax, 0x4000
        jne    loc_1_1DAC
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0x94]
        mov    dword ptr [ecx + 0x59], eax
loc_1_1DAC:
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1E10
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1E10
loc_1_1DE6:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0560
loc_1_1DEF:
        push   0
        push   0x728
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_1E10:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x65], eax
        lea    eax, [ebp - 0x38]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1E50
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x95], eax
        jmp    loc_1_1E5D
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x95], 0x10
loc_1_1E5D:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1E99
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_1_1E99
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, ecx
        add    edx, 0x7f
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1E99:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055A
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1EC5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x724], 5
        jmp    loc_1_1ED4
loc_1_1EC5:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x724], eax
loc_1_1ED4:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0560
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1FB0
        or     eax, eax
        jne    loc_1_1DEF
        sub    eax, eax
        sub    ecx, ecx
        cmp    ecx, 7
        je     loc_1_1F15
        cmp    eax, 8
        je     loc_1_1F39
        cmp    eax, 9
        je     loc_1_1F60
        cmp    eax, 0xa
        je     loc_1_1F84
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F15:
        push   7
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000003
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F39:
        push   8
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000005
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_1F60:
        push   9
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa00000c
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F84:
        push   0xa
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x39b0]       ; -> obj1:0x39B0
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x26
        push   eax
        call   sub_1_04D6
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_2213
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xa], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x16], 1
        mov    eax, dword ptr [esi + 0x41]
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + 8]
        mov    dword ptr [ecx + 0x1a], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1e], 0x20
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 0x49]
        mov    dword ptr [ecx + 0x22], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   edi
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0x26
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_04DC
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2213
        mov    dword ptr [esi + 0x6f3], 0
        mov    dword ptr [esi + 0x6f7], 0x3620 ; -> loc_1_3620
        mov    dword ptr [esi + 0x6fb], esi
        push   0x1000
        mov    eax, dword ptr [esi + 0x55]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0x5d]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05A2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2091
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_21FD
loc_1_2091:
        push   esi
        call   sub_1_06D0
        or     al, al
        jne    loc_1_20B9
        push   0
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_21FD
loc_1_20B9:
        push   esi
        call   sub_1_0810
        lea    ecx, [esi + 0x7f]
        cmp    byte ptr [ecx], 0
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x80], 0
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x81], 0
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x82], 0
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x83], 0
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x84], 0
        jne    loc_1_2108
        mov    eax, dword ptr [esi + 0x79]
        lea    edx, [esi + 0x79]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_2108:
        lea    ebx, [esi + 0x24]
        push   ebx
        push   esi
        push   ecx
        push   0x3610                        ; -> loc_1_3610
        push   0x3590                        ; -> loc_1_3590
        push   0x3510                        ; -> loc_1_3510
        mov    eax, dword ptr [esi + 0x95]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_1_2150
        push   3
        push   2
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_21FD
        db  090h
loc_1_2150:
        mov    eax, dword ptr [esi + 0x3c]
        cmp    eax, 1
        je     loc_1_215D
        cmp    eax, 2
        jne    loc_1_2163
loc_1_215D:
        push   esi
        call   sub_1_0750
loc_1_2163:
        push   esi
        call   sub_1_08D0
        or     al, al
        jne    loc_1_2190
        mov    dword ptr [ebp - 4], 0xc0010011
        push   1
        push   2
        push   2
        push   0xaa000003
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        jmp    loc_1_21F5
        db  08Dh, 064h, 024h, 000h
loc_1_2190:
        push   esi
        call   sub_1_0AC0
        or     al, al
        jne    loc_1_21B9
        mov    dword ptr [ebp - 4], 0xc0010011
        push   1
        push   2
        push   2
        push   0xaa000003
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        jmp    loc_1_21F5
loc_1_21B9:
        push   1
        push   1
        mov    eax, dword ptr [esi + 0x65]
        push   eax
        push   eax
        push   0xd90                         ; -> sub_1_0D90
        push   esi
        push   0xd70                         ; -> sub_1_0D70
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04BE
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2220
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05A8
        add    esp, 0xc
loc_1_21F5:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_1_21FD:
        push   esi
        call   sub_1_06B0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_2213:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_1_2220:
        mov    dword ptr [esi + 0x70b], 0
        mov    dword ptr [esi + 0x70f], 0xd90 ; -> sub_1_0D90
        lea    eax, [esi + 0x6ff]
        mov    dword ptr [esi + 0x713], esi
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x703], 0x1940 ; -> loc_1_1940
        mov    dword ptr [esi + 0x707], esi
        push   0x7d0
        push   eax
        call   sub_1_04B2
        lea    ecx, [esi + 0x718]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esi + 0x724]
        shl    eax, 2
        mov    dword ptr [esi + 0x720], esi
        lea    eax, [eax + eax*4]
        mov    dword ptr [esi + 0x71c], 0x19c0 ; -> loc_1_19C0
        lea    eax, [eax + eax*4]
        push   eax
        push   ecx
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_22A0:
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
loc_1_22C3:
        or     edx, edx
        je     loc_1_22D2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_22C3
        mov    dword ptr [ecx], edx
loc_1_22D2:
        cmp    dword ptr [ecx], -1
        jne    loc_1_22E4
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_22E4:
        mov    eax, dword ptr [0x39b0]       ; -> obj1:0x39B0
        push   eax
        push   0
        push   0x71
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D6
        or     eax, eax
        je     loc_1_2322
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2322:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x8d]
        cmp    dword ptr [esi + 0x85], 0
        jne    loc_1_2343
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_2343
        push   esi
        call   sub_1_0A70
loc_1_2343:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x85]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x85], eax
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_2440
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        mov    edi, dword ptr [ebp + 0x1c]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0170
        or     al, al
        je     loc_1_2440
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0xee
        mov    dword ptr [esi + 0x6d6], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_2412
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_23FA
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_23FA
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_2412
loc_1_23FA:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2412:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_2433
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_2433
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_2433
        push   esi
        call   sub_1_1790
loc_1_2433:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_1_2440:
        dec    dword ptr [esi + 0x8d]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x85], eax
        push   0x71
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04DC
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05A8
        add    esp, 0x14
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0x85]
        cmp    esi, edx
        je     loc_1_24B3
loc_1_24A3:
        mov    eax, dword ptr [esi + 0x14]
        cmp    edx, eax
        jae    loc_1_24AC
        mov    edx, eax
loc_1_24AC:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_24A3
loc_1_24B3:
        or     edx, edx
        jne    loc_1_24BC
        mov    edx, 0xee
loc_1_24BC:
        mov    dword ptr [ecx + 0x6d6], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_24F3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_24F3:
        inc    dword ptr [esi + 0x8d]
        inc    dword ptr [ebx + 0x18]
        mov    ecx, dword ptr [esi + 0x85]
        cmp    ecx, ebx
        jne    loc_1_2511
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x85], eax
        jmp    loc_1_2527
loc_1_2511:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_251E
loc_1_2516:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_2516
loc_1_251E:
        lea    eax, [ebx + 0xc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_2527:
        push   esi
        call   sub_1_0A10
        or     eax, eax
        jne    loc_1_2560
        mov    eax, dword ptr [esi + 0x24]
        push   0
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [esi + 0x24]
        push   0x71
        mov    dword ptr [eax + 0x10], 0
        push   ebx
        call   sub_1_04DC
        dec    dword ptr [esi + 0x8d]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2560:
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0240
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2598
        mov    eax, dword ptr [ebx + 0x18]
        cmp    eax, 2
        je     loc_1_2590
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        mov    edi, 0x103
        jmp    loc_1_25C4
        db  08Dh, 064h, 024h, 000h
loc_1_2590:
        sub    eax, 2
        mov    dword ptr [ebx + 0x18], eax
        jmp    loc_1_25C4
loc_1_2598:
        cmp    edi, 0x103
        jne    loc_1_25B0
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        jmp    loc_1_25C4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_25B0:
        cmp    edi, 0xc001000e
        jne    loc_1_25C1
        mov    byte ptr [ebx + 0x1c], 1
        mov    edi, 0x103
loc_1_25C1:
        dec    dword ptr [ebx + 0x18]
loc_1_25C4:
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_2600
        mov    eax, dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x6d6], eax
        jne    loc_1_25DB
        push   esi
        call   sub_1_2490
loc_1_25DB:
        push   0
        push   0x71
        push   ebx
        call   sub_1_04DC
        cmp    dword ptr [esi + 0x85], 0
        jne    loc_1_2614
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_2614
        push   esi
        call   sub_1_0AA0
        jmp    loc_1_2614
        db  090h
loc_1_2600:
        mov    eax, dword ptr [esi + 0x89]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0x89], ebx
        mov    edi, 0x103
loc_1_2614:
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_2657
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_263E
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_263E
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_2657
loc_1_263E:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2657:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_2678
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_2678
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_2678
        push   esi
        call   sub_1_1790
loc_1_2678:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [edi + 0x8d]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_26B2
        mov    esi, 0xc0010002
        jmp    loc_1_26F5
loc_1_26B2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        jne    loc_1_26C8
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    loc_1_26F5
loc_1_26C8:
        cmp    ecx, 1
        jne    loc_1_26F0
        cmp    byte ptr [edi + 0x6cd], 0
        je     loc_1_26DD
        mov    esi, 0xc001000d
        jmp    loc_1_26F5
loc_1_26DD:
        push   eax
        push   ebx
        push   edi
        call   sub_1_2B80
        mov    esi, eax
        jmp    loc_1_26F5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_26F0:
        mov    esi, 0x10001
loc_1_26F5:
        dec    dword ptr [ebx + 0x18]
        mov    eax, dword ptr [edi + 0x8d]
        dec    eax
        mov    dword ptr [edi + 0x8d], eax
        jne    loc_1_2740
        cmp    byte ptr [edi + 0x6cd], 0
        jne    loc_1_2722
        cmp    dword ptr [edi + 0x6e6], 0
        jne    loc_1_2722
        cmp    dword ptr [edi + 0x89], 0
        je     loc_1_2740
loc_1_2722:
        push   1
        add    edi, 0x6f3
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2740:
        cmp    dword ptr [edi + 0x99], 0
        je     loc_1_2761
        cmp    dword ptr [edi + 0xa1], 0
        jne    loc_1_2761
        cmp    byte ptr [edi + 0x6f2], 0
        jne    loc_1_2761
        push   edi
        call   sub_1_1790
loc_1_2761:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
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
        je     loc_1_2990
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_27E1
        shl    eax, 2
        jmp    dword ptr [eax + 0x2ae4]      ; -> obj1:0x2AE4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27C0:
        cmp    eax, 0x1010102
        je     loc_1_29B0
        cmp    eax, 0x1010103
        je     loc_1_29D0
        cmp    eax, 0x1010104
        je     loc_1_2A90
loc_1_27E1:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_27F0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2805
        mov    esi, 0x3a30                   ; -> loc_1_3A30
        mov    edi, 0x5c
        jmp    loc_1_2A9C
loc_1_2805:
        mov    esi, 0x39b4                   ; -> loc_1_39B4
        mov    edi, 0x7c
        jmp    loc_1_2A9C
loc_1_2814:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x6cd], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_1_282E
        mov    dword ptr [ebp - 0x14], 0
loc_1_282E:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2A9C
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_2850:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2860:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2870:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_2880:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0xa1], 1
        sbb    eax, eax
        and    eax, 0x5ee
        jmp    loc_1_2A99
loc_1_2896:
        mov    dword ptr [ebp - 8], 0x2000
        jmp    loc_1_2A9C
loc_1_28A2:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_2A9C
        db  08Bh, 0FFh
loc_1_28B0:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2A9C
        db  08Dh, 064h, 024h, 000h
loc_1_28C0:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x79
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05BC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    loc_1_2A9C
loc_1_28E6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x3c]
        cmp    eax, 2
        je     loc_1_2905
        cmp    eax, 3
        je     loc_1_2914
        mov    esi, 0x3c78                   ; -> loc_1_3C78
        mov    edi, 0x23
        jmp    loc_1_2A9C
loc_1_2905:
        mov    esi, 0x3c1c                   ; -> loc_1_3C1C
        mov    edi, 0x29
        jmp    loc_1_2A9C
loc_1_2914:
        mov    esi, 0x3c48                   ; -> obj1:0x3C48
        mov    edi, 0x2c
        jmp    loc_1_2A9C
loc_1_2923:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2937
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_2A99
loc_1_2937:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_1_2A99
loc_1_2945:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2959
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x6d6]
        jmp    loc_1_2A99
loc_1_2959:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        jmp    loc_1_2A99
loc_1_2964:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_1_2A9C
loc_1_2977:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2A9C
loc_1_2983:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_2A9C
        db  090h
loc_1_2990:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x79
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2A9C
        db  090h
loc_1_29B0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x7f
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05BC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2A9C
        db  090h
loc_1_29D0:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_2A35
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_2A00
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2A9C
        db  08Dh, 049h, 000h
loc_1_2A00:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebx + 0x24]
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
        je     loc_1_2A9C
        jmp    loc_1_2A9C
loc_1_2A35:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 0x24]
        push   eax
        call   sub_2_0660
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_2A60
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2A9C
loc_1_2A60:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_2A9C
        db  090h
loc_1_2A90:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x95]
loc_1_2A99:
        mov    dword ptr [ebp - 8], eax
loc_1_2A9C:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2AD8
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_2AD8
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_2AC8
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], edi
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_2AC8:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05BC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_2AD8:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
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
        dd  loc_1_28E6               ; fixup
        dd  loc_1_2923               ; fixup
        dd  loc_1_2945               ; fixup
        dd  loc_1_2964               ; fixup
        dd  loc_1_2977               ; fixup
        dd  loc_1_27E1               ; fixup
        dd  loc_1_2983               ; fixup
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
        je     loc_1_2CC7
        cmp    eax, 0x1010103
        je     loc_1_2CF4
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D35
loc_1_2BD7:
        cmp    edi, 4
        je     loc_1_2BF4
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 4
        jmp    loc_1_2D35
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
        jmp    loc_1_2D35
loc_1_2C21:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2D50
        jmp    loc_1_2D33
loc_1_2C38:
        cmp    edi, 4
        je     loc_1_2C55
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D35
loc_1_2C55:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05BC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0xfffffffe
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_2CC0
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x6d6]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_2C96
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x6d6], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_2D35
loc_1_2C96:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x14], eax
        jne    loc_1_2CB7
        mov    eax, dword ptr [ecx + 0x14]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_2CB7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        push   edx
        call   sub_1_2490
        jmp    loc_1_2D35
loc_1_2CB7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_2D35
        db  090h
loc_1_2CC0:
        mov    ebx, 0xc0010014
        jmp    loc_1_2D35
loc_1_2CC7:
        cmp    edi, 4
        je     loc_1_2CE1
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D35
loc_1_2CE1:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x1d
        push   ecx
        call   sub_1_05BC
        sub    ebx, ebx
        jmp    loc_1_2D35
loc_1_2CF4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_1_2D18
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2D35
loc_1_2D18:
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
        call   sub_1_2DB0
loc_1_2D33:
        mov    ebx, eax
loc_1_2D35:
        or     ebx, ebx
        jne    loc_1_2D43
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_2D43:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2D50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6cd], 0
        jne    loc_1_2D9A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2D90
        inc    dword ptr [esi + 0x18]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    eax, dword ptr [ecx + 0x24]
        push   eax
        call   sub_2_05E0
        dec    dword ptr [esi + 0x18]
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D90:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_2D9A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6cd], 0
        jne    loc_1_2DFA
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2DF0
        inc    dword ptr [esi + 0x18]
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    eax, dword ptr [ecx + 0x24]
        push   eax
        call   sub_2_0300
        dec    dword ptr [esi + 0x18]
        pop    esi
        pop    ebp
        ret    0x14
        db  090h
loc_1_2DF0:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_1_2DFA:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E10:
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
        jne    loc_1_2F26
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2E96
        je     loc_1_2EE0
        cmp    eax, 0x20101
        je     loc_1_2EB2
        cmp    eax, 0x20102
        je     loc_1_2EC0
        cmp    eax, 0x20103
        je     loc_1_2EC8
        cmp    eax, 0x20104
        je     loc_1_2ED0
        mov    edi, 0xc0010017
        jmp    loc_1_2F09
loc_1_2E96:
        cmp    eax, 0x1020101
        je     loc_1_2EE8
        cmp    eax, 0x1020102
        je     loc_1_2EF1
        cmp    eax, 0x1020103
        je     loc_1_2F00
        mov    edi, 0xc0010017
        jmp    loc_1_2F09
loc_1_2EB2:
        mov    eax, dword ptr [ebx + 0x6b5]
        jmp    loc_1_2F06
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2EC0:
        mov    eax, dword ptr [ebx + 0x6b9]
        jmp    loc_1_2F06
loc_1_2EC8:
        mov    eax, dword ptr [ebx + 0x6bd]
        jmp    loc_1_2F06
loc_1_2ED0:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2F09
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EE0:
        mov    eax, dword ptr [ebx + 0x6c9]
        jmp    loc_1_2F06
loc_1_2EE8:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2F09
loc_1_2EF1:
        mov    eax, dword ptr [ebx + 0x6c1]
        jmp    loc_1_2F06
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2F00:
        mov    eax, dword ptr [ebx + 0x6c5]
loc_1_2F06:
        mov    dword ptr [ebp - 4], eax
loc_1_2F09:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_2F26
        or     edi, edi
        jne    loc_1_2F26
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05BC
        add    dword ptr [ebp - 8], 4
loc_1_2F26:
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
loc_1_2F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2F72
        cmp    eax, 0x20101
        jae    loc_1_2F93
        cmp    eax, 0x10101
        jb     loc_1_2F8E
        cmp    eax, 0x10111
        jbe    loc_1_2F93
        cmp    eax, 0x10113
        je     loc_1_2F93
        jmp    loc_1_2F8E
loc_1_2F72:
        cmp    eax, 0x1010101
        jb     loc_1_2F8E
        cmp    eax, 0x1010104
        jbe    loc_1_2F93
        cmp    eax, 0x1020101
        jb     loc_1_2F8E
        cmp    eax, 0x1020103
        jbe    loc_1_2F93
loc_1_2F8E:
        mov    esi, 0xc0010017
loc_1_2F93:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x8d]
        or     esi, esi
        jne    loc_1_2FA9
        push   ecx
        push   edi
        call   sub_1_2E10
        mov    esi, eax
loc_1_2FA9:
        mov    eax, dword ptr [edi + 0x8d]
        dec    eax
        mov    dword ptr [edi + 0x8d], eax
        jne    loc_1_2FF0
        cmp    byte ptr [edi + 0x6cd], 0
        jne    loc_1_2FD3
        cmp    dword ptr [edi + 0x6e6], 0
        jne    loc_1_2FD3
        cmp    dword ptr [edi + 0x89], 0
        je     loc_1_2FF0
loc_1_2FD3:
        push   1
        add    edi, 0x6f3
        push   edi
        call   sub_1_04B2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FF0:
        cmp    dword ptr [edi + 0x99], 0
        je     loc_1_3011
        cmp    dword ptr [edi + 0xa1], 0
        jne    loc_1_3011
        cmp    byte ptr [edi + 0x6f2], 0
        jne    loc_1_3011
        push   edi
        call   sub_1_1790
loc_1_3011:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0420
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3050:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x6ff]
        push   eax
        call   sub_1_04B8
        push   0x2625a0
        call   sub_1_04D0
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x718]
        push   eax
        call   sub_1_04B8
        push   0x2625a0
        call   sub_1_04D0
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0120
        lea    eax, [esi + 0xc]
        push   eax
        call   sub_1_04C4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0438
        push   0
        push   0x728
        push   esi
        call   sub_1_04DC
        push   esi
        call   sub_1_06B0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_30D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x1c], 0
        je     loc_1_30F3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_30F3:
        inc    dword ptr [esi + 0x18]
        inc    dword ptr [edi + 0x8d]
        cmp    byte ptr [edi + 0x6cd], 0
        je     loc_1_3120
        dec    dword ptr [esi + 0x18]
        dec    dword ptr [edi + 0x8d]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_3120:
        mov    ebx, dword ptr [edi + 0x85]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_1_3153
loc_1_3131:
        inc    dword ptr [ebx + 0x18]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010004
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 0xc]
        dec    dword ptr [ebx + 0x18]
        mov    ebx, eax
        or     eax, eax
        jne    loc_1_3131
loc_1_3153:
        mov    byte ptr [edi + 0x6cd], 1
        mov    dword ptr [edi + 0x6ce], 0
        mov    dword ptr [edi + 0x6d2], esi
        push   edi
        call   sub_1_31F0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 0x18]
        mov    eax, dword ptr [edi + 0x8d]
        dec    eax
        mov    dword ptr [edi + 0x8d], eax
        jne    loc_1_31C0
        cmp    byte ptr [edi + 0x6cd], 0
        jne    loc_1_31A0
        cmp    dword ptr [edi + 0x6e6], 0
        jne    loc_1_31A0
        cmp    dword ptr [edi + 0x89], 0
        je     loc_1_31C0
loc_1_31A0:
        push   1
        add    edi, 0x6f3
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
loc_1_31C0:
        cmp    dword ptr [edi + 0x99], 0
        je     loc_1_31E1
        cmp    dword ptr [edi + 0xa1], 0
        jne    loc_1_31E1
        cmp    byte ptr [edi + 0x6f2], 0
        jne    loc_1_31E1
        push   edi
        call   sub_1_1790
loc_1_31E1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x8d]
        cmp    dword ptr [esi + 0x6e6], 0
        je     loc_1_323F
loc_1_3207:
        mov    ecx, dword ptr [esi + 0x6e6]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x6e6], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_1_3233
        cmp    eax, 8
        je     loc_1_3233
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_3233:
        dec    dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_3207
loc_1_323F:
        cmp    dword ptr [esi + 0xa1], 0
        je     loc_1_3263
        mov    dword ptr [esi + 0x6ce], 2
        dec    dword ptr [esi + 0x8d]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3263:
        dec    dword ptr [esi + 0x8d]
        push   esi
        call   sub_1_3280
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3280:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x8d]
        inc    ecx
        mov    dword ptr [eax + 0x8d], ecx
        cmp    dword ptr [eax + 0xa1], 0
        je     loc_1_32B5
        mov    dword ptr [eax + 0x6ce], 3
        dec    dword ptr [eax + 0x8d]
        mov    eax, 0x103
        pop    ebp
        ret    4
loc_1_32B5:
        dec    ecx
        mov    dword ptr [eax + 0x8d], ecx
        push   eax
        call   sub_1_32D0
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x8d]
        mov    ecx, dword ptr [esi + 0xa1]
        or     ecx, ecx
        je     loc_1_330A
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0xa1], 0
loc_1_330A:
        mov    edx, dword ptr [esi + 0x99]
        or     edx, edx
        je     loc_1_333E
loc_1_3314:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x99], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x99]
        or     edx, edx
        jne    loc_1_3314
loc_1_333E:
        mov    edx, dword ptr [esi + 0x6da]
        or     edx, edx
        je     loc_1_3372
loc_1_3348:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x6da], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x6da]
        or     edx, edx
        jne    loc_1_3348
loc_1_3372:
        cmp    byte ptr [esi + 0x6f2], 0
        je     loc_1_3393
        add    eax, 0
loc_1_3380:
        push   0x2710
        call   sub_1_04D0
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_3380
loc_1_3393:
        push   esi
        call   sub_1_0B90
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x6cd], 0
        mov    edi, dword ptr [esi + 0x85]
        or     edi, edi
        je     loc_1_33F6
        sub    ebx, ebx
loc_1_33B8:
        inc    dword ptr [edi + 0x18]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_33CF
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_33CF:
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
        dec    dword ptr [edi + 0x18]
        mov    edi, eax
        cmp    eax, ebx
        jne    loc_1_33B8
loc_1_33F6:
        dec    dword ptr [esi + 0x8d]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3410:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x6ce], eax
        jne    loc_1_3503
        cmp    eax, 1
        je     loc_1_3499
        cmp    eax, 2
        je     loc_1_34A1
        cmp    eax, 3
        je     loc_1_34A9
        mov    eax, dword ptr [ebp - 4]
loc_1_343B:
        cmp    eax, 0x103
        je     loc_1_345A
        push   eax
        mov    eax, dword ptr [esi + 0x6d2]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 0x6d2]
        dec    dword ptr [eax + 0x18]
loc_1_345A:
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_34E2
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_3484
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_3484
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_34E2
loc_1_3484:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3499:
        push   esi
        call   sub_1_31F0
        jmp    loc_1_343B
loc_1_34A1:
        push   esi
        call   sub_1_3280
        jmp    loc_1_343B
loc_1_34A9:
        mov    eax, dword ptr [esi + 0x8d]
        inc    eax
        mov    dword ptr [esi + 0x8d], eax
        cmp    dword ptr [esi + 0xa1], 0
        je     loc_1_34D0
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_34D0:
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        push   esi
        call   sub_1_32D0
        jmp    loc_1_343B
loc_1_34E2:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_3503
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_3503
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_3503
        push   esi
        call   sub_1_1790
loc_1_3503:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3510:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_3523
        lea    edx, [eax + 0x49]
loc_1_3523:
        cmp    byte ptr [ecx + 0x6cd], 0
        je     loc_1_3535
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_3535:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6e6], 0
        jne    loc_1_3570
        mov    dword ptr [ecx + 0x6ea], edx
        mov    dword ptr [ecx + 0x6e6], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_3570:
        mov    eax, dword ptr [ecx + 0x6ea]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6ea], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3590:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_35A3
        lea    edx, [eax + 0x21]
loc_1_35A3:
        cmp    byte ptr [ecx + 0x6cd], 0
        je     loc_1_35B5
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_35B5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6e6], 0
        jne    loc_1_35F0
        mov    dword ptr [ecx + 0x6ea], edx
        mov    dword ptr [ecx + 0x6e6], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_35F0:
        mov    eax, dword ptr [ecx + 0x6ea]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6ea], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3610:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x18]
        ret    4
        db  08Dh, 049h, 000h
loc_1_3620:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x8d]
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_36E4
        cmp    dword ptr [esi + 0x6ee], 0
        jne    loc_1_36E4
        lea    esp, [esp]
loc_1_3650:
        mov    edi, dword ptr [esi + 0x6e6]
        or     edi, edi
        je     loc_1_36BC
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x6e6], eax
        cmp    dword ptr [esi + 0x6ea], edi
        jne    loc_1_3674
        mov    dword ptr [esi + 0x6ea], 0
loc_1_3674:
        mov    dword ptr [esi + 0x6ee], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_3690
        cmp    eax, 0xa
        je     loc_1_3690
        push   esi
        call   sub_1_37D0
        jmp    loc_1_3696
        db  090h
loc_1_3690:
        push   esi
        call   sub_1_3830
loc_1_3696:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_36B0
        cmp    ecx, 4
        je     loc_1_36B0
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_36B0:
        dec    dword ptr [ebx + 0x18]
        cmp    byte ptr [esi + 0x6cd], 0
        je     loc_1_3650
loc_1_36BC:
        mov    dword ptr [esi + 0x6ee], 0
        cmp    byte ptr [esi + 0x6cd], 0
        je     loc_1_36E4
        dec    dword ptr [esi + 0x8d]
        push   1
        push   esi
        call   sub_1_3410
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_36E4:
        mov    edi, dword ptr [esi + 0x89]
        or     edi, edi
        je     loc_1_3760
        sub    ebx, ebx
loc_1_36F0:
        cmp    dword ptr [edi + 0x18], 0
        jbe    loc_1_3700
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_3744
        db  08Dh, 049h, 000h
loc_1_3700:
        mov    eax, dword ptr [esi + 0x6d6]
        cmp    dword ptr [edi + 0x14], eax
        jne    loc_1_3711
        push   esi
        call   sub_1_2490
loc_1_3711:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_3730
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_3736
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3730:
        mov    dword ptr [esi + 0x89], eax
loc_1_3736:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x71
        push   eax
        call   sub_1_04DC
loc_1_3744:
        or     edi, edi
        jne    loc_1_36F0
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_3760
        cmp    dword ptr [esi + 0x85], 0
        jne    loc_1_3760
        push   esi
        call   sub_1_0AA0
loc_1_3760:
        mov    eax, dword ptr [esi + 0x8d]
        dec    eax
        mov    dword ptr [esi + 0x8d], eax
        jne    loc_1_37A0
        cmp    byte ptr [esi + 0x6cd], 0
        jne    loc_1_378A
        cmp    dword ptr [esi + 0x6e6], 0
        jne    loc_1_378A
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_37A0
loc_1_378A:
        push   1
        add    esi, 0x6f3
        push   esi
        call   sub_1_04B2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  090h
loc_1_37A0:
        cmp    dword ptr [esi + 0x99], 0
        je     loc_1_37C1
        cmp    dword ptr [esi + 0xa1], 0
        jne    loc_1_37C1
        cmp    byte ptr [esi + 0x6f2], 0
        jne    loc_1_37C1
        push   esi
        call   sub_1_1790
loc_1_37C1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_37D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        test   al, 0x20
        mov    dword ptr [esi + 0xa7], eax
        je     loc_1_37F0
        lea    eax, [esi + 0x70]
        mov    byte ptr [eax], 3
        jmp    loc_1_3819
        db  090h
loc_1_37F0:
        test   al, 4
        push   esi
        je     loc_1_3800
        call   sub_1_0CD0
        jmp    loc_1_3805
        db  08Dh, 064h, 024h, 000h
loc_1_3800:
        call   sub_1_3830
loc_1_3805:
        cmp    dword ptr [esi + 0xa7], 0
        lea    eax, [esi + 0x70]
        je     loc_1_3816
        mov    byte ptr [eax], 2
        jmp    loc_1_3819
loc_1_3816:
        mov    byte ptr [eax], 0
loc_1_3819:
        mov    edx, dword ptr [esi + 0x49]
        add    dx, 5
        mov    al, byte ptr [eax]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C50
        push   esi
        call   sub_1_0D20
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 02Fh, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 018h, 019h, 01Ah, 01Bh
        db  01Ch, 01Dh, 01Eh, 01Fh, 020h, 021h, 022h, 023h, 024h, 025h, 026h, 027h, 038h, 039h, 03Ah, 03Bh
        db  03Ch, 03Dh, 03Eh, 03Fh, 040h, 041h, 042h, 043h, 044h, 045h, 046h, 047h, 058h, 059h, 05Ah, 05Bh
        db  05Ch, 05Dh, 05Eh, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 02Fh, 0A0h, 0A1h, 0A2h, 0A3h, 0A4h, 0A5h, 0A6h, 0A7h, 0B8h, 0B9h, 0BAh, 0BBh
        db  0BCh, 0BDh, 0BEh, 00Fh, 0C0h, 0C1h, 0C2h, 0C3h, 0C4h, 0C5h, 0C6h, 0C7h, 0D8h, 0D9h, 0DAh, 0DBh
        db  0DCh, 0DDh, 0DEh, 0DFh, 0E0h, 0E1h, 0E2h, 0E3h, 0E4h, 0E5h, 0E6h, 0E7h, 0F8h, 0F9h, 0FAh, 0FBh
        db  0FCh, 0FDh, 0FEh, 000h, 000h, 000h
loc_1_3930:
        mov    al, 0x38
        add    byte ptr [eax], al
        dd  obj1:0x38F0              ; fixup
loc_1_3938:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x6d6f
loc_1_3940:
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
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x45206d6f
        je     loc_1_39EE
        jb     loc_1_39F7
        je     0x39b7
        dec    ebp
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_1_3A03
        jb     0x39cc
        and    byte ptr [esi + 0x31], dh
        xor    byte ptr cs:[eax], dh
        and    byte ptr [eax], ch
        xor    byte ptr [ecx], bh
        xor    dword ptr [ecx + edi], esi
        xor    al, 0x29
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_39B4:
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
loc_1_39EE:
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
loc_1_39F7:
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
loc_1_3A03:
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
loc_1_3A30:
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
        inc    ebx
        dec    ebp
        xor    cl, byte ptr [esi + 0x44]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4d], al
        xor    cl, byte ptr [esi + 0x44]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x45], al
        dec    ebp
        sub    eax, 0x76204949
        xor    dword ptr [esi], ebp
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        inc    ebp
        dec    ebp
        sub    eax, 0x76204949
        xor    dword ptr [esi], ebp
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        popal  
        jo     0x3b3d
        jb     0x3b20
        jns    loc_1_3B3E
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     0x3b49
        jb     loc_1_3B2C
        jns    0x3b4a
        add    byte ptr gs:[ecx + 0x6f], cl
        inc    edx
        popal  
        jae    loc_1_3B47
        inc    ecx
        jb     loc_1_3B4C
        jae    loc_1_3B5C
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], cl
        inc    edx
        popal  
        jae    loc_1_3B57
        inc    ecx
        jb     loc_1_3B5C
        jae    0x3b6c
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_3B7B
        inc    edx
        popal  
        jae    loc_1_3B6B
        inc    ecx
        jb     0x3b70
        jae    loc_1_3B80
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_3B8F
        inc    edx
        popal  
        jae    0x3b7f
        inc    ecx
        jb     0x3b84
        jae    0x3b94
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_3B8D
        jb     0x3b9c
        jne    0x3b9c
loc_1_3B2C:
        je     loc_1_3B7C
        jne    0x3b9d
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x6e], cl
        je     0x3b9d
        jb     0x3bac
        jne    0x3bac
        je     loc_1_3B8C
loc_1_3B3E:
        jne    0x3bad
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x52], cl
        push   ecx
loc_1_3B47:
        dec    esp
        jbe    0x3bb0
        insb   byte ptr es:[edi], dx
loc_1_3B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3B50:
        dec    ecx
        push   edx
        push   ecx
        dec    esp
        jbe    0x3bbc
loc_1_3B57:
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3B5C:
        dec    ebp
        popal  
        js     loc_1_3BC9
        insd   dword ptr es:[edi], dx
        jne    0x3bd0
        dec    ebp
        jne    0x3bd2
        je     loc_1_3BD1
        arpl   word ptr [ecx + 0x73], sp
loc_1_3B6B:
        je     0x3bb9
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_3B74:
        dec    ebp
        popal  
        js     loc_1_3BE1
        insd   dword ptr es:[edi], dx
        jne    loc_1_3BE8
loc_1_3B7B:
        dec    ebp
loc_1_3B7C:
        jne    loc_1_3BEA
        je     loc_1_3BE9
loc_1_3B80:
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_3BD1
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_3B8C:
        inc    ebx
loc_1_3B8D:
        dec    edi
        dec    ebp
loc_1_3B8F:
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        push   eax
        outsd  dx, dword ptr [esi]
        jb     0x3c0f
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        push   eax
        outsd  dx, dword ptr [esi]
        jb     0x3c17
        add    byte ptr [eax + 0x6f], dl
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x65746e49
        jb     0x3c27
        popal  
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax + 0x6f], dl
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x67], 0x65746e49
        jb     loc_1_3C37
        popal  
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        inc    ebx
        inc    edx
loc_1_3BC9:
        popal  
        jae    loc_1_3C31
        inc    ecx
        jb     loc_1_3BD1
loc_1_3BD1:
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        inc    ebx
        inc    edx
        popal  
        jae    0x3c41
        inc    ecx
        jb     loc_1_3BE1
loc_1_3BE1:
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 0x49], dl
        inc    ebx
loc_1_3BE8:
        inc    edx
loc_1_3BE9:
        popal  
loc_1_3BEA:
        jae    loc_1_3C51
        inc    ecx
        jb     loc_1_3BF1
loc_1_3BF1:
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 0x49], dl
        inc    ebx
        inc    edx
        popal  
        jae    0x3c61
        inc    ecx
        jb     loc_1_3C01
loc_1_3C01:
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x61], dl
        imul   esi, dword ptr [ebx + edx*2 + 0x74], 0x73657461
        add    byte ptr [eax], al
        push   edi
        popal  
        imul   esi, dword ptr [ebx + edx*2 + 0x74], 0x73657461
        add    byte ptr [eax], al
loc_1_3C1C:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x43206d6f
        jb     loc_1_3C8B
        imul   esi, dword ptr fs:[ebx + eax*2 + 0x61], 0x45206472
        je     0x3c99
loc_1_3C31:
        jb     0x3ca2
        je     0x3c62
loc_1_3C37:
        dec    ebp
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x3cb6
        jb     0x3c73
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x69], bl
        jb     0x3caf
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebx + 0x72], al
loc_1_3C51:
        imul   esi, dword ptr fs:[ebx + eax*2 + 0x61], 0x45206472
        je     0x3cc5
        jb     0x3cce
        je     0x3c8e
        dec    ebp
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x49], cl
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x3ce5
        jb     0x3ca2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3C78:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x43206d6f
        jb     0x3ce7
        imul   esi, dword ptr fs:[ebx + eax*2 + 0x61], 0x45206472
loc_1_3C8B:
        je     0x3cf5
        jb     0x3cfe
        je     0x3cb3
        inc    ecx
        popal  
        jo     0x3d0c
        jb     0x3cc9
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x3c9c]       ; -> obj1:0x3C9C
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
        mov    eax, dword ptr [0x3c9c]       ; -> obj1:0x3C9C
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
