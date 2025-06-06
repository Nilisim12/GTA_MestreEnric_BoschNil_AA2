#include "main.h"
#include <iostream>

void GTASanAndreas::showView()
{
    system("cls");

    std::cout << "                 GTA SAN ANDREAS" << std::endl;

    
    map[player.previousPosition.y][player.previousPosition.x] = ' ';

    // Dibuja el dinero
    Money* Money = money;
    while (Money != nullptr)
    {
        map[Money->position.y][Money->position.x] = '$';
        Money = Money->next;
    }

    // Dibuja los coches del sistema
    carSystem.drawCarsOnMap(map);

    // Dibuja el coche controlado o el jugador
    if (controlledCar != nullptr)
    {
        map[controlledCar->position.y][controlledCar->position.x] = 'C';
    }
    else
    {
        map[player.position.y][player.position.x] = player.direction;
    }

    // Dibuja a los peatones, incluyendo Big Smoke
    for (int i = 0; i < pedestriansCount; i++)
    {
        if (pedestrians[i].alive)
        {
            char symbol = pedestrians[i].isBigSmoke ? 'B' : 'P';
            map[pedestrians[i].position.y][pedestrians[i].position.x] = symbol;
        }
    }

    // Determina la posicion actual para centrar la vista
    Position currentPosition;
    if (controlledCar != nullptr)
    {
        currentPosition = controlledCar->position;
    }
    else
    {
        currentPosition = player.position;
    }

    int viewX = currentPosition.x - viewWidth / 2;
    int viewY = currentPosition.y - viewHeight / 2;

    if (viewX < 0)
    {
        viewX = 0;
    }
    if (viewX > config.width - viewWidth)
    {
        viewX = config.width - viewWidth;
    }
    if (viewY < 0)
    {
        viewY = 0;
    }
    if (viewY > config.height - viewHeight)
    {
        viewY = config.height - viewHeight;
    }

   
    for (int i = viewY; i < viewY + viewHeight && i < config.height; i++)
    {
        for (int j = viewX; j < viewX + viewWidth && j < config.width; j++)
        {
            std::cout << map[i][j];
        }
        std::cout << '\n';
    }


    std::cout << "\nCJ Health: [";
    int currentLife;
    if (player.life < 0)
    {
        currentLife = 0;
    }
    else
    {
        currentLife = player.life;
    }

    int maxLife;
    if (config.cjLife <= 0)
    {
        maxLife = 1;
    }
    else
    {
        maxLife = config.cjLife;
    }


    float lifeRatio = static_cast<float>(currentLife) / maxLife;

    int lifeBars = static_cast<int>(20 * lifeRatio);

    if (lifeBars < 0)
    {
        lifeBars = 0;
    }
    if (lifeBars > 20)
    {
        lifeBars = 20;
    }

    for (int i = 0; i < 20; i++)
    {
        std::cout << (i < lifeBars ? '#' : '-');
    }

    std::cout << "] " << currentLife << "/" << maxLife << std::endl;
    std::cout << " | Money: $" << player.money << std::endl;
    std::cout << " | Money to SAN FIERRO: $" << config.sanFierroToll << std::endl;
    std::cout << " | Money to  LAS VENTURAS: $" << config.lasVenturasToll << std::endl;

    // Informar si estas en un coche
    if (controlledCar != nullptr)
    {
        std::cout << "In car (WASD to move, E to exit)" << std::endl;
    }

    // Limpia la posicion del jugador si no esta en coche
    if (controlledCar == nullptr)
    {
        map[player.position.y][player.position.x] = ' ';
    }

    // Detecta si Big Smoke está vivo
    bool bigSmokeAlive = false;
    Position bigSmokePos;

    for (int i = 0; i < pedestriansCount; i++)
    {
        if (pedestrians[i].isBigSmoke && pedestrians[i].alive)
        {
            bigSmokeAlive = true;
            bigSmokePos = pedestrians[i].position;
            break;
        }
    }

    
}
