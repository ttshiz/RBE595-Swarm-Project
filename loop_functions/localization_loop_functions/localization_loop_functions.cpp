#include "localization_loop_functions.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

#include <argos3/plugins/robots/kheperaiv/control_interface/buzz_controller_kheperaiv.h>
#include <buzz/buzzvm.h>
#include <argos3/plugins/robots/kheperaiv/simulator/kheperaiv_entity.h>
/*#include <buzz/argos/buzz_controller_footbot.h>
#include <buzz/argos/buzz_controller.h> */

#include <list>
#include <sstream>
#include <string>

static const std::string QUEUEFILE        = "queue_";
static const std::string FILE_PREFIX      = "conv_";

/****************************************/
/****************************************/

CLocalizationLoopFunctions::CLocalizationLoopFunctions() :
  /**m_cLocalizationArenaSideX(-0.9f, 1.7f),
     m_cLocalizationArenaSideY(-1.7f, 1.7f),
  */
  m_pcFloor(NULL) {
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Init(TConfigurationNode& t_node) {
  try {
    /* Parse the configuration file */
    TConfigurationNode& tLocalization = GetNode(t_node, "localization");
    /* Get a pointer to the floor entity */
    m_pcFloor = &GetSpace().GetFloorEntity();
    /* Get the output file name from XML */
    GetNodeAttribute(tLocalization, "output", m_strOutput);
    Reset();
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions!", ex);
  }
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Reset() {
  OpenFile(m_cQueueOutFile, QUEUEFILE);
  OpenFile(m_cOutput,      FILE_PREFIX);
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::Destroy() {
  /* Close the file */
  CloseFile(m_cQueueOutFile);
  CloseFile(m_cOutput);
}

/****************************************/
/****************************************/

CColor CLocalizationLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
  if((c_position_on_plane.GetX() > -1.0f) && (c_position_on_plane.GetX() < 1.0f)
     && (c_position_on_plane.GetY() > -1.0f) && (c_position_on_plane.GetY() < 1.0f)) {
    return CColor::GRAY50;
  }
  return CColor::WHITE;
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::PostStep() {
  /* Output the current time step */
  m_cQueueOutFile << GetSpace().GetSimulationClock();
  m_cOutput << GetSpace().GetSimulationClock();
  /* Go through the robots */
  m_bDone = true;
  buzzvm_t tBuzzVM;
  buzzobj_t tObj;
  /* for each robot */
  CSpace::TMapPerType kmap = GetSpace().GetEntitiesByType("kheperaiv");
  m_cOutput << ", numKBots " << kmap.size();
  for(auto it = kmap.begin(); it != kmap.end(); ++it) {
    /* Get reference to the VM */
    tBuzzVM = dynamic_cast<CBuzzController&>(any_cast<CKheperaIVEntity*>(it->second)->GetControllableEntity().GetController()).GetBuzzVM();
    /* Make sure no error occurred in the script */
    if(tBuzzVM->state != BUZZVM_STATE_ERROR) {
      /* Output output queue size */
      m_cQueueOutFile << "," << buzzoutmsg_queue_size(tBuzzVM);
      /* Update VS data */ /*
      if(!m_vecDone[i]) {
	buzzvm_pushs(tBuzzVM, buzzvm_string_register(tBuzzVM, "est_location", 0));
	buzzvm_gload(tBuzzVM);
	tObj = buzzvm_stack_at(tBuzzVM, 1);
	buzzvm_pop(tBuzzVM);
	m_vecDone[i] =
	  tObj->o.type == BUZZTYPE_TABLE &&
	  tObj->i.value == m_vecControllers.size() - 1;
	m_bDone &= m_vecDone[i];
	m_cOutput << ","
		  << (tObj->o.type == BUZZTYPE_TABLE ? tObj->i.value : 0);
      }
      else {
	m_cOutput << "," << m_vecControllers.size() - 1;
	} 
			   */
    }
  }
  /* Close the record */
  m_cQueueOutFile << std::endl;
  m_cOutput << std::endl;
}

/****************************************/
/****************************************/
void CLocalizationLoopFunctions::OpenFile(std::ofstream& c_stream, const std::string& str_prefix) {
  /* Make filename */
  std::string strFName = str_prefix + m_strOutput;
  /* Close file and reopen it */
  CloseFile(c_stream);
  c_stream.open(strFName.c_str(),
		std::ofstream::out | std::ofstream::trunc);
  if(c_stream.fail())
    THROW_ARGOSEXCEPTION("Error opening \"" << strFName << "\": " << strerror(errno));
}

/****************************************/
/****************************************/

void CLocalizationLoopFunctions::CloseFile(std::ofstream& c_stream) {
  if(c_stream.is_open()) c_stream.close();
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CLocalizationLoopFunctions, "localization_loop_functions")
