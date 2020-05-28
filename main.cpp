#include <ncurses.h>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <iostream>
#include "Car.h"
#include "Tank.h"
#include "DeliveryCar.h"
#include "Stall.h"

int CARS_NUMBER;
int PUMPS_NUMBER = 2;
int PLACES_NUMBER = 5;
int CHECKOUT_NUMBER = 2;

// zmienne do ncurses
#define AVAILABLE_PAIR 1
#define UNAVAILABLE_PAIR 2
bool running;
bool deliveryCar_running = true;

Tank* tank;
DeliveryCar* deliveryCar;
std::vector<Car> cars;
std::vector<Stall> pumps;
std::vector<Stall> parking_places;

bool running_screen = true;
std::mutex m;

// wątek odświeżania ekranu
void refresh_screen()
{
    int tank_max = tank->get_max_volume();
    char buffer[50];
    std::vector<int> positions;
    for(int i = 0; i< CARS_NUMBER; i++)
    {
        if(i%2 != 0)
        {
            positions.push_back(i*10);
        }
        else
        {
            positions.push_back(50-i*10);
        }
        
    }
    while(true)
    {
        clear();
        
        // fuel status
        mvprintw(0, 0, "Fuel left:");
        std::sprintf(buffer,"%d/%d", tank->get_current_volume(), tank_max);
        mvprintw(0, 11, buffer);

        // delivery car
        mvprintw(1,0, "Delivery car: ");
        int deliveryCar_status = deliveryCar->get_status();
        if(deliveryCar_status == 1)
        {
            mvprintw(1, 25, "Waiting...");
        }
        else if (deliveryCar_status == 0)
        {
            mvprintw(1, 25, "Refueling...");
        }
        else if (deliveryCar_status == 4)
        {
            mvprintw(1, 25, "Parking...");
        }
        std::sprintf(buffer, "%d", deliveryCar->get_progress());
        mvprintw(1, 50, buffer);
        int pnumber = deliveryCar->get_parking();
        if(deliveryCar->get_parking() != -1)
        {
            mvprintw(2, 80+(pnumber*3), "D");
        }

        //cars
        mvprintw(2,0, "Cars: ");
        int not_riding_cars = 0;
        for(int i = 0; i<CARS_NUMBER; i++)
        {
            
           
            
            std::sprintf(buffer, "Car%d:", i+1);
            mvprintw(2+i, 0, buffer);
            if(cars[i].get_status() == 0)
            {
                mvprintw(2+i, 25, "Refueling...");
                not_riding_cars++;
            }
            else if (cars[i].get_status() == 1)
            {
                mvprintw(2+i, 25, "Riding...");
            }
            else if (cars[i].get_status() == 2)
            {
                mvprintw(2+i, 25, "Waiting for a pump...");
                not_riding_cars++;
            }
            std::sprintf(buffer, "%d", cars[i].get_progress());
            mvprintw(2+i, 50, buffer);
            int pump_number = cars[i].get_pump();
            int parking_place_number = cars[i].get_parking();
            if(parking_place_number != -1)
            {
                std::sprintf(buffer, "%d", i);
                mvprintw(2, 80+(parking_place_number*3), buffer);
            }

            int ticket = cars[i].get_ticket();
            if(ticket != -1)
            {
                std::sprintf(buffer, "Ticket:%d", cars[i].get_ticket());
                mvprintw(2+i, 57, buffer);
                if(cars[i].get_chosen_pump() == 0)
                {
                    int offset1 = cars[i].get_ticket() - pumps[0].print_current_ticket();
                    std::sprintf(buffer, "%d", i+1);
                    mvprintw(14+0, 20+offset1, buffer);

                }
                if(cars[i].get_chosen_pump() == 1)
                {
                    int offset2 = cars[i].get_ticket() - pumps[1].print_current_ticket();
                    std::sprintf(buffer, "%d", i+1);
                    mvprintw(14+1, 20+offset2, buffer);
                }
               
            }

            if(positions[i] > 100)
                positions[i] = 0;
            if(positions[i] < 0)
                positions[i] = 100;

            std::sprintf(buffer, "%d:", i+1);
            if(i%2 == 0)
            {
                positions[i]--;
                if(cars[i].get_status() != 0)
                {
                    mvprintw(9, positions[i], buffer);
                }
            } 
            else
            {
                positions[i]++;
                if(cars[i].get_status() != 0)
                {
                    mvprintw(11, positions[i], buffer);
                }
            }

            
        }
        mvprintw(0, 80, "PARKING");
        for(int i = 0; i<PLACES_NUMBER; i++)
        {
            std::sprintf(buffer, "P%d", i);
            mvprintw(1, 80+(i*3), buffer);
        }

        for(int i = 0; i< PUMPS_NUMBER; i++)
        {
            int current_ticket = pumps[i].print_current_ticket();
            std::sprintf(buffer, "Pump:%d, Ticket:%d", i, pumps[i].print_current_ticket());
            mvprintw(14+i, 0, buffer);
        }

        for(int i = 0; i < 100; i++)
        {
            mvprintw(8, 0+i, "#");
            mvprintw(10, 0+i, "-");
            mvprintw(12, 0+i, "#");
        }

        refresh();
        usleep(10000);
        m.lock();
        if(running_screen == false)
            break;
        m.unlock();
    }
}

void input()
{
    char ch = getch();
    bool running_input = true;
    while(running_input)
    {
        switch(ch)
        {
            case 'q':
                running_input = false;
                m.lock();
                running = false;
                m.unlock();
                break;
            default:
                break;
        }
    }

}

int main(int argc, char *argv[])
{
    
    CARS_NUMBER = atoi(argv[1]);
    std::vector<std::thread> car_threads;
    // initializing ncurses
    initscr();
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(AVAILABLE_PAIR, COLOR_GREEN, -1);
    init_pair(UNAVAILABLE_PAIR, COLOR_RED, -1);
    running = true;
    //inicjalizacja cysterny
    tank = new Tank(500);

    //inicjalizacja dystrybutorow
    for(int i = 0; i<PUMPS_NUMBER; i++)
    {
        pumps.push_back(Stall(i));
    }

    //inicjalizacja miejsc parkingowych
    for(int i = 0; i<PLACES_NUMBER; i++)
    {
        parking_places.push_back(Stall(i));
    }

    // inicjalizacja samochodow
    for(int i = 0; i<CARS_NUMBER; i++) 
    {
        cars.push_back(Car(i, tank, pumps, parking_places));
    }

    // inicjalizacja samochodu dostawczego
    deliveryCar = new DeliveryCar(tank, parking_places);
    std::thread deliveryCar_thread = deliveryCar->live_thread();

    // inicjalizacja watkow samochodow
    for(int i = 0; i<CARS_NUMBER; i++)
    {
        car_threads.push_back(cars[i].live_thread());
    }

    // start watkow obrazu i inputu
    std::thread screen(refresh_screen);
    std::thread handle_input(input);

    // joining threads
    handle_input.join();
    if(!running)
    {
         for(int i = 0; i<CARS_NUMBER; i++)
         {
            car_threads[i].join();
         }
    }
    deliveryCar_running = false;
    deliveryCar_thread.join();

    m.lock();
    running_screen = false;
    m.unlock();
    screen.join();

    endwin();

    return 0;




}