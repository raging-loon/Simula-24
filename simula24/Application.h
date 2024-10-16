#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

#include <graphics/AppWindowMgr.h>
#include <core/error/error.h>
#include <graphics/tile/TileSheet.h>
#include <core/stl/smart_ptr.h>

#include <smcore/map/ObjectMap.h>
#include <objectmanager/ObjectTable.h>
#include <graphics/renderer/RenderManager.h>
#include <smcore/Simulation.h>
namespace simula24
{

class Application
{
public:

    Application();
    ~Application();

    Status init();
    void run();

private:

    void update();
    
private:
    bool m_shouldRun{ false };

    Simulation m_activeSim;
};


} // simula24

#endif // GAME_APPLICATION_H_