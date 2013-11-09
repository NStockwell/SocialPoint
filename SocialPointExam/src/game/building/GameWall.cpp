
#include "game/building/GameWall.hpp"
#include "game/GameBuildingView.hpp"

GameWall::GameWall() :
GameBuildingDestroyable("wall_1", 15, TileSize(1, 1))
{
}

GameWall::~GameWall()
{
}

void GameWall::onTile(const Tile& tile)
{
    Tile tleft(tile.x-getTileSize().width, tile.y);
    Tile tright(tile.x, tile.y-getTileSize().height);
    bool left = false;
    bool right = false;
    
    if(getMap().isTileOccupied(tleft))
    {
        GameEntity& eleft = getMap().getTileEntity(tleft);
        if(dynamic_cast<GameWall*>(&eleft))
        {
            left = true;
        }
    }
    
    if(getMap().isTileOccupied(tright))
    {
        GameEntity& eright = getMap().getTileEntity(tright);
        if(dynamic_cast<GameWall*>(&eright))
        {
            right = true;
        }
    }
    
    unsigned index = 1;
    
    if(left && right)
    {
        index = 4;
    }
    else if(right)
    {
        index = 3;
    }
    else if(left)
    {
        index = 2;
    }

    getBuildingView()->setBaseIndex(index);
    GameEntity::onTile(tile);
}
