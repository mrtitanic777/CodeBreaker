; CENDIS -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5492 instructions recovered by recursive descent

Declare_Virtual_Device CENDIS, 3, 10, CENDIS_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3818  R-X
loc_1_0000:
        mov    al, byte ptr [0x698]          ; -> obj1:0x0698
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x698]              ; -> obj1:0x0698
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_1A00
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
CENDIS_Control:
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
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [ecx]
        test   al, 1
        je     loc_1_0120
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0115
        cmp    al, 0xff
        jne    loc_1_0115
        cmp    byte ptr [ecx + 1], 0xff
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
        mov    ecx, dword ptr [ebp + 0x10]
        lea    eax, [esi + 0x2c]
        mov    ecx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 2], ecx
        jne    loc_1_0149
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_0150
loc_1_0149:
        mov    dword ptr [ebp - 0x10], 1
loc_1_0150:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0160
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_018A
        db  090h
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
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
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
sub_1_04D4:
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
        db  'CENDIS  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  CENDIS_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x40]
        lea    ecx, [eax + 0x3f8]
        cmp    ecx, eax
        jbe    loc_1_06D3
        mov    bl, byte ptr [ebp + 0xc]
loc_1_06B7:
        mov    dl, byte ptr [eax]
        cmp    dl, bl
        je     loc_1_06D3
        or     dl, dl
        jne    loc_1_06C6
        add    eax, 4
        jmp    loc_1_06CF
loc_1_06C6:
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        lea    eax, [eax + edx*2 + 4]
loc_1_06CF:
        cmp    ecx, eax
        ja     loc_1_06B7
loc_1_06D3:
        cmp    byte ptr [eax], 0xff
        jne    loc_1_06DA
        sub    eax, eax
loc_1_06DA:
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_06E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0x35c8                   ; -> loc_1_35C8
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x35d0                   ; -> loc_1_35D0
        mov    edx, dword ptr [esi + 0x40]
        cmp    dword ptr [esi + 0x28], 2
        je     loc_1_0705
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0705:
        add    edx, 0x14
        cmp    byte ptr [0x35c8], 0          ; -> loc_1_35C8
        je     loc_1_0722
loc_1_0711:
        mov    eax, ebx
        inc    ebx
        mov    cl, byte ptr [edx]
        cmp    byte ptr [eax], cl
        jne    loc_1_0757
        add    edx, 2
        cmp    byte ptr [ebx], 0
        jne    loc_1_0711
loc_1_0722:
        push   0x15
        push   esi
        call   sub_1_06A0
        add    eax, 0x2c
        mov    dword ptr [esi + 0x2c], 1
        cmp    byte ptr [0x35d0], 0          ; -> loc_1_35D0
        je     loc_1_0767
loc_1_073D:
        mov    edx, edi
        inc    edi
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx], cl
        jne    loc_1_0760
        add    eax, 2
        cmp    byte ptr [edi], 0
        jne    loc_1_073D
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0757:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0760:
        mov    dword ptr [esi + 0x2c], 0
loc_1_0767:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_0770:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x40]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_0790
        mov    byte ptr [edi + 0x800], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0790:
        mov    byte ptr [edi + 0x806], 0xd4
        push   esi
        call   sub_1_0870
        or     al, al
        jne    loc_1_07B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07B0:
        push   0x30d40
        call   sub_1_0500
        mov    eax, dword ptr [esi + 0x4d]
        or     eax, eax
        jne    loc_1_07D8
        test   byte ptr [edi + 0x806], 1
        jne    loc_1_07D0
        jmp    loc_1_07E5
        db  08Dh, 064h, 024h, 000h
loc_1_07D0:
        add    esi, 0x4c
        mov    byte ptr [esi], 1
        jmp    loc_1_07EB
loc_1_07D8:
        cmp    eax, 2
        jne    loc_1_07E5
        add    esi, 0x4c
        mov    byte ptr [esi], 1
        jmp    loc_1_07EB
loc_1_07E5:
        add    esi, 0x4c
        mov    byte ptr [esi], 0
loc_1_07EB:
        cmp    byte ptr [esi], 0
        je     loc_1_07F7
        mov    byte ptr [edi + 0x806], 0x55
loc_1_07F7:
        push   0x30d40
        call   sub_1_0500
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x28]
        cmp    eax, 1
        jne    loc_1_083B
        sub    ecx, ecx
loc_1_0821:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, cx
        add    dx, 0x1a
        in     al, dx
        mov    byte ptr [esi + ecx + 0x5c], al
        inc    ecx
        cmp    ecx, 6
        jl     loc_1_0821
        pop    esi
        pop    ebp
        ret    4
loc_1_083B:
        cmp    eax, 2
        jne    loc_1_085E
        push   0x22
        push   esi
        call   sub_1_06A0
        sub    ecx, ecx
        lea    ebx, [ebx]
loc_1_0850:
        mov    dl, byte ptr [eax + ecx*2 + 0xa]
        mov    byte ptr [esi + ecx + 0x5c], dl
        inc    ecx
        cmp    ecx, 6
        jl     loc_1_0850
loc_1_085E:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x2c], 0
        je     loc_1_088C
        mov    edx, dword ptr [ecx + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_088C:
        mov    byte ptr [ecx + 0x51], 0x52
        cmp    dword ptr [ecx + 0x28], 2
        jne    loc_1_08A4
        cmp    byte ptr [ecx + 0x4c], 0
        je     loc_1_08A4
        lea    edi, [ecx + 0x52]
        mov    byte ptr [edi], 0x20
        jmp    loc_1_08AA
loc_1_08A4:
        lea    edi, [ecx + 0x52]
        mov    byte ptr [edi], 0xa0
loc_1_08AA:
        mov    si, word ptr [ecx + 0x34]
        mov    al, byte ptr [ecx + 0x51]
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
        mov    dx, word ptr [ecx + 0x34]
        out    dx, al
        inc    dx
        mov    si, word ptr [ecx + 0x34]
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
loc_1_08E9:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x62]
        out    dx, al
        inc    bl
        cmp    bl, 6
        jb     loc_1_08E9
        mov    al, byte ptr [edi]
        or     al, 4
        lea    edx, [esi + 7]
        out    dx, al
        mov    eax, dword ptr [ecx + 0x8a]
        test   al, 0x20
        jne    loc_1_093B
        test   al, 4
        je     loc_1_093B
        sub    bl, bl
loc_1_0920:
        mov    dl, bl
        and    edx, 0xffff00ff
        add    dx, si
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_0920
        jmp    loc_1_095F
loc_1_093B:
        sub    bl, bl
        lea    ecx, [ecx]
loc_1_0940:
        mov    al, bl
        and    eax, 0xffff00ff
        lea    edx, [esi + eax]
        add    dx, 8
        and    eax, 0xffff
        mov    al, byte ptr [ecx + eax + 0x54]
        out    dx, al
        inc    bl
        cmp    bl, 8
        jb     loc_1_0940
loc_1_095F:
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
        mov    al, byte ptr [ecx + 0x51]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_09AA
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_09AA:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0xff
        je     loc_1_09BD
        or     al, al
        jne    loc_1_09F2
loc_1_09BD:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        not    al
        out    dx, al
        in     al, dx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1]
        not    ecx
        and    eax, 0xff
        cmp    ecx, eax
        jne    loc_1_09F0
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 8
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_09F0:
        sub    al, al
loc_1_09F2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x2c], 0
        je     loc_1_0A17
        mov    edx, dword ptr [ecx + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0A17:
        mov    si, word ptr [ecx + 0x34]
        mov    al, 2
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [ecx + 0x53]
        or     al, 4
        out    dx, al
        lea    edx, [esi + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 0x81
        out    dx, al
        cmp    dword ptr [ecx + 0x28], 1
        jne    loc_1_0A42
        lea    edx, [esi + 0x10]
        mov    al, 0x1d
        out    dx, al
loc_1_0A42:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x2c], 0
        je     loc_1_0A66
        mov    edx, dword ptr [ecx + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0A66:
        mov    cx, word ptr [ecx + 0x34]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x28], 1
        jne    loc_1_0A9C
        mov    edx, dword ptr [ebx + 0x34]
        add    dx, 0x10
        mov    al, 0xf
        out    dx, al
loc_1_0A9C:
        cmp    dword ptr [ebx + 0x2c], 0
        je     loc_1_0AAC
        mov    edx, dword ptr [ebx + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0AAC:
        mov    edx, dword ptr [ebx + 0x34]
        add    dx, 5
        mov    al, 6
        out    dx, al
        mov    esi, 0x35c0                   ; -> loc_1_35C0
        add    eax, 0
loc_1_0AC0:
        mov    ecx, dword ptr [ebx + 0x34]
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
        cmp    esi, 0x35c8                   ; -> loc_1_35C8
        jb     loc_1_0AC0
        mov    edx, dword ptr [ebx + 0x34]
        add    dx, 0xa
        mov    al, 0x82
        out    dx, al
        push   0xfa
        call   sub_1_0500
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 8], 0x35c0   ; -> loc_1_35C0
loc_1_0B07:
        mov    edx, dword ptr [ebx + 0x34]
        add    dx, 5
        in     al, dx
        test   al, 0x40
        jne    loc_1_0B65
        push   2
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebx + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        cmp    word ptr [ebp - 0xa], 0x3d
        jne    loc_1_0B65
        push   0x1f
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebx + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        lea    edi, [ebp - 0x50]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 0xf
        mov    esi, dword ptr [eax]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0B65
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    loc_1_0B65
        add    dword ptr [ebp - 8], 4
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 8], 0x35c8   ; -> loc_1_35C8
        jb     loc_1_0B07
loc_1_0B65:
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
        mov    dx, word ptr [ebx + 0x34]
        in     al, dx
        mov    dx, word ptr [ebx + 0x34]
        out    dx, al
        inc    dx
        cmp    dword ptr [ebx + 0x28], 1
        mov    si, word ptr [ebx + 0x34]
        in     al, dx
        lea    edx, [esi + 1]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, 4
        out    dx, al
        jne    loc_1_0B99
        lea    edx, [esi + 0x10]
        mov    al, 0xd
        out    dx, al
loc_1_0B99:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A50
        push   esi
        call   sub_1_0870
        push   esi
        call   sub_1_0A00
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
sub_1_0BD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        sub    edx, edx
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_0C1B
loc_1_0BE2:
        mov    ecx, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + ecx]
        mov    esi, 8
        lea    ecx, [ecx]
loc_1_0BF0:
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
        je     loc_1_0C12
        xor    eax, 0x4c11db7
loc_1_0C12:
        dec    esi
        jne    loc_1_0BF0
        inc    edx
        cmp    edx, dword ptr [ebp + 0xc]
        jb     loc_1_0BE2
loc_1_0C1B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C30:
        push   ebp
        mov    ebp, esp
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BD0
        sub    ecx, ecx
        mov    edx, 1
loc_1_0C45:
        add    ecx, ecx
        test   eax, edx
        je     loc_1_0C4E
        or     ecx, 1
loc_1_0C4E:
        add    edx, edx
        cmp    edx, 0x40
        jb     loc_1_0C45
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
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, 8
loc_1_0C89:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [esi + ecx + 0x54], dl
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_0C89
        push   0x34e0                        ; -> obj1:0x34E0
        lea    eax, [ebp - 8]
        push   eax
        push   0x60
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_0CE7
        sub    edi, edi
loc_1_0CB8:
        dec    dword ptr [ebp - 8]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax*2 + 0x34e0]         ; -> obj1:0x34E0
        push   eax
        call   sub_1_0C30
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        or     byte ptr [eax + esi + 0x54], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_0CB8
loc_1_0CE7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_0D09
        mov    edx, dword ptr [edi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0D09:
        mov    si, word ptr [edi + 0x34]
        mov    al, byte ptr [edi + 0x51]
        or     al, 0x80
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 0x52]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D24:
        lea    edx, [ebx + esi]
        add    dx, 8
        mov    al, 0xff
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0D24
        mov    al, byte ptr [edi + 0x52]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 0x51]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_0D69
        mov    edx, dword ptr [edi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0D69:
        mov    si, word ptr [edi + 0x34]
        mov    al, byte ptr [edi + 0x51]
        or     al, 0x80
        lea    edx, [esi + 6]
        out    dx, al
        lea    ecx, [esi + 7]
        mov    al, byte ptr [edi + 0x52]
        or     al, 4
        mov    dx, cx
        out    dx, al
        sub    ebx, ebx
loc_1_0D84:
        lea    edx, [esi + ebx]
        add    dx, 8
        mov    al, byte ptr [edi + ebx + 0x54]
        out    dx, al
        inc    ebx
        cmp    ebx, 8
        jb     loc_1_0D84
        mov    al, byte ptr [edi + 0x52]
        or     al, 8
        mov    dx, cx
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [edi + 0x51]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0DB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x34]
        sub    al, al
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_0F25
        mov    byte ptr [esi + 0x6d5], 1
        inc    dword ptr [esi + 0x70]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_0E03
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0E03:
        sub    edi, edi
        sub    ebx, ebx
loc_1_0E07:
        mov    dx, word ptr [esi + 0x34]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x34]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, bl
        jne    loc_1_0E21
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_0E97
loc_1_0E21:
        test   byte ptr [ebp - 2], 0x81
        je     loc_1_0E3F
        test   byte ptr [ebp - 2], 1
        je     loc_1_0E33
        inc    dword ptr [esi + 0x6ac]
loc_1_0E33:
        push   esi
        call   sub_1_0F30
        push   esi
        call   sub_1_17E0
loc_1_0E3F:
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_0E4D
        cmp    byte ptr [esi + 0x88], bl
        je     loc_1_0E8D
loc_1_0E4D:
        mov    dx, word ptr [esi + 0x34]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dword ptr [esi + 0x74], ebx
        cmp    byte ptr [esi + 0x88], bl
        je     loc_1_0E70
        mov    byte ptr [esi + 0x89], 2
        mov    byte ptr [esi + 0x88], bl
        jmp    loc_1_0E79
        db  090h
loc_1_0E70:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x89], al
loc_1_0E79:
        mov    byte ptr [esi + 0x6fa], bl
        cmp    dword ptr [esi + 0x84], ebx
        je     loc_1_0E8D
        push   esi
        call   sub_1_1160
loc_1_0E8D:
        inc    edi
        cmp    edi, 8
        jl     loc_1_0E07
loc_1_0E97:
        cmp    dword ptr [esi + 0x6bd], 0
        je     loc_1_0EB8
loc_1_0EA0:
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_0EB8
        push   esi
        call   sub_1_0F30
        cmp    dword ptr [esi + 0x6bd], 0
        jne    loc_1_0EA0
loc_1_0EB8:
        mov    cx, word ptr [esi + 0x34]
        mov    byte ptr [esi + 0x6d5], 0
        lea    edx, [ecx + 2]
        mov    al, 0x80
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, 0x81
        out    dx, al
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_0F07
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_0EF0
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_0EF0
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_0F07
loc_1_0EF0:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0F07:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_0F25
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_0F25
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_0F25
        push   esi
        call   sub_1_17E0
loc_1_0F25:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_0F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 3], 1
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_0F54
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_0F54:
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_10D4
        mov    cx, word ptr [esi + 0x34]
        mov    al, 0x81
        lea    edx, [ecx + 1]
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        test   al, 0x40
        jne    loc_1_10D4
        inc    dword ptr [esi + 0x74]
        mov    dword ptr [esi + 0x6c5], 0
        push   2
        lea    eax, [esi + 0x690]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    ax, word ptr [esi + 0x692]
        mov    word ptr [esi + 0x8e], ax
        cmp    ax, 6
        jb     loc_1_106A
        mov    edi, dword ptr [esi + 0x6b9]
        add    di, 0xe
        cmp    di, ax
        jbe    loc_1_0FC3
        mov    di, ax
loc_1_0FC3:
        mov    bl, 1
        test   byte ptr [esi + 0x8a], 0x28
        jne    loc_1_1040
        push   3
        lea    eax, [esi + 0x90]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0x8e], 6
        sub    di, 6
        cmp    dword ptr [esi + 0x92], -1
        jne    loc_1_1010
        cmp    byte ptr [esi + 0x90], 0xff
        jne    loc_1_1010
        cmp    byte ptr [esi + 0x91], 0xff
        jne    loc_1_1010
        jmp    loc_1_106A
        db  08Dh, 064h, 024h, 000h
loc_1_1010:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0x96]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0x8e], di
        jmp    loc_1_106C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1040:
        mov    eax, edi
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [esi + 0x90]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        sub    word ptr [esi + 0x8e], di
        jmp    loc_1_106C
loc_1_106A:
        sub    bl, bl
loc_1_106C:
        or     bl, bl
        je     loc_1_1085
        push   esi
        call   sub_1_1410
        cmp    eax, 3
        je     loc_1_1085
        inc    dword ptr [esi + 0x69c]
        mov    byte ptr [ebp - 2], 1
loc_1_1085:
        mov    ax, word ptr [esi + 0x8e]
        or     ax, ax
        je     loc_1_10BF
        cmp    ax, 8
        ja     loc_1_10B5
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        jmp    loc_1_10BF
loc_1_10B5:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0xe
        mov    al, 4
        out    dx, al
loc_1_10BF:
        cmp    dword ptr [esi + 0x74], 5
        jbe    loc_1_0F54
        mov    byte ptr [ebp - 3], 0
        mov    dword ptr [esi + 0x74], 0
loc_1_10D4:
        cmp    dword ptr [esi + 0x6bd], 0
        je     loc_1_1122
        lea    ecx, [ecx]
loc_1_10E0:
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_112B
        mov    edi, dword ptr [esi + 0x6bd]
        push   edi
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x6bd], eax
        mov    dword ptr [esi + 0x6c5], edi
        mov    ebx, dword ptr [edi + 0x28]
        push   esi
        call   sub_1_1270
        push   0
        mov    eax, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_044A
        dec    dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x6bd], 0
        jne    loc_1_10E0
loc_1_1122:
        cmp    byte ptr [esi + 0x6b0], 0
        je     loc_1_1137
loc_1_112B:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1137:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_1146
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0290
loc_1_1146:
        mov    al, byte ptr [ebp - 3]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x84]
        or     ecx, ecx
        jne    loc_1_1194
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
loc_1_1194:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_11A4
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_11A4:
        mov    al, byte ptr [esi + 0x89]
        test   al, 0x80
        je     loc_1_11E3
        test   al, 4
        je     loc_1_11DB
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 4
        in     al, dx
        and    eax, 0xff
        and    eax, 0xfffffff0
        cmp    eax, 0x10
        jne    loc_1_11D5
        inc    dword ptr [esi + 0x6a4]
        inc    dword ptr [esi + 0x698]
        jmp    loc_1_11E9
loc_1_11D5:
        inc    dword ptr [esi + 0x6a8]
loc_1_11DB:
        inc    dword ptr [esi + 0x698]
        jmp    loc_1_11E9
loc_1_11E3:
        inc    dword ptr [esi + 0x6a0]
loc_1_11E9:
        cmp    byte ptr [ecx + 0x2c], 0
        jne    loc_1_1212
        mov    al, byte ptr [esi + 0x89]
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
        jmp    loc_1_1239
loc_1_1212:
        cmp    dword ptr [esi + 0x6bd], 0
        jne    loc_1_1223
        mov    dword ptr [esi + 0x6bd], ecx
        jmp    loc_1_122C
loc_1_1223:
        mov    eax, dword ptr [esi + 0x6c1]
        mov    dword ptr [eax + 0x24], ecx
loc_1_122C:
        mov    dword ptr [esi + 0x6c1], ecx
        mov    dword ptr [ecx + 0x24], 0
loc_1_1239:
        mov    dword ptr [esi + 0x84], 0
        cmp    byte ptr [esi + 0x6b0], 0
        je     loc_1_1260
        push   2
        push   esi
        call   sub_1_30D0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1260:
        push   esi
        call   sub_1_17E0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1270:
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
        jae    loc_1_12A0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12A0:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x6b9]
        add    edi, 0xe
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_12B4
        mov    edi, dword ptr [ebp - 4]
loc_1_12B4:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 0x90]
        push   eax
        call   sub_1_1330
        cmp    edi, 0xe
        jae    loc_1_12F0
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 0x90]
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
loc_1_12F0:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        sub    edi, 0xe
        push   edi
        lea    eax, [esi + 0x9e]
        push   eax
        push   0xe
        lea    eax, [esi + 0x90]
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
sub_1_1330:
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
        je     loc_1_1384
loc_1_1355:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     loc_1_1384
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1355
loc_1_1384:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1395
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1395:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0xc], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_13A4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_13B3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_13B6
loc_1_13B3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_13B6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        cmp    esi, edi
        je     loc_1_13F2
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_13FD
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        jmp    loc_1_13A4
loc_1_13F2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_13FD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1410:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 0x692]
        cmp    eax, 0x5ea
        jbe    loc_1_1430
        mov    eax, 3
        pop    ebp
        ret    4
        db  090h
loc_1_1430:
        mov    dword ptr [edx + 0x694], eax
        mov    ecx, dword ptr [edx + 0x6b9]
        add    ecx, 0xe
        cmp    ecx, eax
        jbe    loc_1_1445
        mov    ecx, eax
loc_1_1445:
        cmp    ecx, 0xe
        jae    loc_1_1469
        push   0
        push   0
        push   0
        push   ecx
        lea    eax, [edx + 0x90]
        push   eax
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 0x24]
        push   eax
        call   sub_1_00B0
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1469:
        sub    eax, 0xe
        push   eax
        sub    ecx, 0xe
        push   ecx
        lea    eax, [edx + 0x9e]
        push   eax
        push   0xe
        lea    eax, [edx + 0x90]
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
loc_1_14A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        add    esi, 0xe
        cmp    dword ptr [edi + 0x6c5], 0
        je     loc_1_1547
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, esi
        push   eax
        call   sub_1_0530
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_151A
loc_1_14DA:
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
        mov    eax, dword ptr [edi + 0x6c5]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_1330
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_151A
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_14DA
loc_1_151A:
        cmp    esi, ebx
        jbe    loc_1_1535
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1535:
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    ebx, esi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1547:
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_1557
        mov    edx, dword ptr [edi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_1557:
        mov    ax, word ptr [edi + 0x8e]
        or     ax, ax
        je     loc_1_1592
        and    eax, 0xffff
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        mov    eax, dword ptr [edi + 0x6b9]
        add    eax, edi
        add    eax, 0x9e
        push   eax
        mov    eax, dword ptr [edi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03F1
        mov    word ptr [edi + 0x8e], 0
loc_1_1592:
        mov    ecx, dword ptr [edi + 0x694]
        mov    eax, dword ptr [ebp + 0x14]
        lea    edx, [esi + eax]
        cmp    edx, ecx
        jbe    loc_1_15C7
        cmp    esi, ecx
        jbe    loc_1_15C0
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
loc_1_15C0:
        sub    ecx, esi
        mov    dword ptr [ebp - 0x18], ecx
        jmp    loc_1_15CA
loc_1_15C7:
        mov    dword ptr [ebp - 0x18], eax
loc_1_15CA:
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
        je     loc_1_164B
loc_1_15FA:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        mov    ebx, eax
        ja     loc_1_1607
        mov    ebx, dword ptr [ebp - 8]
loc_1_1607:
        push   ebx
        lea    eax, [esi + edi]
        add    eax, 0x90
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05EC
        add    esi, ebx
        sub    dword ptr [ebp - 0xc], ebx
        je     loc_1_164B
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_164B
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_15FA
loc_1_164B:
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
loc_1_1670:
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
        je     loc_1_16A0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16A0:
        cmp    byte ptr [esi + 0x6b0], 0
        je     loc_1_16B7
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16B7:
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [esi + 0x70]
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
        mov    ecx, dword ptr [esi + 0x64]
        lea    edx, [esi + 0x62]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    ecx, eax
        jae    loc_1_1703
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1737
loc_1_1703:
        jbe    loc_1_1710
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_1737
        db  08Bh, 0FFh
loc_1_1710:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jae    loc_1_1727
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1737
loc_1_1727:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_1737
        mov    dword ptr [ebp - 4], 0
loc_1_1737:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1757
        test   byte ptr [ebx + 0x1d], 2
        je     loc_1_1745
        jmp    loc_1_1757
loc_1_1745:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_0330
        mov    byte ptr [edi + 8], al
        jmp    loc_1_175B
loc_1_1757:
        mov    byte ptr [edi + 8], 0
loc_1_175B:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_1770
        mov    eax, dword ptr [esi + 0x80]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_1776
        db  090h
loc_1_1770:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x7c], ecx
loc_1_1776:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x80], ecx
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_17B0
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_17A0
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_17A0
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_17B0
loc_1_17A0:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        jmp    loc_1_17CE
loc_1_17B0:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_17CE
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_17CE
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_17CE
        push   esi
        call   sub_1_17E0
loc_1_17CE:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_17FC
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_17FC:
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_198F
        mov    di, word ptr [ebp - 4]
        sub    ebx, ebx
        nop    
loc_1_1810:
        mov    eax, dword ptr [esi + 0x7c]
        cmp    eax, ebx
        je     loc_1_198F
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x7c], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x84], eax
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
        jb     loc_1_184E
        mov    eax, dword ptr [ebp - 0x14]
loc_1_184E:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 8
        cmp    dword ptr [ebp - 8], ebx
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        je     loc_1_18F8
loc_1_1863:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
        cmp    dword ptr [ebp - 0xc], ebx
        jne    loc_1_187B
        jmp    loc_1_18E2
loc_1_187B:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_18B0
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        inc    dword ptr [ebp - 0x10]
        shl    ax, 8
        dec    dword ptr [ebp - 0xc]
        or     di, ax
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 8
        cmp    dword ptr [ebp - 0xc], ebx
        mov    ax, di
        out    dx, ax
        mov    byte ptr [ebp - 1], bl
        jne    loc_1_18B0
        jmp    loc_1_18E2
        db  08Dh, 049h, 000h
loc_1_18B0:
        cmp    dword ptr [ebp - 0xc], 1
        jbe    loc_1_18CC
        mov    eax, dword ptr [ebp - 0xc]
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_18CC:
        test   byte ptr [ebp - 0xc], 1
        je     loc_1_18E2
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  di, byte ptr [eax + ecx - 1]
        mov    byte ptr [ebp - 1], 1
loc_1_18E2:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_1863
loc_1_18F8:
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_1911
        or     di, 0x2000
        mov    edx, dword ptr [esi + 0x34]
        inc    dword ptr [ebp - 0x14]
        add    dx, 8
        mov    ax, di
        out    dx, ax
loc_1_1911:
        cmp    dword ptr [ebp - 0x14], 0x3c
        jae    loc_1_1933
        mov    eax, 0x3c
        sub    eax, dword ptr [ebp - 0x14]
        shr    eax, 1
        push   eax
        push   0x35e0                        ; -> loc_1_35E0
        mov    eax, dword ptr [esi + 0x34]
        add    eax, 8
        push   eax
        call   sub_1_03B4
loc_1_1933:
        test   byte ptr [esi + 0x8a], 0x20
        je     loc_1_1942
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [eax + 0x2c], bl
loc_1_1942:
        cmp    byte ptr [esi + 0x6b0], bl
        jne    loc_1_1969
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0xa
        mov    al, 0x81
        out    dx, al
        cmp    dword ptr [esi + 0x84], ebx
        je     loc_1_1810
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1969:
        mov    eax, dword ptr [ebp - 0x18]
        push   0
        mov    edi, dword ptr [eax + 0x28]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        mov    dword ptr [esi + 0x84], 0
        push   3
        push   esi
        call   sub_1_30D0
        dec    dword ptr [edi + 0x18]
loc_1_198F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_19A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x6fa], 0
        je     loc_1_19D7
        cmp    dword ptr [esi + 0x84], 0
        je     loc_1_19E7
        mov    byte ptr [esi + 0x88], 1
        mov    byte ptr [esi + 0x6fa], 0
        push   0
        lea    eax, [esi + 0x6ee]
        push   eax
        call   sub_1_04E2
        jmp    loc_1_19E7
loc_1_19D7:
        cmp    dword ptr [esi + 0x84], 0
        je     loc_1_19E7
        mov    byte ptr [esi + 0x6fa], 1
loc_1_19E7:
        push   0x7d0
        add    esi, 0x6e2
        push   esi
        call   sub_1_04E2
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
sub_1_1A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        mov    word ptr [ebp - 0x14], 7
        mov    word ptr [ebp - 0x12], 7
        mov    dword ptr [ebp - 0x10], 0x3700 ; -> loc_1_3700
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_041A
        mov    eax, dword ptr [0x361c]       ; -> obj1:0x361C
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_1_1A51
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A51:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x4c], 3
        mov    byte ptr [ebp - 0x4b], 0
        mov    dword ptr [ebp - 0x4a], 0
        mov    dword ptr [ebp - 0x46], 0x2090 ; -> loc_1_2090
        mov    dword ptr [ebp - 0x42], 0x2290 ; -> loc_1_2290
        mov    dword ptr [ebp - 0x3e], 0x1670 ; -> loc_1_1670
        mov    dword ptr [ebp - 0x3a], 0x14a0 ; -> loc_1_14A0
        mov    dword ptr [ebp - 0x36], 0x2de0 ; -> loc_1_2DE0
        mov    dword ptr [ebp - 0x32], 0x23f0 ; -> loc_1_23F0
        mov    dword ptr [ebp - 0x2e], 0x2c80 ; -> loc_1_2C80
        mov    dword ptr [ebp - 0x2a], 0x2d50 ; -> loc_1_2D50
        mov    dword ptr [ebp - 0x26], 0x1b00 ; -> loc_1_1B00
        mov    dword ptr [ebp - 0x22], 0x2d80 ; -> loc_1_2D80
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
        jne    loc_1_1AE6
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1AE6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x70
        push   esi
        push   edi
        mov    word ptr [ebp - 0x48], 0xc
        mov    word ptr [ebp - 0x46], 0xc
        mov    dword ptr [ebp - 0x44], 0x3714 ; -> obj1:0x3714
        mov    word ptr [ebp - 0x40], 0xe
        mov    word ptr [ebp - 0x3e], 0xe
        mov    dword ptr [ebp - 0x3c], 0x3730 ; -> obj1:0x3730
        mov    word ptr [ebp - 0x30], 0x10
        mov    word ptr [ebp - 0x2e], 0x10
        mov    dword ptr [ebp - 0x2c], 0x3778 ; -> obj1:0x3778
        mov    word ptr [ebp - 0x28], 0x15
        mov    word ptr [ebp - 0x26], 0x15
        mov    dword ptr [ebp - 0x24], 0x37a0 ; -> loc_1_37A0
        mov    word ptr [ebp - 0x38], 0xc
        mov    word ptr [ebp - 0x36], 0xc
        mov    dword ptr [ebp - 0x34], 0x37c4 ; -> loc_1_37C4
        mov    eax, dword ptr [0x361c]       ; -> obj1:0x361C
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x6fb
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_1B90
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_1B90:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0x1be
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
        jne    loc_1_1D12
        push   0
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_058A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1D09
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x28]
        cmp    eax, 1
        je     loc_1_1C07
        cmp    eax, 2
        jne    loc_1_1D09
loc_1_1C07:
        push   1
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
        jne    loc_1_1D09
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x30], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x28], 2
        jne    loc_1_1CB2
        mov    word ptr [ebp - 0x70], 0
        lea    edi, [ebp - 0x6e]
        sub    eax, eax
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0x20
        lea    eax, [ebp - 0x70]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04D4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1D09
        mov    eax, dword ptr [ebp - 0x68]
        sub    eax, dword ptr [ebp - 0x6c]
        inc    eax
        cmp    eax, 0x1000
        jb     loc_1_1C8F
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x6c]
        mov    dword ptr [ecx + 0x38], eax
loc_1_1C8F:
        mov    eax, dword ptr [ebp - 0x58]
        sub    eax, dword ptr [ebp - 0x5c]
        inc    eax
        cmp    eax, 0x4000
        jne    loc_1_1CA8
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x5c]
        mov    dword ptr [ecx + 0x3c], eax
        jmp    loc_1_1CB2
loc_1_1CA8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x3c], 0
loc_1_1CB2:
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
        je     loc_1_1CE0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x4d], 0
        jmp    loc_1_1CEC
        db  005h, 000h, 000h, 000h, 000h
loc_1_1CE0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x4d], eax
loc_1_1CEC:
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
        je     loc_1_1D30
loc_1_1D09:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_1_1D12:
        push   0
        push   0x6fb
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_1_1D30:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x48], eax
        lea    eax, [ebp - 0x28]
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
        jne    loc_1_1D67
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x78], eax
        jmp    loc_1_1D71
loc_1_1D67:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x78], 0x10
loc_1_1D71:
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
        jne    loc_1_1DAD
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_1_1DAD
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, ecx
        add    edx, 0x62
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], al
loc_1_1DAD:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1DE0
        or     eax, eax
        jne    loc_1_1D12
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x361c]       ; -> obj1:0x361C
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x26
        push   eax
        call   sub_1_0506
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_2037
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
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [ecx + 0x1a], eax
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_1_1E50
        cmp    dword ptr [esi + 0x2c], 0
        mov    eax, dword ptr [ebp - 8]
        je     loc_1_1E46
        jmp    loc_1_1E53
loc_1_1E46:
        mov    dword ptr [eax + 0x1e], 0x10
        jmp    loc_1_1E5A
        db  090h
loc_1_1E50:
        mov    eax, dword ptr [ebp - 8]
loc_1_1E53:
        mov    dword ptr [eax + 0x1e], 0x20
loc_1_1E5A:
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 0x34]
        mov    dword ptr [ecx + 0x22], eax
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
        jne    loc_1_2037
        mov    dword ptr [esi + 0x6d6], 0
        mov    dword ptr [esi + 0x6da], 0x32c0 ; -> loc_1_32C0
        mov    dword ptr [esi + 0x6de], esi
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_1_1F12
        push   0x1000
        mov    eax, dword ptr [esi + 0x38]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0x40]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1EE2
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2027
loc_1_1EE2:
        lea    ecx, [esi + 0x44]
        cmp    dword ptr [ecx], 0
        je     loc_1_1F12
        push   0x4000
        mov    eax, dword ptr [esi + 0x3c]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_05D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1F12
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2027
loc_1_1F12:
        push   esi
        call   sub_1_06E0
        or     al, al
        jne    loc_1_1F28
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2027
loc_1_1F28:
        push   esi
        call   sub_1_0810
        lea    ecx, [esi + 0x62]
        cmp    byte ptr [ecx], 0
        jne    loc_1_1F68
        cmp    byte ptr [esi + 0x63], 0
        jne    loc_1_1F68
        cmp    byte ptr [esi + 0x64], 0
        jne    loc_1_1F68
        cmp    byte ptr [esi + 0x65], 0
        jne    loc_1_1F68
        cmp    byte ptr [esi + 0x66], 0
        jne    loc_1_1F68
        cmp    byte ptr [esi + 0x67], 0
        jne    loc_1_1F68
        mov    eax, dword ptr [esi + 0x5c]
        lea    edx, [esi + 0x5c]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
loc_1_1F68:
        lea    ebx, [esi + 0x24]
        push   ebx
        push   esi
        push   ecx
        push   0x32b0                        ; -> loc_1_32B0
        push   0x3230                        ; -> loc_1_3230
        push   0x31b0                        ; -> loc_1_31B0
        mov    eax, dword ptr [esi + 0x78]
        push   eax
        call   sub_2_0000
        or     al, al
        jne    loc_1_1FB0
        push   3
        push   2
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0x14
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2027
        db  08Dh, 064h, 024h, 000h
loc_1_1FB0:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_1_1FBC
        push   esi
        call   sub_1_0770
loc_1_1FBC:
        push   esi
        call   sub_1_0870
        or     al, al
        jne    loc_1_1FD0
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_201F
        db  090h
loc_1_1FD0:
        push   esi
        call   sub_1_0A80
        or     al, al
        jne    loc_1_1FE3
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_201F
loc_1_1FE3:
        push   1
        push   0
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        push   eax
        push   0xdd0                         ; -> loc_1_0DD0
        push   esi
        push   0xdb0                         ; -> loc_1_0DB0
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04EE
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2043
        push   0
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_05D8
        add    esp, 0xc
loc_1_201F:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_2_0120
loc_1_2027:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_049E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0438
loc_1_2037:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2043:
        mov    dword ptr [esi + 0x6ee], 0
        mov    dword ptr [esi + 0x6f2], 0xdd0 ; -> loc_1_0DD0
        lea    eax, [esi + 0x6e2]
        mov    dword ptr [esi + 0x6f6], esi
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x6e6], 0x19a0 ; -> loc_1_19A0
        mov    dword ptr [esi + 0x6ea], esi
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
        db  090h
loc_1_2090:
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
loc_1_20B3:
        or     edx, edx
        je     loc_1_20C2
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_20B3
        mov    dword ptr [ecx], edx
loc_1_20C2:
        cmp    dword ptr [ecx], -1
        jne    loc_1_20D4
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_20D4:
        mov    eax, dword ptr [0x361c]       ; -> obj1:0x361C
        push   eax
        push   0
        push   0x71
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
        or     eax, eax
        je     loc_1_2112
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
loc_1_2112:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x70]
        cmp    dword ptr [esi + 0x68], 0
        jne    loc_1_212A
        cmp    dword ptr [esi + 0x6c], 0
        jne    loc_1_212A
        push   esi
        call   sub_1_0A00
loc_1_212A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x68], eax
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_2212
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
        je     loc_1_2212
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
        mov    dword ptr [esi + 0x6b9], 0xee
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_21EA
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_21D2
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_21D2
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_21EA
loc_1_21D2:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_21EA:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_2208
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_2208
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_2208
        push   esi
        call   sub_1_17E0
loc_1_2208:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_2212:
        dec    dword ptr [esi + 0x70]
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x68], eax
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
        db  090h
sub_1_2250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edx, dword ptr [eax + 0x68]
        cmp    edx, ecx
        je     loc_1_2270
loc_1_2260:
        mov    esi, dword ptr [edx + 0x14]
        cmp    ecx, esi
        jae    loc_1_2269
        mov    ecx, esi
loc_1_2269:
        mov    edx, dword ptr [edx + 0xc]
        or     edx, edx
        jne    loc_1_2260
loc_1_2270:
        or     ecx, ecx
        jne    loc_1_2279
        mov    ecx, 0xee
loc_1_2279:
        mov    dword ptr [eax + 0x6b9], ecx
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_22B3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_22B3:
        inc    dword ptr [esi + 0x70]
        inc    dword ptr [ebx + 0x18]
        mov    ecx, dword ptr [esi + 0x68]
        cmp    ebx, ecx
        jne    loc_1_22C8
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x68], eax
        jmp    loc_1_22E1
loc_1_22C8:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_22D8
loc_1_22D0:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_22D0
loc_1_22D8:
        lea    eax, [ebx + 0xc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_22E1:
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_0240
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2318
        mov    eax, dword ptr [ebx + 0x18]
        cmp    eax, 2
        je     loc_1_2310
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        mov    edi, 0x103
        jmp    loc_1_2344
        db  08Dh, 049h, 000h
loc_1_2310:
        sub    eax, 2
        mov    dword ptr [ebx + 0x18], eax
        jmp    loc_1_2344
loc_1_2318:
        cmp    edi, 0x103
        jne    loc_1_2330
        mov    byte ptr [ebx + 0x1c], 1
        sub    dword ptr [ebx + 0x18], 2
        jmp    loc_1_2344
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2330:
        cmp    edi, 0xc001000e
        jne    loc_1_2341
        mov    byte ptr [ebx + 0x1c], 1
        mov    edi, 0x103
loc_1_2341:
        dec    dword ptr [ebx + 0x18]
loc_1_2344:
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_2379
        mov    eax, dword ptr [esi + 0x6b9]
        cmp    dword ptr [ebx + 0x14], eax
        jne    loc_1_235B
        push   esi
        call   sub_1_2250
loc_1_235B:
        push   0
        push   0x71
        push   ebx
        call   sub_1_050C
        cmp    dword ptr [esi + 0x68], 0
        jne    loc_1_2387
        cmp    dword ptr [esi + 0x6c], 0
        jne    loc_1_2387
        push   esi
        call   sub_1_0A50
        jmp    loc_1_2387
loc_1_2379:
        mov    eax, dword ptr [esi + 0x6c]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0x6c], ebx
        mov    edi, 0x103
loc_1_2387:
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_23C1
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_23A8
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_23A8
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_23C1
loc_1_23A8:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23C1:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_23DF
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_23DF
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_23DF
        push   esi
        call   sub_1_17E0
loc_1_23DF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [ebx + 0x18]
        inc    dword ptr [edi + 0x70]
        cmp    byte ptr [ebx + 0x1c], 0
        je     loc_1_2410
        mov    esi, 0xc0010002
        jmp    loc_1_244C
        db  090h
loc_1_2410:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        or     eax, eax
        jne    loc_1_2426
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2870
        mov    esi, eax
        jmp    loc_1_244C
loc_1_2426:
        cmp    eax, 1
        jne    loc_1_2447
        cmp    byte ptr [edi + 0x6b0], 0
        je     loc_1_243B
        mov    esi, 0xc001000d
        jmp    loc_1_244C
loc_1_243B:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_28C0
        mov    esi, eax
        jmp    loc_1_244C
loc_1_2447:
        mov    esi, 0x10001
loc_1_244C:
        dec    dword ptr [ebx + 0x18]
        mov    eax, dword ptr [edi + 0x70]
        dec    eax
        mov    dword ptr [edi + 0x70], eax
        jne    loc_1_2487
        cmp    byte ptr [edi + 0x6b0], 0
        jne    loc_1_2470
        cmp    dword ptr [edi + 0x6c9], 0
        jne    loc_1_2470
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_2487
loc_1_2470:
        push   1
        add    edi, 0x6d6
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2487:
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_1_24A5
        cmp    dword ptr [edi + 0x84], 0
        jne    loc_1_24A5
        cmp    byte ptr [edi + 0x6d5], 0
        jne    loc_1_24A5
        push   edi
        call   sub_1_17E0
loc_1_24A5:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_24B0:
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
        ja     loc_1_2500
        je     loc_1_26D0
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2521
        shl    eax, 2
        jmp    loc_1_281C
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2500:
        cmp    eax, 0x1010102
        je     loc_1_26F0
        cmp    eax, 0x1010103
        je     loc_1_2710
        cmp    eax, 0x1010104
        je     loc_1_27C9
loc_1_2521:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_27D2
        db  08Dh, 049h, 000h
loc_1_2530:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2545
        mov    esi, 0x369c                   ; -> loc_1_369C
        mov    edi, 0x5c
        jmp    loc_1_27D2
loc_1_2545:
        mov    esi, 0x3620                   ; -> loc_1_3620
        mov    edi, 0x7c
        jmp    loc_1_27D2
loc_1_2554:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x6b0], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_1_256E
        mov    dword ptr [ebp - 0x14], 0
loc_1_256E:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_27D2
        db  005h, 000h, 000h, 000h, 000h
loc_1_2580:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_27D2
        db  08Dh, 049h, 000h
loc_1_2590:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25A0:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25B0:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_25C0:
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x84], 1
        sbb    eax, eax
        and    eax, 0x5ee
        jmp    loc_1_27CF
loc_1_25D6:
        mov    dword ptr [ebp - 8], 0x2000
        jmp    loc_1_27D2
loc_1_25E2:
        mov    dword ptr [ebp - 8], 0x5ee
        jmp    loc_1_27D2
        db  08Bh, 0FFh
loc_1_25F0:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_2600:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x5c
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        and    dword ptr [ebp - 8], 0xffffff00
        lea    esi, [ebp - 8]
        mov    edi, 4
        jmp    loc_1_27D2
loc_1_2626:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x28]
        cmp    eax, 2
        jne    loc_1_2640
        mov    esi, 0x37d0                   ; -> loc_1_37D0
        mov    edi, 0x23
        jmp    loc_1_27D2
loc_1_2640:
        cmp    eax, 1
        jne    loc_1_27D2
        mov    esi, 0x37f4                   ; -> loc_1_37F4
        mov    edi, 0x1f
        jmp    loc_1_27D2
loc_1_2658:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2670
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_27CF
        db  08Dh, 064h, 024h, 000h
loc_1_2670:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_1_27CF
        db  08Bh, 0FFh
loc_1_2680:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2694
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x6b9]
        jmp    loc_1_27CF
loc_1_2694:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        jmp    loc_1_27CF
        db  090h
loc_1_26A0:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edi, 2
        jmp    loc_1_27D2
loc_1_26B3:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_27D2
        db  090h
loc_1_26C0:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_27D2
        db  08Dh, 064h, 024h, 000h
loc_1_26D0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x5c
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_27D2
        db  090h
loc_1_26F0:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x62
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_05EC
        lea    esi, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_1_27D2
        db  090h
loc_1_2710:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_276A
        mov    eax, dword ptr [ebx + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2742
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_27D2
loc_1_2742:
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
        jmp    loc_1_27D2
loc_1_276A:
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
        jae    loc_1_279A
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_1_27D2
loc_1_279A:
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
        jmp    loc_1_27D2
loc_1_27C9:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x78]
loc_1_27CF:
        mov    dword ptr [ebp - 8], eax
loc_1_27D2:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_280E
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_280E
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_27FE
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
loc_1_27FE:
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05EC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_280E:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
loc_1_281C:
        xor    byte ptr [0x25540000], ah     ; -> loc_1_2554
        add    byte ptr [eax], al
        and    byte ptr [0x25800000], 0      ; -> loc_1_2580
        add    byte ptr [eax - 0x5fffffdb], dl ; -> loc_1_2590
        and    eax, 0x25b00000               ; -> loc_1_25B0
        add    byte ptr [eax], al
        shl    byte ptr [0x25d60000], 0      ; -> loc_1_25D6
        add    dl, ah                        ; -> loc_1_25E2
        and    eax, 0x25f00000               ; -> loc_1_25F0
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [eax], al
        add    byte ptr es:[eax], al
        pop    eax
        add    byte ptr es:[eax], al
        and    byte ptr [esi], 0
        add    byte ptr [eax - 0x4cffffda], ah ; -> loc_1_26A0
        add    byte ptr es:[eax], al
        and    dword ptr [0x26c00000], esp   ; -> loc_1_26C0
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
sub_1_2870:
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
        call   sub_1_24B0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_28C0:
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
        je     loc_1_2917
        cmp    eax, 0x1010f
        je     loc_1_2978
        cmp    eax, 0x10112
        je     loc_1_2A07
        cmp    eax, 0x1010103
        je     loc_1_2A34
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2A75
loc_1_2917:
        cmp    edi, 4
        je     loc_1_2934
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 4
        jmp    loc_1_2A75
loc_1_2934:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_2961
        mov    ebx, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2A75
loc_1_2961:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_1_2A90
        jmp    loc_1_2A73
loc_1_2978:
        cmp    edi, 4
        je     loc_1_2995
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2A75
loc_1_2995:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05EC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0xfffffffe
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_2A00
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x6b9]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_29D6
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x6b9], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_2A75
loc_1_29D6:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x14], eax
        jne    loc_1_29F7
        mov    eax, dword ptr [ecx + 0x14]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_29F7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        push   edx
        call   sub_1_2250
        jmp    loc_1_2A75
loc_1_29F7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_2A75
        db  090h
loc_1_2A00:
        mov    ebx, 0xc0010014
        jmp    loc_1_2A75
loc_1_2A07:
        cmp    edi, 4
        je     loc_1_2A21
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2A75
loc_1_2A21:
        push   4
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x1d
        push   ecx
        call   sub_1_05EC
        sub    ebx, ebx
        jmp    loc_1_2A75
loc_1_2A34:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_1_2A58
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2A75
loc_1_2A58:
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
        call   sub_1_2AF0
loc_1_2A73:
        mov    ebx, eax
loc_1_2A75:
        or     ebx, ebx
        jne    loc_1_2A83
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_2A83:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_2A90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6b0], 0
        jne    loc_1_2ADA
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2AD0
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
loc_1_2AD0:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_2ADA:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2AF0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x6b0], 0
        jne    loc_1_2B3A
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_2B30
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
loc_1_2B30:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_1_2B3A:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B50:
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
        call   sub_1_24B0
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_1_2C66
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2BD6
        je     loc_1_2C20
        cmp    eax, 0x20101
        je     loc_1_2BF2
        cmp    eax, 0x20102
        je     loc_1_2C00
        cmp    eax, 0x20103
        je     loc_1_2C08
        cmp    eax, 0x20104
        je     loc_1_2C10
        mov    edi, 0xc0010017
        jmp    loc_1_2C49
loc_1_2BD6:
        cmp    eax, 0x1020101
        je     loc_1_2C28
        cmp    eax, 0x1020102
        je     loc_1_2C31
        cmp    eax, 0x1020103
        je     loc_1_2C40
        mov    edi, 0xc0010017
        jmp    loc_1_2C49
loc_1_2BF2:
        mov    eax, dword ptr [ebx + 0x698]
        jmp    loc_1_2C46
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2C00:
        mov    eax, dword ptr [ebx + 0x69c]
        jmp    loc_1_2C46
loc_1_2C08:
        mov    eax, dword ptr [ebx + 0x6a0]
        jmp    loc_1_2C46
loc_1_2C10:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2C49
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C20:
        mov    eax, dword ptr [ebx + 0x6ac]
        jmp    loc_1_2C46
loc_1_2C28:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_2C49
loc_1_2C31:
        mov    eax, dword ptr [ebx + 0x6a4]
        jmp    loc_1_2C46
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C40:
        mov    eax, dword ptr [ebx + 0x6a8]
loc_1_2C46:
        mov    dword ptr [ebp - 4], eax
loc_1_2C49:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_2C66
        or     edi, edi
        jne    loc_1_2C66
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_05EC
        add    dword ptr [ebp - 8], 4
loc_1_2C66:
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
loc_1_2C80:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2CB2
        cmp    eax, 0x20101
        jae    loc_1_2CD3
        cmp    eax, 0x10101
        jb     loc_1_2CCE
        cmp    eax, 0x10111
        jbe    loc_1_2CD3
        cmp    eax, 0x10113
        je     loc_1_2CD3
        jmp    loc_1_2CCE
loc_1_2CB2:
        cmp    eax, 0x1010101
        jb     loc_1_2CCE
        cmp    eax, 0x1010104
        jbe    loc_1_2CD3
        cmp    eax, 0x1020101
        jb     loc_1_2CCE
        cmp    eax, 0x1020103
        jbe    loc_1_2CD3
loc_1_2CCE:
        mov    esi, 0xc0010017
loc_1_2CD3:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x70]
        or     esi, esi
        jne    loc_1_2CE6
        push   ecx
        push   edi
        call   sub_1_2B50
        mov    esi, eax
loc_1_2CE6:
        mov    eax, dword ptr [edi + 0x70]
        dec    eax
        mov    dword ptr [edi + 0x70], eax
        jne    loc_1_2D20
        cmp    byte ptr [edi + 0x6b0], 0
        jne    loc_1_2D07
        cmp    dword ptr [edi + 0x6c9], 0
        jne    loc_1_2D07
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_2D20
loc_1_2D07:
        push   1
        add    edi, 0x6d6
        push   edi
        call   sub_1_04E2
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_2D20:
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_1_2D3E
        cmp    dword ptr [edi + 0x84], 0
        jne    loc_1_2D3E
        cmp    byte ptr [edi + 0x6d5], 0
        jne    loc_1_2D3E
        push   edi
        call   sub_1_17E0
loc_1_2D3E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D50:
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
loc_1_2D80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x6e2]
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
        push   0x6fb
        push   esi
        call   sub_1_050C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_2DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x1c], 0
        je     loc_1_2E03
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2E03:
        inc    dword ptr [esi + 0x18]
        inc    dword ptr [edi + 0x70]
        cmp    byte ptr [edi + 0x6b0], 0
        je     loc_1_2E26
        dec    dword ptr [esi + 0x18]
        dec    dword ptr [edi + 0x70]
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2E26:
        mov    ebx, dword ptr [edi + 0x68]
        mov    dword ptr [ebp - 4], 0
        or     ebx, ebx
        je     loc_1_2E56
loc_1_2E34:
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
        jne    loc_1_2E34
loc_1_2E56:
        mov    byte ptr [edi + 0x6b0], 1
        mov    dword ptr [edi + 0x6b1], 0
        mov    dword ptr [edi + 0x6b5], esi
        push   edi
        call   sub_1_2EE0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 0x18]
        mov    eax, dword ptr [edi + 0x70]
        dec    eax
        mov    dword ptr [edi + 0x70], eax
        jne    loc_1_2EB4
        cmp    byte ptr [edi + 0x6b0], 0
        jne    loc_1_2E9A
        cmp    dword ptr [edi + 0x6c9], 0
        jne    loc_1_2E9A
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_2EB4
loc_1_2E9A:
        push   1
        add    edi, 0x6d6
        push   edi
        call   sub_1_04E2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2EB4:
        cmp    dword ptr [edi + 0x7c], 0
        je     loc_1_2ED2
        cmp    dword ptr [edi + 0x84], 0
        jne    loc_1_2ED2
        cmp    byte ptr [edi + 0x6d5], 0
        jne    loc_1_2ED2
        push   edi
        call   sub_1_17E0
loc_1_2ED2:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_2EE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x70]
        cmp    dword ptr [esi + 0x6c9], 0
        je     loc_1_2F2C
loc_1_2EF4:
        mov    ecx, dword ptr [esi + 0x6c9]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x6c9], eax
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        je     loc_1_2F20
        cmp    eax, 8
        je     loc_1_2F20
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_045C
loc_1_2F20:
        dec    dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_2EF4
loc_1_2F2C:
        cmp    dword ptr [esi + 0x84], 0
        je     loc_1_2F50
        mov    dword ptr [esi + 0x6b1], 2
        dec    dword ptr [esi + 0x70]
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2F50:
        dec    dword ptr [esi + 0x70]
        push   esi
        call   sub_1_2F60
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_2F60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x70]
        inc    ecx
        mov    dword ptr [eax + 0x70], ecx
        cmp    dword ptr [eax + 0x84], 0
        je     loc_1_2F8C
        mov    dword ptr [eax + 0x6b1], 3
        dec    dword ptr [eax + 0x70]
        mov    eax, 0x103
        pop    ebp
        ret    4
loc_1_2F8C:
        dec    ecx
        mov    dword ptr [eax + 0x70], ecx
        push   eax
        call   sub_1_2FA0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x70]
        mov    ecx, dword ptr [esi + 0x84]
        or     ecx, ecx
        je     loc_1_2FD7
        mov    edi, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0x84], 0
loc_1_2FD7:
        mov    edx, dword ptr [esi + 0x7c]
        or     edx, edx
        je     loc_1_3004
        mov    edi, edi
loc_1_2FE0:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x7c], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x7c]
        or     edx, edx
        jne    loc_1_2FE0
loc_1_3004:
        mov    edx, dword ptr [esi + 0x6bd]
        or     edx, edx
        je     loc_1_303A
        mov    edi, edi
loc_1_3010:
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0x6bd], ecx
        mov    edi, dword ptr [eax + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_044A
        dec    dword ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x6bd]
        or     edx, edx
        jne    loc_1_3010
loc_1_303A:
        cmp    byte ptr [esi + 0x6d5], 0
        je     loc_1_3056
loc_1_3043:
        push   0x2710
        call   sub_1_0500
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_3043
loc_1_3056:
        push   esi
        call   sub_1_0BB0
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x6b0], 0
        mov    edi, dword ptr [esi + 0x68]
        or     edi, edi
        je     loc_1_30B6
        sub    ebx, ebx
loc_1_3078:
        inc    dword ptr [edi + 0x18]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_308F
        push   ebx
        push   ebx
        push   0x40010007
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_046E
loc_1_308F:
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
        jne    loc_1_3078
loc_1_30B6:
        dec    dword ptr [esi + 0x70]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x6b1], eax
        jne    loc_1_31A8
        cmp    eax, 1
        je     loc_1_3150
        cmp    eax, 2
        je     loc_1_3158
        cmp    eax, 3
        je     loc_1_3160
        mov    eax, dword ptr [ebp - 4]
loc_1_30FB:
        cmp    eax, 0x103
        je     loc_1_311A
        push   eax
        mov    eax, dword ptr [esi + 0x6b5]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0456
        mov    eax, dword ptr [esi + 0x6b5]
        dec    dword ptr [eax + 0x18]
loc_1_311A:
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_318A
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_313B
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_313B
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_318A
loc_1_313B:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3150:
        push   esi
        call   sub_1_2EE0
        jmp    loc_1_30FB
loc_1_3158:
        push   esi
        call   sub_1_2F60
        jmp    loc_1_30FB
loc_1_3160:
        mov    eax, dword ptr [esi + 0x70]
        inc    eax
        mov    dword ptr [esi + 0x70], eax
        cmp    dword ptr [esi + 0x84], 0
        je     loc_1_317B
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_317B:
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        push   esi
        call   sub_1_2FA0
        jmp    loc_1_30FB
loc_1_318A:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_31A8
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_31A8
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_31A8
        push   esi
        call   sub_1_17E0
loc_1_31A8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_31B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_31C3
        lea    edx, [eax + 0x49]
loc_1_31C3:
        cmp    byte ptr [ecx + 0x6b0], 0
        je     loc_1_31D5
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x1c
loc_1_31D5:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6c9], 0
        jne    loc_1_3210
        mov    dword ptr [ecx + 0x6cd], edx
        mov    dword ptr [ecx + 0x6c9], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_3210:
        mov    eax, dword ptr [ecx + 0x6cd]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6cd], edx
        mov    eax, 0x103
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3230:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [eax + 8]
        or     edx, edx
        jne    loc_1_3243
        lea    edx, [eax + 0x21]
loc_1_3243:
        cmp    byte ptr [ecx + 0x6b0], 0
        je     loc_1_3255
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_3255:
        mov    dword ptr [edx + 4], eax
        inc    dword ptr [eax + 0x18]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 0x6c9], 0
        jne    loc_1_3290
        mov    dword ptr [ecx + 0x6cd], edx
        mov    dword ptr [ecx + 0x6c9], edx
        mov    eax, 0x103
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_3290:
        mov    eax, dword ptr [ecx + 0x6cd]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x6cd], edx
        mov    eax, 0x103
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_32B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x18]
        ret    4
        db  08Dh, 049h, 000h
loc_1_32C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x70]
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_3376
        cmp    dword ptr [esi + 0x6d1], 0
        jne    loc_1_3376
loc_1_32E6:
        mov    edi, dword ptr [esi + 0x6c9]
        or     edi, edi
        je     loc_1_3351
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x6c9], eax
        cmp    dword ptr [esi + 0x6cd], edi
        jne    loc_1_330A
        mov    dword ptr [esi + 0x6cd], 0
loc_1_330A:
        mov    dword ptr [esi + 0x6d1], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_3325
        cmp    eax, 0xa
        je     loc_1_3325
        push   esi
        call   sub_1_3450
        jmp    loc_1_332B
loc_1_3325:
        push   esi
        call   sub_1_34C0
loc_1_332B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_3345
        cmp    ecx, 4
        je     loc_1_3345
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_045C
loc_1_3345:
        dec    dword ptr [ebx + 0x18]
        cmp    byte ptr [esi + 0x6b0], 0
        je     loc_1_32E6
loc_1_3351:
        mov    dword ptr [esi + 0x6d1], 0
        cmp    byte ptr [esi + 0x6b0], 0
        je     loc_1_3376
        dec    dword ptr [esi + 0x70]
        push   1
        push   esi
        call   sub_1_30D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3376:
        mov    edi, dword ptr [esi + 0x6c]
        or     edi, edi
        je     loc_1_33F1
        sub    ebx, ebx
        nop    
loc_1_3380:
        cmp    dword ptr [edi + 0x18], 0
        jbe    loc_1_33A0
        cmp    dword ptr [esi + 0x84], 0
        je     loc_1_3395
        push   esi
        call   sub_1_1160
loc_1_3395:
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_33DB
        db  08Dh, 064h, 024h, 000h
loc_1_33A0:
        mov    eax, dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0x6b9], eax
        jne    loc_1_33B1
        push   esi
        call   sub_1_2250
loc_1_33B1:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0444
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0xc]
        lea    ecx, [edi + 0xc]
        je     loc_1_33CA
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_33CD
loc_1_33CA:
        mov    dword ptr [esi + 0x6c], eax
loc_1_33CD:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   0x71
        push   eax
        call   sub_1_050C
loc_1_33DB:
        or     edi, edi
        jne    loc_1_3380
        cmp    dword ptr [esi + 0x6c], 0
        jne    loc_1_33F1
        cmp    dword ptr [esi + 0x68], 0
        jne    loc_1_33F1
        push   esi
        call   sub_1_0A50
loc_1_33F1:
        mov    eax, dword ptr [esi + 0x70]
        dec    eax
        mov    dword ptr [esi + 0x70], eax
        jne    loc_1_3427
        cmp    byte ptr [esi + 0x6b0], 0
        jne    loc_1_3412
        cmp    dword ptr [esi + 0x6c9], 0
        jne    loc_1_3412
        cmp    dword ptr [esi + 0x6c], 0
        je     loc_1_3427
loc_1_3412:
        push   1
        add    esi, 0x6d6
        push   esi
        call   sub_1_04E2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3427:
        cmp    dword ptr [esi + 0x7c], 0
        je     loc_1_3445
        cmp    dword ptr [esi + 0x84], 0
        jne    loc_1_3445
        cmp    byte ptr [esi + 0x6d5], 0
        jne    loc_1_3445
        push   esi
        call   sub_1_17E0
loc_1_3445:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
sub_1_3450:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x24]
        mov    eax, dword ptr [eax + 0xc]
        test   al, 0x20
        mov    dword ptr [esi + 0x8a], eax
        je     loc_1_3470
        lea    ecx, [esi + 0x53]
        mov    byte ptr [ecx], 7
        jmp    loc_1_3499
        db  090h
loc_1_3470:
        test   al, 4
        push   esi
        je     loc_1_3480
        call   sub_1_0CF0
        jmp    loc_1_3485
        db  08Dh, 064h, 024h, 000h
loc_1_3480:
        call   sub_1_34C0
loc_1_3485:
        cmp    dword ptr [esi + 0x8a], 0
        lea    ecx, [esi + 0x53]
        je     loc_1_3496
        mov    byte ptr [ecx], 6
        jmp    loc_1_3499
loc_1_3496:
        mov    byte ptr [ecx], 4
loc_1_3499:
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_34A9
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 0x10
        mov    al, 5
        out    dx, al
loc_1_34A9:
        mov    edx, dword ptr [esi + 0x34]
        add    dx, 5
        mov    al, byte ptr [ecx]
        out    dx, al
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_34C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C70
        push   esi
        call   sub_1_0D50
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
loc_1_35C0:
        inc    eax
        xor    eax, 0x35800000               ; -> obj1:0x3580
        add    byte ptr [eax], al
loc_1_35C8:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x6d6f
loc_1_35D0:
        inc    ebp
        je     loc_1_363B
        jb     0x3644
        je     loc_1_3604
        dec    ebp
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_35E0:
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
loc_1_3604:
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
loc_1_3620:
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
loc_1_363B:
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
loc_1_369C:
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
        inc    ebp
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
loc_1_3700:
        inc    ebx
        inc    ebp
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
        inc    ecx
        popal  
        jo     0x3781
        jb     0x3764
        jns    loc_1_3782
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_378D
        jb     loc_1_3770
        jns    0x378e
        add    byte ptr gs:[ecx + 0x6f], cl
        inc    edx
        popal  
        jae    0x378b
        inc    ecx
        jb     loc_1_3790
        jae    loc_1_37A0
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], cl
        inc    edx
        popal  
        jae    0x379b
        inc    ecx
        jb     loc_1_37A0
        jae    0x37b0
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x37bf
        inc    edx
        popal  
        jae    loc_1_37AF
        inc    ecx
        jb     0x37b4
        jae    loc_1_37C4
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x65], cl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x37d3
        inc    edx
        popal  
        jae    0x37c3
        inc    ecx
        jb     0x37c8
        jae    loc_1_37D8
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_37D1
        jb     0x37e0
        jne    0x37e0
loc_1_3770:
        je     0x37c0
        jne    loc_1_37E1
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_37E1
        jb     loc_1_37F0
        jne    loc_1_37F0
        je     loc_1_37D0
loc_1_3782:
        jne    0x37f1
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ebp + 0x61], cl
        js     loc_1_37F5
        insd   dword ptr es:[edi], dx
loc_1_378D:
        jne    0x37fc
        dec    ebp
loc_1_3790:
        jne    loc_1_37FE
        je     0x37fd
        arpl   word ptr [ecx + 0x73], sp
        je     0x37e5
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_37A0:
        dec    ebp
        popal  
        js     loc_1_380D
        insd   dword ptr es:[edi], dx
        jne    0x3814
        dec    ebp
        jne    0x3816
        je     0x3815
        arpl   word ptr [ecx + 0x73], sp
loc_1_37AF:
        je     0x37fd
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x53], 0x63656c65
        je     loc_1_37C4
loc_1_37C4:
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x53], 0x63656c65
        je     loc_1_37D0
loc_1_37D0:
        pop    eax
loc_1_37D1:
        imul   esi, dword ptr [edx + 0x63], 0x50206d6f
loc_1_37D8:
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_37FE
        inc    ebp
        je     0x3849
loc_1_37E1:
        jb     0x3852
        je     loc_1_3807
        dec    ecx
        dec    ecx
        dec    ecx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x3864
loc_1_37F0:
        jb     0x3821
        db  000h
loc_1_37F4:
        inc    esi
loc_1_37F5:
        jne    0x3861
        imul   esi, dword ptr [ebx + 0x74], 0x424d2075
loc_1_37FE:
        dec    eax
        xor    dword ptr [eax], esi
        xor    esi, dword ptr [eax]
        xor    ah, byte ptr [eax]
        inc    edx
        popal  
loc_1_3807:
        jae    0x386e
        and    byte ptr fs:[ecx + 0x64], al
loc_1_380D:
        popal  
        jo     0x3884
        jb     0x3841
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
        mov    ebx, dword ptr [0x3814]       ; -> obj1:0x3814
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
        mov    eax, dword ptr [0x3814]       ; -> obj1:0x3814
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
        mov    ebx, dword ptr [edx + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0552
        mov    bx, word ptr [edx]
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
        call   sub_1_060C
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
        lea    ebx, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebx + 2]
        mov    eax, dword ptr [eax + 2]
        cmp    edi, eax
        jbe    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jae    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    di, word ptr [eax]
        mov    ax, word ptr [ebx]
        cmp    di, ax
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
