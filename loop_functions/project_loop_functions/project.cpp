#include "project.h"
#include "buzz/buzzvm.h"
#include <stdlib.h>
/****************************************/
/****************************************/

/**
 * Functor to get data from the robots
 */
struct GetRobotData : public CBuzzLoopFunctions::COperation {

  /** Constructor */
  GetRobotData(int n_tasks) : m_vecTaskCounts(n_tasks, 0) {}

  /** The action happens here */
  virtual void operator()(const std::string& str_robot_id, buzzvm_t t_vm) {
    /* Get the current task */
    buzzobj_t tFaultDetected = BuzzGet(t_vm, "fault_detected");
    /* Make sure it's the type we expect (an integer) */
    if (!buzzobj_isint(tFaultDetected)) {
        LOGERR << str_robot_id << ": variable 'task' has wrong type " << buzztype_desc[tFaultDetected->o.type] << std::endl;
        return;
    }
    /* Get the value */
    int nFaultDetected = buzzobj_getint(tFaultDetected);
    m_vecFaultDetected[t_vm->robot] = nFaultDetected;

    buzzobj_t tSelfFault = BuzzGet(t_vm, "self_faulty");
    /* Make sure it's the type we expect (an integer) */
    if (!buzzobj_isint(tSelfFault)) {
        LOGERR << str_robot_id << ": variable 'task' has wrong type " << buzztype_desc[tSelfFault->o.type] << std::endl;
        return;
    }
    /* Get the value */
    int nSelfFault = buzzobj_getint(tSelfFault);
    m_vecSelfFault[t_vm->robot] = nSelfFault;

    buzzobj_t tFaultInit = BuzzGet(t_vm, "fault_step");
    /* Make sure it's the type we expect (an integer) */
    if (!buzzobj_isint(tFaultInit)) {
        LOGERR << str_robot_id << ": variable 'task' has wrong type " << buzztype_desc[tFaultInit->o.type] << std::endl;
        return;
    }
    /* Get the value */
    int nFaultInit = buzzobj_getint(tFaultInit);
    m_vecFaultInit[t_vm->robot] = nFaultInit;
  }

  std::map<int,int> m_vecFaultDetected;
  std::map<int,int> m_vecSelfFault;
  std::map<int,int> m_vecFaultInit;
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
struct PutRandomFault : public CBuzzLoopFunctions::COperation {

  /** Constructor */
  PutRandomFault(int randNumber, int leftWheel, int rightWheel) : RandomNum(randNumber), LeftWheel(leftWheel), RightWheel(rightWheel) {}
  
  /** The action happens here */
  virtual void operator()(const std::string& str_robot_id,
                          buzzvm_t t_vm) {
    BuzzPut(t_vm, "RandomID", RandomNum);
    BuzzPut(t_vm, "fault_left_wheel", LeftWheel);
    BuzzPut(t_vm, "fault_right_wheel", RightWheel);
  }

  /** Calculated stimuli */
  const int RandomNum; 
  const int LeftWheel;
  const int RightWheel;
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

  this->randId = rand() % GetNumRobots();
  this->leftWheel = int(rand() % 12) - 6;
  this->rightWheel = int(rand() % 12) - 6;
}

/****************************************/
/****************************************/

void CFaultDetection::Reset() {
   /* Reset the output file */
  m_cOutFile.open(m_strOutFile.c_str(),
                  std::ofstream::out | std::ofstream::trunc);
  this->randId = rand() % GetNumRobots();
  this->leftWheel = int(rand() % 12) - 6;
  LOGERR << "LEftWheel" << this->leftWheel << std::endl;
  this->rightWheel = int(rand() % 12) - 6;
  LOGERR << "rightWheel" << this->rightWheel << std::endl;

  BuzzForeachVM(PutRandomFault(this->randId, this->leftWheel, this->rightWheel));

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
  for(int i = 0; i < GetNumRobots(); ++i) {
    m_cOutFile << GetSpace().GetSimulationClock() << "\t" // step number 
      << i << "\t" //robot id
      << cGetRobotData.m_vecFaultDetected[i] << "\t"
      << cGetRobotData.m_vecSelfFault[i] << "\t"
      << cGetRobotData.m_vecFaultInit[i] << "\t";
    m_cOutFile << std::endl;
  }
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
  BuzzForeachVM(PutRandomFault(this->randId, this->leftWheel, this->rightWheel));

}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CFaultDetection, "fault_detection");
