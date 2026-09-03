; ELNKMC -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 5839 instructions recovered by recursive descent

Declare_Virtual_Device ELNKMC, 3, 10, ELNKMC_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3734  R-X
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
ELNKMC_Control:
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
sub_1_057E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0584:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
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
        db  'ELNKMC  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  ELNKMC_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    eax, dword ptr [esi + 0x45]
        mov    dx, word ptr [esi + 0x1a]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    ax, word ptr [eax + ecx + 4]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    word ptr [ecx + 0xffc4], ax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0xffc2], 0x100
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0710:
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
        jne    loc_1_07D9
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
loc_1_07D9:
        mov    eax, dword ptr [edi + 0x16a]
        mov    edi, dword ptr [ebx + eax + 6]
        mov    ebx, dword ptr [ebp - 0x14]
        or     ebx, ebx
        je     loc_1_0827
loc_1_07EA:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_07FE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_05EC
loc_1_07FE:
        add    edi, dword ptr [ebp - 4]
        cmp    ebx, 1
        jbe    loc_1_0824
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
loc_1_0824:
        dec    ebx
        jne    loc_1_07EA
loc_1_0827:
        cmp    dword ptr [ebp - 0x10], 0x3c
        jae    loc_1_084C
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
loc_1_084C:
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
sub_1_0870:
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
        je     loc_1_08E0
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
loc_1_08E0:
        cmp    dword ptr [edi + 0x6d], 0xffff
        jne    loc_1_08EC
        mov    dword ptr [edi + 0x6d], esi
loc_1_08EC:
        sub    ebx, ebx
        mov    edi, edi
loc_1_08F0:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0x1a]
        mov    eax, dword ptr [edi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_0917
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_08F0
loc_1_0917:
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
sub_1_0940:
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
loc_1_0962:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_0989
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_0962
loc_1_0989:
        mov    eax, dword ptr [esi + 0x21]
        mov    dword ptr [esi + 0x45], eax
        push   esi
        push   0x6a0                         ; -> loc_1_06A0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        sub    ebx, ebx
loc_1_09A3:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        or     ax, ax
        je     loc_1_09CA
        push   0x32
        call   sub_1_0500
        inc    ebx
        cmp    ebx, 0x4e20
        jbe    loc_1_09A3
loc_1_09CA:
        sub    esi, esi
        mov    ebx, 0x8000
loc_1_09D1:
        mov    ax, word ptr [edi]
        test   bx, ax
        jne    loc_1_09E9
        push   0x32
        call   sub_1_0500
        inc    esi
        cmp    esi, 0x4e20
        jbe    loc_1_09D1
loc_1_09E9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 0x65], 0
        je     loc_1_0A30
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 0x69]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x69]
        inc    eax
        mov    dword ptr [edx + 0x69], eax
        cmp    dword ptr [edx + 0x21], eax
        ja     loc_1_0A27
        mov    dword ptr [edx + 0x69], 0
loc_1_0A27:
        dec    dword ptr [edx + 0x65]
        mov    al, 1
        pop    ebp
        ret    8
loc_1_0A30:
        mov    byte ptr [edx + 0x85], 0
        sub    al, al
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0A40:
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
        jne    loc_1_0A6C
        mov    dword ptr [edx + 0x71], 0xffff
loc_1_0A6C:
        mov    word ptr [ecx + 4], 0xffff
        mov    word ptr [ecx], 0
        inc    dword ptr [edx + 0x65]
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x16a]
        or     eax, eax
        je     loc_1_0A9B
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0A9B:
        mov    eax, dword ptr [esi + 0x16e]
        or     eax, eax
        je     loc_1_0AAF
        push   0
        push   0
        push   eax
        call   sub_1_050C
loc_1_0AAF:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AC0:
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
loc_1_0B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        sub    edx, edx
        inc    dword ptr [esi + 0x59]
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_1_0B6B
        mov    eax, dword ptr [ebp + 0x14]
loc_1_0B5A:
        cmp    dword ptr [eax], 0
        je     loc_1_0B67
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_1_0B5A
loc_1_0B67:
        cmp    ecx, edx
        jne    loc_1_0B75
loc_1_0B6B:
        mov    edi, 0xc0010019
        jmp    loc_1_0C2E
loc_1_0B75:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [0x35e0]       ; -> obj1:0x35E0
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x45
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_0BA0
        mov    edi, 0xc000009a
        jmp    loc_1_0C2E
        db  005h, 000h, 000h, 000h, 000h
loc_1_0BA0:
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
        call   sub_1_1D30
        mov    edi, 0x103
        cmp    byte ptr [esi + 0x54], 0
        je     loc_1_0C2E
        mov    byte ptr [esi + 0x54], 0
        push   0x7d0
        lea    eax, [esi + 0x149]
        push   eax
        call   sub_1_04E2
loc_1_0C2E:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_0C66
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_0C58
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_0C58
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_0C66
loc_1_0C58:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_0C66:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [edi + 0x18], 0
        jne    loc_1_0CB0
        lea    eax, [edi + 0x19]
        mov    dword ptr [edi + 0x29], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        push   eax
        push   esi
        call   sub_1_1D30
        dec    dword ptr [edi + 0x14]
        mov    edi, 0x103
        jmp    loc_1_0CB5
        db  005h, 000h, 000h, 000h, 000h
loc_1_0CB0:
        mov    edi, 0xc0010002
loc_1_0CB5:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_0CED
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_0CDF
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_0CDF
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_0CED
loc_1_0CDF:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_0CED:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x3730]       ; -> obj1:0x3730
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
        push   0
        mov    eax, dword ptr [0x371c]       ; -> obj1:0x371C
        push   eax
        call   sub_1_0420
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D30:
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
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0DA0:
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
loc_1_0DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
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
        je     loc_1_0DF8
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, 0x40
        out    dx, al
        in     al, dx
loc_1_0DF8:
        test   byte ptr [ebp - 4], 0x40
        je     loc_1_0E08
        cmp    byte ptr [esi + 0x4e], 0
        je     loc_1_0E08
        mov    byte ptr [esi + 0x4e], 0
loc_1_0E08:
        mov    byte ptr [esi + 0x58], 0
        mov    ax, word ptr [ebp - 4]
        and    ax, 0xf000
        je     loc_1_0E80
        cmp    byte ptr [esi + 0x53], 0
        je     loc_1_0E60
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1a]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        sub    bl, bl
        mov    word ptr [ecx + 0xffc2], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        push   1
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0E60:
        mov    al, byte ptr [esi + 0x1c]
        and    al, 0xfb
        mov    byte ptr [esi + 0x1c], al
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        mov    bl, 1
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0E80:
        mov    byte ptr [esi + 0x57], 1
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
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
        jne    loc_1_113A
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
        jne    loc_1_0F6F
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc0]
        mov    word ptr [ebp - 6], ax
        test   al, 0x40
        je     loc_1_0EFE
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0EFE
        mov    byte ptr [esi + 0x4e], bl
loc_1_0EFE:
        and    word ptr [ebp - 6], 0xf000
        je     loc_1_0F6F
        mov    dword ptr [ebp - 0xc], ebx
loc_1_0F09:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_0F33
        push   0x32
        call   sub_1_0500
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 0x4e20
        jbe    loc_1_0F09
loc_1_0F33:
        xor    edx, edx
        mov    dx, word ptr [esi + 0x1a]
        mov    ecx, dword ptr [esi + 2]
        sub    ecx, edx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 0xffc2], ax
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        push   1
        call   sub_1_0500
        or     di, word ptr [ebp - 6]
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
loc_1_0F6F:
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_0F91
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        jmp    loc_1_0F95
loc_1_0F91:
        mov    word ptr [ebp - 2], bx
loc_1_0F95:
        test   di, 0xe000
        jne    loc_1_0FEB
        cmp    word ptr [ebp - 2], bx
        je     loc_1_0FA8
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_0FEB
loc_1_0FA8:
        test   di, 0x1000
        je     loc_1_0FB4
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0FEB
loc_1_0FB4:
        cmp    dword ptr [esi + 0x75], ebx
        jne    loc_1_0FEB
        cmp    byte ptr [esi + 0x8f], bl
        je     loc_1_0FC7
        cmp    dword ptr [esi + 0x59], 1
        je     loc_1_0FEB
loc_1_0FC7:
        cmp    byte ptr [esi + 0x86], bl
        jne    loc_1_10B0
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_10B0
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_10B0
loc_1_0FEB:
        mov    eax, dword ptr [esi + 0x59]
        inc    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    byte ptr [esi + 0x8f], bl
        je     loc_1_1003
        cmp    eax, 2
        je     loc_1_10A7
loc_1_1003:
        test   di, 0x4000
        jne    loc_1_1016
        test   di, 0x1000
        je     loc_1_1025
        cmp    byte ptr [esi + 0x4e], bl
        jne    loc_1_1025
loc_1_1016:
        push   esi
        call   sub_1_1150
        cmp    al, bl
        je     loc_1_1025
        and    di, 0xafff
loc_1_1025:
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_1065
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        lea    eax, [eax + eax*4]
        mov    eax, dword ptr [ecx + eax]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        test   ah, 0x40
        jne    loc_1_1065
        cmp    ax, bx
        je     loc_1_1065
        push   esi
        call   sub_1_1320
        test   di, 0xa000
        je     loc_1_106A
        test   di, 0x1000
        jne    loc_1_1025
        jmp    loc_1_106A
loc_1_1065:
        and    di, 0x5fff
loc_1_106A:
        cmp    byte ptr [esi + 0x86], bl
        jne    loc_1_1088
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_1088
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_1088
        push   esi
        call   sub_1_34A0
loc_1_1088:
        push   esi
        call   sub_1_1750
        dec    dword ptr [esi + 0x59]
        test   di, 0x1000
        je     loc_1_0ED1
        cmp    byte ptr [esi + 0x4e], bl
        je     loc_1_0ED1
        jmp    loc_1_10B0
loc_1_10A7:
        push   esi
        call   sub_1_2A60
        dec    dword ptr [esi + 0x59]
loc_1_10B0:
        mov    edi, dword ptr [esi + 0xd9]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_110E
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_110E
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0444
        mov    eax, dword ptr [esi + 0x35]
        or     eax, eax
        je     loc_1_10EF
        cmp    dword ptr [eax + 0x10], 4
        jne    loc_1_10EF
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_10EF
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x35], eax
        push   esi
        call   sub_1_1D70
loc_1_10EF:
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
loc_1_110E:
        mov    byte ptr [esi + 0x50], 0
        push   esi
        push   0xda0                         ; -> loc_1_0DA0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        cmp    byte ptr [esi + 0x4d], 0
        je     loc_1_113A
        mov    byte ptr [esi + 0x4d], 0
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_1_0290
loc_1_113A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1150:
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
loc_1_1170:
        mov    byte ptr [esi + 0x91], 1
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 2], ax
        test   ax, 0xbfff
        je     loc_1_1272
        cmp    edi, 0xa
        ja     loc_1_12C0
        test   ah, 0x20
        jne    loc_1_11D5
        test   al, 0x80
        je     loc_1_11A2
        inc    dword ptr [esi + 0xc3]
        jmp    loc_1_11AE
loc_1_11A2:
        test   byte ptr [ebp - 2], 0x40
        je     loc_1_11AE
        inc    dword ptr [esi + 0xc7]
loc_1_11AE:
        push   ebx
        push   esi
        call   sub_1_12E0
        mov    eax, dword ptr [esi + 0x3d]
        cmp    dword ptr [esi + 0x25], eax
        jne    loc_1_11C4
        mov    dword ptr [esi + 0x3d], 0
loc_1_11C4:
        mov    ecx, dword ptr [esi + 0x3d]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 0x16e]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    loc_1_1170
loc_1_11D5:
        inc    dword ptr [esi + 0x9f]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_12CB
        mov    ax, word ptr [ebx + 0x16]
        mov    word ptr [ebp - 2], ax
        and    eax, 0x3fff
        lea    edx, [ebx + 8]
        mov    dword ptr [ebp - 8], edx
        or     edx, edx
        je     loc_1_1240
        or     eax, eax
        je     loc_1_1220
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0xc], edx
        or     edx, edx
        je     loc_1_1240
        or     eax, eax
        je     loc_1_1220
        cmp    eax, 0x5dc
        ja     loc_1_1240
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        jmp    loc_1_1226
loc_1_1220:
        push   0
        push   0
        push   0
loc_1_1226:
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
loc_1_1240:
        push   ebx
        push   esi
        call   sub_1_12E0
        mov    byte ptr [esi + 0x4d], 1
        mov    eax, dword ptr [esi + 0x3d]
        inc    eax
        mov    dword ptr [esi + 0x3d], eax
        cmp    dword ptr [esi + 0x25], eax
        jne    loc_1_125E
        mov    dword ptr [esi + 0x3d], 0
loc_1_125E:
        mov    ecx, dword ptr [esi + 0x3d]
        shl    ecx, 4
        add    ecx, dword ptr [esi + 0x16e]
        inc    edi
        mov    ebx, dword ptr [ecx]
        jmp    loc_1_1170
loc_1_1272:
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
        jne    loc_1_12A0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_12A0:
        mov    eax, dword ptr [esi + 0x3d]
        shl    eax, 4
        add    eax, dword ptr [esi + 0x16e]
        push   eax
        push   esi
        call   sub_1_2B60
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12C0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_12CB:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12E0:
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
        jne    loc_1_131C
        mov    dword ptr [edx + 0x41], 0
loc_1_131C:
        pop    ebp
        ret    8
sub_1_1320:
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
        jne    loc_1_1372
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
loc_1_1372:
        mov    byte ptr [esi + 0x90], 1
        cmp    dword ptr [ebp - 8], 4
        je     loc_1_13C0
        mov    edi, dword ptr [esi + 0x6d]
        push   esi
        call   sub_1_14E0
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_1540
        lea    eax, [edi + edi*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    eax, dword ptr [eax + ecx + 0xc]
        mov    dword ptr [esi + 0x6d], eax
        cmp    eax, 0xffff
        jne    loc_1_14CF
        mov    dword ptr [esi + 0x71], 0xffff
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_13C0:
        mov    edx, dword ptr [esi + 0x6d]
        test   byte ptr [ebp - 1], 0x20
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    edx, dword ptr [edx + eax + 0x10]
        lea    edi, [edx + 0x24]
        je     loc_1_1410
        inc    dword ptr [esi + 0x9b]
        test   byte ptr [ebp - 2], 0x80
        je     loc_1_13F0
        inc    dword ptr [esi + 0xb7]
        jmp    loc_1_144E
loc_1_13F0:
        mov    ax, word ptr [ebp - 2]
        and    ax, 0xf
        cmp    ax, 1
        jne    loc_1_1406
        inc    dword ptr [esi + 0xbb]
        jmp    loc_1_144E
loc_1_1406:
        jbe    loc_1_144E
        inc    dword ptr [esi + 0xbf]
        jmp    loc_1_144E
loc_1_1410:
        test   byte ptr [ebp - 2], 0x20
        je     loc_1_1420
        inc    dword ptr [esi + 0xa7]
        jmp    loc_1_144C
        db  08Bh, 0FFh
loc_1_1420:
        test   byte ptr [ebp - 1], 4
        je     loc_1_1430
        inc    dword ptr [esi + 0xab]
        jmp    loc_1_144C
        db  08Bh, 0FFh
loc_1_1430:
        test   byte ptr [ebp - 1], 2
        je     loc_1_1440
        inc    dword ptr [esi + 0xb3]
        jmp    loc_1_144C
        db  08Bh, 0FFh
loc_1_1440:
        test   byte ptr [ebp - 1], 1
        je     loc_1_144C
        inc    dword ptr [esi + 0xaf]
loc_1_144C:
        sub    cl, cl
loc_1_144E:
        cmp    dword ptr [edi], 0
        jne    loc_1_145D
        mov    dword ptr [esi + 0x81], 0
loc_1_145D:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x7d], eax
        cmp    byte ptr [edi + 0xd], 0
        jne    loc_1_1485
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
        jmp    loc_1_14A2
loc_1_1485:
        mov    byte ptr [edi + 0xc], cl
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_1493
        mov    dword ptr [esi + 0x75], edx
        jmp    loc_1_1499
loc_1_1493:
        mov    eax, dword ptr [esi + 0x79]
        mov    dword ptr [eax + 0x24], edx
loc_1_1499:
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x79], edx
loc_1_14A2:
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_1540
        mov    eax, dword ptr [esi + 0x6d]
        push   eax
        push   esi
        call   sub_1_0A40
        cmp    byte ptr [esi + 0x85], 0
        jne    loc_1_14CF
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_14CF
        mov    byte ptr [esi + 0x85], 1
loc_1_14CF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14E0:
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
        jne    loc_1_1530
        mov    eax, dword ptr [esi + 0x35]
        or     eax, eax
        je     loc_1_1538
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
        call   sub_1_1D70
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1530:
        cmp    eax, 1
        je     loc_1_1538
        dec    dword ptr [eax + 0x14]
loc_1_1538:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1540:
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
        je     loc_1_15AB
        mov    byte ptr [ecx + 0x18], 0
        mov    dword ptr [esi + 0x45], eax
        sub    edi, edi
        sub    ebx, ebx
loc_1_1572:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_1599
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_1572
loc_1_1599:
        push   esi
        push   0x6a0                         ; -> loc_1_06A0
        add    esi, 0x131
        push   esi
        call   sub_1_04FA
loc_1_15AB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15C0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    al, al
        cmp    byte ptr [ecx + 0x50], al
        jne    loc_1_1602
        cmp    byte ptr [ecx + 0x8f], al
        jne    loc_1_1602
        cmp    byte ptr [ecx + 0x90], al
        jne    loc_1_1602
        cmp    byte ptr [ecx + 0x91], al
        jne    loc_1_1602
        mov    al, byte ptr [ecx + 0x92]
        cmp    al, 0xa
        lea    edx, [eax + 1]
        mov    byte ptr [ecx + 0x92], dl
        jae    loc_1_1600
        sub    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1600:
        mov    al, 1
loc_1_1602:
        mov    byte ptr [ecx + 0x92], 0
        mov    byte ptr [ecx + 0x90], 0
        mov    byte ptr [ecx + 0x91], 0
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x6d]
        cmp    eax, 0xffff
        je     loc_1_16DF
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [esi + 0x16a]
        cmp    byte ptr [eax + 0x18], 0
        je     loc_1_16DB
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [esi + 0x6d]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x16a]
        mov    eax, dword ptr [eax + ecx]
        mov    ax, word ptr [eax]
        test   ah, 0x40
        jne    loc_1_1690
        or     ax, ax
        je     loc_1_1690
        cmp    byte ptr [esi + 0x58], 0
        jne    loc_1_1690
        mov    byte ptr [esi + 0x57], 1
        mov    byte ptr [esi + 0x58], 1
        push   1
        lea    eax, [esi + 0x155]
        push   eax
        call   sub_1_04E2
        jmp    loc_1_16DF
loc_1_1690:
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_16DF
        mov    byte ptr [esi + 0x53], 1
        push   0
        push   esi
        call   sub_1_2F40
        cmp    byte ptr [esi + 0x50], 0
        jne    loc_1_16DF
        mov    al, byte ptr [esi + 0x1c]
        and    al, 0xfb
        mov    byte ptr [esi + 0x1c], al
        inc    dword ptr [esi + 0x59]
        mov    dx, word ptr [esi]
        add    dx, 6
        out    dx, al
        push   esi
        call   sub_1_2A60
        dec    dword ptr [esi + 0x59]
        push   esi
        push   0xda0                         ; -> loc_1_0DA0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        jmp    loc_1_16DF
loc_1_16DB:
        mov    byte ptr [eax + 0x18], 1
loc_1_16DF:
        push   esi
        call   sub_1_15C0
        or     al, al
        je     loc_1_1729
        mov    byte ptr [esi + 0x53], 1
        push   0
        push   esi
        call   sub_1_2F40
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
        call   sub_1_2A60
        dec    dword ptr [esi + 0x59]
        push   esi
        push   0xda0                         ; -> loc_1_0DA0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
loc_1_1729:
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
sub_1_1750:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 0x75]
        or     esi, esi
        je     loc_1_183A
        lea    edi, [esi + 0x24]
        cmp    dword ptr [edi], 0
        jne    loc_1_1776
        mov    dword ptr [ebx + 0x79], 0
loc_1_1776:
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
        jae    loc_1_17DA
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_17DA
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 0x172]
        push   eax
        push   0x100
        push   0
        push   esi
        call   sub_1_1890
        lea    eax, [ebx + 0x172]
        mov    dword ptr [ebp - 8], eax
loc_1_17DA:
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_17F2
        cmp    dword ptr [ebp - 4], 5
        jbe    loc_1_181B
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    loc_1_1809
loc_1_17F2:
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
loc_1_1809:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0x166]
        push   eax
        call   sub_1_00B0
loc_1_181B:
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
loc_1_183A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1850:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [ecx + 0x24]
        mov    edx, dword ptr [eax + 0x81]
        or     edx, edx
        je     loc_1_186A
        mov    dword ptr [edx + 0x24], ecx
loc_1_186A:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0x81], ecx
        cmp    dword ptr [eax + 0x7d], 0
        jne    loc_1_187F
        mov    dword ptr [eax + 0x7d], ecx
loc_1_187F:
        dec    dword ptr [eax + 0xa3]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1890:
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
        je     loc_1_195A
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
        je     loc_1_195A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_1955
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_18E0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_190C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1955
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    loc_1_1950
loc_1_190C:
        or     edi, edi
        je     loc_1_1929
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_1921
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1950
loc_1_1921:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1929:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_1937
        mov    ebx, eax
loc_1_1937:
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
loc_1_1950:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_18E0
loc_1_1955:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_195A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
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
        je     loc_1_1A3B
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
        je     loc_1_1A3B
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
        je     loc_1_1A36
        mov    edi, dword ptr [ebp + 0x14]
loc_1_19C4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_19F0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1A36
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        jmp    loc_1_1A31
loc_1_19F0:
        or     edi, edi
        je     loc_1_1A0D
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_1A05
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1A31
loc_1_1A05:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1A0D:
        mov    ebx, dword ptr [ebp + 0xc]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_1A1B
        mov    ebx, eax
loc_1_1A1B:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    esi, ebx
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1A31:
        cmp    dword ptr [ebp + 0xc], esi
        ja     loc_1_19C4
loc_1_1A36:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_1A3B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x8f], 0
        je     loc_1_1A6B
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_1A6B:
        mov    cl, byte ptr [ebp + 0x18]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_1_1AA0
        or     al, al
        je     loc_1_1A8B
        mov    eax, 0x103
        pop    ebp
        ret    0x14
loc_1_1A8B:
        sub    eax, eax
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    al, al
        mov    dx, 2
        mov    ecx, dword ptr [ebp + 0x10]
        test   cl, 0x24
        je     loc_1_1AB8
        mov    dx, 3
        jmp    loc_1_1AC0
loc_1_1AB8:
        test   cl, 8
        je     loc_1_1AC0
        sub    dx, dx
loc_1_1AC0:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 0xcb], dx
        je     loc_1_1B66
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
        jne    loc_1_1B43
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
loc_1_1B43:
        mov    edx, dword ptr [ecx + 0x21]
        mov    eax, dword ptr [ecx + 0x16a]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 0x14], 0
        mov    eax, dword ptr [ecx + 0x21]
        push   eax
        push   ecx
        call   sub_1_0870
        mov    al, 1
loc_1_1B66:
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_1B70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [eax + 8]
        cmp    byte ptr [edx + 0x8f], 0
        je     loc_1_1B8B
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_1B8B:
        mov    cl, byte ptr [ebp + 0x20]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edx
        call   sub_1_1BB0
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        mov    ecx, dword ptr [ebp + 8]
        je     loc_1_1BEB
        sub    edx, edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
loc_1_1BCA:
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, edx
loc_1_1BD1:
        mov    bl, byte ptr [esi + eax]
        mov    edi, dword ptr [ecx + 0x1d]
        add    edi, edx
        mov    byte ptr [esi + edi + 8], bl
        inc    esi
        cmp    esi, 6
        jb     loc_1_1BD1
        add    edx, 6
        dec    dword ptr [ebp - 4]
        jne    loc_1_1BCA
loc_1_1BEB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x1d]
        imul   ax, ax, 6
        mov    word ptr [edx + 6], ax
        mov    eax, dword ptr [ecx + 0x1d]
        cmp    byte ptr [ebp + 0x18], 0
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ecx + 0x1d]
        mov    word ptr [eax + 2], 3
        jne    loc_1_1C33
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
loc_1_1C33:
        mov    edx, dword ptr [ecx + 0x21]
        mov    eax, dword ptr [ecx + 0x16a]
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        mov    dword ptr [eax + edx + 0x14], 0
        cmp    byte ptr [ecx + 0x53], 0
        je     loc_1_1C60
        push   ecx
        call   sub_1_0940
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_1_1C60:
        mov    eax, dword ptr [ecx + 0x21]
        push   eax
        push   ecx
        call   sub_1_0870
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x14]
        ret    4
        db  08Dh, 049h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1CE7
        cmp    byte ptr [edx + 0x18], 0
        jne    loc_1_1CE0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        jl     loc_1_1CBC
        cmp    ecx, 1
        jle    loc_1_1CC3
loc_1_1CBC:
        mov    edi, 0xc00000bb
        jmp    loc_1_1CEC
loc_1_1CC3:
        inc    dword ptr [edx + 0x14]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 0
        push   eax
        mov    edi, 0x103
        push   esi
        call   sub_1_1D30
        jmp    loc_1_1CEC
        db  08Dh, 049h, 000h
loc_1_1CE0:
        mov    edi, 0xc0010002
        jmp    loc_1_1CEC
loc_1_1CE7:
        mov    edi, 0xc001000d
loc_1_1CEC:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_1D24
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1D16
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_1D16
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_1D24
loc_1_1D16:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_1D24:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_1D30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x35], 0
        je     loc_1_1D50
        mov    edx, dword ptr [eax + 0x39]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0x39], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D50:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x35], ecx
        mov    dword ptr [eax + 0x39], ecx
        push   eax
        call   sub_1_1D70
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x51], 0
        jne    loc_1_1F90
        mov    byte ptr [esi + 0x51], 1
        mov    edi, dword ptr [esi + 0x35]
        or     edi, edi
        je     loc_1_1F8C
loc_1_1D95:
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_1F8C
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_1DC6
        cmp    eax, 1
        je     loc_1_1DEC
        cmp    eax, 2
        je     loc_1_1E12
        cmp    eax, 3
        je     loc_1_1E36
        cmp    eax, 4
        je     loc_1_1EB2
        jmp    loc_1_1F35
loc_1_1DC6:
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
        call   sub_1_2120
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F35
loc_1_1DEC:
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
        call   sub_1_1FB0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F35
loc_1_1E12:
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
        call   sub_1_2120
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1F35
loc_1_1E36:
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
        jne    loc_1_1E71
        push   0
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_043E
        push   0
        push   0
        push   ebx
        call   sub_1_050C
        jmp    loc_1_1EA6
loc_1_1E71:
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
loc_1_1EA6:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1F35
loc_1_1EB2:
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
        jne    loc_1_1ED7
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_1EF0
loc_1_1ED7:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_1EF0
        cmp    dword ptr [ebp - 4], 0xc001000e
        jne    loc_1_1EF0
        mov    dword ptr [ebp - 4], 0x103
loc_1_1EF0:
        cmp    byte ptr [esi + 0x52], 0
        je     loc_1_1F00
        mov    eax, dword ptr [esi + 0x21]
        push   eax
        push   esi
        call   sub_1_0870
loc_1_1F00:
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
loc_1_1F35:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_1_1F99
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x35], eax
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 2
        jne    loc_1_1F64
        inc    dword ptr [esi + 0x59]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_047A
        dec    dword ptr [esi + 0x59]
        jmp    loc_1_1F81
loc_1_1F64:
        or     eax, eax
        je     loc_1_1F6D
        cmp    eax, 1
        jne    loc_1_1F81
loc_1_1F6D:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 0x14]
loc_1_1F81:
        mov    edi, dword ptr [esi + 0x35]
        or     edi, edi
        jne    loc_1_1D95
loc_1_1F8C:
        mov    byte ptr [esi + 0x51], 0
loc_1_1F90:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1F99:
        mov    byte ptr [esi + 0x51], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010e
        mov    dword ptr [edi], 0
        je     loc_1_1FF9
        cmp    eax, 0x1010f
        je     loc_1_2073
        cmp    eax, 0x10112
        je     loc_1_2086
        cmp    eax, 0x1010103
        je     loc_1_20C5
        mov    eax, 0xc0010017
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_1FF9:
        mov    esi, dword ptr [ebp + 0x18]
        cmp    esi, 4
        je     loc_1_2014
        mov    dword ptr [edi], 4
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2014:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 4], 0xf050
        je     loc_1_2048
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc00000bb
        mov    dword ptr [ecx], 4
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2048:
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
loc_1_2073:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_2086:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_1_20A0
        mov    dword ptr [edi], 4
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_1_20A0:
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
loc_1_20C5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        cdq    
        idiv   ecx
        or     edx, edx
        je     loc_1_20E5
        mov    dword ptr [edi], ecx
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_20E5:
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
sub_1_2120:
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
        ja     loc_1_2180
        je     loc_1_2573
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_21A1
        shl    eax, 2
        jmp    loc_1_269C
        db  08Dh, 049h, 000h
loc_1_2180:
        cmp    eax, 0x1010102
        je     loc_1_2598
        cmp    eax, 0x1010103
        je     loc_1_25C0
        cmp    eax, 0x1010104
        je     loc_1_2650
loc_1_21A1:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_21B3
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2657
loc_1_21B3:
        cmp    edx, 0x1020102
        ja     loc_1_2230
        je     loc_1_2384
        cmp    edx, 0x20105
        ja     loc_1_2204
        je     loc_1_2334
        cmp    edx, 0x20101
        je     loc_1_2294
        cmp    edx, 0x20102
        je     loc_1_22A5
        cmp    edx, 0x20103
        je     loc_1_22B6
        cmp    edx, 0x20104
        je     loc_1_22D9
        jmp    loc_1_2288
loc_1_2204:
        cmp    edx, 0x2020d
        je     loc_1_2347
        cmp    edx, 0x2020e
        je     loc_1_2360
        cmp    edx, 0x1020101
        je     loc_1_2371
        jmp    loc_1_2288
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2230:
        cmp    edx, 0x1020203
        ja     loc_1_2264
        je     loc_1_23C8
        cmp    edx, 0x1020103
        je     loc_1_2395
        cmp    edx, 0x1020201
        je     loc_1_23A6
        cmp    edx, 0x1020202
        je     loc_1_23B7
        jmp    loc_1_2288
loc_1_2264:
        cmp    edx, 0x1020204
        je     loc_1_23E0
        cmp    edx, 0x1020205
        je     loc_1_23F1
        cmp    edx, 0x1020206
        je     loc_1_2402
loc_1_2288:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2657
loc_1_2294:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x9b]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_22A5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x9f]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_22B6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xaf]
        add    eax, dword ptr [edi + 0xb3]
        add    eax, dword ptr [edi + 0xa7]
        add    eax, dword ptr [edi + 0xab]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_22D9:
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
        jmp    loc_1_2657
loc_1_2334:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2657
loc_1_2347:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2657
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2360:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa3]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_2371:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2657
loc_1_2384:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xbb]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_2395:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xbf]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_23A6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xb7]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_23B7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xa7]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_23C8:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x16]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2657
        db  005h, 000h, 000h, 000h, 000h
loc_1_23E0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xaf]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_23F1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xb3]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_2402:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xab]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_2413:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_2428
        mov    esi, 0x360c                   ; -> loc_1_360C
        mov    ecx, 0xa0
        jmp    loc_1_2657
loc_1_2428:
        mov    esi, 0x36ac                   ; -> obj1:0x36AC
        mov    ecx, 0x5c
        jmp    loc_1_2657
loc_1_2437:
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x8f], 0
        je     loc_1_2450
        mov    dword ptr [ebp - 0x10], 2
        jmp    loc_1_2464
        db  08Dh, 064h, 024h, 000h
loc_1_2450:
        cmp    byte ptr [edi + 0x53], 0
        mov    dword ptr [ebp - 0x10], 1
        jne    loc_1_2464
        mov    dword ptr [ebp - 0x10], 0
loc_1_2464:
        lea    esi, [ebp - 0x10]
        mov    ecx, 4
        jmp    loc_1_2657
loc_1_2471:
        lea    esi, [ebp - 0x1c]
        mov    ecx, 4
        jmp    loc_1_2657
        db  08Bh, 0FFh
loc_1_2480:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_2490:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_24A0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x21]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
        db  08Bh, 0FFh
loc_1_24C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x25]
        mov    edx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [edx + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
        db  08Bh, 0FFh
loc_1_24E0:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2657
        db  08Dh, 064h, 024h, 000h
loc_1_24F0:
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
        jmp    loc_1_2657
loc_1_2516:
        mov    esi, 0x3708                   ; -> loc_1_3708
        mov    ecx, 0xf
        jmp    loc_1_2657
loc_1_2525:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        je     loc_1_2540
        mov    eax, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2540:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x166]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2657
loc_1_2554:
        mov    word ptr [ebp - 4], 0x300
        lea    esi, [ebp - 4]
        mov    ecx, 2
        jmp    loc_1_2657
loc_1_2567:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_2657
loc_1_2573:
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
        jmp    loc_1_2657
loc_1_2598:
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
        jmp    loc_1_2657
        db  08Dh, 049h, 000h
loc_1_25C0:
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        jne    loc_1_25F8
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
        jmp    loc_1_2657
loc_1_25F8:
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
        jne    loc_1_2633
        mov    eax, dword ptr [ebp - 0x14]
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_2657
loc_1_2633:
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x166]
        push   eax
        call   sub_2_0660
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        jmp    loc_1_2657
        db  090h
loc_1_2650:
        mov    dword ptr [ebp - 8], 0x10
loc_1_2657:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_268E
        cmp    dword ptr [ebp + 0x18], ecx
        jae    loc_1_267A
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
loc_1_267A:
        mov    eax, dword ptr [ebp + 0x1c]
        or     ecx, ecx
        mov    dword ptr [eax], ecx
        je     loc_1_268E
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_05EC
loc_1_268E:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_269C:
        adc    esp, dword ptr [eax + eax]
        add    byte ptr [edi], dh            ; -> loc_1_2437
        and    al, 0
        add    byte ptr [ecx + 0x24], dh     ; -> loc_1_2471
        add    byte ptr [eax], al
        jno    0x26ce
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        and    byte ptr [eax + eax], 0
        nop    
        and    al, 0
        add    byte ptr [eax - 0x3fffffdc], ah ; -> loc_1_24A0
        and    al, 0
        add    al, ah                        ; -> loc_1_24E0
        and    al, 0
        add    al, ah                        ; -> loc_1_24E0
        and    al, 0
        add    al, dh                        ; -> loc_1_24F0
        and    al, 0
        add    byte ptr [esi], dl            ; -> loc_1_2516
        and    eax, 0x25250000               ; -> loc_1_2525
        add    byte ptr [eax], al
        and    byte ptr [eax + eax], 0
        push   esp
        and    eax, 0x24e00000               ; -> loc_1_24E0
        add    byte ptr [eax], al
        mov    eax, dword ptr [0x67000021]   ; -> loc_1_2567
        and    eax, 0x648d0000
        and    al, 0
        lea    esp, [esp]
loc_1_26F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_2730
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 2
        je     loc_1_2714
        mov    edi, 0xc00000bb
        jmp    loc_1_2735
loc_1_2714:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        push   eax
        push   esi
        call   sub_1_1D30
        mov    edi, 0x103
        jmp    loc_1_2735
        db  090h
loc_1_2730:
        mov    edi, 0xc001000d
loc_1_2735:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_276D
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_275F
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_275F
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_276D
loc_1_275F:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_276D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2780:
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
        push   1
        mov    word ptr [eax + 0xffc2], 0x10
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    byte ptr [esi + 0x4e], 1
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_27E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0xffc2], 0x440
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        push   1
        mov    word ptr [eax + 0xffc2], 0x80
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2880:
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
        jne    loc_1_28F8
        cmp    byte ptr [ebx + 0x18], 0
        jne    loc_1_28B2
        inc    dword ptr [ebx + 0x14]
        push   ebx
        push   edi
        call   sub_1_2F40
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_28B7
loc_1_28B2:
        mov    esi, 0xc0010002
loc_1_28B7:
        mov    eax, dword ptr [edi + 0x59]
        dec    eax
        mov    dword ptr [edi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_28EF
        cmp    byte ptr [edi + 0x8f], 0
        jne    loc_1_28E1
        cmp    dword ptr [edi + 0x75], 0
        jne    loc_1_28E1
        mov    eax, dword ptr [edi + 0xd9]
        sub    eax, edi
        cmp    eax, 0xd9
        je     loc_1_28EF
loc_1_28E1:
        push   0
        add    edi, 0x155
        push   edi
        call   sub_1_04E2
loc_1_28EF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_28F8:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2910:
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
        je     loc_1_2950
        mov    edi, dword ptr [eax + 0xc]
loc_1_2950:
        test   edi, 0x20
        je     loc_1_2963
        mov    word ptr [esi + 0xcb], 3
        jmp    loc_1_2987
loc_1_2963:
        test   edi, 4
        je     loc_1_2976
        mov    word ptr [esi + 0xcb], 3
        jmp    loc_1_2987
loc_1_2976:
        test   edi, 8
        je     loc_1_2987
        mov    word ptr [esi + 0xcb], 0
loc_1_2987:
        cmp    byte ptr [esi + 0x56], 0
        jne    loc_1_2996
        or     word ptr [esi + 0xcb], 0x8800
loc_1_2996:
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
        call   sub_1_0940
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 4], 0xffff
        sub    eax, eax
        lea    ebx, [ebx]
loc_1_2A00:
        mov    cl, byte ptr [esi + eax + 0x2f]
        mov    edx, dword ptr [esi + 0x1d]
        mov    byte ptr [eax + edx + 6], cl
        inc    eax
        cmp    eax, 6
        jb     loc_1_2A00
        push   esi
        call   sub_1_0940
        test   edi, 2
        je     loc_1_2A51
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
        jne    loc_1_2A51
        push   1
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_1BB0
loc_1_2A51:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
loc_1_2A71:
        or     edi, edi
        je     loc_1_2A81
        cmp    edi, 1
        je     loc_1_2A86
        cmp    edi, 2
        je     loc_1_2A90
        jmp    loc_1_2A96
loc_1_2A81:
        mov    ebx, dword ptr [esi + 0x75]
        jmp    loc_1_2A96
loc_1_2A86:
        mov    ebx, dword ptr [esi + 0x7d]
        jmp    loc_1_2A96
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A90:
        mov    ebx, dword ptr [esi + 0x87]
loc_1_2A96:
        or     ebx, ebx
        je     loc_1_2ABA
loc_1_2A9A:
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
        jne    loc_1_2A9A
loc_1_2ABA:
        inc    edi
        cmp    edi, 3
        jb     loc_1_2A71
        push   esi
        call   sub_1_2AD0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_2AD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0AC0
        push   esi
        call   sub_1_2C40
        mov    bl, al
        or     bl, bl
        je     loc_1_2B42
        push   esi
        call   sub_1_3040
        push   esi
        call   sub_1_2910
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
        call   sub_1_2DF0
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_1_2B35
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
loc_1_2B35:
        mov    byte ptr [esi + 0x53], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2B42:
        push   0xc0000001
        push   esi
        call   sub_1_2DF0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x4f], 0
        je     loc_1_2BE9
        mov    eax, dword ptr [ebp + 0xc]
        sub    edi, edi
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x49], eax
        sub    ebx, ebx
loc_1_2B85:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_2BAC
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_2B85
loc_1_2BAC:
        push   esi
        push   0x2780                        ; -> loc_1_2780
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
        sub    edi, edi
        sub    ebx, ebx
loc_1_2BC2:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        mov    ax, word ptr [eax + 0xffc2]
        cmp    ax, bx
        je     loc_1_2BE9
        push   0x32
        call   sub_1_0500
        inc    edi
        cmp    edi, 0x4e20
        jbe    loc_1_2BC2
loc_1_2BE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_1_2C22
        push   esi
        push   0x27e0                        ; -> loc_1_27E0
        lea    eax, [esi + 0x131]
        push   eax
        call   sub_1_04FA
loc_1_2C22:
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
sub_1_2C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    dx, 6
        in     al, dx
        test   al, 8
        je     loc_1_2C61
        mov    dx, word ptr [esi]
        add    dx, 7
        mov    al, 0x40
        out    dx, al
        in     al, dx
loc_1_2C61:
        push   esi
        call   sub_1_2FE0
        cmp    byte ptr [esi + 0x4f], 0
        jne    loc_1_2CB9
        mov    eax, dword ptr [esi + 0xa]
        shl    eax, 0xa
        push   eax
        mov    eax, dword ptr [esi + 6]
        push   eax
        mov    eax, dword ptr [esi + 0xf1]
        lea    ecx, [esi + 2]
        push   eax
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2CB5
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
loc_1_2CB5:
        mov    byte ptr [esi + 0x4f], 1
loc_1_2CB9:
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
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x1a]
        mov    eax, dword ptr [esi + 2]
        sub    eax, ecx
        add    eax, 0xffc0
        mov    dword ptr [esi + 0x16], eax
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
        mov    dword ptr [esi + 0x5d], ecx
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, 0x83
        out    dx, al
        push   0x3e8
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, 7
        out    dx, al
        mov    eax, dword ptr [esi + 0xe]
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
        push   1
        mov    word ptr [eax + 0xe], 0
        mov    dx, word ptr [esi]
        add    dx, 6
        mov    al, byte ptr [esi + 0x1c]
        or     al, 0x40
        out    dx, al
        call   sub_1_0500
        mov    cx, word ptr [esi]
        add    cx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 0x1c]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2DF0:
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
        je     loc_1_2E58
loc_1_2E10:
        inc    dword ptr [ebx + 0x14]
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_2E2B
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_046E
loc_1_2E2B:
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
        jne    loc_1_2E10
loc_1_2E58:
        or     edi, edi
        je     loc_1_2E6C
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0456
        dec    dword ptr [edi + 0x14]
loc_1_2E6C:
        mov    byte ptr [esi + 0x8f], 0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_2E99
        mov    eax, dword ptr [esi + 0x3d]
        shl    eax, 4
        add    eax, dword ptr [esi + 0x16e]
        push   eax
        push   esi
        call   sub_1_2B60
        push   esi
        call   sub_1_1D70
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2E99:
        push   1
        push   esi
        call   sub_1_2EB0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2EB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x35]
        add    esi, 0x35
        or     edi, edi
        je     loc_1_2F36
        cmp    dword ptr [edi + 0x10], 4
        jne    loc_1_2ED5
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 0x14]
        mov    edi, dword ptr [esi]
loc_1_2ED5:
        or     edi, edi
        je     loc_1_2F36
loc_1_2ED9:
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 4
        jne    loc_1_2EE5
        mov    esi, edi
        jmp    loc_1_2F30
loc_1_2EE5:
        cmp    eax, 3
        jne    loc_1_2F17
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_2F13
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
        jmp    loc_1_2F30
loc_1_2F13:
        mov    esi, edi
        jmp    loc_1_2F30
loc_1_2F17:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    ebx, dword ptr [edi + 4]
        push   0xc001000c
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_045C
        dec    dword ptr [ebx + 0x14]
loc_1_2F30:
        mov    edi, dword ptr [esi]
        or     edi, edi
        jne    loc_1_2ED9
loc_1_2F36:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_2F40:
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
        call   sub_1_2C00
        mov    byte ptr [esi + 0x1c], 0x87
        mov    edi, dword ptr [esi + 0xd1]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff2f
        je     loc_1_2FA4
loc_1_2F76:
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
        jne    loc_1_2F76
loc_1_2FA4:
        mov    byte ptr [esi + 0x8f], 1
        mov    byte ptr [esi + 0x85], 0
        push   0
        push   esi
        call   sub_1_2EB0
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x93], eax
        or     eax, eax
        je     loc_1_2FCA
        inc    dword ptr [eax + 0x14]
loc_1_2FCA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        in     al, dx
        mov    byte ptr [esi + 0x29], al
        inc    dx
        in     al, dx
        mov    byte ptr [esi + 0x2a], al
        mov    dx, word ptr [esi]
        add    dx, 2
        in     al, dx
        mov    byte ptr [esi + 0x2b], al
        mov    dx, word ptr [esi]
        add    dx, 3
        in     al, dx
        mov    byte ptr [esi + 0x2c], al
        mov    dx, word ptr [esi]
        add    dx, 4
        in     al, dx
        mov    byte ptr [esi + 0x2d], al
        mov    dx, word ptr [esi]
        add    dx, 5
        in     al, dx
        mov    byte ptr [esi + 0x2e], al
        cmp    byte ptr [esi + 0x55], 0
        jne    loc_1_303B
        mov    ecx, dword ptr [esi + 0x29]
        mov    dword ptr [esi + 0x2f], ecx
        mov    cl, byte ptr [esi + 0x2d]
        mov    byte ptr [esi + 0x33], cl
        lea    eax, [esi + 0x2f]
        mov    cl, byte ptr [esi + 0x2e]
        mov    byte ptr [eax + 5], cl
loc_1_303B:
        pop    esi
        pop    ebp
        ret    4
sub_1_3040:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [esi + 0x61]
        cmp    dword ptr [esi + 0x21], 0
        jbe    loc_1_3123
        sub    edx, edx
        sub    eax, eax
loc_1_3064:
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
        inc    dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + edx - 0xf]
        and    ecx, 0xffff
        mov    dword ptr [ebx + 0x14], ecx
        mov    ecx, dword ptr [esi + 0x16a]
        mov    ebx, dword ptr [edx + ecx - 0x13]
        add    ebx, 0x5f0
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x21], ecx
        ja     loc_1_3064
loc_1_3123:
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
        mov    dword ptr [ebp - 8], 0
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    eax, dword ptr [esi + 0x16a]
        mov    word ptr [ecx + eax + 0xa], 0xffff
        mov    eax, dword ptr [esi + 0x1d]
        mov    word ptr [eax + 4], 0xffff
        mov    ebx, dword ptr [esi + 0x5d]
        cmp    dword ptr [esi + 0x25], 0
        jbe    loc_1_32B2
        mov    dword ptr [ebp - 4], 0
loc_1_31F0:
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
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [eax + edi], ebx
        mov    cx, word ptr [esi + 0x1a]
        sub    cx, word ptr [esi + 2]
        add    cx, bx
        mov    eax, dword ptr [esi + 0x16e]
        mov    word ptr [edi + eax + 4], cx
        lea    ecx, [ebx + 0x16]
        sub    cx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 0x16e]
        mov    eax, dword ptr [edi + eax]
        sub    edx, edx
        add    cx, word ptr [esi + 0x1a]
        mov    word ptr [eax + 6], cx
        mov    ecx, dword ptr [ebp - 8]
        inc    ecx
        mov    eax, ecx
        div    dword ptr [esi + 0x25]
        mov    eax, dword ptr [esi + 0x16e]
        mov    dword ptr [edi + eax + 0xc], edx
        add    ebx, 0x20
        mov    eax, dword ptr [esi + 0x16e]
        mov    dword ptr [edi + eax + 8], ebx
        mov    eax, dword ptr [esi + 0x16e]
        add    eax, edi
        mov    edx, dword ptr [eax + 8]
        add    dword ptr [ebp - 4], 0x10
        sub    dx, word ptr [esi + 2]
        add    dx, word ptr [esi + 0x1a]
        mov    word ptr [eax + 6], dx
        mov    eax, dword ptr [esi + 0x16e]
        mov    ebx, dword ptr [edi + eax + 8]
        add    ebx, 0x5f0
        mov    dword ptr [ebp - 8], ecx
        cmp    dword ptr [esi + 0x25], ecx
        ja     loc_1_31F0
loc_1_32B2:
        sub    ecx, ecx
        cmp    dword ptr [esi + 0x25], ecx
        jbe    loc_1_3307
        sub    edx, edx
        add    eax, 0
loc_1_32C0:
        mov    edi, dword ptr [esi + 0x16e]
        add    edi, edx
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
        add    edx, 0x10
        inc    ecx
        cmp    dword ptr [esi + 0x25], ecx
        ja     loc_1_32C0
loc_1_3307:
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
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3330:
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
        jne    loc_1_3450
        cmp    byte ptr [ebx + 0x18], 0
        jne    loc_1_3440
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
        je     loc_1_3434
        cmp    dword ptr [ebp - 0xc], 0x5ea
        ja     loc_1_3434
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
        je     loc_1_33C3
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xd], 0
        jmp    loc_1_33D9
loc_1_33C3:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x166]
        push   eax
        call   sub_1_0330
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xd], al
loc_1_33D9:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], ebx
        mov    eax, dword ptr [esi + 0x8b]
        or     eax, eax
        jne    loc_1_33F1
        mov    dword ptr [esi + 0x87], edi
        jmp    loc_1_33F4
loc_1_33F1:
        mov    dword ptr [eax + 0x24], edi
loc_1_33F4:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x8b], edi
        mov    dword ptr [ecx], 0
        inc    dword ptr [esi + 0xa3]
        cmp    byte ptr [esi + 0x86], 0
        jne    loc_1_3457
        sub    ebx, ebx
loc_1_3414:
        cmp    dword ptr [esi + 0x87], ebx
        je     loc_1_3457
        cmp    byte ptr [esi + 0x85], bl
        je     loc_1_3457
        push   esi
        call   sub_1_34A0
        cmp    byte ptr [esi + 0x86], bl
        je     loc_1_3414
        jmp    loc_1_3457
loc_1_3434:
        dec    dword ptr [ebx + 0x14]
        mov    dword ptr [ebp - 4], 0xc000009a
        jmp    loc_1_3457
loc_1_3440:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_3457
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3450:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_1_3457:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_348F
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_3481
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_3481
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_348F
loc_1_3481:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_348F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_34A0:
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
        call   sub_1_0A00
        or     al, al
        je     loc_1_3508
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x87], eax
        or     eax, eax
        jne    loc_1_34DD
        mov    dword ptr [esi + 0x8b], 0
loc_1_34DD:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_1_0710
        push   edi
        push   esi
        call   sub_1_1850
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_0870
        mov    byte ptr [esi + 0x86], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3508:
        mov    byte ptr [esi + 0x86], 0
        mov    byte ptr [esi + 0x85], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_3520:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x59]
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_3590
        cmp    byte ptr [edi + 0x18], 0
        jne    loc_1_3587
        inc    dword ptr [edi + 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        test   cl, 1
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        jne    loc_1_356A
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
        jmp    loc_1_3595
loc_1_356A:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        and    ecx, 0xfffffffe
        push   eax
        add    ecx, dword ptr [ebp + 0x10]
        push   ecx
        call   sub_1_1970
        dec    dword ptr [edi + 0x14]
        sub    edi, edi
        jmp    loc_1_3595
loc_1_3587:
        mov    edi, 0xc001000c
        jmp    loc_1_3595
        db  08Bh, 0FFh
loc_1_3590:
        mov    edi, 0xc001000d
loc_1_3595:
        mov    eax, dword ptr [esi + 0x59]
        dec    eax
        mov    dword ptr [esi + 0x59], eax
        cmp    eax, 1
        jne    loc_1_35CD
        cmp    byte ptr [esi + 0x8f], 0
        jne    loc_1_35BF
        cmp    dword ptr [esi + 0x75], 0
        jne    loc_1_35BF
        mov    eax, dword ptr [esi + 0xd9]
        sub    eax, esi
        cmp    eax, 0xd9
        je     loc_1_35CD
loc_1_35BF:
        push   0
        add    esi, 0x155
        push   esi
        call   sub_1_04E2
loc_1_35CD:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_35E4:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        dec    ebp
        inc    ebx
        add    byte ptr [eax], al
loc_1_35EC:
        pop    es
        add    byte ptr [edi], al
        db  000h
loc_1_35F0:
        in     al, 0x35
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
loc_1_360C:
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
        add    eax, 0xd000201
        add    al, byte ptr [edx]
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
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
loc_1_3708:
        inc    ebp
        insb   byte ptr es:[edi], dx
        outsb  dx, byte ptr [esi]
        imul   ecx, dword ptr [ebp + 0x43], 0x20
        inc    ecx
        popal  
        jo     0x3788
        jb     loc_1_3717
loc_1_3717:
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
        mov    ebx, dword ptr [0x3718]       ; -> obj1:0x3718
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
        mov    eax, dword ptr [0x3718]       ; -> obj1:0x3718
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x830  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        mov    word ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0xe], 1
        mov    word ptr [ebp - 0xc], 0x6042
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        lea    eax, [ebp - 0x10]
        mov    dword ptr [0x3728], eax       ; -> obj1:0x3728
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x371c], eax       ; -> obj1:0x371C
        mov    byte ptr [ebp - 0x48], 3
        mov    byte ptr [ebp - 0x47], 0
        mov    dword ptr [ebp - 0x42], 0xb40 ; -> loc_1_0B40
        mov    dword ptr [ebp - 0x3e], 0xc70 ; -> loc_1_0C70
        mov    dword ptr [ebp - 0x2e], 0x1c90 ; -> loc_1_1C90
        mov    dword ptr [ebp - 0x3a], 0x3330 ; -> loc_1_3330
        mov    dword ptr [ebp - 0x36], 0x3520 ; -> loc_1_3520
        mov    dword ptr [ebp - 0x32], 0x2880 ; -> loc_1_2880
        mov    dword ptr [ebp - 0x2a], 0x26f0 ; -> loc_1_26F0
        mov    dword ptr [ebp - 0x26], 0xd00 ; -> loc_1_0D00
        mov    dword ptr [ebp - 0x22], 0x5b0 ; -> loc_4_05B0
        mov    dword ptr [ebp - 0x1e], 0xd30 ; -> loc_1_0D30
        mov    eax, dword ptr [0x35ec]       ; -> loc_1_35EC
        mov    ecx, dword ptr [0x35f0]       ; -> loc_1_35F0
        mov    dword ptr [ebp - 0x1a], eax
        lea    eax, [ebp - 0x48]
        mov    dword ptr [ebp - 0x16], ecx
        mov    dword ptr [0x3724], 0x3720    ; -> obj1:0x3724
        mov    dword ptr [0x3720], 0x3720    ; -> obj1:0x3720
        push   0x36
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   0x3730                        ; -> obj1:0x3730
        push   eax
        call   sub_1_0426
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_00D6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_00D6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   edi
        mov    eax, dword ptr [0x35e0]       ; -> obj1:0x35E0
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x2d2
        push   eax
        call   sub_1_0506
        or     eax, eax
        jne    loc_4_04AA
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xb4
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xed], eax
        mov    edx, dword ptr [ebp + 0x20]
        or     edx, edx
        mov    eax, dword ptr [ebp - 4]
        je     loc_4_0157
        mov    byte ptr [eax + 0x55], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 0x2f], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 0x33], cl
        add    eax, 0x2f
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
        jmp    loc_4_015B
loc_4_0157:
        mov    byte ptr [eax + 0x55], 0
loc_4_015B:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0x87
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x56], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x1a], 0xc000
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 0x10
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x162], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x21], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x25], 8
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
        mov    eax, dword ptr [ebp - 4]
        sub    cl, bl
        mov    byte ptr [eax + 0x161], cl
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0560
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0AC0
        lea    edi, [ebp - 0x2c]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x2c], eax
        mov    byte ptr [ebp - 0x28], al
        mov    dword ptr [ebp - 0x22], 3
        mov    dword ptr [ebp - 0x1e], eax
        mov    dword ptr [ebp - 0x1a], eax
        mov    dword ptr [ebp - 0x16], 1
        mov    eax, dword ptr [ebp - 4]
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
        je     loc_4_02C0
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
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
        db  090h
loc_4_02C0:
        cmp    byte ptr [ebp + 0x2c], 0
        je     loc_4_030B
        push   0
        push   2
        push   2
        mov    eax, dword ptr [ebp + 0x30]
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
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
loc_4_030B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_04C0
        or     al, al
        jne    loc_4_0360
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
        mov    eax, 0xc000009a
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
        db  08Bh, 0FFh
loc_4_0360:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x149], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14d], 0x1620 ; -> loc_1_1620
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x151], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x155], 0
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        mov    dword ptr [eax + 0x159], 0xe90 ; -> loc_1_0E90
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x15d], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x166], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_0780
        or     al, al
        mov    eax, dword ptr [ebp - 4]
        jne    loc_4_03F0
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
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
        db  08Bh, 0FFh
loc_4_03F0:
        add    eax, 0x166
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe9
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2f
        push   eax
        push   0x1c80                        ; -> loc_1_1C80
        push   0x1a50                        ; -> loc_1_1A50
        push   0x1b70                        ; -> loc_1_1B70
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_4_0468
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
        mov    eax, 0xc000009a
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
loc_4_0468:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe1], 0x3720 ; -> obj1:0x3720
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [0x3724]       ; -> obj1:0x3724
        mov    dword ptr [ecx + 0xe5], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x3724]       ; -> obj1:0x3724
        add    eax, 0xe1
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xe1
        mov    dword ptr [0x3724], eax       ; -> obj1:0x3724
        sub    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
loc_4_04AA:
        mov    eax, 0xc000009a
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x2c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_04C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [0x35e0]       ; -> obj1:0x35E0
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
        je     loc_4_0511
        push   0
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   esi
        call   sub_1_0A80
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0511:
        mov    eax, dword ptr [0x35e0]       ; -> obj1:0x35E0
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x25]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 0x16e]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_4_0557
        push   1
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        push   esi
        call   sub_1_0A80
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_4_0557:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_0560:
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
loc_4_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x24], 0
        sub    edi, edi
        sub    bl, bl
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 8], edi
        je     loc_4_05E8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_05E8:
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0480
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_0610
        mov    bl, 1
        mov    eax, 0xaa00000a
        jmp    loc_4_0744
        db  08Dh, 064h, 024h, 000h
loc_4_0610:
        cmp    word ptr [ebp - 0x2c], 0x6042
        je     loc_4_0624
        mov    bl, 1
        mov    eax, 0xaa000004
        jmp    loc_4_0744
loc_4_0624:
        mov    al, byte ptr [ebp - 0x2a]
        and    al, 6
        shr    al, 1
        and    eax, 0xff
        or     eax, eax
        je     loc_4_0645
        cmp    eax, 1
        je     loc_4_0650
        cmp    eax, 2
        je     loc_4_0658
        cmp    eax, 3
        je     loc_4_0660
        jmp    loc_4_0666
loc_4_0645:
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_0666
        db  08Dh, 049h, 000h
loc_4_0650:
        mov    word ptr [ebp - 2], 0x1300
        jmp    loc_4_0666
loc_4_0658:
        mov    word ptr [ebp - 2], 0x2300
        jmp    loc_4_0666
loc_4_0660:
        mov    word ptr [ebp - 2], 0x3300
loc_4_0666:
        mov    al, byte ptr [ebp - 0x2a]
        and    al, 0x18
        shr    al, 3
        and    eax, 0xff
        or     eax, eax
        je     loc_4_0688
        cmp    eax, 1
        je     loc_4_0691
        cmp    eax, 2
        je     loc_4_06A0
        cmp    eax, 3
        je     loc_4_06B0
        jmp    loc_4_06B7
loc_4_0688:
        mov    dword ptr [ebp - 0x10], 0xc0000
        jmp    loc_4_06B7
loc_4_0691:
        mov    dword ptr [ebp - 0x10], 0xc8000
        jmp    loc_4_06B7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_06A0:
        mov    dword ptr [ebp - 0x10], 0xd0000
        jmp    loc_4_06B7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_06B0:
        mov    dword ptr [ebp - 0x10], 0xd8000
loc_4_06B7:
        mov    al, byte ptr [ebp - 0x2a]
        shr    al, 6
        and    eax, 0xff
        or     eax, eax
        je     loc_4_06D7
        cmp    eax, 1
        je     loc_4_06E0
        cmp    eax, 2
        je     loc_4_06F0
        cmp    eax, 3
        je     loc_4_0700
        jmp    loc_4_0707
loc_4_06D7:
        mov    dword ptr [ebp - 0xc], 0xc
        jmp    loc_4_0707
loc_4_06E0:
        mov    dword ptr [ebp - 0xc], 7
        jmp    loc_4_0707
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_06F0:
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_4_0707
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0700:
        mov    dword ptr [ebp - 0xc], 9
loc_4_0707:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 0x20], 6
        jne    loc_4_0741
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0741
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    eax, ecx
        lea    edi, [ebp - 0x34]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x34], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x30], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x2f], al
loc_4_0741:
        mov    eax, dword ptr [ebp - 0x18]
loc_4_0744:
        push   eax
        push   ebx
        push   0x20
        push   0x10
        push   edi
        mov    ax, word ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [0x3730]       ; -> obj1:0x3730
        push   eax
        call   sub_4_00F0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0590
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_4_0780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2C00
        push   2
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        lea    edi, [esi + 0x131]
        push   ebx
        push   0xe90                         ; -> loc_1_0E90
        push   esi
        push   0xdc0                         ; -> loc_1_0DC0
        mov    eax, dword ptr [esi + 0xf1]
        push   eax
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0803
        push   esi
        call   sub_1_2AD0
        or     al, al
        je     loc_4_07D8
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_07D8:
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
loc_4_0803:
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
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
VxD__LTEXT ENDS
