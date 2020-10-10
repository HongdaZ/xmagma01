#include <R.h>
#include <Rinternals.h>
#include <xmagma.h>
#include <time.h>
extern "C" {
    SEXP matmult( SEXP A, SEXP B ) {
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

        double gpu_time = magma_sync_wtime( NULL );
        copy( LHS, xa );
        copy( RHS, xb );
        gpu_time = magma_sync_wtime( NULL ) - gpu_time;
        printf( "COPY TIME:%f\n", gpu_time );

        gpu_time = magma_sync_wtime( NULL );
        xc = xa * xb;
        gpu_time = magma_sync_wtime( NULL ) - gpu_time;
        printf( "GPU TIME:%f\n", gpu_time );

        gpu_time = magma_sync_wtime(NULL );
        copy( xc, ANS );
        gpu_time = magma_sync_wtime(NULL ) - gpu_time;
        printf( "COPY TIME:%f\n", gpu_time );


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
    // SEXP blastmatmult( SEXP A, SEXP B ) {
    //     int nra, nca, nrb, ncb, m, n, k;
    //     SEXP Adim, Bdim;
    //     Adim = getAttrib( A, R_DimSymbol );
    //     Bdim = getAttrib( B, R_DimSymbol );
    //     m = nra = INTEGER( Adim )[ 0 ];
    //     k = nca = INTEGER( Adim )[ 1 ];
    //     nrb = INTEGER( Bdim )[ 0 ];
    //     n = ncb = INTEGER( Bdim )[ 1 ];
    //     const size_t a_ld = m;
    //     const size_t b_ld = n;
    //     const size_t c_ld = n;
    //
    //     SEXP ans;
    //     PROTECT( ans = allocMatrix( REALSXP, nra, ncb ) );
    //
    //     double *L = REAL( A ), *R = REAL( B ), *rans = REAL( ans );
    //
    //     xmagma::RMatrix< double > LHS1( L, nra, nca );
    //     xmagma::RMatrix< double > RHS1( R, nrb, ncb );
    //     xmagma::RMatrix< double > ANS1( rans, nra, ncb );
    //
    //     xmagma::RMatrix< float > LHS( nra, nca );
    //     xmagma::RMatrix< float > RHS( nrb, ncb );
    //     xmagma::RMatrix< float > ANS( nra, ncb );
    //
    //     host_copy( LHS1, LHS );
    //     host_copy( RHS1, RHS );
    //     host_copy( ANS1, ANS );
    //
    //     // Initializes the OpenCL platform
    //     cl_uint num_platforms;
    //     clGetPlatformIDs(0, NULL, &num_platforms);
    //     cl_platform_id* platforms = (cl_platform_id*)malloc(num_platforms*sizeof(cl_platform_id));
    //     clGetPlatformIDs(num_platforms, platforms, NULL);
    //     cl_platform_id platform = platforms[ 0 ];
    //
    //     // Initializes the OpenCL device
    //     cl_uint num_devices;
    //     clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
    //     cl_device_id* devices = (cl_device_id*)malloc(num_devices*sizeof(cl_device_id));
    //     clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
    //     cl_device_id device = devices[ 0 ];
    //
    //     // Creates the OpenCL context, queue, and an event
    //     cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    //     cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);
    //     cl_event event = NULL;
    //
    //     // Copy the matrices to the device
    //     cl_mem device_a = clCreateBuffer(context, CL_MEM_READ_WRITE, m*k*sizeof(float), NULL, NULL);
    //     cl_mem device_b = clCreateBuffer(context, CL_MEM_READ_WRITE, n*k*sizeof(float), NULL, NULL);
    //     cl_mem device_c = clCreateBuffer(context, CL_MEM_READ_WRITE, m*n*sizeof(float), NULL, NULL);
    //     clEnqueueWriteBuffer(queue, device_a, CL_TRUE, 0, m*k*sizeof(float), LHS.begin(), 0, NULL, NULL);
    //     clEnqueueWriteBuffer(queue, device_b, CL_TRUE, 0, n*k*sizeof(float), RHS.begin(), 0, NULL, NULL);
    //
    //     // Call the SGEMM routine.
    //     CLBlastSgemm(CLBlastLayoutColMajor,
    //                  CLBlastTransposeNo, CLBlastTransposeNo,
    //                  m, n, k,
    //                  1.0f,
    //                  device_a, 0, a_ld,
    //                  device_b, 0, b_ld,
    //                  0.0f,
    //                  device_c, 0, c_ld,
    //                  &queue, &event);
    //     clEnqueueReadBuffer(queue, device_c, CL_TRUE, 0, sizeof(float)* m * n,
    //                         ANS.begin(), 0, NULL, NULL);
    //     host_copy( ANS, ANS1 );
    //     // Clean-up
    //     free(platforms);
    //     free(devices);
    //     clReleaseMemObject(device_a);
    //     clReleaseMemObject(device_b);
    //     clReleaseMemObject(device_c);
    //     clReleaseCommandQueue(queue);
    //     clReleaseContext(context);
    //     UNPROTECT( 1 );
    //     return( ans );
    //
    // }
} // extern "C"
