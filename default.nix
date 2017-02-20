with import <nixpkgs> {}; 

stdenv.mkDerivation {
    name = "hex_hav";
    buildInputs = [ stdenv cmake pkgconfig gnome2.gtkmm xorg.libpthreadstubs ];
    src = ./.;
    configureFlags = '' 
        -DCMAKE_BUILD_TYPE=RELEASE
    '';
    enableParallelBuilding = true;
}

