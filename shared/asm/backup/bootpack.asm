        ;FILE "bootpack.c"
MISMATCH: "        .section        .text.unlikely,"ax",@progbits"
.LCOLDB0: 
        SECTION .text
.LHOTB0: 
        GLOBAL  HariMain
        GLOBAL HariMain:function
HariMain: 
.LFB0: 
MISMATCH: "        .cfi_startproc"
L2:
L3:
        jmp L3
MISMATCH: "        .cfi_endproc"
.LFE0: 
        GLOBAL  HariMain:function (.-HariMain)
MISMATCH: "        .section        .text.unlikely"
.LCOLDE0: 
        SECTION .text
.LHOTE0: 
        ;IDENT "GCC: (Debian 4.9.2-10) 4.9.2"
MISMATCH: "        .section        .note.GNU-stack,"",@progbits"

