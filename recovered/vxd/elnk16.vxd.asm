; ELNK16 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 6081 instructions recovered by recursive descent

Declare_Virtual_Device ELNK16, 3, 10, ELNK16_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3788  R-X
loc_1_0000:
        mov    al, byte ptr [0x698]          ; -> obj1:0x0698
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x698]              ; -> obj1:0x0698
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_4_0000
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
ELNK16_Control:
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
sub_1_047A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
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
sub_1_057E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0584:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_058A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0590:
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
        db  'ELNK16  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  ELNK16_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    eax, dword ptr [edx + 0x45]
        mov    si, word ptr [edx + 0x1a]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [edx + 0x16a]
        mov    ax, word ptr [eax + ecx + 4]
        mov    ecx, dword ptr [edx + 2]
        sub    ecx, esi
        mov    word ptr [ecx + 0xffc4], ax
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x1a]
        mov    eax, dword ptr [edx + 2]
        sub    eax, ecx
        pop    esi
        mov    word ptr [eax + 0xffc2], 0x100
        mov    dx, word ptr [edx]
        add    dx, 0xb
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [ecx + 0x24]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [edx + edx*4]
        lea    ebx, [eax + eax*4]
        mov    eax, dword ptr [edi + 0x16a]
        add    eax, ebx
        mov    esi, dword ptr [eax]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [edi + 0x16a]
        mov    dword ptr [ebx + eax + 0xc], 0xffff
        mov    eax, dword ptr [edi + 0x16a]
        mov    dword ptr [ebx + eax + 0x14], 0
        lea    eax, [ebp - 0x10]
        mov    word ptr [esi], 0
        mov    word ptr [esi + 4], 0xffff
        mov    word ptr [esi + 2], 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   ecx
        call   sub_1_0530
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        push   6
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0xc], 0xc
        mov    eax, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 4], 0xe
        mov    ax, word ptr [eax]
        mov    word ptr [esi + 0xe], ax
        add    dword ptr [ebp - 0xc], 2
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_07C9
        dec    dword ptr [ebp - 0x14]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
loc_1_07C9:
        mov    eax, dword ptr [edi + 0x16a]
        mov    edi, dword ptr [ebx + eax + 6]
        mov    ebx, dword ptr [ebp - 0x14]
        or     ebx, ebx
        je     loc_1_0817
loc_1_07DA:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_07EE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_05EC
loc_1_07EE:
        add    edi, dword ptr [ebp - 4]
        cmp    ebx, 1
        jbe    loc_1_0814
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
loc_1_0814:
        dec    ebx
        jne    loc_1_07DA
loc_1_0817:
        cmp    dword ptr [ebp - 0x10], 0x3c
        jae    loc_1_083C
        mov    edx, 0x3c
        sub    edx, dword ptr [ebp - 0x10]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    dword ptr [ebp - 0x10], 0x3c
loc_1_083C:
        mov    eax, dword ptr [ebp - 0x10]
        sub    ax, 0xe
        or     ax, 0x8000
        mov    word ptr [esi + 0x10], ax
        mov    eax, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 8], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_0860:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [esi + esi*4]
        lea    ecx, [eax + eax*4]
        mov    eax, dword ptr [edi + 0x16a]
        mov    eax, dword ptr [ecx + eax]
        mov    edx, dword ptr [edi + 0x71]
        mov    bx, word ptr [eax + 2]
        or     bx, 0xa000
        mov    word ptr [eax + 2], bx
        mov    eax, dword ptr [edi + 0x16a]
        mov    byte ptr [ecx + eax + 0x18], 0
        mov    eax, dword ptr [edi + 0x16a]
        cmp    edx, 0xffff
        mov    dword ptr [eax + ecx + 0xc], 0xffff
        mov    dword ptr [edi + 0x71], esi
        je     loc_1_08D0
        lea    ecx, [edx + edx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [edi + 0x16a]
        mov    dword ptr [ecx + eax + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_08D0:
        cmp    dword ptr [edi + 0x6d], 0xffff
        jne    loc_1_08DC
        mov    dword ptr [edi + 0x6d], esi
loc_1_08DC:
        sub    ebx, ebx
        mov    edi, edi
loc_1_08E0:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0x1a]
        mov    eax, dword ptr [edi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_0907
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_08E0
loc_1_0907:
        mov    dword ptr [edi + 0x45], esi
        push   edi
        push   0x6a0                         ; -> loc_1_06A0
        add    edi, 0x131
        push   edi
        call   sub_1_04FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x1d]
        mov    ax, word ptr [edi + 2]
        or     ax, 0x8000
        mov    word ptr [edi + 2], ax
        mov    word ptr [edi], 0
loc_1_0952:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_0979
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_0952
loc_1_0979:
        mov    eax, dword ptr [esi + 0x21]
        mov    dword ptr [esi + 0x45], eax
        push   esi
        push   0x6a0                         ; -> loc_1_06A0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        sub    ebx, ebx
loc_1_0993:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_09BA
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_0993
loc_1_09BA:
        sub    esi, esi
        mov    ebx, 0x8000
loc_1_09C1:
        mov    ax, word ptr [edi]
        test   bx, ax
        jne    loc_1_09D9
        push   0x32
        call   sub_1_0500
        inc    esi
        cmp    esi, 0x4e20
        jbe    loc_1_09C1
loc_1_09D9:
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
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 0x65], 0
        je     loc_1_0A20
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x69]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x69]
        inc    eax
        mov    dword ptr [edx + 0x69], eax
        cmp    dword ptr [edx + 0x21], eax
        ja     loc_1_0A17
        mov    dword ptr [edx + 0x69], 0
loc_1_0A17:
        dec    dword ptr [edx + 0x65]
        mov    al, 1
        pop    ebp
        ret    8
loc_1_0A20:
        mov    byte ptr [edx + 0x85], 0
        sub    al, al
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0x16a]
        lea    ecx, [esi + esi*4]
        lea    ecx, [ecx + ecx*4]
        add    eax, ecx
        cmp    dword ptr [edx + 0x71], esi
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [edx + 0x6d], eax
        jne    loc_1_0A5C
        mov    dword ptr [edx + 0x71], 0xffff
loc_1_0A5C:
        mov    word ptr [ecx + 4], 0xffff
        mov    word ptr [ecx], 0
        inc    dword ptr [edx + 0x65]
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_0A70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x16a]
        or     eax, eax
        je     loc_1_0A8B
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A8B:
        mov    eax, dword ptr [esi + 0x16e]
        or     eax, eax
        je     loc_1_0A9F
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A9F:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xa3], 0
        mov    dword ptr [eax + 0x35], 0
        mov    dword ptr [eax + 0x39], 0
        mov    dword ptr [eax + 0x6d], 0xffff
        mov    dword ptr [eax + 0x71], 0xffff
        mov    dword ptr [eax + 0x69], 0
        mov    ecx, dword ptr [eax + 0x21]
        mov    dword ptr [eax + 0x65], ecx
        mov    dword ptr [eax + 0x75], 0
        mov    dword ptr [eax + 0x79], 0
        mov    dword ptr [eax + 0x7d], 0
        mov    dword ptr [eax + 0x81], 0
        mov    byte ptr [eax + 0x85], 1
        mov    byte ptr [eax + 0x86], 0
        mov    dword ptr [eax + 0x87], 0
        mov    dword ptr [eax + 0x8b], 0
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        sub    edx, edx
        inc    dword ptr [esi + 0x59]
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    edx, ecx
        je     loc_1_0B5B
        mov    eax, dword ptr [ebp + 0x14]
loc_1_0B4A:
        cmp    dword ptr [eax], 0
        je     loc_1_0B57
        add    eax, 4
        inc    edx
        cmp    edx, ecx
        jb     loc_1_0B4A
loc_1_0B57:
        cmp    edx, ecx
        jne    loc_1_0B65
loc_1_0B5B:
        mov    edi, 0xc0010019
        jmp    loc_1_0C1E
loc_1_0B65:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [0x3570]       ; -> obj1:0x3570
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x45
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_0B90
        mov    edi, 0xc000009a
        jmp    loc_1_0C1E
        db  005h, 000h, 000h, 000h, 000h
loc_1_0B90:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x41], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x29], 3
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x19
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x19
        push   eax
        push   esi
        call   sub_1_1D10
        mov    edi, 0x103
        cmp    byte ptr [esi + 0x54], 0
        je     loc_1_0C1E
        mov    byte ptr [esi + 0x54], 0
        push   0x7d0
        lea    eax, [esi + 0x149]
        push   eax
        call   sub_1_04E2
loc_1_0C1E:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_0C56
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_0C48
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_0C48
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_0C56
loc_1_0C48:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_0C56:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0C60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [edi + 0x18], 0
        jne    loc_1_0CA0
        lea    eax, [edi + 0x19]
        mov    dword ptr [edi + 0x29], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        push   eax
        push   esi
        call   sub_1_1D10
        dec    dword ptr [edi + 0x14]
        mov    edi, 0x103
        jmp    loc_1_0CA5
        db  005h, 000h, 000h, 000h, 000h
loc_1_0CA0:
        mov    edi, 0xc0010002
loc_1_0CA5:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_0CDD
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_0CCF
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_0CCF
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_0CDD
loc_1_0CCF:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_0CDD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x3784]       ; -> obj1:0x3784
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [0x3770]       ; -> obj1:0x3770
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x149]
        push   eax
        call   sub_1_04E8
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04F4
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_2_0120
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_0438
        mov    eax, dword ptr [esi + 0xe1]
        mov    ecx, dword ptr [esi + 0xe5]
        lea    edx, [esi + 0xe1]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   esi
        call   sub_1_050C
        mov    dx, 0x100
        sub    al, al
        out    dx, al
        push   esi
        call   sub_1_3290
        mov    dx, 0x100
        sub    al, al
        out    dx, al
        push   esi
        call   sub_1_3290
        push   esi
        call   sub_1_3290
        mov    dx, 0x100
        mov    al, 0xff
        out    dx, al
        mov    al, 0x10
        out    0xf, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [ecx + 0x1c]
        or     al, 4
        mov    byte ptr [ecx + 0x1c], al
        mov    dx, word ptr [ecx]
        add    dx, 6
        out    dx, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc0]
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        test   al, 8
        je     loc_1_0E17
        mov    dx, word ptr [esi]
        add    dx, 0xa
        mov    al, 0xff
        out    dx, al
loc_1_0E17:
        test   byte ptr [ebp - 4], 0x40
        je     loc_1_0E27
        cmp    byte ptr [esi + 0x4e], 0
        je     loc_1_0E27
        mov    byte ptr [esi + 0x4e], 0
loc_1_0E27:
        mov    byte ptr [esi + 0x58], 0
        mov    ax, word ptr [ebp - 4]
        and    ax, 0xf000
        je     loc_1_0E80
        cmp    byte ptr [esi + 0x53], 0
        je     loc_1_0E63
        xor    edi, edi
        mov    di, word ptr [esi + 0x1a]
        mov    edx, dword ptr [esi + 2]
        sub    edx, edi
        sub    cl, cl
        mov    word ptr [edx + 0xffc2], ax
        mov    dx, word ptr [esi]
        add    dx, 0xb
        sub    al, al
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0E63:
        mov    cl, 1
        mov    al, byte ptr [esi + 0x1c]
        and    al, 0xfb
        mov    byte ptr [esi + 0x1c], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        mov    al, cl
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0E80:
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 6], di
        cmp    byte ptr [esi + 0x50], 0
        jne    loc_1_1125
        mov    byte ptr [esi + 0x50], 1
        mov    al, byte ptr [esi + 0x1c]
        and    al, 0xfb
        mov    byte ptr [esi + 0x1c], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        cmp    byte ptr [esi + 0x57], 0
        je     loc_1_0ECF
        mov    byte ptr [esi + 0x57], 0
        mov    di, 0x8000
loc_1_0ECF:
        sub    ebx, ebx
loc_1_0ED1:
        test   di, 0xf000
        jne    loc_1_0F53
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc0]
        mov    word ptr [ebp - 6], ax
        test   al, 0x40
        je     loc_1_0EFA
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0EFA
        mov    byte ptr [esi + 0x4e], bl
loc_1_0EFA:
        and    word ptr [ebp - 6], 0xf000
        je     loc_1_0F53
        mov    dword ptr [ebp - 0xc], ebx
loc_1_0F05:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_0F2F
        push   0x32
        call   sub_1_0500
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 0x4e20
        jbe    loc_1_0F05
loc_1_0F2F:
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1a]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 0xffc2], ax
        or     di, word ptr [ebp - 6]
        mov    dx, word ptr [esi]
        add    dx, 0xb
        mov    al, bl
        out    dx, al
loc_1_0F53:
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_0F75
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        jmp    loc_1_0F79
loc_1_0F75:
        mov    word ptr [ebp - 2], bx
loc_1_0F79:
        test   di, 0xe000
        jne    loc_1_0FCF
        cmp    word ptr [ebp - 2], bx
        je     loc_1_0F8C
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_0FCF
loc_1_0F8C:
        test   di, 0x1000
        je     loc_1_0F98
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0FCF
loc_1_0F98:
        cmp    dword ptr [esi + 0x75], ebx
        jne    loc_1_0FCF
        cmp    byte ptr [esi + 0x8f], bl
        je     loc_1_0FAB
        cmp    dword ptr [esi + 0x59], 1
        je     loc_1_0FCF
loc_1_0FAB:
        cmp    byte ptr [esi + 0x86], bl
        jne    loc_1_109B
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_109B
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_109B
loc_1_0FCF:
        mov    eax, dword ptr [esi + 0x59]
        inc    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    byte ptr [esi + 0x8f], bl
        je     loc_1_0FE7
        cmp    eax, 2
        je     loc_1_1092
loc_1_0FE7:
        test   di, 0x4000
        jne    loc_1_0FFA
        test   di, 0x1000
        je     loc_1_1010
        cmp    byte ptr [esi + 0x4e], bl
        jne    loc_1_1010
loc_1_0FFA:
        push   esi
        call   sub_1_1130
        cmp    al, bl
        je     loc_1_1010
        and    di, 0xafff
        lea    esp, [esp]
loc_1_1010:
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_1050
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        test   ah, 0x40
        jne    loc_1_1050
        cmp    ax, bx
        je     loc_1_1050
        push   esi
        call   sub_1_1300
        test   di, 0xa000
        je     loc_1_1055
        test   di, 0x1000
        jne    loc_1_1010
        jmp    loc_1_1055
loc_1_1050:
        and    di, 0x5fff
loc_1_1055:
        cmp    byte ptr [esi + 0x86], bl
        jne    loc_1_1073
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_1073
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_1073
        push   esi
        call   sub_1_3430
loc_1_1073:
        push   esi
        call   sub_1_1730
        dec    dword ptr [esi + 0x59]
        test   di, 0x1000
        je     loc_1_0ED1
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0ED1
        jmp    loc_1_109B
loc_1_1092:
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 0x59]
loc_1_109B:
        mov    edi, dword ptr [esi + 0xd9]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_10F9
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_10F9
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0444
        mov    eax, dword ptr [esi + 0x35]
        or     eax, eax
        je     loc_1_10DA
        cmp    dword ptr [eax + 0x10], 4
        jne    loc_1_10DA
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_10DA
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x35], eax
        push   esi
        call   sub_1_1D50
loc_1_10DA:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0
        push   edi
        call   sub_1_050C
        dec    dword ptr [esi + 0xcd]
loc_1_10F9:
        mov    byte ptr [esi + 0x50], 0
        push   esi
        push   0xdc0                         ; -> loc_1_0DC0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        cmp    byte ptr [esi + 0x4d], 0
        je     loc_1_1125
        mov    byte ptr [esi + 0x4d], 0
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_1_0290
loc_1_1125:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_1130:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ecx, dword ptr [esi + 0x3d]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 0x16e]
        mov    ebx, dword ptr [ecx]
loc_1_1150:
        mov    byte ptr [esi + 0x91], 1
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 2], ax
        test   ax, 0xbfff
        je     loc_1_1252
        cmp    edi, 0xa
        ja     loc_1_12A0
        test   ah, 0x20
        jne    loc_1_11B5
        test   al, 0x80
        je     loc_1_1182
        inc    dword ptr [esi + 0xc3]
        jmp    loc_1_118E
loc_1_1182:
        test   byte ptr [ebp - 2], 0x40
        je     loc_1_118E
        inc    dword ptr [esi + 0xc7]
loc_1_118E:
        push   ebx
        push   esi
        call   sub_1_12C0
        mov    eax, dword ptr [esi + 0x3d]
        cmp    dword ptr [esi + 0x25], eax
        jne    loc_1_11A4
        mov    dword ptr [esi + 0x3d], 0
loc_1_11A4:
        mov    ecx, dword ptr [esi + 0x3d]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 0x16e]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    loc_1_1150
loc_1_11B5:
        inc    dword ptr [esi + 0x9f]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_12AB
        mov    ax, word ptr [ebx + 0x16]
        mov    word ptr [ebp - 2], ax
        and    eax, 0x3fff
        lea    edx, [ebx + 8]
        mov    dword ptr [ebp - 8], edx
        or     edx, edx
        je     loc_1_1220
        or     eax, eax
        je     loc_1_1200
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0xc], edx
        or     edx, edx
        je     loc_1_1220
        or     eax, eax
        je     loc_1_1200
        cmp    eax, 0x5dc
        ja     loc_1_1220
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        jmp    loc_1_1206
loc_1_1200:
        push   0
        push   0
        push   0
loc_1_1206:
        push   0xe
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        mov    eax, dword ptr [ecx + 8]
        or     eax, 1
        push   eax
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_1_00B0
loc_1_1220:
        push   ebx
        push   esi
        call   sub_1_12C0
        mov    byte ptr [esi + 0x4d], 1
        mov    eax, dword ptr [esi + 0x3d]
        inc    eax
        mov    dword ptr [esi + 0x3d], eax
        cmp    dword ptr [esi + 0x25], eax
        jne    loc_1_123E
        mov    dword ptr [esi + 0x3d], 0
loc_1_123E:
        mov    ecx, dword ptr [esi + 0x3d]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 0x16e]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    loc_1_1150
loc_1_1252:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc0]
        and    ax, 0x40
        cmp    ax, 1
        sbb    al, al
        neg    al
        or     al, al
        jne    loc_1_1280
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1280:
        mov    eax, dword ptr [esi + 0x3d]
        shl    eax, 4
        add    eax, dword ptr [esi + 0x16e]
        push   eax
        push   esi
        call   sub_1_2AF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12A0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_12AB:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x41]
        shl    eax, 4
        mov    ecx, dword ptr [edx + 0x16e]
        mov    ecx, dword ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax], 0
        mov    word ptr [eax + 2], 0xc000
        mov    word ptr [ecx + 2], 0
        mov    eax, dword ptr [edx + 0x41]
        inc    eax
        mov    dword ptr [edx + 0x41], eax
        cmp    dword ptr [edx + 0x25], eax
        jne    loc_1_12FC
        mov    dword ptr [edx + 0x41], 0
loc_1_12FC:
        pop    ebp
        ret    8
sub_1_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, 1
        mov    eax, dword ptr [esi + 0x6d]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    edx, dword ptr [esi + 0x16a]
        mov    eax, dword ptr [eax + edx]
        mov    dx, word ptr [eax]
        mov    word ptr [ebp - 2], dx
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 8], ax
        and    dword ptr [ebp - 8], 7
        jne    loc_1_1352
        push   0
        push   0xaa00000b
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1352:
        mov    byte ptr [esi + 0x90], 1
        cmp    dword ptr [ebp - 8], 4
        je     loc_1_13A0
        mov    edi, dword ptr [esi + 0x6d]
        push   esi
        call   sub_1_14C0
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_1520
        lea    eax, [edi + edi*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    eax, dword ptr [eax + ecx + 0xc]
        mov    dword ptr [esi + 0x6d], eax
        cmp    eax, 0xffff
        jne    loc_1_14AF
        mov    dword ptr [esi + 0x71], 0xffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_13A0:
        mov    edx, dword ptr [esi + 0x6d]
        test   byte ptr [ebp - 1], 0x20
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    edx, dword ptr [edx + eax + 0x10]
        lea    edi, [edx + 0x24]
        je     loc_1_13F0
        inc    dword ptr [esi + 0x9b]
        test   byte ptr [ebp - 2], 0x80
        je     loc_1_13D0
        inc    dword ptr [esi + 0xb7]
        jmp    loc_1_142E
loc_1_13D0:
        mov    ax, word ptr [ebp - 2]
        and    ax, 0xf
        cmp    ax, 1
        jne    loc_1_13E6
        inc    dword ptr [esi + 0xbb]
        jmp    loc_1_142E
loc_1_13E6:
        jbe    loc_1_142E
        inc    dword ptr [esi + 0xbf]
        jmp    loc_1_142E
loc_1_13F0:
        test   byte ptr [ebp - 2], 0x20
        je     loc_1_1400
        inc    dword ptr [esi + 0xa7]
        jmp    loc_1_142C
        db  08Bh, 0FFh
loc_1_1400:
        test   byte ptr [ebp - 1], 4
        je     loc_1_1410
        inc    dword ptr [esi + 0xab]
        jmp    loc_1_142C
        db  08Bh, 0FFh
loc_1_1410:
        test   byte ptr [ebp - 1], 2
        je     loc_1_1420
        inc    dword ptr [esi + 0xb3]
        jmp    loc_1_142C
        db  08Bh, 0FFh
loc_1_1420:
        test   byte ptr [ebp - 1], 1
        je     loc_1_142C
        inc    dword ptr [esi + 0xaf]
loc_1_142C:
        sub    cl, cl
loc_1_142E:
        cmp    dword ptr [edi], 0
        jne    loc_1_143D
        mov    dword ptr [esi + 0x81], 0
loc_1_143D:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x7d], eax
        cmp    byte ptr [edi + 0xd], 0
        jne    loc_1_1465
        mov    edi, dword ptr [edi + 4]
        cmp    cl, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x14]
        jmp    loc_1_1482
loc_1_1465:
        mov    byte ptr [edi + 0xc], cl
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_1473
        mov    dword ptr [esi + 0x75], edx
        jmp    loc_1_1479
loc_1_1473:
        mov    eax, dword ptr [esi + 0x79]
        mov    dword ptr [eax + 0x24], edx
loc_1_1479:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x79], edx
loc_1_1482:
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_1520
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_0A30
        cmp    byte ptr [esi + 0x85], 0
        jne    loc_1_14AF
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_14AF
        mov    byte ptr [esi + 0x85], 1
loc_1_14AF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x6d]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 0x16a]
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        jne    loc_1_1510
        mov    eax, dword ptr [esi + 0x35]
        or     eax, eax
        je     loc_1_1518
        mov    edi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x35], ecx
        push   0
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_045C
        dec    dword ptr [edi + 0x14]
        push   esi
        call   sub_1_1D50
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1510:
        cmp    eax, 1
        je     loc_1_1518
        dec    dword ptr [eax + 0x14]
loc_1_1518:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x16a]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    ecx, eax
        mov    eax, dword ptr [ecx + 0xc]
        cmp    eax, 0xffff
        je     loc_1_158B
        mov    byte ptr [ecx + 0x18], 0
        mov    dword ptr [esi + 0x45], eax
        sub    edi, edi
        sub    ebx, ebx
loc_1_1552:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_1579
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_1552
loc_1_1579:
        push   esi
        push   0x6a0                         ; -> loc_1_06A0
        add    esi, 0x131
        push   esi
        call   sub_1_04FA
loc_1_158B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    al, al
        cmp    byte ptr [ecx + 0x50], al
        jne    loc_1_15E2
        cmp    byte ptr [ecx + 0x8f], al
        jne    loc_1_15E2
        cmp    byte ptr [ecx + 0x90], al
        jne    loc_1_15E2
        cmp    byte ptr [ecx + 0x91], al
        jne    loc_1_15E2
        mov    al, byte ptr [ecx + 0x92]
        cmp    al, 0xa
        lea    edx, [eax + 1]
        mov    byte ptr [ecx + 0x92], dl
        jae    loc_1_15E0
        sub    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_15E0:
        mov    al, 1
loc_1_15E2:
        mov    byte ptr [ecx + 0x92], 0
        mov    byte ptr [ecx + 0x90], 0
        mov    byte ptr [ecx + 0x91], 0
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1600:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_16BF
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 0x16a]
        cmp    byte ptr [eax + 0x18], 0
        je     loc_1_16BB
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [esi + 0x6d]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    eax, dword ptr [eax + ecx]
        mov    ax, word ptr [eax]
        test   ah, 0x40
        jne    loc_1_1670
        or     ax, ax
        je     loc_1_1670
        cmp    byte ptr [esi + 0x58], 0
        jne    loc_1_1670
        mov    byte ptr [esi + 0x57], 1
        mov    byte ptr [esi + 0x58], 1
        push   1
        lea    eax, [esi + 0x155]
        push   eax
        call   sub_1_04E2
        jmp    loc_1_16BF
loc_1_1670:
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_16BF
        mov    byte ptr [esi + 0x53], 1
        push   0
        push   esi
        call   sub_1_2EA0
        cmp    byte ptr [esi + 0x50], 0
        jne    loc_1_16BF
        mov    al, byte ptr [esi + 0x1c]
        and    al, 0xfb
        mov    byte ptr [esi + 0x1c], al
        inc    dword ptr [esi + 0x59]
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 0x59]
        push   esi
        push   0xdc0                         ; -> loc_1_0DC0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        jmp    loc_1_16BF
loc_1_16BB:
        mov    byte ptr [eax + 0x18], 1
loc_1_16BF:
        push   esi
        call   sub_1_15A0
        or     al, al
        je     loc_1_1709
        mov    byte ptr [esi + 0x53], 1
        push   0
        push   esi
        call   sub_1_2EA0
        mov    cl, byte ptr [esi + 0x1c]
        and    cl, 0xfb
        mov    byte ptr [esi + 0x1c], cl
        inc    dword ptr [esi + 0x59]
        mov    di, word ptr [esi]
        add    di, 6
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   esi
        call   sub_1_29F0
        dec    dword ptr [esi + 0x59]
        push   esi
        push   0xdc0                         ; -> loc_1_0DC0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
loc_1_1709:
        push   0x3e8
        add    esi, 0x149
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1730:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 0x75]
        or     esi, esi
        je     loc_1_181A
        lea    edi, [esi + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_1756
        mov    dword ptr [ebx + 0x79], 0
loc_1_1756:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0x75], eax
        mov    byte ptr [ebx + 0x4d], 1
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_0530
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 4], 0x100
        jae    loc_1_17BA
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_17BA
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 0x172]
        push   eax
        push   0x100
        push   0
        push   esi
        call   sub_1_1870
        lea    eax, [ebx + 0x172]
        mov    dword ptr [ebp - 8], eax
loc_1_17BA:
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_17D2
        cmp    dword ptr [ebp - 4], 5
        jbe    loc_1_17FB
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    loc_1_17E9
loc_1_17D2:
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xe
        push   eax
        push   0xe
loc_1_17E9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0x166]
        push   eax
        call   sub_1_00B0
loc_1_17FB:
        cmp    byte ptr [edi + 0xc], 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_044A
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
loc_1_181A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [edx + 0x24]
        mov    eax, dword ptr [ecx + 0x81]
        or     eax, eax
        je     loc_1_184A
        mov    dword ptr [eax + 0x24], edx
loc_1_184A:
        mov    dword ptr [esi], 0
        mov    dword ptr [ecx + 0x81], edx
        cmp    dword ptr [ecx + 0x7d], 0
        jne    loc_1_185F
        mov    dword ptr [ecx + 0x7d], edx
loc_1_185F:
        dec    dword ptr [ecx + 0xa3]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_193A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_193A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_1935
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_18C0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_18EC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1935
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    loc_1_1930
loc_1_18EC:
        or     edi, edi
        je     loc_1_1909
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_1901
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1930
loc_1_1901:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1909:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_1917
        mov    ebx, eax
loc_1_1917:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1930:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_1_18C0
loc_1_1935:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_193A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1950:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_1A1B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_1_1A1B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_1A16
        mov    edi, dword ptr [ebp + 0x14]
loc_1_19A4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_19D0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1A16
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    loc_1_1A11
loc_1_19D0:
        or     edi, edi
        je     loc_1_19ED
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_19E5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1A11
loc_1_19E5:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_19ED:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, esi
        mov    ebx, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_19FB
        mov    ebx, eax
loc_1_19FB:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1A11:
        cmp    esi, dword ptr [ebp + 0xc]
        jb     loc_1_19A4
loc_1_1A16:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_1A1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x8f], 0
        je     loc_1_1A4B
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_1A4B:
        mov    cl, byte ptr [ebp + 0x18]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_1_1A80
        or     al, al
        je     loc_1_1A6B
        mov    eax, 0x103
        pop    ebp
        ret    0x14
loc_1_1A6B:
        sub    eax, eax
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A80:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    al, al
        mov    dx, 2
        mov    ecx, dword ptr [ebp + 0x10]
        test   cl, 0x24
        je     loc_1_1A98
        mov    dx, 3
        jmp    loc_1_1AA0
loc_1_1A98:
        test   cl, 8
        je     loc_1_1AA0
        sub    dx, dx
loc_1_1AA0:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 0xcb], dx
        je     loc_1_1B46
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [ecx + 0xcb], dx
        mov    word ptr [eax + 2], 2
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 6], 0x60c
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 8], 0x2600
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 0xa], 0x6000
        mov    eax, dword ptr [ecx + 0x1d]
        cmp    byte ptr [ebp + 0x18], 0
        mov    word ptr [eax + 0xc], 0xf200
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 0xe], dx
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 0x10], 0x40
        jne    loc_1_1B23
        mov    esi, dword ptr [ecx + 0x21]
        mov    edx, dword ptr [ecx + 0x16a]
        lea    esi, [esi + esi*4]
        mov    eax, dword ptr [ebp + 0x14]
        lea    esi, [esi + esi*4]
        mov    dword ptr [edx + esi + 0x14], eax
        mov    byte ptr [ecx + 0x52], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1B23:
        mov    edx, dword ptr [ecx + 0x21]
        mov    eax, dword ptr [ecx + 0x16a]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 0x14], 0
        mov    eax, dword ptr [ecx + 0x21]
        push   eax
        push   ecx
        call   sub_1_0860
        mov    al, 1
loc_1_1B46:
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x8f], 0
        je     loc_1_1B6B
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_1B6B:
        mov    cl, byte ptr [ebp + 0x20]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edx
        call   sub_1_1B90
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        mov    ecx, dword ptr [ebp + 8]
        je     loc_1_1BCD
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
loc_1_1BAD:
        sub    edi, edi
        nop    
loc_1_1BB0:
        lea    edx, [eax + edi]
        mov    dl, byte ptr [esi + edx]
        mov    ebx, dword ptr [ecx + 0x1d]
        add    ebx, edi
        mov    byte ptr [esi + ebx + 8], dl
        inc    edi
        cmp    edi, 6
        jb     loc_1_1BB0
        add    esi, 6
        dec    dword ptr [ebp - 4]
        jne    loc_1_1BAD
loc_1_1BCD:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x1d]
        imul   ax, ax, 6
        mov    word ptr [edx + 6], ax
        mov    eax, dword ptr [ecx + 0x1d]
        cmp    byte ptr [ebp + 0x18], 0
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 2], 3
        jne    loc_1_1C15
        mov    esi, dword ptr [ecx + 0x21]
        mov    edx, dword ptr [ecx + 0x16a]
        lea    esi, [esi + esi*4]
        mov    eax, dword ptr [ebp + 0x14]
        lea    esi, [esi + esi*4]
        mov    dword ptr [edx + esi + 0x14], eax
        mov    byte ptr [ecx + 0x52], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1C15:
        mov    edx, dword ptr [ecx + 0x21]
        mov    eax, dword ptr [ecx + 0x16a]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 0x14], 0
        cmp    byte ptr [ecx + 0x53], 0
        je     loc_1_1C41
        push   ecx
        call   sub_1_0930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1C41:
        mov    eax, dword ptr [ecx + 0x21]
        push   eax
        push   ecx
        call   sub_1_0860
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x14]
        ret    4
        db  08Dh, 049h, 000h
loc_1_1C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1CC7
        cmp    byte ptr [eax + 0x18], 0
        jne    loc_1_1CC0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x10]
        or     edx, edx
        jl     loc_1_1C9C
        cmp    edx, 1
        jle    loc_1_1CA3
loc_1_1C9C:
        mov    edi, 0xc00000bb
        jmp    loc_1_1CCC
loc_1_1CA3:
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        push   ecx
        mov    edi, 0x103
        push   esi
        call   sub_1_1D10
        jmp    loc_1_1CCC
        db  08Dh, 049h, 000h
loc_1_1CC0:
        mov    edi, 0xc0010002
        jmp    loc_1_1CCC
loc_1_1CC7:
        mov    edi, 0xc001000d
loc_1_1CCC:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_1D04
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1CF6
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_1CF6
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_1D04
loc_1_1CF6:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_1D04:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_1D10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x35], 0
        je     loc_1_1D30
        mov    edx, dword ptr [eax + 0x39]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0x39], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D30:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x35], ecx
        mov    dword ptr [eax + 0x39], ecx
        push   eax
        call   sub_1_1D50
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x51], 0
        jne    loc_1_1F70
        mov    byte ptr [esi + 0x51], 1
        mov    edi, dword ptr [esi + 0x35]
        or     edi, edi
        je     loc_1_1F6C
loc_1_1D75:
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1F6C
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_1DA6
        cmp    eax, 1
        je     loc_1_1DCC
        cmp    eax, 2
        je     loc_1_1DF2
        cmp    eax, 3
        je     loc_1_1E16
        cmp    eax, 4
        je     loc_1_1E92
        jmp    loc_1_1F15
loc_1_1DA6:
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_2100
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F15
loc_1_1DCC:
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_1F90
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F15
loc_1_1DF2:
        lea    eax, [edi + 0x24]
        push   eax
        lea    eax, [edi + 0x20]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        push   0
        push   esi
        call   sub_1_2100
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F15
loc_1_1E16:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax + 0x166]
        push   eax
        call   sub_2_0170
        or     al, al
        jne    loc_1_1E51
        push   0
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        jmp    loc_1_1E86
loc_1_1E51:
        lea    eax, [esi + 0xd1]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0xd5]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0xd5]
        push   0
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0xd5], ebx
        inc    dword ptr [esi + 0xcd]
        inc    dword ptr [ebx + 0x14]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_043E
loc_1_1E86:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1F15
loc_1_1E92:
        mov    byte ptr [esi + 0x52], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_2_0240
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_1EB7
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_1ED0
loc_1_1EB7:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_1ED0
        cmp    dword ptr [ebp - 4], 0xc001000e
        jne    loc_1_1ED0
        mov    dword ptr [ebp - 4], 0x103
loc_1_1ED0:
        cmp    byte ptr [esi + 0x52], 0
        je     loc_1_1EE0
        mov    eax, dword ptr [esi + 0x21]
        push   eax
        push   esi
        call   sub_1_0860
loc_1_1EE0:
        mov    byte ptr [ebx + 0x18], 1
        dec    dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0xd9]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0xdd]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0xdd]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0xdd], ebx
loc_1_1F15:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_1F79
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x35], eax
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 2
        jne    loc_1_1F44
        inc    dword ptr [esi + 0x59]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_047A
        dec    dword ptr [esi + 0x59]
        jmp    loc_1_1F61
loc_1_1F44:
        or     eax, eax
        je     loc_1_1F4D
        cmp    eax, 1
        jne    loc_1_1F61
loc_1_1F4D:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 0x14]
loc_1_1F61:
        mov    edi, dword ptr [esi + 0x35]
        or     edi, edi
        jne    loc_1_1D75
loc_1_1F6C:
        mov    byte ptr [esi + 0x51], 0
loc_1_1F70:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1F79:
        mov    byte ptr [esi + 0x51], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010e
        mov    dword ptr [edi], 0
        je     loc_1_1FD9
        cmp    eax, 0x1010f
        je     loc_1_2053
        cmp    eax, 0x10112
        je     loc_1_2066
        cmp    eax, 0x1010103
        je     loc_1_20A5
        mov    eax, 0xc0010017
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1FD9:
        mov    esi, dword ptr [ebp + 0x18]
        cmp    esi, 4
        je     loc_1_1FF4
        mov    dword ptr [edi], 4
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1FF4:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 4], 0xf050
        je     loc_1_2028
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc00000bb
        mov    dword ptr [ecx], 4
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2028:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x166]
        push   eax
        call   sub_2_05E0
        mov    ecx, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2053:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2066:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_2080
        mov    dword ptr [edi], 4
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_1_2080:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x41
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_20A5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        cdq    
        idiv   ecx
        or     edx, edx
        je     loc_1_20C5
        mov    dword ptr [edi], ecx
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_20C5:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq    
        idiv   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x166]
        push   eax
        call   sub_2_0300
        mov    ecx, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  005h, 000h, 000h, 000h, 000h
sub_1_2100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 0
        lea    esi, [ebp - 8]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [eax], 0
        mov    ecx, 4
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [eax], 0
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, edx
        cmp    eax, 0x1010101
        ja     loc_1_2160
        je     loc_1_2553
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2181
        shl    eax, 2
        jmp    loc_1_267C
        db  08Dh, 049h, 000h
loc_1_2160:
        cmp    eax, 0x1010102
        je     loc_1_2578
        cmp    eax, 0x1010103
        je     loc_1_25A0
        cmp    eax, 0x1010104
        je     loc_1_2630
loc_1_2181:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_2193
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2637
loc_1_2193:
        cmp    edx, 0x1020102
        ja     loc_1_2210
        je     loc_1_2364
        cmp    edx, 0x20105
        ja     loc_1_21E4
        je     loc_1_2314
        cmp    edx, 0x20101
        je     loc_1_2274
        cmp    edx, 0x20102
        je     loc_1_2285
        cmp    edx, 0x20103
        je     loc_1_2296
        cmp    edx, 0x20104
        je     loc_1_22B9
        jmp    loc_1_2268
loc_1_21E4:
        cmp    edx, 0x2020d
        je     loc_1_2327
        cmp    edx, 0x2020e
        je     loc_1_2340
        cmp    edx, 0x1020101
        je     loc_1_2351
        jmp    loc_1_2268
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2210:
        cmp    edx, 0x1020203
        ja     loc_1_2244
        je     loc_1_23A8
        cmp    edx, 0x1020103
        je     loc_1_2375
        cmp    edx, 0x1020201
        je     loc_1_2386
        cmp    edx, 0x1020202
        je     loc_1_2397
        jmp    loc_1_2268
loc_1_2244:
        cmp    edx, 0x1020204
        je     loc_1_23C0
        cmp    edx, 0x1020205
        je     loc_1_23D1
        cmp    edx, 0x1020206
        je     loc_1_23E2
loc_1_2268:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2637
loc_1_2274:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x9b]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2285:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x9f]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2296:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xaf]
        add    eax, dword ptr [edi + 0xa7]
        add    eax, dword ptr [edi + 0xab]
        add    eax, dword ptr [edi + 0xb3]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_22B9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 2], ax
        and    eax, 0xffff
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 2], ax
        and    eax, 0xffff
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 2], ax
        and    eax, 0xffff
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 0xc3]
        add    eax, dword ptr [edi + 0xc7]
        add    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2314:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2637
loc_1_2327:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2637
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2340:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2351:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2637
loc_1_2364:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xbb]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2375:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xbf]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2386:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xb7]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2397:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_23A8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2637
        db  005h, 000h, 000h, 000h, 000h
loc_1_23C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xaf]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_23D1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xb3]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_23E2:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_23F3:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_2408
        mov    esi, 0x365c                   ; -> loc_1_365C
        mov    ecx, 0xa0
        jmp    loc_1_2637
loc_1_2408:
        mov    esi, 0x36fc                   ; -> obj1:0x36FC
        mov    ecx, 0x5c
        jmp    loc_1_2637
loc_1_2417:
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x8f], 0
        je     loc_1_2430
        mov    dword ptr [ebp - 0x10], 2
        jmp    loc_1_2444
        db  08Dh, 064h, 024h, 000h
loc_1_2430:
        cmp    byte ptr [edi + 0x53], 0
        mov    dword ptr [ebp - 0x10], 1
        jne    loc_1_2444
        mov    dword ptr [ebp - 0x10], 0
loc_1_2444:
        lea    esi, [ebp - 0x10]
        mov    ecx, 4
        jmp    loc_1_2637
loc_1_2451:
        lea    esi, [ebp - 0x1c]
        mov    ecx, 4
        jmp    loc_1_2637
        db  08Bh, 0FFh
loc_1_2460:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_2470:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_2480:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x21]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
        db  08Bh, 0FFh
loc_1_24A0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x25]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
        db  08Bh, 0FFh
loc_1_24C0:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2637
        db  08Dh, 064h, 024h, 000h
loc_1_24D0:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x29
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    ecx, 4
        jmp    loc_1_2637
loc_1_24F6:
        mov    esi, 0x3758                   ; -> loc_1_3758
        mov    ecx, 0xf
        jmp    loc_1_2637
loc_1_2505:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        je     loc_1_2520
        mov    eax, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2520:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x166]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2637
loc_1_2534:
        mov    word ptr [ebp - 4], 0x300
        lea    esi, [ebp - 4]
        mov    ecx, 2
        jmp    loc_1_2637
loc_1_2547:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_2637
loc_1_2553:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x29]
        add    edi, 0x29
        mov    dword ptr [ebp - 0x28], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x24], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 0x28]
        mov    byte ptr [ebp - 0x23], al
        mov    ecx, 6
        jmp    loc_1_2637
loc_1_2578:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x2f]
        add    edi, 0x2f
        mov    dword ptr [ebp - 0x28], eax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebp - 0x24], al
        mov    al, byte ptr [edi + 5]
        lea    esi, [ebp - 0x28]
        mov    byte ptr [ebp - 0x23], al
        mov    ecx, 6
        jmp    loc_1_2637
        db  08Dh, 049h, 000h
loc_1_25A0:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        jne    loc_1_25D8
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 0x166]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_2_0760
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp + 0x14]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_2637
loc_1_25D8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x166]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_2_06A0
        cmp    dword ptr [ebp - 0x18], 0
        mov    esi, dword ptr [ebp + 0x14]
        jne    loc_1_2613
        mov    eax, dword ptr [ebp - 0x14]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_2637
loc_1_2613:
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x166]
        push   eax
        call   sub_2_0660
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_2637
        db  090h
loc_1_2630:
        mov    dword ptr [ebp - 8], 0x10
loc_1_2637:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_266E
        cmp    dword ptr [ebp + 0x18], ecx
        jae    loc_1_265A
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010016
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_265A:
        mov    eax, dword ptr [ebp + 0x1c]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     loc_1_266E
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_05EC
loc_1_266E:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_267C:
        and    eax, dword ptr [eax]
        add    byte ptr [edi], dl            ; -> loc_1_2417
        and    al, 0
        add    byte ptr [ecx + 0x24], dl     ; -> loc_1_2451
        add    byte ptr [eax], al
        push   ecx
        and    al, 0
        add    byte ptr [eax + 0x24], ah     ; -> loc_1_2460
        add    byte ptr [eax], al
        pushal 
        and    al, 0
        add    byte ptr [eax + 0x24], dh     ; -> loc_1_2470
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        mov    al, byte ptr [0xc0000024]     ; -> loc_1_24C0
        and    al, 0
        add    al, al                        ; -> loc_1_24C0
        and    al, 0
        add    al, dl                        ; -> loc_1_24D0
        and    al, 0
        add    dh, dh                        ; -> loc_1_24F6
        and    al, 0
        add    byte ptr [0x60000025], al     ; -> loc_1_2505
        and    al, 0
        add    byte ptr [0x24c00000], dh     ; -> loc_1_2534
        add    byte ptr [eax], al
        and    dword ptr [ecx], 0x25470000   ; -> loc_1_2547
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
loc_1_26D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_2710
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 2
        je     loc_1_26F4
        mov    edi, 0xc00000bb
        jmp    loc_1_2715
loc_1_26F4:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        push   eax
        push   esi
        call   sub_1_1D10
        mov    edi, 0x103
        jmp    loc_1_2715
        db  090h
loc_1_2710:
        mov    edi, 0xc001000d
loc_1_2715:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_274D
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_273F
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_273F
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_274D
loc_1_273F:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_274D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2760:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1a]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    eax, dword ptr [esi + 0x49]
        mov    word ptr [ecx + 0xffc6], ax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    word ptr [eax + 0xffc2], 0x10
        mov    dx, word ptr [esi]
        add    dx, 0xb
        sub    al, al
        out    dx, al
        mov    byte ptr [esi + 0x4e], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x1a]
        mov    ecx, dword ptr [eax + 2]
        sub    ecx, edx
        pop    ebp
        mov    word ptr [ecx + 0xffc2], 0x440
        mov    dx, word ptr [eax]
        add    dx, 0xb
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x1a]
        mov    ecx, dword ptr [eax + 2]
        sub    ecx, edx
        pop    ebp
        mov    word ptr [ecx + 0xffc2], 0x80
        mov    dx, word ptr [eax]
        add    dx, 0xb
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 0x103
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 0x59]
        cmp    byte ptr [edi + 0x8f], 0
        jne    loc_1_2888
        cmp    byte ptr [ebx + 0x18], 0
        jne    loc_1_2842
        inc    dword ptr [ebx + 0x14]
        push   ebx
        push   edi
        call   sub_1_2EA0
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_2847
loc_1_2842:
        mov    esi, 0xc0010002
loc_1_2847:
        mov    eax, dword ptr [edi + 0x59]
        dec    eax
        mov    dword ptr [edi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_287F
        cmp    byte ptr [edi + 0x8f], 0
        jne    loc_1_2871
        cmp    dword ptr [edi + 0x75], 0
        jne    loc_1_2871
        mov    eax, dword ptr [edi + 0xd9]
        sub    eax, edi
        cmp    eax, 0xd9
        je     loc_1_287F
loc_1_2871:
        push   0
        add    edi, 0x155
        push   edi
        call   sub_1_04E2
loc_1_287F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2888:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_28A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x1d]
        add    ecx, 6
        mov    edi, 0
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    word ptr [esi + 0xcb], 2
        mov    eax, dword ptr [esi + 0x166]
        or     eax, eax
        je     loc_1_28E0
        mov    edi, dword ptr [eax + 0xc]
loc_1_28E0:
        test   edi, 0x20
        je     loc_1_28F3
        mov    word ptr [esi + 0xcb], 3
        jmp    loc_1_2917
loc_1_28F3:
        test   edi, 4
        je     loc_1_2906
        mov    word ptr [esi + 0xcb], 3
        jmp    loc_1_2917
loc_1_2906:
        test   edi, 8
        je     loc_1_2917
        mov    word ptr [esi + 0xcb], 0
loc_1_2917:
        cmp    byte ptr [esi + 0x56], 0
        jne    loc_1_2926
        or     word ptr [esi + 0xcb], 0x8800
loc_1_2926:
        mov    word ptr [ecx], 0x60c
        mov    word ptr [ecx + 2], 0x2600
        mov    word ptr [ecx + 4], 0x6000
        mov    word ptr [ecx + 6], 0xf200
        mov    word ptr [ecx + 0xa], 0x40
        mov    ax, word ptr [esi + 0xcb]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi + 0x1d]
        push   esi
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 2], 2
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 4], 0xffff
        call   sub_1_0930
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 4], 0xffff
        sub    eax, eax
        lea    ebx, [ebx]
loc_1_2990:
        mov    cl, byte ptr [eax + esi + 0x2f]
        mov    edx, dword ptr [esi + 0x1d]
        mov    byte ptr [eax + edx + 6], cl
        inc    eax
        cmp    eax, 6
        jb     loc_1_2990
        push   esi
        call   sub_1_0930
        test   edi, 2
        je     loc_1_29E1
        lea    edi, [esi + 0x272]
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_29E1
        push   1
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_1B90
loc_1_29E1:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
loc_1_2A01:
        or     edi, edi
        je     loc_1_2A11
        cmp    edi, 1
        je     loc_1_2A16
        cmp    edi, 2
        je     loc_1_2A20
        jmp    loc_1_2A26
loc_1_2A11:
        mov    ebx, dword ptr [esi + 0x75]
        jmp    loc_1_2A26
loc_1_2A16:
        mov    ebx, dword ptr [esi + 0x7d]
        jmp    loc_1_2A26
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A20:
        mov    ebx, dword ptr [esi + 0x87]
loc_1_2A26:
        or     ebx, ebx
        je     loc_1_2A4A
loc_1_2A2A:
        lea    eax, [ebx + 0x24]
        mov    ecx, ebx
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   0xc001000c
        dec    dword ptr [eax + 0x14]
        push   ecx
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_044A
        or     ebx, ebx
        jne    loc_1_2A2A
loc_1_2A4A:
        inc    edi
        cmp    edi, 3
        jb     loc_1_2A01
        push   esi
        call   sub_1_2A60
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_2A60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AB0
        push   esi
        call   sub_1_2BD0
        mov    bl, al
        or     bl, bl
        je     loc_1_2AD2
        push   esi
        call   sub_1_2FB0
        push   esi
        call   sub_1_28A0
        mov    cl, byte ptr [esi + 0x1c]
        or     cl, 4
        mov    byte ptr [esi + 0x1c], cl
        mov    di, word ptr [esi]
        add    di, 6
        mov    dx, di
        mov    al, cl
        out    dx, al
        push   0
        push   esi
        call   sub_1_2D50
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_1_2AC5
        push   0x1388
        add    esi, 0x149
        push   esi
        call   sub_1_04E2
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2AC5:
        mov    byte ptr [esi + 0x53], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2AD2:
        push   0xc0000001
        push   esi
        call   sub_1_2D50
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x4f], 0
        je     loc_1_2B79
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x49], eax
        sub    ebx, ebx
loc_1_2B15:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_2B3C
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_2B15
loc_1_2B3C:
        push   esi
        push   0x2760                        ; -> loc_1_2760
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        sub    edi, edi
        sub    ebx, ebx
loc_1_2B52:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_2B79
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_2B52
loc_1_2B79:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_1_2BB2
        push   esi
        push   0x27b0                        ; -> loc_1_27B0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
loc_1_2BB2:
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        mov    dx, word ptr [esi]
        add    dx, 6
        and    al, 0xfb
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2BD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2F40
        cmp    byte ptr [esi + 0x4f], 0
        jne    loc_1_2C34
        mov    eax, dword ptr [esi + 0xa]
        shl    eax, 0xa
        push   eax
        mov    eax, dword ptr [esi + 6]
        push   eax
        mov    eax, dword ptr [esi + 0xf1]
        lea    ecx, [esi + 2]
        push   eax
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2C30
        push   0
        push   1
        push   2
        push   0xaa000001
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2C30:
        mov    byte ptr [esi + 0x4f], 1
loc_1_2C34:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0xfff4
        mov    dword ptr [esi + 0xe], eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0xffe0
        mov    dword ptr [esi + 0x12], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x1a]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, eax
        add    ecx, 0xffc0
        mov    dword ptr [esi + 0x16], ecx
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0xff58
        mov    dword ptr [esi + 0x1d], eax
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [esi + 0x61], eax
        mov    ecx, dword ptr [esi + 0x21]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 3
        lea    ecx, [edx + ecx*8]
        lea    ecx, [eax + ecx*8]
        mov    eax, dword ptr [esi + 0xe]
        mov    dword ptr [esi + 0x5d], ecx
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 0xe]
        mov    word ptr [eax + 0xa], 0
        mov    eax, dword ptr [esi + 0xe]
        mov    word ptr [eax + 8], 0xffe0
        mov    eax, dword ptr [esi + 0x12]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 0x12]
        mov    word ptr [eax + 2], 0xffc0
        mov    eax, dword ptr [esi + 0x12]
        mov    word ptr [eax], 1
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 4], 0xffff
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 6], 0xffff
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 0xa], 0
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [esi + 0x16]
        mov    word ptr [eax + 0xe], 0
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, 0x80
        out    dx, al
        push   0x3e8
        call   sub_1_0500
        mov    si, word ptr [esi]
        mov    al, 4
        lea    ecx, [esi + 6]
        mov    dx, cx
        out    dx, al
        mov    al, 0x84
        out    dx, al
        lea    edx, [esi + 0xb]
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        mov    edi, dword ptr [esi + 0x93]
        mov    ebx, dword ptr [esi + 0xd1]
        sub    eax, ebx
        cmp    eax, 0xffffff2f
        je     loc_1_2DB8
loc_1_2D70:
        inc    dword ptr [ebx + 0x14]
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_2D8B
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_046E
loc_1_2D8B:
        push   4
        lea    eax, [ebp + 0xc]
        push   eax
        push   0x40010005
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0474
        dec    dword ptr [ebx + 0x14]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0xffffff2f
        jne    loc_1_2D70
loc_1_2DB8:
        or     edi, edi
        je     loc_1_2DCC
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0456
        dec    dword ptr [edi + 0x14]
loc_1_2DCC:
        mov    byte ptr [esi + 0x8f], 0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_2DF9
        mov    eax, dword ptr [esi + 0x3d]
        shl    eax, 4
        add    eax, dword ptr [esi + 0x16e]
        push   eax
        push   esi
        call   sub_1_2AF0
        push   esi
        call   sub_1_1D50
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2DF9:
        push   1
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x35]
        add    esi, 0x35
        or     edi, edi
        je     loc_1_2E96
        cmp    dword ptr [edi + 0x10], 4
        jne    loc_1_2E35
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 0x14]
        mov    edi, dword ptr [esi]
loc_1_2E35:
        or     edi, edi
        je     loc_1_2E96
loc_1_2E39:
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 4
        jne    loc_1_2E45
        mov    esi, edi
        jmp    loc_1_2E90
loc_1_2E45:
        cmp    eax, 3
        jne    loc_1_2E77
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_2E73
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        jmp    loc_1_2E90
loc_1_2E73:
        mov    esi, edi
        jmp    loc_1_2E90
loc_1_2E77:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    ebx, dword ptr [edi + 4]
        push   0xc001000c
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 0x14]
loc_1_2E90:
        mov    edi, dword ptr [esi]
        or     edi, edi
        jne    loc_1_2E39
loc_1_2E96:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_2EA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x149]
        push   eax
        call   sub_1_04E8
        push   esi
        call   sub_1_2B90
        mov    byte ptr [esi + 0x1c], 0x84
        mov    edi, dword ptr [esi + 0xd1]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff2f
        je     loc_1_2F04
loc_1_2ED6:
        inc    dword ptr [edi + 0x14]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_046E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0474
        dec    dword ptr [edi + 0x14]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff2f
        jne    loc_1_2ED6
loc_1_2F04:
        mov    byte ptr [esi + 0x8f], 1
        mov    byte ptr [esi + 0x85], 0
        push   0
        push   esi
        call   sub_1_2E10
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x93], eax
        or     eax, eax
        je     loc_1_2F2A
        inc    dword ptr [eax + 0x14]
loc_1_2F2A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2F40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [esi]
        lea    edx, [ecx + 6]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [esi + 0x29], al
        mov    dx, cx
        inc    dx
        in     al, dx
        mov    byte ptr [esi + 0x2a], al
        mov    dx, cx
        add    dx, 2
        in     al, dx
        mov    byte ptr [esi + 0x2b], al
        mov    dx, cx
        add    dx, 3
        in     al, dx
        mov    byte ptr [esi + 0x2c], al
        mov    dx, cx
        add    dx, 4
        in     al, dx
        mov    byte ptr [esi + 0x2d], al
        mov    dx, cx
        add    dx, 5
        in     al, dx
        mov    byte ptr [esi + 0x2e], al
        cmp    byte ptr [esi + 0x55], 0
        jne    loc_1_2FA7
        mov    ecx, dword ptr [esi + 0x29]
        mov    dword ptr [esi + 0x2f], ecx
        mov    cl, byte ptr [esi + 0x2d]
        mov    byte ptr [esi + 0x33], cl
        lea    eax, [esi + 0x2f]
        mov    cl, byte ptr [esi + 0x2e]
        mov    byte ptr [eax + 5], cl
loc_1_2FA7:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], 0
        mov    ebx, dword ptr [esi + 0x61]
        cmp    dword ptr [esi + 0x21], 0
        jbe    loc_1_3093
        sub    edx, edx
        sub    eax, eax
loc_1_2FD4:
        mov    edi, ebx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [edx + ecx + 0xc], 0xffff
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [edx + ecx + 0x10], eax
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [edx + ecx + 0x14], eax
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [edx + ecx], ebx
        mov    cx, word ptr [esi + 0x1a]
        sub    cx, word ptr [esi + 2]
        add    cx, bx
        mov    edi, dword ptr [esi + 0x16a]
        mov    word ptr [edx + edi + 4], cx
        lea    ecx, [ebx + 0x18]
        mov    edi, dword ptr [esi + 0x16a]
        mov    dword ptr [edx + edi + 6], ecx
        mov    ecx, dword ptr [esi + 0x16a]
        add    ecx, edx
        mov    edi, dword ptr [ecx + 6]
        sub    di, word ptr [esi + 2]
        add    di, word ptr [esi + 0x1a]
        mov    word ptr [ecx + 0xa], di
        lea    ecx, [ebx + 0x10]
        sub    cx, word ptr [esi + 2]
        add    cx, word ptr [esi + 0x1a]
        mov    word ptr [ebx + 6], cx
        mov    word ptr [ebx + 4], 0xffff
        mov    word ptr [ebx + 0x12], 0xffff
        mov    ecx, dword ptr [esi + 0x16a]
        add    edx, 0x19
        inc    dword ptr [ebp - 0xc]
        mov    cx, word ptr [ecx + edx - 0xf]
        and    ecx, 0xffff
        mov    dword ptr [ebx + 0x14], ecx
        mov    ecx, dword ptr [esi + 0x16a]
        mov    ebx, dword ptr [edx + ecx - 0x13]
        add    ebx, 0x5f0
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x21], ecx
        ja     loc_1_2FD4
loc_1_3093:
        mov    edi, dword ptr [esi + 0x1d]
        sub    eax, eax
        mov    ecx, 0x1a
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esi + 0x21]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    dword ptr [ecx + eax + 0xc], 0xffff
        mov    ecx, dword ptr [esi + 0x21]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    dword ptr [ecx + eax + 0x10], 0
        mov    ecx, dword ptr [esi + 0x21]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    dword ptr [ecx + eax + 0x14], 0
        mov    eax, dword ptr [esi + 0x21]
        mov    edx, dword ptr [esi + 0x1d]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [eax + ecx], edx
        mov    eax, dword ptr [esi + 0x21]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    word ptr [eax + ecx + 4], 0xff58
        mov    ecx, dword ptr [esi + 0x21]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    dword ptr [ecx + eax + 6], 0
        mov    ecx, dword ptr [esi + 0x21]
        sub    edx, edx
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    word ptr [ecx + eax + 0xa], 0xffff
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 4], 0xffff
        mov    ebx, dword ptr [esi + 0x5d]
        cmp    dword ptr [esi + 0x25], edx
        jbe    loc_1_321D
        mov    dword ptr [ebp - 4], edx
loc_1_3156:
        mov    eax, dword ptr [esi + 0x16e]
        add    eax, dword ptr [ebp - 4]
        mov    edi, ebx
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x16e]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + ecx], ebx
        mov    di, word ptr [esi + 0x1a]
        sub    di, word ptr [esi + 2]
        add    di, bx
        mov    eax, dword ptr [esi + 0x16e]
        mov    word ptr [ecx + eax + 4], di
        lea    edi, [ebx + 0x16]
        sub    di, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 0x16e]
        mov    eax, dword ptr [ecx + eax]
        add    di, word ptr [esi + 0x1a]
        mov    word ptr [eax + 6], di
        lea    eax, [edx + 1]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        div    dword ptr [esi + 0x25]
        mov    eax, dword ptr [esi + 0x16e]
        mov    dword ptr [ecx + eax + 0xc], edx
        add    ebx, 0x20
        mov    eax, dword ptr [esi + 0x16e]
        mov    dword ptr [ecx + eax + 8], ebx
        mov    eax, dword ptr [esi + 0x16e]
        add    eax, ecx
        mov    ecx, dword ptr [eax + 8]
        sub    cx, word ptr [esi + 2]
        add    cx, word ptr [esi + 0x1a]
        mov    word ptr [eax + 6], cx
        mov    eax, dword ptr [esi + 0x16e]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + ecx + 8]
        add    ebx, 0x5f0
        add    dword ptr [ebp - 4], 0x10
        mov    edx, dword ptr [ebp - 8]
        mov    eax, edx
        cmp    dword ptr [esi + 0x25], eax
        ja     loc_1_3156
loc_1_321D:
        sub    edx, edx
        cmp    dword ptr [esi + 0x25], edx
        jbe    loc_1_326D
        sub    ecx, ecx
loc_1_3226:
        mov    edi, dword ptr [esi + 0x16e]
        add    edi, ecx
        mov    eax, dword ptr [edi + 0xc]
        mov    ebx, dword ptr [edi]
        mov    di, word ptr [edi + 6]
        and    edi, 0xffff
        mov    dword ptr [ebx + 0x1a], edi
        mov    word ptr [ebx + 0x1e], 0x85ea
        mov    word ptr [ebx + 0x16], 0
        mov    word ptr [ebx + 0x18], 0xffff
        shl    eax, 4
        mov    edi, dword ptr [esi + 0x16e]
        mov    ax, word ptr [eax + edi + 4]
        mov    word ptr [ebx + 4], ax
        add    ecx, 0x10
        inc    edx
        cmp    dword ptr [esi + 0x25], edx
        ja     loc_1_3226
loc_1_326D:
        mov    word ptr [ebx + 2], 0xc000
        mov    dword ptr [esi + 0x3d], 0
        mov    eax, dword ptr [esi + 0x25]
        dec    eax
        mov    dword ptr [esi + 0x41], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3290:
        mov    al, 0xff
        mov    ecx, 0xff
loc_1_3297:
        mov    dx, 0x100
        out    dx, al
        test   al, 0x80
        je     loc_1_32AA
        add    al, al
        xor    al, 0xe7
        dec    ecx
        jne    loc_1_3297
        ret    4
loc_1_32AA:
        add    al, al
        dec    ecx
        jne    loc_1_3297
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_32C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x103
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_33E0
        cmp    byte ptr [ebx + 0x18], 0
        jne    loc_1_33D0
        inc    dword ptr [ebx + 0x14]
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   0
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        call   sub_1_0530
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_33C4
        cmp    dword ptr [ebp - 0xc], 0x5ea
        ja     loc_1_33C4
        lea    ecx, [edi + 0x24]
        mov    dword ptr [ebp - 8], ecx
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0530
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0554
        test   byte ptr [ebx + 0x41], 2
        je     loc_1_3353
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xd], 0
        jmp    loc_1_3369
loc_1_3353:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_1_0330
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xd], al
loc_1_3369:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], ebx
        mov    eax, dword ptr [esi + 0x8b]
        or     eax, eax
        jne    loc_1_3381
        mov    dword ptr [esi + 0x87], edi
        jmp    loc_1_3384
loc_1_3381:
        mov    dword ptr [eax + 0x24], edi
loc_1_3384:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x8b], edi
        mov    dword ptr [ecx], 0
        inc    dword ptr [esi + 0xa3]
        cmp    byte ptr [esi + 0x86], 0
        jne    loc_1_33E7
        sub    ebx, ebx
loc_1_33A4:
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_33E7
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_33E7
        push   esi
        call   sub_1_3430
        cmp    byte ptr [esi + 0x86], bl
        je     loc_1_33A4
        jmp    loc_1_33E7
loc_1_33C4:
        dec    dword ptr [ebx + 0x14]
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_1_33E7
loc_1_33D0:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_33E7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_33E0:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_1_33E7:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_341F
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_3411
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_3411
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_341F
loc_1_3411:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_341F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_3430:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [esi + 0x87]
        mov    byte ptr [esi + 0x86], 1
        push   eax
        push   esi
        call   sub_1_09F0
        or     al, al
        je     loc_1_3498
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x87], eax
        or     eax, eax
        jne    loc_1_346D
        mov    dword ptr [esi + 0x8b], 0
loc_1_346D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0700
        push   edi
        push   esi
        call   sub_1_1830
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_0860
        mov    byte ptr [esi + 0x86], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3498:
        mov    byte ptr [esi + 0x86], 0
        mov    byte ptr [esi + 0x85], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_34B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_3520
        cmp    byte ptr [edi + 0x18], 0
        jne    loc_1_3517
        inc    dword ptr [edi + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        test   cl, 1
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        jne    loc_1_34FA
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0xe
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_057E
        dec    dword ptr [edi + 0x14]
        sub    edi, edi
        jmp    loc_1_3525
loc_1_34FA:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        and    ecx, 0xfffffffe
        push   eax
        add    ecx, dword ptr [ebp + 0x10]
        push   ecx
        call   sub_1_1950
        dec    dword ptr [edi + 0x14]
        sub    edi, edi
        jmp    loc_1_3525
loc_1_3517:
        mov    edi, 0xc001000c
        jmp    loc_1_3525
        db  08Bh, 0FFh
loc_1_3520:
        mov    edi, 0xc001000d
loc_1_3525:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_355D
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_354F
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_354F
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_355D
loc_1_354F:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_355D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_3574:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
loc_1_357C:
        pop    es
        add    byte ptr [edi], al
        add    byte ptr [ebp + esi], dh      ; -> loc_1_3574
        add    byte ptr [esi + 0x45], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        push   esp
        jb     loc_1_3600
        outsb  dx, byte ptr [esi]
        jae    0x3605
        imul   esi, dword ptr gs:[esi + 0x65], 0x72540072
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_1_3611
        imul   esi, dword ptr gs:[esi + 0x65], 0x6e490072
        je     0x361d
        jb     0x362c
        jne    0x362c
        je     loc_1_360C
        jne    0x362d
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x6e], cl
        je     0x362d
        jb     loc_1_363C
        jne    loc_1_363C
        je     0x361c
        jne    0x363d
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x4f], cl
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_35DC:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x3663
        dec    ebp
        popal  
        jo     loc_1_365E
        inc    edx
        popal  
        jae    0x3659
        inc    ecx
        jb     loc_1_365E
        jae    0x366e
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_1_3600:
        jb     loc_1_367B
        dec    ebp
        popal  
        jo     0x3676
        inc    edx
        popal  
        jae    loc_1_3671
loc_1_360C:
        inc    ecx
        jb     0x3676
loc_1_3611:
        jae    0x3686
        add    byte ptr [edx + 0x65], bl
        jb     loc_1_3687
        push   edi
        popal  
        imul   esi, dword ptr [ebx + edx*2 + 0x74], 0x657461
        add    byte ptr [eax], al
        pop    edx
        jb     0x3697
        push   edi
        popal  
        imul   esi, dword ptr [ebx + edx*2 + 0x74], 0x657461
        add    byte ptr [eax], al
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_36B3
        dec    ebp
        popal  
loc_1_363C:
        jo     0x36ae
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_36C7
        dec    ebp
        popal  
        jo     0x36c2
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0
loc_1_365C:
        add    dword ptr [ecx], eax
loc_1_365E:
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
loc_1_3671:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
loc_1_367B:
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
loc_1_3687:
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
loc_1_36B3:
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0xd000201
        add    al, byte ptr [edx]
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
loc_1_36C7:
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
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    al, 2
        add    al, byte ptr [ecx]
        add    eax, 0x6010202
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
loc_1_3758:
        inc    ebp
        insb   byte ptr es:[edi], dx
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ecx], 0x36
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x37d8
        jb     loc_1_3767
loc_1_3767:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x376c]       ; -> obj1:0x376C
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
        mov    eax, dword ptr [0x376c]       ; -> obj1:0x376C
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0xa80  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   esi
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_1_041A
        mov    dword ptr [0x377c], esi       ; -> obj1:0x377C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x3770], eax       ; -> obj1:0x3770
        mov    byte ptr [ebp - 0x40], 3
        mov    byte ptr [ebp - 0x3f], 0
        mov    dword ptr [ebp - 0x3a], 0xb30 ; -> loc_1_0B30
        mov    dword ptr [ebp - 0x36], 0xc60 ; -> loc_1_0C60
        mov    dword ptr [ebp - 0x26], 0x1c70 ; -> loc_1_1C70
        mov    dword ptr [ebp - 0x32], 0x32c0 ; -> loc_1_32C0
        mov    dword ptr [ebp - 0x2e], 0x34b0 ; -> loc_1_34B0
        mov    dword ptr [ebp - 0x2a], 0x2810 ; -> loc_1_2810
        mov    dword ptr [ebp - 0x22], 0x26d0 ; -> loc_1_26D0
        mov    dword ptr [ebp - 0x1e], 0xcf0 ; -> loc_1_0CF0
        mov    dword ptr [ebp - 0x1a], 0x5d0 ; -> loc_4_05D0
        mov    dword ptr [ebp - 0x16], 0xd20 ; -> loc_1_0D20
        mov    eax, dword ptr [0x357c]       ; -> loc_1_357C
        mov    ecx, dword ptr [0x3580]       ; -> obj1:0x3580
        mov    dword ptr [ebp - 0x12], eax
        lea    eax, [ebp - 0x40]
        mov    dword ptr [ebp - 0xe], ecx
        mov    dword ptr [0x3778], 0x3774    ; -> obj1:0x3778
        mov    dword ptr [0x3774], 0x3774    ; -> obj1:0x3774
        push   0x36
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x3784                        ; -> obj1:0x3784
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_00C4
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_00C4:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   edi
        mov    eax, dword ptr [0x3570]       ; -> obj1:0x3570
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x2d2
        push   eax
        call   sub_1_0506
        or     eax, eax
        jne    loc_4_04D1
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xb4
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xed], eax
        mov    edx, dword ptr [ebp + 0x2c]
        or     edx, edx
        mov    eax, dword ptr [ebp - 4]
        je     loc_4_0146
        mov    byte ptr [eax + 0x55], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 0x2f], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 0x33], cl
        add    eax, 0x2f
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
        jmp    loc_4_014A
loc_4_0146:
        mov    byte ptr [eax + 0x55], 0
loc_4_014A:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebp + 0x20]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0x84
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 6], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xa], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x4f], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0xd1
        mov    dword ptr [ecx + 0xd5], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xd5]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0xd9
        mov    dword ptr [ecx + 0xdd], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xdd]
        mov    dword ptr [ecx + 0xd9], eax
        mov    cl, 0x1f
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp - 4]
        sub    cl, dl
        mov    byte ptr [eax + 0x161], cl
        mov    eax, dword ptr [ebp - 4]
        lea    edi, [ebp - 0x2c]
        mov    dword ptr [eax + 0x162], edx
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x2c], eax
        mov    byte ptr [ebp - 0x28], al
        mov    dword ptr [ebp - 0x22], 1
        mov    dword ptr [ebp - 0x1e], eax
        mov    dword ptr [ebp - 0x1a], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x16], 1
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x12], eax
        mov    dword ptr [ebp - 0xe], 0x10
        mov    dword ptr [ebp - 0xa], 0
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 0xed]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xf1
        push   eax
        call   sub_1_0432
        or     eax, eax
        je     loc_4_0276
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_0276:
        cmp    byte ptr [ebp + 0x38], 0
        je     loc_4_02C0
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp + 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_0438
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_02C0:
        mov    al, byte ptr [ebp + 0x28]
        push   eax
        mov    al, byte ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_07F0
        or     al, al
        jne    loc_4_031A
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000004
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_0438
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_031A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_04E0
        or     al, al
        jne    loc_4_036C
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_0438
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_036C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0580
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0AB0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x149], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14d], 0x1600 ; -> loc_1_1600
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x151], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x155], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x159], 0xe90 ; -> loc_1_0E90
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [eax + 0x15d], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x162]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0740
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        jne    loc_4_0410
        push   eax
        call   sub_1_0A70
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_0438
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_0410:
        add    eax, 0x166
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe9
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2f
        push   eax
        push   0x1c60                        ; -> loc_1_1C60
        push   0x1a30                        ; -> loc_1_1A30
        push   0x1b50                        ; -> loc_1_1B50
        mov    eax, dword ptr [ebp + 0x30]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_4_0490
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0A70
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        push   eax
        call   sub_1_0438
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_0490:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe1], 0x3774 ; -> obj1:0x3774
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [0x3778]       ; -> obj1:0x3778
        mov    dword ptr [ecx + 0xe5], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x3778]       ; -> obj1:0x3778
        add    eax, 0xe1
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe1
        mov    dword ptr [0x3778], eax       ; -> obj1:0x3778
        sub    eax, eax
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
loc_4_04D1:
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x38
        db  08Dh, 049h, 000h
sub_4_04E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [0x3570]       ; -> obj1:0x3570
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 0x21]
        lea    eax, [eax + eax*4 + 5]
        lea    eax, [eax + eax*4]
        push   eax
        lea    eax, [esi + 0x16a]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_4_0531
        push   0
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   esi
        call   sub_1_0A70
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0531:
        mov    eax, dword ptr [0x3570]       ; -> obj1:0x3570
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x25]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 0x16e]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_4_0577
        push   1
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   esi
        call   sub_1_0A70
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0577:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_0580:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x59], 1
        mov    byte ptr [eax + 0x8f], 0
        mov    dword ptr [eax + 0x93], 0
        mov    word ptr [eax + 0xcb], 2
        mov    byte ptr [eax + 0x54], 1
        mov    byte ptr [eax + 0x53], 1
        mov    byte ptr [eax + 0x50], 0
        mov    byte ptr [eax + 0x51], 0
        mov    dword ptr [eax + 0x97], 0x12345678
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_05D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x38], 7
        sub    esi, esi
        mov    word ptr [ebp - 0x36], 7
        mov    dword ptr [ebp - 0x34], 0x35dc ; -> loc_1_35DC
        sub    bl, bl
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 8], esi
        je     loc_4_0614
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0614:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 0x20], 6
        jne    loc_4_064E
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_064E
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, ecx
        lea    esi, [ebp - 0x30]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x30], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x2c], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x2b], al
loc_4_064E:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0674
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dx, word ptr [eax + 4]
        jmp    loc_4_0678
loc_4_0674:
        mov    dx, 0x300
loc_4_0678:
        mov    ecx, 0x200
        lea    ecx, [ecx]
loc_4_0680:
        cmp    ecx, 0x270
        je     loc_4_06BB
        cmp    ecx, 0x2f0
        je     loc_4_06BB
        cmp    ecx, 0x370
        je     loc_4_06BB
        cmp    ecx, 0x3b0
        je     loc_4_06BB
        cmp    ecx, 0x3c0
        je     loc_4_06BB
        cmp    ecx, 0x3d0
        je     loc_4_06BB
        mov    eax, edx
        and    eax, 0xffff
        cmp    eax, ecx
        je     loc_4_06C6
loc_4_06BB:
        add    ecx, 0x10
        cmp    ecx, 0x3f0
        jb     loc_4_0680
loc_4_06C6:
        mov    eax, edx
        and    eax, 0xffff
        cmp    eax, ecx
        je     loc_4_06E0
        mov    bl, 1
        mov    edi, 0xaa00000a
        mov    ecx, dword ptr [ebp - 0x24]
        mov    al, byte ptr [ebp - 2]
        jmp    loc_4_06FB
loc_4_06E0:
        mov    ecx, 5
        mov    dword ptr [ebp - 0x14], 0xd0000
        mov    dword ptr [ebp - 0x18], 0x8000
        mov    al, 1
        mov    byte ptr [ebp - 1], al
        mov    edi, dword ptr [ebp - 0x28]
loc_4_06FB:
        push   edi
        push   ebx
        push   0x20
        push   0x10
        push   esi
        mov    bl, byte ptr [ebp - 1]
        push   ebx
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [0x3784]       ; -> obj1:0x3784
        push   eax
        call   sub_4_00E0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_0740:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2B90
        push   1
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        lea    edi, [esi + 0x131]
        push   ebx
        push   0xe90                         ; -> loc_1_0E90
        push   esi
        push   0xde0                         ; -> loc_1_0DE0
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_07C3
        push   esi
        call   sub_1_2A60
        or     al, al
        je     loc_4_0798
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0798:
        push   0
        push   0
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   edi
        call   sub_1_04F4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_07C3:
        push   ebx
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        cmp    byte ptr [0x3768], 0          ; -> obj1:0x3768
        jne    loc_4_0820
        mov    dx, 0x100
        sub    al, al
        out    dx, al
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3290
        mov    dx, 0x100
        sub    al, al
        out    dx, al
        mov    byte ptr [0x3768], 1          ; -> obj1:0x3768
        jmp    loc_4_0823
loc_4_0820:
        mov    esi, dword ptr [ebp + 8]
loc_4_0823:
        mov    cx, word ptr [esi]
        sub    al, al
        lea    edx, [ecx + 6]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [esi]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [esi]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [esi]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [esi]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, word ptr [esi]
        add    dx, 5
        in     al, dx
        cmp    byte ptr [ebp - 7], 0x2a
        jne    loc_4_0A6D
        cmp    byte ptr [ebp - 6], 0x33
        jne    loc_4_0A6D
        cmp    byte ptr [ebp - 5], 0x43
        jne    loc_4_0A6D
        cmp    byte ptr [ebp - 4], 0x4f
        jne    loc_4_0A6D
        cmp    byte ptr [ebp - 3], 0x4d
        jne    loc_4_0A6D
        cmp    al, 0x2a
        jne    loc_4_0A6D
        mov    dx, word ptr [esi]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, al
        and    cl, 0x80
        cmp    cl, 1
        sbb    cl, cl
        neg    cl
        mov    byte ptr [esi + 0x56], cl
        mov    dx, word ptr [esi]
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, al
        and    ecx, 0xf
        mov    dword ptr [esi + 0x162], ecx
        mov    dx, word ptr [esi]
        add    dx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 0x20
        je     loc_4_0950
        lea    eax, [esi + 0xa]
        mov    dword ptr [eax], 0x40
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 0xf
        je     loc_4_0910
        cmp    ecx, 1
        je     loc_4_0920
        cmp    ecx, 2
        je     loc_4_0930
        cmp    ecx, 3
        je     loc_4_0940
        mov    dword ptr [esi + 6], 0xf80000
        jmp    loc_4_09D7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0910:
        mov    dword ptr [esi + 6], 0xf00000
        jmp    loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0920:
        mov    dword ptr [esi + 6], 0xf20000
        jmp    loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0930:
        mov    dword ptr [esi + 6], 0xf40000
        jmp    loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0940:
        mov    dword ptr [esi + 6], 0xf60000
        jmp    loc_4_09D7
        db  08Dh, 064h, 024h, 000h
loc_4_0950:
        mov    al, byte ptr [ebp - 1]
        and    eax, 3
        je     loc_4_0970
        cmp    eax, 1
        je     loc_4_0980
        cmp    eax, 2
        lea    eax, [esi + 0xa]
        je     loc_4_0990
        mov    dword ptr [eax], 0x40
        jmp    loc_4_0996
        db  08Dh, 049h, 000h
loc_4_0970:
        lea    eax, [esi + 0xa]
        mov    dword ptr [eax], 0x10
        jmp    loc_4_0996
        db  005h, 000h, 000h, 000h, 000h
loc_4_0980:
        lea    eax, [esi + 0xa]
        mov    dword ptr [eax], 0x20
        jmp    loc_4_0996
        db  005h, 000h, 000h, 000h, 000h
loc_4_0990:
        mov    dword ptr [eax], 0x30
loc_4_0996:
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 0x18
        je     loc_4_09B1
        cmp    ecx, 8
        je     loc_4_09C0
        cmp    ecx, 0x10
        je     loc_4_09D0
        mov    dword ptr [esi + 6], 0xd8000
        jmp    loc_4_09D7
loc_4_09B1:
        mov    dword ptr [esi + 6], 0xc0000
        jmp    loc_4_09D7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_09C0:
        mov    dword ptr [esi + 6], 0xc8000
        jmp    loc_4_09D7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_09D0:
        mov    dword ptr [esi + 6], 0xd0000
loc_4_09D7:
        mov    eax, dword ptr [eax]
        cmp    eax, 0x10
        je     loc_4_09F6
        cmp    eax, 0x20
        je     loc_4_0A13
        cmp    eax, 0x30
        je     loc_4_0A30
        cmp    eax, 0x40
        je     loc_4_0A50
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_09F6:
        mov    word ptr [esi + 0x1a], 0xc000
        mov    dword ptr [esi + 0x21], 2
        mov    dword ptr [esi + 0x25], 8
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0A13:
        mov    word ptr [esi + 0x1a], 0x8000
        mov    dword ptr [esi + 0x21], 5
        mov    dword ptr [esi + 0x25], 0x10
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0A30:
        mov    word ptr [esi + 0x1a], 0x4000
        mov    dword ptr [esi + 0x21], 7
        mov    dword ptr [esi + 0x25], 0x18
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_4_0A50:
        mov    word ptr [esi + 0x1a], 0
        mov    dword ptr [esi + 0x21], 0xa
        mov    dword ptr [esi + 0x25], 0x20
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_0A6D:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
VxD__LTEXT ENDS
