#include "project.h"
#include "buzz/buzzvm.h"

/****************************************/
/****************************************/

/**
 * Functor to get data from the robots
 */
struct GetRobotData : public CBuzzLoopFunctions::COperation {

   /** Constructor */
   GetRobotData(int n_tasks) : m_vecTaskCounts(n_tasks, 0) {}

   /** The action happens here */
   virtual void operator()(const std::string& str_robot_id,
                           buzzvm_t t_vm) {
      /* Get the current task */
    //   buzzobj_t tFaultDetected = BuzzGet(t_vm, "fault_detected");
    //   /* Make sure it's the type we expect (an integer) */
    //   if(!buzzobj_isint(tFaultDetected)) {
    //      LOGERR << str_robot_id << ": variable 'task' has wrong type " << buzztype_desc[tFaultDetected->o.type] << std::endl;
    //      return;
    //   }
    //   /* Get the value */
    //   int nFaultDetected = buzzobj_getint(tFaultDetected);
      /* Get the current thresholds */
    //   BuzzTableOpen(t_vm, "close_neighbors");
    //   BuzzTableOpenNested(t_vm, "front");
    //   buzzobj_t tFrontRID = BuzzTableGet(t_vm, "rid");
    //   BuzzTableCloseNested(t_vm, "front");
    //   BuzzTableOpenNested(t_vm, "back");
    //   buzzobj_t tBackRID = BuzzTableGet(t_vm, RID);

    //   buzzobj_t tCloseNeighbors = BuzzGet(t_vm, "close_neighbors");
    //   buzzobj_t tFrontNeighbor = BuzzGet(t_vm, "front");
    //   buzzobj_t tBackNeighbor = BuzzGet(t_vm, "back");

      /* Make sure it's the type we expect (a table) */
    //   if (!buzzobj_istable(tCloseNeighbors)) {
    //      LOGERR << str_robot_id << ": variable 'close_neighbors' has wrong type " << buzztype_desc[tCloseNeighbors->o.type] << std::endl;
    //      return;
    //   } else if (!buzzobj_istable(tFrontNeighbor)){
    //         LOGERR << str_robot_id << ": variable 'front_neighbors' has wrong type " << buzztype_desc[tFrontNeighbor->o.type] << std::endl;
    //   } else if (!buzzobj_istable(tBackNeighbor)){
    //         LOGERR << str_robot_id << ": variable 'back_neighbors' has wrong type " << buzztype_desc[tBackNeighbor->o.type] << std::endl;
    //   } else {
    //         LOGERR << str_robot_id <<  ": all tables loaded in" << std::endl;
    //   }
      /* Get the values */
    //   m_vecRobotsThresholds[t_vm->robot].resize(m_vecTaskCounts.size(), 0.0);
    //   for(int i = 0; i < m_vecTaskCounts.size(); ++i) {
    //      /* Get the object */
    //      buzzobj_t tThresholdValue = BuzzTableGet(t_vm, i);
    //      /* Make sure it's the type we expect (a float) */
    //      if(!buzzobj_isfloat(tThresholdValue)) {
    //         LOGERR << str_robot_id << ": element 'threshold[" << i << "]' has wrong type " << buzztype_desc[tThresholdValue->o.type] << std::endl;
    //      }
    //      else {
    //         /* Get the value */
    //         float fThresholdValue = buzzobj_getfloat(tThresholdValue);
    //         /* Set the mapping */
    //         m_vecRobotsThresholds[t_vm->robot][i] = fThresholdValue;
    //      }
    //   }
   }

   /** Task counter */
   std::vector<int> m_vecTaskCounts;
   /* Task-robot mapping */
   std::map<int,int> m_vecRobotsTasks;
   /* Robot-threshold mapping */
   std::map<int,std::vector<float> > m_vecRobotsThresholds;
};

/****************************************/
/****************************************/

/**
 * Functor to put the stimulus in the Buzz VMs.
 */
struct PutStimuli : public CBuzzLoopFunctions::COperation {

   /** Constructor */
   PutStimuli(const std::vector<Real>& vec_stimuli) : m_vecStimuli(vec_stimuli) {}
   
   /** The action happens here */
   virtual void operator()(const std::string& str_robot_id,
                           buzzvm_t t_vm) {
      /* Set the values of the table 'stimulus' in the Buzz VM */
        BuzzTableOpen(t_vm, "stimulus");
        for(int i = 0; i < m_vecStimuli.size(); ++i) {
            BuzzTablePut(t_vm, i, static_cast<float>(m_vecStimuli[i]));
        }
        BuzzTableClose(t_vm);
   }

   /** Calculated stimuli */
   const std::vector<Real>& m_vecStimuli;
};

/****************************************/
/****************************************/

void CFaultDetection::Init(TConfigurationNode& t_tree) {
   /* Call parent Init() */
   CBuzzLoopFunctions::Init(t_tree);
   /* Parse XML tree */
   GetNodeAttribute(t_tree, "outfile", m_strOutFile);
   /* Create a new RNG */
   m_pcRNG = CRandom::CreateRNG("argos");
   /* Open the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);
}

/****************************************/
/****************************************/

void CFaultDetection::Reset() {
   /* Reset the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);
}

/****************************************/
/****************************************/

void CFaultDetection::Destroy() {
   m_cOutFile.close();
}

/****************************************/
/****************************************/
/**
 * Values to write to file:
 * RID
 * step fault init
 * step fault detected
 * fault detected
 * self fault
 */ 
void CFaultDetection::PostStep() {
   /* Get robot data */
   GetRobotData cGetRobotData(0);
   BuzzForeachVM(cGetRobotData);
   /* Flush data to the output file */
//    if(!cGetRobotData.m_vecRobotsThresholds.empty()) {
   for(int i = 0; i < GetNumRobots(); ++i) {
        LOGERR << i << std::endl;
        m_cOutFile << GetSpace().GetSimulationClock() << "\t" // step number 
        << i << "\t"; //robot id
//                    << cGetRobotData.m_vecRobotsTasks[i]; // ###CHANGE###
        m_cOutFile << std::endl;
    }
//    }
}

/****************************************/
/****************************************/

bool CFaultDetection::IsExperimentFinished() {
   /* Feel free to try out custom ending conditions */
   return false;
}

/****************************************/
/****************************************/

int CFaultDetection::GetNumRobots() const {
   return m_mapBuzzVMs.size();
}

/****************************************/
/****************************************/

void CFaultDetection::BuzzBytecodeUpdated() {
   /* Convey the stimuli to every robot */
   
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CFaultDetection, "fault_detection");
