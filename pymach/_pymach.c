#define PY_SSIZE_T_CLEAN
#include<Python.h>
#include <sys/mman.h>
#include <stddef.h>
typedef struct{
	PyObject_HEAD void *ptr;
	int length;
}
	ExeMmap;
static int ExeMmap_init(ExeMmap *self, PyObject *args, PyObject *kwds){
   char *code;
   if(!PyArg_ParseTuple(args, "y#", &code, &self->length)){
   	return -1;
   }
   self->ptr = mmap(NULL, self->length, PROT_WRITE|PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
   if(!self->ptr){
	PyErr_SetFromErrno(PyExc_OSError);
	return -1;
   }
   memcpy(self->ptr, code, self->length);
   return 0;
}
static void ExeMmap_dealloc(ExeMmap *self){
	if(self->ptr){
		munmap(self->ptr, self->length);
	}
	Py_TYPE(self)->tp_free((PyObject *)self);
		
}

static PyObject *ExeMmap_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
	ExeMmap *self = (ExeMmap*)type->tp_alloc(type, 0);
	if(self!=NULL){
		self->ptr = NULL;
		self->length = -1;
	}
	return (PyObject*)self;

}
static PyObject *ExeMmap_patch(PyObject *self, PyObject *args){
	const char *code;
	Py_ssize_t size;
	int offset;
	
	if(!PyArg_ParseTuple(args, "y#i", &code, &size, &offset)){
		return NULL;
	}
	if (size+offset>((ExeMmap*)self)->length||offset<0){
		PyErr_SetString(PyExc_ValueError, "patch out of range");
		return NULL;
	}
	memcpy(((ExeMmap*)self)->ptr+offset, code, size);
	Py_RETURN_NONE;
}
static PyMethodDef ExeMmap_methods[] = {
      {"patch", &ExeMmap_patch, METH_VARARGS, "rewrites mmap"},
      {NULL, NULL, 0, NULL}
};
static struct PyMemberDef ExeMmap_members[] = {
   {"ptr", Py_T_LONG, offsetof(ExeMmap, ptr), 0, "mmap pointer"},
   {"length", Py_T_INT, offsetof(ExeMmap, length), 0, "length of the mmap"},
   {NULL}
};
static PyTypeObject ExeMmapType = {
   PyVarObject_HEAD_INIT(NULL, 0).tp_name="pymach.ExeMmap",
   .tp_doc = "executable mmap",
   .tp_basicsize = sizeof(ExeMmap),
   .tp_itemsize = 0,
   .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
   .tp_new = ExeMmap_new,
   .tp_init = (initproc)ExeMmap_init,
   .tp_dealloc = (destructor)ExeMmap_dealloc,
   .tp_members = ExeMmap_members,
   .tp_methods = ExeMmap_methods

};
static PyMethodDef pymachMethods[] = {
      {NULL, NULL, 0, NULL}
};
static struct PyModuleDef pymachmodule = {
	PyModuleDef_HEAD_INIT,
	"_pymach",
	NULL,
	-1,
	pymachMethods
};
PyMODINIT_FUNC
PyInit__pymach(void)
{	if (PyType_Ready(&ExeMmapType)<0){
		return NULL;
					 }
	PyObject *m = PyModule_Create(&pymachmodule);
	Py_INCREF(&ExeMmapType);
	if (PyModule_AddObject(m, "ExeMmap", (PyObject*)&ExeMmapType)<0){
		Py_DECREF(&ExeMmapType);
		Py_DECREF(m);
		return NULL;
	}
	return m;
}
