[GLOBAL disable_pic]

disable_pic:
    mov al, 0xff
    out 0xa1, al
    out 0x21, al
