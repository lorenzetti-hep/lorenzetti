#ifndef CaloPhiRange_h 
#define CaloPhiRange_h 

#include <cmath>

class CaloPhiRange
{
	public:
	    static float twopi ();
	    static float phi_min ();
	    static float phi_max ();
	    static float fix ( float phi );
	
	    /*
			 * @brief simple phi1 - phi2 calculation, but result is fixed to respect range.
	     */
	    static float diff ( float phi1,  float phi2 );
	
	private:
	    
	    static const float m_phi_min;
	    static const float m_twopi;
	    static const float m_phi_max;
};


inline float CaloPhiRange::twopi()
{ 
	return m_twopi;
}

inline float CaloPhiRange::phi_min()
{ 
	return m_phi_min;
}

inline float CaloPhiRange::phi_max()
{ 
	return m_phi_max;
}

#endif
