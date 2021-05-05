#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>

#include "kheperaiv_proximity_group6_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   class CKheperaIVProximityGroup6SensorImpl : public CProximityDefaultSensor {

   public:

      void Init(TConfigurationNode& t_tree) override {
         /* Parse original attributes */
         CProximityDefaultSensor::Init(t_tree);
         /* Parse the range attribute */
         GetNodeAttribute(t_tree, "range", m_fRange);
         /* Go through the sensors */
         for(auto itSensor = m_pcProximityEntity->GetSensors().begin();
             itSensor != m_pcProximityEntity->GetSensors().end();
             ++itSensor) {
            /* Change the range */
            (*itSensor)->Direction.Normalize();
            (*itSensor)->Direction *= m_fRange;
         }
      }

      void SetRobot(CComposableEntity& c_entity) override {
         try {
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
            m_pcProximityEntity = &(c_entity.GetComponent<CProximitySensorEquippedEntity>("proximity_sensors[proximity]"));
            m_pcProximityEntity->Enable();
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the Khepera IV proximity group6 sensor", ex);
         }
      }

      Real CalculateReading(Real f_distance) override {
         /* Feel free to change this function! */
         return Exp(-f_distance);
      }

   protected:

      Real m_fRange;

   };

   /****************************************/
   /****************************************/

   CKheperaIVProximityGroup6Sensor::CKheperaIVProximityGroup6Sensor() :
      m_pcProximityImpl(new CKheperaIVProximityGroup6SensorImpl()) {}

   /****************************************/
   /****************************************/

   CKheperaIVProximityGroup6Sensor::~CKheperaIVProximityGroup6Sensor() {
      delete m_pcProximityImpl;
   }

   /****************************************/
   /****************************************/

   void CKheperaIVProximityGroup6Sensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcProximityImpl->SetRobot(c_entity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the Khepera IV proximity group6 sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CKheperaIVProximityGroup6Sensor::Init(TConfigurationNode& t_tree) {
      m_pcProximityImpl->Init(t_tree);
   }

   /****************************************/
   /****************************************/

   void CKheperaIVProximityGroup6Sensor::Update() {
      m_pcProximityImpl->Update();
      for(size_t i = 0; i < m_pcProximityImpl->GetReadings().size(); ++i) {
         m_tReadings[i].Value = m_pcProximityImpl->GetReadings()[i];
      }
   }

   /****************************************/
   /****************************************/

   void CKheperaIVProximityGroup6Sensor::Reset() {
      m_pcProximityImpl->Reset();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CKheperaIVProximityGroup6Sensor,
                   "kheperaiv_proximity", "group6",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The Khepera IV proximity sensor adapted for Group 6.",
                   "This sensor accesses the Khepera IV proximity sensor. For a complete description\n"
                   "of its usage, refer to the ci_kheperaiv_proximity_sensor.h interface. For the XML\n"
                   "configuration, refer to the default proximity sensor.\n",
                   "Usable"
		  );

}
