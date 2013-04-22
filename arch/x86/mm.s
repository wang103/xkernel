; Use the INT 0x15, eax=0xE820 BIOS function to get physical memory size.
; inputs:   destination buffer of 24 bytes.
; outputs:  physical memory size, or -1 if failed.
[GLOBAL get_phys_mem_size]

get_phys_mem_size:
    mov eax, [esp + 4]          ; get the pointer to the buffer
    mov [es:di], eax

    xor ebx, ebx                ; ebx must be 0 to start
    xor ebp, ebp                ; store memory size in ebp
    mov edx, 0x534D4150         ; place "SMAP" into edx
    
    mov eax, 0xE820
    mov [es:di + 20], dword 1   ; to make map compatible with ACPI
    mov ecx, 24                 ; ask for 24 bytes
    
    int 0x15

    jc short .failed            ; carry set on first call means 
                                ; "unsupported function"
    mov edx, 0x534D4150         ; replacing in case it's trashed
    cmp eax, edx                ; should be same on success
    jne short .failed

    test ebx, ebx               ; ebx must be non-zero
    je short .failed

    ; success

    mov ecx, [es:di + 8]        ; get length
    add ebp, ecx

.repeat:
    mov eax, 0xE820
    mov [es:di + 20], dword 1   ; to make map compatible with ACPI
    mov ecx, 24                 ; ask for 24 bytes
    
    int 0x15

    jc short .done              ; carry set means it's over

    ; success
    
    mov edx, 0x534D4150         ; replacing in case it's trashed
    
    mov ecx, [es:di + 8]        ; get length
    add ebp, ecx

    test ebx, ebx               ; if ebx is zero, it's the end
    je short .done

    jmp short .repeat

.done:
    stc
    mov eax, ebp
    ret

.failed:
    stc                         ; "function unsupported" error exit
    mov eax, -1                 ; -1 is the error code
    ret
