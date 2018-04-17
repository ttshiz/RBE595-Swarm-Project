#include "localization_loop_functions.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

/****************************************/
/****************************************/

CLocalizationLoopFunctions::CLocalizationLoopFunctions() :
   m_cLocalizationArenaSideX(-0.9f, 1.7f),
   m_cLocalizationArenaSideY(-1.7f, 1.7f),
   m_pcFloor(NULL) {
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Init(TConfigurationNode& t_node) {
   try {
     TConfigurationNode& tLocalization = GetNode(t_node, "localization");
      /* Get a pointer to the floor entity */
      m_pcFloor = &GetSpace().GetFloorEntity();
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions!", ex);
   }
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Reset() {
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Destroy() {
}

/****************************************/
/****************************************/

CColor CLocalizationLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
   if(c_position_on_plane.GetX() < -1.0f) {
      return CColor::GRAY50;
   }
   return CColor::WHITE;
   m_pcFloor->SetChanged();
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CLocalizationLoopFunctions, "localization_loop_functions")
