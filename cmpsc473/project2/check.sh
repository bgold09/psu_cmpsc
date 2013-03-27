#!/bin/bash

set -e

FF=0
BF=1
WF=2
BS=3
INDIR="tests/inputs"
OUTDIR="tests/outputs"

function print_type()
{
	case "$1" in 
		"$FF") 
			TYPE="First Fit allocation" ;;
		"$BF") 
			TYPE="Best Fit allocation" ;;
		"$WF") 
			TYPE="Worst Fit allocation" ;;
		"$BS") 
			TYPE="Buddy System allocation" ;;
		*)     
			echo unknown allocation, exiting...
			exit 1
			;;
	esac
	echo checking $TYPE...
}

function make_and_check()
{
	make NUM=$1 &> /dev/null && ./out$1 && \
	RES=`diff test${1}_output.txt \
		$OUTDIR/test${1}_output.txt`
	if [ -z "$RES" ]
	then
		echo -e "\toutputs for test${1} match"
	else
		echo -e "\toutputs for test${1} do not match"
	fi
}

for var in "$@"  # check each allocation type, one at a time
do 
	print_type $var
	ls -1 $INDIR/test* | while read FILENAME  # check each file
	do
		NUM=`echo $FILENAME | sed -e 's/.*test//' -e 's/\.c//'`
		case "$var" in 
			"$FF") 
				if [ -n "`grep "First Fit" $FILENAME`" ]
				then
					make_and_check $NUM
				fi
				;;
			"$BF") 
				if [ -n "`grep "Best Fit" $FILENAME`" ]
				then
					make_and_check $NUM
				fi
				;;
			"$WF") 
				if [ -n "`grep "Worst Fit" $FILENAME`" ]
				then
					make_and_check $NUM
				fi
				;;
			"$BS") 
				if [ -n "`grep "Buddy System" $FILENAME`" ]
				then
					make_and_check $NUM
				fi
				;;
			*)     
				echo unknown allocation, exiting...
				exit 1
				;;
		esac
	done
done

exit 0
