
#ifndef IMANAGER_HPP
    #define IMANAGER_HPP

namespace raytracer {

typedef enum {
    WORKING,
    FINISH,
    WAIT,
    ASK,
} State;

class Core;

// Selecteur de mode (local / serveur / client). Le rendu CPU historique
// (Update qui calculait des rangees de pixels) a ete retire : le rendu passe
// sur GPU. Cette interface sera reetendue quand on branchera le multi.
class IManager {
public:
    virtual ~IManager() = default;

    virtual void InitCore(Core& core) = 0;
    virtual State GetState(void) = 0;
};

}

#endif
