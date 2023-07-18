#include <cpp11.hpp>
#include <omp.h>

using namespace cpp11;

[[cpp11::register]] list squared_unnamed_(doubles x) {
    // create vectors y = x^2 and z = thread number
    int n = x.size();
    writable::doubles y(n);
    writable::doubles z(n);
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        y[i] = x[i] * x[i];
        z[i] = omp_get_thread_num();
    }

    //create a list containing y and z
    writable::list out;
    out.push_back(y);
    out.push_back(z);
    return out;
}

[[cpp11::register]] list squared_named_(doubles x) {
    // create vectors y = x^2 and z = thread number
    int n = x.size();
    writable::doubles y(n);
    writable::doubles z(n);
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        y[i] = x[i] * x[i];
        z[i] = omp_get_thread_num();
    }

    //create a list containing y and z
    writable::list out;
    out.push_back({"x^2"_nm = y});
    out.push_back({"thread"_nm = z});
    return out;
}
