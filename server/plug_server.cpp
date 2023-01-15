#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <ncurses.h>

int main()
{
    sf::TcpSocket socket;

    char type;

    while (true)
    {
        std::cout << "Choose type of connection:\n"
                  << "C -- for client\n"
                  << "S -- for server\n";
        std::cin >> type;

        if (type == 's' || type == 'c')
        {
            break;
        }
        std::cout << "Incorrect input, try again" << std::endl;
    }

    if (type == 's')
    {
        std::cout << "Ok, here the IP:" << sf::IpAddress::getPublicAddress() << std::endl;
        sf::TcpListener listener;
        listener.listen(2000);

        if (listener.accept(socket) != sf::Socket::Done)
        {
            std::cout << "Error! Cannot accept client\n";
        }
    }
    else
    {
        sf::IpAddress ip;
        std::cout << "Input server ip:" << std::endl;
        std::cin >> ip;
        if (socket.connect(ip, 2000) != sf::Socket::Done)
        {
            std::cout << "Error! Cannot connect to server\n";
        }
    }

    std::string name;
    std::cout << "Enter your name:\n";
    std::cin >> name;

    socket.setBlocking(false);

    std::string message = "";
    sf::Packet packet;

    initscr();
    cbreak;
    noecho;
    keypad(stdscr, TRUE);

    while (true)
    {
        int key = getch();
        if (key == 27)
        {
            std::cout << "print your message\n";
            std::cin >> message;

            packet.clear();
            packet << name << message;

            socket.send(packet);

            message = "";
        }

        if (socket.receive(packet) == sf::Socket::Done)
        {
            std::string nameRec;
            std::string messageRec;

            packet >> nameRec >> messageRec;
            std::cout << nameRec << ":" << messageRec << std::endl;
        }
    }

    endwin();
    return 0;
}