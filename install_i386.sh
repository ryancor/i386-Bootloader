nasm -f bin x86_kern_src/boot/bootsect.asm -i'x86_kern_src/boot/' -o image/bootsect.bin
echo "Compile second stage bootloader"
nasm -f elf x86_kern_src/boot/kernel_entry_i386.asm -o x86_kern_src/boot/kasm.o
nasm -f elf x86_kern_src/cpu/interrupt.asm -o x86_kern_src/cpu/interrupt.o
echo "Finished compiling asm file"
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/kernel.c -o x86_kern_src/kernel/kc.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/cpu/idt.c -o x86_kern_src/cpu/idt.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/cpu/isr.c -o x86_kern_src/cpu/isr.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/cpu/timer.c -o x86_kern_src/cpu/timer.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/userspace/shell.c -o x86_kern_src/userspace/shell.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/grub_framework/s_string.c -o x86_kern_src/grub_framework/s_string.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/drivers/mouse.c -o x86_kern_src/drivers/mouse.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/drivers/screen.c -o x86_kern_src/drivers/screen.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/drivers/cmos.c -o x86_kern_src/drivers/cmos.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/ports/ports.c -o x86_kern_src/kernel/ports/ports.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/interrupts/int.c -o x86_kern_src/kernel/interrupts/int.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/fs/fs.c -o x86_kern_src/kernel/fs/fs.o
i386-elf-gcc -fno-stack-protector -Wno-int-conversion -c x86_kern_src/kernel/fs/list.c -o x86_kern_src/kernel/fs/list.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/fs/pipe.c -o x86_kern_src/kernel/fs/pipe.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/mem/mem.c -o x86_kern_src/kernel/mem/mem.o
i386-elf-gcc -fno-stack-protector -Wno-int-conversion -c x86_kern_src/kernel/mem/shm.c -o x86_kern_src/kernel/mem/shm.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/sys/process.c -o x86_kern_src/kernel/sys/process.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/sys/task.c -o x86_kern_src/kernel/sys/task.o
i386-elf-gcc -fno-stack-protector -Wno-incompatible-pointer-types -c x86_kern_src/kernel/sys/syscall.c -o x86_kern_src/kernel/sys/syscall.o
i386-elf-gcc -fno-stack-protector -c x86_kern_src/kernel/devices/fpu.c -o x86_kern_src/kernel/devices/fpu.o
echo "Finished compiling object files"
i386-elf-ld -T link/link.ld -o image/kernel x86_kern_src/boot/kasm.o x86_kern_src/kernel/kc.o x86_kern_src/cpu/interrupt.o x86_kern_src/cpu/idt.o x86_kern_src/cpu/isr.o x86_kern_src/cpu/timer.o x86_kern_src/userspace/shell.o x86_kern_src/grub_framework/s_string.o x86_kern_src/kernel/ports/ports.o x86_kern_src/kernel/fs/fs.o x86_kern_src/kernel/fs/pipe.o x86_kern_src/kernel/fs/list.o x86_kern_src/kernel/mem/mem.o x86_kern_src/kernel/mem/shm.o x86_kern_src/drivers/screen.o x86_kern_src/drivers/mouse.o x86_kern_src/drivers/cmos.o x86_kern_src/kernel/interrupts/int.o x86_kern_src/kernel/sys/process.o x86_kern_src/kernel/sys/task.o x86_kern_src/kernel/sys/syscall.o x86_kern_src/kernel/devices/fpu.o --oformat binary
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
rm -rf x86_kern_src/kernel/sys/*.o
rm -rf x86_kern_src/drivers/*.o
rm -rf x86_kern_src/kernel/interrupts/*.o
rm -rf x86_kern_src/kernel/devices/*.o

if [ ! -f image/kernel ]; then
	kstr=$(printf "\n\nkernel image could not be created..")
	echo "$kstr"
	exit 1
fi

echo "..."

echo "Creating os-image from 1st stage boot"
cat image/bootsect.bin image/kernel > image/os-image.bin

echo "Starting emulation..."
qemu-system-i386 -s -fda image/os-image.bin
