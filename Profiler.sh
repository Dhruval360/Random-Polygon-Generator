iterations=$1
#echo "Profiling $1 algorithm by running $2 iterations"
echo "num_polygons,time,size,algorithm"
for i in $(seq 1 $iterations); do 
    numPolygons=$((100*$i))
    ./bin/polygonGenerator -a spacePartition -n $numPolygons -p 1
    ./bin/polygonGenerator -a polar -n $numPolygons -p 1
done