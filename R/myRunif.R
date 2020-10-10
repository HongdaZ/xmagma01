#myRunif.R
myRunif <-
    function( n, min = 0, max = 1 ){

        unifRandom <- .Call( 'myRunif', n, min, max )

        return( unifRandom )
}
myRunif2 <-
    function( n, min = 0, max = 1 ){

        unifRandom <- .Call( 'myRunif2', n, min, max )

        return( unifRandom )
    }
