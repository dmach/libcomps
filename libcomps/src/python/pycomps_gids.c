/* libcomps - C alternative to yum.comps library
 * Copyright (C) 2013 Jindrich Luza
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to  Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA
 */

#include "pycomps_gids.h"

void PyCOMPSGID_dealloc(PyCOMPS_GID *self)
{
    COMPS_OBJECT_DESTROY(self->gid);
    Py_TYPE(self)->tp_free((PyObject*)self);
    self = NULL;
}

PyObject* PyCOMPSGID_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCOMPS_GID *self;

   (void) args;
   (void) kwds;

    self = (PyCOMPS_GID*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->gid = (COMPS_DocGroupId*)comps_object_create(
                                            &COMPS_DocGroupId_ObjInfo, NULL);
    }
    return (PyObject*) self;
}

char pycomps_p2c_bool_convert(PyObject* pobj, void *cobj) {
    if (!PyBool_Check(pobj)) {
        cobj = NULL;
        return 0;
    } else {
        if (pobj == Py_True) {
            *(char*)cobj = 1;
        } else {
            *(char*)cobj = 0;
        }
        return 1;
    }
}

PyObject* pycomps_c2p_bool_convert(void *cobj) {
    if (*(char*)cobj)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

int PyCOMPSGID_init(PyCOMPS_GID *self, PyObject *args, PyObject *kwds)
{
    char *name = NULL;
    char def = 0;

    char* keywords[] = {"name", "default", NULL};
    if (args) {
        if (PyArg_ParseTupleAndKeywords(args, kwds,
                                        "s|O&", keywords, &name,
                                        &pycomps_p2c_bool_convert, &def
                                           )) {
            comps_docgroupid_set_name(((PyCOMPS_GID*)self)->gid, name, 1);
            comps_docgroupid_set_default(((PyCOMPS_GID*)self)->gid, def);
            return 0;
        } else {
            return -1;
        }
    } else {
        self->gid->name = NULL;
        self->gid->def = 0;
        return 0;
    }
    return -1;
}

PyObject* PyCOMPSGID_get_default(PyCOMPS_GID *self, void *closure) {
    (void) closure;
    char def = self->gid->def;
        return Py_BuildValue("O&", &pycomps_c2p_bool_convert, &def);
    Py_RETURN_NONE;
}

int PyCOMPSGID_set_default(PyCOMPS_GID *self, PyObject *value, void *closure){
   (void) closure;
    if (!PyBool_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Not a Bool");
        return -1;
    }
    if (value == Py_True)
        self->gid->def = 1;
    else
        self->gid->def = 0;
    return 0;
}

__COMPS_STRPROP_GETSET_CLOSURE(COMPS_DocGroupId) DocGroupId_NameClosure = {
    .get_f = &comps_docgroupid_get_name,
    .set_f = &comps_docgroupid_set_name,
    .c_offset = offsetof(PyCOMPS_GID, gid)
};

PyGetSetDef gid_getset[] = {
    {"name",
     (getter)__PyCOMPS_get_strattr, (setter)__PyCOMPS_set_strattr,
     "Group ID name",
     (void*)&DocGroupId_NameClosure},
    {"default",
     (getter)PyCOMPSGID_get_default, (setter)PyCOMPSGID_set_default,
     "Group ID default",
     NULL},
    {NULL}  /* Sentinel */
};

PyObject* PyCOMPSGID_str(PyObject *self) {
    char *x;
    PyObject *ret;
    x = comps_object_tostr((COMPS_Object*)((PyCOMPS_GID*)self)->gid);
    ret = PyUnicode_FromString(x);
    free(x);
    return ret;
}

int PyCOMPSGID_print(PyObject *self, FILE *f, int flags) {
    const char *def;
    char *name;

    (void) flags;
    def = (((PyCOMPS_GID*)self)->gid->def)?"true":"false";
    name = comps_object_tostr((COMPS_Object*)((PyCOMPS_GID*)self)->gid->name);
    fprintf(f, "<COPMS_GroupId name='%s' default='%s'>", name, def);
    free(name);
    return 0;
}

COMPS_DocGroupId* comps_gid_from_str(PyObject *other) {
    COMPS_DocGroupId *gid;
    char *name;
    gid = (COMPS_DocGroupId*)comps_object_create(&COMPS_DocGroupId_ObjInfo,
                                                 NULL);
    __pycomps_stringable_to_char(other, &name);
    gid->name = comps_str_x(name);
    if (!gid->name) {
        COMPS_OBJECT_DESTROY(gid);
        return NULL;
    }
    return gid;
}

PyObject* PyCOMPSGID_cmp(PyObject *self, PyObject *other, int op) {
    char ret;
    CMP_OP_EQ_NE_CHECK(op)
    COMPS_DocGroupId *gid2;
    char created = 0;

    if (PyUnicode_Check(other) || PyBytes_Check(other)) {
        gid2 = comps_gid_from_str(other);
        created = 1;
    } else if (other == NULL || ( Py_TYPE(other) != &PyCOMPS_GIDType
                           && other != Py_None)) {
        PyErr_Format(PyExc_TypeError, "Not %s instance",
                        Py_TYPE(self)->tp_name);
        return NULL;
    } else {
        gid2 = ((PyCOMPS_GID*)other)->gid;
    }
    CMP_NONE_CHECK(op, self, other)

    ret = COMPS_OBJECT_CMP((COMPS_Object*)((PyCOMPS_GID*)self)->gid,
                           (COMPS_Object*)gid2);
    if (created) {
        COMPS_OBJECT_DESTROY(gid2);
    }
    if (op == Py_EQ) {
        if (!ret) Py_RETURN_FALSE;
    } else {
        if (ret) Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

PyMemberDef PyCOMPSGID_members[] = {
    {NULL}};

PyMethodDef PyCOMPSGID_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject PyCOMPS_GIDType = {
    PY_OBJ_HEAD_INIT
    "libcomps.GroupId",   /*tp_name*/
    sizeof(PyCOMPS_GID), /*tp_basicsize*/
    0,                        /*tp_itemsize*/
    (destructor)PyCOMPSGID_dealloc, /*tp_dealloc*/
    PyCOMPSGID_print,         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,//PyCOMPSPack_cmp,           /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    PyCOMPS_hash,              /*tp_hash */
    0,                         /*tp_call*/
    PyCOMPSGID_str,           /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Comps GroupId",           /* tp_doc */
    0,                    /* tp_traverse */
    0,                     /* tp_clear */
    PyCOMPSGID_cmp,       /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    PyCOMPSGID_methods,        /* tp_methods */
    PyCOMPSGID_members,        /* tp_members */
    gid_getset,               /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyCOMPSGID_init,      /* tp_init */
    0,                               /* tp_alloc */
    PyCOMPSGID_new,                 /* tp_new */};

COMPS_Object* comps_gids_in(PyObject *item) {
    return comps_object_incref((COMPS_Object*)((PyCOMPS_GID*)item)->gid);
}

COMPS_Object* comps_gids_str_in(PyObject *item) {
    //printf("comps gids str in\n");
    return (COMPS_Object*)comps_gid_from_str(item);
}

PyObject* comps_gids_out(COMPS_Object *cobj) {
    PyCOMPS_GID *ret;
    ret = (PyCOMPS_GID*)PyCOMPSGID_new(&PyCOMPS_GIDType, NULL, NULL);
    PyCOMPSGID_init(ret, NULL, NULL);
    COMPS_OBJECT_DESTROY(ret->gid);
    ret->gid = (COMPS_DocGroupId*)cobj;
    return (PyObject*)ret;
}

PyCOMPS_ItemInfo PyCOMPS_GIDsInfo = {
#if PY_MAJOR_VERSION >= 3
    .itemtypes = (PyTypeObject*[]){&PyCOMPS_GIDType,
                                   &PyUnicode_Type},
    .in_convert_funcs = (PyCOMPS_in_itemconvert[])
                        {&comps_gids_in,
                         &comps_gids_str_in},
    .item_types_len = 2,
#else
    .itemtypes = (PyTypeObject*[]){&PyCOMPS_GIDType, &PyString_Type,
                                   &PyUnicode_Type},
    .in_convert_funcs = (PyCOMPS_in_itemconvert[])
                        {&comps_gids_in,
                         &comps_gids_str_in,
                         &comps_gids_str_in},
    .item_types_len = 3,
#endif
    .out_convert_func = &comps_gids_out
};

int PyCOMPSGIDs_init(PyCOMPS_Sequence *self, PyObject *args, PyObject *kwds)
{
    (void) args;
    (void) kwds;
    self->it_info = &PyCOMPS_GIDsInfo;
    return 0;
}

PyMemberDef PyCOMPSGIDs_members[] = {
    {NULL}};

PyMethodDef PyCOMPSGIDs_methods[] = {
    {NULL}  /* Sentinel */
};

PyNumberMethods PyCOMPSGIDs_Nums = {0};

PyTypeObject PyCOMPS_GIDsType = {
    PY_OBJ_HEAD_INIT
    "libcomps.IdList",   /*tp_name*/
    sizeof(PyCOMPS_Sequence), /*tp_basicsize*/
    sizeof(PyCOMPS_GID),   /*tp_itemsize*/
    (destructor)PyCOMPSSeq_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    PyCOMPSSeq_str,       /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,        /*tp_flags*/
    "Comps GIDs list",           /* tp_doc */
    0,                    /* tp_traverse */
    0,                     /* tp_clear */
    &PyCOMPSSeq_cmp,       /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    PyCOMPSGIDs_methods,        /* tp_methods */
    PyCOMPSGIDs_members,        /* tp_members */
    0,                         /* tp_getset */
    &PyCOMPS_SeqType,           /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyCOMPSGIDs_init,      /* tp_init */
    0,                               /* tp_alloc */
    &PyCOMPSSeq_new,                 /* tp_new */};
