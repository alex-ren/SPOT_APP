
#include "mystate.hh"


MyState::MyState(Location const &loc, Evaluation const &eva): m_loc(loc), m_eva(eva) 
{}

MyState::MyState(MyState const &src): m_eva(src.m_eva), m_loc(src.m_loc)
{
}

int MyState::compare(const state* other) const
{
    MyState const *m = dynamic_cast<MyState const *>(other);
    if (m_loc > m->m_loc) return 1;
    if (m_loc < m->m_loc) return -1;

    if (m_eva.size() > m->m_eva.size()) return 1;
    if (m_eva.size() < m->m_eva.size()) return -1;

    std::pair<Evaluation::const_iterator, Evaluation::const_iterator> res =
        std::mismatch(m_eva.begin(), m_eva.end(), m->m_eva.begin());

    if (res.first == m_eva.end()) return 0;

    if (*res.first > *res.second)
    {
        return 1;
    }
    else 
    {
        return -1;
    }
}

size_t MyState::hash() const
{

   int seed = 131; 
   size_t hash = 0;
   hash = (hash * seed) + m_loc;

   Evaluation::const_iterator iter = m_eva.begin();
   for (; iter != m_eva.end(); ++iter)
   {
      hash = (hash * seed) + iter->second;
   }

   return hash;
}

MyState * MyState::clone() const
{
    MyState *p = new MyState(*this);
    return p;
}

void MyState::destroy() const
{
    delete this;
}

MyState::Location MyState::getLoc() const
{
    return m_loc;
}

void MyState::updateLoc(Location loc)
{
    m_loc = loc;
}

int MyState::getValue(std::string const & id) const
{
    return m_eva.at(id);
}

void MyState::setValue(std::string const & id, int v)
{
    m_eva[id] = v;
}

MyState::~MyState()
{}







