cat > iso/boot/grub/grub.cfg << EOF
menuentry "boot.iso" {
	multiboot /boot/boot.iso
}
EOF
