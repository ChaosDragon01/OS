; filepath: /c:/Users/safin/Desktop/GithubRepos/OS/samplecodes/test.asm

section .text
    global _start

_start:
    ; Infinite loop to overload the CPU
    jmp _start


    