script(){
for i in "$1"/*
do
	if test -d "$i"
	then
		if [[ ~$(ls -A $i) ]];
		then
			echo "$i e gol sau are doar subdirectoare"
		fi
		script $i
	fi
done
}

script $1
