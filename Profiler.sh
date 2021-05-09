iterations=$2
echo "Profiling $1 algorithm by running $2 iterations"
for i in $(seq 1 $iterations); do 
    numPolygons=$((100*$i))
    ./bin/polygonGenerator -a $1 -n $numPolygons -p 1
done