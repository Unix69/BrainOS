#!/bin/bash

commit_message=""
if [ -z "$1" ]
then
	while  [ -z "${commit_message}" ]
    	do
        	echo "[$0] Set commit message"
        	echo -n "[$0] message="
        	read -t 60 commit_message
        	if [ -z "${commit_message}" ]
        	then
            		echo "[$0] Invalid commit message"
            		
        	fi
    	done
else
	echo "[$0] Commit message = $1"
	commit_message="$1"
fi

echo "[$0] Commit..."
sudo git commit -m "$commit_message"
wait
if [ $? -ne 0 ]
then
    echo "[$0] Commit operation fail"
    exit
fi


echo "[$0] Push..."
sudo git push -u origin master:master 
