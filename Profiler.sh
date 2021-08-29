#!/bin/bash
# check if stdout is a terminal...
if test -t 1; then

    # see if it supports colors...
    ncolors=$(tput colors)

    if test -n "$ncolors" && test $ncolors -ge 8; then
        bold="$(tput bold)"
        underline="$(tput smul)"
        standout="$(tput smso)"
        normal="$(tput sgr0)"
        black="$(tput setaf 0)"
        red="$(tput setaf 1)"
        green="$(tput setaf 2)"
        yellow="$(tput setaf 3)"
        blue="$(tput setaf 4)"
        magenta="$(tput setaf 5)"
        cyan="$(tput setaf 6)"
        white="$(tput setaf 7)"
    fi
fi

if [ "$#" -lt 1 ] 
then 
    echo "${red}Number of iterations not specified. Using a default value of 5...${normal}"
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
    echo "${green}${bold}Finished generating $numPolygons polygons using every algorithm. [$(($i * 100 / $iterations))% complete]${normal}"
    echo "----------------------------------------------------------------------------------------------"
done
echo
echo "${yellow}${bold}Profiling complete. Plotting the graphs...${normal}"
python3 Metrics.py
