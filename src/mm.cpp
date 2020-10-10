#include <R.h>
#include <Rinternals.h>
extern "C" SEXP mm( SEXP A, SEXP B );
SEXP mm( SEXP A, SEXP B ){

    SEXP current;
    PROTECT(
        current = eval( lang2( install("getNamespace"),
                                    ScalarString(mkChar("xmagma01")) ),
                                    R_GlobalEnv )
    );

    SEXP AB;
    PROTECT(
        AB = eval( lang3( install("mm"),
                               A,
                               B ),
                               current
        ));

    UNPROTECT(2);

    //Rprintf( "hello \n" );

    return( AB );

}
