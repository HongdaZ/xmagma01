#include <R.h>
#include <Rinternals.h>
extern "C" SEXP myRunif2( SEXP n, SEXP min, SEXP max );
SEXP myRunif2( SEXP n, SEXP min, SEXP max ){

    SEXP statsPackage;
    PROTECT(
        statsPackage = eval( lang2( install("getNamespace"),
                                    ScalarString(mkChar("stats")) ),
                                    R_GlobalEnv
        )
    );

    SEXP RCallBack, S;
    S = RCallBack = PROTECT( allocVector(LANGSXP, 4 ));
//    SETCAR( RCallBack,
//            findFun( install("runif"), statsPackage )
//    );

    SETCAR( RCallBack,
            install("runif")
    );

    RCallBack = CDR( RCallBack );
    SETCAR( RCallBack, n );
    SET_TAG( RCallBack, install("n") );

    RCallBack = CDR( RCallBack );
    SETCAR( RCallBack, min );
    SET_TAG( RCallBack, install("max") );

    RCallBack = CDR( RCallBack );
    SETCAR( RCallBack, max );
    SET_TAG( RCallBack, install("min"));

    SEXP randoms;
    PROTECT(
        randoms = eval( S, statsPackage )
    );

    UNPROTECT(3);
    return( randoms );

}
