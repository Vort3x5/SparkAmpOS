cat > iso/boot/grub/grub.cfg << EOF
menuentry "SparkAmpOS" {
	multiboot /boot/SparkAmpOS
}
EOF
