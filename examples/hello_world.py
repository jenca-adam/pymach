import ctypes
import pymach
"""
your classic example.
since there is no way to store data on a reliable address when using raw machine code like this, we have to replace the address in the machine code
"""
code = (
    b"\x48\xC7\xC0\x01\x00\x00\x00"  # mov rax, 1         (sys_write)
    b"\x48\xC7\xC7\x01\x00\x00\x00"  # mov rdi, 1         (stdout)
    b"\x48\xBE\x00\x00\x00\x00\x00\x00\x00\x00"  # movabs rsi, <placeholder>
    b"\x48\xC7\xC2\x0E\x00\x00\x00"  # mov rdx, 13        (length)
    b"\x0F\x05"                      # syscall (write)
    b"\xc3"                          # ret
    b"Hello, World!\n"               # String stored in memory
)
addr_offset=code.index(bytes(8)) #find placeholder address
hello_offset=code.index(b'Hello, World!\n') #find hello world address
hello_world = pymach.Code(code, [], None) # create function
hello_addr = hello_world.mmap.ptr+hello_offset # get address
hello_addr_bytes = hello_addr.to_bytes(8, "little") # create bytes
hello_world.patch(hello_addr_bytes, addr_offset)
hello_world()

