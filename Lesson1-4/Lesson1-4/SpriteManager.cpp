#include "SpriteManager.h"
#include "Sky.h"

SpriteManager::SpriteManager(LPDIRECT3DDEVICE9 _d3ddev)
{
	//Just for use. Direct3D is resposible to destroy it.
	d3ddev = _d3ddev;
}

SpriteManager::~SpriteManager()
{
	for (auto it = spriteVec.begin(); it != spriteVec.end(); ++it) {
		delete *it;
	}
	spriteVec.clear();

	delete rotater;
}

void SpriteManager::initSprites(const D3DXVECTOR3 &center, Terrain *pTerrain, Camera *camera)
{
	Snowman *snowman = new Snowman();
	snowman->init(d3ddev, center);
	spriteVec.push_back(snowman);

	Box *box = new Box();
	box->init(d3ddev, center);
	spriteVec.push_back(box);

	StandingSnowman *standingSnowman = new StandingSnowman();
	standingSnowman->init(d3ddev, center);
	spriteVec.push_back(standingSnowman);

	Sky *sky = new Sky();
	sky->init(d3ddev, center, pTerrain, camera);
	spriteVec.push_back(sky);
	
	rotater = new Rotater(pTerrain);
	Rotatable *pr = dynamic_cast<Rotatable *>(snowman);
	pr->setRotater(rotater);
	pr = dynamic_cast<Rotatable *>(box);
	pr->setRotater(rotater);

}

void SpriteManager::renderAll()
{
	for (auto it = spriteVec.begin(); it != spriteVec.end(); ++it) {
		(*it)->render(d3ddev);
	}
}