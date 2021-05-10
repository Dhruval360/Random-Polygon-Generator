iterations=$1
echo "Profiling all the algorithms for $1 iterations"
echo "num_polygons,time,size,algorithm" > metrics.csv
for i in $(seq 1 $iterations); do 
    numPolygons=$((100*$i))
    ./bin/polygonGenerator -a polar -n $numPolygons -p 1 >> metrics.csv
    ./bin/polygonGenerator -a spacePartition -n $numPolygons -p 1 >> metrics.csv
done
echo "Profiling complete. Plotting the graphs..."
python3 Metrics.py
