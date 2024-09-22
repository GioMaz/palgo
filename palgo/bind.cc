#include <emscripten/bind.h>

#include "palgo.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
    value_object<Record>("Record")
        .field("g", &Record::g)
        .field("m", &Record::m)
        .field("s", &Record::s);
    register_vector<Record>("RecordVec");
    register_vector<int>("IntVec");
    function("palgo", &palgo);
}
