# ConquestGame
Description:

Creators:
Samuel Vachon 40281580,
Ryan Medlej 40199472
Kirubel Asrat 40153970
Farhan Ali Khan 40287970
Youry Nelson 40343198

## How to run the application
Compile all the files with a command like “g++ -std=c++17 .\Card.cpp .\CardsDrive.cpp .\Map.cpp .\MapDriver.cpp .\OrderDriver.cpp .\Orders.cpp .\Player.cpp .\PlayerDriver.cpp .\GameEngine.cpp .\GameEngineDriver.cpp .\MainDriver.cpp -o main”

From this, run the main.exe

This will open a command line where you can run many commands to start the game, here are the steps:

1. Load the map (loadmap <name.map>)
2.validate the map (validatemap), should say something like map validated successfully 
3. add atleast two players (addplayer X addPlayer Y)
4. Assign the countries (assigncountries)
5. issue orders (issueorder)
6. Finish issuing (endissueorders)
7. execute all orders (executeorders)
8. end executionphase (endexecorders)
Once that ends you can play another round by doing:
play
then repeating everything from step 5-8
