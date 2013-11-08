
#include "game/GameScene.hpp"
#include "game/GameHudLayer.hpp"
#include "game/GameMapView.hpp"
#include "game/GameBuilding.hpp"
#include "game/GameUnit.hpp"
#include "game/building/GameWall.hpp"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

GameScene* GameScene::create()
{
    GameScene* ret = new GameScene();
    if(ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameScene::init()
{
    addChild(_map.getView());
    _hud = GameHudLayer::create(_map);
    addChild(_hud);
    
    // test map
    
    for(unsigned i=0; i<20; i += 1)
    {
        _map.putEntity(Tile(19, 20+i), new GameWall());
        _map.putEntity(Tile(i, 20), new GameWall());
    }

    _map.putEntity(Tile(5, 35), new GameBuilding("enemy_headquarter_1", TileSize(4, 3)));

    _map.putEntity(Tile(18, 31), new GameBuilding("archer_tower_1", TileSize(2, 2)));
    _map.putEntity(Tile(8, 22), new GameBuilding("archer_tower_1", TileSize(2, 2)));
    _map.putEntity(Tile(18, 22), new GameBuilding("archer_tower_1", TileSize(2, 2)));
    _map.putEntity(Tile(5, 39), new GameBuilding("clan_portal_1", TileSize(2, 2)));
    _map.putEntity(Tile(5, 30), new GameBuilding("clan_portal_1", TileSize(2, 2)));
    _map.putEntity(Tile(8, 34), new GameBuilding("gold_storage_1", TileSize(3, 3)));
    
    
    for(unsigned i=0; i<10; ++i)
    {
        _map.putEntity(Tile(12, 25+i), new GameUnit("goblin_1"));
    }

    _map.putEntity(Tile(35, 5), new GameBuilding("headquarter_1", TileSize(4, 4)));
    
    for(unsigned i=0; i<10; ++i)
    {
        GameUnit* unit = new GameUnit("mage_1");
        unit->setOrientation(GameUnitOrientation::West);
        _map.putEntity(Tile(20+i, 10+i), unit);
        unit = new GameUnit("soldier_1");
        unit->setOrientation(GameUnitOrientation::West);
        _map.putEntity(Tile(20+i, 6+i), unit);
    }
    
    return true;
}