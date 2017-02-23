(define-module (hex_hav)
  #:use-module (guix gexp) 
  #:use-module (guix packages) 
  #:use-module (gnu packages cmake) 
  #:use-module (gnu packages pkg-config)
  #:use-module (gnu packages gtk)
  #:use-module (guix build-system cmake)
  #:use-module (guix licenses))

(define-public hex_hav
  (package
    (name "hex_hav")
    (version "1.0")
    (source (local-file "" #:recursive? #t))
    (build-system cmake-build-system)
    (native-inputs `(("pkg-config" ,pkg-config)))
    (inputs `(("gtkmm" ,gtkmm-2)))
    (arguments '(#:tests? #f)) 
    (synopsis "Hex and Havannah games with MCTS-based AI players")
    (description "Hex and Havannah connection games with MCTS AI and several interfaces (command line, gui).")
    (home-page "https://github.com/fteytaud/hex_hav")
    (license bsd-3)))

