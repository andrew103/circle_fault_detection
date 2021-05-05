#ifndef KHEPERAIV_PROXIMITY_GROUP6_SENSOR_H
#define KHEPERAIV_PROXIMITY_GROUP6_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CKheperaIVProximityGroup6Sensor;
}

#include <argos3/plugins/robots/kheperaiv/control_interface/ci_kheperaiv_proximity_sensor.h>
#include <argos3/plugins/robots/generic/simulator/proximity_default_sensor.h>

namespace argos {

   class CKheperaIVProximityGroup6Sensor : public CCI_KheperaIVProximitySensor,
                                           public CSimulatedSensor {

   public:

      CKheperaIVProximityGroup6Sensor();

      virtual ~CKheperaIVProximityGroup6Sensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CProximityDefaultSensor* m_pcProximityImpl;

   };

}

#endif
