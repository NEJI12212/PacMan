#include "PacTileMap.h"
namespace
{
    // txture size magic number
    constexpr float textureSize = 16.0f;
    // instance
    std::unique_ptr<PacTileMap> sInstance;
}


//----------------------------------------------------------------------------------
void PacTileMap::StaticInitialize()
{
    // create the map
    XASSERT(sInstance == nullptr, "TileMap already initialized!");
    sInstance = std::make_unique<PacTileMap>();
}

//----------------------------------------------------------------------------------
void PacTileMap::StaticTerminate()
{
    // clear out the instance
    sInstance.reset();
}

//----------------------------------------------------------------------------------
PacTileMap& PacTileMap::Get()
{
    // get the map instance
    XASSERT(sInstance != nullptr, "No tile map created!");
    return *sInstance;
}

//----------------------------------------------------------------------------------
void PacTileMap::Load()
{
    //get the stage text file
    std::string line;
    std::ifstream myFile("stage3.txt");
    unsigned int rows = 0;
    bool firstFlag = false;
    std::vector<int> tile;
    //zero number
    char tempChar = '0';
    int compareValue = static_cast<int>(tempChar);
    //read through the file
    if(myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            if (rows == 0)
                rows = line.length();
            else if (rows != line.length())
            {
                XASSERT(rows == line.length(), "ERROR: Uneven rows");
            }
            mColumns++;
            for (int i = 0; i < rows; i++)
            {
                char value = line[i];
                int intValue = static_cast<int>(value) - compareValue;
                tile.push_back(intValue);
            }
        }
        mRows = rows;
        myFile.close();
    }

    //apply the files data
    mTiles.reset(new int[mColumns * mRows]);
    for (int y = 0; y < mColumns; ++y)
    {
        for (int x = 0; x < mRows; ++x)
        {
            int i = GetIndex(x, y);
            mTiles.get()[i] = tile[i];
        }
    }
    //Open,
    mTilesTexture.push_back(X::LoadTexture("black3.png"));
    //Wall,
    mTilesTexture.push_back(X::LoadTexture("blue3.png"));
    //Ball
    mTilesTexture.push_back(X::LoadTexture("orb2.png"));
    //Power Orb
    mTilesTexture.push_back(X::LoadTexture("power_orb.png"));
    //White Border
    mTilesTexture.push_back(X::LoadTexture("white2.png"));
}

//----------------------------------------------------------------------------------
void PacTileMap::Render()
{
    // render the map
    for (int y = 0; y < mColumns; ++y)
    {
        for (int x = 0; x < mRows; ++x)
        {
            int i = GetIndex(x, y);
            X::TextureId id = mTilesTexture[mTiles.get()[i]];
            X::Math::Vector2 pos{ x * textureSize, y * textureSize };
            X::DrawSprite(id, pos, X::Pivot::TopLeft);
        }
    }
}

//----------------------------------------------------------------------------------
void PacTileMap::RenderOutside()
{
    // render the outer bounds so it will overlap the player/enemies for teleporting
    for (int y = 0; y < mColumns; ++y)
    {
        for (int x = 0; x < mRows; ++x)
        {
            if (y == 0 || y == mColumns -1 || x == 0 || x == mRows -1 || y == mColumns || x == mRows)
            {
                int i = GetIndex(x, y);
                X::TextureId id = mTilesTexture[mTiles.get()[i]];
                X::Math::Vector2 pos{ x * textureSize, y * textureSize };
                X::DrawSprite(id, pos, X::Pivot::TopLeft);
            }
        }
    }
}
//----------------------------------------------------------------------------------
void PacTileMap::Unload()
{
    // unload the map
    mTilesTexture.clear();
}

//----------------------------------------------------------------------------------
void PacTileMap::Update(float deltaTime)
{
    //update the power mode to see if the player is in a power up state
    mPowerMode = mPowerTimer > 0;
    mPowerTimer -= deltaTime;
}

int PacTileMap::GetIndex(int row, int column) const
{
    // get the tile
    return row + (column * mRows);
}

//----------------------------------------------------------------------------------
X::Math::Vector2 PacTileMap::GetMaxBoundaries() const
{
    // get the max boundaries of the map
    float removeHalfTexture = textureSize / 2;
    float x = (mRows * textureSize) - removeHalfTexture;
    float y = (mColumns * textureSize) - removeHalfTexture;
    return X::Math::Vector2{ x, y }; 
}

//----------------------------------------------------------------------------------
int PacTileMap::CheckPlayerCollision(const X::Math::LineSegment& lineSegment) const
{
    // get max/min of the X/Y values
    int startX = static_cast<int>(lineSegment.from.x / textureSize);
    int startY = static_cast<int>(lineSegment.from.y / textureSize);
    int endX = static_cast<int>(lineSegment.to.x / textureSize);
    int endY = static_cast<int>(lineSegment.to.y / textureSize);

    for (int x = startX; x <= endX; ++x)
    {
        for (int y = startY; y <= endY; ++y)
        {
            int index = GetIndex(x, y);
            mTeleport = (x == 0 || x == mRows);
            int tileValue = mTiles.get()[index];
            // if a point is grabbed change the texture
            if (mTiles.get()[index] == static_cast<int>(TileTypes::POWERORB) ||
                mTiles.get()[index] == static_cast<int>(TileTypes::BALL))
            {
                mTiles.get()[index] = static_cast<int>(TileTypes::OPEN);
            }
            return tileValue;
        }
    }
    return static_cast<int>(TileTypes::WALL);
}

//----------------------------------------------------------------------------------
bool PacTileMap::CheckCollision(const X::Math::LineSegment& lineSegment) const
{
    // get max/min of the X/Y values
    int startX = static_cast<int>(lineSegment.from.x / textureSize);
    int startY = static_cast<int>(lineSegment.from.y / textureSize);
    int endX = static_cast<int>(lineSegment.to.x / textureSize);
    int endY = static_cast<int>(lineSegment.to.y / textureSize);

    for (int x = startX; x <= endX; ++x)
    {
        for (int y = startY; y <= endY; ++y)
        {
            int index = GetIndex(x, y);
            if (mTiles.get()[index] == 1) // Wall
                return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------
bool PacTileMap::HitEnemy(const X::Math::Rect player, const X::Math::Rect enemy) const
{
    // see if a player and an enemy collide
    return X::Math::Intersect(player, enemy);
}