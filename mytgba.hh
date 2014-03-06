
#ifndef MY_TGBA_H
#define MY_TGBA_H

#include "tgba/tgba.hh"

/// \brief Encapsulation of a \a petri_net in a
/// \a spot::tgba.
class MyTGBA : public spot::tgba {
public:
  /// \brief Construct a tgba view of \a pn where the set of
  /// atomic propositions which are obseved are in \a sap. The
  /// constructor registers these propositions in \a dict.
  ///
  /// \a sap can be a null pointer if the tgba will not be used
  /// for checking. In this case, \a dict can also be a null pointer.
  /// The pointers \a pn and \a dict (if not null) are supposed to be valide
  /// during all the live of the constructed instance.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline pn_tgba::pn_tgba
  /// \until } //
  MyTGBA(const petri_net* pn,
          const spot::ltl::atomic_prop_set* sap=0,
          spot::bdd_dict* dict=0);

  /// \brief Return the bdd corresponding to the values of
  /// the observed atomic propositions in the marking \a m.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline bdd pn_tgba::current_condition
  /// \until } //
  bdd current_condition(const marking& m) const;

  /// \brief Unregister all the used propositions.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline pn_tgba::~pn_tgba
  /// \until } //
  virtual ~pn_tgba();

  /// \brief Get the initial state of the automaton.
  ///
  /// The state has been allocated with \c new.  It is the
  /// responsability of the caller to \c delete it when no
  /// longer needed.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline spot::state* pn_tgba::get_init_state
  /// \until } //
  spot::state* get_init_state() const;

  /// \brief Get an iterator over the successors of \a local_state.
  ///
  /// The iterator has been allocated with \c new.  It is the
  /// responsability of the caller to \c delete it when no
  /// longer needed.
  ///
  /// The two last parameters are not used here
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline spot::tgba_succ_iterator* pn_tgba::succ_iter
  /// \until } //
  spot::tgba_succ_iterator* succ_iter (const spot::state* local_state,
          const spot::state*, const spot::tgba*) const;

  /// \brief Get the dictionary associated to the automaton.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline bdd_dict* pn_tgba::get_dict
  /// \until } //
  spot::bdd_dict* get_dict() const;

  /// \brief Format the state as a string for printing.
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline std::string pn_tgba::format_state
  /// \until } //
  std::string format_state(const spot::state* state) const;

  /// \brief Format the pointed transition as a string for printing.
  ///
  /// \param t a non-done pn_succ_iterator for this automata
  ///
  /// \dontinclude pntgba.cpp
  /// \skipline std::string pn_tgba::transition_annotation
  /// \until } //
  std::string transition_annotation(const spot::tgba_succ_iterator* t) const;

  /// \brief Return the empty set (false) as the Petri net accepts 
  /// all infinite sequences. Take care that blocking sequences are
  /// not taken into account here.
  bdd all_acceptance_conditions() const;

  /// \brief Return true.
  bdd neg_acceptance_conditions() const;
  
protected:
  /// Do the actual computation of tgba::support_conditions(). Return true.
  bdd compute_support_conditions(const spot::state* state) const;

  /// Do the actual computation of tgba::support_variables(). Return true.
  bdd compute_support_variables(const spot::state* state) const;

private:
  /// \brief not implemented (assert(false))
  pn_tgba(const pn_tgba& p);
  /// \brief not implemented (assert(false))
  pn_tgba& operator=(const pn_tgba& p);
  

  /// Reference the encapsulated Petri net. 
  const petri_net& pn;
  
  /// Point to the associated dictionnary.
  spot::bdd_dict* dict;
  
  /// Map the indexes of places used as atomic propositions to the 
  /// corresponding indexes of bdd variables. 
  std::map<int, int> mplace_at_prop;
};

#endif



