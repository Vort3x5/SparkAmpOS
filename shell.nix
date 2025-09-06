{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	buildInputs = with pkgs; [
		qemu
		gdb
		fasm
		gnumake

		jack2
		qjackctl
		patchage
    ];

	shellHook = ''
		fish
	'';
}
