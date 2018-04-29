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
  headers_output = false;
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
  CSpace::TMapPerType kmap = GetSpace().GetEntitiesByType("kheperaiv");
  
  if (headers_output == false){
    headers_output = true;
    m_cOutput << "time_step";
    for(auto it = kmap.begin(); it != kmap.end(); ++it) {
      std::string id = any_cast<CKheperaIVEntity*>(it->second)->GetId();
      m_cOutput << "," << id << "_X" << "," << id << "_Y"
		<< "," << id << "_X_EST" << "," << id << "_Y_EST"
		<< "," << id << "_X_AVG" << "," << id << "_Y_AVG"
		<< "," << id << "_X_MIN" << "," << id << "_Y_MIN"
		<< "," << id << "_X_MAX" << "," << id << "_Y_MAX"
		<< "NUM_GPSBOTS" << "FRAC_MOVING_GPSBOTS"
		<< "NUM_MOVING_GPSBOTS" << "NUM_CONNECTED_GPSBOTS";
    }
    m_cOutput << std::endl;
  }
 
  /* Output the current time step */
  m_cQueueOutFile << GetSpace().GetSimulationClock();
  m_cOutput << GetSpace().GetSimulationClock();
  /* Go through the robots */
  buzzvm_t tBuzzVM;
  buzzobj_t tObj;
  /* for each robot */
  /*m_cOutput << ", numKBots " << kmap.size(); */
  for(auto it = kmap.begin(); it != kmap.end(); ++it) {
    /* Get reference to the VM */
    tBuzzVM = dynamic_cast<CBuzzController&>(any_cast<CKheperaIVEntity*>(it->second)->GetControllableEntity().GetController()).GetBuzzVM();
    /* Make sure no error occurred in the script */
    if(tBuzzVM->state != BUZZVM_STATE_ERROR) {
      /* Output output queue size */
      m_cQueueOutFile << "," << buzzoutmsg_queue_size(tBuzzVM);
      const char *vars[] = {"X", "Y","X_EST", "Y_EST", "X_AVG", "Y_AVG", "X_MIN", "Y_MIN", "X_MAX"
			    , "Y_MAX", "FRAC_MOVING_GPSBOTS"};
      m_cOutput << ",";
      for(int v = 0; v < 11; v++) {
	buzzvm_pushs(tBuzzVM, buzzvm_string_register(tBuzzVM, vars[v], 0));
	buzzvm_gload(tBuzzVM);
	tObj = buzzvm_stack_at(tBuzzVM, 1);
	buzzvm_pop(tBuzzVM);
	if (tObj->o.type == BUZZTYPE_FLOAT){
	  /*m_cOutput << vars[v] << "=" << tObj->f.value;*/
	  m_cOutput << tObj->f.value;
	  if(v!=11) {
	    m_cOutput << ",";
	  }
	} else {
	  printf("ERROR, %s was a %d", vars[v], tObj->o.type);
	}
      }
      m_cOutput << ",";
      const char *int_vars[] = {"NUM_GPSBOTS", "NUM_MOVING_GPSBOTS", "NUM_CONNECTED_GPSBOTS"};
      for(int v = 0; v < 3; v++) {
	buzzvm_pushs(tBuzzVM, buzzvm_string_register(tBuzzVM, int_vars[v], 0));
	buzzvm_gload(tBuzzVM);
	tObj = buzzvm_stack_at(tBuzzVM, 1);
	buzzvm_pop(tBuzzVM);
	if (tObj->o.type == BUZZTYPE_INT){
	  /*m_cOutput << vars[v] << "=" << tObj->f.value;*/
	  m_cOutput << tObj->i.value;
	  if(v!=3) {
	    m_cOutput << ",";
	  }
	} else {
	  printf("ERROR, %s was a %d", int_vars[v], tObj->o.type);
	}
      }
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
