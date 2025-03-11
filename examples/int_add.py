import pymach
import ctypes

"""
Integer addition, no overflow checking
mov rax, rdi
add rax, rsi
ret
"""
add_2_ints = pymach.Code(
    b"\x48\x89\xf8\x48\x01\xf0\xc3", [ctypes.c_int, ctypes.c_int], ctypes.c_int
)
print(add_2_ints(2, 5))
print(add_2_ints(-6, 4))
print(add_2_ints(983098430921, 384320943280943))
