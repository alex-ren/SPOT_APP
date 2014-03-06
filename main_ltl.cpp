
#include "ltlparse/public.hh"
#include "ltlvisit/destroy.hh"

#include "misc/timer.hh"
#include "ltlast/atomic_prop.hh"
#include "ltlvisit/apcollect.hh"
#include "ltlenv/environment.hh"
#include "ltlparse/public.hh"
#include "tgbaalgos/ltl2tgba_fm.hh"
#include "tgba/tgbaproduct.hh"
#include "tgbaalgos/gtec/gtec.hh"

#include <iostream>

// #include "mykripke.hh"

using std::endl;

void model_check(const spot::ltl::formula* f1, const spot::ltl::formula* f2,
                  bool fm_exprop_opt, bool fm_symb_merge_opt,
                                  bool post_branching, bool fair_loop_approx);


int main(int argc, char *argv[])
{
    std::cout << "hello world!" << std::endl;
    bool fm_exprop_opt = false;
    bool fm_symb_merge_opt = true;
    bool post_branching = false;
    bool fair_loop_approx = false;
    // MyKripke d();

    std::string ltl_string1 = "G (a -> b)";
    std::string ltl_string2 = "X (X (~b))";

    spot::ltl::parse_error_list pel1;
    spot::ltl::parse_error_list pel2;

    spot::ltl::formula const * f1 = spot::ltl::parse(ltl_string1, pel1);
    if (spot::ltl::format_parse_errors(std::cerr, ltl_string1, pel1)) {
        std::cout << "formula 1 is not valid\n";
        f1->destroy();
        exit(1);
    }

    spot::ltl::formula const * f2 = spot::ltl::parse(ltl_string2, pel2);
    if (spot::ltl::format_parse_errors(std::cerr, ltl_string2, pel2)) {
        std::cout << "formula 2 is not valid\n";
        f1->destroy();
        f2->destroy();
        exit(1);
    }

    std::cout << "f1 is " << f1->dump() << endl;
    std::cout << "f2 is " << f2->dump() << endl;


    model_check(f1, f2, fm_exprop_opt, fm_symb_merge_opt,
                  post_branching, fair_loop_approx);
    f1->destroy();
    f2->destroy();

    std::cout << std::endl;


    return 0;
}


void model_check(const spot::ltl::formula* f1, const spot::ltl::formula* f2,
                  bool fm_exprop_opt, bool fm_symb_merge_opt,
                                  bool post_branching, bool fair_loop_approx)
{
    double start_time, build_time, check_time;

    const std::string* s;

    spot::bdd_dict dict;

    spot::ltl::atomic_prop_set* sap1 = spot::ltl::atomic_prop_collect(f1);
    spot::ltl::atomic_prop_set* sap2 = spot::ltl::atomic_prop_collect(f2);

    spot::timer_map timers;
    timers.start("construction");
    spot::tgba* a1 = spot::ltl_to_tgba_fm(f1, &dict, fm_exprop_opt,
                         fm_symb_merge_opt, post_branching, fair_loop_approx);
    spot::tgba* a2 = spot::ltl_to_tgba_fm(f2, &dict, fm_exprop_opt,
                         fm_symb_merge_opt, post_branching, fair_loop_approx);

    spot::tgba_product prod(a1, a2);

    spot::emptiness_check *ec= new spot::couvreur99_check(&prod);
    timers.stop("construction");

    timers.start("emptiness check");
    spot::emptiness_check_result* res = ec->check();
    timers.stop("emptiness check");

    std::cout << "===== ec->print_stats" << endl;
    ec->print_stats(std::cout);

    std::cout << "===== timers.print" << endl;
    timers.print(std::cout);

    if (res) 
    {
        std::cout << "an accepting run exists" << std::endl;
        spot::tgba_run* run = res->accepting_run();
        if (run)
        {
            std::cout << "===== print_tgba_run" << endl;
            spot::print_tgba_run(std::cout, &prod, run);
            delete run;
        }
        delete res;
    }
    else 
    {
        std::cout << "no accepting run found" << std::endl;
    }
  
    delete ec;
    delete a1;
    delete a2;
    delete sap1;
    delete sap2;
  
    return;
}





