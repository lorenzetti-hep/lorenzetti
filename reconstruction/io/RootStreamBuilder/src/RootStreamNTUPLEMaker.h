#ifndef RootStreamNTUPLEMaker_h
#define RootStreamNTUPLEMaker_h

#include "GaugiKernel/Algorithm.h"
#include "CaloCell/enumeration.h"
#include "EventInfo/EventInfo.h"
#include "TruthParticle/TruthParticle.h"
#include "CaloCluster/CaloCluster.h"
#include "CaloRings/CaloRings.h"
#include "Particle/Electron.h"

class RootStreamNTUPLEMaker : public Gaugi::Algorithm
{

public:
  /** Constructor **/
  RootStreamNTUPLEMaker(std::string);

  virtual ~RootStreamNTUPLEMaker();

  virtual StatusCode initialize() override;
  /**
   * @brief Crates the ntuple TTree branches
   * 
   * @param ctx The event context
   * @return StatusCode 
   */
  virtual StatusCode bookHistograms(SG::EventContext &ctx) const override;

  virtual StatusCode pre_execute(SG::EventContext &ctx) const override;

  virtual StatusCode execute(SG::EventContext &ctx, const G4Step *step) const override;

  virtual StatusCode execute(SG::EventContext &ctx, int evt) const override;

  virtual StatusCode post_execute(SG::EventContext &ctx) const override;

  virtual StatusCode fillHistograms(SG::EventContext &ctx) const override;

  virtual StatusCode finalize() override;

  bool computeForwardDecision(xAOD::CaloCluster *cluster, std::string workingpoint) const;

private:
  /**
   * @brief Deserializes the AOD event to multiple rows to the ntuple TTree.
   * 
   * @param evt The event number
   * @param ctx The event context
   * @return StatusCode 
   */
  StatusCode deserialize(int evt, SG::EventContext &ctx) const;

  template <class T>
  void InitBranch(TTree *fChain, std::string branch_name, T *param) const;

  /**
   * @brief The key to be added after the CaloCellContainer
   * 
   */
  std::string m_cellsKey;
  /**
   * @brief The key to be added after the EventInfoContainer
   * 
   */
  std::string m_eventKey;
  /**
   * @brief The key to be added after the TruthParticleContainer
   * 
   */
  std::string m_truthKey;
  /**
   * @brief The key to be added after the CaloClusterContainer
   * 
   */
  std::string m_clusterKey;
  /**
   * @brief The key to be added after the CaloRingsContainer
   * 
   */
  std::string m_ringerKey;
  /**
   * @brief The key to be added after the ElectronContainer
   * 
   */
  std::string m_electronKey;
  /**
   * @brief The AOD input file name
   * 
   */
  std::string m_inputFile;
  /**
   * @brief The NTuple output file name
   * 
   */
  std::string m_ntupleName;
  /**
   * @brief The NTuple TTree name
   * 
   */
  std::string m_outputNtupleName;
  
  std::vector<float> m_secondLambdaCuts;
  std::vector<float> m_lateralMomCuts;
  std::vector<float> m_longMomCuts;
  std::vector<float> m_fracMaxCuts;
  std::vector<float> m_secondRCuts;
  std::vector<float> m_lambdaCenterCuts;

  int m_outputLevel;
};

#endif
