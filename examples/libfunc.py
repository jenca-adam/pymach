import ctypes
import pymach
import readelf
"""
demonstrates how you can call library functions using their raw machine code.
a vast majority of them won't work without a large amount of patching.
"""
libm = readelf.readelf("/usr/lib/libm.so.6")
symtab = libm.find_section(".dynsym")
fmax_code = symtab.get_symbol('fmax').value
fmax = pymach.Code(fmax_code, [ctypes.c_double, ctypes.c_double], ctypes.c_double)
print(fmax(0.4, 0.6))
print(fmax(-80392,282))

