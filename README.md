# Polygon Generator
## Sample Maps

## Samples of Distribution Plots

## Sample of Metrics Analysis
<img src = "/Images/metrics.png">

## Dependencies
- [popt](http://ftp.rpm.org/mirror/popt/)
- OpenGL
## Compiling and running
Compile using the make utility:

```bash
$ make polygonGenerator -j$(($(nproc)+1))
```

Run the program using:
```bash
$ ./bin/polygonGenerator -?
```

Run the Profiler using (The argument "5" can be replaced with the desired number of iterations):
```bash
$ ./Profiler.sh 5
```