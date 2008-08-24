/* Python LRU library
 *
 * Copyright Evan Klitzke, 2008
 */

#include <Python.h>
#include <glib.h>

typedef struct
{
	PyObject_HEAD
	GQueue *queue;
} LRUObject;

static int
lru_init(LRUObject *self)
{
	Py_INCREF(Py_None);
	PyObject *head = Py_None;
	Py_INCREF(Py_None);
	PyObject *tail = Py_None;

#if 0
	static char *kwlist[] = {"head", "tail", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO", kwlist, &head, &tail))
		return -1;
#endif

	/* TODO: check return */
	PyObject_SetAttrString((PyObject *) self, "head", head);
	PyObject_SetAttrString((PyObject *) self, "tail", tail);

	self->queue = g_queue_new();

	return 0;
}

static PyObject*
lru_push(LRUObject *self, PyObject *obj)
{
	Py_INCREF(obj);

	g_queue_push_tail(self->queue, (gpointer) obj);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject*
lru_pop(LRUObject *self)
{
	/* TODO: assert queue isn't empty */
	PyObject *obj = (PyObject *) g_queue_pop_head(self->queue);

	/* No need to decref here! */
	return obj;
}

static void
lru_dealloc(LRUObject* self)
{
	g_queue_free(self->queue);
}

static PyMethodDef lru_methods[] = {
	{ "push", (PyCFunction) lru_push, METH_O, "push(obj)" },
	{ "pop", (PyCFunction) lru_pop, METH_NOARGS, "pop()" },
	{ NULL }
};

static PyTypeObject lru_LRUType = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,								/* ob_size */
	"lru.LRUCache",					/* tp_name */
	sizeof(LRUObject),				/* tp_basicsize */
	0,								/* tp_itemsize */
	(destructor)lru_dealloc,		/* tp_dealloc */
	0,								/* tp_print */
	0,								/* tp_getattr */
	0,								/* tp_setattr */
	0,								/* tp_compare */
	0,								/* tp_repr */
	0,								/* tp_as_number */
	0,								/* tp_as_sequence */
	0,								/* tp_as_mapping */
	0,								/* tp_hash */
	0,								/* tp_call */
	0,								/* tp_str */
	PyObject_GenericGetAttr,		/* tp_getattro */
	PyObject_GenericSetAttr,		/* tp_setattro */
	0,								/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,				/* tp_flags*/ /* FIXME: cannot subtype */
"Simple LRU cache using GQueue.",	/* tp_doc */
	0,								/* tp_traverse */
	0,								/* tp_clear */
	0,								/* tp_richcompare */
	0,								/* tp_weaklistoffset */
	0,								/* tp_iter */
	0,								/* tp_iternext */
	lru_methods,					/* tp_methods */
	0,								/* tp_members */
	0,								/* tp_getset */
	0,								/* tp_base */
	0,								/* tp_dict */
	0,								/* tp_descr_get */
	0,								/* tp_descr_set */
	0,								/* tp_dictoffset */
	(initproc)lru_init,				/* tp_init */
	0,								/* tp_alloc */
	PyType_GenericNew,				/* tp_new */
};

static PyMethodDef lru_module_methods[] = {
	{NULL}	/* Sentinel */
};

PyMODINIT_FUNC initlru(void)
{
	PyObject* m;

	if (PyType_Ready(&lru_LRUType) < 0)
		return;

	m = Py_InitModule3("lru", lru_module_methods, "Easy to use LRU library.");

	Py_INCREF(&lru_LRUType);
	PyModule_AddObject(m, "LRUCache", (PyObject *) &lru_LRUType);
}
