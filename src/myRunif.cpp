#include <R.h>
#include <Rinternals.h>
extern "C" SEXP myRunif( SEXP n, SEXP min, SEXP max );
SEXP myRunif( SEXP n, SEXP min, SEXP max ){

    SEXP statsPackage;
    PROTECT(
        statsPackage = eval( lang2( install("getNamespace"),
                                    ScalarString(mkChar("stats")) ),
                                    R_GlobalEnv )
    );

    SEXP randoms;
    PROTECT(
        randoms = eval( lang4( install("runif"),
                               n,
                               min,
                               max),
                               statsPackage
        ));

    UNPROTECT(2);


    return( randoms );

}
