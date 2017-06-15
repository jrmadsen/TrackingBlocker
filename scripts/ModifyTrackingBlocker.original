#!/bin/bash


if [ -z "$1" ]; then
    echo -e '\n\tPlease enter [install] [on] [off] [stat] [original] [safe] [ultrasafe] [add] [remove] [reremove]'
    echo -e '\tINSTALL : copies some files and puts this file in your PATH so you can access it from anyhere in terminal'
    echo -e '\tON : same as ultrasafe (see below)'
    echo -e '\tOFF : same as original (see below)'
    echo -e '\tSTAT : display approximate number of tracking sites blocked'
    echo -e '\tORIGINAL : no blocking of tracking sites'
    echo -e '\tSAFE : uses list of tracking sites from one source'
    echo -e '\tULTRASAFE : uses lists from 3 different source'
    echo -e '\tADD : add website domain to ultrasafe (requires additional parameter - website domain)'
    echo -e '\tREMOVE : remove a website domain (requires additional parameter'
    echo -e '\tREREMOVE : remove previously removed websites (from hosts.removed)\n'
    exit
fi

if [ "$0" == "sudo" ]; then
    echo -e "\n\tDo not run as administrator, you will be prompted for adminstrator password when needed\n"
    exit
#else
#    echo -e "\n\tEnter adminstrator password when requested (when entering password, what you type will not appear in screen)\n"
fi


CMD=$(echo "$1" | awk '{print tolower($1)}')
DNSFLUSH=""

if [ "$CMD" == "add" -a -z "$2" ]; then
    echo -e '\n\tUse of "ADD" requires an additional parameter that specifies the website domain\n'
    exit
fi

#########################################################################
#   Utility functions
#########################################################################
function ISTRUE() { return 0; }
function ISFALSE() { return 1; }

#########################################################################
#  Set the folder paths
#########################################################################
INSTALL_FOLDER=/usr/local/bin
DATA_FOLDER=/usr/local/share/TrackingBlocker
HOST_FOLDER=/etc

#########################################################################
#   Make sure the install directory exists
#########################################################################
if [ ! -d $INSTALL_FOLDER ]; then
    echo "Making $INSTALL_FOLDER for installation..."
    sudo mkdir -p $INSTALL_FOLDER
fi

#########################################################################
#   Make sure the data directory exists
#########################################################################
if [ ! -d $DATA_FOLDER ]; then
    echo "Making $DATA_FOLDER for installation..."
    sudo mkdir -p $DATA_FOLDER
fi

#########################################################################
#   Function for Checking whether to modify file
#########################################################################
function ShouldModify () {
    
    if [ -z "$1" ]; then
	echo -e "\nError is function : ShouldModify() - Not pased an argument\n"
	exit
    else
	echo "Checking condition"
	if [ $(diff $DATA_FOLDER/$1 $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
	    return $(ISTRUE)
	else
	    return $(ISFALSE)
	fi
    fi
}


case "$CMD" in

"install") 
	echo "Copying HOSTS files to $DATA_FOLDER"
	if [ $(ls $(pwd)/hosts.* | wc -l) -eq 0 ]; then
	    echo -e "\n\tNo HOSTS files in current directory. Please go to the install directory with hosts files\n"
	    exit
	fi
	# Always copy over HOSTS FILES
	echo "Installing host files..."
	sudo cp $(pwd)/hosts* $DATA_FOLDER/
	# if this is an update or not been installed before
	if [ ! -e $INSTALL_FOLDER/ModifyTrackingBlocker ] || [ $(diff $PWD/ModifyTrackingBlocker $INSTALL_FOLDER/ModifyTrackingBlocker | wc -l) -gt 0 ]; then
	    echo "Installing executable..."
	    sudo cp $(pwd)/ModifyTrackingBlocker $INSTALL_FOLDER/
	fi
        # Saving original HOST file
	if [ ! -e $DATA_FOLDER/hosts.original ]; then
	    echo "Making a copy of original hosts file..."
	    # backwards compatibility
	    if [ -e $HOST_FOLDER/hosts.original ]; then
		sudo cp $HOST_FOLDER/hosts.original $DATA_FOLDER/
	    else
		sudo cp $HOST_FOLDER/hosts $DATA_FOLDER/hosts.original
	    fi
	fi
	# Checking if $INSTALL_FOLDER is already in PATH
	LINE=0
	echo "Checking if PATH is preconfigured to look in $INSTALL_FOLDER..."
	if [ -e $HOME/.bash_profile ]; then
	    LINE=$(echo $PATH | grep $INSTALL_FOLDER | wc -l)
	    if [ $(grep Tracking_Blocker $HOME/.bash_profile | wc -l) -gt 0 ]; then
		echo "Removing previous Tracking_Blocker settings..."
		cp $HOME/.bash_profile $HOME/.previous_bash_profile
		echo '' > $HOME/.bash_profile
		while read line
		do
		    if [ $(echo $line | grep -v Tracking_Blocker | wc -l) -gt 0 ]; then
			echo $line >> $HOME/.bash_profile
		    fi
		done < "$HOME/.previous_bash_profile"
	    fi
	fi
	# Adding this script to PATH
	if [ $LINE -eq 0 ]; then
	    echo "Adding $INSTALL_FOLDER to PATH..."
	    echo -e "\nexport PATH=\$PATH:$INSTALL_FOLDER\n" >> ~/.bash_profile
	fi  
	. $HOME/.bash_profile
	;;
"original")
	echo -e "Reverting HOST file to original..."
	if [ ! $(diff $DATA_FOLDER/hosts.original $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
            sudo cp $DATA_FOLDER/hosts.original $HOST_FOLDER/hosts
	    $DNSFLUSH
	fi
	;;
"off")
	echo -e "Reverting HOST file to original..."
	if [ ! $(diff $DATA_FOLDER/hosts.original $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
            sudo cp $DATA_FOLDER/hosts.original $HOST_FOLDER/hosts
	    $DNSFLUSH
	fi
	;;
"safe") 
	echo -e "Setting HOST file to safe mode... (~ $(cat $DATA_FOLDER/hosts.safe | wc -l) sites blocked)"
	if [ ! $(diff $DATA_FOLDER/hosts.safe $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
	    sudo cp $DATA_FOLDER/hosts.safe $HOST_FOLDER/hosts
	    $DNSFLUSH
	fi
	;;
"on")
	echo -e "Setting HOST file to ultrasafe mode... (~ $(cat $DATA_FOLDER/hosts.ultrasafe | wc -l) sites blocked)"
	if [ ! $(diff $DATA_FOLDER/hosts.ultrasafe $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
	    sudo cp $DATA_FOLDER/hosts.ultrasafe $HOST_FOLDER/hosts
	    $DNSFLUSH
	fi
	;;
"ultrasafe")
	echo -e "Setting HOST file to ultrasafe mode... (~ $(cat $DATA_FOLDER/hosts.ultrasafe | wc -l) sites blocked)"
	if [ $(diff $DATA_FOLDER/hosts.ultrasafe $HOST_FOLDER/hosts | wc -l) -eq 0 ]; then
	    sudo cp $DATA_FOLDER/hosts.ultrasafe $HOST_FOLDER/hosts
	    $DNSFLUSH
	fi
	;;
"stat") echo -e "\n\t ~ $(cat $HOST_FOLDER/hosts | wc -l) sites blocked\n"
	;;
"add")
	if [ -z "$2" ]; then
	    echo -e "\nPlease provide website address\n"
	    exit
	else
	    echo "127.0.0.1  www."$2 | sudo tee -a $DATA_FOLDER/hosts.ultrasafe
	    echo "127.0.0.1  $2" | sudo tee -a $DATA_FOLDER/hosts.ultrasafe
	fi
	;;
"remove")
	if [ -z "$2" ]; then
	    echo -e "\nPlease provide website address\n"
	    exit
	else
        string="$(grep -h "$2" $DATA_FOLDER/hosts.ultrasafe)"
        echo "STRING = $string --"
        if [ ! -z "$string" ]; then
            domain="$(echo $string | awk '{print $1}')"
            addr="$(echo $string | awk '{print $2}')"
            echo -e "$domain  $addr " | sudo tee -a $DATA_FOLDER/hosts.removed
            sudo sed -i "/$2/d" $DATA_FOLDER/hosts.ultrasafe
        fi
	fi
	;;
"reremove")
    while read line
    do
        #sudo sed -i "/$(echo $line | awk '{print $2}')/d" $DATA_FOLDER/hosts.ultrasafe
        sudo sed -i "/$line /d" $DATA_FOLDER/hosts.ultrasafe
    done < $DATA_FOLDER/hosts.removed
    ;;
*) echo "Unrecognized command: Options are install, off, on, stat, original, safe, ultrasafe, add"
	exit
	;;

esac
