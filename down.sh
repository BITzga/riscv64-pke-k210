make clean
#replace 123 with your pwd
echo 123| sudo  chmod -R  777 /dev/ttyUSB0
make k210
echo ""
echo --------down.sh ------
echo user program entry is: 
riscv64-unknown-elf-objdump -d obj/riscv-pke |grep main


