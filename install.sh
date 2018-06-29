nasm -f elf32 kernel.asm -o kasm.o
echo "Finished compiling asm file"
gcc -m32 -c kernel.c -o kc.o
echo "Finished compiling user-land binary"
ld -m elf_i386 -T link.ld -o image/kernel kasm.o kc.o
echo "Completed linkage on both files.."
rm -rf *.o

echo "Starting emulation..."
qemu-system-i386 -kernel image/kernel
