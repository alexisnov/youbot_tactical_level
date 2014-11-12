//Hokuyo Laser functions (implementation)
//Authors: S. Diane, ...
//Created: 23.10.2014

#include "includes.h"

//деструктор
HokuyoLaser::~HokuyoLaser()
{
delete[] urg;
}

//конструктор
HokuyoLaser::HokuyoLaser(int argc, char *argv[])
{
Connection_information information(argc, argv);
urg=new Urg_driver();
  if (!urg->open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg->what() << endl;
        return;
    }
}

vector<long> HokuyoLaser::GetValues()
{

  urg->set_scanning_parameter(urg->deg2step(-45), urg->deg2step(+45), 3);

    urg->start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);

 vector<long> data;
        long time_stamp = 0;
        if (!urg->get_distance(data, &time_stamp)) {
            cout << "Urg_driver::get_distance(): " << urg->what() << endl;
            return vector<long>();
        }

 urg->stop_measurement();
    return data;
}