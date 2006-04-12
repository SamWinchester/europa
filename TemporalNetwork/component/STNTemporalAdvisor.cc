#include "STNTemporalAdvisor.hh"
#include "Token.hh"
#include "Object.hh"
#include "TokenVariable.hh"
#include "PlanDatabase.hh"
#include "TemporalNetworkDefs.hh"
#include "TemporalPropagator.hh"
#include "Debug.hh"

namespace EUROPA {

  STNTemporalAdvisor::STNTemporalAdvisor(const TemporalPropagatorId& propagator)
    : DefaultTemporalAdvisor(propagator->getConstraintEngine()), m_propagator(propagator) {}

  STNTemporalAdvisor::~STNTemporalAdvisor(){}

  bool STNTemporalAdvisor::canPrecede(const TokenId& first, const TokenId& second){    
    if (!DefaultTemporalAdvisor::canPrecede(first, second))
      return false;

    bool retval = m_propagator->canPrecede(first->getEnd(), second->getStart());
    return (retval);
  }

  bool STNTemporalAdvisor::canFitBetween(const TokenId& token, const TokenId& predecessor, const TokenId& successor){
    if (!DefaultTemporalAdvisor::canFitBetween(token, predecessor, successor))
      return false;
    return m_propagator->canFitBetween(token->getStart(), token->getEnd(), predecessor->getEnd(), successor->getStart());
  }

  /**
   * @brief 2 tokens can be concurrent if the temporal distance between them can be 0
   */
  bool STNTemporalAdvisor::canBeConcurrent(const TokenId& first, const TokenId& second){
    debugMsg("STNTemporalAdvisor:canBeConcurrent", "first [" << first->getStart() << ", " << first->getEnd() << "]");
    debugMsg("STNTemporalAdvisor:canBeConcurrent", "second[" << second->getStart() << ", " << second->getEnd() << "]"); 

   return (m_propagator->canBeConcurrent(first->getStart(), second->getStart()) &&
	    m_propagator->canBeConcurrent(first->getEnd(), second->getEnd()));
  }

  /**
   * @brief Gets the temporal distance between two temporal variables. 
   * @param exact if set to true makes this distance calculation exact.
   */
  const IntervalIntDomain STNTemporalAdvisor::getTemporalDistanceDomain(const TimeVarId& first, const TimeVarId& second, const bool exact) {
    return (m_propagator->getTemporalDistanceDomain(first, second, exact));
  }

  unsigned int STNTemporalAdvisor::mostRecentRepropagation() const{
    return m_propagator->mostRecentRepropagation();
  }
}
