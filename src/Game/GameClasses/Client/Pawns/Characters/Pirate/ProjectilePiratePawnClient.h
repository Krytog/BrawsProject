#pragma once

#include <Game/GameClasses/Client/Pawns/Projectiles/ProjectilePawnClient.h>

class ProjectilePiratePawnClient : public ProjectilePawnClient {
public:
    ProjectilePiratePawnClient();
    ~ProjectilePiratePawnClient() override;

    static const size_t kTypeId;

    class Mark : public PersistentVisibleObject {
    public:
        Mark();
        ~Mark();

        void UpdateRotation(const Vector2D& rotator);

        static const size_t kTypeId;
    };
};
