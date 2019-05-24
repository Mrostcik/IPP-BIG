#!/bin/bash

file="$1"
shift

if [ ! -f $file ]; then
    exit 1
fi

if(("${#@}" == 0)); then
    exit 1
fi

for routeId in "$@"; do
    if((routeId < 1 || routeId > 999)); then
        exit 1
    fi
    
    result=$(grep "^$routeId" $file)
    
    if((${#result} != 0)); then
        sum=0
        counter=0
        IFS=';' read -ra lines <<< "$result"
        for x in "${lines[@]}"; do
            let "counter += 1"
            if((counter % 3 == 0)); then
                let "sum += $x"
            fi
        done
        echo "$routeId;$sum"
    fi
    
done

exit 0