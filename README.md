# Polygon Generator
## Maps
Sample outputs:
[]

## Distribution Plots

## Metrics Analysis
<img src = "/Images/metrics.png">

## Dependencies
- [popt](http://ftp.rpm.org/mirror/popt/)
- OpenGL
## Compiling and running
Compile using the make utility:

```bash
$ make polygonGenerator -j$(($(nproc)+1))
```

Run the following command to print the help doc for the program:
```bash
$ ./bin/polygonGenerator -?
```

Run the Profiler using:
```bash
$ ./Profiler.sh 5
```

Note: The argument "5" can be replaced with the desired number of iterations