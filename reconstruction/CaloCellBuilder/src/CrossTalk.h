#ifndef CrossTalk_h
#define CrossTalk_h

#include "GaugiKernel/AlgTool.h"
#include "GaugiKernel/EDM.h"



class CrossTalk : public Gaugi::AlgTool
{

  public:
    /** Constructor **/
    CrossTalk( std::string name );
    virtual ~CrossTalk();
    virtual StatusCode initialize() override;
    virtual StatusCode finalize() override;

    virtual StatusCode execute( SG::EventContext &ctx, Gaugi::EDM * ) const override;

  private:

    float m_minEnergy;
    std::string m_collectionKey;

};

#endif

