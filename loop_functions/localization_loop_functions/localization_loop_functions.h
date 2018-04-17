#ifndef LOCALIZATION_LOOP_FUNCTIONS_H
#define LOCALIZATION_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/core/utility/math/range.h>

using namespace argos;

class CLocalizationLoopFunctions : public CLoopFunctions {

public:

   CLocalizationLoopFunctions();
   virtual ~CLocalizationLoopFunctions() {}

   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual CColor GetFloorColor(const CVector2& c_position_on_plane);
   
private:

   CRange<Real> m_cLocalizationArenaSideX, m_cLocalizationArenaSideY;
   CFloorEntity* m_pcFloor;
};

#endif
