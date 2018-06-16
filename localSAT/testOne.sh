
timeout 2m /usr/bin/time -f "wtime=%e" ./local $1 &> $1.testres
timeout 2m /usr/bin/time -f "wtime=%e" ./notest $1 &> $1.res


