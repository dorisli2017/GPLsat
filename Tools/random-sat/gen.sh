#!/bin/bash

# variables from 200 to 400, step 20: 200,220,240, ... , 400

seq 400 20 600 | parallel ./gen10.sh $1 {} $2 $3 $4 $5
