#### Instruction for installing gcc 9.3 or gcc 10 on ubuntu 18.04 (experimental)

		1. sudo apt update
		2. sudo apt upgrade
		3. sudo apt install build-essential
		4. sudo apt install flex
		5. git clone https://github.com/gcc-mirror/gcc.git gcc-10
		6. cd gcc-10
		7. contrib/download_prerequisites    (wait for this command to finish)
		8. cd ..
		9. mkdir build
		10. cd build
		11. ../gcc-10/configure -v --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --prefix=/usr/local/gcc-10 --enable-checking=release --enable-languages=c,c++,fortran --disable-multilib --program-suffix=-10
		12. make -j 8
		13. sudo make install-strip
		14. Add the following two lines to your shell rc file. For example .zshrc
			export export PATH=/usr/local/gcc-10/bin:$PATH
			export LD_LIBRARY_PATH=/usr/local/gcc-10/lib64:$LD_LIBRARY_PATH
		15. source ~/.<rc file> or logout and login
		16. Compile and run with   g++-10 -std=c++20 ranges.cpp && ./a.out