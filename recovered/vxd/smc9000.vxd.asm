; SMC9000 -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 4770 instructions recovered by recursive descent

Declare_Virtual_Device SMC9000, 3, 10, SMC9000_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x778  R-X
loc_1_0000:
        mov    al, byte ptr [0x698]          ; -> obj1:0x0698
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x698]              ; -> obj1:0x0698
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_5_0000
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
SMC9000_Control:
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
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
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
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
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
sub_1_03A0:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Fh, 05Eh, 0C2h, 00Ch, 000h
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
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_03F1:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
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
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SMC9000 '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SMC9000_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    dword ptr [eax], esi
        xor    byte ptr [eax], dh
        add    byte ptr [ecx + 0x4f], cl
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_06B0:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x45], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x45], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [edx + edx*2 + 0x41], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + edx*2 + 0x41], dl
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebx + 0x45], dl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_0704:
        push   ebx
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
        xor    dword ptr [eax], esi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        push   esp
        add    byte ptr [ecx], dh
        xor    byte ptr [edx + 0x41], al
        push   ebx
        inc    ebp
        push   esp
        add    byte ptr [ecx + 0x55], al
        dec    ecx
        add    byte ptr [ecx + 0x55], al
        dec    ecx
        add    byte ptr [edx + 0x4e], al
        inc    ebx
        add    byte ptr [edx + 0x4e], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 005h, 000h, 000h, 000h
loc_1_0738:
        push   ebx
        je     0x79c
        outsb  dx, byte ptr [esi]
        popal  
        jb     0x7a4
        and    byte ptr [ebp + 0x69], cl
        arpl   word ptr [edx + 0x6f], si
        jae    0x7c1
        jae    0x7be
        insd   dword ptr es:[edi], dx
        jae    loc_1_076E
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     0x7c2
        outsd  dx, dword ptr [esi]
        jb     0x7b6
        je     0x7c0
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx], bh
        xor    dword ptr [ebx + 0x39], eax
        xor    byte ptr [eax], ah
        inc    ebp
        je     0x7cb
        jb     0x7d4
        je     0x789
        inc    ecx
        popal  
        jo     0x7e2
loc_1_076E:
        jb     loc_1_0771
loc_1_0771:
        add    byte ptr [eax], al
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
        mov    ebx, dword ptr [0x774]        ; -> obj1:0x0774
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
        mov    eax, dword ptr [0x774]        ; -> obj1:0x0774
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
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0552
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
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
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_04F0
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x138  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh, 008h, 000h, 008h, 000h
        dd  loc_1_06A0               ; fixup
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h
        db  009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h
        db  00Dh, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 010h, 001h, 001h, 000h
        db  011h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 001h, 001h, 002h, 000h
        db  002h, 001h, 002h, 000h, 003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h, 005h, 001h, 002h, 000h
        db  001h, 002h, 002h, 000h, 002h, 002h, 002h, 000h, 003h, 002h, 002h, 000h, 004h, 002h, 002h, 000h
        db  005h, 002h, 002h, 000h, 006h, 002h, 002h, 000h, 007h, 002h, 002h, 000h, 008h, 002h, 002h, 000h
        db  009h, 002h, 002h, 000h, 00Ah, 002h, 002h, 000h, 00Bh, 002h, 002h, 000h, 00Ch, 002h, 002h, 000h
        db  00Dh, 002h, 002h, 000h, 00Eh, 002h, 002h, 000h, 001h, 001h, 001h, 001h, 002h, 001h, 001h, 001h
        db  003h, 001h, 001h, 001h, 004h, 001h, 001h, 001h, 001h, 001h, 002h, 001h, 002h, 001h, 002h, 001h
        db  003h, 001h, 002h, 001h, 001h, 002h, 002h, 001h, 002h, 002h, 002h, 001h, 003h, 002h, 002h, 001h
        db  004h, 002h, 002h, 001h, 005h, 002h, 002h, 001h, 006h, 002h, 002h, 001h, 007h, 002h, 002h, 001h
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h
        db  009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h
        db  00Dh, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 010h, 001h, 001h, 000h
        db  011h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 001h, 001h, 001h, 001h
        db  002h, 001h, 001h, 001h, 003h, 001h, 001h, 001h, 004h, 001h, 001h, 001h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x28e0  R-X
sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        mov    word ptr [ebp - 0x14], 0
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        lea    ecx, [ebp - 0x14]
        push   0
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 0x12], 0
        push   eax
        push   ecx
        push   edx
        call   sub_1_041A
loc_5_0027:
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        lea    edx, [ebp - 4]
        push   ecx
        push   0
        push   0x14
        push   edx
        call   sub_1_0506
loc_5_003B:
        test   eax, eax
        je     loc_5_004C
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_004C:
        mov    eax, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    esi, dword ptr [0]            ; -> obj4:0x0004
        mov    ecx, dword ptr [4]            ; -> obj4:0x0008
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [eax + 0x10], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ebp - 0x1e], esi
        mov    byte ptr [ebp - 0x4b], bl
        mov    byte ptr [ebp - 0x4c], 3
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_5_0950
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_5_0AA0
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_5_0C80
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_5_0CC0
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_5_0D80
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_5_0B40
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_5_0120
        push   0x36
        mov    dword ptr [ebp - 0x1a], ecx
        lea    eax, [ebp - 0x4c]
        mov    edx, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   edx
        lea    eax, [edx + 4]
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_5_0920
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_5_0160
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_5_0890
        push   ecx
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        call   sub_1_0426
loc_5_00E1:
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_5_00FC
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   0x14
        push   eax
        call   sub_1_050C
loc_5_00F2:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0420
loc_5_00FC:
        mov    eax, 0
        pop    esi
        cmp    dword ptr [ebp - 0xc], 1
        adc    eax, -1
        pop    ebx
        and    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_042C
loc_5_0138:
        mov    edi, dword ptr [esi]
        push   0
        push   0x14
        push   esi
        call   sub_1_050C
loc_5_0144:
        push   0
        push   edi
        call   sub_1_0420
loc_5_014C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0160:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 0x78
        mov    word ptr [ebp - 0x50], ax
        push   ebx
        mov    word ptr [ebp - 0x4e], ax
        push   esi
        push   edi
        mov    eax, 0xa
        mov    dword ptr [ebp - 0x4c], 0     ; -> loc_1_06B0
        mov    dword ptr [ebp - 0x44], 0     ; -> obj1:0x06C4
        mov    dword ptr [ebp - 0x3c], 0     ; -> obj1:0x06EC
        mov    word ptr [ebp - 0x48], ax
        mov    word ptr [ebp - 0x46], ax
        mov    eax, 0xc
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_0704
        mov    word ptr [ebp - 0x40], ax
        mov    word ptr [ebp - 0x3e], ax
        mov    eax, 0xb
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x0718
        mov    word ptr [ebp - 0x20], ax
        mov    word ptr [ebp - 0x1e], ax
        mov    eax, 8
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x0724
        mov    word ptr [ebp - 0x28], ax
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x072C
        mov    word ptr [ebp - 0x26], ax
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 4
        push   esi
        mov    word ptr [ebp - 0x38], ax
        lea    ecx, [ebp - 0xc]
        mov    word ptr [ebp - 0x36], ax
        push   ecx
        mov    word ptr [ebp - 0x30], ax
        mov    word ptr [ebp - 0x2e], ax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0584
loc_5_0201:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0213
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0213:
        mov    ebx, dword ptr [0]            ; -> obj4:0x0000
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x118
        push   eax
        call   sub_1_0506
loc_5_022A:
        test   eax, eax
        je     loc_5_0245
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
loc_5_0237:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0245:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x46
        push   ebx
        rep stosd dword ptr es:[edi], eax
        push   eax
        push   0x5f0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x9f
        push   eax
        call   sub_1_0506
loc_5_0266:
        test   eax, eax
        je     loc_5_0291
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
loc_5_0273:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x118
        push   eax
        call   sub_1_050C
loc_5_0283:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0291:
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x6e], ebx
        mov    eax, dword ptr [ebp - 4]
        push   1
        lea    ecx, [ebp - 0x50]
        mov    word ptr [eax + 0x76], 9
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    dword ptr [eax + 0x78], 0x300
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebp - 8]
        mov    dword ptr [eax + 0x7d], 0x5ea
        mov    edx, dword ptr [ebp - 0xc]
        lea    eax, [ebp - 0x10]
        push   edx
        push   eax
        push   ecx
        call   sub_1_058A
loc_5_02CE:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_032C
        mov    eax, dword ptr [ebp - 0x10]
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x78], edx
        mov    ecx, dword ptr [ebp - 4]
        test   dword ptr [ecx + 0x78], 0x1c1f
        je     loc_5_032C
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
loc_5_02F8:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x5f0
        mov    ecx, dword ptr [eax + 0x9f]
        push   ecx
        call   sub_1_050C
loc_5_030E:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_031E:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_032C:
        lea    edi, [ebp - 0x78]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, 1
        mov    dword ptr [ebp - 0x6e], eax
        mov    dword ptr [ebp - 0x62], eax
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 0x78]
        mov    ecx, dword ptr [eax + 0x78]
        add    eax, 0x78
        push   edx
        mov    dword ptr [ebp - 0x5e], ecx
        mov    dword ptr [ebp - 0x56], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x5a], 0x10
        push   eax
        mov    edx, dword ptr [ebx + 4]
        push   esi
        lea    eax, [ecx + 8]
        push   ecx
        push   edx
        push   eax
        call   sub_1_0432
loc_5_0375:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_5_03B7
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
loc_5_0385:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x5f0
        mov    ecx, dword ptr [eax + 0x9f]
        push   ecx
        call   sub_1_050C
loc_5_039B:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_03AB:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_03B7:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_16B0
        push   0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_5_0424
        push   0xaa000004
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_03DA:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        call   sub_1_0590
loc_5_03E6:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_03F2:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_0408:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_0418:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0424:
        lea    eax, [ebp - 0x48]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        lea    edx, [ebp - 0x10]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        call   sub_1_058A
loc_5_0439:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_04FD
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        xor    eax, eax
        mov    word ptr [ecx + 0x76], dx
        mov    ecx, dword ptr [ebp - 4]
        and    word ptr [ecx + 0x81], 0xfff9
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 0x76]
        sub    eax, 3
        cmp    eax, 8
        ja     loc_5_047C
        xor    ecx, ecx
        mov    cl, byte ptr [eax]            ; -> obj5:0x0880
        jmp    dword ptr [ecx*4]             ; -> obj5:0x086C
loc_5_047C:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa00000a
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_048F:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        call   sub_1_0590
loc_5_049B:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_04A7:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_04BD:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_04CD:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_04DB:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 0x81], 2
        jmp    loc_5_04FD
loc_5_04E7:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 0x81], 4
        jmp    loc_5_04FD
loc_5_04F3:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 0x81], 6
loc_5_04FD:
        push   2
        lea    eax, [ebp - 0x40]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
loc_5_0514:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_053E
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_052C:
        test   al, al
        je     loc_5_06C1
loc_5_0534:
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 0x82], 1
loc_5_053E:
        push   0
        lea    eax, [ebp - 0x20]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_058A
loc_5_0555:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_05B5
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 4], 1
        jne    loc_5_05B5
        mov    eax, dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        call   sub_1_048C
loc_5_0579:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_05B5
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x49], 1
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_5_0761
        mov    eax, dword ptr [ebp - 0x14]
        test   byte ptr [eax], 1
        jne    loc_5_0761
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx + 0x91], ecx
        add    edx, 0x91
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
loc_5_05B5:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0590
loc_5_05BE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0xc
        xor    esi, esi
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x10]
        mov    dword ptr [eax + 0xc], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], esi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0x14
        mov    dword ptr [eax + 0x18], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1c
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x24], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x24
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x40], 1
        mov    eax, dword ptr [ebp - 4]
        push   0x1388
        mov    dword ptr [eax + 0x34], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x38], 0     ; -> loc_5_2850
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x3c], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x34
        push   eax
        call   sub_1_04E2
loc_5_0639:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x72
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x30
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x91
        push   eax
        push   0                             ; -> loc_5_0FD0
        push   0                             ; -> loc_5_0F50
        push   0                             ; -> loc_5_0EC0
        push   0x10
        call   sub_2_0000
loc_5_0666:
        test   al, al
        jne    loc_5_07C0
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_0680:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_068F:
        push   esi
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_06A4:
        push   esi
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_06B3:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_06C1:
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_06D3:
        test   al, al
        jne    loc_5_0534
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_06ED:
        test   al, al
        je     loc_5_0702
        mov    eax, dword ptr [ebp - 4]
        and    word ptr [eax + 0x81], 0xfeff
        jmp    loc_5_053E
loc_5_0702:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa00000a
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_0715:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        call   sub_1_0590
loc_5_0721:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_072D:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_0743:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_0753:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0761:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000009
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_0774:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        call   sub_1_0590
loc_5_0780:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_078C:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_07A2:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_07B2:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_07C0:
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    ax, word ptr [eax + 0x76]
        push   0
        and    eax, 0xffff
        push   eax
        push   eax
        push   0                             ; -> loc_5_1050
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> loc_5_1000
        mov    ecx, dword ptr [eax + 8]
        add    eax, 0x54
        push   ecx
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_04EE
loc_5_07F1:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_084D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000005
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_05D8
loc_5_080A:
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        push   edx
        call   sub_1_0438
loc_5_0819:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0x72]
        push   edx
        call   sub_2_0120
loc_5_0825:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x5f0
        mov    edx, dword ptr [ecx + 0x9f]
        push   edx
        call   sub_1_050C
loc_5_083B:
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   0x118
        push   ecx
        call   sub_1_050C
loc_5_084B:
        jmp    loc_5_0856
loc_5_084D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_1910
loc_5_0856:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    word ptr [eax + 0x6c], 1
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
        ; ---- dispatch table (5 entries) ----
        dd  loc_5_04DB
        dd  loc_5_04FD
        dd  loc_5_04E7
        dd  loc_5_04F3
        dd  loc_5_047C
        db  000h, 004h, 004h, 004h, 004h, 004h, 001h, 002h, 003h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x40], 0
        je     loc_5_08B6
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x34]
        mov    byte ptr [esi + 0x40], 0
        push   eax
        push   ecx
        call   sub_1_04E8
loc_5_08B6:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_5_08DB
        mov    edi, 0xea60
loc_5_08C1:
        push   0x3e8
        call   sub_1_0500
loc_5_08CB:
        dec    edi
        jne    loc_5_08C1
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x34]
        push   eax
        push   ecx
        call   sub_1_04E8
loc_5_08DB:
        mov    ecx, dword ptr [esi + 0x78]
        mov    eax, 2
        lea    edx, [ecx + 0xe]
        out    dx, ax
        lea    edx, [ecx + 0xd]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_04F4
loc_5_08F7:
        mov    ecx, dword ptr [esi + 0x72]
        push   ecx
        call   sub_2_0120
loc_5_0900:
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   sub_1_0438
loc_5_0909:
        push   0
        push   0x118
        push   esi
        call   sub_1_050C
loc_5_0916:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0920:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 0x6c], 2
        mov    ebp, esp
        jne    loc_5_0937
        mov    eax, 0xc001000d
        pop    ebp
        ret    8
loc_5_0937:
        push   0
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_5_1EC0
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0950:
        push   ebp
        mov    edx, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [ebp + 0x14]
        cmp    edx, esi
        je     loc_5_097B
        mov    ecx, ebx
loc_5_0968:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_5_097B
        cmp    eax, 5
        je     loc_5_097B
        add    ecx, 4
        inc    esi
        cmp    esi, edx
        jb     loc_5_0968
loc_5_097B:
        cmp    esi, edx
        jne    loc_5_098D
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_098D:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        push   ecx
        lea    edx, [ebp - 4]
        push   0
        mov    dword ptr [eax], esi
        push   0x20
        push   edx
        call   sub_1_0506
loc_5_09A6:
        test   eax, eax
        je     loc_5_09CE
        push   0
        mov    edi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_05D8
loc_5_09BD:
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_09CE:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax + 0x18], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1c
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 0x72]
        push   eax
        push   ecx
        call   sub_2_0170
loc_5_0A01:
        test   al, al
        jne    loc_5_0A33
        push   0
        mov    eax, dword ptr [edi + 8]
        push   0xaa000002
        push   eax
        call   sub_1_05D8
loc_5_0A15:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   0x20
        push   eax
        call   sub_1_050C
loc_5_0A25:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_0A33:
        inc    dword ptr [edi + 0x4a]
        cmp    word ptr [edi + 0x6c], 2
        mov    eax, dword ptr [ebp - 4]
        jne    loc_5_0A4D
        mov    word ptr [eax + 0xc], 8
        mov    eax, 0x103
        jmp    loc_5_0A55
loc_5_0A4D:
        mov    word ptr [eax + 0xc], 1
        xor    eax, eax
loc_5_0A55:
        lea    ecx, [edi + 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [edi + 0x10]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 0x10]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x10], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebx + esi*4]
        mov    dword ptr [ecx + 0x14], ebx
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0x14], 0
        jne    loc_5_0A8B
        inc    byte ptr [edi + 0x53]
        jmp    loc_5_0A8E
loc_5_0A8B:
        inc    byte ptr [edi + 0x52]
loc_5_0A8E:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], edx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_5_0AA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0xc], 1
        mov    edi, dword ptr [esi + 8]
        je     loc_5_0ABD
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0ABD:
        cmp    word ptr [edi + 0x6c], 2
        jne    loc_5_0AD5
        mov    eax, 0x103
        pop    edi
        mov    word ptr [esi + 0xc], 5
        pop    esi
        pop    ebp
        ret    4
loc_5_0AD5:
        dec    dword ptr [edi + 0x4a]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], edx
        cmp    dword ptr [edi + 0x4a], 0
        jne    loc_5_0AF3
        push   edi
        call   sub_5_1910
loc_5_0AF3:
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_5_0AFE
        dec    byte ptr [edi + 0x53]
        jmp    loc_5_0B01
loc_5_0AFE:
        dec    byte ptr [edi + 0x52]
loc_5_0B01:
        push   0
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    ecx, dword ptr [edi + 0x72]
        push   ecx
        call   sub_2_0240
loc_5_0B10:
        cmp    eax, 0xc001000e
        jne    loc_5_0B22
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0B22:
        push   0
        push   0x20
        push   esi
        call   sub_1_050C
loc_5_0B2C:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0B40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ax, word ptr [esi + 0x6c]
        cmp    ax, 2
        je     loc_5_0C66
        cmp    ax, 3
        je     loc_5_0C66
        mov    word ptr [esi + 0x6c], 3
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0xc
        je     loc_5_0B97
loc_5_0B7A:
        push   0
        mov    eax, dword ptr [edi + 0x18]
        push   0
        push   0x40010004
        push   eax
        call   sub_1_046E
loc_5_0B8C:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0xc
        jne    loc_5_0B7A
loc_5_0B97:
        push   esi
        lea    eax, [esi + 0x54]
        mov    word ptr [esi + 0x6c], 2
        push   0                             ; -> sub_5_1910
        push   eax
        call   sub_1_04FA
loc_5_0BAC:
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0xc
        je     loc_5_0C55
loc_5_0BC0:
        mov    edi, dword ptr [ebp - 4]
        push   0
        lea    ebx, [edi + 0x18]
        push   0
        push   0x40010005
        mov    ecx, dword ptr [ebx]
        push   ecx
        call   sub_1_046E
loc_5_0BD7:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0474
loc_5_0BDF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [edi + 0xc]
        mov    dword ptr [ebp - 4], ecx
        cmp    ax, 5
        jne    loc_5_0C2F
        push   0
        dec    dword ptr [esi + 0x4a]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [esi + 0x72]
        mov    ecx, dword ptr [edi + 0x1c]
        push   ecx
        push   edx
        call   sub_2_0240
loc_5_0C12:
        cmp    eax, 0xc001000e
        je     loc_5_0C47
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0444
loc_5_0C23:
        push   0
        push   0x20
        push   edi
        call   sub_1_050C
loc_5_0C2D:
        jmp    loc_5_0C47
loc_5_0C2F:
        cmp    ax, 8
        jne    loc_5_0C47
        mov    word ptr [edi + 0xc], 1
        push   0
        mov    eax, dword ptr [ebx]
        push   0
        push   eax
        call   sub_1_043E
loc_5_0C47:
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0xc
        jne    loc_5_0BC0
loc_5_0C55:
        xor    eax, eax
        pop    edi
        mov    word ptr [esi + 0x6c], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0C66:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0C80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 0x6c], 2
        jne    loc_5_0C9A
        mov    eax, 0xc001000d
        pop    ebp
        ret    8
loc_5_0C9A:
        cmp    word ptr [eax + 0xc], 1
        je     loc_5_0CAA
        mov    eax, 0xc0000001
        pop    ebp
        ret    8
loc_5_0CAA:
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_5_1EC0
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0CC0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [edx + 8]
        cmp    word ptr [esi + 0x6c], 2
        jne    loc_5_0CE1
        mov    eax, 0xc001000d
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0CE1:
        cmp    word ptr [edx + 0xc], 1
        je     loc_5_0CF4
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0CF4:
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ecx + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 8]
        mov    dword ptr [eax + 4], ecx
        push   edx
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_0530
loc_5_0D16:
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_5_0D66
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_5_0D66
        inc    dword ptr [esi + 0xa8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        lea    ecx, [ebp - 4]
        push   ecx
        push   esi
        call   sub_5_1B00
        test   al, al
        je     loc_5_0D5A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1C20
loc_5_0D5A:
        mov    eax, 0x103
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0D66:
        mov    eax, 0xc001000f
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0D80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    ecx, dword ptr [ebp + 0x10]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edx, dword ptr [esi + 0x9f]
        cmp    word ptr [esi + 0x6c], 2
        lea    edi, [edx + ecx + 0xe]
        jne    loc_5_0DB2
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0DB2:
        cmp    word ptr [eax + 0xc], 1
        je     loc_5_0DC7
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0DC7:
        cmp    byte ptr [esi + 0x45], 0
        jne    loc_5_0DDB
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0DDB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax + 4]
        sub    esi, dword ptr [ebp + 0x10]
        sub    esi, 0xe
        jns    loc_5_0DF7
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0DF7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 0x1c]
        lea    ecx, [ebp - 0x10]
        lea    edx, [ebp - 8]
        push   ecx
        mov    dword ptr [eax], ebx
        push   edx
        mov    eax, dword ptr [ebp + 0x18]
        push   ebx
        push   ebx
        push   eax
        call   sub_1_0530
loc_5_0E11:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    esi, eax
        jle    loc_5_0E1A
        mov    esi, eax
loc_5_0E1A:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    esi, eax
        jle    loc_5_0E23
        mov    esi, eax
loc_5_0E23:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     loc_5_0E4F
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0xe
        push   eax
        push   ecx
        push   esi
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        push   ecx
        call   sub_1_057E
loc_5_0E44:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0E4F:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0EA2
loc_5_0E55:
        test   esi, esi
        je     loc_5_0EA2
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_0554
loc_5_0E6A:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0E8F
        cmp    dword ptr [ebp - 4], esi
        jbe    loc_5_0E78
        mov    dword ptr [ebp - 4], esi
loc_5_0E78:
        sub    esi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        push   ecx
        call   sub_1_05EC
loc_5_0E8C:
        add    edi, dword ptr [ebp - 4]
loc_5_0E8F:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_0578
loc_5_0E9C:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_0E55
loc_5_0EA2:
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        pop    esi
        mov    dword ptr [eax], ebx
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0EC0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 4
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        mov    ebx, dword ptr [edi + 0x78]
        jne    loc_5_0F26
        lea    esi, [ebx + 0xe]
        mov    edx, esi
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
loc_5_0EF0:
        mov    edx, ecx
        inc    cx
        and    edx, 0xffff
        cmp    cx, 4
        mov    byte ptr [edi + edx*2 + 0x98], al
        mov    byte ptr [edi + edx*2 + 0x97], al
        lea    edx, [ebx + edx*2]
        out    dx, ax
        jb     loc_5_0EF0
        mov    ax, word ptr [ebp - 2]
        mov    edx, esi
        out    dx, ax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_5_0F26:
        lea    ecx, [esi + esi*2]
        mov    eax, dword ptr [ebp + 0x14]
        lea    ebx, [eax + ecx*2 - 6]
loc_5_0F30:
        push   ebx
        call   sub_5_1DD0
        push   1
        sub    ebx, 6
        push   eax
        push   edi
        call   sub_5_1E40
        dec    esi
        jne    loc_5_0F30
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_5_0F50:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [eax + 8]
        xor    si, si
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        test   al, 1
        je     loc_5_0F6B
        mov    si, 0x100
loc_5_0F6B:
        test   al, 2
        je     loc_5_0F73
        mov    si, 0x100
loc_5_0F73:
        test   al, 8
        je     loc_5_0F7B
        mov    si, 0x100
loc_5_0F7B:
        test   al, 4
        je     loc_5_0F83
        mov    si, 0x104
loc_5_0F83:
        test   al, 0x20
        je     loc_5_0F91
        mov    byte ptr [ecx + 0x48], 1
        mov    si, 0x102
        jmp    loc_5_0F95
loc_5_0F91:
        mov    byte ptr [ecx + 0x48], 0
loc_5_0F95:
        or     si, 0x200
        mov    ebx, dword ptr [ecx + 0x78]
        lea    ecx, [ebx + 0xe]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        xor    eax, eax
        out    dx, ax
        mov    ax, si
        lea    edx, [ebx + 4]
        out    dx, ax
        mov    ax, di
        mov    edx, ecx
        out    dx, ax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0FD0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        call   sub_1_0444
loc_5_0FE2:
        push   0
        push   0x20
        push   esi
        call   sub_1_050C
loc_5_0FEC:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1000:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [ecx + 0x78]
        lea    edi, [esi + 0xe]
        mov    edx, edi
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0xc]
        in     al, dx
        test   al, 0x3b
        je     loc_5_103A
        lea    edx, [esi + 0xd]
        xor    al, al
        out    dx, al
        mov    ebx, 1
        mov    byte ptr [ecx + 0x47], bl
loc_5_103A:
        mov    edx, edi
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        pop    edi
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_5_1050:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x47], 1
        mov    byte ptr [esi + 0x43], 1
        mov    edi, dword ptr [esi + 0x78]
        lea    eax, [edi + 0xc]
        lea    ecx, [edi + 0xe]
        mov    edx, ecx
        mov    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, ecx
        mov    eax, 2
        mov    dword ptr [ebp - 0xc], ecx
        out    dx, ax
        lea    edx, [edi + 6]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [edi + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
loc_5_1094:
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x10]
        xor    ebx, ebx
        in     al, dx
        cmp    byte ptr [esi + 0x41], 0
        mov    bl, al
        je     loc_5_10B7
        cmp    byte ptr [esi + 0x44], 0
        jne    loc_5_10B7
        and    ebx, 0x3b
        jmp    loc_5_10BA
loc_5_10B7:
        and    ebx, 0x33
loc_5_10BA:
        test   ebx, ebx
        je     loc_5_1102
        test   bl, 1
        je     loc_5_10CB
        push   esi
        call   sub_5_1180
        jmp    loc_5_1094
loc_5_10CB:
        test   bl, 2
        je     loc_5_10D8
        push   esi
        call   sub_5_1300
        jmp    loc_5_1094
loc_5_10D8:
        test   bl, 8
        je     loc_5_10E5
        push   esi
        call   sub_5_1590
        jmp    loc_5_1094
loc_5_10E5:
        test   bl, 0x30
        je     loc_5_1094
        test   bl, 0x20
        je     loc_5_10F5
        push   esi
        call   sub_5_1660
loc_5_10F5:
        test   bl, 0x10
        je     loc_5_1094
        push   esi
        call   sub_5_1620
        jmp    loc_5_1094
loc_5_1102:
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        lea    edx, [edi + 6]
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    byte ptr [esi + 0x41], 0
        out    dx, ax
        lea    edx, [edi + 2]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 0x33
        je     loc_5_112E
        cmp    byte ptr [esi + 0x44], 0
        jne    loc_5_112E
        mov    al, 0x3b
loc_5_112E:
        mov    byte ptr [esi + 0x43], 0
        lea    edx, [edi + 0xd]
        out    dx, al
        cmp    byte ptr [esi + 0x46], 0
        mov    ax, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0xc]
        out    dx, ax
        je     loc_5_1152
        mov    byte ptr [esi + 0x46], 0
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_1_0290
loc_5_1152:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_5_1170
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        call   sub_5_1B00
        test   al, al
        je     loc_5_1170
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   esi
        call   sub_5_1C20
loc_5_1170:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [ebp - 8], eax
        add    eax, 8
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 5
        mov    edx, eax
        mov    dword ptr [ebp - 0x14], eax
        in     al, dx
        test   al, 0x80
        jne    loc_5_12E9
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        mov    dword ptr [ebp - 0x18], eax
loc_5_11B5:
        mov    ebx, dword ptr [esi + 0x9f]
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, 0xe000
        push   2
        out    dx, ax
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_03F1
loc_5_11D1:
        mov    di, word ptr [ebx]
        test   di, 0xac00
        mov    cx, word ptr [ebx + 2]
        jne    loc_5_12B6
        sub    cx, 6
        test   di, 0x1000
        je     loc_5_11F0
        inc    cx
loc_5_11F0:
        lea    eax, [ecx - 0xe]
        mov    edx, dword ptr [esi + 0x9f]
        mov    word ptr [ebp - 2], ax
        mov    ebx, ecx
        and    ebx, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], edx
        lea    eax, [ebx + 3]
        sar    eax, 2
        push   eax
        push   edx
        push   ecx
        call   sub_1_0406
loc_5_1219:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0x80
        mov    dword ptr [ebp - 0x20], 0
        mov    byte ptr [esi + 0x45], 1
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [ebp - 0x1c], ebx
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 0x20]
        add    eax, 0xe
        mov    edx, dword ptr [esi + 0x72]
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   eax
        push   ecx
        push   edx
        call   sub_1_00B0
loc_5_1254:
        mov    byte ptr [esi + 0x45], 0
        mov    byte ptr [esi + 0x46], 1
        inc    dword ptr [esi + 0xb0]
        test   di, 0x4000
        je     loc_5_1277
        add    dword ptr [esi + 0xe8], ebx
        inc    dword ptr [esi + 0xec]
        jmp    loc_5_1298
loc_5_1277:
        test   di, 1
        je     loc_5_128C
        add    dword ptr [esi + 0xe0], ebx
        inc    dword ptr [esi + 0xe4]
        jmp    loc_5_1298
loc_5_128C:
        add    dword ptr [esi + 0xd8], ebx
        inc    dword ptr [esi + 0xdc]
loc_5_1298:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   al, 1
        jne    loc_5_1298
        mov    edx, dword ptr [ebp - 0x14]
        in     al, dx
        test   al, 0x80
        je     loc_5_11B5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_12B6:
        inc    dword ptr [esi + 0xb8]
        test   di, 0x8000
        je     loc_5_12C9
        inc    dword ptr [esi + 0xf4]
loc_5_12C9:
        test   di, 0x2000
        je     loc_5_12D6
        inc    dword ptr [esi + 0xf0]
loc_5_12D6:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0x80
        out    dx, ax
loc_5_12E0:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   al, 1
        jne    loc_5_12E0
loc_5_12E9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 0
        mov    ebx, dword ptr [esi + 0x78]
        dec    dword ptr [esi + 0xa4]
        lea    edx, [ebx + 4]
        in     al, dx
        lea    edx, [ebx + 2]
        out    dx, al
        lea    edx, [ebx + 6]
        mov    eax, 0x6000
        out    dx, ax
        lea    edx, [ebx + 8]
        in     ax, dx
        mov    cx, ax
        in     ax, dx
        mov    edx, ebx
        lea    edi, [eax - 6]
        test   cx, 0x8e10
        mov    eax, 0xa0
        out    dx, ax
        lea    edx, [ebx + 0xc]
        mov    al, 2
        out    dx, al
        je     loc_5_1397
        mov    dword ptr [ebp - 0x14], 0xc0000001
        inc    dword ptr [esi + 0xb4]
        test   ch, 0x80
        je     loc_5_1369
        inc    dword ptr [esi + 0x10c]
loc_5_1369:
        test   ch, 4
        je     loc_5_1374
        inc    dword ptr [esi + 0x114]
loc_5_1374:
        test   ch, 2
        je     loc_5_137F
        inc    dword ptr [esi + 0x104]
loc_5_137F:
        test   cl, 0x10
        je     loc_5_138A
        inc    dword ptr [esi + 0x100]
loc_5_138A:
        test   cl, 0x20
        je     loc_5_1405
        inc    dword ptr [esi + 0x110]
        jmp    loc_5_1405
loc_5_1397:
        inc    dword ptr [esi + 0xac]
        test   cl, 0x40
        je     loc_5_13B7
        mov    eax, edi
        inc    dword ptr [esi + 0xd4]
        and    eax, 0xffff
        add    dword ptr [esi + 0xd0], eax
        jmp    loc_5_13E4
loc_5_13B7:
        test   cl, 8
        je     loc_5_13D1
        mov    eax, edi
        inc    dword ptr [esi + 0xcc]
        and    eax, 0xffff
        add    dword ptr [esi + 0xc8], eax
        jmp    loc_5_13E4
loc_5_13D1:
        mov    eax, edi
        inc    dword ptr [esi + 0xc4]
        and    eax, 0xffff
        add    dword ptr [esi + 0xc0], eax
loc_5_13E4:
        test   cl, 0x80
        je     loc_5_13EF
        inc    dword ptr [esi + 0x108]
loc_5_13EF:
        test   cl, 4
        je     loc_5_13FA
        inc    dword ptr [esi + 0xfc]
loc_5_13FA:
        test   cl, 2
        je     loc_5_1405
        inc    dword ptr [esi + 0xf8]
loc_5_1405:
        lea    ecx, [esi + 0x24]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_5_142A
        dec    dword ptr [esi + 0xa8]
        mov    dword ptr [ebp - 0xc], eax
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        cmp    dword ptr [eax], 0
        jne    loc_5_142A
        mov    dword ptr [esi + 0x28], ecx
loc_5_142A:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_1534
        mov    eax, dword ptr [ebp - 0xc]
        lea    edx, [ebp - 0x18]
        push   edx
        mov    ecx, dword ptr [eax + 4]
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x1c], ecx
        push   eax
        push   0
        push   0
        push   ecx
        call   sub_1_0530
loc_5_144F:
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_0554
loc_5_1460:
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [esi + 0x72]
        push   ecx
        push   edx
        call   sub_1_0330
loc_5_146D:
        test   al, al
        je     loc_5_151D
        cmp    byte ptr [esi + 0x48], 0
        jne    loc_5_151D
        mov    ebx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x18], ebx
        jbe    loc_5_14F1
        lea    eax, [ebx - 0xe]
        cmp    eax, dword ptr [esi + 0x7d]
        jae    loc_5_14F1
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x9f]
loc_5_149A:
        mov    eax, dword ptr [esi + 0x7d]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, ebx
        cmp    ecx, eax
        jbe    loc_5_14AB
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_5_14AB:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        lea    edx, [edi + ebx]
        push   ecx
        push   edx
        call   sub_1_05EC
loc_5_14BC:
        add    ebx, dword ptr [ebp - 4]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
loc_5_14CC:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_14F1
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_0554
loc_5_14E3:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_14F1
        lea    eax, [ebx - 0xe]
        cmp    eax, dword ptr [esi + 0x7d]
        jb     loc_5_149A
loc_5_14F1:
        mov    eax, dword ptr [ebp - 0x18]
        sub    ebx, 0xe
        sub    eax, 0xe
        lea    ecx, [edi + 0xe]
        mov    byte ptr [esi + 0x45], 1
        push   eax
        push   ebx
        push   ecx
        lea    eax, [ebp - 0x1c]
        push   0xe
        mov    edx, dword ptr [esi + 0x72]
        push   edi
        push   edi
        push   eax
        push   edx
        call   sub_1_00B0
loc_5_1515:
        mov    byte ptr [esi + 0x45], 0
        mov    byte ptr [esi + 0x46], 1
loc_5_151D:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x1c]
        push   edx
        mov    ecx, dword ptr [ecx + 8]
        mov    eax, dword ptr [ecx + 0x18]
        push   eax
        call   sub_1_044A
loc_5_1534:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_5_1558
loc_5_153A:
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_5_1B00
        test   al, al
        je     loc_5_1558
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_5_1C20
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_5_153A
loc_5_1558:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 050h, 078h, 083h, 0C2h, 00Ch, 0B0h, 004h, 0EEh, 05Dh
        db  0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1590:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 0x78]
        lea    esi, [edi + 0x14]
        mov    byte ptr [edi + 0x41], cl
        lea    edx, [ebx + 3]
        in     al, dx
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     loc_5_15C5
        mov    dword ptr [ebp - 4], edx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi], edx
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [edx], ecx
        jne    loc_5_15C5
        mov    dword ptr [edi + 0x18], esi
loc_5_15C5:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_15DD
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0xc], al
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        call   sub_5_1C20
        jmp    loc_5_15EA
loc_5_15DD:
        lea    edx, [ebx + 2]
        out    dx, al
        mov    edx, ebx
        mov    eax, 0xa0
        out    dx, ax
loc_5_15EA:
        cmp    dword ptr [esi], 0
        je     loc_5_160C
loc_5_15EF:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_5_1B00
        test   al, al
        je     loc_5_160C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        call   sub_5_1C20
        cmp    dword ptr [esi], 0
        jne    loc_5_15EF
loc_5_160C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1620:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 0x78]
        lea    esi, [ecx + 0xe]
        mov    edx, esi
        out    dx, ax
        lea    edx, [ecx + 4]
        inc    dword ptr [edi + 0xbc]
        in     ax, dx
        and    ax, 0xfffe
        out    dx, ax
        mov    edx, esi
        mov    eax, 2
        out    dx, ax
        lea    edx, [ecx + 0xc]
        mov    al, 0x10
        out    dx, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_1660:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    edi, dword ptr [ecx + 0x78]
        lea    ebx, [edi + 0xe]
        mov    edx, ebx
        out    dx, ax
        lea    edx, [edi + 2]
        in     ax, dx
        mov    si, ax
        mov    edx, edi
        in     ax, dx
        or     al, 1
        out    dx, ax
        mov    edx, ebx
        mov    eax, 2
        out    dx, ax
        test   si, 0x10
        je     loc_5_169C
        inc    dword ptr [ecx + 0x100]
loc_5_169C:
        test   si, 0x20
        je     loc_5_16A9
        inc    dword ptr [ecx + 0x110]
loc_5_16A9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_5_16B0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20
        mov    eax, dword ptr [ecx + 0x78]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x14], eax
        push   edi
        lea    edx, [eax + 0xe]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 0x14]
        add    ecx, 0xa
        mov    dword ptr [ebp - 0xc], edx
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0xffff
        out    dx, ax
        mov    edx, ecx
        mov    dword ptr [ebp - 0x18], ecx
        in     ax, dx
        inc    ax
        je     loc_5_16FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_16FC:
        mov    edx, dword ptr [ebp - 0x18]
        xor    eax, eax
        out    dx, ax
        in     ax, dx
        test   ax, ax
        je     loc_5_1718
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1718:
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 8]
        in     ax, dx
        mov    word ptr [ecx + 0x81], ax
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, 0x40
        out    dx, ax
        mov    edx, dword ptr [ebp - 0xc]
        xor    eax, eax
        out    dx, ax
        mov    edi, dword ptr [ebp - 0x14]
        add    edi, 8
        mov    edx, edi
        in     ax, dx
        mov    bx, ax
        mov    si, ax
        and    ebx, 0xff
        cmp    ebx, 0xff
        jne    loc_5_1761
        inc    ebx
loc_5_1761:
        shl    ebx, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, 0x20
        out    dx, ax
        mov    edx, dword ptr [ebp - 0xc]
        xor    eax, eax
        and    esi, 0xff00
        mov    dword ptr [ecx + 0x83], ebx
        out    dx, ax
        shr    ebx, 0xc
        mov    edx, edi
        in     ax, dx
        and    eax, 0xff00
        mov    byte ptr [ecx + 0xa3], bl
        sub    esi, eax
        cmp    bl, 1
        mov    dword ptr [ecx + 0x87], esi
        jbe    loc_5_17B7
        dec    bl
        mov    byte ptr [ecx + 0xa3], bl
        jmp    loc_5_17C3
loc_5_17B7:
        jae    loc_5_17C3
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xa3], 1
loc_5_17C3:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x81]
        and    eax, 6
        cmp    eax, 3
        ja     loc_5_1806
        jmp    dword ptr [eax*4]             ; -> obj5:0x18F4
loc_5_17DC:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x76], 9
        jmp    loc_5_1806
loc_5_17E7:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x76], 3
        jmp    loc_5_1806
loc_5_17F2:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x76], 0xa
        jmp    loc_5_1806
loc_5_17FD:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x76], 0xb
loc_5_1806:
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x14]
        add    ecx, 0x8b
        add    edx, 6
        mov    esi, ecx
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], edx
        add    ecx, 0xc
        mov    dword ptr [ebp - 8], ecx
loc_5_182D:
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x1c]
        add    eax, 0x20
        out    dx, ax
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 1
        out    dx, ax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 6
        out    dx, ax
        xor    ebx, ebx
loc_5_1858:
        mov    edx, dword ptr [ebp - 8]
        in     ax, dx
        test   ax, 3
        mov    di, ax
        je     loc_5_187D
        push   0x2710
        inc    ebx
        call   sub_1_0500
loc_5_1871:
        cmp    ebx, 3
        jl     loc_5_1858
        test   di, 3
        jne    loc_5_1887
loc_5_187D:
        mov    edx, dword ptr [ebp - 0x18]
        in     ax, dx
        mov    word ptr [esi], ax
        jmp    loc_5_1890
loc_5_1887:
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [esi], 0
loc_5_1890:
        add    esi, 2
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x10], 3
        jl     loc_5_182D
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x49], 0
        jne    loc_5_18BA
        mov    eax, dword ptr [ebp - 0x20]
        lea    edx, [ecx + 0x91]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
loc_5_18BA:
        cmp    byte ptr [ebp - 1], 0
        je     loc_5_18D6
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   0xaa000009
        mov    eax, dword ptr [ecx + 8]
        push   eax
        call   sub_1_05D8
loc_5_18D3:
        add    esp, 0xc
loc_5_18D6:
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        add    edx, 0xd
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (4 entries) ----
        dd  loc_5_17DC
        dd  loc_5_17E7
        dd  loc_5_17F2
        dd  loc_5_17FD
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1910:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 2
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x78]
        lea    edx, [esi + 0xe]
        out    dx, ax
        lea    edx, [esi + 0xd]
        xor    eax, eax
        out    dx, ax
        lea    edx, [esi + 0xe]
        mov    ebx, 0x10
        out    dx, ax
        lea    edx, [esi + 4]
        mov    eax, 0x8000
        out    dx, ax
loc_5_1941:
        push   0x2710
        call   sub_1_0500
loc_5_194B:
        dec    ebx
        jne    loc_5_1941
        lea    edx, [esi + 4]
        xor    eax, eax
        out    dx, ax
        push   0x2710
        call   sub_1_0500
loc_5_195F:
        lea    ecx, [edi + 0x91]
        lea    edx, [esi + 0xe]
        mov    eax, 1
        add    ecx, 2
        out    dx, ax
        mov    ax, word ptr [ecx - 2]
        lea    edx, [esi + 4]
        out    dx, ax
        mov    ax, word ptr [ecx]
        lea    edx, [esi + 6]
        out    dx, ax
        mov    ax, word ptr [ecx + 2]
        lea    edx, [esi + 8]
        out    dx, ax
        mov    ax, word ptr [edi + 0x81]
        mov    edx, esi
        out    dx, ax
        lea    edx, [esi + 0xe]
        mov    eax, 3
        out    dx, ax
        xor    ecx, ecx
loc_5_19A3:
        lea    edx, [ecx + esi]
        mov    al, byte ptr [edi + ecx + 0x97]
        out    dx, al
        inc    ecx
        cmp    ecx, 6
        jl     loc_5_19A3
        lea    edx, [esi + 0xe]
        mov    eax, 1
        out    dx, ax
        lea    edx, [esi + 0xc]
        mov    eax, 0x20
        out    dx, ax
        lea    edx, [esi + 0xe]
        xor    eax, eax
        out    dx, ax
        mov    edx, esi
        mov    eax, 0x1481
        out    dx, ax
        lea    edx, [esi + 4]
        mov    eax, 0x300
        out    dx, ax
        lea    edx, [esi + 0xa]
        xor    eax, eax
        out    dx, ax
        lea    edx, [esi + 0xe]
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0xd]
        mov    al, 0x33
        out    dx, al
        push   edi
        call   sub_5_1A10
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_1A10:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], edx
        lea    eax, [ebp - 8]
        mov    byte ptr [ecx + 0x42], dl
        mov    byte ptr [ecx + 0x44], dl
        mov    byte ptr [ecx + 0x43], dl
        mov    byte ptr [ecx + 0x41], dl
        lea    esi, [ecx + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx + 0xa8], edx
        mov    dword ptr [ecx + 0xa4], edx
        cmp    dword ptr [esi], edx
        je     loc_5_1A65
loc_5_1A44:
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        cmp    dword ptr [edx], 0
        jne    loc_5_1A52
        mov    dword ptr [ecx + 0x28], esi
loc_5_1A52:
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [eax], edx
        cmp    dword ptr [esi], 0
        jne    loc_5_1A44
loc_5_1A65:
        lea    edx, [ecx + 0x1c]
        cmp    dword ptr [edx], 0
        je     loc_5_1A8E
loc_5_1A6D:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    loc_5_1A7B
        mov    dword ptr [ecx + 0x20], edx
loc_5_1A7B:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    loc_5_1A6D
loc_5_1A8E:
        lea    edx, [ecx + 0x14]
        cmp    dword ptr [edx], 0
        je     loc_5_1AB7
loc_5_1A96:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    loc_5_1AA4
        mov    dword ptr [ecx + 0x18], edx
loc_5_1AA4:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    loc_5_1A96
loc_5_1AB7:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_1AEB
        xor    esi, esi
loc_5_1ABF:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 8], edx
        cmp    dword ptr [ecx], esi
        jne    loc_5_1AD1
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_5_1AD1:
        push   0xc001000c
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 0x18]
        push   edx
        call   sub_1_044A
loc_5_1AE6:
        cmp    dword ptr [ebp - 8], esi
        jne    loc_5_1ABF
loc_5_1AEB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x41], 0
        jne    loc_5_1C15
        mov    edi, dword ptr [esi + 0x14]
        test   edi, edi
        je     loc_5_1C15
        mov    ecx, dword ptr [esi + 0xa4]
        xor    eax, eax
        mov    al, byte ptr [esi + 0xa3]
        cmp    eax, ecx
        jbe    loc_5_1C15
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [ebp - 0xc], eax
        add    eax, 0xe
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [ebp - 0xc]
        add    edx, 8
        in     ax, dx
        mov    edx, dword ptr [ebp - 8]
        cmp    ah, 8
        jae    loc_5_1B75
        xor    eax, eax
        pop    edi
        mov    ax, word ptr [ebp - 2]
        pop    esi
        out    dx, ax
        xor    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1B75:
        mov    eax, 2
        inc    ecx
        mov    byte ptr [esi + 0x44], 1
        mov    byte ptr [esi + 0x41], 1
        out    dx, ax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [esi + 0xa4], ecx
        push   eax
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_0530
loc_5_1B9E:
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        add    ecx, 6
        mov    ebx, 0xa
        shr    ecx, 8
        or     cl, 0x20
        mov    ax, cx
        out    dx, ax
        add    edx, 0xc
loc_5_1BBA:
        in     ax, dx
        test   al, 8
        jne    loc_5_1BC3
        dec    ebx
        jne    loc_5_1BBA
loc_5_1BC3:
        test   ebx, ebx
        je     loc_5_1BF3
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0xc]
        add    edx, 3
        mov    dword ptr [eax], edi
        in     al, dx
        mov    byte ptr [edi + 0xc], al
        mov    eax, dword ptr [esi + 0x14]
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi + 0x14], edi
        cmp    dword ptr [eax], 0
        jne    loc_5_1BE9
        lea    eax, [esi + 0x14]
        mov    dword ptr [esi + 0x18], eax
loc_5_1BE9:
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x41], 0
        jmp    loc_5_1BFF
loc_5_1BF3:
        cmp    byte ptr [esi + 0x43], 0
        jne    loc_5_1BFF
        push   esi
        call   sub_5_2810
loc_5_1BFF:
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        pop    edi
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1C15:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [esi + 0x42], 0
        jne    loc_5_1DB4
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x42], 1
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [esi + 0x28], edi
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [esi + 0x78]
        mov    dword ptr [ebp - 0x20], ecx
        lea    ebx, [ecx + 8]
        add    ecx, 0xe
        mov    edx, ecx
        mov    dword ptr [ebp - 0x2c], ecx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, ecx
        mov    eax, 2
        mov    ecx, dword ptr [ebp - 0x20]
        out    dx, ax
        add    ecx, 6
        mov    edx, ecx
        mov    dword ptr [ebp - 0x24], ecx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp - 0x20]
        add    ecx, 2
        mov    edx, ecx
        mov    dword ptr [ebp - 0x28], ecx
        in     al, dx
        mov    byte ptr [ebp - 2], al
loc_5_1C8F:
        mov    edx, dword ptr [ebp - 0x28]
        mov    al, byte ptr [edi + 0xc]
        out    dx, al
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, 0x4000
        out    dx, ax
        lea    eax, [ebp - 0x1c]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   0
        push   0
        push   edx
        call   sub_1_0530
loc_5_1CB5:
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        mov    ecx, dword ptr [ebp - 0x1c]
        add    cx, 6
        mov    ax, cx
        cmp    dword ptr [ebp - 0x14], 0
        out    dx, ax
        je     loc_5_1D39
loc_5_1CCD:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_0554
loc_5_1CDE:
        cmp    dword ptr [ebp - 0xc], 7
        jbe    loc_5_1D09
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        and    eax, 0x7ffc
        shr    eax, 2
        push   eax
        push   ecx
        push   ebx
        call   sub_1_03C9
loc_5_1CFA:
        mov    eax, dword ptr [ebp - 0xc]
        and    dword ptr [ebp - 0xc], 3
        and    eax, 0x7ffc
        add    dword ptr [ebp - 0x10], eax
loc_5_1D09:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_1D22
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        and    eax, 0x7fff
        push   eax
        push   ecx
        push   ebx
        call   sub_1_03A0
loc_5_1D22:
        lea    eax, [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_0578
loc_5_1D2F:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], ecx
        test   ecx, ecx
        jne    loc_5_1CCD
loc_5_1D39:
        test   byte ptr [ebp - 0x1c], 1
        mov    edx, ebx
        je     loc_5_1D46
        mov    al, 0x30
        out    dx, al
        jmp    loc_5_1D4D
loc_5_1D46:
        mov    eax, 0x10
        out    dx, ax
loc_5_1D4D:
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, 0xc0
        out    dx, ax
        mov    eax, dword ptr [esi + 0x1c]
        test   eax, eax
        jne    loc_5_1D68
        mov    byte ptr [esi + 0x42], 0
        mov    byte ptr [ebp - 1], 0
        jmp    loc_5_1D88
loc_5_1D68:
        mov    edi, eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x1c], eax
        cmp    dword ptr [edi], 0
        jne    loc_5_1D7A
        lea    eax, [esi + 0x1c]
        mov    dword ptr [esi + 0x20], eax
loc_5_1D7A:
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [esi + 0x28], edi
        mov    dword ptr [eax], edi
loc_5_1D88:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_5_1C8F
        mov    edx, dword ptr [ebp - 0x28]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        mov    edx, dword ptr [ebp - 0x24]
        out    dx, ax
        mov    ax, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x2c]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    8
loc_5_1DB4:
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [eax], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_5_1DD0:
        push   ebp
        mov    eax, 0xffffffff
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 4], 6
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   edi
loc_5_1DE7:
        mov    si, 8
        mov    bl, byte ptr [ecx]
        inc    ecx
loc_5_1DEE:
        mov    edx, eax
        and    edx, 0x80000000
        cmp    edx, 1
        mov    dl, bl
        sbb    edi, edi
        add    eax, eax
        and    dx, 1
        inc    edi
        shr    bl, 1
        xor    di, dx
        test   di, di
        je     loc_5_1E1E
        xor    eax, 0x4c11db6
        mov    edx, edi
        and    edx, 0xffff
        or     eax, edx
loc_5_1E1E:
        dec    si
        jne    loc_5_1DEE
        dec    word ptr [ebp - 4]
        jne    loc_5_1DE7
        shr    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E40:
        push   ebp
        xor    dl, dl
        mov    ebp, esp
        sub    esp, 4
        mov    cx, word ptr [ebp + 0xc]
        push   ebx
        mov    ax, cx
        push   esi
        shr    ax, 3
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, eax
        and    esi, 0xffff
        mov    al, 1
        mov    ebx, dword ptr [edi + 0x78]
        and    cl, 7
        shl    al, cl
        lea    edi, [esi + edi + 0x97]
        cmp    byte ptr [ebp + 0x10], dl
        mov    cl, byte ptr [edi]
        je     loc_5_1E85
        test   al, cl
        jne    loc_5_1E7F
        mov    dl, 1
loc_5_1E7F:
        or     cl, al
        mov    byte ptr [edi], cl
        jmp    loc_5_1E91
loc_5_1E85:
        test   al, cl
        je     loc_5_1E8B
        mov    dl, 1
loc_5_1E8B:
        not    al
        and    al, cl
        mov    byte ptr [edi], al
loc_5_1E91:
        test   dl, dl
        je     loc_5_1EB5
        lea    ecx, [ebx + 0xe]
        mov    edx, ecx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    eax, 3
        out    dx, ax
        lea    edx, [esi + ebx]
        mov    al, byte ptr [edi]
        out    dx, al
        mov    ax, word ptr [ebp - 2]
        mov    edx, ecx
        out    dx, ax
loc_5_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_5_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebp - 0xc], edi
        cmp    eax, edi
        je     loc_5_1EF6
        cmp    eax, 1
        je     loc_5_2131
        cmp    eax, 2
        je     loc_5_2274
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_5_26DD
loc_5_1EF6:
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010101
        ja     loc_5_1F17
        je     loc_5_20AE
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_5_1F38
        jmp    dword ptr [eax*4]             ; -> obj5:0x2718
loc_5_1F17:
        cmp    eax, 0x1010102
        je     loc_5_20C1
        cmp    eax, 0x1010103
        je     loc_5_20D4
        cmp    eax, 0x1010104
        je     loc_5_211D
loc_5_1F38:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        xor    edi, edi
        jmp    loc_5_26DD
loc_5_1F46:
        mov    ecx, 0                        ; -> obj4:0x00DC
        mov    edi, 0x5c
        jmp    loc_5_26E0
loc_5_1F55:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + 0x6c]
        lea    ecx, [ebp - 8]
        sub    ax, 2
        mov    edi, 4
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_1F79:
        mov    ecx, 0                        ; -> obj1:0x0730
        mov    edi, 8
        jmp    loc_5_26E0
loc_5_1F88:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, 4
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 8]
        jmp    loc_5_26E0
loc_5_1F9E:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_5_26E0
loc_5_1FB2:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_5_26E0
loc_5_1FC6:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x83]
        lea    ecx, [ebp - 8]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_1FE2:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x87]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_1FFB:
        push   3
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        lea    eax, [ebp - 0x14]
        push   ecx
        mov    edi, 4
        push   eax
        call   sub_1_05EC
loc_5_2015:
        mov    byte ptr [ebp - 0x11], 0
        lea    ecx, [ebp - 0x14]
        jmp    loc_5_26E0
loc_5_2021:
        mov    ecx, 0                        ; -> loc_1_0738
        mov    edi, 0x39
        jmp    loc_5_26E0
loc_5_2030:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, 4
        mov    ecx, dword ptr [eax + 0x1c]
        mov    edx, dword ptr [ecx + 8]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_5_26E0
loc_5_2049:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x7d]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_205F:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_5_26E0
loc_5_2072:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_5_26E0
loc_5_2086:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_26E0
loc_5_209A:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 7
        jmp    loc_5_26E0
loc_5_20AE:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        jmp    loc_5_26E0
loc_5_20C1:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x91
        jmp    loc_5_26E0
loc_5_20D4:
        mov    eax, dword ptr [esi + 0x18]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [esi + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0x10]
        push   edx
        mov    ecx, dword ptr [eax + 0x1c]
        push   ecx
        mov    edx, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 0xc]
        mov    eax, dword ptr [edx + 0x72]
        push   eax
        push   ecx
        call   sub_2_06A0
loc_5_20F7:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_210F
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_26DD
loc_5_210F:
        mov    dword ptr [esi + 0x20], 0
        xor    edi, edi
        jmp    loc_5_26DD
loc_5_211D:
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x10
        jmp    loc_5_26E0
loc_5_2131:
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_5_215A
        je     loc_5_221E
        cmp    eax, 0x1010e
        je     loc_5_2166
        cmp    eax, 0x1010f
        je     loc_5_21B8
        cmp    eax, 0x10112
        je     loc_5_21EE
loc_5_215A:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_5_26DD
loc_5_2166:
        cmp    dword ptr [esi + 0x1c], 4
        jae    loc_5_217F
        mov    dword ptr [esi + 0x24], 4
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_5_26DD
loc_5_217F:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_05EC
loc_5_218E:
        push   1
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0x1c]
        mov    edx, dword ptr [ecx + 0x72]
        push   eax
        push   edx
        call   sub_2_05E0
loc_5_21A9:
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_26DD
loc_5_21B8:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_05EC
loc_5_21C7:
        cmp    dword ptr [ebp - 8], 0x5ea
        jbe    loc_5_21D7
        mov    dword ptr [ebp - 0xc], 0xc000009a
loc_5_21D7:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x7d], eax
        jae    loc_5_26DD
        mov    dword ptr [ecx + 0x7d], eax
        jmp    loc_5_26DD
loc_5_21EE:
        cmp    dword ptr [esi + 0x1c], 4
        jae    loc_5_2207
        mov    dword ptr [esi + 0x24], 4
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_5_26DD
loc_5_2207:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x10
        push   eax
        call   sub_1_05EC
loc_5_2219:
        jmp    loc_5_26DD
loc_5_221E:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_5_223F
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [esi + 0x20], ecx
        jmp    loc_5_26DD
loc_5_223F:
        push   1
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   0
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [edx + 0x72]
        push   eax
        push   ecx
        call   sub_2_0300
loc_5_2266:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [esi + 0x20], ecx
        jmp    loc_5_26DD
loc_5_2274:
        mov    edi, 4
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20101
        ja     loc_5_229A
        je     loc_5_24EF
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_5_2304
        jmp    dword ptr [eax*4]             ; -> obj5:0x2764
loc_5_229A:
        cmp    eax, 0x20201
        ja     loc_5_22B8
        je     loc_5_2535
        sub    eax, 0x20102
        cmp    eax, 3
        ja     loc_5_2304
        jmp    dword ptr [eax*4]             ; -> obj5:0x27B0
loc_5_22B8:
        cmp    eax, 0x1010101
        ja     loc_5_22D6
        je     loc_5_25F9
        sub    eax, 0x20202
        cmp    eax, 0xc
        ja     loc_5_2304
        jmp    dword ptr [eax*4]             ; -> obj5:0x27C0
loc_5_22D6:
        cmp    eax, 0x1020101
        ja     loc_5_2312
        je     loc_5_266F
        cmp    eax, 0x1010102
        je     loc_5_260C
        cmp    eax, 0x1010103
        je     loc_5_261F
        cmp    eax, 0x1010104
        je     loc_5_265E
loc_5_2304:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        xor    edi, edi
        jmp    loc_5_26DD
loc_5_2312:
        cmp    eax, 0x1020201
        ja     loc_5_2337
        je     loc_5_2690
        cmp    eax, 0x1020102
        je     loc_5_267A
        cmp    eax, 0x1020103
        je     loc_5_2685
        jmp    loc_5_2304
loc_5_2337:
        sub    eax, 0x1020202
        cmp    eax, 5
        ja     loc_5_2304
        jmp    dword ptr [eax*4]             ; -> obj5:0x27F4
loc_5_2348:
        mov    ecx, 0                        ; -> obj4:0x000C
        mov    edi, 0xd0
        jmp    loc_5_26E0
loc_5_2357:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + 0x6c]
        lea    ecx, [ebp - 8]
        sub    ax, 2
        mov    edi, 4
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_237B:
        mov    ecx, 0                        ; -> obj1:0x0730
        mov    edi, 8
        jmp    loc_5_26E0
loc_5_238A:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + 0x52]
        test   al, al
        je     loc_5_23C1
        cmp    byte ptr [ecx + 0x53], 0
        je     loc_5_23A9
        mov    ecx, 0                        ; -> obj1:0x0730
        mov    edi, 8
        jmp    loc_5_26E0
loc_5_23A9:
        test   al, al
        je     loc_5_23C1
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5
        jmp    loc_5_26E0
loc_5_23C1:
        cmp    byte ptr [ecx + 0x53], 0
        je     loc_5_26DD
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_26E0
loc_5_23DF:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_5_26E0
loc_5_23F3:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_5_26E0
loc_5_2407:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x83]
        lea    ecx, [ebp - 8]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_2423:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x87]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_243C:
        push   3
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        lea    eax, [ebp - 0x14]
        push   ecx
        mov    edi, 4
        push   eax
        call   sub_1_05EC
loc_5_2456:
        mov    byte ptr [ebp - 0x11], 0
        lea    ecx, [ebp - 0x14]
        jmp    loc_5_26E0
loc_5_2462:
        mov    ecx, 0                        ; -> loc_1_0738
        mov    edi, 0x39
        jmp    loc_5_26E0
loc_5_2471:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x72]
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_5_26E0
loc_5_248A:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x7d]
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_26E0
loc_5_24A0:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_5_26E0
loc_5_24B3:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_5_26E0
loc_5_24C7:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_26E0
loc_5_24DB:
        mov    edi, 4
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 7
        jmp    loc_5_26E0
loc_5_24EF:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xac
        jmp    loc_5_26E0
loc_5_24FD:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb0
        jmp    loc_5_26E0
loc_5_250B:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb4
        jmp    loc_5_26E0
loc_5_2519:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb8
        jmp    loc_5_26E0
loc_5_2527:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xbc
        jmp    loc_5_26E0
loc_5_2535:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc0
        jmp    loc_5_26E0
loc_5_2543:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc4
        jmp    loc_5_26E0
loc_5_2551:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc8
        jmp    loc_5_26E0
loc_5_255F:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xcc
        jmp    loc_5_26E0
loc_5_256D:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd0
        jmp    loc_5_26E0
loc_5_257B:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd4
        jmp    loc_5_26E0
loc_5_2589:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd8
        jmp    loc_5_26E0
loc_5_2597:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xdc
        jmp    loc_5_26E0
loc_5_25A5:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe0
        jmp    loc_5_26E0
loc_5_25B3:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe4
        jmp    loc_5_26E0
loc_5_25C1:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe8
        jmp    loc_5_26E0
loc_5_25CF:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xec
        jmp    loc_5_26E0
loc_5_25DD:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf0
        jmp    loc_5_26E0
loc_5_25EB:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xa8
        jmp    loc_5_26E0
loc_5_25F9:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        jmp    loc_5_26E0
loc_5_260C:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x91
        jmp    loc_5_26E0
loc_5_261F:
        mov    eax, dword ptr [esi + 0x18]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [esi + 0x1c]
        push   ecx
        mov    ebx, dword ptr [ebp + 0xc]
        push   edx
        lea    ecx, [ebp - 0xc]
        mov    eax, dword ptr [ebx + 0x72]
        push   eax
        push   ecx
        call   sub_2_0760
loc_5_263B:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_2653
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_26DD
loc_5_2653:
        mov    dword ptr [esi + 0x20], 0
        xor    edi, edi
        jmp    loc_5_26DD
loc_5_265E:
        lea    ecx, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0x10
        jmp    loc_5_26E0
loc_5_266F:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf4
        jmp    loc_5_26E0
loc_5_267A:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf8
        jmp    loc_5_26E0
loc_5_2685:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xfc
        jmp    loc_5_26E0
loc_5_2690:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x108
        jmp    loc_5_26E0
loc_5_269B:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x100
        jmp    loc_5_26E0
loc_5_26A6:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xbc
        jmp    loc_5_26E0
loc_5_26B1:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x10c
        jmp    loc_5_26E0
loc_5_26BC:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x110
        jmp    loc_5_26E0
loc_5_26C7:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x114
        jmp    loc_5_26E0
loc_5_26D2:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x104
        jmp    loc_5_26E0
loc_5_26DD:
        mov    ecx, dword ptr [ebp - 0x10]
loc_5_26E0:
        test   edi, edi
        je     loc_5_270A
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, edi
        jae    loc_5_26F7
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [esi + 0x24], edi
        mov    edi, eax
loc_5_26F7:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x20], edi
        test   edi, edi
        je     loc_5_270A
loc_5_2701:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [eax], dl
        inc    eax
        dec    edi
        jne    loc_5_2701
loc_5_270A:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
        ; ---- dispatch table (61 entries) ----
        dd  loc_5_1F46
        dd  loc_5_1F55
        dd  loc_5_1F79
        dd  loc_5_1F88
        dd  loc_5_1F9E
        dd  loc_5_1F9E
        dd  loc_5_1FB2
        dd  loc_5_1FC6
        dd  loc_5_1FC6
        dd  loc_5_1FE2
        dd  loc_5_1FE2
        dd  loc_5_1FFB
        dd  loc_5_2021
        dd  loc_5_2030
        dd  loc_5_2049
        dd  loc_5_205F
        dd  loc_5_2072
        dd  loc_5_2086
        dd  loc_5_209A
        dd  loc_5_2348
        dd  loc_5_2357
        dd  loc_5_237B
        dd  loc_5_238A
        dd  loc_5_23DF
        dd  loc_5_23DF
        dd  loc_5_23F3
        dd  loc_5_2407
        dd  loc_5_2407
        dd  loc_5_2423
        dd  loc_5_2423
        dd  loc_5_243C
        dd  loc_5_2462
        dd  loc_5_2471
        dd  loc_5_248A
        dd  loc_5_24A0
        dd  loc_5_24B3
        dd  loc_5_24C7
        dd  loc_5_24DB
        dd  loc_5_24FD
        dd  loc_5_250B
        dd  loc_5_2519
        dd  loc_5_2527
        dd  loc_5_2543
        dd  loc_5_2551
        dd  loc_5_255F
        dd  loc_5_256D
        dd  loc_5_257B
        dd  loc_5_2589
        dd  loc_5_2597
        dd  loc_5_25A5
        dd  loc_5_25B3
        dd  loc_5_25C1
        dd  loc_5_25CF
        dd  loc_5_25DD
        dd  loc_5_25EB
        dd  loc_5_269B
        dd  loc_5_26A6
        dd  loc_5_26B1
        dd  loc_5_26BC
        dd  loc_5_26C7
        dd  loc_5_26D2
        db  08Dh, 064h, 024h, 000h
sub_5_2810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x78]
        lea    ecx, [ebx + 0xe]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        lea    edx, [ebx + 0xd]
        in     al, dx
        mov    bl, al
        xor    al, al
        out    dx, al
        or     bl, 8
        mov    byte ptr [esi + 0x44], 0
        mov    al, bl
        out    dx, al
        mov    edx, ecx
        mov    eax, edi
        and    eax, 0xffff
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_2850:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x40], 0
        je     loc_5_288E
        cmp    byte ptr [esi + 0x43], 0
        jne    loc_5_2878
        cmp    byte ptr [esi + 0x47], 0
        jne    loc_5_2878
        push   esi
        lea    eax, [esi + 0x54]
        push   0                             ; -> loc_5_28A0
        push   eax
        call   sub_1_04FA
loc_5_2878:
        push   0x1388
        lea    eax, [esi + 0x34]
        mov    byte ptr [esi + 0x47], 0
        push   eax
        call   sub_1_04E2
loc_5_288A:
        mov    byte ptr [esi + 0x47], 0
loc_5_288E:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_28A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x78]
        lea    ecx, [esi + 0xe]
        mov    edx, ecx
        in     ax, dx
        mov    di, ax
        mov    eax, 2
        out    dx, ax
        lea    edx, [esi + 0xd]
        in     al, dx
        mov    bl, al
        xor    al, al
        out    dx, al
        mov    al, bl
        out    dx, al
        mov    edx, ecx
        mov    eax, edi
        and    eax, 0xffff
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
VxD__LTEXT ENDS
