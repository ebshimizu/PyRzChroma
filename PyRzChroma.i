%module(directors="1") PyRzChroma
%feature("director");

%include "typemaps.i"
%include "windows.i"
%include "cpointer.i"

%include "stl.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_shared_ptr.i"

%{
#include "PyRzChroma.h"
%}

%include "PyRzChroma.h"