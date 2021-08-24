#!/bin/bash
if [ "$#" -lt 1 ] 
then 
    echo "Number of iterations not specified. Using a default value of 5..."
    echo 
    iterations=5;    
else 
    iterations=$1
fi

echo "Profiling started..."
echo "num_polygons,time,size,algorithm" > metrics.csv
for i in $(seq 1 $iterations); do 
    numPolygons=$((100*$i))
    echo "Iteration $i (Generating $numPolygons polygons using each algorithm)"
    echo -e "\tExecuting Polar algorithm..."
    ./build/bin/polygonGenerator -a polar -n $numPolygons -p 1 >> metrics.csv
    echo -e "\tExecuting Space Partition algorithm..."
    ./build/bin/polygonGenerator -a spacePartition -n $numPolygons -p 1 >> metrics.csv
    echo -e "\tExecuting Naive Polygon algorithm..."
    ./build/bin/polygonGenerator -a naivePoly -n $numPolygons -p 1 >> metrics.csv
    echo "Finished generating $numPolygons polygons using every algorithm. [$(($i * 100 / $iterations))% complete]"
    echo "----------------------------------------------------------------------------------------------"
done
echo "Profiling complete. Plotting the graphs..."
python3 Metrics.py
