# PACMAN - Advanced programming Luca Letroye
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

### Gameplay

#### StartMenu
Hier staan de top 5 score op dat er op dit apparaat zijn gespeeld, je kan met een ENTER door gaan om de game te starten of met ESC de game afsluiten.
### Level
hier zal pacman onderaan de map starten als een geel bolletje en zodra je beweegt met volgende pijltjes: LEFT,RIGHt,UP of DOWN zal pacman die richting aannemen en beginnen bewegen. Het doel is om al de munten en geesten op te eten om naar het volgende level te gaan. Linksbovenaan het scherm staan de hoeveelheid levens dat je nog hebt als kleine pacmans. Om een hoek sneller af te slagen kan je altijd op voorhand de richting klikken, het systeem zal altijd de laatste gekozen (mogelijke) richting gebruiken.
### level completed!
Indien je een level hebt verslagen zal je dit zien. hier kan je je huidige score zien en op ENTER duwen om verder te gaan 
