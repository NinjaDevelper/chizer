/*
 * Copyright (c) 2015, Shinya Yagyu
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file chizer_python.c
 * @brief glue code for c to python by Python extention module. 
 * @author utamaro
 * @date 2015.2.20
 */

#include "chizer.h"
#include <Python.h>
 
#if 0
static PyObject *chizer_get_histogram(PyObject *self, PyObject *args){
    int i=0;
    unsigned char *buffer=NULL;
    int bytes;
    PyObject *hist_;
    long hist[256];
    for(i=0;i<256;i++) hist[i]=0;

    if (!PyArg_ParseTuple(args, "s#O", &buffer,&bytes,&hist_)){
        return NULL;
    }
    for(i=0;i<PyList_Size(hist_);i++){
        hist[i]=PyLong_AsLong(PyList_GetItem(hist_,i));
    }
    getHistogram(buffer, hist,bytes);
    for(i=0;i<256;i++){
      PyList_SetItem(hist_,i,PyLong_FromLong(hist[i]));
    }
    Py_RETURN_NONE;
}

static PyObject *chizer_calc_chi(PyObject *self, PyObject *args){
    int i=0;
    int bytes;
    PyObject *hist_;
    long hist[256];
    for(i=0;i<256;i++) hist[i]=0;

    if (!PyArg_ParseTuple(args, "Oi",&hist_,&bytes)){
        return NULL;
    }
    for(i=0;i<PyList_Size(hist_);i++){
        hist[i]=PyLong_AsLong(PyList_GetItem(hist_,i));
    }
    double chi=calcChi(hist,bytes);
    return Py_BuildValue("d",chi);
}

static PyObject *chizer_get_file_chi(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    double chi=getFileChi(fname);
    return Py_BuildValue("d", chi);
}
#endif

/**
 * @fn
 * @breaf glue code for isFileEncrypted function, which calculates chi 
 * square of whole file and determin file is encrypted or not.
 * @param PyObject sel
 * @param args arguments
 * @return Python Object that means true or flase
 */
static PyObject *chizer_is_file_encrypted(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    int isEncrypted=isFileEncrypted(fname);
    return Py_BuildValue("b", isEncrypted);
}

/*
*/
/**
 * @fn
 * @breaf glue code for isChunksEncrypted function, which calculates chi
 *  square of 32bytes chunks of a file and  determin file is encrypted or not.
 * @param PyObject sel
 * @param args arguments
 * @return Python Object that means true or flase
 */
static PyObject *chizer_are_chunks_encrypted(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    int areEncrypted=areChunksEncrypted(fname);
    return Py_BuildValue("b", areEncrypted);
}

static PyMethodDef chizerMethods[] = {
    {"is_file_encrypted", chizer_is_file_encrypted, METH_VARARGS, 
        "Judge file is Encrypted or not"},
    {"are_chunks_encrypted", chizer_are_chunks_encrypted, METH_VARARGS, 
        "Judge chunks are Encrypted or not"},
#if 0
    {"get_file_chi", chizer_get_file_chi, METH_VARARGS, 
        "get chi square distribution of file"},
    {"get_histogram", chizer_get_histogram, METH_VARARGS, "get histogram"},
    {"calc_chi", chizer_calc_chi, METH_VARARGS, "calculate chi"},
#endif
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef chizerModule = {
   PyModuleDef_HEAD_INIT,
   "chizer",   /* name of module */
   "judge file is Encrypted or not", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   chizerMethods
};

PyMODINIT_FUNC PyInit_chizer(void){
     return PyModule_Create(&chizerModule);
}

#else

void initchizer(void) {
    Py_InitModule("chizer", chizerMethods);
}

#endif
