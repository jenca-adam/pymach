from ._pymach import ExeMmap
import ctypes

class Code:
    def __init__(self, code, argtypes, restype):
        self.code=code
        self.mmap = ExeMmap(code)
        self.funcptr=ctypes.cast(self.mmap.ptr, ctypes.CFUNCTYPE(restype, *argtypes))
    def __call__(self, *args):
        return self.funcptr(*args)
    def patch(self, code, offset):
        self.mmap.patch(code, offset);
        
