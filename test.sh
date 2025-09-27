#!/bin/bash


echo "Compiling the programs"
make

echo
echo "Statistics regarding data sets"
for csv in *.csv; do
    echo "$csv   total:" $(sort -u $csv | wc -l) "   F:" $(sort -u $csv | awk '/^0,/' | wc -l) "   T:" $(sort -u $csv | awk '/^1,/' | wc -l)
done

echo
echo "Running the OFFLINE algorithm on the data sets"
for csv in *.csv; do
    ./offline $csv > ${csv/.csv/.log}
done

for csv in *.csv; do
    mv ${csv/.csv/.log} ${csv/.csv/-offline.log}
done

echo
echo "Running the ONLINE algorithm on the data sets"
for csv in *.csv; do
    ./online $csv > ${csv/.csv/.log}
done

echo
echo -n "Test the correctness of the output (takes time)? [y/n]: "
read -n 1 answer
echo

case "$answer" in
    y|Y)
        echo
        echo "Checking output with Python script"
        for csv in *.csv; do
            echo -n "*** ${csv/.csv/: }"
            ./check-horn ${csv/.csv/}
        done
        ;;
    *)
        ;;
esac

for csv in *.csv; do
    mv ${csv/.csv/.log} ${csv/.csv/-online.log}
done

echo
echo "Comparing offline and online output (no output is good!)"
for csv in *.csv; do
    diff -q ${csv/.csv/-offline.log} ${csv/.csv/-online.log}
done

echo
echo "Cleaning up"
make scratch
for csv in *.csv; do
    rm ${csv/.csv/-offline.log} ${csv/.csv/-online.log}
done
