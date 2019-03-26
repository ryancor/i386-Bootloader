nasm -f elf32 x86_kern_src/boot/kernel_entry.asm -o x86_kern_src/boot/kasm.o
nasm -f elf32 x86_kern_src/cpu/interrupt.asm -o x86_kern_src/cpu/interrupt.o
echo "Finished compiling asm file"
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/kernel.c -o x86_kern_src/kernel/kc.o
gcc -fno-stack-protector -m32 -c x86_kern_src/cpu/idt.c -o x86_kern_src/cpu/idt.o
gcc -fno-stack-protector -m32 -c x86_kern_src/cpu/isr.c -o x86_kern_src/cpu/isr.o
gcc -fno-stack-protector -m32 -c x86_kern_src/cpu/timer.c -o x86_kern_src/cpu/timer.o
gcc -fno-stack-protector -m32 -c x86_kern_src/userspace/shell.c -o x86_kern_src/userspace/shell.o
gcc -fno-stack-protector -m32 -c x86_kern_src/grub_framework/s_string.c -o x86_kern_src/grub_framework/s_string.o
gcc -fno-stack-protector -m32 -c x86_kern_src/drivers/mouse.c -o x86_kern_src/drivers/mouse.o
gcc -fno-stack-protector -m32 -c x86_kern_src/drivers/screen.c -o x86_kern_src/drivers/screen.o
gcc -fno-stack-protector -m32 -c x86_kern_src/drivers/cmos.c -o x86_kern_src/drivers/cmos.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/ports/ports.c -o x86_kern_src/kernel/ports/ports.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/interrupts/int.c -o x86_kern_src/kernel/interrupts/int.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/fs/fs.c -o x86_kern_src/kernel/fs/fs.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/fs/list.c -o x86_kern_src/kernel/fs/list.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/fs/pipe.c -o x86_kern_src/kernel/fs/pipe.o
gcc -fno-stack-protector -m32 -c x86_kern_src/kernel/mem/mem.c -o x86_kern_src/kernel/mem/mem.o
echo "Finished compiling object files"
ld -m elf_i386 -T link/link.ld -o image/kernel x86_kern_src/boot/kasm.o x86_kern_src/kernel/kc.o x86_kern_src/cpu/interrupt.o x86_kern_src/cpu/idt.o x86_kern_src/cpu/isr.o x86_kern_src/cpu/timer.o x86_kern_src/userspace/shell.o x86_kern_src/grub_framework/s_string.o x86_kern_src/kernel/ports/ports.o x86_kern_src/kernel/fs/fs.o x86_kern_src/kernel/fs/pipe.o x86_kern_src/kernel/fs/list.o x86_kern_src/kernel/mem/mem.o x86_kern_src/drivers/screen.o x86_kern_src/drivers/mouse.o x86_kern_src/drivers/cmos.o x86_kern_src/kernel/interrupts/int.o
echo "Completed linkage on both files.."
rm -rf x86_kern_src/*.o
rm -rf x86_kern_src/cpu/*.o
rm -rf x86_kern_src/boot/*.o
rm -rf x86_kern_src/kernel/*.o
rm -rf x86_kern_src/userspace/*.o
rm -rf x86_kern_src/grub_framework/*.o
rm -rf x86_kern_src/kernel/ports/*.o
rm -rf x86_kern_src/kernel/fs/*.o
rm -rf x86_kern_src/kernel/mem/*.o
rm -rf x86_kern_src/drivers/*.o
rm -rf x86_kern_src/kernel/interrupts/*.o

if [ ! -f image/kernel ]; then
	kstr=$(printf "\n\nkernel image could not be created..")
	echo "$kstr"
	exit 1
fi

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
