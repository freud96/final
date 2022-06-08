all : main.cpp 
	riscv32-unknown-elf-g++ -std=c++14 main.cpp -o main -march=rv32ima -mabi=ilp32

sim: all
	$(EE6470)/riscv-vp/vp/build/bin/riscv-vp-acc_mul1 --intercept-syscalls main

dump-elf: all
	riscv32-unknown-elf-readelf -a main

clean:
	rm -f main
