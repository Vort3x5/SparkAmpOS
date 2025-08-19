{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	buildInputs = with pkgs; [
		qemu
		gdb
		fasm
		gnumake
    ];

	shellHook = ''
		fish
	'';
}
