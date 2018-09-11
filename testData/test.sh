g++ ../wc.cpp -o a.out

./a.out -cwla 1.cpp > 1.out
diff 1.out 1.std
t=$?
if [ "$t" == 0 ]
then
	echo "1.cpp is ok!"
else
	echo "1.cpp is wrong!"
fi
./a.out -cwla 2.cpp > 2.out
diff 2.out 2.std
t=$?
if [ "$t" == 0 ]
then
	echo "2.cpp is ok!"
else
	echo "2.cpp is wrong!"
fi

./a.out -cwla 3.cpp > 3.out
diff 3.out 3.std
t=$?
if [ "$t" == 0 ]
then
	echo "3.cpp is ok!"
else
	echo "3.cpp is wrong!"
fi

./a.out -cwla 4.cpp > 4.out
diff 4.out 4.std
t=$?
if [ "$t" == 0 ]
then
	echo "4.cpp is ok!"
else
	echo "4.cpp is wrong!"
fi

./a.out -cwla 5.cpp > 5.out
diff 5.out 5.std
t=$?
if [ "$t" == 0 ]
then
	echo "5.cpp is ok!"
else
	echo "5.cpp is wrong!"
fi

./a.out -cwla 6.cpp > 6.out
diff 6.out 6.std
t=$?
if [ "$t" == 0 ]
then
	echo "6.cpp is ok!"
else
	echo "6.cpp is wrong!"
fi

./a.out -cwlas temp
