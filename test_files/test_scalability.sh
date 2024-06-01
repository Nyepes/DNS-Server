for ((i=1; i<=100; i++))
do
    echo "$i"
    ./../bin/client "www.google.com"
done