; DSVXD -- a virtual device driver, reconstructed from the image by CodeBreaker.
;
; One segment per object of the original. Nothing here is an address: every place the
; loader patches is written as the label it refers to. Labels are L<object>_<offset in the
; original>; names the image itself carries (entry points, the descriptor block) are used
; where there are any. Instructions written as bytes are ones the assembler would encode
; differently from the original; their mnemonic is in the comment.

.386p
.model flat

; object 1: 565 bytes, preload
S1 segment dword public use32 'O1CODE'
DSVXD_control:
        cmp    eax, 1
        jne    short L1_0000A
        jmp  L2_00000                        ; jmp 0x7a000010
L1_0000A:
        cmp    eax, 2
        jne    short L1_00014
        jmp  L2_000FF                        ; jmp 0x7a000024
L1_00014:
        cmp    eax, 5
        jne    short L1_0001B
        jmp    short L1_0001D
L1_0001B:
        clc
        ret
L1_0001D:
        xor    esi, esi
        mov    edx, dword ptr [L1_00218]
        or     edx, edx
        je     short L1_00030
        xor    ecx, ecx
        call   near ptr L1_0015E
L1_00030:
        mov    edx, dword ptr [L1_00214]
        or     edx, edx
        je     short L1_00043
        mov    cx, 04000h
        call   near ptr L1_0015E
L1_00043:
        xor    ecx, ecx
        xchg   dword ptr [L1_00208], ecx
        jecxz  L1_00055
        mov    edi, dword ptr [L1_00204]
        mov    dword ptr [edi], ecx
L1_00055:
        clc
        ret
DSVXD_v86_api:
        movzx  ecx, word ptr [ebp + 01Ch]
        jecxz  L1_000D4
        cmp    ecx, 2
        ja     short L1_000D4
        movzx  eax, byte ptr [ebp + 010h]
        cmp    eax, dword ptr [L1_00224]
        jae    short L1_000D4
        mov    esi, offset L1_00214
        mov    edi, offset L1_00218
        cmp    cl, 1
        mov    cx, 04000h
        je     short L1_00085
        xchg   edi, esi
        xor    ecx, ecx
L1_00085:
        btr    dword ptr [edi], eax
        jb     short L1_0008D
        bts    dword ptr [esi], eax
L1_0008D:
        push   ebx
        mul    dword ptr [L1_00220]
        add    eax, dword ptr [L1_0021C]
L1_0009A:
        int  20h
        dd   00001003Bh    ; Get_Next_VM_Handle
        cmp    ebx, dword ptr [esp]
        je     short L1_000CD
        test   dword ptr [ebx], 010h
        jne    short L1_0009A
        test   dword ptr [ebx], 8
        jne    short L1_000C6
        mov    esi, dword ptr [ebx + 4]
        add    esi, eax
        and    word ptr [esi + 043h], 0BFFFh
        or     word ptr [esi + 043h], cx
        jmp    short L1_0009A
L1_000C6:
        call   near ptr L1_000DA
        jmp    short L1_0009A
L1_000CD:
        pop    ebx
        and    word ptr [ebp + 02Ch], 0FFFEh
        ret
L1_000D4:
        or     word ptr [ebp + 02Ch], 1
        ret
L1_000DA:
        push   eax
        movzx  eax, byte ptr [ebp + 010h]
        mov    edi, dword ptr [L1_00210]
        add    edi, ebx
        jecxz  L1_000F4
        btr    dword ptr [edi + 4], eax
        jb     short L1_000FD
        bts    dword ptr [edi], eax
        jmp    short L1_000FD
L1_000F4:
        btr    dword ptr [edi], eax
        jb     short L1_000FD
        bts    dword ptr [edi + 4], eax
L1_000FD:
        mov    eax, dword ptr [edi]
        or     eax, dword ptr [edi + 4]
        je     short L1_0011B
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        jne    short L1_00119
        mov    esi, offset L1_0012C
        int  20h
        dd   00001000Fh    ; Schedule_VM_Event
        mov    dword ptr [edi + 8], esi
L1_00119:
        pop    eax
        ret
L1_0011B:
        xchg   dword ptr [edi + 8], eax
        or     eax, eax
        je     short L1_00119
        mov    esi, eax
        int  20h
        dd   000010013h    ; Cancel_VM_Event
        jmp    short L1_00119
L1_0012C:
        mov    esi, dword ptr [ebx + 4]
        mov    edi, dword ptr [L1_00210]
        add    edi, ebx
        xor    ecx, ecx
        xor    edx, edx
        xchg   dword ptr [edi + 4], edx
        or     edx, edx
        je     short L1_00147
        call   near ptr L1_0015E
L1_00147:
        mov    cx, 04000h
        xor    edx, edx
        xchg   dword ptr [edi], edx
        or     edx, edx
        je     short L1_00158
        call   near ptr L1_0015E
L1_00158:
        xor    eax, eax
        mov    dword ptr [edi + 8], eax
        ret
L1_0015E:
        bsf    eax, edx
        je     short L1_00182
        btr    edx, eax
        push   edx
        mul    dword ptr [L1_00220]
        pop    edx
        add    eax, dword ptr [L1_0021C]
        add    eax, esi
        and    word ptr [eax + 043h], 0BFFFh
        or     word ptr [eax + 043h], cx
        jmp    short L1_0015E
L1_00182:
        ret
L1_00183:
        mov    ecx, 5
        int  20h
        dd   00001001Fh    ; Begin_Critical_Section
        mov    ax, 0FF53h
        mov    ebx, 0F3830180h
        mov    cl, 1
        mov    esi, offset L1_001C8
        int  20h
        dd   0000D0001h    ; VKD_Define_Hot_Key
L1_001A4:
        mov    edi, offset L1_00228
        mov    eax, 080001030h
        mov    ecx, dword ptr [L1_0020C]
        int  20h
        dd   000170003h    ; SHELL_SYSMODAL_Message
        jmp    short L1_001A4
        db   0CDh, 020h, 020h, 000h, 001h, 000h, 0CDh, 020h, 04Ah, 080h, 001h, 000h
L1_001C8:
        ret
        db   000h, 000h, 000h
public DSVXD_DDB
DSVXD_DDB:
        db   000h, 000h, 000h, 000h, 000h, 003h, 03Bh, 000h, 003h, 000h, 000h, 000h
        db   'DSVXD   '
        db   000h, 000h, 000h, 080h
        dd   offset DSVXD_control
        dd   offset DSVXD_v86_api
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
L1_00204:
        db   000h, 000h, 000h, 000h
L1_00208:
        db   000h, 000h, 000h, 000h
L1_0020C:
        db   000h, 000h, 000h, 000h
L1_00210:
        db   000h, 000h, 000h, 000h
L1_00214:
        db   000h, 000h, 000h, 000h
L1_00218:
        db   000h, 000h, 000h, 000h
L1_0021C:
        db   000h, 000h, 000h, 000h
L1_00220:
        db   058h, 000h, 000h, 000h
L1_00224:
        db   000h, 000h, 000h, 000h
L1_00228:
        db   000h
L1_00229:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_0022D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_00231:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
S1 ends

; object 2: 348 bytes, loadoncall discardable
S2 segment dword public use32 'O2CODE'
L2_00000:
        push   0
        push   0Ch
        int  20h
        dd   0000100A7h    ; _Allocate_Device_CB_Area
        add    esp, 8
        or     eax, eax
        je     near ptr L2_000F9
        mov    dword ptr [L1_00210], eax
        sub    esp, 06Ch
        push   edi
        lea    edi, [esp + 4]
        int  20h
        dd   00001008Dh    ; Save_Client_State
        pop    edi
        int  20h
        dd   000010083h    ; Begin_Nest_Exec
        mov    esi, dword ptr [ebx + 8]
        mov    word ptr [esi + 01Ch], 05200h
        mov    eax, 021h
        int  20h
        dd   000010084h    ; Exec_Int
        movzx  edi, word ptr [esi + 038h]
        shl    edi, 4
        movzx  ecx, word ptr [esi + 010h]
        add    edi, ecx
        mov    eax, dword ptr [edi + 016h]
        movzx  ecx, ax
        shr    eax, 010h
        shl    eax, 4
        add    eax, ecx
        mov    dword ptr [L1_0021C], eax
        movzx  eax, byte ptr [edi + 021h]
        mov    dword ptr [L1_00224], eax
        mov    word ptr [esi + 01Ch], 04A11h
        mov    word ptr [esi + 010h], 0Eh
        mov    eax, 02Fh
        int  20h
        dd   000010084h    ; Exec_Int
        cmp    word ptr [esi + 01Ch], 0
        jne    short L2_000E2
        movzx  edx, word ptr [esi + 010h]
        movzx  eax, word ptr [esi + 038h]
        shl    eax, 4
        lea    edx, [edx + eax - 0Ah]
        add    edx, dword ptr [ebx + 4]
L2_0009C:
        add    edx, 0Ah
        cmp    word ptr [edx], 0
        je     short L2_000E2
        cmp    dword ptr [edx + 4], 0
        je     short L2_0009C
        mov    edx, dword ptr [edx + 4]
        shld   eax, edx, 014h
        and    eax, 0FFFF0h
        and    edx, 0FFFFh
        add    edx, eax
        add    edx, dword ptr [ebx + 4]
        mov    dword ptr [L1_00229], edx
        movzx  eax, word ptr [edx + 2]
        mov    dword ptr [L1_0022D], eax
        shl    eax, 4
        movzx  edx, word ptr [edx + 8]
        add    eax, edx
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [L1_00231], eax
L2_000E2:
        int  20h
        dd   000010086h    ; End_Nest_Exec
        push   esi
        lea    esi, [esp + 4]
        int  20h
        dd   00001008Eh    ; Restore_Client_State
        pop    esi
        add    esp, 06Ch
        clc
        ret
L2_000F9:
        int  20h
        dd   0000100BFh    ; Fatal_Memory_Error
L2_000FF:
        movzx  eax, dx
        shr    edx, 010h
        shl    edx, 4
        add    edx, eax
        mov    dword ptr [L1_00204], edx
        mov    eax, dword ptr [L1_00231]
        or     eax, eax
        je     short L2_0015B
        movzx  eax, word ptr [eax + 2]
        cmp    eax, dword ptr [L1_0022D]
        je     short L2_0015B
        mov    eax, dword ptr [L1_00229]
        test   word ptr [eax + 6], 1
        jne    short L2_0015B
        mov    esi, offset L1_00183
        int  20h
        dd   00001000Bh    ; Allocate_V86_Call_Back
        jb     short L2_0015B
        xchg   dword ptr [edx], eax
        mov    dword ptr [L1_00208], eax
        mov    edx, dword ptr [edx + 4]
        movzx  eax, dx
        shr    edx, 010h
        shl    edx, 4
        add    edx, eax
        mov    dword ptr [L1_0020C], edx
        clc
L2_0015B:
        ret
S2 ends

end
