#!/bin/bash  
  
if [ "$#" -ne 1 ]; then  
    echo "Usage: $0 directory"  
    exit 1  
fi  
 
current_date=$(date +%Y-%m-%d)  
current_time=$(date +%H-%M)  
suffix="-$current_date-$current_time"  

for file in "$1"/*; do  
    if [ -d "$file" ]; then  
        continue  
    fi  
    filename=$(basename "$file")  
    if [[ $filename == *"-"$current_date"-"$current_time ]]; then  
        new_filename="${filename%-*}-${current_date}-${current_time}"  
        mv "$file" "${new_filename}${filename##*.}"  
    else   
        new_filename="$filename$suffix"  
        mv "$file" "$new_filename"  
    fi  
done
