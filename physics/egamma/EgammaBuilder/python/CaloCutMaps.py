__all__ = ['CutBasedIsEM']

class CutBasedIsEM:
	def __init__(self):

		self.etHadCut = {'loose':[0.1218],
					     'medium':[0.0270375],
					     'tight':[0.0270375],
						 'vloose': [0.157]}
	    
		self.rEtaCut={'loose':[0.57],
	                  'medium':[0.814625],
	                	'tight':[0.83125],
						'vloose':[0.752]}

		self.eRatioCut = {'loose':[0.47],	
	                	  'medium':[0.57],
	                	  'tight':[0.65],
						  'vloose':[0.52]}
	
	def getCuts(self,workingPoint):
		return [self.etHadCut[workingPoint],
    			self.rEtaCut[workingPoint],
    			self.eRatioCut[workingPoint],]