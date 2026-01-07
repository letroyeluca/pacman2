# PACMAN - Advanced programming 
## Luca Letroye - s0242098
Dit is mijn uitwerking van het project PacMan van het vak Advanced programming 2025/2026. Het project is opgedeelt in
2 delen: app en logic. de App is de representatie van de game, dit houdt in dat dit al de input bepaalt en de output van de game maar geen verwerking. De logic library doet de verwerking van de input en met dit bepaalt de app dan wat hij moet tonen.
De logic kan ook apart worden compilerd en dit kan via github action bewezen worden met dat dit voor de laatste ingediende build compilert zonder de app.

--- 

## vereisten
- Ubuntu: 24.04.3
- SFML: 2.6.1
- CMake: 3.28.3
- G++: 13.2.0
- Clang: 18.0.0

---

## Handleiding

### Compiling & running

```bash
cmake .
make 
cd app
./pacman_game
```

## geimplementeerd
alle core gameplay van de orginele pacman inclusief ghost AI en level modifiers werken.
WEL geimplementeerd:
- extendable maploader
- assetManager


wat NIET geimplementeerd is:

- geluid
- slimmere ghosts
- random maps
