
#include "mykripke_succ_iterator.hh"

MyKripkeSuccIterator::MyKripkeSuccIterator(const bdd& cond, MyState const *state): 
    kripke_succ_iterator(cond), m_state(state), m_ch(), m_max()
{
    MyState::Location loc = state->getLoc();
    if (2 == loc)
    {
        m_max = 2;
    }
    else
    {
        m_max = 1;
    }
    m_ch = 0;
}

void MyKripkeSuccIterator::first()
{
    m_ch = 0;
}

void MyKripkeSuccIterator::next()
{
    assert(m_ch < m_max);
    m_ch++;
}

bool MyKripkeSuccIterator::done() const
{
    return m_ch == m_max;
}

MyState * MyKripkeSuccIterator::current_state() const
{
    assert(!done());
    MyState::Location loc = m_state->getLoc();
    // Program:
    // 0: x = 1
    // 1: y = x + y
    // 2: if
    //    :: true =>
    //        x = y - 3
    //    :: true =>
    //        x = y + 1
    // 3: if
    //    :: x >= 5 =>
    //        x = 0
    //    :: x < 0 =>
    //        x = 5
    //    :: => ()
    // 4: if
    //    :: y >= 2 =>
    //        y = 0
    //    :: y < 0 =>
    //        y = 2
    //    :: => ()
    switch (loc)
    {
    case 0:
        {
            MyState *s = m_state->clone();
            // x = 1
            s->setValue("x", 1);
            s->updateLoc(1);
            return s;
        }
    case 1:
        {
            MyState *s = m_state->clone();
            // y = x + y
            s->setValue("y", s->getValue("x") + s->getValue("y"));
            s->updateLoc(2);
            return s;
        }
    case 2:
        {
            MyState *s = m_state->clone();
            if (0 == m_ch)
            {
                // x = y - 3
                s->setValue("x", s->getValue("y") - 3);
            }
            else
            {
                // x = y + 1
                s->setValue("x", s->getValue("y") + 1);
            }
            s->updateLoc(3);
            return s;
        }
    case 3:
        {
            MyState *s = m_state->clone();
            int x = s->getValue("x");
            if (x >= 5)
            {
                // x = 0
                s->setValue("x", 0);
            }
            else if (x < 0)
            {
                // x = 5
                s->setValue("x", 5);
            }
            else {}
            s->updateLoc(4);
            return s;
        }
    case 4:
        {
            MyState *s = m_state->clone();
            int y = s->getValue("y");
            if (y >= 2)
            {
                // y = 0
                s->setValue("y", 0);
            }
            else if (y < 0)
            {
                // y = 5
                s->setValue("y", 2);
            }
            else {}
            s->updateLoc(0);
            return s;
        }
    }
}

std::string MyKripkeSuccIterator::formatTransition() const
{
    assert(!done());
    MyState::Location loc = m_state->getLoc();
    switch (loc)
    {
    case 0:
        {
            return "x = 1";
        }
    case 1:
        {
            return "y = x + y";
        }
    case 2:
        {
            if (0 == m_ch)
            {
                return "x = y - 3";
            }
            else
            {
                return "x = y + 1";
            }
        }
    case 3:
        {
            int x = m_state->getValue("x");
            if (x >= 5)
            {
                return "x = 0";
            }
            else if (x < 0)
            {
                return "x = 5";
            }
            else 
            {
                return "t";
            }
        }
    case 4:
        {
            int y = m_state->getValue("y");
            if (y >= 2)
            {
                return "y = 0";
            }
            else if (y < 0)
            {
                return "y = 2";
            }
            else 
            {
                return "t";
            }
        }
    }
}


MyKripkeSuccIterator::~MyKripkeSuccIterator()
{
    delete m_state;
}



    



