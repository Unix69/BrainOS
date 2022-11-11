#!/bin/bash

commit_message=""
email="giuseppe.pedone.developer@gmail.com"
user="Unix69"

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

if [ -z "$2" ]
then
	while  [ -z "${user}" ]
    	do
        	echo "[$0] Set user"
        	echo -n "[$0] user="
        	read -t 60 user
        	if [ -z "${user}" ]
        	then
            		echo "[$0] Invalid user"
        	fi
    	done
else
	echo "[$0] User = $2"
	commit_message="$2"
fi

if [ -z "$3" ]
then
	while  [ -z "${email}" ]
    	do
        	echo "[$0] Set email"
        	echo -n "[$0] email="
        	read -t 60 email
        	if [ -z "${email}" ]
        	then
            		echo "[$3] Invalid email"
        	fi
    	done
else
	echo "[$0] User = $3"
	email="$2"
fi

sudo git config user.email $email
sudo git config user.name $user
sudo git add *.c *.h *.S *.sh

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
