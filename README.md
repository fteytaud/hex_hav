# hex_hav

The Hex and Havannah connection games with MCTS-based AI players.


## Games 

- Hex
- Havannah

TODO lien wikipedia, screenshot ?


## MCTS-based algorithms (AI players)

- `MCTS`: vanilla MCTS (TODO ref papier)
- `Rave`: MCTS with the RAVE improvement (TODO ref papier)
- `PPR`: RAVE with the improvement proposed in TODO 


## Build 

TODO 

TODO dependances

```
mkdir build
cd build
cmake ..  # for hex in debug mode
# for havannah in release mode :
# CXXFLAGS="-DBOARD_TYPE=BoardHavannah" cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make 
```

## Run graphical user interfaces

TODO

```
./hstar_gui.out                    # GUI
./hstar_cli.out ...                # CLI for comparing two AI
./hstar_cli.out ... 2> /dev/null   # CLI with no log
```

## Run command line interface

TODO

```
./hstar_gui.out                    # GUI
./hstar_cli.out ...                # CLI for comparing two AI
./hstar_cli.out ... 2> /dev/null   # CLI with no log
```

## Contributors

- Fabien Teytaud
- Julien Dehos
- Joris Duguépéroux
- Ahmad Mazyad


## License

See <LICENSE.txt> (BSD-3)


