#ifndef LOCALIZATION_LOOP_FUNCTIONS_H
#define LOCALIZATION_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/core/utility/math/range.h>


// #include <argos3/plugins/robots/kheperaiv/control_interface/buzz_controller_kheperaiv.h>
#include <buzz/buzzvm.h>
#include <buzz/argos/buzz_controller_footbot.h>
#include <buzz/argos/buzz_controller.h> 

#include <list>
#include <sstream>
#include <string>


using namespace argos;

class CLocalizationLoopFunctions : public CLoopFunctions {

public:

   CLocalizationLoopFunctions();
   virtual ~CLocalizationLoopFunctions() {}

   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual CColor GetFloorColor(const CVector2& c_position_on_plane);

   virtual void PostStep();
   
private:

   CRange<Real> m_cLocalizationArenaSideX, m_cLocalizationArenaSideY;
   CFloorEntity* m_pcFloor;

   void OpenFile(std::ofstream& c_stream, const std::string& str_prefix);
   void CloseFile(std::ofstream& c_stream);

   std::string m_strOutput;
   std::ofstream m_cOutput;
   std::ofstream m_cQueueOutFile;
   bool headers_output;
};

#endif
