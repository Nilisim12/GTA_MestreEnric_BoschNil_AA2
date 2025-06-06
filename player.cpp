#include "main.h"
#include <windows.h>

void GTASanAndreas::initializePlayer()
{
    // Inicializar CJ (jugador)
    player.position.x = config.width / 6;
    player.position.y = config.height / 2;
    player.previousPosition = player.position;
    player.direction = '^';
    player.money = 0;
    player.life = config.cjLife;
    player.attackPower = config.cjAttack;
}

void GTASanAndreas::playerAttack()
{
    if (GetAsyncKeyState(ATTACK_KEY))
    {
        for (int i = 0; i < pedestriansCount; i++)
        {
            if (!pedestrians[i].alive)
            {
                continue;
            }
            // Verificar si el jugador está al lado del peaton
            if (abs(player.position.x - pedestrians[i].position.x) <= 1 && abs(player.position.y - pedestrians[i].position.y) <= 1)
            {
                // Comportamiento especial para Big Smoke
                if (pedestrians[i].isBigSmoke)
                {
                    // Big Smoke recibe daño reducido
                    pedestrians[i].life -= player.attackPower / 2;

                    if (pedestrians[i].life <= 0)
                    {
                        pedestrians[i].alive = false;
                        map[pedestrians[i].position.y][pedestrians[i].position.x] = ' ';
                        
                    }
                }
                else // Peatones normales
                {
                    pedestrians[i].life -= player.attackPower;

                    // Peaton muerto
                    if (pedestrians[i].life <= 0)
                    {
                        pedestrians[i].alive = false;
                        map[pedestrians[i].position.y][pedestrians[i].position.x] = ' ';

                        // Determinar cantidad de dinero segun la isla
                        int moneyAmount;
                        if (pedestrians[i].position.x < islandWidth) // Los Santos
                        {
                            moneyAmount = rand() % config.losSantosMaxMoney + 1;
                        }
                        else if (pedestrians[i].position.x < islandWidth * 2) // San Fierro
                        {
                            moneyAmount = rand() % config.sanFierroMaxMoney + 1;
                        }
                        else // Las Venturas
                        {
                            moneyAmount = rand() % config.lasVenturasMaxMoney + 1;
                        }

                        addMoney(pedestrians[i].position, moneyAmount);
                        regeneratePedestrian(i);
                    }
                }

                // Contraataque de peatones agresivos
                if (pedestrians[i].isAggressive && pedestrians[i].attackCooldown <= 0)
                {
                    player.life -= pedestrians[i].attackPower;

                    // Big Smoke hace mas daño
                    if (pedestrians[i].isBigSmoke)
                    {
                        player.life -= 10; 
                    }

                    pedestrians[i].attackCooldown = FPS; 
                }
            }
        }
    }
}

void GTASanAndreas::collectMoney()
{
    Money* current = money;
    Money* previous = nullptr;

    while (current != nullptr)
    {
        if (player.position.x == current->position.x &&  player.position.y == current->position.y)
        {
            player.money += current->amount;

            if (previous == nullptr)
            {
                money = current->next;
                delete current;
                current = money;
            }
            else
            {
                previous->next = current->next;
                delete current;
                current = previous->next;
            }
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
}