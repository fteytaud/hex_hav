with import <nixpkgs> {}; 

stdenv.mkDerivation {
    name = "hex_hav";
    buildInputs = [ stdenv cmake pkgconfig gnome2.gtkmm xorg.libpthreadstubs pcre python cxxtest ];
    src = ./.;

    # force parallel building
    buildPhase = ''
	make -j8 
    '';
    # enableParallelBuilding = true;  
}

