; OTCETH -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 6257 instructions recovered by recursive descent

Declare_Virtual_Device OTCETH, 3, 10, OTCETH_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x942  R-X
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
OTCETH_Control:
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
sub_1_05DE:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_05E4:
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
        db  'OTCETH  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  OTCETH_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_06A0:
        dec    edi
        push   esp
        inc    ebx
        inc    ebp
        push   esp
        dec    eax
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
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
        inc    ecx
        inc    esp
        inc    ecx
        push   eax
        push   esp
        inc    ebp
        push   edx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [ecx + 0x44], al
        inc    ecx
        push   eax
        push   esp
        inc    ebp
        push   edx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_06E0:
        dec    ebp
        inc    ecx
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
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
        db  000h
loc_1_06FC:
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
loc_1_073C:
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
        add    byte ptr [ecx], dh
        xor    byte ptr [edx + 0x41], al
        push   ebx
        inc    ebp
        xor    al, byte ptr [eax]
loc_1_0760:
        xor    dword ptr [eax], esi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        xor    al, byte ptr [eax]
        inc    ecx
        push   ebp
        push   esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0770:
        inc    ecx
        push   ebp
        push   esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0778:
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
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
        add    al, byte ptr [edx]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    byte ptr [ebx], al
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x6000202
        add    al, byte ptr [edx]
        add    byte ptr [edi], al
        add    al, byte ptr [edx]
        add    byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx], cl
        add    al, byte ptr [edx]
        add    byte ptr [ebx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], cl
        add    al, byte ptr [eax]
        or     eax, 0xe000202
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_08A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, 0x4f000000
        jae    0x918
        je     loc_1_0916
        arpl   word ptr [eax + 0x20], bp
        push   esp
        jb     loc_1_092C
        insd   dword ptr es:[edi], dx
        jo     0x91d
        popal  
        jb     0x921
        and    byte ptr [eax + 0x43], dl
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [ebp + 0x74], al
        push   0x656e7265
        je     loc_1_08EE
        inc    ecx
        popal  
        jo     0x947
        jb     loc_1_08D6
loc_1_08D6:
        add    byte ptr [eax], al
loc_1_08D8:
        add    byte ptr [eax], al
loc_1_08DA:
        add    byte ptr [eax], al
loc_1_08DC:
        add    byte ptr [eax], al
loc_1_08DE:
        add    byte ptr [eax], al
loc_1_08E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08EC:
        dec    edi
        push   esp
loc_1_08EE:
        inc    ebx
        inc    ebp
        push   esp
        dec    eax
        add    byte ptr [edi + 0x73], cl
        imul   esi, dword ptr [ebp + 0x63], 0x6f432068
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        jne    0x96f
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    loc_1_092A
        dec    ecx
        outsb  dx, byte ptr [esi]
        arpl   word ptr [esi], bp
        add    byte ptr [eax], al
loc_1_0910:
        dec    edi
        push   ebx
        dec    ecx
        push   esp
        inc    ebp
        inc    ebx
loc_1_0916:
        dec    eax
        add    byte ptr [eax + esi], dh
        xor    byte ptr [eax], dh
        xor    dword ptr [ecx], edi
        add    byte ptr [eax], al
loc_1_0920:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_092A:
        add    byte ptr [eax], al
loc_1_092C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x938]        ; -> obj1:0x0938
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
        mov    eax, dword ptr [0x938]        ; -> obj1:0x0938
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
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_05E0
        mov    edi, eax
        or     edi, edi
        je     loc_2_0269
        cmp    edi, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    edi, eax
loc_2_0280:
        or     edi, edi
        je     loc_2_028C
        cmp    edi, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0468
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_050C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    edi, 0xc001000e
loc_2_02EB:
        mov    eax, edi
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
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
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
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0xc], edx
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
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        mov    dword ptr [edi + 0xc], 0
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_0651
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
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

VxD__LDATA SEGMENT   ; object 4  vsize=0xc  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh, 007h, 000h, 007h, 000h
        dd  loc_1_06A0               ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x3610  R-X
sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        push   edi
        mov    word ptr [ebp - 0x14], 0
        mov    word ptr [ebp - 0x12], 0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_041A
loc_5_0026:
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x14
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0506
loc_5_0039:
        or     eax, eax
        je     loc_5_0050
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0050:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        lea    edi, [ebp - 0x4c]
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        mov    ecx, 0xd
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [4]            ; -> obj4:0x0004
        mov    ecx, dword ptr [8]            ; -> obj4:0x0008
        mov    dword ptr [ebp - 0x1e], eax
        mov    dword ptr [ebp - 0x1a], ecx
        mov    byte ptr [ebp - 0x4c], 3
        mov    byte ptr [ebp - 0x4b], 0xa
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_5_0B20
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_5_0C90
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_5_0EC0
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_5_0F90
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_5_0D30
        lea    eax, [ebp - 0x4c]
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_5_0E80
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_5_0AF0
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_5_0170
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_5_01B0
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_5_0A20
        push   0x36
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0426
loc_5_0108:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0132
        push   0
        push   0x14
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
loc_5_011B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0420
loc_5_0126:
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0132:
        call   sub_5_3330
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_5_0162
        push   0
        push   0x14
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
loc_5_014B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0420
loc_5_0156:
        mov    eax, 0xc0000001
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0162:
        sub    eax, eax
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_5_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_042C
loc_5_0188:
        mov    edi, dword ptr [esi]
        push   0
        push   0x14
        push   esi
        call   sub_1_050C
loc_5_0194:
        push   0
        push   edi
        call   sub_1_0420
loc_5_019C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x94
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x64], 7
        mov    word ptr [ebp - 0x62], 7
        mov    dword ptr [ebp - 0x60], 0x10  ; -> loc_1_06B0
        mov    word ptr [ebp - 0x5c], 0xc
        mov    word ptr [ebp - 0x5a], 0xc
        mov    dword ptr [ebp - 0x58], 0x24  ; -> obj1:0x06C4
        mov    word ptr [ebp - 0x54], 0xd
        mov    word ptr [ebp - 0x52], 0xd
        mov    dword ptr [ebp - 0x50], 0x40  ; -> loc_1_06E0
        mov    word ptr [ebp - 0x4c], 0xa
        mov    word ptr [ebp - 0x4a], 0xa
        mov    dword ptr [ebp - 0x48], 0x5c  ; -> loc_1_06FC
        mov    word ptr [ebp - 0x44], 0xc
        mov    word ptr [ebp - 0x42], 0xc
        mov    dword ptr [ebp - 0x40], 0x84  ; -> obj1:0x0724
        mov    word ptr [ebp - 0x24], 0xb
        mov    word ptr [ebp - 0x22], 0xb
        mov    dword ptr [ebp - 0x20], 0x9c  ; -> loc_1_073C
        mov    word ptr [ebp - 0x2c], 8
        mov    word ptr [ebp - 0x2a], 8
        mov    dword ptr [ebp - 0x28], 0xb0  ; -> obj1:0x0750
        mov    word ptr [ebp - 0x3c], 8
        mov    word ptr [ebp - 0x3a], 8
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x38], 0xc0  ; -> loc_1_0760
        mov    word ptr [ebp - 0x34], 5
        mov    word ptr [ebp - 0x32], 5
        mov    dword ptr [ebp - 0x30], 0xd0  ; -> loc_1_0770
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0584
loc_5_0278:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0290
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_0290:
        mov    ebx, dword ptr [0]            ; -> obj4:0x0000
        push   ebx
        push   0
        push   0x118
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0506
loc_5_02A7:
        or     eax, eax
        je     loc_5_02C2
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_02B4:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_02C2:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0x46
        rep stosd dword ptr es:[edi], eax
        push   ebx
        push   eax
        push   0x5f0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x9f
        push   eax
        call   sub_1_0506
loc_5_02E3:
        or     eax, eax
        je     loc_5_0310
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_02F0:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0300:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_5_0310:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x6e], ebx
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x76], 9
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x78], 0x300
        mov    eax, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [eax + 0x7d], 0x5ea
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x81], 0xf8b7
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x53], 3
        lea    eax, [ebp - 0x64]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_0360:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_09C0
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    dword ptr [ecx + 0x78], eax
        push   0
        lea    eax, [ebp - 0x5c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_0393:
        push   0
        lea    eax, [ebp - 0x54]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_03AA:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xa3], 2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_03C9
        mov    eax, dword ptr [ebp - 0x14]
        mov    cl, byte ptr [eax + 4]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xa3], cl
loc_5_03C9:
        lea    edi, [ebp - 0x94]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x8a], 8
        mov    dword ptr [ebp - 0x7e], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x78]
        add    eax, 0x78
        mov    dword ptr [ebp - 0x7a], ecx
        mov    dword ptr [ebp - 0x76], 0x20
        mov    dword ptr [ebp - 0x72], eax
        lea    eax, [ebp - 0x94]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebx + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_0432
loc_5_0421:
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_5_0463
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_0431:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0447:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0457:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0463:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_047A:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_0960
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx + 0x76], ax
        lea    eax, [ebp - 0x44]
        push   2
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_04A8:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_04DB
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_04C0:
        or     al, al
        je     loc_5_0550
        mov    eax, dword ptr [ebp - 8]
        or     word ptr [eax + 0x81], 0x100
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x7c], 1
loc_5_04DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_2C70
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_5_0610
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_0502:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_050E:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_051A:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0530:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0540:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_5_0550:
        push   1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_0562:
        or     al, al
        je     loc_5_0580
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 0x81], 0xfeff
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x7c], 2
        jmp    loc_5_04DB
        db  08Bh, 0FFh
loc_5_0580:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 4
        push   eax
        call   sub_1_0596
loc_5_0592:
        or     al, al
        je     loc_5_05B0
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 0x81], 0xfeff
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x7c], 0
        jmp    loc_5_04DB
        db  08Bh, 0FFh
loc_5_05B0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_05C3:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_05CF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_05DB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_05F1:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0601:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_5_0610:
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [eax + 0x78], 0x10
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_19B0
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        push   0
        je     loc_5_0684
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_063A:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_0646:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_0652:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0668:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0678:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0684:
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_058A
loc_5_0699:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_06F5
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 4], 1
        jne    loc_5_06F5
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_048C
loc_5_06BD:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_06F5
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x48], 1
        cmp    dword ptr [ebp - 0x1c], 6
        jne    loc_5_0820
        mov    eax, dword ptr [ebp - 0x18]
        test   byte ptr [eax], 1
        jne    loc_5_0820
        mov    edx, dword ptr [ebp - 8]
        add    edx, 0x91
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
loc_5_06F5:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_06FE:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0xc
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0x14
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0x1c
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0x24
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [ebp - 8]
        push   0x1388
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x38], 0     ; -> loc_5_2BD0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 0x3c], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x34
        push   eax
        call   sub_1_04E2
loc_5_0785:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x72
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x30
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x91
        push   eax
        push   0                             ; -> loc_5_1200
        push   0                             ; -> loc_5_1170
        push   0                             ; -> loc_5_10D0
        push   0x10
        call   sub_2_0000
loc_5_07B2:
        or     al, al
        jne    loc_5_0880
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000002
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_07CD:
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x34
        push   eax
        call   sub_1_04E8
loc_5_07E0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_07EC:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0802:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0812:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0820:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000009
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_0833:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_083F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_084B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0861:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0871:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_5_0880:
        mov    eax, dword ptr [ebp - 8]
        push   1
        mov    ax, word ptr [eax + 0x76]
        and    eax, 0xffff
        push   1
        push   eax
        push   eax
        push   0                             ; -> loc_5_12A0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0                             ; -> loc_5_1230
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x54
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_04EE
loc_5_08B4:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0930
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000005
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_08CD:
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x34
        push   eax
        call   sub_1_04E8
loc_5_08E0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_08EC:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x72]
        push   eax
        call   sub_2_0120
loc_5_08F8:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_090E:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_091E:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_0930:
        push   0                             ; -> loc_5_0AC0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05DE
loc_5_0942:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_1B40
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        mov    word ptr [eax + 0x6c], 1
        mov    eax, dword ptr [ebp - 0xc]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_5_0960:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa00000c
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_0973:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_097F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_098B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_09A1:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_09B1:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_5_09C0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa00000c
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_05D8
loc_5_09D3:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0590
loc_5_09DF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0438
loc_5_09EB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x5f0
        mov    eax, dword ptr [eax + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0A01:
        push   0
        push   0x118
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_050C
loc_5_0A11:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_5_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 2
        mov    cx, word ptr [esi + 0x78]
        mov    word ptr [esi + 0x6c], 9
        lea    edx, [ecx + 0xe]
        out    dx, ax
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05E4
loc_5_0A4C:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_04E8
loc_5_0A59:
        push   0x2625a0
        call   sub_1_0500
loc_5_0A63:
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_04F4
loc_5_0A6C:
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_2_0120
loc_5_0A75:
        push   0
        push   0x5f0
        mov    eax, dword ptr [esi + 0x9f]
        push   eax
        call   sub_1_050C
loc_5_0A88:
        push   esi
        call   sub_5_2DC0
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0438
loc_5_0A97:
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_049E
loc_5_0AA0:
        push   0
        push   0x118
        push   esi
        call   sub_1_050C
loc_5_0AAD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0AC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x6c], 9
        mov    cx, word ptr [eax + 0x78]
        mov    ax, 2
        lea    edx, [ecx + 0xe]
        out    dx, ax
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        push   0x3d090
        call   sub_1_0500
loc_5_0AE9:
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_0AF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 0x6c], 2
        jne    loc_5_0B06
        mov    eax, 0xc001000d
        pop    ebp
        ret    8
loc_5_0B06:
        push   0
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_5_2130
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    word ptr [esi + 0x6c], 9
        jne    loc_5_0B41
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_0B41:
        sub    ebx, ebx
        mov    edx, dword ptr [ebp + 0x18]
        cmp    edx, ebx
        je     loc_5_0B63
        mov    ecx, dword ptr [ebp + 0x14]
loc_5_0B50:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_5_0B63
        cmp    eax, 5
        je     loc_5_0B63
        add    ecx, 4
        inc    ebx
        cmp    edx, ebx
        ja     loc_5_0B50
loc_5_0B63:
        cmp    edx, ebx
        jne    loc_5_0B75
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_0B75:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x20
        push   eax
        call   sub_1_0506
loc_5_0B8D:
        or     eax, eax
        je     loc_5_0BB2
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
loc_5_0BA1:
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_0BB2:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 0x18], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1c
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_2_0170
loc_5_0BE2:
        or     al, al
        jne    loc_5_0C14
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D8
loc_5_0BF6:
        add    esp, 0xc
        push   0
        push   0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_050C
loc_5_0C06:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_5_0C14:
        inc    dword ptr [esi + 0x49]
        cmp    word ptr [esi + 0x6c], 2
        mov    eax, dword ptr [ebp - 4]
        jne    loc_5_0C30
        mov    word ptr [eax + 0xc], 8
        mov    eax, 0x103
        jmp    loc_5_0C38
        db  08Bh, 0FFh
loc_5_0C30:
        mov    word ptr [eax + 0xc], 1
        sub    eax, eax
loc_5_0C38:
        mov    edx, dword ptr [ebp - 4]
        lea    ecx, [esi + 0xc]
        mov    dword ptr [edx], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [edx + 4], ecx
        mov    edx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + ebx*4]
        mov    dword ptr [edx + 0x14], ecx
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0x14], 0
        jne    loc_5_0C71
        inc    byte ptr [esi + 0x52]
        jmp    loc_5_0C74
loc_5_0C71:
        inc    byte ptr [esi + 0x51]
loc_5_0C74:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    word ptr [esi + 0xc], 1
        je     loc_5_0CB0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_0CB0:
        cmp    word ptr [edi + 0x6c], 2
        jne    loc_5_0CC8
        mov    word ptr [esi + 0xc], 5
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0CC8:
        dec    dword ptr [edi + 0x49]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [edi + 0x49], 0
        jne    loc_5_0CE6
        push   edi
        call   sub_5_1B40
loc_5_0CE6:
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_5_0CF1
        dec    byte ptr [edi + 0x52]
        jmp    loc_5_0CF4
loc_5_0CF1:
        dec    byte ptr [edi + 0x51]
loc_5_0CF4:
        push   0
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x72]
        push   eax
        call   sub_2_0240
loc_5_0D03:
        or     eax, eax
        je     loc_5_0D12
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0D12:
        push   0
        push   0x20
        push   esi
        call   sub_1_050C
loc_5_0D1C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        mov    ax, word ptr [esi + 0x6c]
        cmp    ax, 9
        jne    loc_5_0D57
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0D57:
        cmp    ax, 2
        je     loc_5_0E70
        cmp    ax, 3
        je     loc_5_0E70
        mov    word ptr [esi + 0x6c], 3
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0xc
        je     loc_5_0D9D
loc_5_0D80:
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        call   sub_1_046E
loc_5_0D92:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0xc
        jne    loc_5_0D80
loc_5_0D9D:
        mov    word ptr [esi + 0x6c], 2
        push   esi
        push   0                             ; -> sub_5_1B40
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_04FA
loc_5_0DB2:
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0xc
        je     loc_5_0E5D
loc_5_0DC6:
        mov    edi, dword ptr [ebp - 4]
        mov    ebx, edi
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [ebx + 0x18]
        add    ebx, 0x18
        push   eax
        call   sub_1_046E
loc_5_0DE0:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0474
loc_5_0DE8:
        mov    eax, edi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [edi + 0xc]
        cmp    ax, 5
        jne    loc_5_0E37
        dec    dword ptr [esi + 0x49]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_2_0240
loc_5_0E1A:
        cmp    eax, 0xc001000e
        je     loc_5_0E4F
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_0444
loc_5_0E2B:
        push   0
        push   0x20
        push   edi
        call   sub_1_050C
loc_5_0E35:
        jmp    loc_5_0E4F
loc_5_0E37:
        cmp    ax, 8
        jne    loc_5_0E4F
        mov    word ptr [edi + 0xc], 1
        push   0
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_043E
loc_5_0E4F:
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0xc
        jne    loc_5_0DC6
loc_5_0E5D:
        mov    word ptr [esi + 0x6c], 1
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0E70:
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0E80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 0x6c], 2
        jne    loc_5_0E99
        mov    eax, 0xc001000d
        pop    ebp
        ret    8
loc_5_0E99:
        cmp    word ptr [eax + 0xc], 1
        je     loc_5_0EA9
        mov    eax, 0xc0000001
        pop    ebp
        ret    8
loc_5_0EA9:
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_5_2130
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_5_0EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 8]
        mov    ax, word ptr [esi + 0x6c]
        cmp    ax, 9
        jne    loc_5_0EE3
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0EE3:
        cmp    ax, 2
        jne    loc_5_0EF5
        mov    eax, 0xc001000d
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0EF5:
        cmp    word ptr [edx + 0xc], 1
        je     loc_5_0F08
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0F08:
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ecx + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_0530
loc_5_0F2A:
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_5_0F7A
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_5_0F7A
        inc    dword ptr [esi + 0xa8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     loc_5_0F6E
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1E70
loc_5_0F6E:
        mov    eax, 0x103
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0F7A:
        mov    eax, 0xc001000f
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax + 8]
        mov    esi, dword ptr [edx + 0x9f]
        lea    esi, [ecx + esi + 0xe]
        cmp    word ptr [edx + 0x6c], 2
        jne    loc_5_0FC1
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0FC1:
        cmp    word ptr [eax + 0xc], 1
        je     loc_5_0FD6
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0FD6:
        cmp    byte ptr [edx + 0x44], 0
        jne    loc_5_0FEA
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_0FEA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [eax + 4]
        sub    edi, dword ptr [ebp + 0x10]
        sub    edi, 0xe
        jns    loc_5_1006
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_5_1006:
        mov    eax, dword ptr [ebp + 0x1c]
        sub    ebx, ebx
        mov    dword ptr [eax], ebx
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0530
loc_5_1020:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, edi
        jge    loc_5_1029
        mov    edi, eax
loc_5_1029:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    eax, edi
        jge    loc_5_1032
        mov    edi, eax
loc_5_1032:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_5_1060
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0xe
        push   eax
        push   ecx
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_057E
loc_5_1053:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_5_1060:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_10B3
loc_5_1066:
        or     edi, edi
        je     loc_5_10B3
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0554
loc_5_107B:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_10A0
        cmp    dword ptr [ebp - 4], edi
        jbe    loc_5_1089
        mov    dword ptr [ebp - 4], edi
loc_5_1089:
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_05EC
loc_5_109D:
        add    esi, dword ptr [ebp - 4]
loc_5_10A0:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0578
loc_5_10AD:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_1066
loc_5_10B3:
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x10]
        or     edi, edi
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [esi + 0x78]
        jne    loc_5_1139
        lea    edi, [ebx + 0xe]
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 3
        out    dx, ax
        sub    cx, cx
        sub    eax, eax
loc_5_1100:
        mov    edx, ecx
        and    edx, 0xffff
        lea    edx, [esi + edx*2]
        inc    cx
        mov    byte ptr [edx + 0x98], al
        mov    byte ptr [edx + 0x97], al
        lea    edx, [ebx + ecx*2 - 2]
        out    dx, ax
        cmp    cx, 4
        jb     loc_5_1100
        mov    dx, di
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_5_1139:
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [edi + edi*2]
        lea    ebx, [eax + ecx*2 - 6]
loc_5_1143:
        push   ebx
        call   sub_5_2040
        push   1
        push   eax
        push   esi
        call   sub_5_20B0
        sub    ebx, 6
        dec    edi
        jne    loc_5_1143
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        sub    si, si
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebp + 0xc]
        test   al, 1
        je     loc_5_118C
        mov    si, 0x100
loc_5_118C:
        test   al, 2
        je     loc_5_1194
        mov    si, 0x100
loc_5_1194:
        test   al, 8
        je     loc_5_119C
        mov    si, 0x100
loc_5_119C:
        test   al, 4
        je     loc_5_11A4
        mov    si, 0x104
loc_5_11A4:
        test   al, 0x20
        je     loc_5_11B2
        mov    si, 0x102
        mov    byte ptr [ecx + 0x47], 1
        jmp    loc_5_11B6
loc_5_11B2:
        mov    byte ptr [ecx + 0x47], 0
loc_5_11B6:
        mov    edx, dword ptr [ecx + 0x78]
        add    dx, 0xe
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    di, word ptr [ecx + 0x78]
        or     si, 0x200
        lea    ecx, [edi + 0xe]
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, si
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        call   sub_1_0444
loc_5_1212:
        push   0
        push   0x20
        push   esi
        call   sub_1_050C
loc_5_121C:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    ecx, dword ptr [edi + 0x78]
        lea    esi, [ecx + 0xe]
        lea    edx, [ecx - 1]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0xff
        jne    loc_5_1260
        mov    word ptr [edi + 0x6c], 9
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1260:
        test   byte ptr [ebp - 1], 2
        je     loc_5_1292
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, 2
        out    dx, ax
        lea    edx, [ecx + 0xc]
        in     al, dx
        test   al, 0x3b
        je     loc_5_1289
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        mov    byte ptr [edi + 0x46], 1
        mov    bl, 1
loc_5_1289:
        mov    dx, si
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
loc_5_1292:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_12A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x46], 1
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [ebp - 0x10], eax
        add    eax, 0xc
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [esi + 0x42], 1
        lea    edi, [eax + 0xe]
        dec    ax
        mov    word ptr [ebp - 0x18], ax
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ebp - 0x18]
        mov    al, byte ptr [esi + 0x53]
        out    dx, al
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ah, 0x33
        je     loc_5_12F7
        mov    word ptr [esi + 0x6c], 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_5_12F7:
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    cx, word ptr [ebp - 0x10]
        add    cx, 6
        mov    word ptr [ebp - 0x20], cx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 0xa], ax
        mov    cx, word ptr [ebp - 0x10]
        add    cx, 2
        mov    word ptr [ebp - 0x1c], cx
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
loc_5_1328:
        mov    dx, di
        in     ax, dx
        cmp    ah, 0x33
        jne    loc_5_13A1
        mov    ax, 2
        out    dx, ax
        mov    dx, word ptr [ebp - 0x14]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi + 0x40], 0
        je     loc_5_1353
        cmp    byte ptr [esi + 0x43], 0
        jne    loc_5_1353
        mov    bl, al
        and    bl, 0x3b
        jmp    loc_5_1359
loc_5_1353:
        mov    bl, byte ptr [ebp - 1]
        and    bl, 0x33
loc_5_1359:
        or     bl, bl
        je     loc_5_13A7
        test   bl, 1
        je     loc_5_136A
        push   esi
        call   sub_5_1430
        jmp    loc_5_1328
loc_5_136A:
        test   bl, 2
        je     loc_5_1377
        push   esi
        call   sub_5_15D0
        jmp    loc_5_1328
loc_5_1377:
        test   bl, 8
        je     loc_5_1384
        push   esi
        call   sub_5_1870
        jmp    loc_5_1328
loc_5_1384:
        test   bl, 0x30
        je     loc_5_1328
        test   bl, 0x20
        je     loc_5_1394
        push   esi
        call   sub_5_1950
loc_5_1394:
        test   bl, 0x10
        je     loc_5_1328
        push   esi
        call   sub_5_1900
        jmp    loc_5_1328
loc_5_13A1:
        mov    word ptr [esi + 0x6c], 9
loc_5_13A7:
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    dx, word ptr [ebp - 0x20]
        mov    ax, word ptr [ebp - 0xa]
        out    dx, ax
        mov    dx, word ptr [ebp - 0x1c]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 0x33
        cmp    byte ptr [esi + 0x40], 0
        je     loc_5_13D6
        cmp    byte ptr [esi + 0x43], 0
        jne    loc_5_13D6
        mov    byte ptr [ebp - 1], 0x3b
loc_5_13D6:
        mov    dx, word ptr [ebp - 0x10]
        add    dx, 0xd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        cmp    byte ptr [esi + 0x45], 0
        mov    dx, di
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    byte ptr [esi + 0x42], 0
        mov    dx, word ptr [ebp - 0x18]
        in     al, dx
        je     loc_5_1407
        mov    byte ptr [esi + 0x45], 0
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_1_0290
loc_5_1407:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_5_1425
        lea    eax, [ebp - 0x24]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     loc_5_1425
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        push   esi
        call   sub_5_1E70
loc_5_1425:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_5_1430:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [ebp - 0xc], eax
        add    eax, 8
        mov    dword ptr [ebp - 0x14], eax
        mov    ax, word ptr [ebp - 0xc]
        add    ax, 5
        mov    word ptr [ebp - 0x18], ax
        mov    dx, ax
        in     al, dx
        test   al, 0x80
        jne    loc_5_15C4
        mov    ax, word ptr [ebp - 0xc]
        add    ax, 6
        mov    word ptr [ebp - 0x1c], ax
        lea    esp, [esp]
loc_5_1470:
        mov    ebx, dword ptr [esi + 0x9f]
        mov    dx, word ptr [ebp - 0x1c]
        mov    ax, 0xe000
        out    dx, ax
        push   2
        push   ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_03F1
loc_5_148C:
        mov    di, word ptr [ebx]
        mov    cx, word ptr [ebx + 2]
        test   di, 0xac00
        jne    loc_5_1586
        sub    cx, 6
        test   di, 0x1000
        je     loc_5_14AB
        inc    cx
loc_5_14AB:
        lea    eax, [ecx - 0xe]
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [esi + 0x9f]
        mov    ebx, ecx
        and    ebx, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebx + 3]
        sar    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0406
loc_5_14D7:
        mov    dx, word ptr [ebp - 0xc]
        mov    ax, 0x80
        out    dx, ax
        mov    dword ptr [ebp - 0x20], ebx
        mov    dword ptr [ebp - 0x24], 0
        mov    byte ptr [esi + 0x44], 1
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0xe
        push   eax
        push   0xe
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_1_00B0
loc_5_1512:
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x45], 1
        inc    dword ptr [esi + 0xb0]
        test   di, 0x4000
        je     loc_5_1535
        add    dword ptr [esi + 0xe8], ebx
        inc    dword ptr [esi + 0xec]
        jmp    loc_5_155C
loc_5_1535:
        test   di, 1
        je     loc_5_1550
        add    dword ptr [esi + 0xe0], ebx
        inc    dword ptr [esi + 0xe4]
        jmp    loc_5_155C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_1550:
        add    dword ptr [esi + 0xd8], ebx
        inc    dword ptr [esi + 0xdc]
loc_5_155C:
        mov    ecx, 1
loc_5_1561:
        mov    dx, word ptr [ebp - 0xc]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   cx, ax
        jne    loc_5_1561
        mov    dx, word ptr [ebp - 0x18]
        in     al, dx
        test   al, 0x80
        je     loc_5_1470
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1586:
        inc    dword ptr [esi + 0xb8]
        test   di, 0x8000
        je     loc_5_1599
        inc    dword ptr [esi + 0xf4]
loc_5_1599:
        test   di, 0x2000
        je     loc_5_15A6
        inc    dword ptr [esi + 0xf0]
loc_5_15A6:
        mov    dx, word ptr [ebp - 0xc]
        mov    ax, 0x80
        out    dx, ax
        mov    ecx, 1
loc_5_15B5:
        mov    dx, word ptr [ebp - 0xc]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   cx, ax
        jne    loc_5_15B5
loc_5_15C4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1c], 0
        mov    ecx, dword ptr [esi + 0x78]
        dec    dword ptr [esi + 0xa4]
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 6]
        mov    ax, 0x6000
        out    dx, ax
        lea    edx, [ecx + 8]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    dx, cx
        sub    word ptr [ebp - 6], 6
        mov    ax, 0xa0
        test   word ptr [ebp - 4], 0x8e10
        out    dx, ax
        lea    edx, [ecx + 0xc]
        mov    al, 2
        out    dx, al
        je     loc_5_1673
        mov    dword ptr [ebp - 0x1c], 0xc0000001
        inc    dword ptr [esi + 0xb4]
        test   byte ptr [ebp - 3], 0x80
        je     loc_5_1641
        inc    dword ptr [esi + 0x10c]
loc_5_1641:
        test   byte ptr [ebp - 3], 4
        je     loc_5_164D
        inc    dword ptr [esi + 0x114]
loc_5_164D:
        test   byte ptr [ebp - 3], 2
        je     loc_5_1659
        inc    dword ptr [esi + 0x104]
loc_5_1659:
        test   byte ptr [ebp - 4], 0x10
        je     loc_5_1665
        inc    dword ptr [esi + 0x100]
loc_5_1665:
        test   byte ptr [ebp - 4], 0x20
        je     loc_5_16E6
        inc    dword ptr [esi + 0x110]
        jmp    loc_5_16E6
loc_5_1673:
        inc    dword ptr [esi + 0xac]
        test   byte ptr [ebp - 4], 0x40
        je     loc_5_1693
        xor    eax, eax
        inc    dword ptr [esi + 0xd4]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0xd0], eax
        jmp    loc_5_16C2
loc_5_1693:
        test   byte ptr [ebp - 4], 8
        je     loc_5_16B0
        xor    eax, eax
        inc    dword ptr [esi + 0xcc]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0xc8], eax
        jmp    loc_5_16C2
        db  08Dh, 049h, 000h
loc_5_16B0:
        xor    eax, eax
        inc    dword ptr [esi + 0xc4]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0xc0], eax
loc_5_16C2:
        test   byte ptr [ebp - 4], 0x80
        je     loc_5_16CE
        inc    dword ptr [esi + 0x108]
loc_5_16CE:
        test   byte ptr [ebp - 4], 4
        je     loc_5_16DA
        inc    dword ptr [esi + 0xfc]
loc_5_16DA:
        test   byte ptr [ebp - 4], 2
        je     loc_5_16E6
        inc    dword ptr [esi + 0xf8]
loc_5_16E6:
        lea    ecx, [esi + 0x24]
        mov    dword ptr [ebp - 0x14], 0
        cmp    dword ptr [ecx], 0
        je     loc_5_170F
        dec    dword ptr [esi + 0xa8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax], 0
        jne    loc_5_170F
        mov    dword ptr [esi + 0x28], ecx
loc_5_170F:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_5_1820
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x24], eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_0530
loc_5_1737:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0554
loc_5_1748:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_1_0330
loc_5_1755:
        or     al, al
        je     loc_5_1809
        cmp    byte ptr [esi + 0x47], 0
        jne    loc_5_1809
        mov    ebx, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x20]
        cmp    ebx, eax
        jae    loc_5_17DD
        mov    eax, ebx
        sub    eax, 0xe
        cmp    eax, dword ptr [esi + 0x7d]
        jae    loc_5_17DD
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x9f]
loc_5_1786:
        mov    eax, dword ptr [esi + 0x7d]
        mov    ecx, dword ptr [ebp - 0xc]
        add    ecx, ebx
        cmp    ecx, eax
        jbe    loc_5_1797
        sub    eax, ebx
        mov    dword ptr [ebp - 0xc], eax
loc_5_1797:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        lea    eax, [edi + ebx]
        push   eax
        call   sub_1_05EC
loc_5_17A8:
        add    ebx, dword ptr [ebp - 0xc]
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0578
loc_5_17B8:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_17DD
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0554
loc_5_17CF:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_17DD
        lea    eax, [ebx - 0xe]
        cmp    eax, dword ptr [esi + 0x7d]
        jb     loc_5_1786
loc_5_17DD:
        mov    byte ptr [esi + 0x44], 1
        mov    eax, dword ptr [ebp - 0x20]
        sub    eax, 0xe
        push   eax
        sub    ebx, 0xe
        push   ebx
        lea    eax, [edi + 0xe]
        push   eax
        push   0xe
        push   edi
        push   edi
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 0x72]
        push   eax
        call   sub_1_00B0
loc_5_1801:
        mov    byte ptr [esi + 0x44], 0
        mov    byte ptr [esi + 0x45], 1
loc_5_1809:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_044A
loc_5_1820:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_5_1844
loc_5_1826:
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     loc_5_1844
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   esi
        call   sub_5_1E70
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_5_1826
loc_5_1844:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 0x78]
        add    dx, 0xc
        mov    al, 4
        out    dx, al
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        lea    esi, [edi + 0x14]
        mov    ecx, dword ptr [edi + 0x78]
        mov    byte ptr [edi + 0x40], 0
        lea    edx, [ecx + 3]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_5_18AB
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    loc_5_18AB
        mov    dword ptr [edi + 0x18], esi
loc_5_18AB:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_18C6
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax + 0xc], cl
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1E70
        jmp    loc_5_18D6
loc_5_18C6:
        lea    edx, [ecx + 2]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, cx
        mov    ax, 0xa0
        out    dx, ax
loc_5_18D6:
        cmp    dword ptr [esi], 0
        je     loc_5_18F8
loc_5_18DB:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1D40
        or     al, al
        je     loc_5_18F8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1E70
        cmp    dword ptr [esi], 0
        jne    loc_5_18DB
loc_5_18F8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        mov    edi, dword ptr [ecx + 0x78]
        lea    esi, [edi + 0xe]
        mov    dx, si
        out    dx, ax
        lea    edx, [edi + 4]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 0xfffe
        mov    ax, word ptr [ebp - 2]
        inc    dword ptr [ecx + 0xbc]
        out    dx, ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0xc]
        mov    al, 0x10
        out    dx, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_5_1950:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    ax, ax
        mov    esi, dword ptr [edi + 0x78]
        lea    ecx, [esi + 0xe]
        mov    dx, cx
        out    dx, ax
        lea    edx, [esi + 2]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        or     word ptr [ebp - 4], 1
        mov    ax, word ptr [ebp - 4]
        test   byte ptr [ebp - 2], 0x10
        out    dx, ax
        mov    dx, cx
        mov    ax, 2
        out    dx, ax
        je     loc_5_199B
        inc    dword ptr [edi + 0x100]
loc_5_199B:
        test   byte ptr [ebp - 2], 0x20
        je     loc_5_19A7
        inc    dword ptr [edi + 0x110]
loc_5_19A7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_5_19B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, 1
        mov    edi, dword ptr [ecx + 0x78]
        lea    esi, [edi + 0xe]
        mov    dx, si
        out    dx, ax
        lea    edx, [edi + 0xa]
        mov    ax, 0xffff
        out    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        inc    word ptr [ebp - 2]
        je     loc_5_19F0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_19F0:
        sub    ax, ax
        out    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        or     ax, ax
        je     loc_5_1A10
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_1A10:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 0x7c]
        mov    ax, 0xf8b7
        or     edx, edx
        je     loc_5_1A24
        cmp    edx, 1
        je     loc_5_1A30
        jmp    loc_5_1A34
loc_5_1A24:
        mov    ax, 0xf8b7
        jmp    loc_5_1A34
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_1A30:
        mov    ax, 0xf9b7
loc_5_1A34:
        or     ax, 0x1000
        mov    dx, di
        out    dx, ax
        mov    word ptr [ecx + 0x81], ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        mov    dx, di
        mov    ax, 0x40
        out    dx, ax
        mov    dx, si
        sub    ax, ax
        out    dx, ax
        lea    ebx, [edi + 8]
        mov    dx, bx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_5_1A77
        inc    eax
loc_5_1A77:
        shl    eax, 8
        mov    dword ptr [ecx + 0x83], eax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        mov    dx, di
        mov    ax, 0x20
        out    dx, ax
        mov    dx, si
        sub    ax, ax
        out    dx, ax
        mov    dx, bx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ax, word ptr [ebp - 4]
        and    eax, 0xff00
        mov    dx, word ptr [ebp - 6]
        and    edx, 0xff00
        sub    eax, edx
        mov    dword ptr [ecx + 0x87], eax
        mov    al, byte ptr [ecx + 0xa3]
        cmp    al, 9
        jbe    loc_5_1AD1
        mov    byte ptr [ecx + 0xa3], 8
        jmp    loc_5_1AE9
loc_5_1AD1:
        cmp    al, 1
        jbe    loc_5_1AE0
        dec    al
        mov    byte ptr [ecx + 0xa3], al
        jmp    loc_5_1AE9
        db  090h
loc_5_1AE0:
        jae    loc_5_1AE9
        mov    byte ptr [ecx + 0xa3], 1
loc_5_1AE9:
        mov    eax, 0                        ; -> obj1:0x093C
        lea    edx, [ecx + 0x8b]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [edx], ebx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        cmp    byte ptr [ecx + 0x48], 0
        jne    loc_5_1B18
        add    ecx, 0x91
        mov    eax, ebx
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ecx + 4], ax
loc_5_1B18:
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0xd]
        sub    al, al
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 2
        mov    edi, dword ptr [esi + 0x78]
        lea    edx, [edi + 0xe]
        out    dx, ax
        lea    edx, [edi + 0xd]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 0xe]
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, 0x8000
        out    dx, ax
        mov    ebx, 0x10
loc_5_1B70:
        push   0x2710
        call   sub_1_0500
loc_5_1B7A:
        dec    ebx
        jne    loc_5_1B70
        lea    edx, [edi + 4]
        sub    ax, ax
        out    dx, ax
        push   0x2710
        call   sub_1_0500
loc_5_1B8F:
        lea    ecx, [esi + 0x91]
        lea    edx, [edi + 0xe]
        mov    ax, 1
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    ecx, 2
        lea    edx, [edi + 6]
        mov    ax, word ptr [ecx]
        out    dx, ax
        lea    edx, [edi + 8]
        mov    ax, word ptr [ecx + 2]
        out    dx, ax
        mov    dx, di
        mov    ax, word ptr [esi + 0x81]
        out    dx, ax
        lea    edx, [edi + 0xe]
        mov    ax, 3
        out    dx, ax
        sub    ecx, ecx
loc_5_1BD1:
        lea    edx, [edi + ecx]
        mov    al, byte ptr [esi + ecx + 0x97]
        out    dx, al
        inc    ecx
        cmp    ecx, 6
        jl     loc_5_1BD1
        lea    edx, [edi + 0xe]
        mov    ax, 1
        out    dx, ax
        lea    edx, [edi + 0xc]
        mov    ax, 0x20
        out    dx, ax
        lea    edx, [edi + 0xe]
        sub    ax, ax
        out    dx, ax
        mov    dx, di
        mov    ax, 0x1481
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, 0x300
        out    dx, ax
        lea    edx, [edi + 0xa]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 0xe]
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0xd]
        mov    al, 0x33
        out    dx, al
        push   esi
        call   sub_5_1C40
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebp - 8]
        mov    byte ptr [ecx + 0x41], 0
        mov    byte ptr [ecx + 0x43], 0
        mov    byte ptr [ecx + 0x42], 0
        mov    byte ptr [ecx + 0x40], 0
        lea    edx, [ecx + 0x24]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx + 0xa8], 0
        mov    dword ptr [ecx + 0xa4], 0
        cmp    dword ptr [edx], 0
        je     loc_5_1CA4
loc_5_1C83:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    loc_5_1C91
        mov    dword ptr [ecx + 0x28], edx
loc_5_1C91:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    loc_5_1C83
loc_5_1CA4:
        lea    edx, [ecx + 0x1c]
        cmp    dword ptr [edx], 0
        je     loc_5_1CCD
loc_5_1CAC:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    loc_5_1CBA
        mov    dword ptr [ecx + 0x20], edx
loc_5_1CBA:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    loc_5_1CAC
loc_5_1CCD:
        lea    edx, [ecx + 0x14]
        cmp    dword ptr [edx], 0
        je     loc_5_1CF6
loc_5_1CD5:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    loc_5_1CE3
        mov    dword ptr [ecx + 0x18], edx
loc_5_1CE3:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    loc_5_1CD5
loc_5_1CF6:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_1D2E
        sub    esi, esi
        mov    edi, edi
loc_5_1D00:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ecx], esi
        jne    loc_5_1D14
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_5_1D14:
        push   0xc001000c
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_044A
loc_5_1D29:
        cmp    dword ptr [ebp - 8], esi
        jne    loc_5_1D00
loc_5_1D2E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x40], 0
        jne    loc_5_1E5A
        cmp    dword ptr [esi + 0x14], 0
        je     loc_5_1E5A
        xor    eax, eax
        mov    al, byte ptr [esi + 0xa3]
        cmp    eax, dword ptr [esi + 0xa4]
        jbe    loc_5_1E5A
        mov    edi, dword ptr [esi + 0x78]
        lea    eax, [edi + 0xe]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        lea    edx, [edi + 0xe]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 8]
        in     ax, dx
        cmp    ah, 8
        lea    edx, [edi + 0xe]
        jae    loc_5_1DB0
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1DB0:
        mov    ax, 2
        inc    dword ptr [esi + 0xa4]
        out    dx, ax
        mov    byte ptr [esi + 0x43], 1
        mov    byte ptr [esi + 0x40], 1
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [ebp - 0xc], ecx
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_0530
loc_5_1DDD:
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 6
        shr    ecx, 8
        or     cx, 0x20
        mov    dx, di
        mov    ax, cx
        out    dx, ax
        mov    ebx, 0xa
        lea    edx, [edi + 0xc]
        mov    ecx, 8
        nop    
loc_5_1E00:
        in     ax, dx
        test   cx, ax
        jne    loc_5_1E0A
        dec    ebx
        jne    loc_5_1E00
loc_5_1E0A:
        or     ebx, ebx
        je     loc_5_1E3A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        lea    edx, [edi + 3]
        in     al, dx
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x14], ecx
        cmp    dword ptr [eax], 0
        jne    loc_5_1E30
        lea    eax, [esi + 0x14]
        mov    dword ptr [esi + 0x18], eax
loc_5_1E30:
        mov    byte ptr [esi + 0x43], 0
        mov    byte ptr [esi + 0x40], 0
        jmp    loc_5_1E46
loc_5_1E3A:
        cmp    byte ptr [esi + 0x42], 0
        jne    loc_5_1E46
        push   esi
        call   sub_5_2B70
loc_5_1E46:
        lea    edx, [edi + 0xe]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1E5A:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [ecx + 0x41], 0
        jne    loc_5_2020
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x41], 1
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [ecx + 0x28], esi
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ecx + 0x78]
        mov    dword ptr [ebp - 0x24], eax
        lea    edi, [eax + 8]
        add    ax, 0xe
        mov    word ptr [ebp - 0x30], ax
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 0x30]
        mov    ax, 2
        out    dx, ax
        mov    cx, word ptr [ebp - 0x24]
        add    cx, 6
        mov    word ptr [ebp - 0x28], cx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ebp - 0x24]
        add    cx, 2
        mov    word ptr [ebp - 0x2c], cx
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ebp + 8]
        add    edx, 0x1c
        mov    dword ptr [ebp - 0x1c], edx
        sub    ebx, ebx
loc_5_1EF6:
        mov    dx, word ptr [ebp - 0x2c]
        mov    al, byte ptr [esi + 0xc]
        out    dx, al
        mov    dx, word ptr [ebp - 0x28]
        mov    ax, 0x4000
        out    dx, ax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0530
loc_5_1F1B:
        mov    dx, di
        mov    ax, bx
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x20]
        add    ax, 6
        cmp    dword ptr [ebp - 0x14], ebx
        out    dx, ax
        je     loc_5_1F9C
loc_5_1F31:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0554
loc_5_1F42:
        cmp    dword ptr [ebp - 0xc], 7
        jbe    loc_5_1F6D
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0x7ffc
        shr    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   edi
        call   sub_1_03C9
loc_5_1F5E:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0x7ffc
        add    dword ptr [ebp - 0x10], eax
        and    dword ptr [ebp - 0xc], 3
loc_5_1F6D:
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_5_1F85
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0x7fff
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   edi
        call   sub_1_03A0
loc_5_1F85:
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0578
loc_5_1F92:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], eax
        cmp    eax, ebx
        jne    loc_5_1F31
loc_5_1F9C:
        test   byte ptr [ebp - 0x20], 1
        mov    dx, di
        je     loc_5_1FAA
        mov    al, 0x30
        out    dx, al
        jmp    loc_5_1FB0
loc_5_1FAA:
        mov    ax, 0x10
        out    dx, ax
loc_5_1FB0:
        mov    dx, word ptr [ebp - 0x24]
        mov    ax, 0xc0
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [edx]
        cmp    eax, ebx
        jne    loc_5_1FD0
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ecx + 0x41], bl
        jmp    loc_5_1FF0
        db  08Bh, 0FFh
loc_5_1FD0:
        mov    esi, eax
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], ebx
        jne    loc_5_1FE3
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x20], edx
loc_5_1FE3:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi], ebx
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [ecx + 0x28], esi
        mov    dword ptr [eax], esi
loc_5_1FF0:
        cmp    byte ptr [ebp - 1], bl
        jne    loc_5_1EF6
        mov    dx, word ptr [ebp - 0x2c]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    dx, word ptr [ebp - 0x28]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        mov    dx, word ptr [ebp - 0x30]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_5_2020:
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [ecx + 0x20], esi
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_5_2040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 6
loc_5_2057:
        mov    bl, byte ptr [esi]
        inc    esi
        mov    cx, 8
        mov    edi, edi
loc_5_2060:
        mov    edx, eax
        and    edx, 0x80000000
        cmp    edx, 1
        sbb    edi, edi
        inc    edi
        mov    dl, bl
        and    dx, 1
        xor    di, dx
        add    eax, eax
        shr    bl, 1
        or     di, di
        je     loc_5_208F
        xor    eax, 0x4c11db6
        mov    edx, edi
        and    edx, 0xffff
        or     eax, edx
loc_5_208F:
        dec    cx
        jne    loc_5_2060
        dec    word ptr [ebp - 4]
        jne    loc_5_2057
        shr    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_20B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    dl, dl
        mov    cx, word ptr [ebp + 0xc]
        mov    bx, cx
        shr    bx, 3
        mov    edi, dword ptr [eax + 0x78]
        mov    esi, ebx
        and    esi, 0xffff
        lea    esi, [eax + esi + 0x97]
        mov    al, 1
        and    cl, 7
        shl    al, cl
        cmp    byte ptr [ebp + 0x10], dl
        mov    cl, byte ptr [esi]
        je     loc_5_20F3
        test   cl, al
        jne    loc_5_20EF
        mov    dl, 1
loc_5_20EF:
        or     al, cl
        jmp    loc_5_20FD
loc_5_20F3:
        test   cl, al
        je     loc_5_20F9
        mov    dl, 1
loc_5_20F9:
        not    al
        and    al, cl
loc_5_20FD:
        mov    byte ptr [esi], al
        or     dl, dl
        je     loc_5_2124
        lea    ecx, [edi + 0xe]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 3
        out    dx, ax
        lea    edx, [edi + ebx]
        mov    al, byte ptr [esi]
        out    dx, al
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
loc_5_2124:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_5_2130:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x10]
        cmp    eax, edi
        je     loc_5_2170
        cmp    eax, 1
        je     loc_5_23C4
        cmp    eax, 2
        je     loc_5_2510
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_5_2A5B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_2170:
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010101
        ja     loc_5_2193
        je     loc_5_2340
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_5_21B4
        shl    eax, 2
        jmp    loc_5_2A98
loc_5_2193:
        cmp    eax, 0x1010102
        je     loc_5_2353
        cmp    eax, 0x1010103
        je     loc_5_2366
        cmp    eax, 0x1010104
        je     loc_5_23B0
loc_5_21B4:
        sub    edi, edi
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_5_2A5B
loc_5_21C2:
        mov    ecx, 0xd0                     ; -> obj1:0x0848
        mov    edi, 0x5c
        jmp    loc_5_2A5E
loc_5_21D1:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + 0x6c]
        sub    ax, 2
        cmp    ax, 1
        lea    ecx, [ebp - 8]
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_21F5:
        mov    ecx, 0x12c                    ; -> loc_1_08A4
        mov    edi, 8
        jmp    loc_5_2A5E
loc_5_2204:
        mov    eax, dword ptr [ebp + 0x10]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    loc_5_2A5E
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_2220:
        mov    dword ptr [ebp - 8], 0x5dc
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2234:
        mov    dword ptr [ebp - 8], 0x186a0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2248:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x83]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2263:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x87]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2280:
        push   3
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        push   ecx
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_05EC
loc_5_2295:
        mov    byte ptr [ebp - 0x11], 0
        lea    ecx, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_22A6:
        mov    ecx, 0x134                    ; -> obj1:0x08AC
        mov    edi, 0x2a
        jmp    loc_5_2A5E
loc_5_22B5:
        mov    eax, dword ptr [ebp + 0x10]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    edi, 4
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_22D0:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x7d]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_22E6:
        mov    word ptr [ebp - 2], 0x30a
        lea    ecx, [ebp - 2]
        mov    edi, 2
        jmp    loc_5_2A5E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2300:
        mov    dword ptr [ebp - 8], 0x5ea
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2314:
        mov    dword ptr [ebp - 8], 0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2328:
        mov    dword ptr [ebp - 8], 7
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2340:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        jmp    loc_5_2A5E
loc_5_2353:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x91
        jmp    loc_5_2A5E
loc_5_2366:
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ecx + 0x72]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
loc_5_2389:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_23A1
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_2A5B
loc_5_23A1:
        mov    dword ptr [esi + 0x20], 0
        sub    edi, edi
        jmp    loc_5_2A5B
        db  090h
loc_5_23B0:
        mov    dword ptr [ebp - 8], 0x10
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_23C4:
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010e
        je     loc_5_23F7
        cmp    eax, 0x1010f
        je     loc_5_2449
        cmp    eax, 0x10112
        je     loc_5_2480
        cmp    eax, 0x1010103
        je     loc_5_24B0
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_5_2A5B
loc_5_23F7:
        cmp    dword ptr [esi + 0x1c], 4
        jae    loc_5_2410
        mov    dword ptr [esi + 0x24], 4
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_5_2A5B
loc_5_2410:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
loc_5_241F:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   0
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ecx + 0x72]
        push   eax
        call   sub_2_05E0
loc_5_243A:
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [esi + 0x20], 4
        jmp    loc_5_2A5B
loc_5_2449:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05EC
loc_5_2458:
        cmp    dword ptr [ebp - 8], 0x5ea
        jbe    loc_5_2468
        mov    dword ptr [ebp - 0xc], 0xc000009a
loc_5_2468:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x7d], eax
        jae    loc_5_2A5B
        mov    dword ptr [ecx + 0x7d], eax
        jmp    loc_5_2A5B
        db  090h
loc_5_2480:
        cmp    dword ptr [esi + 0x1c], 4
        jae    loc_5_2499
        mov    dword ptr [esi + 0x24], 4
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_5_2A5B
loc_5_2499:
        push   4
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x10
        push   eax
        call   sub_1_05EC
loc_5_24AB:
        jmp    loc_5_2A5B
loc_5_24B0:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     loc_5_24D4
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_2A5B
loc_5_24D4:
        push   1
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   0
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ecx + 0x72]
        push   eax
        call   sub_2_0300
loc_5_24FB:
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_2A5B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2510:
        mov    edi, 4
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20101
        ja     loc_5_2538
        je     loc_5_2810
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_5_258F
        shl    eax, 2
        jmp    loc_5_2AE4
loc_5_2538:
        cmp    eax, 0x2020d
        ja     loc_5_25D6
        je     loc_5_2920
        cmp    eax, 0x20205
        ja     loc_5_25C3
        je     loc_5_28A0
        cmp    eax, 0x20201
        ja     loc_5_25A0
        je     loc_5_2860
        cmp    eax, 0x20102
        je     loc_5_2820
        cmp    eax, 0x20103
        je     loc_5_2830
        cmp    eax, 0x20104
        je     loc_5_2840
        cmp    eax, 0x20105
        je     loc_5_2850
loc_5_258F:
        sub    edi, edi
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_5_2A5B
        db  08Dh, 049h, 000h
loc_5_25A0:
        cmp    eax, 0x20202
        je     loc_5_2870
        cmp    eax, 0x20203
        je     loc_5_2880
        cmp    eax, 0x20204
        je     loc_5_2890
        jmp    loc_5_258F
loc_5_25C3:
        sub    eax, 0x20206
        cmp    eax, 6
        ja     loc_5_258F
        shl    eax, 2
        jmp    loc_5_2B30
loc_5_25D6:
        cmp    eax, 0x1020103
        ja     loc_5_2640
        je     loc_5_29E0
        cmp    eax, 0x1010103
        ja     loc_5_2616
        je     loc_5_2966
        cmp    eax, 0x2020e
        je     loc_5_2930
        cmp    eax, 0x1010101
        je     loc_5_2940
        cmp    eax, 0x1010102
        je     loc_5_2953
        jmp    loc_5_258F
loc_5_2616:
        cmp    eax, 0x1010104
        je     loc_5_29B0
        cmp    eax, 0x1020101
        je     loc_5_29C4
        cmp    eax, 0x1020102
        je     loc_5_29D2
        jmp    loc_5_258F
        db  08Dh, 064h, 024h, 000h
loc_5_2640:
        sub    eax, 0x1020201
        cmp    eax, 6
        ja     loc_5_258F
        jmp    loc_5_2B4C
loc_5_2655:
        mov    ecx, 0                        ; -> loc_1_0778
        mov    edi, 0xd0
        jmp    loc_5_2A5E
loc_5_2664:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + 0x6c]
        sub    ax, 2
        cmp    ax, 1
        lea    ecx, [ebp - 8]
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_2688:
        mov    ecx, 0x12c                    ; -> loc_1_08A4
        mov    edi, 8
        jmp    loc_5_2A5E
loc_5_2697:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + 0x51]
        or     al, al
        je     loc_5_26D0
        cmp    byte ptr [ecx + 0x52], 0
        je     loc_5_26B6
        mov    ecx, 0x12c                    ; -> loc_1_08A4
        mov    edi, 8
        jmp    loc_5_2A5E
loc_5_26B6:
        or     al, al
        je     loc_5_26D0
        mov    dword ptr [ebp - 8], 5
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_26D0:
        cmp    byte ptr [ecx + 0x52], 0
        je     loc_5_2A5B
        mov    dword ptr [ebp - 8], 0
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_26F0:
        mov    dword ptr [ebp - 8], 0x5dc
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2704:
        mov    dword ptr [ebp - 8], 0x186a0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2718:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x83]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_2733:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x87]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2750:
        push   3
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        push   ecx
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_05EC
loc_5_2765:
        mov    byte ptr [ebp - 0x11], 0
        lea    ecx, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_2776:
        mov    ecx, 0x134                    ; -> obj1:0x08AC
        mov    edi, 0x2a
        jmp    loc_5_2A5E
loc_5_2785:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x72]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_27A0:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 0x7d]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_27B6:
        mov    word ptr [ebp - 2], 0x300
        lea    ecx, [ebp - 2]
        mov    edi, 2
        jmp    loc_5_2A5E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_27D0:
        mov    dword ptr [ebp - 8], 0x5ea
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_27E4:
        mov    dword ptr [ebp - 8], 0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
loc_5_27F8:
        mov    dword ptr [ebp - 8], 7
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2810:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xac
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2820:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb0
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2830:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2840:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xb8
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2850:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xbc
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2860:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc0
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2870:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2880:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xc8
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2890:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xcc
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28A0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd0
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28B0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28C0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xd8
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28D0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xdc
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28E0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe0
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28F0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe4
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2900:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xe8
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2910:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xec
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2920:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf0
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2930:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xa8
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2940:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x8b
        jmp    loc_5_2A5E
loc_5_2953:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x91
        jmp    loc_5_2A5E
loc_5_2966:
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ecx + 0x72]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_0760
loc_5_2982:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_29A0
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 0x20], eax
        jmp    loc_5_2A5B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_29A0:
        mov    dword ptr [esi + 0x20], 0
        sub    edi, edi
        jmp    loc_5_2A5B
        db  08Bh, 0FFh
loc_5_29B0:
        mov    dword ptr [ebp - 8], 0x10
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    loc_5_2A5E
loc_5_29C4:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf4
        jmp    loc_5_2A5E
loc_5_29D2:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xf8
        jmp    loc_5_2A5E
loc_5_29E0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xfc
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_29F0:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x108
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A00:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x100
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A10:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0xbc
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A20:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x10c
        jmp    loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A30:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x110
        jmp    loc_5_2A5E
        db  005h, 000h, 000h, 000h, 000h
loc_5_2A40:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x114
        jmp    loc_5_2A5E
        db  005h, 000h, 000h, 000h, 000h
loc_5_2A50:
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x104
        jmp    loc_5_2A5E
loc_5_2A5B:
        mov    ecx, dword ptr [ebp - 0x10]
loc_5_2A5E:
        or     edi, edi
        je     loc_5_2A89
        cmp    dword ptr [esi + 0x1c], edi
        jae    loc_5_2A74
        mov    dword ptr [esi + 0x24], edi
        mov    edi, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0xc], 0xc0010014
loc_5_2A74:
        mov    dword ptr [esi + 0x20], edi
        mov    eax, dword ptr [esi + 0x18]
        or     edi, edi
        je     loc_5_2A89
        mov    edi, edi
loc_5_2A80:
        mov    dl, byte ptr [ecx]
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        dec    edi
        jne    loc_5_2A80
loc_5_2A89:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_5_2A98:
        adc    cl, byte ptr [eax]
        add    byte ptr [eax], al
        and    dword ptr [eax], ecx
        add    byte ptr [eax], al
        inc    ebp
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx], dl      ; -> loc_5_2204
        add    byte ptr [eax + 8], dh        ; -> loc_5_2220
        add    byte ptr [eax], al
        jo     0x2ab6
        add    byte ptr [eax], al
        test   byte ptr [eax], cl
        add    byte ptr [eax], al
        cwde   
        or     byte ptr [eax], al
        add    byte ptr [eax - 0x4cfffff8], bl ; -> loc_5_2248
        or     byte ptr [eax], al
        add    byte ptr [ebx - 0x2ffffff8], dh ; -> loc_5_2263
        or     byte ptr [eax], al
        add    dh, dh                        ; -> loc_5_22A6
        or     byte ptr [eax], al
        add    byte ptr [0x20000009], al     ; -> loc_5_22B5
        or     dword ptr [eax], eax
        add    byte ptr [esi], dh            ; -> loc_5_22E6
        or     dword ptr [eax], eax
        add    byte ptr [eax + 9], dl        ; -> loc_5_2300
        add    byte ptr [eax], al
        or     dword ptr fs:[eax], eax
        add    byte ptr [eax + 9], bh        ; -> loc_5_2328
        add    byte ptr [eax], al
loc_5_2AE4:
        movsd  dword ptr es:[edi], dword ptr [esi]
        or     al, 0
        add    byte ptr [esp + ecx + 0xcd80000], dh ; -> loc_5_2664
        add    byte ptr [eax], al
        out    0xc, eax
        add    byte ptr [eax], al
        inc    eax
        or     eax, 0xd400000                ; -> loc_5_26F0
        add    byte ptr [eax], al
        push   esp
        or     eax, 0xd680000                ; -> loc_5_2718
        add    byte ptr [eax], al
        push   0x8300000d                    ; -> loc_5_2733
        or     eax, 0xd830000                ; -> loc_5_2733
        add    byte ptr [eax], al
        mov    al, byte ptr [0xc600000d]     ; -> loc_5_2776
        or     eax, 0xdd50000                ; -> loc_5_2785
        add    byte ptr [eax], al
        dd  loc_5_27A0               ; fixup
        dd  loc_5_27B6               ; fixup
        dd  loc_5_27D0               ; fixup
        dd  loc_5_27E4               ; fixup
        dd  loc_5_27F8               ; fixup
loc_5_2B30:
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        and    byte ptr [edi], cl
        add    byte ptr [eax], al
        xor    byte ptr [edi], cl
        add    byte ptr [eax], al
        inc    eax
        sldt   word ptr [eax]
        push   eax
        sldt   word ptr [eax]
        pushal 
        sldt   word ptr [eax]
loc_5_2B4C:
        inc    eax
        adc    byte ptr [eax], al
        add    byte ptr [eax + 0x10], dl     ; -> loc_5_2A00
        add    byte ptr [eax], al
        pushal 
        adc    byte ptr [eax], al
        add    byte ptr [eax + 0x10], dh     ; -> loc_5_2A20
        add    byte ptr [eax], al
        adc    byte ptr [eax], 0
        add    byte ptr [eax - 0x5ffffff0], dl ; -> loc_5_2A40
        adc    byte ptr [eax], al
        add    byte ptr [ebp - 0x72ffdb9c], cl
        and    al, 0
sub_5_2B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x78]
        add    dx, 0xe
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, dword ptr [esi + 0x78]
        add    dx, 0xd
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x78]
        or     byte ptr [ebp - 1], 8
        add    dx, 0xd
        sub    al, al
        mov    cx, word ptr [esi + 0x78]
        out    dx, al
        lea    edx, [ecx + 0xd]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        lea    edx, [ecx + 0xe]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    byte ptr [esi + 0x43], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2BD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x42], 0
        jne    loc_5_2BF2
        cmp    byte ptr [esi + 0x46], 0
        jne    loc_5_2BF2
        push   esi
        push   0                             ; -> loc_5_2C10
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_04FA
loc_5_2BF2:
        mov    byte ptr [esi + 0x46], 0
        push   0x1388
        lea    eax, [esi + 0x34]
        push   eax
        call   sub_1_04E2
loc_5_2C04:
        mov    byte ptr [esi + 0x46], 0
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
loc_5_2C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    al, al
        mov    ebx, dword ptr [edi + 0x78]
        lea    esi, [ebx - 1]
        mov    dx, si
        out    dx, al
        lea    ecx, [ebx + 0xe]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, 2
        out    dx, ax
        lea    edx, [ebx + 0xd]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        sub    al, al
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, cx
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    dx, si
        mov    al, byte ptr [edi + 0x53]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 3
        mov    edx, dword ptr [esi + 0x78]
        add    dx, 0xd
        out    dx, al
        mov    edi, 0x927c
loc_5_2C87:
        push   0x28
        call   sub_1_0500
loc_5_2C8E:
        dec    edi
        jne    loc_5_2C87
        push   esi
        call   sub_5_2CD0
        add    esp, 4
        cmp    eax, 1
        je     loc_5_2CAA
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_2CAA:
        push   0                             ; -> obj1:0x093C
        push   esi
        call   sub_5_2E60
        or     eax, eax
        jne    loc_5_2CC1
        push   esi
        call   sub_5_33C0
        sub    eax, eax
loc_5_2CC1:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 6], 0xf8b7
        mov    esi, dword ptr [eax + 0x78]
        mov    al, byte ptr [eax + 0x7c]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_5_2CF8
        mov    word ptr [ebp - 6], 0xf9b7
loc_5_2CF8:
        sub    edi, edi
        lea    eax, [esi + 0xe]
        mov    word ptr [ebp - 0x10], ax
        lea    eax, [esi + 0x1e]
        mov    word ptr [ebp - 0xc], ax
        lea    eax, [esi + 0x1f]
        mov    word ptr [ebp - 0x18], ax
        lea    eax, [esi + 0x10]
        mov    word ptr [ebp - 0x14], ax
        add    si, 0x11
loc_5_2D1A:
        mov    dx, word ptr [ebp - 0x10]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 2
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   0xa
        call   sub_1_0500
loc_5_2D31:
        and    byte ptr [ebp - 1], 0xfd
        mov    dx, word ptr [ebp - 0x10]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ebx, 0x927c
loc_5_2D42:
        push   0x28
        call   sub_1_0500
loc_5_2D49:
        dec    ebx
        jne    loc_5_2D42
        mov    dx, word ptr [ebp - 0xc]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ebp - 0x18]
        in     al, dx
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0xffff00ff
        or     cx, ax
        mov    dx, word ptr [ebp - 0xc]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [ebp - 0x14]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, word ptr [ebp - 0xc]
        mov    ax, cx
        out    dx, ax
        cmp    ch, 0x33
        je     loc_5_2DB0
        inc    edi
        cmp    edi, 0xa
        jl     loc_5_2D1A
        cmp    ch, 0x33
        je     loc_5_2DB0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_5_2DB0:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_5_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x78]
        lea    edx, [esi - 4]
        sub    esi, 0x10
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xbd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, byte ptr [edi + 0x53]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xfd
        lea    edx, [esi + 0xf]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        lea    ebx, [esi + 0xe]
        mov    dx, bx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 2
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   0xd
        call   sub_1_0500
loc_5_2E0B:
        and    byte ptr [ebp - 1], 0xfd
        mov    dx, bx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ebx, 0x3c8c
        add    eax, 0
loc_5_2E20:
        push   0x28
        call   sub_1_0500
loc_5_2E27:
        dec    ebx
        jne    loc_5_2E20
        lea    edx, [esi + 0x1e]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0x10]
        mov    al, 0xb7
        out    dx, al
        lea    edx, [esi + 0x11]
        mov    al, 0xf8
        out    dx, al
        lea    edx, [esi + 0xd]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [edi + 0x53]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xfd
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_5_2E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    esi, dword ptr [edi + 0x78]
        mov    cl, byte ptr [edi + 0x7c]
        lea    eax, [esi + 0x1e]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        lea    edx, [esi + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     ecx, ecx
        je     loc_5_2EA9
        cmp    ecx, 1
        je     loc_5_2EB6
        cmp    ecx, 2
        je     loc_5_2ED2
loc_5_2E95:
        lea    edx, [esi + 0x1e]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_2EA9:
        and    byte ptr [ebp - 1], 0xbf
        lea    edx, [esi + 0xc]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        jmp    loc_5_2E95
loc_5_2EB6:
        or     byte ptr [ebp - 1], 0x40
        lea    edx, [esi + 0xc]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edi, 0xfa
loc_5_2EC6:
        push   0x28
        call   sub_1_0500
loc_5_2ECD:
        dec    edi
        jne    loc_5_2EC6
        jmp    loc_5_2E95
loc_5_2ED2:
        lea    edx, [esi + 0x1e]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [esi + 0x14]
        mov    ax, word ptr [eax]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        lea    ecx, [eax + 2]
        lea    edx, [esi + 0x16]
        mov    ax, word ptr [ecx]
        out    dx, ax
        lea    edx, [esi + 0x18]
        mov    ax, word ptr [ecx + 2]
        and    byte ptr [ebp - 1], 0xbf
        out    dx, ax
        lea    edx, [esi + 0xc]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    byte ptr [edi + 0x7c], 0
        lea    edx, [esi + 0x10]
        mov    ax, 0xf8b7
        out    dx, ax
        mov    ebx, 0xfa
        lea    ebx, [ebx]
loc_5_2F20:
        push   0x28
        call   sub_1_0500
loc_5_2F27:
        dec    ebx
        jne    loc_5_2F20
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_5_2FC0
        or     eax, eax
        je     loc_5_2E95
        or     byte ptr [ebp - 1], 0x40
        lea    edx, [esi + 0xc]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    byte ptr [edi + 0x7c], 1
        lea    edx, [esi + 0x10]
        mov    ax, 0xf9b7
        out    dx, ax
        mov    ebx, 0xfa
loc_5_2F59:
        push   0x28
        call   sub_1_0500
loc_5_2F60:
        dec    ebx
        jne    loc_5_2F59
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_5_2FC0
        or     eax, eax
        je     loc_5_2E95
        push   0
        push   0xaa00000f
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_05D8
loc_5_2F85:
        add    esp, 0xc
        mov    byte ptr [edi + 0x7c], 0
        lea    edx, [esi + 0x10]
        add    dword ptr [edi + 0x78], 0x10
        mov    ax, 0xf8b7
        out    dx, ax
        push   edi
        call   sub_5_2DC0
        sub    dword ptr [edi + 0x78], 0x10
        mov    eax, 0xc001001f
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x114
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0x78]
        add    esi, 0x10
        mov    dword ptr [ebp - 0x10], 0xc0000001
        lea    eax, [esi + 0xe]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [esi + 0xe]
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 0xc01
        out    dx, ax
        lea    edx, [esi + 4]
        mov    ax, 0x300
        out    dx, ax
        lea    edx, [esi + 0xe]
        mov    ax, 2
        out    dx, ax
        lea    edx, [esi + 0xd]
        sub    al, al
        out    dx, al
        mov    al, 0x3b
        out    dx, al
        mov    dx, si
        mov    ax, 0x20
        out    dx, ax
        sub    ebx, ebx
        mov    edi, 0xffffffff
loc_5_3024:
        lea    edx, [esi + 0xc]
        in     al, dx
        test   al, 8
        jne    loc_5_3037
        inc    ebx
        cmp    ebx, 0xc8
        jl     loc_5_3024
        jmp    loc_5_3039
loc_5_3037:
        sub    edi, edi
loc_5_3039:
        or     edi, edi
        je     loc_5_3049
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_31EA
loc_5_3049:
        lea    edx, [esi + 3]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 0x80
        je     loc_5_3060
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_31EA
loc_5_3060:
        lea    eax, [esi + 2]
        mov    dx, ax
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    edx, [esi + 6]
        mov    ax, 0x4000
        out    dx, ax
        lea    edi, [esi + 8]
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        mov    ax, 0x116
        out    dx, ax
        sub    ebx, ebx
        mov    ecx, dword ptr [ebp + 0xc]
loc_5_3089:
        mov    dx, di
        mov    al, byte ptr [ecx + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     loc_5_3089
        sub    ebx, ebx
loc_5_3098:
        mov    dx, di
        mov    al, byte ptr [ecx + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     loc_5_3098
        mov    ax, 0xff16
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        lea    ecx, [ebp - 0x114]
loc_5_30B6:
        mov    dx, di
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    ecx, 2
        lea    eax, [ebp - 0x14]
        cmp    ecx, eax
        jb     loc_5_30B6
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 0xc0
        out    dx, ax
        sub    ebx, ebx
        mov    dword ptr [ebp - 0x14], 0xffffffff
        nop    
loc_5_30E0:
        lea    edx, [esi + 0xc]
        in     al, dx
        test   al, 0x20
        jne    loc_5_3100
        test   al, 2
        jne    loc_5_3110
        push   0xa
        call   sub_1_0500
loc_5_30F3:
        inc    ebx
        cmp    ebx, 0x3a98
        jl     loc_5_30E0
        jmp    loc_5_3117
        db  08Bh, 0FFh
loc_5_3100:
        mov    dword ptr [ebp - 0x14], 1
        jmp    loc_5_3117
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_3110:
        mov    dword ptr [ebp - 0x14], 0
loc_5_3117:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_5_312B
        mov    dword ptr [ebp - 0x10], 0xc0000001
        mov    dword ptr [ebp - 0x14], 0xffffffff
loc_5_312B:
        lea    eax, [esi + 4]
        mov    dx, ax
        in     al, dx
        mov    byte ptr [ebp - 2], al
        lea    eax, [esi + 2]
        mov    dx, ax
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    edx, [esi + 6]
        mov    ax, 0x6000
        out    dx, ax
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        in     ax, dx
        mov    dx, si
        mov    ax, 0xa0
        out    dx, ax
        lea    edx, [esi + 0xc]
        mov    al, 2
        out    dx, al
        mov    ecx, 1
loc_5_3167:
        mov    dx, si
        in     ax, dx
        test   cx, ax
        jne    loc_5_3167
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_5_31EA
        test   word ptr [ebp - 8], 0x8e10
        je     loc_5_3188
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_31EA
loc_5_3188:
        sub    edi, edi
        mov    ebx, 0xffffffff
        nop    
loc_5_3190:
        push   0xa
        call   sub_1_0500
loc_5_3197:
        lea    edx, [esi + 0xc]
        in     al, dx
        test   al, 1
        jne    loc_5_31AA
        inc    edi
        cmp    edi, 0x30d4
        jl     loc_5_3190
        jmp    loc_5_31AC
loc_5_31AA:
        sub    ebx, ebx
loc_5_31AC:
        or     ebx, ebx
        je     loc_5_31C0
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_31EA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_31C0:
        mov    dword ptr [ebp - 0x10], 0
        lea    edx, [esi + 6]
        mov    ax, 0xe000
        out    dx, ax
        mov    dx, si
        mov    ax, 0x80
        out    dx, ax
        mov    ecx, 1
        mov    edi, edi
loc_5_31E0:
        mov    dx, si
        in     ax, dx
        test   cx, ax
        jne    loc_5_31E0
loc_5_31EA:
        lea    edx, [esi + 0xe]
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        out    dx, ax
        lea    edx, [esi + 4]
        out    dx, ax
        lea    edx, [esi + 0xe]
        mov    ax, 2
        out    dx, ax
        lea    edx, [esi + 0xd]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 0xe]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_3220:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [0x50], ax           ; -> obj7:0x0050
        mov    word ptr [0x52], 0            ; -> obj7:0x0052
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0x54], al           ; -> obj7:0x0054
        mov    byte ptr [0x55], 0            ; -> obj7:0x0055
        push   0x50                          ; -> obj7:0x0050
        push   0x12
        push   0
        push   0
        push   7
        call   sub_6_005B
loc_5_3257:
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_5_3260:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [0x50], ax           ; -> obj7:0x0050
        mov    word ptr [0x52], 0            ; -> obj7:0x0052
        mov    byte ptr [0x55], 0            ; -> obj7:0x0055
        push   0x50                          ; -> obj7:0x0050
        push   0x12
        push   0
        push   0
        push   0xa
        call   sub_6_005B
loc_5_328F:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_32A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        mov    word ptr [0x50], ax           ; -> obj7:0x0050
        mov    ax, word ptr [esi + 2]
        mov    word ptr [0x52], ax           ; -> obj7:0x0052
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [0x54], al           ; -> obj7:0x0054
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0x55], al           ; -> obj7:0x0055
        mov    ax, word ptr [esi + 6]
        mov    word ptr [0x56], ax           ; -> obj7:0x0056
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [0x58], eax         ; -> obj7:0x0058
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [0x5c], eax         ; -> obj7:0x005C
        mov    word ptr [0x60], 0x60         ; -> obj7:0x0060
        push   0x50                          ; -> obj7:0x0050
        push   0x60
        push   0
        push   0
        push   0xd
        call   sub_6_005B
loc_5_32FF:
        add    esp, 0x14
        mov    cx, word ptr [0x56]           ; -> obj7:0x0056
        mov    word ptr [esi + 6], cx
        mov    ecx, dword ptr [0x58]         ; -> obj7:0x0058
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [0x5c]         ; -> obj7:0x005C
        mov    dword ptr [esi + 0xc], ecx
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3330:
        push   esi
        push   edi
        call   sub_6_0000
loc_5_3337:
        or     eax, eax
        je     loc_5_3343
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        ret    
loc_5_3343:
        cmp    word ptr [0], 0x200           ; -> loc_1_08D8
        mov    si, 0
        ja     loc_5_3356
        mov    si, 1
loc_5_3356:
        sub    edi, edi
        cmp    word ptr [0], di              ; -> loc_1_08DC
        je     loc_5_337E
loc_5_3361:
        push   esi
        call   sub_5_33E0
        add    esp, 4
        cmp    eax, 1
        je     loc_5_337E
        inc    edi
        inc    si
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> loc_1_08DC
        cmp    eax, edi
        jg     loc_5_3361
loc_5_337E:
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> loc_1_08DC
        cmp    eax, edi
        jne    loc_5_3392
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        ret    
loc_5_3392:
        mov    word ptr [0], si              ; -> loc_1_08DE
        push   0                             ; -> obj1:0x093C
        push   esi
        call   sub_5_3530
        cmp    eax, 1
        je     loc_5_33B1
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        ret    
loc_5_33B1:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_33C0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 0x78]
        add    dx, 0xf
        mov    al, byte ptr [eax + 0x53]
        out    dx, al
        sub    eax, eax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_33E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   0x15
        mov    ax, word ptr [ebp + 8]
        push   eax
        call   sub_5_3220
        or     eax, eax
        jne    loc_5_3520
        push   0x12
        push   0x50                          ; -> obj7:0x0050
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_05EC
loc_5_340D:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_5_32A0
        mov    edx, 0x16
        sub    edi, edi
        mov    esi, 1
        cmp    byte ptr [0], 0               ; -> loc_1_0910
        je     loc_5_345A
        sub    ecx, ecx
        nop    
loc_5_3430:
        mov    al, byte ptr [edx + 0x50]     ; -> obj7:0x0050
        cmp    al, 0x39
        jbe    loc_5_343C
        and    al, 0xdf
loc_5_343C:
        movsx  ebx, byte ptr [edi]           ; -> loc_1_0910
        and    eax, 0xff
        cmp    ebx, eax
        jne    loc_5_3458
        inc    edi
        inc    edx
        cmp    byte ptr [edi], cl            ; -> loc_1_0910
        jne    loc_5_3430
        jmp    loc_5_345A
loc_5_3458:
        sub    esi, esi
loc_5_345A:
        or     esi, esi
        jne    loc_5_3467
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_3467:
        sub    edi, edi
        sub    ebx, ebx
        sub    eax, eax
loc_5_346D:
        cmp    edi, 2
        jge    loc_5_34A0
        cmp    byte ptr [edx + 0x50], al     ; -> obj7:0x0050
        je     loc_5_349A
loc_5_347A:
        cmp    edi, 1
        jne    loc_5_3492
        cmp    ebx, 0x4d
        jae    loc_5_3492
        mov    cl, byte ptr [edx + 0x50]     ; -> obj7:0x0050
        mov    byte ptr [ebx], cl            ; -> obj7:0x0000
        inc    edx
        inc    ebx
loc_5_3492:
        cmp    byte ptr [edx + 0x50], al     ; -> obj7:0x0050
        jne    loc_5_347A
loc_5_349A:
        inc    edi
        inc    edx
        jmp    loc_5_346D
        db  08Bh, 0FFh
loc_5_34A0:
        mov    byte ptr [ebx], 0             ; -> obj7:0x0000
        cmp    dword ptr [0x10], 0           ; -> loc_1_0920
        je     loc_5_3506
        mov    dword ptr [ebp - 4], 0x10     ; -> loc_1_0920
loc_5_34B7:
        mov    edi, edx
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ebx, dword ptr [ecx]
        sub    esi, esi
        cmp    byte ptr [ebx], al
        je     loc_5_34F6
loc_5_34C6:
        mov    cl, byte ptr [edi + 0x50]     ; -> obj7:0x0050
        cmp    cl, 0x39
        jbe    loc_5_34D4
        and    cl, 0xdf
loc_5_34D4:
        movsx  esi, byte ptr [eax + ebx]
        and    ecx, 0xff
        cmp    esi, ecx
        mov    esi, 0
        jne    loc_5_34F6
        mov    esi, 1
        add    eax, esi
        add    edi, esi
        cmp    byte ptr [eax + ebx], 0
        jne    loc_5_34C6
loc_5_34F6:
        or     esi, esi
        jne    loc_5_3513
        add    dword ptr [ebp - 4], 4
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx], 0
        jne    loc_5_34B7
loc_5_3506:
        or     esi, esi
        jne    loc_5_3513
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_3513:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
loc_5_3520:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3530:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   0x90
        mov    si, word ptr [ebp + 8]
        push   esi
        call   sub_5_3220
        or     eax, eax
        jne    loc_5_35B0
        sub    ebx, ebx
        mov    edi, dword ptr [ebp - 4]
loc_5_3551:
        or     ebx, ebx
        jne    loc_5_35C4
        mov    eax, 0x50                     ; -> obj7:0x0050
        lea    ecx, [ebp - 0x18]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ecx + 0x10], ax
        mov    eax, ecx
        push   0
        push   eax
        call   sub_5_32A0
        or     eax, eax
        jne    loc_5_35B5
        mov    edi, 0x14
        cmp    byte ptr [0x64], 4            ; -> obj7:0x0064
        jne    loc_5_35A0
        mov    ebx, 1
        jmp    loc_5_3551
        db  08Bh, 0FFh
loc_5_35A0:
        push   esi
        call   sub_5_3260
        or     eax, eax
        je     loc_5_3551
        jmp    loc_5_35B5
        db  08Dh, 064h, 024h, 000h
loc_5_35B0:
        sub    ebx, ebx
        mov    edi, dword ptr [ebp - 4]
loc_5_35B5:
        or     ebx, ebx
        jne    loc_5_35C4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_35C4:
        add    edi, 2
        sub    dl, dl
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_5_35D0:
        mov    cl, byte ptr [edi + 0x50]     ; -> obj7:0x0050
        add    dl, cl
        mov    byte ptr [esi + eax], cl
        inc    esi
        inc    edi
        cmp    esi, 6
        jl     loc_5_35D0
        add    dl, byte ptr [edi + 0x50]     ; -> obj7:0x0050
        je     loc_5_35F5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_35F5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x174  R-X
sub_6_0000:
        push   ebx
        push   esi
        push   edi
        sub    esp, 0x10
        VxDCall  Dev_097C.Service_0000h          ; cd2000007c09
        jb     loc_6_0054
        mov    al, 0xb
        xor    edx, edx
        xor    esi, esi
        mov    cx, 0xe
        mov    ebx, esp
        mov    word ptr [ebx + 2], 0
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        cmp    ax, 0
        jne    loc_6_0054
        mov    ax, 0x19
        cmp    byte ptr [ebx + 2], 0x43
        jne    loc_6_0054
        cmp    byte ptr [ebx + 3], 0x53
        jne    loc_6_0054
        mov    ax, word ptr [ebx + 4]
        mov    word ptr [0x8dc], ax          ; -> loc_1_08DC
        mov    ax, word ptr [ebx + 8]
        mov    word ptr [0x8d8], ax          ; -> loc_1_08D8
        mov    ax, 0
loc_6_0054:
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_6_005B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 8]
        mov    dx, word ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    cx, word ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        cmp    dword ptr [0x8e8], 0          ; -> loc_1_08E8
        jne    loc_6_009B
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0x8e8], ebx        ; -> loc_1_08E8
loc_6_009B:
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [0x8e8]        ; -> loc_1_08E8
        VxDCall  VPICD.VPICD_Convert_IRQ_To_Int  ; cd200d000300
        jb     loc_6_00BF
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        je     loc_6_00B8
        xor    eax, eax
        jmp    loc_6_00C4
loc_6_00B8:
        mov    eax, 2
        jmp    loc_6_00C4
loc_6_00BF:
        mov    eax, 1
loc_6_00C4:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    word ptr [ebx], cx
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], edx
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        cmp    al, 0x14
        je     loc_6_010F
        cmp    al, 0xd
        jne    loc_6_00E8
        mov    word ptr [0x8da], dx          ; -> loc_1_08DA
        jmp    loc_6_0108
loc_6_00E8:
        cmp    al, 5
        jne    loc_6_0157
        cmp    word ptr [0x8e0], 1           ; -> loc_1_08E0
        jne    loc_6_0108
        cmp    cx, word ptr [0x8de]          ; -> loc_1_08DE
        jne    loc_6_0108
        mov    word ptr [0x8e0], 0           ; -> loc_1_08E0
loc_6_0108:
        mov    esp, ebp
        pop    ebp
        xor    ax, ax
        ret    
loc_6_010F:
        mov    word ptr [ebx + 4], 0xc
        mov    word ptr [ebx + 8], 0x210
        mov    word ptr [ebx + 6], 0x100
        mov    word ptr [ebx + 0xa], 0x1e22
        mov    word ptr [ebx + 0xc], 0x14
        mov    word ptr [ebx + 0xe], 1
        mov    word ptr [ebx + 0x10], 0x15
        mov    word ptr [ebx + 0x12], 1
        mov    edi, ebx
        add    edi, 0x14
        mov    esi, 0x8ec                    ; -> loc_1_08EC
        mov    cx, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebx + 2], 0x16
        jmp    loc_6_0108
loc_6_0157:
        cmp    al, 0x40
        je     loc_6_015D
        jmp    loc_6_0108
loc_6_015D:
        cmp    word ptr [0x8e0], 1           ; -> loc_1_08E0
        je     loc_6_0108
        jne    loc_6_0108
        mov    word ptr [0x8e0], 1           ; -> loc_1_08E0
        jmp    loc_6_0108
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x150  RW-
        db  04Ch, 001h, 003h, 000h, 0CAh, 0F3h, 040h, 02Fh, 01Dh, 014h, 000h, 000h, 069h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Eh, 064h, 061h, 074h, 061h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 025h, 000h, 000h, 000h, 08Ch, 000h, 000h, 000h, 0B1h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 040h, 000h, 030h, 0C0h, 02Eh, 074h, 065h, 078h
        db  074h, 000h, 000h, 000h, 025h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0A0h, 009h, 000h, 000h
        db  0BBh, 000h, 000h, 000h, 05Bh, 00Ah, 000h, 000h, 03Fh, 00Dh, 000h, 000h, 04Ah, 000h, 025h, 001h
        db  020h, 000h, 050h, 060h, 02Eh, 062h, 073h, 073h, 000h, 000h, 000h, 000h, 0C5h, 009h, 000h, 000h
        db  000h, 000h, 000h, 000h, 050h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 030h, 0C0h, 04Fh, 053h, 049h, 054h
        db  045h, 043h, 048h, 000h, 034h, 030h, 030h, 030h, 031h, 039h, 000h, 000h, 008h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 006h, 000h, 055h, 08Bh, 0ECh, 056h, 057h
        db  08Bh, 075h, 008h, 0B0h, 003h, 08Bh, 056h, 078h, 066h, 083h, 0C2h, 00Dh, 0EEh, 0BFh, 07Ch, 092h
        db  000h, 000h, 06Ah, 028h, 0E8h, 000h, 000h, 000h, 000h, 04Fh, 075h, 0F6h, 056h, 0E8h, 000h, 000h
        db  000h, 000h, 083h, 0C4h, 004h, 083h, 0F8h, 001h, 074h, 00Bh, 0B8h, 001h, 000h, 000h, 0C0h, 05Fh
        db  05Eh, 05Dh, 0C2h, 004h, 000h, 068h, 000h, 000h, 000h, 000h, 056h, 0E8h, 000h, 000h, 000h, 000h
        db  00Bh, 0C0h, 075h, 008h, 056h, 0E8h, 000h, 000h, 000h, 000h, 02Bh, 0C0h, 05Fh, 05Eh, 05Dh, 0C2h
        db  004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 0ECh, 083h, 0ECh
        db  018h, 053h, 056h, 057h, 08Bh, 045h, 008h, 066h, 0C7h, 045h, 0FAh, 0B7h, 0F8h, 08Bh, 070h, 078h
        db  08Ah, 040h, 07Ch, 025h, 0FFh, 000h, 000h, 000h, 083h, 0F8h, 001h, 075h, 006h, 066h, 0C7h, 045h
        db  0FAh, 0B7h, 0F9h, 02Bh, 0FFh, 08Dh, 046h, 00Eh, 066h, 089h, 045h, 0F0h, 08Dh, 046h, 01Eh, 066h
VxD__LDATA ENDS
