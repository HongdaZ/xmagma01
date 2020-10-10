matsum <- function( A, B ) {
    if( storage.mode( A ) != "double" ) {
        storage.mode( A ) <- "double"
    }
    if( storage.mode( B ) != "double" ) {
        storage.mode( B ) <- "double"
    }
    .Call( "matsum", A, B )
}
