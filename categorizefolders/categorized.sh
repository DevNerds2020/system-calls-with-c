#!/bin/bash
# shellcheck disable=SC2164
cd categorized
catPath=$(pwd)
recursiveSearch(){
	# shellcheck disable=SC2219
	local var=$(ls)
	# shellcheck disable=SC2219
	local filedir
	for filedir in $var;
	do
		if [[ -f "$filedir" ]]
		then
		    if [[ -d "$catPath/${filedir##*.}Files" ]]
		    then
              #if the file name exists then change the name of the file to file + count
                local count=$(ls "$catPath/${filedir##*.}Files" | wc -l)
                mv "$filedir" "$catPath/${filedir##*.}Files/$filedir$count"

		    else
		      mkdir "$catPath/${filedir##*.}Files"
		      mv "$filedir" "$catPath/${filedir##*.}Files"
        fi
		else
        if [[ -d "$filedir" ]]
        then
           cd "$filedir"
           recursiveSearch
           # shellcheck disable=SC2103
           cd ..
           rm -d "$filedir"
        fi
		fi
	done
}
recursiveSearch



