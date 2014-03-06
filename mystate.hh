#ifndef MY_STATE_HH
#define MY_STATE_HH

#include <tgba/state.hh>

#include <string>
#include <map>

class MyState : public spot::state 
{
public:
    typedef class std::map<std::string, int> Evaluation;
    typedef int Location;

    MyState(Location const &loc, Evaluation const &eva);

    virtual int compare(const state* other) const;
    virtual size_t hash() const;
    virtual MyState * clone() const;
    virtual void destroy() const;

    Location getLoc() const;
    void updateLoc(Location loc);
    int getValue(std::string const & id) const;
    void setValue(std::string const & id, int v);

    virtual ~MyState();

private:
    MyState(MyState const &src);

    MyState & operator = (MyState const &src);

    Evaluation m_eva;
    Location m_loc;

};







#endif


