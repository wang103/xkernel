; Use the INT 0x15, eax=0xE820 BIOS function to get a memory map.
; Sets the carry flag (CF) to 1 if failed.
; inputs:
;   @es:di -> destination buffer of 24 bytes.
; outputs:
;   @bp -> entry count, trashes all registers except esi.

do_e820:
    xor ebx, ebx                ; ebx must be 0 to start
    xor bp, bp                  ; keep an entry count in bp
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
    jmp short .jmpin

.e820lp:
    mov eax, 0xE820
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc short .e820f             ; carry set means "end of list reached"
    mov edx, 0x534D4150

.jmpin:
    jcxz .skipent
    cmp cl, 20                  ; got a 24 byte ACPI 3.X response?
    jbe short .notext
    test byte [es:di + 20], 1   ; if so, is the "ignore this entry" bit set?
    je short .skipent

.notext:
    mov ecx, [es:di + 8]        ; get lower dword of memory region length
    or ecx, [es:di + 12]        ; or it with upper dword to test for zero
    jz .skipent                 ; if length qword is 0, skip entry
    inc bp                      ; got a good entry
    add di, 24                  ; move to next storage spot 

.skipent:
    test ebx, ebx               ; if ebx resets to 0, list is complete
    jne short .e820lp

.e820f:
    mov [mmap_ent], bp          ; store the entry count 
    clc                         ; clear the carry
    ret

.failed:
    stc                         ; "function unsupported" error exit
    ret
