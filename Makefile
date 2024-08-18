run:
	cd build && ninja && ./sadeEngine

debug:
	cd build && ninja && gdb sadeEngine

memCheck:
	cd build && ninja && valgrind --leak-check=full \
                     --show-leak-kinds=all \
                     --track-origins=yes \
                     --verbose \
                     --log-file=../valgrind-out.txt\
                     ./sadeEngine 
