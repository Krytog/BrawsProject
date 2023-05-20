#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "Porter.h"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);
    uint64_t id;
    size_t reply_length = boost::asio::read(s, boost::asio::buffer(&id, sizeof(id)));
    std::cout << "Reply is: ";
    std::cout << id << std::endl;

    Request request = {.type = CreateNewGame, .id = 10, .character_type = Character::PIRATE};
    boost::asio::write(s, boost::asio::buffer(&request, sizeof(request)));
    LobbySettings settings = {.users_count = 1};
    boost::asio::write(s, boost::asio::buffer(&settings, sizeof(settings)));
    uint64_t lobby_id;
    boost::asio::read(s, boost::asio::buffer(&lobby_id, sizeof(lobby_id)));
    std::cout << lobby_id;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}