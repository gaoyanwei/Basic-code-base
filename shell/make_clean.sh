#/bin/bash

LOG_FILE=/tmp/make_log.txt

usge(){
	echo "*********************************"
	echo "	make fclite"
	echo "	cmd source make_clean.sh make"
	echo "	clean fclite"
	echo "	cmd source make_clean.sh clean"
	echo "*********************************"
}

fclite_clean(){
	work=`pwd`
#	echo "$1"
	cd $work/$1
#	echo "$work/$1"
	git clean -f -d *.o.cmd > /dev/null
	cd - > /dev/null

    # 进入工作目录
	if [ "$1" == "work_fclite/fclite" ]; then
		cd $work/$1/src/core >/dev/null 2>&1
	else
		cd $work/$1/src/tgc >/dev/null 2>&1
	fi

	if [ "$?" == "0" ]; then
		make clean >> $LOG_FILE 2>&1
		cd - > /dev/null
	fi
}

fclite_make(){
	work=`pwd`
	cd $work/build
#	echo "$work/bulid"
    rm -rf * > /dev/null
	cmake $work/$1 >/dev/null
	make > $LOG_FILE 2>&1

	if [ "$?" != "0" ]; then
		echo "make error"
	else
		rm -rf /mnt/hgfs/work_linux/package/*
		cp *.tar /mnt/hgfs/work_linux/package/
		cd - > /dev/null
	fi
}

uid=`id -u`

if [ "$uid" != "0" ]; then
    echo "Must use root."
    exit 1
fi

if [ "$1" == "clean" ]; then
	if [ "$2" == "" ]; then
		echo "plesae input dir"
		exit 1;
	fi
	fclite_clean $2
elif [ "$1" == "make" ]; then
	if [ "$2" == "" ]; then
		echo "plesae input dir"
		exit 1;
	fi
	fclite_make $2
	fclite_clean $2
elif [ "$1" == "" ]; then
	usge
else
	usge
fi

