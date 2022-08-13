for f in *.c; do
    if [ $f -nt ${f%%.c}.o ]; then
        echo "compiling $f..."
        x86_64-w64-mingw32-gcc -I. -Wall -pedantic -O -c $f
    fi
done

echo "creating libnova.lib..."
x86_64-w64-mingw32-ar rcs libnova.lib *.o
cp libnova.lib ../../lib/win/
