//Hokuyo Laser functions
//Authors: S. Diane, ...
//Created: 23.10.2014

#include "urg/Urg_driver.h"
#include "urg/Connection_information.h"
#include "urg/math_utilities.h"

using namespace qrk;
using namespace std;

class HokuyoLaser
{
	public:

	Urg_driver *urg;

	HokuyoLaser(int argc, char *argv[]);//конструктор, инициализация
	~HokuyoLaser();//деструктор
	vector<long> GetValues();
};
