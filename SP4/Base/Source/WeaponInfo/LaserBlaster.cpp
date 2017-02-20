#include "LaserBlaster.h"
#include "Laser.h"

CLaserBlaster::CLaserBlaster()
{

}

CLaserBlaster::~CLaserBlaster()
{

}

// Initialise this instance to default values
void CLaserBlaster::Init()
{
    // Call the parent's Init method
    CWeaponInfo::Init();

    // The numbe rof ammunition in a magqazine for this weapon
    magRounds = 5;
    // The maximum number of ammunition for this magazine for this weapon
    maxMagRounds = 5;
    // The current total number of rounds currently carried by this player
    totalRounds = 1000;
    // The max total number of rounds currently carried by this player
    maxTotalRounds = 1000;

    // The time between shots
    timeBetweenShots = 0.1667;
    // The elapsed time (between shots)
    elapsedTime = 0.0;
    // Boolean flag to indicate if weapon can fire now
    bFire = true;
}

// Discharge this weapon
void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
    if (bFire)
    {
        // If there is still ammo in the magazine, then fire
        //if (magRounds > 0)
        {
			Vector3 _direction = (target - position).Normalized();
            // Create a laser with a laser mesh. The length is 10.0f, mesh is also set at 10.0f
            CLaser* aLaser = Create::Laser(
				"laser", 
				position,
				_direction,
				120.0f,
				2.0f,
				100.0f,
				_source);

			aLaser->SetMass(50);	// how hard the laser will hit / pushback
			aLaser->SetDamage(10);	// how much damage the laser will deal
            aLaser->SetCollider(true);
            aLaser->SetIsLaser(true);
            aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5), Vector3(-0.5f, -0.5f, -0.5f));
            bFire = false;
            //magRounds--;
        }
    }
}