#include <R.h>
#include <Rinternals.h>
#include <xmagma.h>
extern "C" {
    SEXP matsum( SEXP A, SEXP B ) {
        int nra, nca, nrb, ncb;
        SEXP Adim, Bdim;
        Adim = getAttrib( A, R_DimSymbol );
        Bdim = getAttrib( B, R_DimSymbol );
        nra = INTEGER( Adim )[ 0 ];
        nca = INTEGER( Adim )[ 1 ];
        nrb = INTEGER( Bdim )[ 0 ];
        ncb = INTEGER( Bdim )[ 1 ];

        SEXP ans;
        PROTECT( ans = allocMatrix( REALSXP, nra, ncb ) );

        double *L = REAL( A ), *R = REAL( B ), *rans = REAL( ans );

        xmagma::RMatrix< double > LHS1( L, nra, nca );
        xmagma::RMatrix< double > RHS1( R, nrb, ncb );
        xmagma::RMatrix< double > ANS1( rans, nra, ncb );

        xmagma::RMatrix< float > LHS( nra, nca );
        xmagma::RMatrix< float > RHS( nrb, ncb );
        xmagma::RMatrix< float > ANS( nra, ncb );

        host_copy( LHS1, LHS );
        host_copy( RHS1, RHS );
        host_copy( ANS1, ANS );

        xmagma::Matrix< float > xa( nra, nca );
        xmagma::Matrix< float > xb( nrb, ncb );
        xmagma::Matrix< float > xc( nra, ncb );

        // double gpu_time = magma_sync_wtime( NULL );
        copy( LHS, xa );
        copy( RHS, xb );
        // gpu_time = magma_sync_wtime( NULL ) - gpu_time;
        // printf( "COPY TIME:%f\n", gpu_time );

        // gpu_time = magma_sync_wtime( NULL );
        xc = xa + xb;
        // gpu_time = magma_sync_wtime( NULL ) - gpu_time;
        // printf( "GPU TIME:%f\n", gpu_time );

        // gpu_time = magma_sync_wtime(NULL );
        copy( xc, ANS );
        // gpu_time = magma_sync_wtime(NULL ) - gpu_time;
        // printf( "COPY TIME:%f\n", gpu_time );


        // gpu_time = magma_wtime();
        // magmablas_dgemm(MagmaNoTrans, MagmaNoTrans, nra, nca, ncb, 1.0,
        //                 xa.get_pointer(), xa.ld(), xb.get_pointer(), xb.ld(),
        //                 0.0, xc.get_pointer(),
        //                 xc.ld(), xmagma::Backend::get_queue());
        // magmablas_dgemm(MagmaNoTrans, MagmaNoTrans, nra, nca, ncb, 1.0,
        //                 xb.get_pointer(), xb.ld(), xb.get_pointer(), xb.ld(),
        //                 0.0, xc.get_pointer(),
        //                 xa.ld(), xmagma::Backend::get_queue());


        host_copy( ANS, ANS1 );
        UNPROTECT( 1 );
        // gpu_time = magma_wtime() - gpu_time;
        // printf( "magma alone:%f\n", gpu_time );

        return( ans );

    }
} // extern "C"
