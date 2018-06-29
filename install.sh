nasm -f elf32 x86_kern_src/kernel.asm -o x86_kern_src/kasm.o
echo "Finished compiling asm file"
gcc -m32 -c x86_kern_src/kernel.c -o x86_kern_src/kc.o
echo "Finished compiling object files"
ld -m elf_i386 -T link/link.ld -o image/kernel x86_kern_src/kasm.o x86_kern_src/kc.o
echo "Completed linkage on both files.."
rm -rf x86_kern_src/*.o

echo "..."

dd if=/dev/zero of=kernel.img bs=1024 count=1440
dd if=image/kernel of=kernel.img seek=0 count=1 conv=notrunc
echo "Finished creating image.."

mv kernel.img image/
genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o image/kernel.iso -b kernel.img -hide kernel.img image/
echo "Finished creating ISO....."

echo "Starting emulation..."
qemu-system-i386 -kernel image/kernel
#qemu-system-i386 -cdrom image/kernel.iso
