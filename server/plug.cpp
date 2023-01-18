#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main() {
    sf::TcpSocket socket;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();

    char type;
    while (true) {
        std::cout << "Enter working type:\n" << "s -- server\n" << "c -- client" << std::endl;
        std::cin >> type;

        if (type == 'c' || type == 's') {
            break;
        }
        std::cout << "Wrong input. Just type 'c' or 's'" << std::endl;
    }

    if (type == 's') { // server
        sf::TcpListener listener;
        listener.listen(2000);

        std::cout << "server started\n" << "server ip: " << ip << std::endl;
        while (listener.accept(socket) != sf::Socket::Done) {
            std::cout << "Connection error, try again" << std::endl;
        }
        std::cout << "Client Connected" << std::endl;

        socket.setBlocking(false);
        sf::Packet packet;
        std::string message;

        while (true) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                std::cin >> message;
            }
            if (!message.empty()) {
                packet.clear();
                packet << message;
                socket.send(packet);
                message.clear();
            }
            if (socket.receive(packet) == sf::Socket::Done) {
                std::string message;
                packet >> message;
                std::cout << "Client: " << message << std::endl;
            }
        }

    } else { // client
        while (true) {
            std::cout << "Enter server ip" << std::endl;
            std::cin >> ip;
            if (socket.connect(ip, 2000) == sf::Socket::Done) {
                std::cout << "Connected successfully" << std::endl;
                break;
            }
            std::cout << "Connection error, try again" << std::endl;
        }

        socket.setBlocking(false);
        sf::Packet packet;
        std::string message;

        while (true) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                std::cin >> message;
            }
            if (!message.empty()) {
                packet.clear();
                packet << message;
                socket.send(packet);
                message.clear();
            }
            if (socket.receive(packet) == sf::Socket::Done) {
                packet >> message;
                std::cout << "Server: " << message << std::endl;
            }
        }

    }
    return 0;
}