# hex_hav

The Hex and Havannah connection games with MCTS-based AI players.


## Games 

- Hex
- Havannah

TODO lien wikipedia


## MCTS-based algorithms (AI players)

- `PlayerMcts`: vanilla MCTS (TODO ref papier)
- `PlayerRave`: MCTS with the RAVE improvement (TODO ref papier)
- `PlayerRaveLgrf1`: TODO
- `PlayerRaveMast`: TODO
- `PlayerRaveNast2`: TODO
- `PlayerRavePpr`: TODO
- `PlayerRavePr`: RAVE with the improvement proposed in TODO (lien vers hal)


## Build 

- cmake >= 3.0
- C++14 compiler
- pkg-config
- gtkmm 2
- cxxtest (optional)

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make -j8
```


## Run 

### Command line interface

For comparing two AI players.

- `hex_cli.out`
- `hav_cli.out`

```
$ ./hav_cli.out 7 10 Mcts 0.25_1000 Rave 70_0.25_1000

new game
black played in 0.146 s (cell 5)
white played in 0.091 s (cell 20)
...

nb games; board type; board size; white; KUCT; nbSimulations; black; KRAVE; KUCT; nbSimulations; white ratio; black ratio; mean nb turns; average time;draw; white ring; white bridge; white fork; black ring; black bridge; black fork;
10; BoardHavannah; 7; Mcts; 0.25; 1000; Rave; 70; 0.25; 1000; 0.5; 0.5; 16.3; 1.0816; 0; 0.1; 0.1; 0.3; 0.1; 0.1; 0.3; 
```

### Graphical user interface (1)

Two players (AI or human).

- `hex_gui1.out`
- `hav_gui1.out`

![](doc/screenshot_hex_gui1.png)


### Graphical user interface (2)

Two human players with AI indications (colors).

- `hex_gui2.out`
- `hav_gui2.out`

![](doc/screenshot_hav_gui2.png)


## TODO

- check unit tests
- complete this readme (references)
- write a documentation 


## Contributors

- Fabien Teytaud
- Julien Dehos
- Joris Duguépéroux
- Ahmad Mazyad


## License

See [LICENSE.txt](LICENSE.txt) (BSD-3)


