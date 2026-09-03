; NICE -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5262 instructions recovered by recursive descent

Declare_Virtual_Device NICE, 3, 10, NICE_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3510  R-X
loc_1_0000:
        mov    al, byte ptr [0x698]          ; -> obj1:0x0698
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x698]              ; -> obj1:0x0698
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_17E0
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
NICE_Control:
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
        db  'NICE    '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  NICE_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, 0x32e8                   ; -> loc_1_32E8
        mov    eax, dword ptr [edx + 0x38]
        cmp    dword ptr [edx + 0x28], 2
        je     loc_1_06C0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_06C0:
        lea    esi, [eax + 0x14]
        cmp    byte ptr [0x32e8], 0          ; -> loc_1_32E8
        je     loc_1_06DD
loc_1_06CC:
        mov    ecx, edi
        inc    edi
        mov    bl, byte ptr [esi]
        cmp    byte ptr [ecx], bl
        jne    loc_1_06F3
        add    esi, 2
        cmp    byte ptr [edi], 0
        jne    loc_1_06CC
loc_1_06DD:
        mov    byte ptr [eax + 0x806], 0xc4
        test   byte ptr [eax + 0x806], 2
        je     loc_1_0700
        mov    byte ptr [edx + 0x40], 0
        jmp    loc_1_070B
loc_1_06F3:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0700:
        mov    byte ptr [edx + 0x40], 1
        mov    byte ptr [eax + 0x806], 0x45
loc_1_070B:
        push   0x30d40
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0720:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x28]
        cmp    eax, 1
        jne    loc_1_074B
        sub    ecx, ecx
loc_1_0731:
        mov    edx, dword ptr [esi + 0x30]
        add    dx, cx
        add    dx, 0x1a
        in     al, dx
        mov    byte ptr [esi + ecx + 0x4c], al
        inc    ecx
        cmp    ecx, 6
        jl     loc_1_0731
        pop    esi
        pop    ebp
        ret    4
loc_1_074B:
        cmp    eax, 2
        jne    loc_1_0766
        sub    eax, eax
loc_1_0752:
        mov    ecx, dword ptr [esi + 0x38]
        inc    eax
        mov    cl, byte ptr [ecx + eax*2 + 0xcc]
        mov    byte ptr [esi + eax + 0x4b], cl
        cmp    eax, 6
        jl     loc_1_0752
loc_1_0766:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0770:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x41], 0x52
        cmp    dword ptr [ecx + 0x28], 2
        jne    loc_1_0794
        cmp    byte ptr [ecx + 0x40], 0
        je     loc_1_0794
        lea    edi, [ecx + 0x42]
        mov    byte ptr [edi], 0x20
        jmp    loc_1_079A
loc_1_0794:
        lea    edi, [ecx + 0x42]
        mov    byte ptr [edi], 0xa0
loc_1_079A:
        mov    si, word ptr [ecx + 0x30]
        mov    al, byte ptr [ecx + 0x41]
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
        mov    dx, word ptr [ecx + 0x30]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 0x30]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 4]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        sub    bl, bl
loc_1_07D9:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x52]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     loc_1_07D9
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 0x7a]
        test   al, 0x20
        jne    loc_1_082B
        test   al, 4
        je     loc_1_082B
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0810:
        mov    dl, bl
        and    edx, 0xffff00ff
        add    dx, si
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_0810
        jmp    loc_1_084F
loc_1_082B:
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0830:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x44]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_0830
loc_1_084F:
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
        lea    edx, [esi + 6]
        mov    al, byte ptr [ecx + 0x41]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0880:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0xff
        je     loc_1_089D
        or     al, al
        jne    loc_1_08D4
loc_1_089D:
        mov    edx, dword ptr [esi + 0x30]
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
        jne    loc_1_08D2
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_08D2:
        sub    al, al
loc_1_08D4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_08E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 2
        mov    si, word ptr [ecx + 0x30]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 0x43]
        or     al, 4
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 0x81
        out    dx, al
        cmp    dword ptr [ecx + 0x28], 1
        jne    loc_1_0912
        lea    edx, [esi + 0x10]
        mov    al, 0x1d
        out    dx, al
loc_1_0912:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0920:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cx, word ptr [eax + 0x30]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x28], 1
        jne    loc_1_095C
        mov    edx, dword ptr [ebx + 0x30]
        add    dx, 0x10
        mov    al, 0xf
        out    dx, al
loc_1_095C:
        mov    edx, dword ptr [ebx + 0x30]
        add    dx, 5
        mov    al, 6
        out    dx, al
        mov    esi, 0x32e0                   ; -> loc_1_32E0
        add    eax, 0
loc_1_0970:
        mov    ecx, dword ptr [ebx + 0x30]
        mov    ax, 0x3d
        lea    edx, [ecx + 8]
        out    dx, ax
        push   0x1f
        mov    eax, dword ptr [esi]
        push   eax
        add    ecx, 8
        push   ecx
        call   sub_1_03B4
        add    esi, 4
        cmp    esi, 0x32e8                   ; -> loc_1_32E8
        jb     loc_1_0970
        mov    edx, dword ptr [ebx + 0x30]
        add    dx, 0xa
        mov    al, 0x82
        out    dx, al
        push   0xfa
        call   sub_1_0500
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 8], 0x32e0   ; -> loc_1_32E0
loc_1_09B7:
        mov    edx, dword ptr [ebx + 0x30]
        add    dx, 5
        in     al, dx
        test   al, 0x40
        jne    loc_1_0A15
        push   2
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebx + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        cmp    word ptr [ebp - 0xa], 0x3d
        jne    loc_1_0A15
        push   0x1f
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebx + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        lea    edi, [ebp - 0x50]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 0xf
        mov    esi, dword ptr [eax]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0A15
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    loc_1_0A15
        add    dword ptr [ebp - 8], 4
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 8], 0x32e8   ; -> loc_1_32E8
        jb     loc_1_09B7
loc_1_0A15:
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [ebx + 0x30]
        in     al, dx
        mov    dx, word ptr [ebx + 0x30]
        out    dx, al
        inc    dx
        cmp    dword ptr [ebx + 0x28], 1
        mov    si, word ptr [ebx + 0x30]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        jne    loc_1_0A49
        lea    edx, [esi + 0x10]
        mov    al, 0xd
        out    dx, al
loc_1_0A49:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0920
        push   esi
        call   sub_1_0770
        push   esi
        call   sub_1_08E0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_0ACB
loc_1_0A92:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0AA0:
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
        je     loc_1_0AC2
        xor    eax, 0x4c11db7
loc_1_0AC2:
        dec    esi
        jne    loc_1_0AA0
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_1_0A92
loc_1_0ACB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AE0:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0A80
        sub    ecx, ecx
        mov    edx, 1
loc_1_0AF5:
        add    ecx, ecx
        test   eax, edx
        je     loc_1_0AFE
        or     ecx, 1
loc_1_0AFE:
        add    edx, edx
        cmp    edx, 0x40
        jb     loc_1_0AF5
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
sub_1_0B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0B39:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x44], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_0B39
        push   0x3200                        ; -> obj1:0x3200
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_0B97
        sub    edi, edi
loc_1_0B68:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x3200]         ; -> obj1:0x3200
        push   eax
        call   sub_1_0AE0
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x44], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_0B68
loc_1_0B97:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0BA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x30]
        mov    al, byte ptr [esi + 0x41]
        or     al, 0x80
        lea    edx, [edi + 6]
        out    dx, al
        lea    ecx, [edi + 7]
        mov    al, byte ptr [esi + 0x42]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0BC4:
        lea    edx, [edi + ebx]
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0BC4
        mov    al, byte ptr [esi + 0x42]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 0x41]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0BF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    si, word ptr [edi + 0x30]
        mov    al, byte ptr [edi + 0x41]
        or     al, 0x80
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 0x42]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0C14:
        lea    edx, [ebx + esi]
        add    dx, 8
        mov    al, byte ptr [edi + ebx + 0x44]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0C14
        mov    al, byte ptr [edi + 0x42]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 0x41]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0C40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x30]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_0D92
        mov    byte ptr [esi + 0x6c5], 1
        inc    dword ptr [esi + 0x60]
        sub    edi, edi
        sub    ebx, ebx
loc_1_0C87:
        mov    dx, word ptr [esi + 0x30]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x30]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    loc_1_0CA1
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_0D00
loc_1_0CA1:
        test   byte ptr [ebp - 2], 0x81
        je     loc_1_0CBF
        test   byte ptr [ebp - 2], 1
        je     loc_1_0CB3
        inc    dword ptr [esi + 0x69c]
loc_1_0CB3:
        push   esi
        call   sub_1_0DA0
        push   esi
        call   sub_1_15E0
loc_1_0CBF:
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_0CCA
        cmp    byte ptr [esi + 0x78], bl
        je     loc_1_0CFA
loc_1_0CCA:
        mov    dx, word ptr [esi + 0x30]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 0x64], ebx
        cmp    byte ptr [esi + 0x78], bl
        je     loc_1_0CE3
        mov    byte ptr [esi + 0x79], 2
        mov    byte ptr [esi + 0x78], bl
        jmp    loc_1_0CE9
loc_1_0CE3:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x79], al
loc_1_0CE9:
        mov    byte ptr [esi + 0x6ea], bl
        cmp    dword ptr [esi + 0x74], ebx
        je     loc_1_0CFA
        push   esi
        call   sub_1_0F90
loc_1_0CFA:
        inc    edi
        cmp    edi, 8
        jl     loc_1_0C87
loc_1_0D00:
        cmp    dword ptr [esi + 0x6ad], 0
        je     loc_1_0D28
        lea    esp, [esp]
loc_1_0D10:
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_0D28
        push   esi
        call   sub_1_0DA0
        cmp    dword ptr [esi + 0x6ad], 0
        jne    loc_1_0D10
loc_1_0D28:
        mov    cx, word ptr [esi + 0x30]
        mov    byte ptr [esi + 0x6c5], 0
        lea    edx, [ecx + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 0x81
        out    dx, al
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_0D77
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_0D60
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_0D60
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_0D77
loc_1_0D60:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0D77:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_0D92
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_0D92
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_0D92
        push   esi
        call   sub_1_15E0
loc_1_0D92:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        mov    esi, dword ptr [ebp + 8]
loc_1_0DB4:
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_0F00
        mov    cx, word ptr [esi + 0x30]
        mov    al, 0x81
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 0x40
        jne    loc_1_0F00
        inc    dword ptr [esi + 0x64]
        mov    dword ptr [esi + 0x6b5], 0
        push   2
        lea    eax, [esi + 0x680]
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 0x682]
        mov    word ptr [esi + 0x7e], ax
        mov    edi, dword ptr [esi + 0x6a9]
        add    di, 0xe
        cmp    di, ax
        jbe    loc_1_0E16
        mov    di, ax
loc_1_0E16:
        mov    bl, 1
        test   byte ptr [esi + 0x7a], 0x28
        jne    loc_1_0E76
        push   3
        lea    eax, [esi + 0x80]
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0x7e], 6
        sub    di, 6
        cmp    dword ptr [esi + 0x82], -1
        jne    loc_1_0E60
        cmp    byte ptr [esi + 0x80], 0xff
        jne    loc_1_0E60
        cmp    byte ptr [esi + 0x81], 0xff
        jne    loc_1_0E60
        sub    bl, bl
        jmp    loc_1_0E9B
        db  005h, 000h, 000h, 000h, 000h
loc_1_0E60:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0x86]
        jmp    loc_1_0E8A
loc_1_0E76:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0x80]
loc_1_0E8A:
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0x7e], di
loc_1_0E9B:
        or     bl, bl
        je     loc_1_0EB4
        push   esi
        call   sub_1_1230
        cmp    eax, 3
        je     loc_1_0EB4
        inc    dword ptr [esi + 0x68c]
        mov    byte ptr [ebp - 2], 1
loc_1_0EB4:
        mov    ax, word ptr [esi + 0x7e]
        or     ax, ax
        je     loc_1_0EEB
        cmp    ax, 8
        ja     loc_1_0EE1
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    loc_1_0EEB
loc_1_0EE1:
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 0xe
        mov    al, 4
        out    dx, al
loc_1_0EEB:
        cmp    dword ptr [esi + 0x64], 5
        jbe    loc_1_0DB4
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 0x64], 0
loc_1_0F00:
        cmp    dword ptr [esi + 0x6ad], 0
        je     loc_1_0F52
        lea    esp, [esp]
loc_1_0F10:
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_0F5B
        mov    ebx, dword ptr [esi + 0x6ad]
        push   ebx
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x6ad], eax
        mov    dword ptr [esi + 0x6b5], ebx
        mov    edi, dword ptr [ebx + 0x28]
        push   esi
        call   sub_1_1090
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x6ad], 0
        jne    loc_1_0F10
loc_1_0F52:
        cmp    byte ptr [esi + 0x6a0], 0
        je     loc_1_0F67
loc_1_0F5B:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F67:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_0F76
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0290
loc_1_0F76:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x74]
        or     ecx, ecx
        jne    loc_1_0FC1
        push   2
        push   6
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FC1:
        mov    al, byte ptr [esi + 0x79]
        test   al, 0x80
        je     loc_1_1000
        test   al, 4
        je     loc_1_0FF6
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 4
        in     al, dx
        and    eax, 0xff
        and    eax, 0xfffffff0
        cmp    eax, 0x10
        jne    loc_1_0FF0
        inc    dword ptr [esi + 0x694]
        inc    dword ptr [esi + 0x688]
        jmp    loc_1_1006
        db  090h
loc_1_0FF0:
        inc    dword ptr [esi + 0x698]
loc_1_0FF6:
        inc    dword ptr [esi + 0x688]
        jmp    loc_1_1006
        db  08Bh, 0FFh
loc_1_1000:
        inc    dword ptr [esi + 0x690]
loc_1_1006:
        cmp    byte ptr [ecx + 0x2c], 0
        jne    loc_1_1030
        mov    al, byte ptr [esi + 0x79]
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
        jmp    loc_1_1057
        db  08Dh, 064h, 024h, 000h
loc_1_1030:
        cmp    dword ptr [esi + 0x6ad], 0
        jne    loc_1_1041
        mov    dword ptr [esi + 0x6ad], ecx
        jmp    loc_1_104A
loc_1_1041:
        mov    eax, dword ptr [esi + 0x6b1]
        mov    dword ptr [eax + 0x24], ecx
loc_1_104A:
        mov    dword ptr [esi + 0x6b1], ecx
        mov    dword ptr [ecx + 0x24], 0
loc_1_1057:
        mov    dword ptr [esi + 0x74], 0
        cmp    byte ptr [esi + 0x6a0], 0
        je     loc_1_1077
        push   2
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1077:
        push   esi
        call   sub_1_15E0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1090:
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
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_10C0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_10C0:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x6a9]
        add    edi, 0xe
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_10D4
        mov    edi, dword ptr [ebp - 4]
loc_1_10D4:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0x80]
        push   eax
        call   sub_1_1150
        cmp    edi, 0xe
        jae    loc_1_1110
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0x80]
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
loc_1_1110:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0x8e]
        push   eax
        push   0xe
        lea    eax, [esi + 0x80]
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
sub_1_1150:
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
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 0x10]
        je     loc_1_11A4
loc_1_1175:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_11A4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1175
loc_1_11A4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_11B5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_11B5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_11C4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_11D3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_11D6
loc_1_11D3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_11D6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_1212
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_121D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        jmp    loc_1_11C4
loc_1_1212:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_121D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1230:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 0x682]
        cmp    eax, 0x5ea
        jbe    loc_1_1250
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_1250:
        mov    dword ptr [edx + 0x684], eax
        mov    ecx, dword ptr [edx + 0x6a9]
        add    ecx, 0xe
        cmp    ecx, eax
        jbe    loc_1_1265
        mov    ecx, eax
loc_1_1265:
        cmp    ecx, 0xe
        jae    loc_1_1289
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0x80]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1289:
        sub    eax, 0xe
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    eax, [edx + 0x8e]
        push   eax
        push   0xe
        lea    eax, [edx + 0x80]
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
loc_1_12C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x6b5], 0
        je     loc_1_1367
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, edi
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_133A
loc_1_12FA:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x6b5]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_1150
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_133A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_12FA
loc_1_133A:
        cmp    edi, ebx
        jbe    loc_1_1355
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1355:
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
loc_1_1367:
        mov    ax, word ptr [esi + 0x7e]
        or     ax, ax
        je     loc_1_139C
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [esi + 0x6a9]
        add    eax, esi
        add    eax, 0x8e
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [esi + 0x7e], 0
loc_1_139C:
        mov    eax, dword ptr [esi + 0x684]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_13D0
        cmp    eax, edi
        jae    loc_1_13C7
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_13C7:
        sub    eax, edi
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_13D3
        db  08Bh, 0FFh
loc_1_13D0:
        mov    dword ptr [ebp - 0x18], ecx
loc_1_13D3:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0530
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_1454
loc_1_1403:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     loc_1_1410
        mov    ebx, dword ptr [ebp - 8]
loc_1_1410:
        push   ebx
        lea    eax, [edi + esi]
        add    eax, 0x80
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05EC
        add    edi, ebx
        sub    dword ptr [ebp - 0xc], ebx
        je     loc_1_1454
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1454
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1403
loc_1_1454:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1470:
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
        je     loc_1_14A0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_14A0:
        cmp    byte ptr [esi + 0x6a0], 0
        je     loc_1_14B7
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_14B7:
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [esi + 0x60]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], ebx
        lea    eax, [ebp - 0xc]
        push   0
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        call   sub_1_0530
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0554
        mov    edx, dword ptr [esi + 0x54]
        lea    ecx, [esi + 0x52]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    edx, eax
        jae    loc_1_1503
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1537
loc_1_1503:
        jbe    loc_1_1510
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_1537
        db  08Bh, 0FFh
loc_1_1510:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    loc_1_1527
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1537
loc_1_1527:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_1537
        mov    dword ptr [ebp - 4], 0
loc_1_1537:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1557
        test   byte ptr [ebx + 0x1d], 2
        je     loc_1_1545
        jmp    loc_1_1557
loc_1_1545:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    loc_1_155B
loc_1_1557:
        mov    byte ptr [edi + 8], 0
loc_1_155B:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_1570
        mov    eax, dword ptr [esi + 0x70]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_1576
        db  08Dh, 064h, 024h, 000h
loc_1_1570:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x6c], ecx
loc_1_1576:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x70], ecx
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_15B0
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_159D
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_159D
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_15B0
loc_1_159D:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        jmp    loc_1_15CB
        db  08Dh, 049h, 000h
loc_1_15B0:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_15CB
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_15CB
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_15CB
        push   esi
        call   sub_1_15E0
loc_1_15CB:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_176C
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
loc_1_15FC:
        mov    eax, dword ptr [esi + 0x6c]
        cmp    eax, ebx
        je     loc_1_176C
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x6c], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x74], eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 0x14], 0x3c
        mov    eax, 0x3c
        jb     loc_1_1637
        mov    eax, dword ptr [ebp - 0x14]
loc_1_1637:
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     loc_1_16DE
loc_1_164C:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_1664
        jmp    loc_1_16C8
loc_1_1664:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_1696
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        inc    dword ptr [ebp - 0x10]
        shl    ax, 8
        dec    dword ptr [ebp - 0xc]
        or     di, ax
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 8
        cmp    dword ptr [ebp - 0xc], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    loc_1_1696
        jmp    loc_1_16C8
loc_1_1696:
        cmp    dword ptr [ebp - 0xc], 1
        jbe    loc_1_16B2
        mov    eax, dword ptr [ebp - 0xc]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_16B2:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_16C8
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_16C8:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_164C
loc_1_16DE:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_16F7
        or     di, 0x2000
        mov    edx, dword ptr [esi + 0x30]
        inc    dword ptr [ebp - 0x14]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_16F7:
        cmp    dword ptr [ebp - 0x14], 0x3c
        jae    loc_1_1719
        mov    eax, 0x3c
        sub    eax, dword ptr [ebp - 0x14]
        shr    eax, 1
        push   eax
        push   0x32f0                        ; -> loc_1_32F0
        mov    eax, dword ptr [esi + 0x30]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_1719:
        test   byte ptr [esi + 0x7a], 0x20
        je     loc_1_1725
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [eax + 0x2c], bl
loc_1_1725:
        cmp    byte ptr [esi + 0x6a0], bl
        jne    loc_1_1749
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 0xa
        mov    al, 0x81
        out    dx, al
        cmp    dword ptr [esi + 0x74], ebx
        je     loc_1_15FC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1749:
        mov    eax, dword ptr [ebp - 0x18]
        push   0
        mov    edi, dword ptr [eax + 0x28]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0x74], 0
        push   3
        push   esi
        call   sub_1_2E10
        dec    dword ptr [edi + 0x18]
loc_1_176C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x6ea], 0
        je     loc_1_17B8
        cmp    dword ptr [esi + 0x74], 0
        je     loc_1_17C5
        mov    byte ptr [esi + 0x78], 1
        mov    byte ptr [esi + 0x6ea], 0
        push   7
        push   1
        push   0xaa000003
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 0x10
        jmp    loc_1_17C5
loc_1_17B8:
        cmp    dword ptr [esi + 0x74], 0
        je     loc_1_17C5
        mov    byte ptr [esi + 0x6ea], 1
loc_1_17C5:
        push   0x7d0
        add    esi, 0x6d2
        push   esi
        call   sub_1_04E2
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        mov    word ptr [ebp - 0x14], 5
        mov    word ptr [ebp - 0x12], 5
        mov    dword ptr [ebp - 0x10], 0x3410 ; -> loc_1_3410
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        mov    eax, dword ptr [0x332c]       ; -> obj1:0x332C
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_1_1831
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1831:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x4c], 3
        mov    byte ptr [ebp - 0x4b], 0
        mov    dword ptr [ebp - 0x4a], 0
        mov    dword ptr [ebp - 0x46], 0x1da0 ; -> loc_1_1DA0
        mov    dword ptr [ebp - 0x42], 0x1fa0 ; -> loc_1_1FA0
        mov    dword ptr [ebp - 0x3e], 0x1470 ; -> loc_1_1470
        mov    dword ptr [ebp - 0x3a], 0x12c0 ; -> loc_1_12C0
        mov    dword ptr [ebp - 0x36], 0x2b20 ; -> loc_1_2B20
        mov    dword ptr [ebp - 0x32], 0x2130 ; -> loc_1_2130
        mov    dword ptr [ebp - 0x2e], 0x29c0 ; -> loc_1_29C0
        mov    dword ptr [ebp - 0x2a], 0x2a90 ; -> loc_1_2A90
        mov    dword ptr [ebp - 0x26], 0x18e0 ; -> loc_1_18E0
        mov    dword ptr [ebp - 0x22], 0x2ac0 ; -> loc_1_2AC0
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x1e], ecx
        mov    dword ptr [ebp - 0x1a], eax
        lea    eax, [ebp - 0x4c]
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
        jne    loc_1_18C6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18C6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_18E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   esi
        push   edi
        mov    word ptr [ebp - 0x40], 0xc
        mov    word ptr [ebp - 0x3e], 0xc
        mov    dword ptr [ebp - 0x3c], 0x3424 ; -> obj1:0x3424
        mov    word ptr [ebp - 0x38], 0xe
        mov    word ptr [ebp - 0x36], 0xe
        mov    dword ptr [ebp - 0x34], 0x3440 ; -> obj1:0x3440
        mov    word ptr [ebp - 0x30], 0x12
        mov    word ptr [ebp - 0x2e], 0x12
        mov    dword ptr [ebp - 0x2c], 0x3464 ; -> obj1:0x3464
        mov    word ptr [ebp - 0x28], 0x10
        mov    word ptr [ebp - 0x26], 0x10
        mov    dword ptr [ebp - 0x24], 0x3488 ; -> obj1:0x3488
        mov    word ptr [ebp - 0x20], 0x15
        mov    word ptr [ebp - 0x1e], 0x15
        mov    dword ptr [ebp - 0x1c], 0x34b0 ; -> loc_1_34B0
        mov    eax, dword ptr [0x332c]       ; -> obj1:0x332C
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x6eb
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_1970
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_1970:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0x1ba
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp - 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0584
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1A64
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1A5B
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x28]
        cmp    eax, 1
        je     loc_1_19E3
        cmp    eax, 2
        jne    loc_1_1A5B
loc_1_19E3:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1A5B
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x28], 2
        jne    loc_1_1A3E
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1A5B
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x34], eax
loc_1_1A3E:
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1A81
loc_1_1A5B:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_1_1A64:
        push   0
        push   0x6eb
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1A81:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x3c], eax
        lea    eax, [ebp - 0x20]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1AB8
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x68], eax
        jmp    loc_1_1AC2
loc_1_1AB8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x68], 0x10
loc_1_1AC2:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_048C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1AFE
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_1_1AFE
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, ecx
        add    edx, 0x52
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1AFE:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1B30
        or     eax, eax
        jne    loc_1_1A64
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x332c]       ; -> obj1:0x332C
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x26
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_1D3B
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
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [ecx + 0x1a], eax
        cmp    dword ptr [esi + 0x28], 2
        mov    eax, dword ptr [ebp - 8]
        jne    loc_1_1B97
        mov    dword ptr [eax + 0x1e], 0x10
        jmp    loc_1_1B9E
loc_1_1B97:
        mov    dword ptr [eax + 0x1e], 0x20
loc_1_1B9E:
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [esi + 0x30]
        mov    dword ptr [eax + 0x22], ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    edi, [esi + 8]
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
        call   sub_1_050C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1D3B
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_1_1C10
        push   0x1000
        mov    eax, dword ptr [esi + 0x34]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0x38]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1C10
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_1D2B
        db  08Dh, 064h, 024h, 000h
loc_1_1C10:
        mov    dword ptr [esi + 0x6c6], 0
        mov    dword ptr [esi + 0x6ca], 0x3000 ; -> loc_1_3000
        mov    dword ptr [esi + 0x6ce], esi
        push   esi
        call   sub_1_06A0
        or     al, al
        jne    loc_1_1C40
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_1D2B
loc_1_1C40:
        push   esi
        call   sub_1_0720
        lea    ecx, [esi + 0x52]
        cmp    byte ptr [ecx], 0
        jne    loc_1_1C80
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_1_1C80
        cmp    byte ptr [esi + 0x54], 0
        jne    loc_1_1C80
        cmp    byte ptr [esi + 0x55], 0
        jne    loc_1_1C80
        cmp    byte ptr [esi + 0x56], 0
        jne    loc_1_1C80
        cmp    byte ptr [esi + 0x57], 0
        jne    loc_1_1C80
        mov    eax, dword ptr [esi + 0x4c]
        lea    edx, [esi + 0x4c]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1C80:
        lea    ebx, [esi + 0x24]
        push   ebx
        push   esi
        push   ecx
        push   0x2ff0                        ; -> loc_1_2FF0
        push   0x2f70                        ; -> loc_1_2F70
        push   0x2ef0                        ; -> loc_1_2EF0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_1_1CC1
        push   3
        push   2
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_1D2B
loc_1_1CC1:
        push   esi
        call   sub_1_0770
        or     al, al
        jne    loc_1_1CD4
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_1D23
loc_1_1CD4:
        push   esi
        call   sub_1_0940
        or     al, al
        jne    loc_1_1CE7
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_1D23
loc_1_1CE7:
        push   1
        push   0
        mov    eax, dword ptr [esi + 0x3c]
        push   eax
        push   eax
        push   0xc60                         ; -> loc_1_0C60
        push   esi
        push   0xc40                         ; -> loc_1_0C40
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1D50
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
loc_1_1D23:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_1_1D2B:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_1D3B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D50:
        mov    dword ptr [esi + 0x6de], 0
        mov    dword ptr [esi + 0x6e2], 0xc60 ; -> loc_1_0C60
        lea    eax, [esi + 0x6d2]
        mov    dword ptr [esi + 0x6e6], esi
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x6d6], 0x1780 ; -> loc_1_1780
        mov    dword ptr [esi + 0x6da], esi
        push   0x7d0
        push   eax
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_1DA0:
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
loc_1_1DC3:
        or     edx, edx
        je     loc_1_1DD2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_1DC3
        mov    dword ptr [ecx], edx
loc_1_1DD2:
        cmp    dword ptr [ecx], -1
        jne    loc_1_1DE4
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1DE4:
        mov    eax, dword ptr [0x332c]       ; -> obj1:0x332C
        push   eax
        push   0
        push   0x71
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_1E22
        push   4
        push   1
        push   2
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1E22:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x60]
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_1E3A
        cmp    dword ptr [esi + 0x5c], 0
        jne    loc_1_1E3A
        push   esi
        call   sub_1_08E0
loc_1_1E3A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x58]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x58], eax
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_1F20
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
        je     loc_1_1F20
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
        mov    dword ptr [esi + 0x6a9], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_1EFA
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_1EE2
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_1EE2
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_1EFA
loc_1_1EE2:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_1EFA:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_1F15
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_1F15
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_1F15
        push   esi
        call   sub_1_15E0
loc_1_1F15:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_1_1F20:
        dec    dword ptr [esi + 0x60]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x58], eax
        push   0x71
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
sub_1_1F60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edx, dword ptr [eax + 0x58]
        cmp    edx, ecx
        je     loc_1_1F80
loc_1_1F70:
        mov    esi, dword ptr [edx + 0x14]
        cmp    ecx, esi
        jae    loc_1_1F79
        mov    ecx, esi
loc_1_1F79:
        mov    edx, dword ptr [edx + 0xc]
        or     edx, edx
        jne    loc_1_1F70
loc_1_1F80:
        or     ecx, ecx
        jne    loc_1_1F89
        mov    ecx, 0xee
loc_1_1F89:
        mov    dword ptr [eax + 0x6a9], ecx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_1FC3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1FC3:
        inc    dword ptr [esi + 0x60]
        inc    dword ptr [ebx + 0x18]
        mov    ecx, dword ptr [esi + 0x58]
        cmp    ecx, ebx
        jne    loc_1_1FD8
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x58], eax
        jmp    loc_1_1FF1
loc_1_1FD8:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_1FE8
loc_1_1FE0:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_1FE0
loc_1_1FE8:
        lea    eax, [ebx + 0xc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_1FF1:
        push   esi
        call   sub_1_0880
        or     al, al
        jne    loc_1_2027
        mov    eax, dword ptr [esi + 0x24]
        push   0
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [esi + 0x24]
        push   0x71
        mov    dword ptr [eax + 0x10], 0
        push   ebx
        call   sub_1_050C
        dec    dword ptr [esi + 0x60]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2027:
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0240
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2060
        mov    eax, dword ptr [ebx + 0x18]
        cmp    eax, 2
        je     loc_1_2053
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        mov    edi, 0x103
        jmp    loc_1_2086
loc_1_2053:
        sub    eax, 2
        mov    dword ptr [ebx + 0x18], eax
        jmp    loc_1_2086
        db  005h, 000h, 000h, 000h, 000h
loc_1_2060:
        cmp    edi, 0x103
        jne    loc_1_2072
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        jmp    loc_1_2086
loc_1_2072:
        cmp    edi, 0xc001000e
        jne    loc_1_2083
        mov    byte ptr [ebx + 0x1c], 1
        mov    edi, 0x103
loc_1_2083:
        dec    dword ptr [ebx + 0x18]
loc_1_2086:
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_20C0
        mov    eax, dword ptr [esi + 0x6a9]
        cmp    dword ptr [ebx + 0x14], eax
        jne    loc_1_209D
        push   esi
        call   sub_1_1F60
loc_1_209D:
        push   0
        push   0x71
        push   ebx
        call   sub_1_050C
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_20CE
        cmp    dword ptr [esi + 0x5c], 0
        jne    loc_1_20CE
        push   esi
        call   sub_1_0920
        jmp    loc_1_20CE
        db  005h, 000h, 000h, 000h, 000h
loc_1_20C0:
        mov    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0x5c], ebx
        mov    edi, 0x103
loc_1_20CE:
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_2108
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_20EF
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_20EF
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_2108
loc_1_20EF:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2108:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_2123
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_2123
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_2123
        push   esi
        call   sub_1_15E0
loc_1_2123:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2130:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [edi + 0x60]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_2150
        mov    esi, 0xc0010002
        jmp    loc_1_218C
        db  090h
loc_1_2150:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        or     eax, eax
        jne    loc_1_2166
        push   ecx
        push   ebx
        push   edi
        call   sub_1_25B0
        mov    esi, eax
        jmp    loc_1_218C
loc_1_2166:
        cmp    eax, 1
        jne    loc_1_2187
        cmp    byte ptr [edi + 0x6a0], 0
        je     loc_1_217B
        mov    esi, 0xc001000d
        jmp    loc_1_218C
loc_1_217B:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2600
        mov    esi, eax
        jmp    loc_1_218C
loc_1_2187:
        mov    esi, 0x10001
loc_1_218C:
        dec    dword ptr [ebx + 0x18]
        mov    eax, dword ptr [edi + 0x60]
        dec    eax
        mov    dword ptr [edi + 0x60], eax
        jne    loc_1_21C7
        cmp    byte ptr [edi + 0x6a0], 0
        jne    loc_1_21B0
        cmp    dword ptr [edi + 0x6b9], 0
        jne    loc_1_21B0
        cmp    dword ptr [edi + 0x5c], 0
        je     loc_1_21C7
loc_1_21B0:
        push   1
        add    edi, 0x6c6
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_21C7:
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_21E2
        cmp    dword ptr [edi + 0x74], 0
        jne    loc_1_21E2
        cmp    byte ptr [edi + 0x6c5], 0
        jne    loc_1_21E2
        push   edi
        call   sub_1_15E0
loc_1_21E2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_21F0:
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
        ja     loc_1_2240
        je     loc_1_2410
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2261
        shl    eax, 2
        jmp    loc_1_255C
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2240:
        cmp    eax, 0x1010102
        je     loc_1_2430
        cmp    eax, 0x1010103
        je     loc_1_2450
        cmp    eax, 0x1010104
        je     loc_1_2509
loc_1_2261:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2512
        db  08Dh, 049h, 000h
loc_1_2270:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2285
        mov    esi, 0x33ac                   ; -> loc_1_33AC
        mov    edi, 0x5c
        jmp    loc_1_2512
loc_1_2285:
        mov    esi, 0x3330                   ; -> loc_1_3330
        mov    edi, 0x7c
        jmp    loc_1_2512
loc_1_2294:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x6a0], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_1_22AE
        mov    dword ptr [ebp - 0x14], 0
loc_1_22AE:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2512
        db  005h, 000h, 000h, 000h, 000h
loc_1_22C0:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2512
        db  08Dh, 049h, 000h
loc_1_22D0:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_22E0:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_22F0:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2300:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x74], 1
        sbb    eax, eax
        and    eax, 0x5ee
        jmp    loc_1_250F
loc_1_2313:
        mov    dword ptr [ebp - 8], 0x2000
        jmp    loc_1_2512
        db  090h
loc_1_2320:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2330:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2340:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x4c
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    loc_1_2512
loc_1_2366:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x28]
        cmp    eax, 2
        jne    loc_1_2380
        mov    esi, 0x34c8                   ; -> loc_1_34C8
        mov    edi, 0x23
        jmp    loc_1_2512
loc_1_2380:
        cmp    eax, 1
        jne    loc_1_2512
        mov    esi, 0x34ec                   ; -> obj1:0x34EC
        mov    edi, 0x1f
        jmp    loc_1_2512
loc_1_2398:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_23B0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_250F
        db  08Dh, 064h, 024h, 000h
loc_1_23B0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_1_250F
        db  08Bh, 0FFh
loc_1_23C0:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_23D4
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x6a9]
        jmp    loc_1_250F
loc_1_23D4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        jmp    loc_1_250F
        db  090h
loc_1_23E0:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_1_2512
loc_1_23F3:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2512
        db  090h
loc_1_2400:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_2512
        db  08Dh, 064h, 024h, 000h
loc_1_2410:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x4c
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2512
        db  090h
loc_1_2430:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x52
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_2512
        db  090h
loc_1_2450:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_24AA
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2482
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2512
loc_1_2482:
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
        jmp    loc_1_2512
loc_1_24AA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 0x24]
        push   eax
        call   sub_2_0660
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_24DA
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_2512
loc_1_24DA:
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
        jmp    loc_1_2512
loc_1_2509:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x68]
loc_1_250F:
        mov    dword ptr [ebp - 8], eax
loc_1_2512:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_254E
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_254E
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_253E
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
loc_1_253E:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_254E:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
loc_1_255C:
        jo     0x2580
        add    byte ptr [eax], al
        xchg   esp, eax
        and    al, byte ptr [eax]
        add    al, al                        ; -> loc_1_22C0
        and    al, byte ptr [eax]
        add    al, al                        ; -> loc_1_22C0
        and    al, byte ptr [eax]
        add    al, dl                        ; -> loc_1_22D0
        and    al, byte ptr [eax]
        add    al, ah                        ; -> loc_1_22E0
        and    al, byte ptr [eax]
        add    al, dh                        ; -> loc_1_22F0
        and    al, byte ptr [eax]
        add    byte ptr [eax], al            ; -> loc_1_2300
        and    eax, dword ptr [eax]
        add    byte ptr [ebx], dl            ; -> loc_1_2313
        and    eax, dword ptr [eax]
        add    byte ptr [eax], ah            ; -> loc_1_2320
        and    eax, dword ptr [eax]
        add    byte ptr [eax], dh            ; -> loc_1_2330
        and    eax, dword ptr [eax]
        add    byte ptr [eax + 0x23], al     ; -> loc_1_2340
        add    byte ptr [eax], al
        and    ax, word ptr [eax]
        add    byte ptr [eax - 0x3fffffdd], bl ; -> loc_1_2398
        and    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_1_23E0
        and    eax, dword ptr [eax]
        add    bl, dh                        ; -> loc_1_23F3
        and    eax, dword ptr [eax]
        add    byte ptr [ecx + 0x22], ah     ; -> loc_1_2261
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [ebp - 0x72ffdb9c], cl
        and    al, 0
sub_1_25B0:
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
        call   sub_1_21F0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2600:
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
        je     loc_1_2657
        cmp    eax, 0x1010f
        je     loc_1_26B8
        cmp    eax, 0x10112
        je     loc_1_2747
        cmp    eax, 0x1010103
        je     loc_1_2774
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_27B5
loc_1_2657:
        cmp    edi, 4
        je     loc_1_2674
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 4
        jmp    loc_1_27B5
loc_1_2674:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_26A1
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_27B5
loc_1_26A1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_27D0
        jmp    loc_1_27B3
loc_1_26B8:
        cmp    edi, 4
        je     loc_1_26D5
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_27B5
loc_1_26D5:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0xfffffffe
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_2740
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x6a9]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_2716
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x6a9], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_27B5
loc_1_2716:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x14], eax
        jne    loc_1_2737
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_2737
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        push   edx
        call   sub_1_1F60
        jmp    loc_1_27B5
loc_1_2737:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_27B5
        db  090h
loc_1_2740:
        mov    ebx, 0xc0010014
        jmp    loc_1_27B5
loc_1_2747:
        cmp    edi, 4
        je     loc_1_2761
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_27B5
loc_1_2761:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x1d
        push   ecx
        call   sub_1_05EC
        sub    ebx, ebx
        jmp    loc_1_27B5
loc_1_2774:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_1_2798
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_27B5
loc_1_2798:
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
        call   sub_1_2830
loc_1_27B3:
        mov    ebx, eax
loc_1_27B5:
        or     ebx, ebx
        jne    loc_1_27C3
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_27C3:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_27D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6a0], 0
        jne    loc_1_281A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2810
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
loc_1_2810:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_281A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6a0], 0
        jne    loc_1_287A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2870
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
loc_1_2870:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_1_287A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2890:
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
        call   sub_1_21F0
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_1_29A6
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2916
        je     loc_1_2960
        cmp    eax, 0x20101
        je     loc_1_2932
        cmp    eax, 0x20102
        je     loc_1_2940
        cmp    eax, 0x20103
        je     loc_1_2948
        cmp    eax, 0x20104
        je     loc_1_2950
        mov    edi, 0xc0010017
        jmp    loc_1_2989
loc_1_2916:
        cmp    eax, 0x1020101
        je     loc_1_2968
        cmp    eax, 0x1020102
        je     loc_1_2971
        cmp    eax, 0x1020103
        je     loc_1_2980
        mov    edi, 0xc0010017
        jmp    loc_1_2989
loc_1_2932:
        mov    eax, dword ptr [ebx + 0x688]
        jmp    loc_1_2986
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2940:
        mov    eax, dword ptr [ebx + 0x68c]
        jmp    loc_1_2986
loc_1_2948:
        mov    eax, dword ptr [ebx + 0x690]
        jmp    loc_1_2986
loc_1_2950:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2989
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2960:
        mov    eax, dword ptr [ebx + 0x69c]
        jmp    loc_1_2986
loc_1_2968:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2989
loc_1_2971:
        mov    eax, dword ptr [ebx + 0x694]
        jmp    loc_1_2986
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2980:
        mov    eax, dword ptr [ebx + 0x698]
loc_1_2986:
        mov    dword ptr [ebp - 4], eax
loc_1_2989:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_29A6
        or     edi, edi
        jne    loc_1_29A6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 8], 4
loc_1_29A6:
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
loc_1_29C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_29F2
        cmp    eax, 0x20101
        jae    loc_1_2A13
        cmp    eax, 0x10101
        jb     loc_1_2A0E
        cmp    eax, 0x10111
        jbe    loc_1_2A13
        cmp    eax, 0x10113
        je     loc_1_2A13
        jmp    loc_1_2A0E
loc_1_29F2:
        cmp    eax, 0x1010101
        jb     loc_1_2A0E
        cmp    eax, 0x1010104
        jbe    loc_1_2A13
        cmp    eax, 0x1020101
        jb     loc_1_2A0E
        cmp    eax, 0x1020103
        jbe    loc_1_2A13
loc_1_2A0E:
        mov    esi, 0xc0010017
loc_1_2A13:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x60]
        or     esi, esi
        jne    loc_1_2A26
        push   ecx
        push   edi
        call   sub_1_2890
        mov    esi, eax
loc_1_2A26:
        mov    eax, dword ptr [edi + 0x60]
        dec    eax
        mov    dword ptr [edi + 0x60], eax
        jne    loc_1_2A60
        cmp    byte ptr [edi + 0x6a0], 0
        jne    loc_1_2A47
        cmp    dword ptr [edi + 0x6b9], 0
        jne    loc_1_2A47
        cmp    dword ptr [edi + 0x5c], 0
        je     loc_1_2A60
loc_1_2A47:
        push   1
        add    edi, 0x6c6
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2A60:
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_2A7B
        cmp    dword ptr [edi + 0x74], 0
        jne    loc_1_2A7B
        cmp    byte ptr [edi + 0x6c5], 0
        jne    loc_1_2A7B
        push   edi
        call   sub_1_15E0
loc_1_2A7B:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2A90:
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
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x6d2]
        push   eax
        call   sub_1_04E8
        push   0x2625a0
        call   sub_1_0500
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0120
        lea    eax, [esi + 0xc]
        push   eax
        call   sub_1_04F4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0438
        push   0
        push   0x6eb
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_2B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x1c], 0
        je     loc_1_2B43
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B43:
        inc    dword ptr [esi + 0x18]
        inc    dword ptr [edi + 0x60]
        cmp    byte ptr [edi + 0x6a0], 0
        je     loc_1_2B66
        dec    dword ptr [esi + 0x18]
        dec    dword ptr [edi + 0x60]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B66:
        mov    ebx, dword ptr [edi + 0x58]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_1_2B96
loc_1_2B74:
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
        jne    loc_1_2B74
loc_1_2B96:
        mov    byte ptr [edi + 0x6a0], 1
        mov    dword ptr [edi + 0x6a1], 0
        mov    dword ptr [edi + 0x6a5], esi
        push   edi
        call   sub_1_2C20
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 0x18]
        mov    eax, dword ptr [edi + 0x60]
        dec    eax
        mov    dword ptr [edi + 0x60], eax
        jne    loc_1_2BF4
        cmp    byte ptr [edi + 0x6a0], 0
        jne    loc_1_2BDA
        cmp    dword ptr [edi + 0x6b9], 0
        jne    loc_1_2BDA
        cmp    dword ptr [edi + 0x5c], 0
        je     loc_1_2BF4
loc_1_2BDA:
        push   1
        add    edi, 0x6c6
        push   edi
        call   sub_1_04E2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2BF4:
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_2C0F
        cmp    dword ptr [edi + 0x74], 0
        jne    loc_1_2C0F
        cmp    byte ptr [edi + 0x6c5], 0
        jne    loc_1_2C0F
        push   edi
        call   sub_1_15E0
loc_1_2C0F:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2C20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x60]
        cmp    dword ptr [esi + 0x6b9], 0
        je     loc_1_2C6C
loc_1_2C34:
        mov    ecx, dword ptr [esi + 0x6b9]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x6b9], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_1_2C60
        cmp    eax, 8
        je     loc_1_2C60
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_2C60:
        dec    dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_2C34
loc_1_2C6C:
        cmp    dword ptr [esi + 0x74], 0
        je     loc_1_2C8A
        mov    dword ptr [esi + 0x6a1], 2
        dec    dword ptr [esi + 0x60]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_2C8A:
        dec    dword ptr [esi + 0x60]
        push   esi
        call   sub_1_2CA0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CA0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x60]
        inc    eax
        mov    dword ptr [ecx + 0x60], eax
        cmp    dword ptr [ecx + 0x74], 0
        je     loc_1_2CC9
        mov    dword ptr [ecx + 0x6a1], 3
        dec    dword ptr [ecx + 0x60]
        mov    eax, 0x103
        pop    ebp
        ret    4
loc_1_2CC9:
        dec    eax
        mov    dword ptr [ecx + 0x60], eax
        push   ecx
        call   sub_1_2CE0
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x60]
        mov    ecx, dword ptr [esi + 0x74]
        or     ecx, ecx
        je     loc_1_2D11
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0x74], 0
loc_1_2D11:
        mov    edx, dword ptr [esi + 0x6c]
        or     edx, edx
        je     loc_1_2D3C
loc_1_2D18:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x6c], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x6c]
        or     edx, edx
        jne    loc_1_2D18
loc_1_2D3C:
        mov    edx, dword ptr [esi + 0x6ad]
        or     edx, edx
        je     loc_1_2D70
loc_1_2D46:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x6ad], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x6ad]
        or     edx, edx
        jne    loc_1_2D46
loc_1_2D70:
        cmp    byte ptr [esi + 0x6c5], 0
        je     loc_1_2D93
        lea    esp, [esp]
loc_1_2D80:
        push   0x2710
        call   sub_1_0500
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_2D80
loc_1_2D93:
        push   esi
        call   sub_1_0A60
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x6a0], 0
        mov    edi, dword ptr [esi + 0x58]
        or     edi, edi
        je     loc_1_2DF3
        sub    ebx, ebx
loc_1_2DB5:
        inc    dword ptr [edi + 0x18]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_2DCC
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_2DCC:
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
        jne    loc_1_2DB5
loc_1_2DF3:
        dec    dword ptr [esi + 0x60]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x6a1], eax
        jne    loc_1_2EE2
        cmp    eax, 1
        je     loc_1_2E90
        cmp    eax, 2
        je     loc_1_2E98
        cmp    eax, 3
        je     loc_1_2EA0
        mov    eax, dword ptr [ebp - 4]
loc_1_2E3B:
        cmp    eax, 0x103
        je     loc_1_2E5A
        push   eax
        mov    eax, dword ptr [esi + 0x6a5]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 0x6a5]
        dec    dword ptr [eax + 0x18]
loc_1_2E5A:
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_2EC7
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_2E7B
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_2E7B
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_2EC7
loc_1_2E7B:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2E90:
        push   esi
        call   sub_1_2C20
        jmp    loc_1_2E3B
loc_1_2E98:
        push   esi
        call   sub_1_2CA0
        jmp    loc_1_2E3B
loc_1_2EA0:
        mov    eax, dword ptr [esi + 0x60]
        inc    eax
        mov    dword ptr [esi + 0x60], eax
        cmp    dword ptr [esi + 0x74], 0
        je     loc_1_2EB8
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2EB8:
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        push   esi
        call   sub_1_2CE0
        jmp    loc_1_2E3B
loc_1_2EC7:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_2EE2
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_2EE2
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_2EE2
        push   esi
        call   sub_1_15E0
loc_1_2EE2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_2F03
        lea    edx, [eax + 0x49]
loc_1_2F03:
        cmp    byte ptr [ecx + 0x6a0], 0
        je     loc_1_2F15
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_2F15:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6b9], 0
        jne    loc_1_2F50
        mov    dword ptr [ecx + 0x6bd], edx
        mov    dword ptr [ecx + 0x6b9], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_2F50:
        mov    eax, dword ptr [ecx + 0x6bd]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6bd], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2F70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_2F83
        lea    edx, [eax + 0x21]
loc_1_2F83:
        cmp    byte ptr [ecx + 0x6a0], 0
        je     loc_1_2F95
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_2F95:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6b9], 0
        jne    loc_1_2FD0
        mov    dword ptr [ecx + 0x6bd], edx
        mov    dword ptr [ecx + 0x6b9], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_2FD0:
        mov    eax, dword ptr [ecx + 0x6bd]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6bd], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x18]
        ret    4
        db  08Dh, 049h, 000h
loc_1_3000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x60]
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_30B6
        cmp    dword ptr [esi + 0x6c1], 0
        jne    loc_1_30B6
loc_1_3026:
        mov    edi, dword ptr [esi + 0x6b9]
        or     edi, edi
        je     loc_1_3091
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x6b9], eax
        cmp    dword ptr [esi + 0x6bd], edi
        jne    loc_1_304A
        mov    dword ptr [esi + 0x6bd], 0
loc_1_304A:
        mov    dword ptr [esi + 0x6c1], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_3065
        cmp    eax, 0xa
        je     loc_1_3065
        push   esi
        call   sub_1_3180
        jmp    loc_1_306B
loc_1_3065:
        push   esi
        call   sub_1_31E0
loc_1_306B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_3085
        cmp    ecx, 4
        je     loc_1_3085
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3085:
        dec    dword ptr [ebx + 0x18]
        cmp    byte ptr [esi + 0x6a0], 0
        je     loc_1_3026
loc_1_3091:
        mov    dword ptr [esi + 0x6c1], 0
        cmp    byte ptr [esi + 0x6a0], 0
        je     loc_1_30B6
        dec    dword ptr [esi + 0x60]
        push   1
        push   esi
        call   sub_1_2E10
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_30B6:
        mov    edi, dword ptr [esi + 0x5c]
        or     edi, edi
        je     loc_1_3121
        sub    ebx, ebx
        nop    
loc_1_30C0:
        cmp    dword ptr [edi + 0x18], 0
        jbe    loc_1_30D0
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_310B
        db  08Dh, 049h, 000h
loc_1_30D0:
        mov    eax, dword ptr [esi + 0x6a9]
        cmp    dword ptr [edi + 0x14], eax
        jne    loc_1_30E1
        push   esi
        call   sub_1_1F60
loc_1_30E1:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_30FA
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_30FD
loc_1_30FA:
        mov    dword ptr [esi + 0x5c], eax
loc_1_30FD:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x71
        push   eax
        call   sub_1_050C
loc_1_310B:
        or     edi, edi
        jne    loc_1_30C0
        cmp    dword ptr [esi + 0x5c], 0
        jne    loc_1_3121
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_3121
        push   esi
        call   sub_1_0920
loc_1_3121:
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    dword ptr [esi + 0x60], eax
        jne    loc_1_3157
        cmp    byte ptr [esi + 0x6a0], 0
        jne    loc_1_3142
        cmp    dword ptr [esi + 0x6b9], 0
        jne    loc_1_3142
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_3157
loc_1_3142:
        push   1
        add    esi, 0x6c6
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3157:
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_3172
        cmp    dword ptr [esi + 0x74], 0
        jne    loc_1_3172
        cmp    byte ptr [esi + 0x6c5], 0
        jne    loc_1_3172
        push   esi
        call   sub_1_15E0
loc_1_3172:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        test   al, 0x20
        mov    dword ptr [esi + 0x7a], eax
        je     loc_1_319C
        lea    eax, [esi + 0x43]
        mov    byte ptr [eax], 7
        jmp    loc_1_31C3
loc_1_319C:
        test   al, 4
        push   esi
        je     loc_1_31A8
        call   sub_1_0BA0
        jmp    loc_1_31AD
loc_1_31A8:
        call   sub_1_31E0
loc_1_31AD:
        cmp    dword ptr [esi + 0x7a], 0
        lea    eax, [esi + 0x43]
        je     loc_1_31C0
        mov    byte ptr [eax], 6
        jmp    loc_1_31C3
        db  005h, 000h, 000h, 000h, 000h
loc_1_31C0:
        mov    byte ptr [eax], 4
loc_1_31C3:
        mov    edx, dword ptr [esi + 0x30]
        add    dx, 5
        mov    al, byte ptr [eax]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0B20
        push   esi
        call   sub_1_0BF0
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
loc_1_32E0:
        pushal 
        xor    al, byte ptr [eax]
        db  000h
        dd  obj1:0x32A0              ; fixup
loc_1_32E8:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x6d6f
loc_1_32F0:
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
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_3330:
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
loc_1_33AC:
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
        dec    esi
        dec    ecx
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3410:
        dec    esi
        dec    ecx
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        popal  
        jo     0x3491
        jb     0x3474
        jns    loc_1_3492
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_349D
        jb     loc_1_3480
        jns    0x349e
        add    byte ptr gs:[ecx + 0x6f], cl
        inc    edx
        popal  
        jae    0x349b
        inc    ecx
        jb     loc_1_34A0
        jae    loc_1_34B0
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], cl
        inc    edx
        popal  
        jae    0x34ab
        inc    ecx
        jb     loc_1_34B0
        jae    0x34c0
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x34cf
        inc    edx
        popal  
        jae    loc_1_34BF
        inc    ecx
        jb     0x34c4
        jae    loc_1_34D4
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x34e3
        inc    edx
        popal  
        jae    0x34d3
        inc    ecx
        jb     0x34d8
        jae    loc_1_34E8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_34E1
        jb     loc_1_34F0
        jne    loc_1_34F0
loc_1_3480:
        je     loc_1_34D0
        jne    0x34f1
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x6e], cl
        je     0x34f1
        jb     0x3500
        jne    0x3500
        je     loc_1_34E0
loc_1_3492:
        jne    loc_1_3501
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ebp + 0x61], cl
        js     loc_1_3505
        insd   dword ptr es:[edi], dx
loc_1_349D:
        jne    0x350c
        dec    ebp
loc_1_34A0:
        jne    0x350e
        je     0x350d
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_34F5
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_34B0:
        dec    ebp
        popal  
        js     0x351d
        insd   dword ptr es:[edi], dx
        jne    0x3524
        dec    ebp
        jne    0x3526
        je     0x3525
        arpl   word ptr [ecx + 0x73], sp
loc_1_34BF:
        je     0x350d
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_34C8:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x50206d6f
loc_1_34D0:
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
loc_1_34D4:
        je     loc_1_34F6
        inc    ebp
        je     0x3541
        jb     0x354a
        je     loc_1_34FF
        dec    ecx
loc_1_34E0:
        dec    ecx
loc_1_34E1:
        dec    ecx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x355c
loc_1_34E8:
        jb     0x3519
        add    byte ptr [esi + 0x75], al
        push   0x69
loc_1_34F0:
        jae    0x3566
        jne    0x3514
        dec    ebp
loc_1_34F5:
        inc    edx
loc_1_34F6:
        dec    eax
        xor    dword ptr [eax], esi
        xor    esi, dword ptr [eax]
        xor    ah, byte ptr [eax]
        inc    edx
        popal  
loc_1_34FF:
        jae    0x3566
loc_1_3501:
        and    byte ptr fs:[ecx + 0x64], al
loc_1_3505:
        popal  
        jo     0x357c
        jb     0x3539
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
        mov    ebx, dword ptr [0x350c]       ; -> obj1:0x350C
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
        mov    eax, dword ptr [0x350c]       ; -> obj1:0x350C
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
