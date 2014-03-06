
#ifndef MY_KRIPKE_SUCC_ITERATOR_HH
#define MY_KRIPKE_SUCC_ITERATOR_HH

#include "mystate.hh"
#include <kripke/kripke.hh>

class MyKripkeSuccIterator: public spot::kripke_succ_iterator
{
public:
    MyKripkeSuccIterator(const bdd& cond, MyState const * state);

    virtual void first();
    virtual void next();
    virtual bool done() const;
    virtual MyState* current_state() const;

    virtual ~MyKripkeSuccIterator();
    std::string formatTransition() const;

private:
    int m_ch;
    int m_max;

    MyState const *m_state;
};



#endif



