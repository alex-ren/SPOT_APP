

#include "mykripke.hh"
#include <sstream>

MyKripke::MyKripke(spot::ltl::atomic_prop_set const * sap, spot::bdd_dict *dict):
    kripke(), m_dict(dict), m_aprop()
{
    if (sap) 
    {
        spot::ltl::atomic_prop_set::iterator it;
        for (it = sap->begin(); it != sap->end(); ++it) 
        {
            m_aprop[(*it)->name()] =
                      m_dict->register_proposition(*it, this);
        }
    }
}

MyState* MyKripke::get_init_state() const
{
    MyState::Evaluation eva;
    eva["x"] = 0;
    eva["y"] = 0;
    MyState::Location loc = 0;

    MyState *p = new MyState(loc, eva);

    return p;
}

bdd MyKripke::getCondFromState(MyState const *state) const
{
    bdd res = bddtrue;
    std::map<int, int>::const_iterator it;
    int x = state->getValue("x");
    int y = state->getValue("y");

    // P1 := x > 2
    // P2 := y < 3
    // P3 := x <= 10
    // P4 := x == 5
    if (x > 2) {
        res &= bdd_ithvar(m_aprop.at("P1"));
    } else {
        res &= bdd_nithvar(m_aprop.at("P1"));
    }

    if (y < 3) {
        res &= bdd_ithvar(m_aprop.at("P2"));
    } else {
        res &= bdd_nithvar(m_aprop.at("P2"));
    }

    if (x <= 10) {
        res &= bdd_ithvar(m_aprop.at("P3"));
    } else {
        res &= bdd_nithvar(m_aprop.at("P3"));
    }

    if (5 == x) {
        res &= bdd_ithvar(m_aprop.at("P4"));
    } else {
        res &= bdd_nithvar(m_aprop.at("P4"));
    }
    return res;
}

spot::bdd_dict* MyKripke::get_dict() const
{
    return m_dict;
}

std::string MyKripke::format_state(const spot::state* state) const
{
    const MyState *ps = dynamic_cast<const MyState *>(state);
    int x = ps->getValue("x");
    int y = ps->getValue("y");
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")" << "@" << ps->getLoc();
    return ss.str();
}


MyKripkeSuccIterator *
    MyKripke::succ_iter(const spot::state* local_state,
	      const spot::state* global_state,
	      const spot::tgba* global_automaton) const
{
    MyState const *pState = dynamic_cast<MyState const *>(local_state);

    bdd cond = this->getCondFromState(pState);
    MyState const *nState = pState->clone();
    MyKripkeSuccIterator *pIter = new MyKripkeSuccIterator(cond, nState);
    return pIter;
}

bdd MyKripke::state_condition(const spot::state* s) const
{
    MyState const *pState = dynamic_cast<MyState const *>(s);
    return this->getCondFromState(pState);
}

std::string MyKripke::transition_annotation(const spot::tgba_succ_iterator* t) const
{
    return dynamic_cast<MyKripkeSuccIterator const *>(t)->formatTransition();
}


MyKripke::~MyKripke()
{
  if (m_dict)
    m_dict->unregister_all_my_variables(this);
}
