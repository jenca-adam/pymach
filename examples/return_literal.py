import pymach
import ctypes

"""
Machine code that returns 42.
Assembly:
mov eax, 42
ret
"""
return_42 = pymach.Code(b"\xb8\x2a\x00\x00\x00\xc3", [], ctypes.c_int)
print(return_42())
