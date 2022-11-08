# PacmanAI!
Hello !
As final project of AI course at Afeka Tel Aviv Collge Of Engineering , I was asked to implement fully AI game of pacman!

pacman vs 3 monsters

Coins collecting & role switch

[Uploading pacman_collect_coins .gif…]()

Role switch twice 

![pacman_role_switch ](https://user-images.githubusercontent.com/87577792/200445696-215448b9-769a-41c2-94ef-b18684453cce.gif)


Above you can view GIFs of the program ^^^

PACMAN

RUN- uses BFS to find closest enemy and goes to square with highest Manhattan distance from the monster . 
COLLECT- if closest monster is more than 25 steps away , pacman uses BFS to collect coins.
CHASE - pacman uses BFS to find closest monster and A* to attack it.

MONSTER

RUN- uses BFS to find pacman and goes to highest manhattan distance square.
CHASE- uses A* to attack pacman

Decision making:
FSM - used to determine in each iteration , what will pacman and the monsters do.

the Execute function will accept (bool escape,bool close) for pacman.

the Execute function will accept (bool escape ) for monster.

escape -> is it pacman's turn to escape or not 
close -> is the closest monster to pacman more than 25 steps away.
every 3 seconds , roles are reversed , attacker becomes target , target becomes attacker.
