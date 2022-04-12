  section  .data
message: db    "Hello, World!", 0Ah, 00h
  global  _main
  section  .text
_main:
  mov    rax, 0x02000004    ; system call for write
  mov    rdi, 1             ; file descriptor 1 is stdout
  mov    rsi, qword message ; get string address
  mov    rdx, 13            ; number of bytes
  syscall                   ; execute syscall (write)
  mov    rax, 0x02000001    ; system call for exit
  mov    rdi, 0             ; exit code 0
  syscall   
