
#ifndef MY_KRIPKE_HH
#define MY_KRIPKE_HH

#include <map>
#include <string>

#include <kripke/kripke.hh>
#include <ltlvisit/apcollect.hh>

#include "mykripke_succ_iterator.hh"

#include "mystate.hh"

class MyKripke: public spot::kripke
{
public:
    MyKripke(spot::ltl::atomic_prop_set const * sap, spot::bdd_dict *dict);

    virtual MyState* get_init_state() const;

    virtual MyKripkeSuccIterator*
    succ_iter(const spot::state* local_state,
	      const spot::state* global_state = 0,
	      const tgba* global_automaton = 0) const;

    virtual std::string
    transition_annotation(const spot::tgba_succ_iterator* t) const;

    virtual bdd state_condition(const spot::state* s) const;
    virtual spot::bdd_dict* get_dict() const;

    virtual ~MyKripke();
    virtual std::string format_state(const spot::state* state) const;

protected:
    bdd getCondFromState(MyState const *state) const;

private:

    /// Point to the associated dictionnary.
    spot::bdd_dict* m_dict;
    std::map<std::string, int> m_aprop;
};



#endif


