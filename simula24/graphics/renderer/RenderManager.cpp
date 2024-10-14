#include "RenderManager.h"

#include <SDL.h>
#include <SDL_image.h>

#include <core/log/log.h>
#include <core/error/error.h>

#include <graphics/tile/TileConfig.h>
#include <graphics/renderer/Camera.h>
#include <graphics/tile/SheetLoader.h>
#include <graphics/tile/TileSheetParser.h>

#include <smcore/map/ObjectMap.h>
#include <smcore/entity/CrewMember.h>

#include <objectmanager/ObjectManager.h>

#include <cassert>
using simula24::RenderManager;
using simula24::TileSheetParser;
using simula24::TileConfig;
using simula24::Status;
using simula24::CrewMember;

RenderManager RenderManager::s_instance;

RenderManager::RenderManager()
    : m_mainWindow(nullptr), m_tileDB{}, m_camera(nullptr)
{
}

Status RenderManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        ENGINE_CRITICAL("Failed to initialize SDL: %s", SDL_GetError());
        return ERR_LIB_INIT;
    }

    ENGINE_INFO("SDL Initialized");

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        ENGINE_ERROR("Failed to initializes SDL_Image: %s", IMG_GetError());
        return ERR_LIB_INIT;
    }

    ENGINE_INFO("SDL_image Initialized");


    return OK;
}

Status RenderManager::terminate()
{
    IMG_Quit();
    SDL_Quit();
    return OK;
}

void RenderManager::setWindow(AppWindow* win)
{
    assert(win);
    m_mainWindow = win;
}

void RenderManager::addTileSheet(const stl::string& configLoc)
{
    SheetLoader loader(m_tileDB, m_mainWindow->getTextureManager());
    loader.loadWorldTiles(configLoc);
}

void RenderManager::renderFromObjectMap(const ObjectMap& om)
{
    for (int i = 0; i < om.getSizeX(); i++)
    {
        for (int j = 0; j < om.getSizeY(); j++)
        {

            int id = om.get(i, j);
            if (id == -1)
                continue;

            int height = OM::getObjectTable().getTileSize(id);

         
            SDL_Texture* tex = m_tileDB.worldTiles[height].getTexture();
            const SDL_Rect* d = &m_tileDB.worldTiles[height].getTile(
                OM::getObjectTable().getTileId(
                    id
                )
            );
            
            SDL_Rect g;
            int h, w;
            if (height == 0)
            {
                h = 8; w = 16;
                g.y = (i + j) * (h / 2);// -(height * 4) * d->h;

            }
            else
            {
                h = 16; w = 16;
                g.y = (i + j) * (8 / 2) - height * 8;
    
            }
            g.w = w;
            g.h = h;
            g.x = (i - j) * (w / 2);
            g.x += m_camera->getX();
            g.y += m_camera->getY();
            m_mainWindow->copyTexture(tex, d, &g);
        }

    }
}

void RenderManager::renderCivilianList(const stl::array<CrewMember>& cl)
{
    for (int i = 0; i < cl.size(); i++)
    {
        auto& loc = cl[i].getLocation();
        SDL_Rect location = {
            .x = (loc.x - loc.y) * (16 / 2) ,
            .y = (loc.x + loc.y) * (8 / 2),
            .w = 16, 
            .h = 8
        };
        location.x += m_camera->getX();
        location.y += m_camera->getY();
        m_mainWindow->copyTexture(m_tileDB.worldTiles[0].getTexture(), &m_tileDB.worldTiles[0].getTile(1), &location);

    }
}
