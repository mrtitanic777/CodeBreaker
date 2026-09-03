; UBNEI -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 6246 instructions recovered by recursive descent

Declare_Virtual_Device UBNEI, 3, 10, UBNEI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5a32  R-X
loc_1_0000:
        mov    al, byte ptr [0x698]          ; -> obj1:0x0698
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x698]              ; -> obj1:0x0698
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_3930
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
UBNEI_Control:
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
        call   sub_1_050C
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
        call   sub_1_050C
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
sub_1_0480:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_048C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_04E2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_04E8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_04EE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_04F4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_04FA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0500:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0506:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_050C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0530:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0554:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0578:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0584:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_058A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0590:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_0596:
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
sub_1_05D2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_05D8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_05EC:
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
sub_1_060C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_062C
        or     ecx, ecx
        je     loc_1_0642
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0640
loc_1_062C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0640
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0640:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0642:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'UBNEI   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  UBNEI_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
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
loc_1_06C3:
        or     edx, edx
        je     loc_1_06D2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_06C3
        mov    dword ptr [ecx], edx
loc_1_06D2:
        cmp    dword ptr [ecx], -1
        jne    loc_1_06E4
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_06E4:
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0xdb], 0
        je     loc_1_0700
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_1_0700:
        mov    eax, dword ptr [0x4060]       ; -> obj1:0x4060
        push   eax
        push   0
        push   0x25
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_0724
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0724:
        mov    eax, dword ptr [esi + 0xaa]
        cmp    dword ptr [esi + 0x4c], eax
        ja     loc_1_0749
        push   0
        push   0x25
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0749:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        call   sub_2_0170
        or     al, al
        jne    loc_1_0790
        push   0
        push   0x25
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h
loc_1_0790:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x9e]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x42]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [esi + 0x42]
        mov    dword ptr [esi + 0x98], eax
        mov    eax, dword ptr [esi + 0xaa]
        inc    eax
        mov    dword ptr [esi + 0xaa], eax
        cmp    eax, 1
        jne    loc_1_0834
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp - 4]
        add    esi, 0x5c
        push   eax
        push   0x940                         ; -> loc_1_0940
        push   eax
        call   sub_1_1D80
        push   esi
        push   0
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0x850                         ; -> sub_1_0850
        push   eax
        call   sub_1_1D80
        or     al, al
        je     loc_1_0834
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0834:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        call   sub_1_0850
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h
sub_1_0850:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [ecx + 0xae], 0
        je     loc_1_0875
        mov    eax, dword ptr [ecx + 0xec]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [ecx + 0xec], edx
        pop    ebp
        ret    8
loc_1_0875:
        mov    eax, dword ptr [ecx + 0xa6]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [ecx + 0xa6], edx
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_043E
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    byte ptr [ebp + 0x20], 0
        mov    eax, dword ptr [ecx + 8]
        je     loc_1_08F2
        cmp    word ptr [eax + 0x100], 0xa
        jae    loc_1_08E9
        mov    eax, dword ptr [ebp + 0x1c]
        push   0
        mov    dword ptr [eax + 4], ecx
        push   0
        push   8
        push   eax
        push   0x920                         ; -> loc_1_0920
        push   ecx
        call   sub_1_1D80
        or     al, al
        je     loc_1_08E0
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_08E0:
        mov    eax, 0xc0000001
        pop    ebp
        ret    0x1c
loc_1_08E9:
        mov    eax, 0xc000009a
        pop    ebp
        ret    0x1c
loc_1_08F2:
        push   0
        push   0
        push   8
        push   0
        push   0x940                         ; -> loc_1_0940
        push   ecx
        call   sub_1_1D80
        or     al, al
        je     loc_1_0912
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
loc_1_0912:
        sub    eax, eax
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0920:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_045C
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0940:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0950:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        sub    si, si
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0xf010
        je     loc_1_0971
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
loc_1_0971:
        test   al, 0x24
        je     loc_1_0980
        mov    eax, 0xc00000bb
        pop    esi
        pop    ebp
        ret    0x14
        db  090h
loc_1_0980:
        test   al, 3
        je     loc_1_0988
        mov    si, 1
loc_1_0988:
        test   al, 8
        je     loc_1_0990
        or     si, 2
loc_1_0990:
        mov    word ptr [ecx + 0x9c], si
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_1_09E4
        cmp    word ptr [ecx + 0x100], 0xa
        jae    loc_1_09DA
        mov    eax, dword ptr [ebp + 0x14]
        push   0
        mov    dword ptr [eax + 4], edx
        push   esi
        push   7
        push   eax
        push   0x920                         ; -> loc_1_0920
        push   edx
        call   sub_1_1D80
        or     al, al
        je     loc_1_09D0
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_1_09D0:
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_1_09DA:
        mov    eax, 0xc000009a
        pop    esi
        pop    ebp
        ret    0x14
loc_1_09E4:
        push   0
        push   esi
        push   7
        push   0
        push   0x940                         ; -> loc_1_0940
        push   edx
        call   sub_1_1D80
        or     al, al
        je     loc_1_0A04
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
loc_1_0A04:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_0A10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   1
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0xdc]
        add    ecx, 0xe0
        mov    dword ptr [eax + 0x10], edx
        mov    dword ptr [ecx - 4], eax
        push   ecx
        call   sub_1_04E2
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0A40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi + 0xdc]
        or     edi, edi
        je     loc_1_0AC9
loc_1_0A53:
        mov    eax, dword ptr [esi + 0xdc]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 0xdc], eax
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x20]
        dec    eax
        mov    dword ptr [edi + 0x20], eax
        jne    loc_1_0ABF
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jne    loc_1_0A7F
        push   ebx
        call   sub_1_3380
loc_1_0A7F:
        cmp    dword ptr [ebx + 0xa6], 0
        jne    loc_1_0AA5
        cmp    byte ptr [ebx + 0xdb], 0
        jne    loc_1_0AA5
        push   0
        push   0
        push   5
        push   edi
        push   0xd90                         ; -> loc_1_0D90
        push   edi
        call   sub_1_1D80
        jmp    loc_1_0ABF
loc_1_0AA5:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0xaa]
loc_1_0ABF:
        mov    edi, dword ptr [esi + 0xdc]
        or     edi, edi
        jne    loc_1_0A53
loc_1_0AC9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_0AD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0xdb], 0
        jne    loc_1_0B72
        cmp    dword ptr [esi + 0x115], 0
        jne    loc_1_0B00
        cmp    word ptr [esi + 0x100], 0
        jne    loc_1_0B00
        mov    byte ptr [esi + 0xda], 0
        jmp    loc_1_0B72
loc_1_0B00:
        cmp    byte ptr [esi + 0xda], 1
        jne    loc_1_0B40
        cmp    dword ptr [esi + 0x119], 0
        jne    loc_1_0B30
        cmp    dword ptr [esi + 0xfc], 0
        jne    loc_1_0B30
        mov    byte ptr [esi + 0xda], 0
        push   esi
        call   sub_1_0B90
        jmp    loc_1_0B72
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0B30:
        mov    byte ptr [esi + 0xda], 0
        jmp    loc_1_0B72
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B40:
        mov    byte ptr [esi + 0xda], 1
        mov    dword ptr [esi + 0x119], 0
        mov    dword ptr [esi + 0xfc], 0
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        or     al, 2
        out    dx, al
        push   esi
        call   sub_1_2690
loc_1_0B72:
        cmp    byte ptr [esi + 8], 0
        jne    loc_1_0B89
        push   0x1388
        add    esi, 0xce
        push   esi
        call   sub_1_04E2
loc_1_0B89:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_0B90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        mov    byte ptr [esi + 0xdb], 1
        mov    ecx, dword ptr [esi + 0x10d]
        or     ecx, ecx
        je     loc_1_0C4C
loc_1_0BC1:
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x10d], eax
        dec    dword ptr [esi + 0x115]
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc0000001
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x20]
        dec    eax
        mov    dword ptr [edi + 0x20], eax
        jne    loc_1_0C3E
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jne    loc_1_0BFE
        push   ebx
        call   sub_1_3380
loc_1_0BFE:
        cmp    dword ptr [ebx + 0xa6], 0
        jne    loc_1_0C24
        cmp    byte ptr [ebx + 0xdb], 0
        jne    loc_1_0C24
        push   0
        push   0
        push   5
        push   edi
        push   0xd90                         ; -> loc_1_0D90
        push   edi
        call   sub_1_1D80
        jmp    loc_1_0C3E
loc_1_0C24:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0xaa]
loc_1_0C3E:
        mov    ecx, dword ptr [esi + 0x10d]
        or     ecx, ecx
        jne    loc_1_0BC1
loc_1_0C4C:
        push   esi
        call   sub_1_2190
        push   0x40010007
        push   esi
        call   sub_1_1120
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x24], 0
        je     loc_1_0C90
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0C90:
        inc    dword ptr [esi + 0x20]
        mov    eax, dword ptr [edi + 0xa6]
        cmp    esi, eax
        jne    loc_1_0CA8
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0xa6], eax
        jmp    loc_1_0CBE
loc_1_0CA8:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_0CB8
loc_1_0CB0:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_0CB0
loc_1_0CB8:
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0CBE:
        push   0
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0xca]
        push   eax
        call   sub_2_0240
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_0CE8
        cmp    ebx, 0x103
        je     loc_1_0D00
        cmp    ebx, 0xc001000e
        je     loc_1_0D10
        jmp    loc_1_0D19
loc_1_0CE8:
        mov    eax, dword ptr [esi + 0x20]
        sub    eax, 2
        mov    dword ptr [esi + 0x20], eax
        je     loc_1_0D1C
        mov    byte ptr [esi + 0x24], 1
        mov    ebx, 0x103
        jmp    loc_1_0D1C
        db  08Bh, 0FFh
loc_1_0D00:
        mov    byte ptr [esi + 0x24], 1
        sub    dword ptr [esi + 0x20], 2
        jmp    loc_1_0D1C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0D10:
        mov    byte ptr [esi + 0x24], 1
        mov    ebx, 0x103
loc_1_0D19:
        dec    dword ptr [esi + 0x20]
loc_1_0D1C:
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_0D80
        mov    eax, dword ptr [edi + 0x98]
        cmp    dword ptr [esi + 0x1c], eax
        jne    loc_1_0D33
        push   edi
        call   sub_1_3380
loc_1_0D33:
        cmp    dword ptr [edi + 0xa6], 0
        jne    loc_1_0D65
        cmp    byte ptr [edi + 0xdb], 0
        jne    loc_1_0D65
        push   0
        push   0
        push   5
        push   esi
        mov    ebx, 0x103
        push   0xd90                         ; -> loc_1_0D90
        push   esi
        call   sub_1_1D80
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D65:
        dec    dword ptr [edi + 0xaa]
        push   0
        push   0x25
        push   esi
        call   sub_1_050C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0D80:
        mov    ebx, 0x103
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0D90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0xaa]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [eax + 0x24], 0
        je     loc_1_0DE0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0DE0:
        cmp    dword ptr [esi + 0xae], 0
        je     loc_1_0DF5
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0DF5:
        mov    eax, dword ptr [ebp + 8]
        push   0x40010004
        inc    dword ptr [eax + 0x20]
        push   esi
        call   sub_1_1120
        mov    dword ptr [esi + 0xae], 1
        mov    ecx, dword ptr [esi + 0x10d]
        or     ecx, ecx
        je     loc_1_0EAB
        mov    edi, edi
loc_1_0E20:
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x10d], eax
        dec    dword ptr [esi + 0x115]
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x20]
        dec    eax
        mov    dword ptr [edi + 0x20], eax
        jne    loc_1_0E9D
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jne    loc_1_0E5D
        push   ebx
        call   sub_1_3380
loc_1_0E5D:
        cmp    dword ptr [ebx + 0xa6], 0
        jne    loc_1_0E83
        cmp    byte ptr [ebx + 0xdb], 0
        jne    loc_1_0E83
        push   0
        push   0
        push   5
        push   edi
        push   0xd90                         ; -> loc_1_0D90
        push   edi
        call   sub_1_1D80
        jmp    loc_1_0E9D
loc_1_0E83:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0xaa]
loc_1_0E9D:
        mov    ecx, dword ptr [esi + 0x10d]
        or     ecx, ecx
        jne    loc_1_0E20
loc_1_0EAB:
        cmp    byte ptr [esi + 0xdb], 0
        jne    loc_1_0ED5
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x1020                        ; -> sub_1_1020
        push   eax
        call   sub_1_1D80
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0ED5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0F10
        or     al, al
        jne    loc_1_0F00
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x80010004
        call   sub_1_1020
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0F00:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0F10:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        push   edi
        call   sub_1_1340
        or     al, al
        je     loc_1_1010
        push   edi
        call   sub_1_1B70
        or     al, al
        je     loc_1_1010
        mov    word ptr [edi + 0x100], 0
        mov    word ptr [edi + 0x102], 0
        mov    word ptr [edi + 0x104], 0
        mov    word ptr [edi + 0x106], 0
        mov    dword ptr [edi + 0xfc], 0
        mov    byte ptr [edi + 0x10c], 0
        mov    byte ptr [edi + 0x125], 0
        mov    dword ptr [edi + 0x10d], 0
        mov    dword ptr [edi + 0x111], 0
        mov    dword ptr [edi + 0x115], 0
        mov    dword ptr [edi + 0x119], 0
        mov    byte ptr [edi + 0xdb], 0
        push   0
        push   0
        push   4
        push   esi
        push   0x940                         ; -> loc_1_0940
        push   esi
        call   sub_1_1D80
        lea    eax, [edi + 0x5c]
        push   eax
        push   0
        push   3
        push   esi
        push   0x940                         ; -> loc_1_0940
        push   esi
        call   sub_1_1D80
        push   0
        mov    ax, word ptr [edi + 0x9c]
        push   eax
        push   7
        push   0
        push   0x940                         ; -> loc_1_0940
        push   esi
        call   sub_1_1D80
        push   0
        push   0
        push   8
        push   0
        push   0x940                         ; -> loc_1_0940
        push   esi
        call   sub_1_1D80
        push   0
        push   0
        push   6
        push   esi
        push   0x1020                        ; -> sub_1_1020
        push   esi
        call   sub_1_1D80
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1010:
        mov    byte ptr [edi + 0xdb], 1
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_1020:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   0x40010005
        mov    edi, dword ptr [esi + 8]
        push   edi
        call   sub_1_1120
        mov    dword ptr [edi + 0xae], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0xc0000001
        jne    loc_1_104F
        mov    eax, 0x80010004
loc_1_104F:
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0456
        push   edi
        call   sub_1_10D0
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x20]
        dec    eax
        mov    dword ptr [esi + 0x20], eax
        jne    loc_1_10BF
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [edi + 0x98], eax
        jne    loc_1_107B
        push   edi
        call   sub_1_3380
loc_1_107B:
        cmp    dword ptr [edi + 0xa6], 0
        jne    loc_1_10A5
        cmp    byte ptr [edi + 0xdb], 0
        jne    loc_1_10A5
        push   0
        push   0
        push   5
        push   esi
        push   0xd90                         ; -> loc_1_0D90
        push   esi
        call   sub_1_1D80
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_10A5:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0xaa]
loc_1_10BF:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xec]
        or     ecx, ecx
        je     loc_1_1115
loc_1_10E1:
        mov    eax, dword ptr [esi + 0xec]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0xec], eax
        mov    eax, dword ptr [esi + 0xa6]
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [esi + 0xa6], ecx
        push   0
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_043E
        mov    ecx, dword ptr [esi + 0xec]
        or     ecx, ecx
        jne    loc_1_10E1
loc_1_1115:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0xa6]
        or     esi, esi
        je     loc_1_11C8
loc_1_113A:
        inc    dword ptr [esi + 0x20]
        mov    edi, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], 0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0474
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x20]
        dec    eax
        mov    dword ptr [esi + 0x20], eax
        jne    loc_1_11BE
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jne    loc_1_117E
        push   ebx
        call   sub_1_3380
loc_1_117E:
        cmp    dword ptr [ebx + 0xa6], 0
        jne    loc_1_11A4
        cmp    byte ptr [ebx + 0xdb], 0
        jne    loc_1_11A4
        push   0
        push   0
        push   5
        push   esi
        push   0xd90                         ; -> loc_1_0D90
        push   esi
        call   sub_1_1D80
        jmp    loc_1_11BE
loc_1_11A4:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   esi
        call   sub_1_050C
        dec    dword ptr [ebx + 0xaa]
loc_1_11BE:
        mov    esi, edi
        or     edi, edi
        jne    loc_1_113A
loc_1_11C8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x54]
        and    eax, 0x8000
        add    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [esi + 0xf4], eax
        mov    eax, dword ptr [esi + 0x54]
        and    eax, 0xff00
        add    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [esi + 0xf8], eax
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    eax, 4
        add    eax, ecx
        lea    edi, [eax*4]
        xor    eax, eax
        mov    ax, word ptr [edi + 0x5854]   ; -> obj1:0x5854
        cmp    eax, 2
        jne    loc_1_1264
        cmp    ecx, 3
        jne    loc_1_1245
        mov    edx, dword ptr [esi + 0xc6]
        inc    dx
        mov    eax, dword ptr [esi + 0x2e]
        shr    eax, 0x14
        and    al, 0xf
        out    dx, al
loc_1_1245:
        mov    eax, dword ptr [esi + 0x2e]
        shr    eax, 0xc
        and    al, 0xfc
        mov    dx, word ptr [esi + 0xc6]
        out    dx, al
        cmp    ecx, 4
        jne    loc_1_1264
        mov    dx, word ptr [esi + 0xc2]
        mov    al, 1
        out    dx, al
loc_1_1264:
        xor    eax, eax
        mov    ax, word ptr [edi + 0x585e]   ; -> obj1:0x585E
        add    eax, 0x1000
        shl    eax, 4
        sub    eax, dword ptr [esi + 0x50]
        shr    eax, 4
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0xb2], al
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        mov    ax, word ptr [ecx*4 + 0x585a] ; -> obj1:0x585A
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0xb3], al
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        mov    ax, word ptr [ecx*4 + 0x585a] ; -> obj1:0x585A
        add    eax, 0x800
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0xb4], al
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        mov    ax, word ptr [ecx*4 + 0x5858] ; -> obj1:0x5858
        push   eax
        push   esi
        call   sub_1_1300
        mov    byte ptr [esi + 0xb5], al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1300:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        lea    esi, [eax*4]
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 4
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x5856]   ; -> obj1:0x5856
        sub    edx, edx
        div    ecx
        xor    ecx, ecx
        mov    cl, al
        mov    cl, byte ptr [esi + ecx + 0x5834] ; -> obj1:0x5834
        and    cl, 0xfd
        mov    al, cl
        pop    esi
        pop    ebp
        ret    8
sub_1_1340:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        sub    al, al
        cmp    dword ptr [esi + 0x46], 4
        jne    loc_1_1378
        push   esi
        call   sub_1_13E0
        or     al, al
        jne    loc_1_137C
        push   esi
        call   sub_1_13E0
        or     al, al
        jne    loc_1_137C
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_1378:
        or     al, al
        je     loc_1_1382
loc_1_137C:
        cmp    dword ptr [esi + 0x46], 4
        je     loc_1_138E
loc_1_1382:
        push   esi
        call   sub_1_1520
        cmp    dword ptr [esi + 0x46], 4
        jne    loc_1_1394
loc_1_138E:
        cmp    byte ptr [esi + 0x4a], 0
        je     loc_1_13A5
loc_1_1394:
        push   esi
        call   sub_1_15C0
        or     al, al
        jne    loc_1_13A5
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13A5:
        push   esi
        call   sub_1_16D0
        or     al, al
        jne    loc_1_13B6
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13B6:
        push   esi
        call   sub_1_17B0
        or     al, al
        jne    loc_1_13C7
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13C7:
        push   esi
        call   sub_1_1810
        or     al, al
        jne    loc_1_13D8
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_13D8:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_13E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x7d0
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 8], eax
        mov    al, 1
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        call   sub_1_0500
        mov    dx, word ptr [esi + 0xbe]
        sub    al, al
        out    dx, al
        push   0x7d0
        call   sub_1_0500
        mov    dx, word ptr [esi + 0xc2]
        mov    al, 0x81
        out    dx, al
        mov    al, byte ptr [esi + 0xb2]
        or     al, 2
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    ax, word ptr [eax + 0x96]
        mov    word ptr [ebp - 2], ax
        mov    ebx, 0xaa
        lea    esp, [esp]
loc_1_1450:
        cmp    word ptr [ebp - 2], bx
        je     loc_1_1477
        push   0x3e8
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 0x96]
        mov    word ptr [ebp - 2], ax
        cmp    edi, 0x5dc
        jb     loc_1_1450
loc_1_1477:
        cmp    word ptr [ebp - 2], 0xaa
        jne    loc_1_14F0
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    word ptr [eax + 0x92], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, 0x55
        mov    ax, word ptr [eax + 0x96]
        mov    word ptr [ebp - 2], ax
loc_1_14A0:
        cmp    word ptr [ebp - 2], bx
        je     loc_1_14C7
        push   0x3e8
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 0x96]
        mov    word ptr [ebp - 2], ax
        cmp    edi, 0x1f4
        jb     loc_1_14A0
loc_1_14C7:
        cmp    word ptr [ebp - 2], 0x55
        jne    loc_1_14F0
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        mov    dx, word ptr [esi + 0xc2]
        mov    al, 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_14F0:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        mov    dx, word ptr [esi + 0xc2]
        mov    al, 1
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax*4 + 0x5862] ; -> obj1:0x5862
        mov    edi, dword ptr [esi + 0xf0]
        and    ecx, dword ptr [esi + 0x54]
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + edi], 0
        mov    word ptr [eax + 0x92], 0
        mov    eax, dword ptr [ebp - 4]
        push   0x3e8
        mov    word ptr [eax + 0x94], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x96], 0
        mov    al, 1
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        call   sub_1_0500
        mov    dx, word ptr [esi + 0xbe]
        sub    al, al
        out    dx, al
        push   0x7a120
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_15C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 2
        xor    edi, edi
        mov    di, word ptr [eax + 0x5862]   ; -> obj1:0x5862
        and    edi, dword ptr [esi + 0x54]
        add    edi, dword ptr [esi + 0xf0]
        mov    dword ptr [ebp - 0xc], 1
        test   byte ptr [eax + 0x5876], 0x80 ; -> obj1:0x5876
        je     loc_1_160B
        mov    dword ptr [ebp - 0xc], 2
loc_1_160B:
        sub    ebx, ebx
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        sub    esi, esi
        lea    ecx, [ecx]
loc_1_1620:
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [ebp - 1], 0x80
        mov    ax, word ptr [eax + 0x94]
        mov    word ptr [ebp - 4], ax
        je     loc_1_1692
        cmp    byte ptr [ebp - 1], 0xff
        je     loc_1_1692
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 4], al
        jne    loc_1_1692
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jne    loc_1_1692
        cmp    al, 0xaa
        jne    loc_1_16C0
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0xc]
        je     loc_1_16B4
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi], 0
        mov    word ptr [eax + 0x92], 0xe
        push   0x3d090
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi], 0xff
        mov    word ptr [eax + 0x94], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x92], 3
loc_1_1692:
        push   0x3e8
        call   sub_1_0500
        inc    esi
        cmp    esi, 0x4e20
        jb     loc_1_1620
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_16B4:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_16C0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_16D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xf8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 0x46]
        mov    edx, eax
        shl    eax, 4
        add    eax, edx
        mov    ax, word ptr [eax*4 + 0x5854] ; -> obj1:0x5854
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_1757
        cmp    eax, 2
        je     loc_1_1781
        mov    esi, dword ptr [ebp - 0xc]
loc_1_1709:
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        mov    ax, word ptr [eax + 0x92]
        mov    word ptr [ebp - 2], ax
        cmp    esi, edi
        je     loc_1_1745
        mov    ebx, 1
loc_1_1722:
        cmp    word ptr [ebp - 2], bx
        je     loc_1_1745
        push   0x3e8
        call   sub_1_0500
        mov    eax, dword ptr [ebp - 8]
        inc    edi
        mov    ax, word ptr [eax + 0x92]
        mov    word ptr [ebp - 2], ax
        cmp    edi, esi
        jb     loc_1_1722
loc_1_1745:
        cmp    word ptr [ebp - 2], 1
        je     loc_1_17A5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1757:
        mov    dx, word ptr [ecx + 0xbe]
        mov    al, byte ptr [ecx + 0xb2]
        out    dx, al
        mov    esi, 0x1f4
        mov    edx, dword ptr [ecx + 0x54]
        and    edx, 0xfefa
        mov    eax, dword ptr [ecx + 0xf0]
        mov    word ptr [edx + eax], 0x5a5a
        jmp    loc_1_1709
loc_1_1781:
        mov    dx, word ptr [ecx + 0xbe]
        mov    al, byte ptr [ecx + 0xb2]
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        mov    esi, 0x1388
        mov    word ptr [eax + 0x92], 0xd
        jmp    loc_1_1709
loc_1_17A5:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_17B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        lea    edi, [esi + 0x62]
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 4], eax
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xf0
        push   eax
        push   edi
        call   sub_1_05EC
        cmp    byte ptr [edi], 0
        jne    loc_1_1806
        cmp    byte ptr [esi + 0x63], 0xdd
        jne    loc_1_1806
        mov    al, byte ptr [esi + 0x64]
        cmp    al, 1
        je     loc_1_17FC
        or     al, al
        jne    loc_1_1806
loc_1_17FC:
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1806:
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, 1
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb5]
        out    dx, al
        mov    edi, dword ptr [esi + 0xf0]
        sub    edx, edx
        test   dword ptr [esi + 0x50], 0xfffffffc
        jbe    loc_1_1854
        mov    eax, edi
        nop    
loc_1_1840:
        mov    dword ptr [eax], 0xf55ccf0
        add    eax, 4
        inc    edx
        mov    ebx, dword ptr [esi + 0x50]
        shr    ebx, 2
        cmp    ebx, edx
        ja     loc_1_1840
loc_1_1854:
        sub    eax, eax
        test   dword ptr [esi + 0x50], 0xfffffffc
        jbe    loc_1_187A
        mov    edx, 0xf55ccf0
loc_1_1864:
        mov    ebx, dword ptr [edi]
        cmp    ebx, edx
        je     loc_1_186C
        sub    cl, cl
loc_1_186C:
        add    edi, 4
        inc    eax
        mov    ebx, dword ptr [esi + 0x50]
        shr    ebx, 2
        cmp    ebx, eax
        ja     loc_1_1864
loc_1_187A:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xf0]
        mov    dword ptr [ebp - 8], edi
        mov    ebx, dword ptr [esi + 0xf4]
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0x46]
        mov    dword ptr [ebp - 0xc], eax
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        mov    edx, dword ptr [esi + 0x50]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x90], 0x40
        mov    word ptr [eax + 0x92], 1
        mov    dword ptr [eax + 0xb0], 0xffffffff
        mov    dword ptr [eax + 0xc0], 0xffffffff
        mov    dword ptr [eax + 0xc4], 0xffffffff
        mov    dword ptr [eax + 0x44], 0xffffffff
        mov    dword ptr [eax + 0x1c], 0
        mov    ecx, 0x2a6
        mov    byte ptr [eax + 0xe0], 0xff
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        mov    edi, dword ptr [esi + 0xf0]
        sub    eax, eax
        rep stosd dword ptr es:[edi], eax
        mov    al, byte ptr [ebp - 0xc]
        mov    byte ptr [ebx + 0x8c1], al
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 4
        add    eax, dword ptr [ebp - 0xc]
        lea    ecx, [eax*4]
        test   byte ptr [ecx + 0x5876], 0x20 ; -> obj1:0x5876
        je     loc_1_197E
        mov    ax, word ptr [ebx + 0x8a2]
        mov    word ptr [ebp - 2], ax
        or     word ptr [ebp - 2], 0x4000
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebx + 0x8a2], ax
loc_1_197E:
        mov    al, byte ptr [ecx + 0x586c]   ; -> obj1:0x586C
        mov    byte ptr [ebx + 0x933], al
        mov    al, byte ptr [ecx + 0x586d]   ; -> loc_1_586D
        mov    byte ptr [ebx + 0x934], al
        sub    al, al
        cmp    dword ptr [ebp - 0xc], 4
        jne    loc_1_19A8
        movsx  eax, byte ptr [esi + 0x4b]
        mov    al, byte ptr [eax + 0x5554]   ; -> obj1:0x5554
loc_1_19A8:
        mov    byte ptr [ebx + 0x935], al
        or     al, 1
        mov    byte ptr [ebx + 0x936], al
        mov    eax, dword ptr [esi + 0x54]
        mov    word ptr [ebx + 0x8b6], ax
        mov    eax, dword ptr [esi + 0x50]
        mov    dword ptr [ebx + 0x8ba], eax
        mov    ax, word ptr [ecx + 0x5866]   ; -> obj1:0x5866
        mov    word ptr [ebx + 0x8a8], ax
        mov    ax, word ptr [ecx + 0x5868]   ; -> obj1:0x5868
        mov    word ptr [ebx + 0x8aa], ax
        mov    ax, word ptr [ecx + 0x5864]   ; -> obj1:0x5864
        mov    word ptr [ebx + 0x8ac], ax
        mov    ax, word ptr [ecx + 0x586a]   ; -> obj1:0x586A
        mov    word ptr [ebx + 0x8ae], ax
        mov    ax, word ptr [ecx + 0x586e]   ; -> obj1:0x586E
        mov    word ptr [ebx + 0x8b0], ax
        mov    ax, word ptr [ecx + 0x5870]   ; -> obj1:0x5870
        mov    word ptr [ebx + 0x8b2], ax
        xor    eax, eax
        mov    word ptr [ebx + 0x8c8], 0x3e8
        mov    word ptr [ebx + 0x8d4], 0x7d0
        mov    ax, word ptr [ecx + 0x5860]   ; -> obj1:0x5860
        shl    eax, 0x10
        add    eax, 0xfff0
        mov    dword ptr [ebx + 0x94a], eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x5860]   ; -> obj1:0x5860
        shl    eax, 0x10
        add    eax, 0xfff6
        mov    dword ptr [ebx + 0x94e], eax
        mov    eax, dword ptr [esi + 0x32]
        mov    word ptr [ebx + 0x8e0], ax
        mov    eax, dword ptr [esi + 0x36]
        mov    word ptr [ebx + 0x8e6], ax
        mov    eax, dword ptr [esi + 0x42]
        mov    word ptr [ebx + 0x8ee], ax
        mov    ax, word ptr [ecx + 0x5858]   ; -> obj1:0x5858
        mov    word ptr [ebx + 0x8e8], ax
        mov    word ptr [ebx + 0x8f2], 0x5ea
        mov    word ptr [ebx + 0x920], 0x5ea
        mov    word ptr [ebx + 0x922], 0x3c
        mov    word ptr [ebx + 0x8ce], 0x2710
        mov    byte ptr [ebx + 0x926], 0xf
        mov    byte ptr [ebx + 0x927], 2
        mov    byte ptr [ebx + 0x928], 0
        mov    byte ptr [ebx + 0x929], 0x60
        mov    word ptr [ebx + 0x92a], 0x200
        mov    byte ptr [ebx + 0x92c], 0xf
        mov    word ptr [ebx + 0x924], 0x10
        mov    byte ptr [ebx + 0x92d], 0
        mov    byte ptr [ebx + 0x92e], 0
        mov    byte ptr [ebx + 0x92f], 0
        mov    byte ptr [ebx + 0x930], 0
        mov    byte ptr [ebx + 0x931], 0
        mov    byte ptr [ebx + 0x932], 0x3c
        mov    ax, word ptr [ecx + 0x5856]   ; -> obj1:0x5856
        mov    word ptr [ebx + 0x8be], ax
        lea    eax, [ecx + 0x5834]           ; -> obj1:0x5834
        push   0x20
        push   eax
        add    ebx, 0x900
        push   ebx
        call   sub_1_05EC
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb5]
        out    dx, al
        push   0x14ee
        push   0x4064                        ; -> loc_1_4064
        mov    eax, dword ptr [esi + 0xf0]
        push   eax
        call   sub_1_05EC
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [esi + 0xf0]
        mov    dword ptr [ebp - 0xc], eax
        mov    edi, dword ptr [esi + 0xf4]
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [esi + 9]
        push   esi
        push   0x2410                        ; -> loc_1_2410
        push   eax
        call   sub_1_04FA
        push   esi
        call   sub_1_1890
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        mov    byte ptr [edi + 0xa20], 0x8c
        mov    byte ptr [edi + 0xa21], 0xc8
        mov    byte ptr [edi + 0xa22], 0x8e
        mov    byte ptr [edi + 0xa23], 0xd8
        out    dx, al
        mov    byte ptr [edi + 0xa24], 0xea
        mov    byte ptr [edi + 0xa25], 0
        mov    byte ptr [edi + 0xa26], 0
        mov    eax, dword ptr [esi + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        mov    ax, word ptr [eax*4 + 0x5858] ; -> obj1:0x5858
        mov    word ptr [edi + 0xa27], ax
        mov    dword ptr [esi + 0x25], 0
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        mov    eax, dword ptr [esi + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [eax*4 + 0x585a] ; -> obj1:0x585A
        mov    word ptr [ecx + 0xda], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebp - 4], 0x8a20
        mov    word ptr [eax + 0xd8], 0x8a20
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x92], 2
        sub    ebx, ebx
        mov    al, byte ptr [esi + 0xb4]
        or     al, 2
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        mov    ax, word ptr [edi + 0x8a0]
        mov    word ptr [ebp - 4], ax
loc_1_1C71:
        test   byte ptr [ebp - 3], 0x80
        je     loc_1_1C7D
        cmp    dword ptr [esi + 0x25], 0
        jne    loc_1_1C9B
loc_1_1C7D:
        push   0x3e8
        call   sub_1_0500
        mov    ax, word ptr [edi + 0x8a0]
        mov    word ptr [ebp - 4], ax
        inc    ebx
        cmp    ebx, 0x3e8
        jb     loc_1_1C71
loc_1_1C9B:
        test   byte ptr [ebp - 3], 0x80
        je     loc_1_1CA7
        cmp    dword ptr [esi + 0x25], 0
        jne    loc_1_1CAB
loc_1_1CA7:
        mov    byte ptr [ebp - 1], 0
loc_1_1CAB:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        push   esi
        push   0x2420                        ; -> loc_1_2420
        lea    eax, [esi + 9]
        push   eax
        call   sub_1_04FA
        cmp    dword ptr [esi + 0x25], 0
        jne    loc_1_1CEA
        movsx  eax, byte ptr [esi + 0x4b]
        push   eax
        push   1
        push   0xaa000005
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0x10
        mov    byte ptr [ebp - 1], 0
loc_1_1CEA:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1D48
        mov    al, byte ptr [esi + 0xb4]
        or     al, 2
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        xor    eax, eax
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0x8f2]
        mov    ax, word ptr [edi + 0x8f4]
        imul   eax, ecx
        mov    dword ptr [esi + 0x68], eax
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 0x8ee]
        mov    cx, word ptr [edi + 0x8f0]
        imul   ecx, eax
        mov    dword ptr [esi + 0x6c], ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 0x8f2]
        mov    dword ptr [esi + 0x70], eax
        xor    eax, eax
        mov    ax, word ptr [edi + 0x8ee]
        mov    dword ptr [esi + 0x74], eax
loc_1_1D48:
        mov    al, byte ptr [esi + 0xb3]
        or     al, 2
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        mov    byte ptr [eax + 0xe0], 0
        mov    byte ptr [eax + 0xe1], 0
        mov    al, byte ptr [ebp - 1]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x100]
        cmp    eax, dword ptr [ecx + 0x36]
        jb     loc_1_1DA3
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x18
loc_1_1DA3:
        cmp    byte ptr [ecx + 0xdb], 0
        je     loc_1_1DB5
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x18
loc_1_1DB5:
        xor    esi, esi
        inc    dword ptr [edi + 0x20]
        mov    si, word ptr [ecx + 0x102]
        inc    word ptr [ecx + 0x100]
        lea    esi, [esi + esi*2]
        shl    esi, 3
        xor    eax, eax
        add    esi, dword ptr [ecx + 0x108]
        mov    ax, word ptr [ecx + 0x102]
        inc    eax
        sub    edx, edx
        div    dword ptr [ecx + 0x36]
        mov    word ptr [ecx + 0x102], dx
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], eax
        mov    ax, word ptr [ebp + 0x14]
        cmp    ax, 3
        mov    word ptr [esi + 0xc], ax
        mov    dx, word ptr [ebp + 0x18]
        mov    word ptr [esi + 0x10], dx
        je     loc_1_1E14
        cmp    ax, 9
        jne    loc_1_1E37
loc_1_1E14:
        sub    di, di
        mov    eax, dword ptr [ebp + 0x1c]
        lea    ebx, [ebx]
loc_1_1E20:
        mov    ebx, edi
        and    ebx, 0xffff
        mov    dl, byte ptr [eax + ebx]
        mov    byte ptr [ebx + esi + 0x12], dl
        inc    di
        cmp    di, 6
        jb     loc_1_1E20
loc_1_1E37:
        push   ecx
        call   sub_1_2690
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0xf4]
        mov    dword ptr [ebp - 8], edx
        cmp    byte ptr [esi + 0xdb], 0
        jne    loc_1_1FAD
        mov    ax, word ptr [esi + 0x102]
        cmp    word ptr [esi + 0x106], ax
        je     loc_1_1F9D
loc_1_1E86:
        mov    cx, word ptr [esi + 0x106]
        mov    eax, ecx
        and    eax, 0xffff
        lea    ebx, [eax + eax*2]
        shl    ebx, 3
        add    ebx, dword ptr [esi + 0x108]
        cmp    word ptr [ebx + 0xc], 8
        jne    loc_1_1EB4
        cmp    byte ptr [esi + 0x10c], 0
        jne    loc_1_1FAD
loc_1_1EB4:
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 0x36]
        mov    word ptr [esi + 0x106], dx
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        mov    eax, dword ptr [esi + 0x54]
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 0x4e]
        and    edi, eax
        mov    edx, dword ptr [esi + 0xf0]
        add    edx, edi
        mov    dword ptr [ebp - 4], edx
        movzx  edi, byte ptr [edx + 5]
        mov    dl, byte ptr [edx]
        and    edx, 0xff
        imul   edi, edx
        mov    edx, dword ptr [ebp - 4]
        mov    dx, word ptr [edx + 6]
        and    edx, 0xffff
        and    edx, eax
        add    edi, edx
        add    edi, dword ptr [esi + 0xf0]
        mov    ax, word ptr [ebx + 0xc]
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], cx
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [edi + 4], ax
        cmp    word ptr [ebx + 0xc], 8
        jne    loc_1_1F39
        mov    byte ptr [esi + 0x10c], 1
        push   esi
        call   sub_1_22A0
        mov    word ptr [edi + 4], ax
loc_1_1F39:
        mov    ax, word ptr [ebx + 0xc]
        cmp    ax, 3
        je     loc_1_1F49
        cmp    ax, 9
        jne    loc_1_1F68
loc_1_1F49:
        sub    dx, dx
        lea    esp, [esp]
loc_1_1F50:
        mov    ecx, edx
        and    ecx, 0xffff
        mov    al, byte ptr [ebx + ecx + 0x12]
        mov    byte ptr [ecx + edi + 6], al
        inc    dx
        cmp    dx, 6
        jb     loc_1_1F50
loc_1_1F68:
        mov    edx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        inc    al
        cmp    byte ptr [edx + 4], al
        jae    loc_1_1F76
        sub    al, al
loc_1_1F76:
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx], al
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        mov    ax, word ptr [esi + 0x102]
        cmp    word ptr [esi + 0x106], ax
        jne    loc_1_1E86
loc_1_1F9D:
        mov    al, byte ptr [esi + 0xb3]
        or     al, 2
        mov    dx, word ptr [esi + 0xbe]
        out    dx, al
loc_1_1FAD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0xf4]
        cmp    byte ptr [ecx + 0xdb], 0
        jne    loc_1_2184
        mov    ax, word ptr [ecx + 0x104]
        cmp    word ptr [ecx + 0x106], ax
        je     loc_1_2184
        mov    dx, word ptr [ecx + 0xbe]
        mov    al, byte ptr [ecx + 0xb4]
        out    dx, al
        mov    ax, word ptr [esi + 0x5e]
        mov    word ptr [ebp - 6], ax
        mov    edx, eax
        and    edx, 0xffff
        and    edx, dword ptr [ecx + 0x54]
        add    edx, dword ptr [ecx + 0xf0]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ebp - 3], al
        movzx  esi, byte ptr [ebp - 3]
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp - 6], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        imul   esi, eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        and    eax, dword ptr [ecx + 0x54]
        add    esi, eax
        add    esi, dword ptr [ecx + 0xf0]
        mov    al, byte ptr [edx]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 3], al
        jne    loc_1_2071
        mov    al, byte ptr [ecx + 0xb3]
        or     al, 2
        mov    dx, word ptr [ecx + 0xbe]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2071:
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp - 6], ax
        or     ax, ax
        mov    dword ptr [ebp - 0xc], 0
        je     loc_1_208C
        mov    dword ptr [ebp - 0xc], 0xc0000001
loc_1_208C:
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ebp - 1], al
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jae    loc_1_20AE
        mov    byte ptr [ebp - 1], 0
loc_1_20AE:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edx + 2], al
        mov    al, byte ptr [ecx + 0xb3]
        or     al, 2
        mov    dx, word ptr [ecx + 0xbe]
        mov    ebx, dword ptr [ebp - 8]
        and    ebx, 0xffff
        out    dx, al
        lea    ebx, [ebx + ebx*2]
        shl    ebx, 3
        add    ebx, dword ptr [ecx + 0x108]
        mov    esi, dword ptr [ebx]
        mov    di, word ptr [ebx + 0xc]
        cmp    di, 8
        jne    loc_1_20EC
        mov    byte ptr [ecx + 0x10c], 0
loc_1_20EC:
        xor    eax, eax
        dec    word ptr [ecx + 0x100]
        mov    ax, word ptr [ecx + 0x104]
        inc    eax
        sub    edx, edx
        div    dword ptr [ecx + 0x36]
        mov    word ptr [ecx + 0x104], dx
        inc    dword ptr [ecx + 0xfc]
        mov    eax, dword ptr [ebx + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [ebx + 8]
        cmp    di, 5
        je     loc_1_2184
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x20]
        dec    eax
        mov    dword ptr [esi + 0x20], eax
        jne    loc_1_2184
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [edi + 0x98], eax
        jne    loc_1_213D
        push   edi
        call   sub_1_3380
loc_1_213D:
        cmp    dword ptr [edi + 0xa6], 0
        jne    loc_1_216A
        cmp    byte ptr [edi + 0xdb], 0
        jne    loc_1_216A
        push   0
        push   0
        push   5
        push   esi
        push   0xd90                         ; -> loc_1_0D90
        push   esi
        call   sub_1_1D80
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_216A:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   esi
        call   sub_1_050C
        dec    dword ptr [edi + 0xaa]
loc_1_2184:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2190:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x102]
        cmp    word ptr [esi + 0x104], ax
        je     loc_1_227E
loc_1_21B0:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x104]
        lea    eax, [eax + eax*2]
        shl    eax, 3
        add    eax, dword ptr [esi + 0x108]
        mov    ebx, dword ptr [eax]
        mov    di, word ptr [eax + 0xc]
        cmp    di, 8
        jne    loc_1_21D8
        mov    byte ptr [esi + 0x10c], 0
loc_1_21D8:
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        push   0xc0000001
        call   dword ptr [eax + 8]
        xor    eax, eax
        inc    dword ptr [esi + 0xfc]
        mov    ax, word ptr [esi + 0x104]
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 0x36]
        mov    word ptr [esi + 0x104], dx
        dec    word ptr [esi + 0x100]
        cmp    di, 5
        je     loc_1_226A
        mov    edi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0x20]
        dec    eax
        mov    dword ptr [ebx + 0x20], eax
        jne    loc_1_226A
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    dword ptr [edi + 0x98], eax
        jne    loc_1_222A
        push   edi
        call   sub_1_3380
loc_1_222A:
        cmp    dword ptr [edi + 0xa6], 0
        jne    loc_1_2250
        cmp    byte ptr [edi + 0xdb], 0
        jne    loc_1_2250
        push   0
        push   0
        push   5
        push   ebx
        push   0xd90                         ; -> loc_1_0D90
        push   ebx
        call   sub_1_1D80
        jmp    loc_1_226A
loc_1_2250:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   ebx
        call   sub_1_050C
        dec    dword ptr [edi + 0xaa]
loc_1_226A:
        mov    ax, word ptr [esi + 0x102]
        cmp    word ptr [esi + 0x104], ax
        jne    loc_1_21B0
loc_1_227E:
        mov    ax, word ptr [esi + 0x104]
        mov    word ptr [esi + 0x106], ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_22A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xf4]
        mov    eax, dword ptr [esi + 0x32]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        lea    eax, [edi + ecx]
        add    eax, 0xa98
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 3
        jne    loc_1_2310
        sub    edx, edx
loc_1_22E4:
        mov    eax, dword ptr [esi + 0x32]
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2]
        mov    al, byte ptr [edi + eax + 0xaa4]
        mov    cl, al
        mov    eax, dword ptr [esi + 0x32]
        inc    edx
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 - 1]
        mov    byte ptr [edi + eax + 0xaaa], cl
        cmp    edx, 6
        jb     loc_1_22E4
        inc    dword ptr [ebp - 8]
loc_1_2310:
        mov    eax, dword ptr [ebp - 8]
        imul   ax, ax, 6
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2320:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xf0]
        cmp    byte ptr [eax + 0x29], 0
        je     loc_1_2350
        inc    dword ptr [eax + 0x25]
        mov    dx, word ptr [eax + 0xbe]
        mov    al, byte ptr [eax + 0xb4]
        out    dx, al
        or     al, 2
        out    dx, al
        sub    al, al
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2350:
        cmp    byte ptr [eax + 0x125], 0
        je     loc_1_2360
        sub    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_2360:
        mov    dx, word ptr [eax + 0xbe]
        mov    bl, byte ptr [eax + 0xb3]
        mov    al, bl
        or     al, 2
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    byte ptr [ecx + 0xe0], 0xff
        mov    byte ptr [ecx + 0xe1], 0
        mov    byte ptr [ecx + 0xe2], 0
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi + 0xf0]
        cmp    byte ptr [esi + 0x125], 0
        jne    loc_1_2405
        mov    byte ptr [esi + 0x125], 1
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        push   esi
        call   sub_1_27D0
        push   esi
        call   sub_1_26D0
        push   esi
        call   sub_1_1E50
        push   esi
        call   sub_1_1FC0
        mov    byte ptr [esi + 0x125], 0
        mov    cl, byte ptr [esi + 0xb3]
        or     cl, 2
        mov    dx, word ptr [esi + 0xbe]
        mov    al, cl
        out    dx, al
        mov    byte ptr [edi + 0xe0], 0
loc_1_2405:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_1_2410:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 0x29], 1
        mov    al, 1
        ret    4
loc_1_2420:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 0x29], 0
        mov    al, 1
        ret    4
loc_1_2430:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebx + 8]
        lea    eax, [esi + 0x24]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_0530
        cmp    dword ptr [ebp - 8], 0xc
        jb     loc_1_2500
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_2500
        cmp    byte ptr [ebx + 0x24], 0
        je     loc_1_2483
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2483:
        cmp    byte ptr [edi + 0xdb], 0
        je     loc_1_24A0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_24A0:
        cmp    dword ptr [edi + 0xae], 0
        je     loc_1_24B7
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_24B7:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebx + 0x20]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        cmp    dword ptr [edi + 0x10d], 0
        jne    loc_1_24D7
        mov    dword ptr [edi + 0x10d], esi
        jmp    loc_1_24E0
loc_1_24D7:
        mov    eax, dword ptr [edi + 0x111]
        mov    dword ptr [eax + 0x24], esi
loc_1_24E0:
        mov    dword ptr [edi + 0x111], esi
        inc    dword ptr [edi + 0x115]
        push   edi
        call   sub_1_2690
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2500:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_2510:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0xf4]
        mov    dx, word ptr [edi + 0xbe]
        mov    al, byte ptr [edi + 0xb4]
        out    dx, al
        mov    al, byte ptr [esi + 0x482]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [esi + 0x480]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        jne    loc_1_2563
        mov    dx, word ptr [edi + 0xbe]
        mov    al, byte ptr [edi + 0xb3]
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2563:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        mov    ax, word ptr [esi + eax*2 + 0x490]
        mov    ebx, eax
        and    ebx, 0xffff
        add    ebx, dword ptr [edi + 0xf0]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 8], 0x3c
        jae    loc_1_259E
        mov    dword ptr [ebp - 8], 0x3c
loc_1_259E:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebp - 8]
        or     ax, 0x8000
        mov    word ptr [ebx], ax
        mov    ax, word ptr [ebx + 0xe]
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, dword ptr [edi + 0xf0]
        mov    al, byte ptr [ebx + 7]
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [edi + 0xbe]
        out    dx, al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   sub_1_2620
        mov    dx, word ptr [edi + 0xbe]
        mov    al, byte ptr [edi + 0xb4]
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        inc    al
        mov    byte ptr [esi + 0x482], al
        mov    al, byte ptr [esi + 0x690]
        mov    byte ptr [ebp - 1], al
        inc    al
        mov    byte ptr [esi + 0x690], al
        mov    dx, word ptr [edi + 0xbe]
        mov    al, byte ptr [edi + 0xb3]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2620:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        push   eax
        call   sub_1_0530
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_267F
        sub    ebx, ebx
loc_1_2648:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_05EC
        add    esi, dword ptr [ebp - 8]
        add    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_1_2648
loc_1_267F:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2690:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 0xf0]
        cmp    byte ptr [ecx + 0x125], 0
        jne    loc_1_26C4
        mov    al, byte ptr [ecx + 0xb3]
        or     al, 2
        mov    dx, word ptr [ecx + 0xbe]
        out    dx, al
        mov    byte ptr [esi + 0xe1], 0
        mov    byte ptr [esi + 0xe3], 0xff
loc_1_26C4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 1], 1
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    ebx, dword ptr [esi + 0x10d]
        or     ebx, ebx
        je     loc_1_27BB
loc_1_26F2:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_27BB
        push   ebx
        push   esi
        call   sub_1_2510
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     loc_1_27A0
        mov    byte ptr [ebp - 2], al
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x10d], eax
        dec    dword ptr [esi + 0x115]
        mov    eax, dword ptr [esi + 0xf0]
        inc    dword ptr [esi + 0x119]
        mov    byte ptr [eax + 0xe4], 0
        mov    edi, dword ptr [ebx + 0x28]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x20]
        dec    eax
        mov    dword ptr [edi + 0x20], eax
        jne    loc_1_27AD
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jne    loc_1_275E
        push   ebx
        call   sub_1_3380
loc_1_275E:
        cmp    dword ptr [ebx + 0xa6], 0
        jne    loc_1_2784
        cmp    byte ptr [ebx + 0xdb], 0
        jne    loc_1_2784
        push   0
        push   0
        push   5
        push   edi
        push   0xd90                         ; -> loc_1_0D90
        push   edi
        call   sub_1_1D80
        jmp    loc_1_27AD
loc_1_2784:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        push   0
        push   0x25
        push   edi
        call   sub_1_050C
        dec    dword ptr [ebx + 0xaa]
        jmp    loc_1_27AD
loc_1_27A0:
        mov    eax, dword ptr [esi + 0xf0]
        mov    byte ptr [eax + 0xe4], 1
loc_1_27AD:
        mov    ebx, dword ptr [esi + 0x10d]
        or     ebx, ebx
        jne    loc_1_26F2
loc_1_27BB:
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], 0
        mov    edi, dword ptr [esi + 0xf4]
        cmp    dword ptr [esi + 0xae], 0
        je     loc_1_2800
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2800:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        mov    al, byte ptr [edi + 0x62]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 0x60]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    loc_1_2840
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_2840:
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        je     loc_1_297A
        lea    esp, [esp]
loc_1_2850:
        cmp    dword ptr [esi + 0xae], 0
        jne    loc_1_297A
        cmp    dword ptr [ebp - 0x10], 0xa
        jbe    loc_1_288C
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        push   esi
        call   sub_1_26D0
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        mov    dword ptr [ebp - 0x10], 0
loc_1_288C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        inc    dword ptr [ebp - 0x10]
        mov    ax, word ptr [edi + eax*2 + 0x70]
        mov    word ptr [ebp - 6], ax
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, dword ptr [esi + 0xf0]
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        mov    ax, word ptr [ecx + 4]
        mov    ebx, dword ptr [esi + 0x42]
        mov    edx, eax
        and    edx, 0xffff
        add    edx, dword ptr [esi + 0xf0]
        mov    ax, word ptr [ecx + 0xc]
        and    eax, 0xffff
        cmp    ebx, eax
        jb     loc_1_28DD
        mov    ebx, eax
loc_1_28DD:
        mov    dword ptr [ebp - 0xc], edx
        or     edx, edx
        je     loc_1_2926
        mov    dword ptr [esi + 0x11d], ecx
        mov    dword ptr [esi + 0x121], eax
        cmp    eax, 0xe
        jae    loc_1_2903
        cmp    eax, 6
        jb     loc_1_2926
        push   0
        push   0
        push   0
        push   eax
        jmp    loc_1_2914
loc_1_2903:
        sub    eax, 0xe
        push   eax
        sub    ebx, 0xe
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0xe
        push   eax
        push   0xe
loc_1_2914:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        call   sub_1_00B0
loc_1_2926:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb4]
        out    dx, al
        inc    byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi + 0x62], al
        mov    al, byte ptr [edi + 0x270]
        xor    ecx, ecx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 6]
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [edi + ecx*2 + 0x280], ax
        mov    al, byte ptr [ebp - 1]
        inc    al
        mov    byte ptr [edi + 0x270], al
        mov    al, byte ptr [edi + 0x62]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 0x60]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    loc_1_2850
loc_1_297A:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb3]
        out    dx, al
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        call   sub_1_0290
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_29A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x10]
        add    ebx, 0xe
        mov    edi, dword ptr [ecx + 0x11d]
        mov    eax, dword ptr [ecx + 0x42]
        mov    dword ptr [ebp - 0x14], eax
        mov    ecx, dword ptr [ecx + 0x121]
        mov    eax, dword ptr [ebp + 0x14]
        lea    edx, [ebx + eax]
        cmp    edx, ecx
        jbe    loc_1_29D5
        sub    ecx, ebx
        mov    dword ptr [ebp - 0x24], ecx
        jmp    loc_1_29D8
loc_1_29D5:
        mov    dword ptr [ebp - 0x24], eax
loc_1_29D8:
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    dx, word ptr [ecx + 0xbe]
        mov    al, byte ptr [ecx + 0xb3]
        out    dx, al
        mov    eax, ebx
        sub    edx, edx
        div    dword ptr [ebp - 0x14]
        or     eax, eax
        je     loc_1_2A18
        mov    edx, eax
        lea    esp, [esp]
loc_1_2A00:
        mov    cx, word ptr [edi + 2]
        mov    edi, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    edi, 0xffff
        add    edi, dword ptr [ecx + 0xf0]
        dec    edx
        jne    loc_1_2A00
loc_1_2A18:
        mov    ecx, dword ptr [ebp - 0x14]
        imul   ecx, eax
        sub    ebx, ecx
        mov    ax, word ptr [edi + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        add    eax, dword ptr [ecx + 0xf0]
        mov    dword ptr [ebp - 0x20], eax
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0530
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0554
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_2B0D
loc_1_2A6A:
        mov    esi, dword ptr [ebp - 0x18]
        sub    esi, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0xc], esi
        jae    loc_1_2A78
        mov    esi, dword ptr [ebp - 0xc]
loc_1_2A78:
        lea    eax, [ebx + esi]
        cmp    eax, dword ptr [ebp - 0x14]
        jbe    loc_1_2A85
        mov    esi, dword ptr [ebp - 0x14]
        sub    esi, ebx
loc_1_2A85:
        push   esi
        mov    eax, dword ptr [ebp - 0x20]
        add    eax, ebx
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_05EC
        add    ebx, esi
        sub    dword ptr [ebp - 0xc], esi
        je     loc_1_2B0D
        cmp    dword ptr [ebp - 0x14], ebx
        jne    loc_1_2ACD
        mov    ax, word ptr [edi + 2]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, eax
        and    edi, 0xffff
        add    edi, dword ptr [ecx + 0xf0]
        sub    ebx, ebx
        mov    ax, word ptr [edi + 4]
        and    eax, 0xffff
        add    eax, dword ptr [ecx + 0xf0]
        mov    dword ptr [ebp - 0x20], eax
loc_1_2ACD:
        add    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_2B03
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_2B0D
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0554
        mov    dword ptr [ebp - 8], 0
loc_1_2B03:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2A6A
loc_1_2B0D:
        mov    ecx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp + 0x1c]
        sub    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x20]
        cmp    byte ptr [esi + 0x24], 0
        je     loc_1_2B50
        mov    eax, 0xc0010002
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B50:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x10]
        or     edx, edx
        jne    loc_1_2B6A
        push   eax
        push   esi
        push   ecx
        call   0x2f50
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B6A:
        cmp    edx, 1
        jne    loc_1_2B95
        cmp    dword ptr [ecx + 0xae], 0
        je     loc_1_2B85
        mov    eax, 0xc001000d
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B85:
        push   eax
        push   esi
        push   ecx
        call   sub_1_2FA0
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    8
loc_1_2B95:
        mov    eax, 0x10001
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BB0:
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
        ja     loc_1_2C00
        je     loc_1_2DA0
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2C21
        shl    eax, 2
        jmp    loc_1_2F04
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C00:
        cmp    eax, 0x1010102
        je     loc_1_2DC0
        cmp    eax, 0x1010103
        je     loc_1_2DE0
        cmp    eax, 0x1010104
        je     loc_1_2EB0
loc_1_2C21:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2EB9
        db  08Dh, 049h, 000h
loc_1_2C30:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2C45
        mov    esi, 0x55e0                   ; -> loc_1_55E0
        mov    edi, 0x5c
        jmp    loc_1_2EB9
loc_1_2C45:
        mov    esi, 0x5564                   ; -> loc_1_5564
        mov    edi, 0x7c
        jmp    loc_1_2EB9
loc_1_2C54:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0xae], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_1_2C6E
        mov    dword ptr [ebp - 0x14], 0
loc_1_2C6E:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2EB9
        db  005h, 000h, 000h, 000h, 000h
loc_1_2C80:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2EB9
        db  08Dh, 049h, 000h
loc_1_2C90:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x42]
        sub    eax, 0xe
        jmp    loc_1_2EB6
        db  08Bh, 0FFh
loc_1_2CA0:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2EB9
        db  08Dh, 064h, 024h, 000h
loc_1_2CB0:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2EB9
        db  08Dh, 064h, 024h, 000h
loc_1_2CC0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x68]
        jmp    loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CD0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x6c]
        jmp    loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CE0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x70]
        jmp    loc_1_2EB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_2CF0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x74]
        jmp    loc_1_2EB6
loc_1_2CFB:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x62
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    loc_1_2EB9
loc_1_2D21:
        mov    esi, 0x563c                   ; -> loc_1_563C
        mov    edi, 0x15
        jmp    loc_1_2EB9
loc_1_2D30:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2D47
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xca]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_2EB6
loc_1_2D47:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_1_2EB6
loc_1_2D55:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2D69
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x98]
        jmp    loc_1_2EB6
loc_1_2D69:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        jmp    loc_1_2EB6
loc_1_2D74:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_1_2EB9
loc_1_2D87:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2EB9
loc_1_2D93:
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_2EB9
        db  090h
loc_1_2DA0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x62
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2EB9
        db  090h
loc_1_2DC0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x5c
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2EB9
        db  090h
loc_1_2DE0:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_2E46
        mov    eax, dword ptr [ebx + 0xca]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2E15
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2EB9
loc_1_2E15:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebx + 0xca]
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
        je     loc_1_2EB9
        jmp    loc_1_2EB9
loc_1_2E46:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        mov    eax, dword ptr [ebx + 0xca]
        push   eax
        call   sub_2_0660
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2E79
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2EB9
loc_1_2E79:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        mov    eax, dword ptr [ebx + 0xca]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_2EB9
        db  005h, 000h, 000h, 000h, 000h
loc_1_2EB0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x32]
loc_1_2EB6:
        mov    dword ptr [ebp - 8], eax
loc_1_2EB9:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2EF5
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_2EF5
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_2EE5
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
loc_1_2EE5:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_2EF5:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
loc_1_2F04:
        xor    byte ptr [eax + eax], ch
        add    byte ptr [esp + ebp], dl      ; -> loc_1_2C54
        add    byte ptr [eax - 0x7fffffd4], al ; -> loc_1_2C80
        sub    al, 0
        add    byte ptr [eax - 0x5fffffd4], dl ; -> loc_1_2C90
        sub    al, 0
        add    byte ptr [eax - 0x3fffffd4], dh ; -> loc_1_2CB0
        sub    al, 0
        add    al, dl                        ; -> loc_1_2CD0
        sub    al, 0
        add    al, ah                        ; -> loc_1_2CE0
        sub    al, 0
        add    al, dh                        ; -> loc_1_2CF0
        sub    al, 0
        add    bl, bh                        ; -> loc_1_2CFB
        sub    al, 0
        add    byte ptr [ecx], ah            ; -> loc_1_2D21
        sub    eax, 0x2d300000               ; -> loc_1_2D30
        add    byte ptr [eax], al
        push   ebp
        sub    eax, 0x2d740000               ; -> loc_1_2D74
        add    byte ptr [eax], al
        xchg   dword ptr [0x2c210000], ebp   ; -> loc_1_2C21
        add    byte ptr [eax], al
        xchg   ebx, eax
        sub    eax, 0x8b550000
        in     al, dx
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
        call   sub_1_2BB0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2FA0:
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
        je     loc_1_2FF7
        cmp    eax, 0x1010f
        je     loc_1_3058
        cmp    eax, 0x10112
        je     loc_1_30E0
        cmp    eax, 0x1010103
        je     loc_1_30E4
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_3125
loc_1_2FF7:
        cmp    edi, 4
        je     loc_1_3014
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_3125
loc_1_3014:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_3041
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_3125
loc_1_3041:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3140
        jmp    loc_1_3123
loc_1_3058:
        cmp    edi, 4
        je     loc_1_3075
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_3125
loc_1_3075:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x42]
        sub    eax, 0xe
        cmp    eax, dword ptr [ebp - 4]
        jb     loc_1_30D8
        mov    eax, dword ptr [ecx + 0x98]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_30B0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x98], eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x1c], eax
        jmp    loc_1_3125
        db  08Bh, 0FFh
loc_1_30B0:
        mov    edx, dword ptr [ebp + 0xc]
        cmp    dword ptr [edx + 0x1c], eax
        jne    loc_1_30D0
        mov    eax, dword ptr [edx + 0x1c]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_30D0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x1c], eax
        push   ecx
        call   sub_1_3380
        jmp    loc_1_3125
        db  08Bh, 0FFh
loc_1_30D0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x1c], eax
        jmp    loc_1_3125
loc_1_30D8:
        mov    ebx, 0xc0000001
        jmp    loc_1_3125
        db  090h
loc_1_30E0:
        sub    ebx, ebx
        jmp    loc_1_3125
loc_1_30E4:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_1_3108
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_3125
loc_1_3108:
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_31A0
loc_1_3123:
        mov    ebx, eax
loc_1_3125:
        or     ebx, ebx
        jne    loc_1_3133
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_3133:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_3140:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xae], 0
        jne    loc_1_318A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_3180
        inc    dword ptr [esi + 0x20]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0xca]
        push   eax
        call   sub_2_05E0
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_1_3180:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_318A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xae], 0
        jne    loc_1_31F0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_31E2
        inc    dword ptr [esi + 0x20]
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0xca]
        push   eax
        call   sub_2_0300
        dec    dword ptr [esi + 0x20]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_31E2:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_1_31F0:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3200:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [eax + 0xf0]
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2BB0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_1_32EF
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_3289
        je     loc_1_32C0
        cmp    eax, 0x20101
        je     loc_1_32A7
        cmp    eax, 0x20102
        je     loc_1_32B0
        cmp    eax, 0x20103
        je     loc_1_32D0
        cmp    eax, 0x20104
        je     loc_1_32B8
        jmp    loc_1_329E
loc_1_3289:
        cmp    eax, 0x1020101
        je     loc_1_32D0
        cmp    eax, 0x1020102
        je     loc_1_32D0
        cmp    eax, 0x1020103
        je     loc_1_32D0
loc_1_329E:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_1_32D7
loc_1_32A7:
        mov    eax, dword ptr [ebx + 0x54]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_32D7
        db  090h
loc_1_32B0:
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_32D7
loc_1_32B8:
        mov    eax, dword ptr [ebx + 0x3c]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_32D7
loc_1_32C0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x94]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_32D7
        db  08Bh, 0FFh
loc_1_32D0:
        mov    dword ptr [ebp - 4], 0
loc_1_32D7:
        cmp    edi, 4
        jb     loc_1_32EF
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0xc], 4
loc_1_32EF:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3310:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 0x14]
        cmp    edx, 0x20105
        ja     loc_1_3345
        cmp    edx, 0x20101
        jae    loc_1_336A
        cmp    edx, 0x10101
        jb     loc_1_3365
        cmp    edx, 0x10111
        jbe    loc_1_336A
        cmp    edx, 0x10113
        je     loc_1_336A
        jmp    loc_1_3365
loc_1_3345:
        cmp    edx, 0x1010101
        jb     loc_1_3365
        cmp    edx, 0x1010104
        jbe    loc_1_336A
        cmp    edx, 0x1020101
        jb     loc_1_3365
        cmp    edx, 0x1020103
        jbe    loc_1_336A
loc_1_3365:
        mov    eax, 0xc0010017
loc_1_336A:
        or     eax, eax
        jne    loc_1_3378
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3200
loc_1_3378:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3380:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0xa6]
        cmp    esi, edx
        je     loc_1_33A3
loc_1_3393:
        mov    eax, dword ptr [esi + 0x1c]
        cmp    edx, eax
        jae    loc_1_339C
        mov    edx, eax
loc_1_339C:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_3393
loc_1_33A3:
        mov    dword ptr [ecx + 0x98], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_33B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xbc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x7c], 8
        mov    word ptr [ebp - 0x7a], 8
        mov    dword ptr [ebp - 0x78], 0x56e4 ; -> obj1:0x56E4
        mov    word ptr [ebp - 0x6c], 0xa
        mov    word ptr [ebp - 0x6a], 0xa
        mov    dword ptr [ebp - 0x68], 0x56f8 ; -> loc_1_56F8
        mov    word ptr [ebp - 0xa4], 0xc
        mov    word ptr [ebp - 0xa2], 0xc
        mov    dword ptr [ebp - 0xa0], 0x5710 ; -> obj1:0x5710
        mov    word ptr [ebp - 0x64], 0xd
        mov    word ptr [ebp - 0x62], 0xd
        mov    dword ptr [ebp - 0x60], 0x572c ; -> obj1:0x572C
        mov    word ptr [ebp - 0x74], 0xd
        mov    word ptr [ebp - 0x72], 0xd
        mov    dword ptr [ebp - 0x70], 0x574c ; -> loc_1_574C
        mov    word ptr [ebp - 0x5c], 0xf
        mov    word ptr [ebp - 0x5a], 0xf
        mov    dword ptr [ebp - 0x58], 0x5784 ; -> obj1:0x5784
        mov    word ptr [ebp - 0x54], 0xd
        lea    eax, [ebp - 0x3b]
        mov    word ptr [ebp - 0x52], 0xd
        mov    dword ptr [ebp - 0x50], 0x57a4 ; -> obj1:0x57A4
        mov    word ptr [ebp - 0x4c], 0xc
        mov    word ptr [ebp - 0x4a], 0xc
        mov    esi, 0x368
        mov    dword ptr [ebp - 0x48], 0x57c0 ; -> obj1:0x57C0
        mov    byte ptr [ebp - 0x3c], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 0
        mov    edi, 0xd8000
        mov    dword ptr [ebp - 0x30], 0
        mov    dword ptr [ebp - 0x24], 0x8000
        mov    dword ptr [ebp - 0x20], 0x10
        mov    dword ptr [ebp - 0x1c], 0x200
        mov    dword ptr [ebp - 0x14], 4
        mov    dword ptr [ebp - 0x18], 0xa
        mov    bl, 5
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 0x9c], 6
        mov    word ptr [ebp - 0x9a], 6
        mov    dword ptr [ebp - 0x98], 0x57d4 ; -> obj1:0x57D4
        mov    word ptr [ebp - 0x94], 6
        mov    word ptr [ebp - 0x92], 6
        mov    dword ptr [ebp - 0x90], 0x57e4 ; -> obj1:0x57E4
        mov    word ptr [ebp - 0x8c], 0xb
        mov    word ptr [ebp - 0x8a], 0xb
        mov    dword ptr [ebp - 0x88], 0x5808 ; -> loc_1_5808
        mov    word ptr [ebp - 0x84], 8
        mov    word ptr [ebp - 0x82], 8
        mov    dword ptr [ebp - 0x80], 0x581c ; -> loc_1_581C
        mov    word ptr [ebp - 0xac], 8
        mov    word ptr [ebp - 0xaa], 8
        mov    dword ptr [ebp - 0xa8], 0x582c ; -> obj1:0x582C
        mov    dword ptr [ebp - 0x28], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3560
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_3560:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 0x34], 6
        jne    loc_1_3597
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3597
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x3c], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x38], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x37], al
loc_1_3597:
        push   1
        lea    eax, [ebp - 0xac]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3680
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 4], 3
        jne    loc_1_35D0
        mov    dword ptr [ebp - 0x14], 3
        jmp    loc_1_3680
loc_1_35D0:
        push   2
        lea    eax, [ebp - 0xa4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3680
        push   1
        lea    eax, [ebp - 0x9c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     loc_1_3616
        mov    dword ptr [ebp - 0x14], 0
        jmp    loc_1_3680
loc_1_3616:
        push   1
        lea    eax, [ebp - 0x94]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     loc_1_3638
        mov    dword ptr [ebp - 0x14], 2
        jmp    loc_1_3680
loc_1_3638:
        push   1
        lea    eax, [ebp - 0x8c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     loc_1_3660
        mov    dword ptr [ebp - 0x14], 4
        jmp    loc_1_3680
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3660:
        push   1
        lea    eax, [ebp - 0x84]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        call   sub_1_0596
        or     al, al
        je     loc_1_3680
        mov    dword ptr [ebp - 0x14], 5
loc_1_3680:
        cmp    dword ptr [ebp - 0x14], 3
        jne    loc_1_3701
        lea    eax, [ebp - 0x44]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0480
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_377E
        test   byte ptr [ebp - 0x42], 1
        je     loc_1_377E
        test   byte ptr [ebp - 0x42], 0x80
        mov    bl, 0xc
        jne    loc_1_36B9
        mov    bl, 3
loc_1_36B9:
        mov    dword ptr [ebp - 0x28], 2
        mov    al, byte ptr [ebp - 0x42]
        and    al, 0x60
        shr    al, 5
        cmp    al, 3
        je     loc_1_377E
        and    eax, 0xff
        mov    eax, dword ptr [eax*4 + 0x5654] ; -> loc_1_5654
        mov    dword ptr [ebp - 0x24], eax
        mov    al, byte ptr [ebp - 0x41]
        and    eax, 0xf
        mov    edi, dword ptr [eax*4 + 0x5664] ; -> obj1:0x5664
        mov    al, byte ptr [ebp - 0x3f]
        and    eax, 0xf
        xor    esi, esi
        mov    si, word ptr [eax*2 + 0x56a4] ; -> loc_1_56A4
        jmp    loc_1_37ED
loc_1_3701:
        push   1
        lea    eax, [ebp - 0x7c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3724
        mov    eax, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [eax + 4]
loc_1_3724:
        sub    al, al
loc_1_3726:
        xor    ecx, ecx
        mov    cl, al
        cmp    dword ptr [ecx*4 + 0x56c4], esi ; -> obj1:0x56C4
        je     loc_1_3739
        inc    al
        cmp    al, 4
        jb     loc_1_3726
loc_1_3739:
        cmp    al, 4
        je     loc_1_377E
        push   1
        lea    eax, [ebp - 0x74]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3760
        mov    eax, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [eax + 4]
loc_1_3760:
        cmp    dword ptr [ebp - 0x14], 4
        jne    loc_1_3795
        cmp    edi, 0x80000
        jb     loc_1_377E
        cmp    edi, 0xe8000
        ja     loc_1_377E
        test   edi, 0x7fff
        je     loc_1_37B2
loc_1_377E:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3795:
        cmp    edi, 0x88000
        jb     loc_1_377E
        cmp    edi, 0xe8000
        ja     loc_1_377E
        lea    eax, [edi + 0x8000]
        test   eax, 0xffff
        jne    loc_1_377E
loc_1_37B2:
        push   1
        lea    eax, [ebp - 0x6c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_37D5
        mov    eax, dword ptr [ebp - 0xc]
        mov    bl, byte ptr [eax + 4]
loc_1_37D5:
        sub    al, al
loc_1_37D7:
        xor    ecx, ecx
        mov    cl, al
        cmp    byte ptr [ecx + 0x56d4], bl   ; -> loc_1_56D4
        je     loc_1_37E9
        inc    al
        cmp    al, 7
        jb     loc_1_37D7
loc_1_37E9:
        cmp    al, 7
        je     loc_1_377E
loc_1_37ED:
        push   0
        lea    eax, [ebp - 0x64]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3813
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x20], eax
loc_1_3813:
        push   0
        lea    eax, [ebp - 0x5c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_384E
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], eax
        cmp    eax, 0x100
        jb     loc_1_3847
        cmp    eax, 0x5ea
        jbe    loc_1_384E
loc_1_3847:
        mov    dword ptr [ebp - 0x1c], 0x100
loc_1_384E:
        push   1
        lea    eax, [ebp - 0x54]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3885
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x18], eax
        cmp    eax, 4
        jb     loc_1_387E
        cmp    eax, 0x14
        jbe    loc_1_3885
loc_1_387E:
        mov    dword ptr [ebp - 0x18], 0xa
loc_1_3885:
        push   1
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_38AB
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 1], al
loc_1_38AB:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
        cmp    dword ptr [ebp - 0x20], 3
        jne    loc_1_38C1
        mov    dword ptr [ebp - 0x20], 4
loc_1_38C1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x2a], esi
        mov    byte ptr [ecx + 0x4b], bl
        mov    dword ptr [ecx + 0x2e], edi
        mov    dword ptr [ecx + 0x46], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ecx + 0x21], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [ecx + 0x50], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ecx + 0x32], eax
        mov    dword ptr [ecx + 0x36], 0xa
        mov    dword ptr [ecx + 0x3a], 6
        mov    dword ptr [ecx + 0x3e], 0x40
        add    ecx, 0x5c
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ecx - 0x1a], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx - 0x10], eax
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx - 0x12], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp - 0x38]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [ebp - 0x37]
        mov    byte ptr [ecx + 5], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_1_3930:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    word ptr [ebp - 0x14], 6
        mov    word ptr [ebp - 0x12], 6
        mov    dword ptr [ebp - 0x10], 0x59d4 ; -> obj1:0x59D4
        mov    word ptr [ebp - 0xc], 0
        mov    word ptr [ebp - 0xa], 1
        mov    word ptr [ebp - 8], 0x7012
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0x59e4                        ; -> loc_1_59E4
        call   sub_1_041A
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [0x5a1e], eax       ; -> loc_1_5A1E
        mov    dword ptr [0x5a22], ecx       ; -> loc_1_5A22
        mov    dword ptr [0x5a26], 0         ; -> loc_1_5A26
        mov    byte ptr [0x59e8], 3          ; -> loc_1_59E8
        mov    byte ptr [0x59e9], 0          ; -> obj1:0x59E9
        mov    dword ptr [0x59ea], 0         ; -> loc_1_59EA
        mov    dword ptr [0x59ee], 0x6a0     ; -> loc_1_59EE
        mov    dword ptr [0x59f2], 0xc70     ; -> loc_1_59F2
        mov    dword ptr [0x59f6], 0x2430    ; -> loc_1_59F6
        mov    dword ptr [0x59fa], 0x29a0    ; -> loc_1_59FA
        mov    dword ptr [0x59fe], 0xdc0     ; -> loc_1_59FE
        mov    dword ptr [0x5a02], 0x2b30    ; -> loc_1_5A02
        mov    dword ptr [0x5a06], 0x3310    ; -> loc_1_5A06
        mov    dword ptr [0x5a0a], 0x3f50    ; -> loc_1_5A0A
        mov    dword ptr [0x5a0e], 0x3a70    ; -> loc_1_5A0E
        mov    dword ptr [0x5a12], 0x3f80    ; -> loc_1_5A12
        mov    dword ptr [0x5a16], eax       ; -> loc_1_5A16
        mov    dword ptr [0x5a1a], ecx       ; -> loc_1_5A1A
        push   0x36
        push   0x59e8                        ; -> loc_1_59E8
        push   0x59e0                        ; -> obj1:0x59E0
        mov    eax, dword ptr [0x59e4]       ; -> loc_1_59E4
        push   eax
        lea    eax, [ebp - 4]
        push   0x59e0                        ; -> obj1:0x59E0
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_3A60
        cmp    dword ptr [0x5a26], 0         ; -> loc_1_5A26
        jbe    loc_1_3A48
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3A48:
        push   0
        mov    eax, dword ptr [0x59e4]       ; -> loc_1_59E4
        push   eax
        call   sub_1_0420
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3A60:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [0x4060]       ; -> obj1:0x4060
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x126
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_3AA0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_1_3AA0:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0x49
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_33B0
        or     eax, eax
        je     loc_1_3AE0
        push   0
        push   0x126
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3AE0:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3B20
        or     eax, eax
        jne    loc_1_3AF9
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3AF9:
        push   0
        push   0x126
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0010008
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    eax, dword ptr [esi + 0x46]
        cmp    eax, 2
        je     loc_1_3B50
        cmp    eax, 3
        je     loc_1_3B50
        cmp    eax, 4
        je     loc_1_3B50
        mov    bl, 1
        mov    dword ptr [ebp - 8], 0xaa00000a
        jmp    loc_1_3C01
        db  08Bh, 0FFh
loc_1_3B50:
        lea    edi, [esi + 0x108]
        mov    eax, dword ptr [0x4060]       ; -> obj1:0x4060
        push   eax
        mov    eax, dword ptr [esi + 0x4c]
        push   0
        lea    eax, [eax + eax*8]
        shl    eax, 4
        add    eax, 0xf0
        push   eax
        push   edi
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_3B90
        mov    dword ptr [edi], 0
        mov    bl, 1
        mov    dword ptr [ebp - 8], 0xaa000002
        jmp    loc_1_3C01
        db  005h, 000h, 000h, 000h, 000h
loc_1_3B90:
        mov    eax, dword ptr [esi + 0x4c]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, 0xa
        mov    dword ptr [esi + 0x36], eax
        mov    eax, dword ptr [esi + 0x50]
        dec    eax
        mov    dword ptr [esi + 0x54], eax
        not    eax
        mov    dword ptr [esi + 0x58], eax
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx*4 + 0x5873] ; -> loc_1_5873
        add    eax, dword ptr [esi + 0x2a]
        mov    dword ptr [esi + 0xbe], eax
        mov    ecx, dword ptr [esi + 0x46]
        mov    eax, ecx
        shl    ecx, 4
        add    ecx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx*4 + 0x5874] ; -> obj1:0x5874
        add    eax, dword ptr [esi + 0x2a]
        mov    dword ptr [esi + 0xc2], eax
        mov    eax, dword ptr [esi + 0x46]
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        mov    al, byte ptr [eax*4 + 0x5875] ; -> loc_1_5875
        and    eax, 0xff
        add    eax, dword ptr [esi + 0x2a]
        mov    dword ptr [esi + 0xc6], eax
loc_1_3C01:
        lea    edi, [ebp - 0x30]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    dword ptr [esi + 0x46], 3
        mov    dword ptr [ebp - 0x26], 3
        je     loc_1_3C23
        mov    dword ptr [ebp - 0x26], 1
loc_1_3C23:
        mov    dword ptr [ebp - 0x1a], 1
        mov    eax, dword ptr [esi + 0x2a]
        mov    dword ptr [ebp - 0x16], eax
        mov    dword ptr [ebp - 0x12], 4
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        mov    eax, dword ptr [0x59e0]       ; -> obj1:0x59E0
        push   eax
        lea    eax, [esi + 4]
        push   eax
        call   sub_1_0432
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_3EC5
        or     bl, bl
        je     loc_1_3C79
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        jmp    loc_1_3EBC
loc_1_3C79:
        mov    eax, dword ptr [esi + 0x50]
        push   eax
        mov    eax, dword ptr [esi + 0x2e]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esi + 0xf0]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_3CB7
        mov    eax, dword ptr [esi + 0x2e]
        push   eax
        push   1
        push   0xaa000001
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0x10
        jmp    loc_1_3EBC
loc_1_3CB7:
        push   esi
        call   sub_1_11E0
        or     al, al
        jne    loc_1_3CE0
        mov    dword ptr [ebp - 4], 0xc0000001
        push   0
        push   0xaa000004
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        jmp    loc_1_3EBC
loc_1_3CE0:
        push   esi
        call   sub_1_1340
        or     al, al
        jne    loc_1_3D24
        push   esi
        call   sub_1_1340
        or     al, al
        jne    loc_1_3D24
        mov    dword ptr [ebp - 4], 0xc0000001
        mov    eax, dword ptr [esi + 0x2e]
        push   eax
        mov    eax, dword ptr [esi + 0x2a]
        push   eax
        movsx  eax, byte ptr [esi + 0x4b]
        push   eax
        mov    eax, dword ptr [esi + 0x46]
        push   eax
        push   4
        push   0xaa000004
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0x1c
        jmp    loc_1_3EBC
loc_1_3D24:
        mov    al, byte ptr [esi + 0x5d]
        or     al, byte ptr [esi + 0x5e]
        or     al, byte ptr [esi + 0x5f]
        or     al, byte ptr [esi + 0x60]
        lea    edx, [esi + 0x5c]
        or     al, byte ptr [esi + 0x61]
        or     al, byte ptr [edx]
        jne    loc_1_3D4E
        mov    ecx, dword ptr [esi + 0x62]
        lea    eax, [esi + 0x62]
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 4], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 5], al
loc_1_3D4E:
        lea    eax, [esi + 0xca]
        push   eax
        push   esi
        push   edx
        push   0xa10                         ; -> loc_1_0A10
        push   0x950                         ; -> loc_1_0950
        push   0x8a0                         ; -> loc_1_08A0
        mov    eax, dword ptr [esi + 0x32]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_1_3D92
        mov    dword ptr [ebp - 4], 0xc0000001
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        jmp    loc_1_3EBC
loc_1_3D92:
        mov    dx, word ptr [esi + 0xbe]
        mov    al, byte ptr [esi + 0xb2]
        out    dx, al
        or     al, 2
        out    dx, al
        mov    byte ptr [esi + 0x29], 1
        lea    ebx, [esi + 9]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        movsx  eax, byte ptr [esi + 0x4b]
        mov    ecx, dword ptr [esi + 0x21]
        push   ecx
        push   1
        push   eax
        push   eax
        push   0x23a0                        ; -> loc_1_23A0
        push   esi
        push   0x2320                        ; -> loc_1_2320
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   ebx
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_3E00
        movsx  eax, byte ptr [esi + 0x4b]
        push   eax
        push   1
        push   0xaa000005
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0x10
        jmp    loc_1_3EB0
        db  08Dh, 049h, 000h
loc_1_3E00:
        mov    dword ptr [esi + 0x9e], 0x59e0 ; -> obj1:0x59E0
        mov    dword ptr [esi + 0xaa], 0
        mov    dword ptr [esi + 0xa6], 0
        mov    eax, dword ptr [0x5a2a]       ; -> loc_1_5A2A
        mov    dword ptr [esi + 0xa2], eax
        mov    dword ptr [0x5a2a], esi       ; -> loc_1_5A2A
        mov    dword ptr [esi + 0xa6], 0
        push   esi
        call   sub_1_1B70
        or     al, al
        jne    loc_1_3EF0
        mov    dword ptr [ebp - 4], 0xc0000001
        push   0
        push   0xaa000003
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        push   ebx
        call   sub_1_04F4
        cmp    dword ptr [0x5a2a], esi       ; -> loc_1_5A2A
        jne    loc_1_3E80
        mov    eax, dword ptr [esi + 0xa2]
        mov    dword ptr [0x5a2a], eax       ; -> loc_1_5A2A
        jmp    loc_1_3EB0
        db  08Dh, 064h, 024h, 000h
loc_1_3E80:
        mov    ecx, dword ptr [0x5a2a]       ; -> loc_1_5A2A
        cmp    dword ptr [ecx + 0xa2], esi
        je     loc_1_3E9E
        mov    edi, edi
loc_1_3E90:
        mov    ecx, dword ptr [ecx + 0xa2]
        cmp    dword ptr [ecx + 0xa2], esi
        jne    loc_1_3E90
loc_1_3E9E:
        mov    eax, dword ptr [ecx + 0xa2]
        mov    eax, dword ptr [eax + 0xa2]
        mov    dword ptr [ecx + 0xa2], eax
loc_1_3EB0:
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        call   sub_2_0120
loc_1_3EBC:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
loc_1_3EC5:
        mov    ecx, dword ptr [esi + 0x108]
        or     ecx, ecx
        je     loc_1_3EE1
        mov    eax, dword ptr [esi + 0x36]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 3
        push   eax
        push   ecx
        call   sub_1_050C
loc_1_3EE1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_3EF0:
        mov    dword ptr [esi + 0xe0], 0
        mov    dword ptr [esi + 0xe4], 0xa40 ; -> loc_1_0A40
        lea    eax, [esi + 0xce]
        mov    dword ptr [esi + 0xe8], esi
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0xd2], 0xad0 ; -> loc_1_0AD0
        mov    dword ptr [esi + 0xd6], esi
        push   0x2710
        push   eax
        call   sub_1_04E2
        mov    eax, dword ptr [esi + 0x9e]
        pop    edi
        inc    dword ptr [eax + 0x46]
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x59e0]       ; -> obj1:0x59E0
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [0x59e4]       ; -> loc_1_59E4
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3F80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 9]
        push   eax
        call   sub_1_04F4
        mov    byte ptr [esi + 8], 1
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0xce]
        push   eax
        call   sub_1_04E8
        cmp    dword ptr [0x5a2a], esi       ; -> loc_1_5A2A
        jne    loc_1_3FC0
        mov    eax, dword ptr [esi + 0xa2]
        mov    dword ptr [0x5a2a], eax       ; -> loc_1_5A2A
        jmp    loc_1_3FF0
        db  08Dh, 064h, 024h, 000h
loc_1_3FC0:
        mov    ecx, dword ptr [0x5a2a]       ; -> loc_1_5A2A
        cmp    dword ptr [ecx + 0xa2], esi
        je     loc_1_3FDE
        mov    edi, edi
loc_1_3FD0:
        mov    ecx, dword ptr [ecx + 0xa2]
        cmp    dword ptr [ecx + 0xa2], esi
        jne    loc_1_3FD0
loc_1_3FDE:
        mov    eax, dword ptr [ecx + 0xa2]
        mov    eax, dword ptr [eax + 0xa2]
        mov    dword ptr [ecx + 0xa2], eax
loc_1_3FF0:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4000
        push   0x1f4
        call   sub_1_0500
loc_1_4000:
        mov    eax, dword ptr [esi + 0xca]
        push   eax
        call   sub_2_0120
        cmp    dword ptr [esi + 0x46], 3
        je     loc_1_4026
        mov    dx, word ptr [esi + 0xbe]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 0xc2]
        sub    al, al
        out    dx, al
loc_1_4026:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0438
        mov    eax, dword ptr [esi + 0x36]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 3
        push   eax
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        call   sub_1_050C
        push   0
        push   0x126
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_4064:
        cli    
        jmp    0xff3240f6
        db  008h, 000h, 050h, 0FFh, 000h, 000h, 052h, 0FFh, 000h, 000h, 054h, 0FFh, 000h, 000h, 056h, 0FFh
        db  009h, 0C0h, 058h, 0FFh, 000h, 000h, 05Ah, 0FFh, 000h, 000h, 05Ch, 0FFh, 000h, 000h, 05Eh, 0FFh
        db  000h, 040h, 060h, 0FFh, 000h, 000h, 062h, 0FFh, 0D0h, 007h, 066h, 0FFh, 001h, 0C0h, 000h, 000h
        db  002h, 000h, 072h, 08Ah, 00Ch, 00Fh, 040h, 06Eh, 000h, 060h, 000h, 0F2h, 000h, 000h, 00Ch, 000h
        db  000h, 000h, 001h, 000h, 07Eh, 08Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        db  090h, 08Ah, 00Ch, 00Fh, 040h, 02Eh, 000h, 060h, 000h, 0F2h, 008h, 000h, 00Ch, 000h, 000h, 000h
        db  003h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 08Ah, 000h, 000h, 007h, 000h
        db  03Ch, 08Ah, 000h, 000h, 005h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h
        db  001h, 000h, 09Ch, 000h, 030h, 000h, 094h, 000h, 028h, 000h, 0FCh, 08Ch, 0D8h, 08Eh, 0C0h, 08Eh
        db  0D0h, 0BCh, 020h, 08Ah, 08Bh, 016h, 0AEh, 088h, 0A0h, 034h, 089h, 0EEh, 08Bh, 016h, 0B0h, 088h
        db  085h, 0D2h, 074h, 004h, 0A0h, 036h, 089h, 0EEh, 033h, 0C0h, 0A3h, 0A4h, 088h, 0B9h, 00Ch, 000h
        db  0BEh, 004h, 000h, 01Eh, 00Eh, 01Fh, 0ADh, 092h, 0ADh, 0F0h, 0EFh, 0E2h, 0F9h, 01Fh, 0E8h, 0E7h
        db  00Eh, 08Bh, 016h, 0ACh, 088h, 0B0h, 040h, 0EEh, 0E8h, 03Bh, 007h, 08Bh, 016h, 0AEh, 088h, 0A0h
        db  033h, 089h, 0EEh, 08Bh, 016h, 0B0h, 088h, 085h, 0D2h, 074h, 004h, 0A0h, 035h, 089h, 0EEh, 081h
        db  00Eh, 0A0h, 088h, 000h, 0A0h, 0EBh, 01Eh, 0C7h, 006h, 0C2h, 088h, 008h, 000h, 0BEh, 008h, 001h
        db  0BFh, 052h, 089h, 0B9h, 004h, 000h, 0F3h, 0A7h, 074h, 011h, 0E8h, 089h, 000h, 0EBh, 00Ch, 0FFh
        db  006h, 0F2h, 000h, 0EBh, 018h, 0FFh, 00Eh, 0C2h, 088h, 07Eh, 0DCh, 0BAh, 050h, 0FFh, 0EDh, 08Bh
        db  0D8h, 087h, 01Eh, 0A4h, 088h, 02Bh, 0C3h, 001h, 006h, 0F0h, 000h, 072h, 0E2h, 0E8h, 0CEh, 000h
        db  0BEh, 070h, 082h, 08Bh, 05Ch, 002h, 03Ah, 01Ch, 075h, 043h, 08Bh, 036h, 03Eh, 089h, 081h, 03Ch
        db  000h, 0A0h, 074h, 03Eh, 0F7h, 006h, 000h, 001h, 040h, 000h, 074h, 03Bh, 083h, 03Eh, 0CEh, 088h
        db  000h, 07Fh, 039h, 08Bh, 036h, 04Eh, 080h, 08Bh, 044h, 002h, 03Ah, 004h, 075h, 033h, 033h, 0C0h
        db  039h, 006h, 0E0h, 000h, 075h, 0AFh, 039h, 006h, 0E2h, 000h, 074h, 0A9h, 0A2h, 0E3h, 000h, 08Bh
        db  016h, 0ACh, 088h, 0B0h, 040h, 0C6h, 006h, 0E1h, 000h, 0FFh, 0EEh, 0EBh, 098h, 0E8h, 0F8h, 004h
        db  0EBh, 0B8h, 0E8h, 028h, 003h, 0EBh, 0BDh, 0E8h, 0D0h, 005h, 0EBh, 0C0h, 0E8h, 044h, 006h, 0EBh
        db  0C2h, 0E8h, 078h, 00Ah, 0EBh, 0C8h, 08Bh, 044h, 0FEh, 08Bh, 0D0h, 03Dh, 0FFh, 0FFh, 075h, 005h
        db  033h, 0D2h, 089h, 054h, 0FEh, 02Bh, 045h, 0FEh, 089h, 055h, 0FEh, 08Bh, 0D9h, 0D1h, 0E3h, 02Eh
        db  08Bh, 097h, 080h, 000h, 009h, 016h, 098h, 000h, 02Eh, 08Bh, 09Fh, 088h, 000h, 033h, 0D2h, 001h
        db  007h, 011h, 057h, 002h, 001h, 006h, 03Ch, 000h, 011h, 016h, 03Eh, 000h, 001h, 006h, 024h, 000h
        db  011h, 016h, 026h, 000h, 083h, 0F9h, 001h, 074h, 008h, 001h, 006h, 050h, 000h, 011h, 016h, 052h
        db  000h, 0E3h, 004h, 0F3h, 0A7h, 075h, 0AFh, 0C3h, 08Bh, 0EAh, 0D1h, 0E5h, 08Bh, 06Ah, 010h, 089h
        db  02Dh, 089h, 05Dh, 002h, 088h, 044h, 002h, 001h, 00Eh, 0EAh, 000h, 0EBh, 070h, 090h, 08Bh, 036h
        db  01Eh, 080h, 08Bh, 054h, 002h, 08Bh, 004h, 03Ah, 0D0h, 074h, 027h, 08Bh, 0C8h, 02Ah, 0CAh, 08Bh
        db  0D8h, 0FEh, 0CBh, 0D1h, 0E3h, 08Bh, 058h, 010h, 0C6h, 047h, 003h, 080h, 0BFh, 046h, 089h, 083h
        db  03Dh, 000h, 074h, 0C4h, 087h, 05Dh, 002h, 0C6h, 047h, 003h, 000h, 088h, 044h, 002h, 001h, 00Eh
        db  0EAh, 000h, 0E9h, 0BBh, 000h, 03Bh, 01Eh, 0C4h, 088h, 075h, 025h, 0A1h, 0F0h, 000h, 02Bh, 006h
        db  0C6h, 088h, 03Bh, 006h, 0C8h, 088h, 07Eh, 015h, 0FFh, 006h, 0F6h, 000h, 0E8h, 0D9h, 006h, 080h
        db  067h, 001h, 0DFh, 080h, 04Fh, 001h, 088h, 0C7h, 006h, 0C4h, 088h, 000h, 000h, 0E9h, 023h, 001h
        db  089h, 01Eh, 0C4h, 088h, 0A1h, 0F0h, 000h, 0A3h, 0C6h, 088h, 0E9h, 016h, 001h, 08Bh, 01Eh, 046h
        db  089h, 085h, 0DBh, 074h, 03Ch, 0F6h, 047h, 001h, 040h, 075h, 0BAh, 0BFh, 000h, 001h, 0F7h, 005h
        db  000h, 002h, 075h, 02Dh, 083h, 07Dh, 002h, 000h, 075h, 027h, 0F7h, 006h, 0A0h, 088h, 000h, 004h
        db  075h, 01Fh, 0F6h, 047h, 001h, 0C0h, 075h, 019h, 089h, 05Dh, 004h, 0C7h, 045h, 002h, 000h, 001h
        db  08Bh, 016h, 0A8h, 088h, 0EEh, 0FFh, 006h, 0ECh, 000h, 0EBh, 006h, 090h, 0EBh, 0BFh, 0E9h, 0B9h
        db  000h, 0E9h, 0CFh, 000h, 0E9h, 0E0h, 000h, 0E9h, 037h, 001h, 0E9h, 0C7h, 000h, 087h, 046h, 000h
        db  088h, 047h, 00Bh, 0E8h, 02Eh, 011h, 089h, 056h, 000h, 0EBh, 078h, 090h, 011h, 054h, 04Ah, 0EBh
        db  05Dh, 090h, 011h, 054h, 046h, 0EBh, 05Dh, 090h, 011h, 054h, 02Eh, 0EBh, 066h, 090h, 011h, 054h
        db  014h, 072h, 005h, 084h, 0C0h, 0EBh, 054h, 090h, 011h, 054h, 016h, 084h, 0C0h, 0EBh, 04Ch, 090h
        db  08Bh, 02Eh, 046h, 089h, 085h, 0EDh, 074h, 0B9h, 0F6h, 046h, 001h, 080h, 074h, 0AEh, 08Eh, 006h
        db  0ECh, 088h, 08Bh, 05Eh, 006h, 033h, 0C0h, 0BEh, 010h, 000h, 08Bh, 07Fh, 004h, 026h, 08Bh, 055h
        db  004h, 03Bh, 016h, 07Ch, 08Ah, 074h, 09Dh, 026h, 0F6h, 005h, 001h, 075h, 09Ah, 099h, 087h, 046h
        db  000h, 0A9h, 03Fh, 05Bh, 075h, 097h, 08Bh, 04Fh, 008h, 001h, 04Ch, 048h, 072h, 09Eh, 0F9h, 011h
        db  054h, 044h, 072h, 09Eh, 0BEh, 090h, 000h, 0D0h, 0E0h, 072h, 0A3h, 074h, 006h, 0F9h, 011h, 054h
        db  02Ch, 072h, 095h, 042h, 001h, 016h, 0EEh, 000h, 0BEh, 080h, 084h, 000h, 014h, 029h, 016h, 0EAh
        db  000h, 074h, 01Eh, 08Bh, 06Eh, 004h, 0F6h, 046h, 001h, 080h, 075h, 0A6h, 089h, 02Eh, 046h, 089h
        db  089h, 016h, 0C4h, 088h, 039h, 016h, 0E4h, 000h, 073h, 012h, 08Ch, 0D8h, 08Eh, 0C0h, 0E9h, 0FCh
        db  0FEh, 088h, 076h, 003h, 0C7h, 006h, 046h, 089h, 000h, 000h, 0EBh, 0E4h, 0C6h, 006h, 0E2h, 000h
        db  0FFh, 0EBh, 0E7h, 0C3h, 0E8h, 06Dh, 010h, 0F6h, 046h, 001h, 020h, 074h, 005h, 0E8h, 0AAh, 006h
        db  033h, 0D2h, 089h, 056h, 000h, 0EBh, 0ACh, 0BEh, 078h, 08Ah, 0B9h, 002h, 000h, 0F3h, 0A7h, 08Bh
        db  07Fh, 004h, 075h, 002h, 00Ch, 001h, 026h, 023h, 015h, 026h, 023h, 055h, 002h, 042h, 075h, 005h
        db  00Ch, 002h, 0EBh, 029h, 090h, 026h, 0F6h, 005h, 001h, 074h, 022h, 00Ch, 020h, 08Bh, 00Eh, 096h
        db  08Ah, 0E3h, 01Ah, 08Bh, 0D1h, 0BEh, 098h, 08Ah, 057h, 0B9h, 006h, 000h, 0F3h, 0A6h, 05Fh, 074h
        db  00Ah, 003h, 0F1h, 083h, 0EAh, 006h, 075h, 0F0h, 0EBh, 003h, 090h, 00Ch, 011h, 088h, 047h, 00Bh
        db  0BEh, 010h, 000h, 084h, 044h, 00Ah, 075h, 09Ch, 0E8h, 009h, 010h, 089h, 056h, 000h, 0E9h, 052h
        db  0FFh, 026h, 023h, 015h, 026h, 023h, 055h, 002h, 042h, 075h, 0BAh, 0F6h, 044h, 00Ah, 002h, 075h
        db  024h, 087h, 046h, 000h, 0A9h, 03Fh, 05Bh, 075h, 023h, 08Bh, 04Fh, 008h, 001h, 04Ch, 054h, 072h
        db  009h, 0F9h, 011h, 054h, 050h, 072h, 008h, 0E9h, 00Fh, 0FFh, 011h, 054h, 056h, 0EBh, 0F2h, 011h
        db  054h, 052h, 0E9h, 004h, 0FFh, 0C6h, 047h, 00Bh, 002h, 0E9h, 058h, 0FFh, 089h, 046h, 000h, 0C6h
        db  047h, 00Bh, 002h, 0E8h, 0BEh, 00Fh, 0E9h, 00Ah, 0FFh, 050h, 08Bh, 0C3h, 08Bh, 05Fh, 004h, 0F6h
        db  007h, 001h, 075h, 00Dh, 08Bh, 0D8h, 0C6h, 047h, 00Eh, 000h, 0E8h, 042h, 010h, 058h, 0E9h, 088h
        db  001h, 08Bh, 00Fh, 023h, 04Fh, 002h, 023h, 04Fh, 004h, 041h, 08Bh, 0D8h, 0C6h, 047h, 00Eh, 010h
        db  075h, 0E8h, 0C6h, 047h, 00Eh, 002h, 0EBh, 0E2h, 0C3h, 0FFh, 006h, 0F4h, 000h, 080h, 04Fh, 001h
        db  080h, 080h, 0E5h, 03Fh, 0EBh, 016h, 090h, 0F6h, 047h, 009h, 080h, 075h, 0ECh, 080h, 0E5h, 03Fh
        db  003h, 0D1h, 08Bh, 05Fh, 002h, 08Bh, 00Fh, 080h, 0C5h, 040h, 073h, 0EBh, 003h, 0CAh, 093h, 089h
        db  04Fh, 00Ch, 03Bh, 00Eh, 020h, 089h, 077h, 0A1h, 033h, 0D2h, 0F9h, 0EBh, 042h, 090h, 0E9h, 038h
        db  001h, 011h, 054h, 016h, 0EBh, 03Eh, 090h, 011h, 054h, 01Eh, 0EBh, 03Dh, 090h, 08Bh, 02Eh, 03Eh
        db  089h, 081h, 07Eh, 000h, 000h, 0A0h, 075h, 0B0h, 0BEh, 010h, 000h, 033h, 0D2h, 0BBh, 060h, 080h
        db  08Bh, 03Fh, 0D1h, 0E7h, 08Dh, 079h, 010h, 089h, 03Eh, 0CAh, 088h, 090h, 089h, 056h, 000h, 08Bh
        db  046h, 006h, 0ABh, 08Bh, 0D8h, 08Bh, 00Fh, 080h, 0C5h, 040h, 073h, 09Bh, 089h, 04Fh, 00Ch, 011h
        db  054h, 014h, 072h, 0BDh, 001h, 04Ch, 01Ch, 072h, 0BEh, 08Bh, 05Fh, 004h, 0F6h, 007h, 001h, 075h
        db  068h, 0F6h, 044h, 00Ah, 005h, 074h, 0A7h, 0F9h, 010h, 016h, 060h, 080h, 072h, 047h, 04Ah, 089h
        db  056h, 006h, 042h, 08Bh, 0CDh, 08Bh, 06Eh, 004h, 081h, 07Eh, 000h, 000h, 0A0h, 074h, 0BDh, 093h
        db  08Bh, 047h, 002h, 0A3h, 042h, 089h, 0B8h, 0FFh, 0FFh, 0A2h, 0E2h, 000h, 023h, 006h, 0E0h, 000h
        db  074h, 028h, 089h, 02Eh, 03Eh, 089h, 08Bh, 0D9h, 0C6h, 047h, 003h, 080h, 087h, 01Eh, 040h, 089h
        db  03Bh, 0D9h, 074h, 004h, 0C6h, 047h, 003h, 000h, 081h, 026h, 0A0h, 088h, 0FFh, 0F7h, 0A1h, 0F0h
        db  000h, 0A3h, 0CCh, 088h, 0C3h, 0BFh, 070h, 080h, 0EBh, 0B4h, 0A2h, 0E3h, 000h, 08Bh, 016h, 0ACh
        db  088h, 0B0h, 040h, 0A2h, 0E1h, 000h, 0EEh, 0EBh, 0C9h, 08Bh, 017h, 023h, 057h, 002h, 023h, 057h
        db  004h, 042h, 075h, 013h, 0F9h, 011h, 054h, 028h, 072h, 05Bh, 001h, 04Ch, 03Ch, 072h, 05Bh, 0F6h
        db  044h, 00Ah, 006h, 074h, 064h, 0EBh, 080h, 033h, 0D2h, 0F9h, 011h, 054h, 024h, 072h, 050h, 001h
        db  04Ch, 038h, 072h, 050h, 08Bh, 00Eh, 096h, 08Ah, 0E3h, 021h, 0BEh, 098h, 08Ah, 08Bh, 017h, 02Bh
        db  014h, 075h, 010h, 08Bh, 057h, 002h, 02Bh, 054h, 002h, 075h, 008h, 08Bh, 057h, 004h, 02Bh, 054h
        db  004h, 074h, 016h, 083h, 0C6h, 006h, 083h, 0E9h, 006h, 075h, 0E2h, 08Bh, 0D1h, 0BEh, 010h, 000h
        db  0F6h, 044h, 00Ah, 004h, 074h, 023h, 0E9h, 03Eh, 0FFh, 0BEh, 010h, 000h, 0F6h, 044h, 00Ah, 005h
        db  074h, 017h, 0E9h, 032h, 0FFh, 011h, 054h, 02Ah, 0EBh, 0A0h, 011h, 054h, 03Eh, 0EBh, 0A0h, 011h
        db  054h, 026h, 0EBh, 0ABh, 011h, 054h, 03Ah, 0EBh, 0ABh, 04Fh, 04Fh, 0BEh, 042h, 089h, 093h, 08Bh
        db  047h, 002h, 089h, 004h, 08Bh, 04Eh, 006h, 03Bh, 0CBh, 074h, 00Dh, 08Bh, 0D9h, 089h, 017h, 08Bh
        db  05Fh, 002h, 0F6h, 047h, 001h, 080h, 074h, 0F5h, 089h, 017h, 0B8h, 010h, 001h, 089h, 047h, 002h
        db  080h, 04Fh, 009h, 080h, 039h, 004h, 074h, 033h, 087h, 05Ch, 002h, 089h, 04Fh, 002h, 0EBh, 000h
        db  0EBh, 000h, 0EBh, 000h, 080h, 067h, 009h, 07Fh, 0BEh, 010h, 000h, 08Bh, 0CDh, 0C7h, 046h, 006h
        db  0FFh, 0FFh, 08Bh, 06Eh, 004h, 081h, 07Eh, 000h, 000h, 0A0h, 075h, 003h, 0E9h, 09Dh, 0FEh, 03Bh
        db  03Eh, 0CAh, 088h, 074h, 003h, 0E9h, 0DEh, 0FEh, 0E9h, 0E7h, 0FEh, 089h, 00Ch, 089h, 05Ch, 002h
        db  0EBh, 0D6h, 0E8h, 006h, 020h, 007h, 018h, 007h, 010h, 007h, 008h, 007h, 000h, 007h, 0F8h, 006h
        db  0F0h, 006h, 0C3h, 074h, 0FDh, 0B9h, 06Eh, 006h, 051h, 0B9h, 000h, 001h, 02Bh, 0C8h, 0C6h, 047h
        db  002h, 000h, 0EBh, 022h, 080h, 067h, 001h, 03Fh, 08Bh, 05Fh, 002h, 001h, 017h, 073h, 0F5h, 0EBh
        db  028h, 090h, 0BFh, 002h, 000h, 0E9h, 0BCh, 000h, 0BBh, 070h, 082h, 08Bh, 007h, 08Bh, 0C8h, 086h
        db  047h, 002h, 02Ah, 0C8h, 076h, 0CDh, 08Bh, 0F0h, 0D1h, 0E6h, 08Dh, 070h, 010h, 0ADh, 08Bh, 0D8h
        db  08Bh, 0E8h, 0BAh, 000h, 040h, 001h, 017h, 073h, 0CBh, 049h, 074h, 0D6h, 08Bh, 0F9h, 083h, 0E7h
        db  007h, 0D1h, 0E7h, 083h, 0C1h, 007h, 0D1h, 0E9h, 0D1h, 0E9h, 0D1h, 0E9h, 02Eh, 08Bh, 085h, 038h
        db  006h, 0BFh, 002h, 000h, 0FFh, 0E0h, 0B8h, 0F0h, 006h, 0EBh, 02Bh, 090h, 0B8h, 0F8h, 006h, 0EBh
        db  025h, 090h, 0B8h, 000h, 007h, 0EBh, 01Fh, 090h, 0B8h, 008h, 007h, 0EBh, 019h, 090h, 0B8h, 010h
        db  007h, 0EBh, 013h, 090h, 0B8h, 018h, 007h, 0EBh, 00Dh, 090h, 0B8h, 020h, 007h, 0EBh, 007h, 090h
        db  0B8h, 028h, 007h, 0EBh, 001h, 090h, 080h, 067h, 001h, 03Fh, 08Bh, 019h, 001h, 017h, 073h, 0F6h
        db  0FFh, 0E0h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0BCh, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0BAh, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B8h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0B6h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B4h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0B2h, 0ADh, 089h, 001h, 093h, 001h, 017h, 073h, 0B0h, 0ADh, 089h, 001h, 093h, 001h, 017h
        db  073h, 0AEh, 0E2h, 0BEh, 0B8h, 010h, 001h, 089h, 001h, 080h, 04Fh, 009h, 080h, 0BEh, 042h, 089h
        db  039h, 004h, 074h, 010h, 087h, 05Ch, 002h, 089h, 029h, 0EBh, 000h, 0EBh, 000h, 0EBh, 000h, 080h
        db  067h, 009h, 07Fh, 0C3h, 089h, 02Ch, 089h, 05Ch, 002h, 0C3h, 0F7h, 006h, 000h, 001h, 040h, 000h
        db  074h, 001h, 0C3h, 0F7h, 006h, 0A0h, 088h, 000h, 010h, 074h, 0F7h, 0E8h, 00Ah, 0FFh, 081h, 03Eh
        db  042h, 089h, 010h, 001h, 074h, 0ECh, 083h, 03Eh, 002h, 001h, 000h, 075h, 0E5h, 0E8h, 02Dh, 0FDh
        db  083h, 03Eh, 002h, 001h, 000h, 075h, 04Bh, 0BFh, 03Eh, 089h, 08Bh, 01Dh, 03Bh, 05Dh, 002h, 074h
        db  041h, 0BFh, 042h, 089h, 08Bh, 005h, 03Dh, 010h, 001h, 074h, 037h, 03Bh, 045h, 002h, 074h, 032h
        db  089h, 047h, 006h, 093h, 0BBh, 000h, 001h, 089h, 047h, 006h, 0C7h, 047h, 002h, 010h, 000h, 08Bh
        db  016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0F7h, 007h, 040h, 000h, 075h, 006h, 0E2h, 0F8h, 0FFh, 006h
        db  0A6h, 088h, 0FFh, 006h, 0E6h, 000h, 0A1h, 0F0h, 000h, 0A3h, 0CCh, 088h, 081h, 026h, 0A0h, 088h
        db  0FFh, 0F7h, 0C3h, 0F7h, 006h, 0A0h, 088h, 000h, 010h, 074h, 03Ah, 083h, 03Eh, 0CEh, 088h, 000h
        db  07Eh, 033h, 0F7h, 006h, 0A0h, 088h, 000h, 008h, 075h, 00Eh, 0A1h, 0F0h, 000h, 02Bh, 006h, 0CCh
        db  088h, 03Bh, 006h, 0CEh, 088h, 07Fh, 001h, 0C3h, 083h, 03Eh, 0EAh, 000h, 000h, 075h, 010h, 0F7h
        db  006h, 0A0h, 088h, 000h, 004h, 075h, 008h, 0FFh, 006h, 0F8h, 000h, 0E8h, 01Ah, 001h, 0C3h, 081h
        db  00Eh, 0A0h, 088h, 000h, 008h, 0C3h, 0E8h, 08Bh, 001h, 033h, 0C0h, 08Ch, 0D9h, 0C1h, 0E9h, 00Ch
        db  0BEh, 070h, 089h, 01Eh, 0C5h, 01Eh, 04Eh, 089h, 088h, 007h, 089h, 077h, 006h, 088h, 04Fh, 008h
        db  01Fh, 0BBh, 000h, 001h, 089h, 05Ch, 002h, 089h, 044h, 004h, 088h, 04Ch, 006h, 089h, 007h, 08Dh
        db  07Fh, 008h, 0B9h, 004h, 000h, 0F3h, 0ABh, 048h, 089h, 047h, 004h, 089h, 047h, 006h, 01Eh, 00Eh
        db  01Fh, 0BEh, 034h, 000h, 0BFh, 060h, 08Ah, 0B9h, 01Ch, 000h, 0EBh, 000h, 0F3h, 0A5h, 0BEh, 06Ch
        db  000h, 0BFh, 030h, 08Ah, 0B9h, 00Ah, 000h, 0EBh, 000h, 0F3h, 0A5h, 01Fh, 0BBh, 07Eh, 08Ah, 0A0h
        db  026h, 089h, 088h, 047h, 007h, 0A0h, 02Fh, 089h, 0C0h, 0E0h, 003h, 00Ah, 006h, 02Eh, 089h, 0C0h
        db  0E0h, 004h, 00Ah, 006h, 02Dh, 089h, 088h, 047h, 00Ah, 0A0h, 027h, 089h, 0C0h, 0E0h, 004h, 080h
        db  067h, 009h, 0CFh, 008h, 047h, 009h, 0A0h, 029h, 089h, 088h, 047h, 00Bh, 032h, 0C0h, 08Ah, 026h
        db  02Ch, 089h, 0C0h, 0E4h, 004h, 00Bh, 006h, 02Ah, 089h, 088h, 067h, 00Dh, 088h, 047h, 00Ch, 0A0h
        db  028h, 089h, 0C0h, 0E0h, 005h, 080h, 067h, 00Eh, 0DFh, 008h, 047h, 00Eh, 0A0h, 031h, 089h, 0C0h
        db  0E0h, 004h, 00Ah, 006h, 030h, 089h, 080h, 067h, 00Fh, 088h, 008h, 047h, 00Fh, 0B0h, 004h, 080h
        db  03Eh, 028h, 089h, 000h, 074h, 002h, 0B0h, 002h, 002h, 006h, 032h, 089h, 088h, 047h, 010h, 0F7h
        db  006h, 0A2h, 088h, 000h, 040h, 074h, 00Ah, 0B0h, 0BFh, 020h, 006h, 068h, 08Ah, 020h, 006h, 086h
        db  08Ah, 01Eh, 0C5h, 01Eh, 04Eh, 089h, 08Ah, 047h, 002h, 01Fh, 03Ch, 0CCh, 074h, 019h, 0A8h, 080h
        db  074h, 015h, 081h, 00Eh, 0A2h, 088h, 000h, 080h, 0BBh, 07Eh, 08Ah, 080h, 04Fh, 00Eh, 040h, 080h
        db  04Fh, 00Ah, 080h, 0C6h, 047h, 00Bh, 055h, 01Eh, 0C5h, 036h, 04Ah, 089h, 0BFh, 078h, 08Ah, 0B9h
        db  003h, 000h, 0EBh, 000h, 0F3h, 0A5h, 01Fh, 0C3h, 050h, 053h, 051h, 052h, 056h, 0E8h, 074h, 000h
        db  0BBh, 000h, 001h, 08Bh, 047h, 00Eh, 0C7h, 007h, 000h, 000h, 0BEh, 070h, 089h, 0C6h, 004h, 001h
        db  08Bh, 016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0E2h, 0FEh, 089h, 047h, 00Eh, 033h, 0C9h, 080h, 03Ch
        db  000h, 0E0h, 0FBh, 0BEh, 000h, 001h, 08Bh, 004h, 03Dh, 000h, 0A0h, 0E0h, 0F9h, 089h, 044h, 002h
        db  0EEh, 083h, 07Ch, 002h, 000h, 0E0h, 0FAh, 083h, 026h, 0A0h, 088h, 0BFh, 0BBh, 060h, 08Ah, 0C7h
        db  007h, 000h, 000h, 0F7h, 047h, 002h, 000h, 080h, 075h, 005h, 08Bh, 05Fh, 004h, 0EBh, 0F0h, 0C7h
        db  044h, 002h, 000h, 001h, 0C7h, 044h, 004h, 060h, 08Ah, 0EEh, 033h, 0C9h, 0F7h, 007h, 000h, 080h
        db  0E1h, 0FAh, 081h, 026h, 0A0h, 088h, 0FFh, 0F7h, 0A1h, 0F0h, 000h, 0A3h, 0CCh, 088h, 05Eh, 05Ah
        db  059h, 05Bh, 058h, 0C3h, 083h, 03Eh, 0AAh, 088h, 000h, 074h, 027h, 050h, 051h, 052h, 08Bh, 016h
        db  0AAh, 088h, 033h, 0C0h, 0EEh, 0B9h, 064h, 000h, 0E2h, 0FEh, 040h, 0EEh, 0B9h, 064h, 000h, 0E2h
        db  0FEh, 08Bh, 016h, 0AEh, 088h, 0A0h, 033h, 089h, 0EEh, 083h, 00Eh, 0A0h, 088h, 040h, 05Ah, 059h
        db  058h, 0C3h, 0F7h, 006h, 0A0h, 088h, 040h, 000h, 074h, 001h, 0C3h, 050h, 053h, 051h, 052h, 056h
        db  006h, 0C4h, 01Eh, 04Eh, 089h, 026h, 08Ah, 047h, 008h, 026h, 08Bh, 05Fh, 006h, 0C1h, 0E0h, 00Ch
        db  08Eh, 0C0h, 026h, 08Bh, 057h, 002h, 026h, 08Ah, 047h, 006h, 026h, 08Bh, 05Fh, 004h, 003h, 0DAh
        db  015h, 000h, 000h, 0C1h, 0E0h, 00Ch, 08Eh, 0C0h, 026h, 0C7h, 047h, 002h, 080h, 000h, 08Bh, 016h
        db  0A8h, 088h, 0BEh, 00Ah, 000h, 0EEh, 033h, 0C9h, 026h, 083h, 07Fh, 002h, 000h, 074h, 00Fh, 0E2h
        db  0F7h, 0FFh, 006h, 0FAh, 000h, 04Eh, 075h, 0EDh, 026h, 0C7h, 047h, 002h, 000h, 000h, 083h, 00Eh
        db  0A0h, 088h, 040h, 007h, 05Eh, 05Ah, 059h, 05Bh, 058h, 0C3h, 050h, 051h, 052h, 056h, 057h, 055h
        db  006h, 053h, 08Bh, 0EBh, 0E8h, 0CFh, 000h, 089h, 01Eh, 0D6h, 088h, 074h, 077h, 0C7h, 006h, 0D8h
        db  088h, 000h, 000h, 080h, 04Fh, 001h, 0C0h, 08Bh, 07Fh, 004h, 08Bh, 047h, 008h, 025h, 0FFh, 03Fh
        db  08Bh, 076h, 004h, 08Bh, 056h, 000h, 081h, 0E2h, 0FFh, 03Fh, 08Bh, 0C8h, 03Bh, 0CAh, 07Eh, 002h
        db  08Bh, 0CAh, 051h, 01Eh, 006h, 08Eh, 006h, 0EAh, 088h, 08Eh, 01Eh, 0ECh, 088h, 0D1h, 0E9h, 0EBh
        db  000h, 0F3h, 0A5h, 073h, 001h, 0A4h, 007h, 01Fh, 059h, 001h, 00Fh, 001h, 00Eh, 0D8h, 088h, 02Bh
        db  0C1h, 02Bh, 0D1h, 074h, 036h, 095h, 08Bh, 0EBh, 0E8h, 07Bh, 000h, 089h, 05Eh, 002h, 074h, 014h
        db  080h, 066h, 001h, 07Fh, 095h, 080h, 04Fh, 001h, 0C0h, 08Bh, 07Fh, 004h, 08Bh, 047h, 008h, 025h
        db  0FFh, 03Fh, 0EBh, 0B6h, 095h, 08Bh, 01Eh, 0D6h, 088h, 0FFh, 077h, 002h, 0E8h, 0F9h, 000h, 03Bh
        db  0D8h, 05Bh, 075h, 0F5h, 0FFh, 006h, 0DAh, 088h, 0EBh, 043h, 090h, 0F6h, 046h, 001h, 080h, 075h
        db  014h, 08Bh, 06Eh, 002h, 08Bh, 076h, 004h, 08Bh, 056h, 000h, 081h, 0E2h, 0FFh, 03Fh, 03Dh, 000h
        db  000h, 074h, 0B2h, 0EBh, 085h, 05Eh, 08Bh, 03Eh, 0D6h, 088h, 0A1h, 0D8h, 088h, 089h, 045h, 00Ch
        db  08Ah, 044h, 00Bh, 088h, 045h, 00Eh, 056h, 053h, 08Bh, 0DFh, 0E8h, 092h, 009h, 08Bh, 0C3h, 05Bh
        db  0E8h, 088h, 000h, 083h, 006h, 0DCh, 088h, 001h, 083h, 016h, 0DEh, 088h, 000h, 05Bh, 007h, 05Dh
        db  05Fh, 05Eh, 05Ah, 059h, 058h, 0C3h, 050h, 051h, 056h, 057h, 055h, 08Bh, 01Eh, 042h, 089h, 081h
        db  0FBh, 010h, 001h, 074h, 050h, 0B8h, 080h, 040h, 0BEh, 002h, 000h, 0BFh, 009h, 000h, 084h, 067h
        db  001h, 074h, 008h, 084h, 001h, 075h, 04Fh, 08Bh, 018h, 0EBh, 0F3h, 084h, 001h, 075h, 047h, 08Bh
        db  018h, 08Bh, 0EBh, 084h, 001h, 075h, 03Fh, 08Bh, 018h, 084h, 001h, 075h, 039h, 08Bh, 018h, 084h
        db  001h, 075h, 033h, 08Bh, 018h, 084h, 001h, 075h, 02Dh, 08Bh, 038h, 084h, 045h, 009h, 075h, 026h
        db  08Bh, 04Dh, 002h, 084h, 066h, 001h, 075h, 017h, 089h, 008h, 08Bh, 0DFh, 0C7h, 007h, 000h, 000h
        db  0C7h, 047h, 002h, 000h, 000h, 05Dh, 05Fh, 05Eh, 059h, 058h, 081h, 0FBh, 010h, 001h, 0C3h, 08Bh
        db  0DDh, 0BFh, 009h, 000h, 0EBh, 0A8h, 0BBh, 010h, 001h, 0EBh, 0EAh, 0C7h, 047h, 002h, 000h, 000h
        db  0BFh, 060h, 080h, 08Bh, 01Dh, 0D1h, 0E3h, 089h, 041h, 010h, 0FEh, 005h, 0C6h, 006h, 0E2h, 000h
        db  0FFh, 0A1h, 0E0h, 000h, 085h, 0C0h, 075h, 00Fh, 0A2h, 0E3h, 000h, 08Bh, 016h, 0ACh, 088h, 0B0h
        db  040h, 0C6h, 006h, 0E1h, 000h, 0FFh, 0EEh, 0C3h, 050h, 053h, 057h, 0C7h, 007h, 000h, 000h, 0C7h
        db  047h, 002h, 010h, 001h, 080h, 04Fh, 009h, 080h, 0BFh, 042h, 089h, 081h, 03Dh, 010h, 001h, 074h
        db  016h, 08Bh, 0C3h, 087h, 05Dh, 002h, 089h, 047h, 002h, 0EBh, 000h, 0EBh, 000h, 0EBh, 000h, 080h
        db  067h, 009h, 07Fh, 05Fh, 05Bh, 058h, 0C3h, 089h, 01Dh, 089h, 05Dh, 002h, 0EBh, 0F5h, 0E4h, 00Eh
        db  055h, 00Ch, 023h, 00Dh, 026h, 00Dh, 07Dh, 00Dh, 096h, 00Dh, 0B0h, 00Dh, 0CCh, 00Dh, 00Fh, 00Eh
        db  089h, 00Eh, 094h, 00Eh, 097h, 00Eh, 0D5h, 00Eh, 0D8h, 00Eh, 0DBh, 00Eh, 08Bh, 036h, 04Eh, 080h
        db  08Bh, 044h, 002h, 03Ah, 004h, 075h, 001h, 0C3h, 0BFh, 000h, 001h, 083h, 07Dh, 002h, 000h, 075h
        db  0F6h, 0F7h, 005h, 000h, 002h, 075h, 0F0h, 083h, 03Eh, 0EAh, 000h, 000h, 075h, 0E9h, 0F7h, 006h
        db  0A0h, 088h, 000h, 008h, 075h, 0E1h, 08Bh, 01Eh, 05Eh, 080h, 08Bh, 007h, 0FEh, 0C0h, 03Ah, 047h
        db  004h, 076h, 002h, 0B0h, 000h, 03Ah, 047h, 002h, 074h, 0CDh, 08Bh, 044h, 002h, 0F6h, 064h, 005h
        db  003h, 044h, 006h, 08Bh, 0E8h, 08Bh, 05Eh, 000h, 0D1h, 0E3h, 02Eh, 0FFh, 0A7h, 0E4h, 00Bh, 0F7h
        db  006h, 0A0h, 088h, 000h, 004h, 075h, 03Ch, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 074h, 003h, 0E8h
        db  0B6h, 0FCh, 0BBh, 030h, 08Ah, 033h, 0C0h, 089h, 007h, 0A3h, 036h, 08Ah, 0A3h, 03Ch, 08Ah, 0A3h
        db  042h, 08Ah, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h, 001h, 08Bh, 016h, 0A8h, 088h
        db  0EEh, 0A1h, 0F0h, 000h, 0A3h, 0D2h, 088h, 081h, 00Eh, 0A0h, 088h, 000h, 004h, 080h, 00Eh, 016h
        db  000h, 020h, 0C3h, 0F7h, 006h, 03Ch, 08Ah, 000h, 080h, 075h, 015h, 0A1h, 0F0h, 000h, 02Bh, 006h
        db  0D2h, 088h, 03Bh, 006h, 0D4h, 088h, 07Fh, 001h, 0C3h, 0FFh, 006h, 0D0h, 088h, 0E8h, 068h, 0FCh
        db  081h, 026h, 0A0h, 088h, 0FFh, 0FBh, 0B2h, 007h, 081h, 03Eh, 030h, 08Ah, 000h, 0A0h, 074h, 002h
        db  0B2h, 004h, 081h, 03Eh, 036h, 08Ah, 000h, 0A0h, 074h, 002h, 0B2h, 004h, 081h, 03Eh, 03Ch, 08Ah
        db  000h, 0A0h, 075h, 020h, 0A0h, 043h, 08Ah, 0A8h, 080h, 075h, 01Bh, 0A8h, 040h, 075h, 015h, 0A8h
        db  020h, 074h, 005h, 080h, 00Eh, 0C8h, 000h, 008h, 0A8h, 010h, 074h, 00Ah, 080h, 00Eh, 0C8h, 000h
        db  004h, 0EBh, 003h, 090h, 0B2h, 004h, 080h, 026h, 016h, 000h, 0F8h, 008h, 016h, 016h, 000h, 080h
        db  026h, 016h, 000h, 0DFh, 0B8h, 000h, 000h, 080h, 0FAh, 007h, 074h, 003h, 0B8h, 00Ah, 000h, 0F7h
        db  006h, 0A0h, 088h, 000h, 060h, 074h, 003h, 0E8h, 07Ah, 0FCh, 0E9h, 0CAh, 001h, 0E9h, 0B8h, 001h
        db  08Dh, 076h, 006h, 0BFh, 078h, 08Ah, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0BBh, 060h, 08Ah, 033h
        db  0C0h, 089h, 007h, 0A3h, 072h, 08Ah, 0A3h, 07Eh, 08Ah, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h
        db  030h, 081h, 00Eh, 080h, 08Ah, 000h, 080h, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h
        db  001h, 08Bh, 016h, 0A8h, 088h, 0EEh, 033h, 0C9h, 0F7h, 006h, 07Eh, 08Ah, 000h, 080h, 0E1h, 0F8h
        db  081h, 026h, 080h, 08Ah, 0FFh, 07Fh, 0B8h, 0FFh, 000h, 0F7h, 006h, 07Eh, 08Ah, 000h, 020h, 074h
        db  003h, 0B8h, 000h, 000h, 0E9h, 070h, 001h, 083h, 03Eh, 01Ah, 000h, 000h, 074h, 006h, 081h, 00Eh
        db  0A0h, 088h, 000h, 010h, 0E8h, 091h, 0FBh, 081h, 026h, 0A0h, 088h, 0FFh, 09Fh, 0E9h, 054h, 001h
        db  0E8h, 001h, 0FCh, 033h, 0C0h, 0A3h, 096h, 08Ah, 0A3h, 0E4h, 088h, 081h, 026h, 0A0h, 088h, 0FFh
        db  0EFh, 081h, 00Eh, 0A0h, 088h, 000h, 040h, 0E9h, 03Ah, 001h, 055h, 0E8h, 0E6h, 0FBh, 0E8h, 013h
        db  004h, 0C7h, 006h, 0E4h, 000h, 000h, 000h, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h, 003h, 0E8h
        db  056h, 0FBh, 05Dh, 0E9h, 01Eh, 001h, 08Bh, 046h, 004h, 0A9h, 0F8h, 0FFh, 075h, 035h, 085h, 0C0h
        db  074h, 00Eh, 0F7h, 006h, 0A0h, 088h, 000h, 060h, 075h, 006h, 081h, 00Eh, 0A0h, 088h, 000h, 010h
        db  0A3h, 01Ah, 000h, 0BBh, 07Eh, 08Ah, 0C7h, 007h, 000h, 000h, 080h, 067h, 00Eh, 0FCh, 0A9h, 002h
        db  000h, 075h, 004h, 080h, 04Fh, 00Eh, 002h, 0A9h, 004h, 000h, 074h, 004h, 080h, 04Fh, 00Eh, 001h
        db  0E9h, 036h, 0FFh, 0B8h, 007h, 000h, 0E9h, 0DEh, 000h, 09Ch, 0FCh, 051h, 052h, 050h, 056h, 057h
        db  0A1h, 0E0h, 088h, 0B9h, 006h, 000h, 0F7h, 0E1h, 0BFh, 098h, 08Ah, 08Bh, 0F7h, 003h, 0F0h, 08Bh
        db  0C8h, 0D1h, 0E9h, 0F3h, 0A5h, 0BBh, 090h, 08Ah, 08Bh, 046h, 004h, 089h, 047h, 006h, 05Fh, 05Eh
        db  058h, 05Ah, 059h, 09Dh, 0BBh, 090h, 08Ah, 0C7h, 007h, 000h, 000h, 0F7h, 006h, 0A0h, 088h, 000h
        db  060h, 075h, 020h, 089h, 01Eh, 004h, 001h, 0C7h, 006h, 002h, 001h, 000h, 001h, 08Bh, 016h, 0A8h
        db  088h, 0EEh, 033h, 0C9h, 0F7h, 007h, 000h, 080h, 0E1h, 0FAh, 0B8h, 0FFh, 000h, 0F7h, 007h, 000h
        db  020h, 074h, 003h, 0B8h, 000h, 000h, 0EBh, 07Fh, 090h, 08Dh, 07Fh, 008h, 003h, 07Fh, 006h, 08Dh
        db  075h, 0FAh, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0B8h, 006h, 000h, 001h, 047h, 006h, 0A3h, 0E4h
        db  088h, 0EBh, 0B4h, 08Dh, 076h, 006h, 0E8h, 0CFh, 000h, 073h, 0ACh, 0EBh, 05Ah, 090h, 0EBh, 054h
        db  090h, 033h, 0C0h, 0BBh, 090h, 000h, 08Dh, 07Fh, 004h, 08Dh, 04Fh, 040h, 02Bh, 0CFh, 0D1h, 0E9h
        db  0F3h, 0ABh, 048h, 089h, 047h, 020h, 089h, 047h, 022h, 089h, 047h, 030h, 089h, 047h, 032h, 089h
        db  047h, 034h, 089h, 047h, 036h, 040h, 0BBh, 010h, 000h, 08Dh, 07Fh, 014h, 08Dh, 04Fh, 064h, 02Bh
        db  0CFh, 0D1h, 0E9h, 0F3h, 0ABh, 048h, 089h, 047h, 034h, 089h, 047h, 036h, 0EBh, 016h, 090h, 0EBh
        db  013h, 090h, 0EBh, 004h, 090h, 0EBh, 00Dh, 090h, 0B8h, 009h, 000h, 0EBh, 00Ah, 090h, 0B8h, 008h
        db  000h, 0EBh, 004h, 090h, 0B8h, 000h, 000h, 08Bh, 04Eh, 002h, 096h, 08Bh, 01Eh, 04Eh, 080h, 08Bh
        db  047h, 002h, 0FEh, 0C0h, 03Ah, 047h, 004h, 076h, 002h, 0B0h, 000h, 088h, 047h, 002h, 08Bh, 01Eh
        db  05Eh, 080h, 08Bh, 007h, 0F6h, 067h, 005h, 003h, 047h, 006h, 096h, 089h, 044h, 002h, 089h, 00Ch
        db  08Bh, 007h, 0FEh, 0C0h, 03Ah, 047h, 004h, 076h, 002h, 0B0h, 000h, 088h, 007h, 0C6h, 006h, 0E2h
        db  000h, 0FFh, 0C3h, 0F6h, 004h, 001h, 074h, 026h, 0E8h, 055h, 000h, 075h, 021h, 08Bh, 047h, 006h
        db  02Bh, 006h, 0E4h, 088h, 03Bh, 006h, 0E2h, 088h, 07Dh, 019h, 005h, 006h, 000h, 089h, 047h, 006h
        db  0C7h, 006h, 0E4h, 088h, 000h, 000h, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0F8h, 0C3h, 0B8h, 007h
        db  000h, 0F9h, 0C3h, 0B8h, 008h, 000h, 0F9h, 0C3h, 0E8h, 025h, 000h, 074h, 01Eh, 08Bh, 077h, 006h
        db  083h, 0EEh, 006h, 02Bh, 036h, 0E4h, 088h, 089h, 077h, 006h, 0C7h, 006h, 0E4h, 088h, 000h, 000h
        db  08Dh, 070h, 008h, 0ADh, 0ABh, 0ADh, 0ABh, 0ADh, 0ABh, 0F8h, 0C3h, 0B8h, 007h, 000h, 0F9h, 0C3h
        db  0BBh, 090h, 08Ah, 08Bh, 047h, 006h, 08Dh, 07Fh, 008h, 085h, 0C0h, 074h, 016h, 02Bh, 006h, 0E4h
        db  088h, 056h, 0B9h, 003h, 000h, 0F3h, 0A7h, 05Eh, 074h, 00Ah, 0D1h, 0E1h, 003h, 0F9h, 02Dh, 006h
        db  000h, 075h, 0EEh, 0C3h, 083h, 0EFh, 006h, 0C3h, 0A1h, 0B6h, 088h, 0F7h, 0D0h, 0A3h, 0B8h, 088h
        db  0B9h, 010h, 000h, 0A1h, 0E8h, 088h, 0F7h, 0E1h, 005h, 0FDh, 014h, 072h, 06Ch, 025h, 0F0h, 0FFh
        db  089h, 016h, 0ECh, 088h, 0C1h, 026h, 0ECh, 088h, 00Ch, 0A3h, 0FCh, 088h, 08Bh, 0FAh, 08Bh, 0F0h
        db  023h, 006h, 0B8h, 088h, 003h, 006h, 0BAh, 088h, 013h, 016h, 0BCh, 088h, 08Bh, 0D8h, 08Bh, 0CAh
        db  02Bh, 0C6h, 01Bh, 0D7h, 08Bh, 036h, 0F2h, 088h, 083h, 0C6h, 003h, 083h, 0E6h, 0FCh, 0F7h, 0F6h
        db  08Bh, 0E8h, 085h, 0C0h, 075h, 004h, 089h, 01Eh, 0FCh, 088h, 083h, 03Eh, 0BAh, 088h, 000h, 074h
        db  01Ch, 08Bh, 0D1h, 033h, 0C0h, 042h, 02Bh, 0C3h, 01Bh, 0D1h, 0F7h, 036h, 0BAh, 088h, 08Bh, 0C8h
        db  0A1h, 0BAh, 088h, 08Bh, 016h, 0BCh, 088h, 0F7h, 0F6h, 0F7h, 0E1h, 003h, 0E8h, 083h, 0FDh, 020h
        db  076h, 003h, 0BDh, 020h, 000h, 089h, 02Eh, 0F4h, 088h, 0B8h, 000h, 080h, 083h, 03Eh, 0BCh, 088h
        db  000h, 075h, 009h, 03Bh, 006h, 0BAh, 088h, 076h, 003h, 0A1h, 0BAh, 088h, 02Dh, 020h, 001h, 08Bh
        db  00Eh, 0EEh, 088h, 083h, 0C1h, 003h, 080h, 0E1h, 0FCh, 083h, 0C1h, 010h, 033h, 0D2h, 0F7h, 0F1h
        db  0A3h, 0F0h, 088h, 089h, 016h, 0FEh, 088h, 02Dh, 0FFh, 000h, 07Eh, 00Ch, 0F7h, 0E1h, 001h, 006h
        db  0FEh, 088h, 0B8h, 0FFh, 000h, 0A3h, 0F0h, 088h, 08Bh, 00Eh, 0F0h, 088h, 0BEh, 020h, 001h, 089h
        db  036h, 0DEh, 000h, 0B8h, 010h, 000h, 0F7h, 0E1h, 003h, 0F0h, 083h, 0C6h, 00Fh, 083h, 0E6h, 0F0h
        db  089h, 036h, 0F6h, 088h, 08Ch, 01Eh, 0EAh, 088h, 0A1h, 0EEh, 088h, 005h, 003h, 000h, 024h, 0FCh
        db  0F7h, 0E1h, 003h, 0F0h, 089h, 036h, 03Ah, 089h, 0BFh, 098h, 08Ah, 0A1h, 0E0h, 088h, 0B9h, 006h
        db  000h, 0F7h, 0E1h, 0A3h, 0E2h, 088h, 03Dh, 012h, 000h, 075h, 003h, 005h, 006h, 000h, 0D1h, 0E0h
        db  003h, 0F8h, 0A1h, 0F0h, 088h, 0B9h, 008h, 000h, 0F7h, 0E1h, 08Bh, 0D8h, 083h, 0C3h, 00Fh, 083h
        db  0E3h, 0F0h, 03Bh, 01Eh, 0FEh, 088h, 077h, 016h, 029h, 01Eh, 0FEh, 088h, 083h, 0C6h, 00Fh, 083h
        db  0E6h, 0F0h, 089h, 036h, 0D6h, 000h, 003h, 0F0h, 089h, 036h, 03Ah, 089h, 0EBh, 00Ch, 083h, 0C7h
        db  00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 0D6h, 000h, 003h, 0F8h, 083h, 0C7h, 00Fh, 083h, 0E7h, 0F0h
        db  089h, 03Eh, 0F8h, 088h, 08Bh, 00Eh, 0F4h, 088h, 0B8h, 008h, 000h, 0F7h, 0E1h, 003h, 0F8h, 083h
        db  0C7h, 00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 0FAh, 088h, 0B8h, 010h, 000h, 0F7h, 0E1h, 003h, 0F8h
        db  08Bh, 00Eh, 0E6h, 088h, 041h, 0B8h, 00Ch, 000h, 0F7h, 0E1h, 005h, 010h, 000h, 083h, 0C7h, 00Fh
        db  083h, 0E7h, 0F0h, 089h, 03Eh, 04Eh, 080h, 003h, 0F8h, 0B8h, 004h, 000h, 0F7h, 0E1h, 005h, 010h
        db  000h, 083h, 0C7h, 00Fh, 083h, 0E7h, 0F0h, 089h, 03Eh, 05Eh, 080h, 003h, 0F8h, 089h, 03Eh, 03Ch
        db  089h, 0BFh, 098h, 08Ah, 08Bh, 00Eh, 03Ch, 089h, 02Bh, 0CFh, 033h, 0C0h, 0D1h, 0E9h, 0F3h, 0ABh
        db  0BBh, 080h, 084h, 089h, 01Eh, 00Eh, 080h, 0BEh, 090h, 086h, 089h, 036h, 01Eh, 080h, 08Ch, 0D8h
        db  0C1h, 0E8h, 00Ch, 08Bh, 03Eh, 0FAh, 088h, 0E8h, 013h, 002h, 088h, 047h, 004h, 088h, 044h, 004h
        db  0BBh, 060h, 080h, 089h, 01Eh, 02Eh, 080h, 0BEh, 070h, 082h, 089h, 036h, 03Eh, 080h, 08Ch, 0D8h
        db  0C1h, 0E8h, 00Ch, 08Bh, 03Eh, 0DEh, 000h, 0E8h, 0F3h, 001h, 088h, 047h, 004h, 088h, 044h, 004h
        db  08Bh, 00Eh, 0E6h, 088h, 08Bh, 01Eh, 04Eh, 080h, 088h, 04Fh, 004h, 0C6h, 047h, 005h, 00Ch, 08Dh
        db  07Fh, 010h, 089h, 07Fh, 006h, 023h, 03Eh, 0B6h, 088h, 089h, 07Fh, 008h, 08Bh, 01Eh, 05Eh, 080h
        db  088h, 04Fh, 004h, 0C6h, 047h, 005h, 004h, 08Dh, 07Fh, 010h, 089h, 07Fh, 006h, 023h, 03Eh, 0B6h
        db  088h, 089h, 07Fh, 008h, 033h, 0C0h, 0BEh, 080h, 084h, 088h, 004h, 088h, 044h, 002h, 0BEh, 090h
        db  086h, 088h, 004h, 088h, 044h, 002h, 0BEh, 060h, 080h, 088h, 004h, 088h, 044h, 002h, 0BEh, 070h
        db  082h, 088h, 004h, 088h, 044h, 002h, 0BBh, 010h, 001h, 0C7h, 047h, 002h, 0FFh, 0FFh, 0C7h, 047h
        db  004h, 01Ah, 001h, 0A1h, 0EAh, 088h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0C7h, 047h, 008h, 002h
        db  080h, 0C7h, 006h, 042h, 089h, 010h, 001h, 0C7h, 006h, 044h, 089h, 000h, 000h, 08Bh, 00Eh, 0F0h
        db  088h, 08Bh, 03Eh, 0F6h, 088h, 08Bh, 01Eh, 0DEh, 000h, 08Bh, 016h, 0EEh, 088h, 089h, 07Fh, 004h
        db  0A1h, 0EAh, 088h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0C6h, 047h, 007h, 000h, 089h, 057h, 008h
        db  0E8h, 075h, 0F9h, 083h, 0C3h, 010h, 003h, 0FAh, 083h, 0C7h, 003h, 083h, 0E7h, 0FCh, 0E2h, 0DDh
        db  08Bh, 00Eh, 0F0h, 088h, 0A1h, 0D6h, 000h, 08Bh, 0D8h, 0C7h, 007h, 000h, 000h, 0C6h, 047h, 003h
        db  000h, 005h, 008h, 000h, 089h, 047h, 004h, 0C7h, 047h, 006h, 0FFh, 0FFh, 0E2h, 0E9h, 0C6h, 047h
        db  003h, 080h, 0A1h, 0D6h, 000h, 089h, 047h, 004h, 0A3h, 03Eh, 089h, 089h, 01Eh, 040h, 089h, 08Bh
        db  00Eh, 0F4h, 088h, 08Eh, 006h, 0ECh, 088h, 08Bh, 03Eh, 0FCh, 088h, 08Bh, 036h, 0FAh, 088h, 08Bh
        db  016h, 0F2h, 088h, 083h, 0C2h, 003h, 080h, 0E2h, 0FCh, 08Bh, 0EFh, 023h, 02Eh, 0B6h, 088h, 003h
        db  0EAh, 085h, 02Eh, 0B8h, 088h, 074h, 00Eh, 085h, 02Eh, 0B6h, 088h, 074h, 008h, 023h, 03Eh, 0B8h
        db  088h, 003h, 03Eh, 0BAh, 088h, 08Bh, 0DEh, 083h, 0C6h, 010h, 0C7h, 007h, 000h, 000h, 0C7h, 047h
        db  002h, 0FFh, 0FFh, 089h, 07Fh, 004h, 08Ch, 0C0h, 0C1h, 0E8h, 00Ch, 088h, 047h, 006h, 0E8h, 0BCh
        db  000h, 088h, 047h, 007h, 08Bh, 0C7h, 023h, 006h, 0B6h, 088h, 089h, 047h, 00Eh, 0F7h, 006h, 0A0h
        db  088h, 000h, 080h, 075h, 00Ah, 057h, 051h, 033h, 0C0h, 08Bh, 0CAh, 0F3h, 0AAh, 059h, 05Fh, 003h
        db  0FAh, 0E2h, 0A6h, 08Ch, 0D8h, 08Eh, 0C0h, 0C7h, 006h, 046h, 089h, 000h, 000h, 08Bh, 00Eh, 0F4h
        db  088h, 08Bh, 03Eh, 0FAh, 088h, 08Bh, 01Eh, 0F8h, 088h, 08Bh, 0F3h, 083h, 0C3h, 008h, 0C7h, 044h
        db  002h, 004h, 000h, 089h, 05Ch, 004h, 089h, 07Ch, 006h, 083h, 0C7h, 010h, 0E2h, 0EBh, 0A1h, 0F8h
        db  088h, 089h, 044h, 004h, 0BEh, 080h, 084h, 08Dh, 074h, 010h, 0BFh, 090h, 086h, 08Dh, 07Dh, 010h
        db  0B9h, 000h, 001h, 08Bh, 01Eh, 0FAh, 088h, 08Bh, 02Eh, 0F8h, 088h, 08Bh, 016h, 0F4h, 088h, 08Bh
        db  0C3h, 023h, 006h, 0B6h, 088h, 089h, 004h, 089h, 02Dh, 083h, 0C3h, 010h, 083h, 0C5h, 008h, 04Ah
        db  075h, 00Ch, 08Bh, 01Eh, 0FAh, 088h, 08Bh, 02Eh, 0F8h, 088h, 08Bh, 016h, 0F4h, 088h, 083h, 0C6h
        db  002h, 083h, 0C7h, 002h, 0E2h, 0D9h, 0BEh, 080h, 084h, 0A1h, 0F4h, 088h, 088h, 004h, 0C3h, 053h
        db  051h, 0B9h, 010h, 000h, 033h, 0DBh, 0D1h, 0D3h, 0D1h, 0E8h, 0E0h, 0FAh, 0F7h, 0D9h, 083h, 0C1h
        db  010h, 040h, 0D3h, 0E0h, 085h, 0DBh, 075h, 002h, 0D1h, 0E8h, 059h, 05Bh, 0C3h, 052h, 08Bh, 0D0h
        db  08Bh, 0C7h, 0F7h, 036h, 0BEh, 088h, 053h, 0BBh, 000h, 089h, 0D7h, 05Bh, 05Ah, 0C3h, 0F6h, 046h
        db  001h, 008h, 075h, 009h, 001h, 044h, 060h, 011h, 054h, 062h, 0EBh, 065h, 090h, 001h, 044h, 05Ch
        db  011h, 054h, 05Eh, 0EBh, 05Ch, 090h, 011h, 054h, 046h, 0EBh, 048h, 090h, 011h, 054h, 04Ah, 0EBh
        db  04Ah, 090h, 011h, 054h, 052h, 0EBh, 012h, 090h, 011h, 054h, 04Eh, 0EBh, 01Ch, 090h, 0F6h, 047h
        db  00Bh, 020h, 075h, 010h, 001h, 044h, 050h, 072h, 0E9h, 001h, 04Ch, 054h, 073h, 033h, 011h, 054h
        db  056h, 0EBh, 02Eh, 090h, 001h, 044h, 04Ch, 072h, 0DFh, 001h, 04Ch, 058h, 073h, 023h, 011h, 054h
        db  05Ah, 0EBh, 01Eh, 090h, 033h, 0C0h, 099h, 040h, 0F6h, 046h, 001h, 020h, 074h, 0A0h, 001h, 044h
        db  044h, 072h, 0B3h, 08Bh, 04Fh, 008h, 001h, 04Ch, 048h, 072h, 0B1h, 0F6h, 047h, 00Bh, 022h, 075h
        db  0BDh, 08Bh, 04Eh, 000h, 091h, 080h, 0E4h, 0FBh, 03Dh, 000h, 0A0h, 075h, 001h, 0C3h, 0BEh, 090h
        db  000h, 0A8h, 080h, 074h, 00Eh, 001h, 04Ch, 014h, 011h, 054h, 016h, 024h, 07Fh, 03Dh, 000h, 0A0h
        db  075h, 001h, 0C3h, 0A8h, 040h, 074h, 012h, 0A8h, 02Fh, 075h, 00Eh, 001h, 04Ch, 02Ch, 011h, 054h
        db  02Eh, 024h, 0BFh, 03Dh, 000h, 0A0h, 075h, 001h, 0C3h, 024h, 02Fh, 074h, 035h, 0A8h, 020h, 074h
        db  00Fh, 083h, 04Ch, 038h, 001h, 001h, 04Ch, 018h, 011h, 054h, 01Ah, 0A0h, 024h, 089h, 0EBh, 018h
        db  001h, 04Ch, 010h, 011h, 054h, 012h, 03Ch, 001h, 075h, 008h, 001h, 04Ch, 024h, 011h, 054h, 026h
        db  0EBh, 006h, 001h, 04Ch, 028h, 011h, 054h, 02Ah, 08Ah, 0C8h, 001h, 04Ch, 01Ch, 011h, 054h, 01Eh
        db  0B1h, 001h, 08Ah, 0C4h, 0A8h, 001h, 074h, 006h, 001h, 04Ch, 03Ch, 011h, 054h, 03Eh, 0C3h, 033h
        db  0C0h, 099h, 040h, 0BEh, 010h, 000h, 001h, 044h, 014h, 011h, 054h, 016h, 08Bh, 04Fh, 00Ch, 001h
        db  04Ch, 01Ch, 011h, 054h, 01Eh, 03Bh, 00Eh, 022h, 089h, 07Ch, 00Dh, 03Bh, 00Eh, 020h, 089h, 07Fh
        db  01Ah, 0F6h, 047h, 00Eh, 012h, 075h, 021h, 0C3h, 001h, 044h, 034h, 011h, 054h, 036h, 001h, 044h
        db  02Ch, 011h, 054h, 02Eh, 0F6h, 047h, 00Eh, 012h, 075h, 00Eh, 0C3h, 001h, 044h, 030h, 011h, 054h
        db  032h, 083h, 00Eh, 098h, 000h, 004h, 0EBh, 0E6h, 0F6h, 047h, 00Eh, 010h, 075h, 00Dh, 001h, 044h
        db  028h, 011h, 054h, 02Ah, 001h, 04Ch, 03Ch, 011h, 054h, 03Eh, 0C3h, 001h, 044h, 024h, 011h, 054h
        db  026h, 001h, 04Ch, 038h, 011h, 054h, 03Ah, 0C3h, 000h, 000h, 000h, 000h, 010h, 020h, 030h, 040h
        db  000h, 050h, 000h, 010h, 000h, 000h, 060h, 000h, 000h, 000h
loc_1_5564:
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
loc_1_55E0:
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
loc_1_563C:
        push   ebp
        inc    edx
        and    byte ptr [ebp + 0x74], al
        push   0x656e7265
        je     loc_1_5668
        inc    ecx
        popal  
        jo     0x56c1
        jb     0x567e
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5654:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
loc_1_5668:
        add    byte ptr [eax + 0xc], al
        add    byte ptr [eax], al
        or     byte ptr [eax + eax], 0
        ror    byte ptr [eax + eax], 0
        add    byte ptr [0xd400000], cl
        add    byte ptr [eax], al
        or     byte ptr [0xdc00000], 0
        add    byte ptr [eax + 0x400000e2], al
        or     al, 0
        add    byte ptr [eax - 0x3ffffff4], al
        or     al, 0
        add    byte ptr [eax], al
        or     eax, 0xd400000
        add    byte ptr [eax], al
        or     byte ptr [0xdc00000], 0
loc_1_56A4:
        push   eax
        add    edx, dword ptr [eax + 3]
        pop    eax
        add    ebx, dword ptr [eax + 3]
        pushal 
        add    esp, dword ptr [eax + 3]
        push   0x68036803
        add    ebp, dword ptr [eax + 3]
        push   0x68036803
        add    ebp, dword ptr [eax + 3]
        push   0x50036803
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 3], bl
        add    byte ptr [eax], al
        pushal 
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 3], ch
        add    byte ptr [eax], al
loc_1_56D4:
        add    al, byte ptr [ebx]
        add    al, 5
        pop    es
        or     dword ptr [eax + eax], ecx
        dec    ecx
        dec    edi
        pop    edi
        push   eax
        outsd  dx, dword ptr [esi]
        jb     0x5757
        add    byte ptr [ecx + 0x4f], cl
        pop    edi
        push   eax
        outsd  dx, dword ptr [esi]
        jb     loc_1_575F
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        pop    edi
        dec    esp
        jbe    0x5759
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        db  000h
loc_1_56F8:
        dec    ecx
        push   edx
        push   ecx
        pop    edi
        dec    esp
        jbe    0x5765
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x64], al
        popal  
        jo     0x577d
        jb     0x5760
        jns    loc_1_577E
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_5789
        jb     0x576c
        jns    0x578a
        add    byte ptr gs:[ebp + 0x61], cl
        js     loc_1_576D
        jne    loc_1_578E
        je     0x578d
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_5729
loc_1_5729:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x61], cl
        js     0x577d
        jne    loc_1_579E
        je     0x579d
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_5739
loc_1_5739:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x57bb
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x776f
        add    byte ptr [eax], al
loc_1_574C:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x57cb
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x776f
        add    byte ptr [eax], al
        push   ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_1_575F:
        je     loc_1_57AF
        jne    loc_1_57D0
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        push   ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     0x57bb
loc_1_576D:
        jne    0x57dc
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        push   edx
        arpl   word ptr gs:[ebp + 0x69], sp
        jbe    0x57e0
        inc    edx
        jne    0x57e4
loc_1_577E:
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x65520000
        arpl   word ptr [ebp + 0x69], sp
loc_1_5789:
        jbe    0x57f0
        inc    edx
        jne    0x57f4
loc_1_578E:
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x614d0000
        js     loc_1_5801
        insd   dword ptr es:[edi], dx
        jne    loc_1_5808
        dec    edi
        jo     loc_1_5803
loc_1_579E:
        outsb  dx, byte ptr [esi]
        jae    loc_1_57A1
loc_1_57A1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x61], cl
        js     loc_1_5811
        insd   dword ptr es:[edi], dx
        jne    loc_1_5818
        dec    edi
        jo     0x5813
        outsb  dx, byte ptr [esi]
loc_1_57AF:
        jae    loc_1_57B1
loc_1_57B1:
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 0x61], al
        outsb  dx, byte ptr [si]
        outsd  dx, dword ptr [esi]
        jae    loc_1_5830
        imul   esp, dword ptr [ebx + 0x73], 0x61694400
        outsb  dx, byte ptr [si]
        outsd  dx, dword ptr [esi]
        jae    0x583c
        imul   esp, dword ptr [ebx + 0x73], 0x4e435000
        dec    ecx
loc_1_57D0:
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    esi
        dec    ecx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x49], cl
        push   ebp
        jo     0x5844
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x49], cl
        push   ebp
        jo     0x584c
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x49], cl
        push   ebp
        jo     0x5864
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x49], cl
        push   ebp
        jo     0x586c
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x49], cl
        push   ebp
        jo     0x5864
loc_1_5801:
        das    
        inc    ebp
loc_1_5803:
        dec    edi
        push   esp
        push   eax
        add    byte ptr [eax], al
loc_1_5808:
        dec    esi
        dec    ecx
        push   ebp
        jo     0x5870
        das    
        inc    ebp
        dec    edi
        push   esp
loc_1_5811:
        push   eax
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        dec    esi
        dec    ecx
loc_1_5818:
        push   ebp
        js     loc_1_581C
loc_1_581C:
        push   eax
        inc    ebx
        dec    esi
        dec    ecx
        push   ebp
        js     loc_1_5824
loc_1_5824:
        inc    edx
        jne    0x589a
        push   esp
        jns    0x589a
        add    byte ptr gs:[edx + 0x75], al
        jae    0x5884
loc_1_5830:
        jns    0x58a2
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_586D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5873:
        add    byte ptr [eax], al
loc_1_5875:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58AF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        adc    al, byte ptr [esi]
        push   ss
        or     bl, byte ptr [edx]
        push   cs
        push   ds
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax + 0x30002000], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], dh
        add    byte ptr [eax], dh
        clc    
        inc    byte ptr [eax]
        add    dword ptr [eax + 0x28000], eax
loc_1_58F3:
        add    al, byte ptr [edi]
        push   es
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 2], dl
        push   es
        or     cl, byte ptr [esi]
        adc    dl, byte ptr [esi]
        sbb    bl, byte ptr [esi]
        and    ah, byte ptr [esi]
        sub    ch, byte ptr [esi]
        xor    dh, byte ptr [esi]
        cmp    bh, byte ptr [esi]
        inc    edx
        inc    esi
        dec    edx
        dec    esi
        push   edx
        push   esi
        pop    edx
        pop    esi
        bound  esp, qword ptr [esi + 0x6a]
        outsb  dx, byte ptr [esi]
        jb     0x5994
        jp     0x599e
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        xor    byte ptr [eax], al
        xor    byte ptr [eax], al
        jo     loc_1_592D
loc_1_592D:
        jo     loc_1_58AF
        inc    dword ptr [eax]
        xor    byte ptr [eax + 0x28000], al
        add    al, byte ptr [edx]
        add    eax, dword ptr [eax + 0x100601]
        add    byte ptr [ecx], al
        add    ah, byte ptr [eax + 0xa060259]
        push   cs
        adc    dl, byte ptr [esi]
        sbb    bl, byte ptr [esi]
        and    ah, byte ptr [esi]
        sub    ch, byte ptr [esi]
        xor    dh, byte ptr [esi]
        cmp    bh, byte ptr [esi]
        inc    edx
        inc    esi
        dec    edx
        dec    esi
        push   edx
        push   esi
        pop    edx
        pop    esi
        bound  esp, qword ptr [esi + 0x6a]
        outsb  dx, byte ptr [esi]
        jb     loc_1_59D8
        jp     loc_1_59E2
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        xor    byte ptr [eax], al
        xor    byte ptr [eax], al
        jo     loc_1_5971
loc_1_5971:
        jo     loc_1_58F3
        inc    dword ptr [eax]
        xor    byte ptr [eax + 0x28000], al
        add    al, byte ptr [edx]
        add    eax, dword ptr [eax + 0x100601]
        add    byte ptr [ecx], al
        add    ah, byte ptr [eax + 0x47]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x42], dl
        dec    esi
        inc    ebp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x42], dl
        dec    esi
        inc    ebp
loc_1_59D8:
        dec    ecx
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_59E2:
        add    byte ptr [eax], al
loc_1_59E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59E8:
        add    byte ptr [eax], al
loc_1_59EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59FA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_59FE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A0A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A0E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A12:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5A2A:
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
        mov    ebx, dword ptr [0x59dc]       ; -> obj1:0x59DC
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_0506
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
        call   sub_1_0506
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
        call   sub_1_0506
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
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x2c], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0x30], dl
        add    eax, 0x2c
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
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
        call   sub_1_050C
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_050C
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_050C
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
        mov    eax, dword ptr [0x59dc]       ; -> obj1:0x59DC
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
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
        call   sub_1_050C
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
        call   sub_1_060C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_060C
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
        call   sub_1_060C
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
        call   sub_1_060C
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
        call   sub_1_060C
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
        call   sub_1_060C
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
        call   sub_1_060C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_060C
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
        call   sub_1_060C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_060C
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
        cmp    ebx, ecx
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
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_071C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_060C
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
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
        call   sub_1_060C
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
