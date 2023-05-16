#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <NormInfrastructure/Client/Communicator.h>
#include <SwarmSystem/Profiler/Profiler.h>


int main() {
    ClientEngine& engine = ClientEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    communicator.RegOnServer();
    communicator.Run();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();

    Profiler& profiler = Profiler::GetInstance();

    while (engine.IsActive()) {
        time.ResetTime();

        auto data = communicator.ReceiveFromServer();
        std::cout << "PACKET SIZE: " << data.size() << std::endl;
        std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - profiler.ExtractTimeMark(&data);
        std::cout << "FROM SERVER " << time_interval.count() * 1000 << "ms" << std::endl;

        if (data[0] == '#') { // means that we probably have a good package
            overmind.ActualizeCerebrates(data);
            overmind.ForceCerebratesExecuteCommands(data);
        }

        engine.Update();

//        overmind.DebugInfo();
//        engine.DebugInfo();

        auto for_server = ClientGameManagement::SerializeInput();
        profiler.AddTimeMark(&for_server);
        communicator.SendToServer(for_server);

        std::this_thread::sleep_for (std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime()))));
    }
    return 0;
}

//int main() {
//    Communicator& communicator = Communicator::GetInstance();
//    Profiler& profiler = Profiler::GetInstance();
//    communicator.RegOnServer();
//    MyTime time;
//    const std::string data = "abobabobabobabobabobabobabobabobabobabobabo";
//    while (true) {
//        time.ResetTime();
//
//        auto package = data;
//        profiler.AddTimeMark(&package);
//        communicator.SendToServer(package);
//
//        auto from = communicator.ReceiveFromServer();
//        std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - profiler.ExtractTimeMark(&from);
//        std::cout << "FROM SERVER " << time_interval.count() * 1000 << "ms" << std::endl;
//
//        std::cout << from << std::endl;
//
//        std::this_thread::sleep_for (std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime()))));
//    }
//}