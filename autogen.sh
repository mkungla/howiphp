#!/bin/sh
# jsoncpp
# 
# Update jsoncpp dist
# Probably could make bash script to update correct jsoncpp tag 
# and make sure that it is checked out!
printf "Searching jsoncpp\n"
cd vendor/jsoncpp
python amalgamate.py
# Could test here that jsoncpp dist files exist
printf "Updating jsoncpp DONE\n"

# configure
# Rebuild configure
printf "Rebuild configure!\n"
cd ../../
autoreconf -vif -I config
printf "Rebuild configure DONE!\n"
