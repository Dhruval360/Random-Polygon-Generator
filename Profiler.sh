iterations=$1
echo "Profiling started..."
echo "num_polygons,time,size,algorithm" > metrics.csv
for i in $(seq 1 $iterations); do 
    numPolygons=$((100*$i))
    ./bin/polygonGenerator -a polar -n $numPolygons -p 1 >> metrics.csv
    ./bin/polygonGenerator -a spacePartition -n $numPolygons -p 1 >> metrics.csv
    ./bin/polygonGenerator -a naivePoly -n $numPolygons -p 1 >> metrics.csv
    echo "$i) Generated $numPolygons polygons using every algorithm. [$(($i * 100 / $iterations))% complete]"
done
echo "Profiling complete. Plotting the graphs..."
python3 Metrics.py