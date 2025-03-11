import pymach
import ctypes

"""
Float addition.
addss xmm0, xmm1
ret
"""
add_float = pymach.Code(
    b"\xf3\x0f\x58\xc1\xc3", [ctypes.c_float, ctypes.c_float], ctypes.c_float
)
print(add_float(0.1, 0.4))
print(add_float(-3.2, 5.48))
print(add_float(3.14159, 10))
