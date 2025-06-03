#include "main.h"

void GTASanAndreas::initializePedestrians()
{
    //Inicializar pedestrians a todas las islas
    pedestriansCount = config.losSantosPedestrians + config.sanFierroPedestrians + config.lasVenturasPedestrians;
    pedestrians = new Pedestrian[pedestriansCount + 1];

    // Los Santos pedestrians
    for (int i = 0; i < config.losSantosPedestrians; i++)
    {
        pedestrians[i].position.x = 5 + rand() % (islandWidth - 10);
        pedestrians[i].position.y = 5 + rand() % (config.height - 10);
        pedestrians[i].direction = (rand() % 2 == 0) ? 'H' : 'V';
        pedestrians[i].alive = true;
        pedestrians[i].isAggressive = (rand() % 100 < 30);
        pedestrians[i].isBigSmoke = false;

        pedestrians[i].life = config.losSantosPedestriansLife;
        pedestrians[i].attackPower = config.losSantosPedestriansAttack;
        map[pedestrians[i].position.y][pedestrians[i].position.x] = 'P';
    }

    // San Fierro pedestrians
    for (int i = 0; i < config.sanFierroPedestrians; i++)
    {
        int index = config.losSantosPedestrians + i;
        pedestrians[index].position.x = islandWidth + 5 + rand() % (islandWidth - 10);
        pedestrians[index].position.y = 5 + rand() % (config.height - 10);
        pedestrians[index].direction = (rand() % 2 == 0) ? 'H' : 'V';
        pedestrians[index].alive = true;
        pedestrians[index].isAggressive = (rand() % 100 < 30);
        pedestrians[index].isBigSmoke = false;

        pedestrians[index].life = config.sanFierroPedestriansLife;
        pedestrians[index].attackPower = config.sanFierroPedestriansAttack;
        map[pedestrians[index].position.y][pedestrians[index].position.x] = 'P';
    }

    // Las Venturas pedestrians
    for (int i = 0; i < config.lasVenturasPedestrians; i++)
    {
        int index = config.losSantosPedestrians + config.sanFierroPedestrians + i;
        pedestrians[index].position.x = islandWidth * 2 + 5 + rand() % (islandWidth - 10);
        pedestrians[index].position.y = 5 + rand() % (config.height - 10);
        pedestrians[index].direction = (rand() % 2 == 0) ? 'H' : 'V';
        pedestrians[index].alive = true;
        pedestrians[index].isAggressive = (rand() % 100 < 30);
        pedestrians[index].isBigSmoke = false;

        pedestrians[index].life = config.lasVenturasPedestriansLife;
        pedestrians[index].attackPower = config.lasVenturasPedestriansAttack;
        map[pedestrians[index].position.y][pedestrians[index].position.x] = 'P';
    }

    //Inicializar a BigSmoke
    int bigSmokeIndex = pedestriansCount;
    pedestrians[bigSmokeIndex].position.x = islandWidth * 2 + 5 + rand() % (islandWidth - 10);
    pedestrians[bigSmokeIndex].position.y = 5 + rand() % (config.height - 10);
    pedestrians[bigSmokeIndex].direction = 'H';
    pedestrians[bigSmokeIndex].alive = true;
    pedestrians[bigSmokeIndex].isAggressive = true;
    pedestrians[bigSmokeIndex].isBigSmoke = true;
    pedestrians[bigSmokeIndex].life = 999;  
    pedestrians[bigSmokeIndex].attackPower = 50;
    map[pedestrians[bigSmokeIndex].position.y][pedestrians[bigSmokeIndex].position.x] = 'B';

    pedestriansCount++; 
}

void GTASanAndreas::regeneratePedestrian(int index)
{
   //que BigSmoke no se regenere
    if (pedestrians[index].isBigSmoke)
    {
        return;
    }

    if (pedestrians[index].position.x < islandWidth)
    {
        // Los Santos pedestrian
        pedestrians[index].position.x = 5 + rand() % (islandWidth - 10);
        pedestrians[index].life = config.losSantosPedestriansLife;
    }
    else if (pedestrians[index].position.x < islandWidth * 2)
    {
        // San Fierro pedestrian
        pedestrians[index].position.x = islandWidth + 5 + rand() % (islandWidth - 10);
        pedestrians[index].life = config.sanFierroPedestriansLife;
    }
    else
    {
        // Las Venturas pedestrian
        pedestrians[index].position.x = islandWidth * 2 + 5 + rand() % (islandWidth - 10);
        pedestrians[index].life = config.lasVenturasPedestriansLife;
    }

    pedestrians[index].position.y = 5 + rand() % (config.height - 10);
    pedestrians[index].alive = true;
    map[pedestrians[index].position.y][pedestrians[index].position.x] = 'P';
}

bool GTASanAndreas::isNextToPlayer(const Position& pedestrianPos)
{
    //Calcular si esta al lado del player
    return (abs(player.position.x - pedestrianPos.x) <= 1 &&abs(player.position.y - pedestrianPos.y) <= 1);
}

void GTASanAndreas::movePedestrians()
{
    for (int i = 0; i < pedestriansCount; i++)
    {
        if (!pedestrians[i].alive || pedestrians[i].isBigSmoke)
        {
            continue;
        }
        if (!isNextToPlayer(pedestrians[i].position))
        {
            map[pedestrians[i].position.y][pedestrians[i].position.x] = ' ';

            if (pedestrians[i].direction == 'H')
            {
                int newX = pedestrians[i].position.x + (rand() % 3 - 1);

                if (newX > 0 && newX < config.width - 1 && map[pedestrians[i].position.y][newX] == ' ')
                {
                    pedestrians[i].position.x = newX;
                }
            }
            else
            {
                int newY = pedestrians[i].position.y + (rand() % 3 - 1);
                if (newY > 0 && newY < config.height - 1 &&  map[newY][pedestrians[i].position.x] == ' ')
                {
                    pedestrians[i].position.y = newY;
                }
            }
        }

       //que no se pinte BigSmoke encima de la P
        if (map[pedestrians[i].position.y][pedestrians[i].position.x] != 'B')
        {
            map[pedestrians[i].position.y][pedestrians[i].position.x] = 'P';
        }
    }
}

void GTASanAndreas::updatePedestrians()
{
    for (int i = 0; i < pedestriansCount; i++)
    {
        if (!pedestrians[i].alive) continue;

        if (pedestrians[i].attackCooldown > 0)
        {
            pedestrians[i].attackCooldown--;
        }

        if (pedestrians[i].isAggressive &&
            isNextToPlayer(pedestrians[i].position) &&
            pedestrians[i].attackCooldown <= 0)
        {
            player.life -= pedestrians[i].attackPower;
            pedestrians[i].attackCooldown = FPS;

           
            if (pedestrians[i].isBigSmoke)
            {
                player.life -= 5; 
            }
        }
    }
}