#!/bin/bash
# This script will create the src/assets folder, and run the make_img.py script for each image
# in the images folder. It will then copy the images to the src/assets folder.

# Create the assets folder
mkdir -p src/assets

#iota
$i = 0;
# Run the make_img.py script for each image in the images folder
for file in images/*; do
    if [[ $file == *.png ]]; then
        python3 make_img.py $file 
    fi
done