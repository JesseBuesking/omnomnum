/*
 * Python C API wrapper for OmNomNum
 *
 * This is a direct C extension using Python.h instead of CFFI.
 * Should have lower overhead at the Python/C boundary.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "../omnomnum.h"

/* OmNomNum Python object structure */
typedef struct {
    PyObject_HEAD
    ParserState *state;
    int precision;
    int parse_second;
    int parse_fractions;
    int reduce_fractions;
    int normalize_percent_symbol;
    int percent_as_decimal;
} OmNomNumObject;

/* Forward declarations */
static PyTypeObject OmNomNumType;

/* Initialize the C library (called once) */
static int omnomnum_initialized = 0;

/* Cached empty string to avoid repeated allocations */
static PyObject *empty_string_cache = NULL;

static void ensure_initialized(void) {
    if (!omnomnum_initialized) {
        initOmNomNum();
        omnomnum_initialized = 1;
    }
}

/* OmNomNum.__new__ */
static PyObject *
OmNomNum_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    OmNomNumObject *self;
    self = (OmNomNumObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->state = NULL;
        self->precision = 6;
        self->parse_second = 0;
        self->parse_fractions = 1;
        self->reduce_fractions = 0;
        self->normalize_percent_symbol = 0;
        self->percent_as_decimal = 0;
    }
    return (PyObject *)self;
}

/* OmNomNum.__init__ */
static int
OmNomNum_init(OmNomNumObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {
        "precision",
        "parse_second",
        "parse_fractions",
        "reduce_fractions",
        "normalize_percent_symbol",
        "percent_as_decimal",
        NULL
    };

    ensure_initialized();

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ippppp", kwlist,
                                     &self->precision,
                                     &self->parse_second,
                                     &self->parse_fractions,
                                     &self->reduce_fractions,
                                     &self->normalize_percent_symbol,
                                     &self->percent_as_decimal)) {
        return -1;
    }

    /* Allocate and initialize ParserState */
    self->state = malloc(sizeof(ParserState));
    if (self->state == NULL) {
        PyErr_NoMemory();
        return -1;
    }

    initParserState(self->state);
    self->state->precision = self->precision;
    self->state->parse_second = self->parse_second;
    self->state->parse_fractions = self->parse_fractions;
    self->state->reduce_fractions = self->reduce_fractions;
    self->state->normalize_percent_symbol = self->normalize_percent_symbol;
    self->state->percent_as_decimal = self->percent_as_decimal;

    return 0;
}

/* OmNomNum.__del__ */
static void
OmNomNum_dealloc(OmNomNumObject *self)
{
    if (self->state) {
        freeParserState(self->state);
        free(self->state);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/* OmNomNum.normalize(text) */
static PyObject *
OmNomNum_normalize(OmNomNumObject *self, PyObject *args, PyObject *kwds)
{
    const char *text;
    Py_ssize_t text_len;
    int reset = 1;
    static char *kwlist[] = {"text", "reset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#|p", kwlist,
                                     &text, &text_len, &reset)) {
        return NULL;
    }

    /* Call C normalize function */
    normalize(text, (size_t)text_len, self->state);

    /* Convert result to Python string */
    PyObject *result;
    if (self->state->result == NULL || sdslen(self->state->result) == 0) {
        /* Use cached empty string to avoid repeated allocations */
        if (empty_string_cache == NULL) {
            empty_string_cache = PyUnicode_FromString("");
        }
        Py_INCREF(empty_string_cache);
        result = empty_string_cache;
    } else {
        result = PyUnicode_FromStringAndSize(self->state->result,
                                            sdslen(self->state->result));
    }

    /* Reset state if requested */
    if (reset) {
        resetParserState(self->state);
    }

    return result;
}

/* OmNomNum.find_numbers(text) */
static PyObject *
OmNomNum_find_numbers(OmNomNumObject *self, PyObject *args, PyObject *kwds)
{
    const char *text;
    Py_ssize_t text_len;
    int reset = 1;
    static char *kwlist[] = {"text", "reset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#|p", kwlist,
                                     &text, &text_len, &reset)) {
        return NULL;
    }

    /* Call C find_numbers function */
    YYSTYPEList list = find_numbers(text, (size_t)text_len, self->state);

    /* Convert to Python list of dicts */
    PyObject *result = PyList_New(list.used);
    if (result == NULL) {
        return NULL;
    }

    for (unsigned int i = 0; i < list.used; i++) {
        YYSTYPE y = list.values[i];
        PyObject *dict = PyDict_New();
        if (dict == NULL) {
            Py_DECREF(result);
            return NULL;
        }

        /* Add fields to dict - properly manage references */
        PyObject *begin = PyLong_FromLong(y.begin);
        PyObject *end = PyLong_FromLong(y.end);
        PyDict_SetItemString(dict, "begin", begin);
        PyDict_SetItemString(dict, "end", end);
        Py_DECREF(begin);
        Py_DECREF(end);

        if (y.is_dbl) {
            PyObject *value = PyFloat_FromDouble(y.dbl);
            PyDict_SetItemString(dict, "value", value);
            Py_DECREF(value);
            Py_INCREF(Py_True);
            PyDict_SetItemString(dict, "is_double", Py_True);
        } else {
            Py_INCREF(Py_False);
            PyDict_SetItemString(dict, "is_double", Py_False);
        }

        if (y.is_frac) {
            PyObject *frac_dict = PyDict_New();
            PyObject *numerator = PyFloat_FromDouble(y.frac_num);
            PyObject *denominator = PyFloat_FromDouble(y.frac_denom);
            PyDict_SetItemString(frac_dict, "numerator", numerator);
            PyDict_SetItemString(frac_dict, "denominator", denominator);
            Py_DECREF(numerator);
            Py_DECREF(denominator);

            PyDict_SetItemString(dict, "fraction", frac_dict);
            Py_DECREF(frac_dict);  /* dict now owns a reference */

            Py_INCREF(Py_True);
            PyDict_SetItemString(dict, "is_fraction", Py_True);
        } else {
            Py_INCREF(Py_False);
            PyDict_SetItemString(dict, "is_fraction", Py_False);
        }

        if (y.suffix > 0) {
            PyObject *suffix = PyLong_FromLong(y.suffix);
            PyDict_SetItemString(dict, "suffix", suffix);
            Py_DECREF(suffix);
        }

        PyList_SET_ITEM(result, i, dict);
    }

    /* Reset state if requested */
    if (reset) {
        resetParserState(self->state);
    }

    return result;
}

/* Method definitions */
static PyMethodDef OmNomNum_methods[] = {
    {"normalize", (PyCFunction)OmNomNum_normalize, METH_VARARGS | METH_KEYWORDS,
     "Normalize numbers in text.\n\n"
     "Args:\n"
     "    text (str): Input text containing numbers\n"
     "    reset (bool): Reset state after normalization (default: True)\n\n"
     "Returns:\n"
     "    str: Text with normalized numbers"
    },
    {"find_numbers", (PyCFunction)OmNomNum_find_numbers, METH_VARARGS | METH_KEYWORDS,
     "Extract all numbers from text.\n\n"
     "Args:\n"
     "    text (str): Input text containing numbers\n"
     "    reset (bool): Reset state after extraction (default: True)\n\n"
     "Returns:\n"
     "    list: List of number dictionaries"
    },
    {NULL}  /* Sentinel */
};

/* Type definition */
static PyTypeObject OmNomNumType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "omnomnum_capi.OmNomNum",
    .tp_doc = "OmNomNum text normalizer (C API version)",
    .tp_basicsize = sizeof(OmNomNumObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = OmNomNum_new,
    .tp_init = (initproc)OmNomNum_init,
    .tp_dealloc = (destructor)OmNomNum_dealloc,
    .tp_methods = OmNomNum_methods,
};

/* Module-level normalize function */
static PyObject *
omnomnum_normalize(PyObject *self, PyObject *args, PyObject *kwds)
{
    const char *text;
    Py_ssize_t text_len;
    int precision = 6;
    int parse_second = 0;
    int parse_fractions = 1;
    int reduce_fractions = 0;
    int normalize_percent_symbol = 0;
    int percent_as_decimal = 0;

    static char *kwlist[] = {
        "text",
        "precision",
        "parse_second",
        "parse_fractions",
        "reduce_fractions",
        "normalize_percent_symbol",
        "percent_as_decimal",
        NULL
    };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#|ippppp", kwlist,
                                     &text, &text_len,
                                     &precision,
                                     &parse_second,
                                     &parse_fractions,
                                     &reduce_fractions,
                                     &normalize_percent_symbol,
                                     &percent_as_decimal)) {
        return NULL;
    }

    ensure_initialized();

    /* Create temporary state */
    ParserState state;
    initParserState(&state);
    state.precision = precision;
    state.parse_second = parse_second;
    state.parse_fractions = parse_fractions;
    state.reduce_fractions = reduce_fractions;
    state.normalize_percent_symbol = normalize_percent_symbol;
    state.percent_as_decimal = percent_as_decimal;

    /* Call C normalize */
    normalize(text, (size_t)text_len, &state);

    /* Convert result */
    PyObject *result;
    if (state.result == NULL || sdslen(state.result) == 0) {
        /* Use cached empty string to avoid repeated allocations */
        if (empty_string_cache == NULL) {
            empty_string_cache = PyUnicode_FromString("");
        }
        Py_INCREF(empty_string_cache);
        result = empty_string_cache;
    } else {
        result = PyUnicode_FromStringAndSize(state.result, sdslen(state.result));
    }

    /* Cleanup */
    freeParserState(&state);

    return result;
}

/* Module methods */
static PyMethodDef module_methods[] = {
    {"normalize", (PyCFunction)omnomnum_normalize, METH_VARARGS | METH_KEYWORDS,
     "Normalize numbers in text (convenience function).\n\n"
     "Args:\n"
     "    text (str): Input text\n"
     "    precision (int): Decimal precision (default: 6)\n"
     "    parse_second (bool): Parse 'second' as ordinal (default: False)\n"
     "    parse_fractions (bool): Enable fraction parsing (default: True)\n"
     "    reduce_fractions (bool): Reduce fractions (default: False)\n"
     "    normalize_percent_symbol (bool): Convert 'percent' to '%' (default: False)\n"
     "    percent_as_decimal (bool): Convert to decimal (default: False)\n\n"
     "Returns:\n"
     "    str: Normalized text"
    },
    {NULL}  /* Sentinel */
};

/* Module definition */
static struct PyModuleDef omnomnum_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "omnomnum_capi",
    .m_doc = "OmNomNum text normalizer (Python C API version)",
    .m_size = -1,
    .m_methods = module_methods,
};

/* Module initialization */
PyMODINIT_FUNC
PyInit_omnomnum_capi(void)
{
    PyObject *m;

    if (PyType_Ready(&OmNomNumType) < 0)
        return NULL;

    m = PyModule_Create(&omnomnum_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&OmNomNumType);
    if (PyModule_AddObject(m, "OmNomNum", (PyObject *)&OmNomNumType) < 0) {
        Py_DECREF(&OmNomNumType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
