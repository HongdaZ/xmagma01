matmult <- function( A, B ) {
    if( storage.mode( A ) != "double" ) {
        storage.mode( A ) <- "double"
    }
    if( storage.mode( B ) != "double" ) {
        storage.mode( B ) <- "double"
    }
    .Call( "matmult", A, B )
}
mm <- function( A, B ) {
    A %*% B
}
matmult2 <-function( A, B ) {
    if( storage.mode( A ) != "double" ) {
        storage.mode( A ) <- "double"
    }
    if( storage.mode( B ) != "double" ) {
        storage.mode( B ) <- "double"
    }
    .Call( "mm", A, B )
}
# blastmatmult <- function( A, B ) {
#     storage.mode( A ) <- storage.mode( B ) <- "double"
#     .Call( "blastmatmult", A, B )
# }
