for ((i=1; i <= 100; i++))
do
	for file in $(cat clients)
	do
		"./$file"
	done
done

exit 0
