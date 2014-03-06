
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
#include "tgbaalgos/dotty.hh"

#include <iostream>
#include <fstream>

#include "mykripke.hh"

using std::cout;
using std::endl;

void model_check(const MyKripke & model, const spot::ltl::formula *f,
                  bool fm_exprop_opt, bool fm_symb_merge_opt,
                                  bool post_branching, bool fair_loop_approx);

int main(int argc, char *argv[])
{
    std::cout << "hello world!" << std::endl;
    bool fm_exprop_opt = false;
    bool fm_symb_merge_opt = true;
    bool post_branching = false;
    bool fair_loop_approx = false;

    std::string ltl_string_all = "P1 & P2 & P3 & P4";

    // P1 := x > 2
    // P2 := y < 3
    // P3 := x <= 10
    // P4 := x == 5
    // std::string ltl_string_f = "~(G P3)";
    std::string ltl_string_f = "G (F P4)";

    spot::ltl::parse_error_list pel1;
    spot::ltl::parse_error_list pel2;

    spot::ltl::formula const * all_prop = spot::ltl::parse(ltl_string_all, pel1);

    spot::ltl::formula const * f = spot::ltl::parse(ltl_string_f, pel2);
    if (spot::ltl::format_parse_errors(std::cerr, ltl_string_f, pel2)) {
        std::cout << "formula is not valid\n";
        all_prop->destroy();
        f->destroy();
        exit(1);
    }

    std::cout << "f is " << f->dump() << endl;

    spot::ltl::atomic_prop_set* sap = spot::ltl::atomic_prop_collect(all_prop);
    spot::bdd_dict dict;

    MyKripke model(sap, &dict);

    std::ofstream dottyfile;
    dottyfile.open ("kripke.gv");
    spot::dotty_reachable(dottyfile, &model) << endl;
    dottyfile.close();


    model_check(model, f, fm_exprop_opt, fm_symb_merge_opt,
                  post_branching, fair_loop_approx);

    all_prop->destroy();
    f->destroy();

    std::cout << std::endl;


    return 0;
}

void model_check(const MyKripke & model, const spot::ltl::formula *f,
                  bool fm_exprop_opt, bool fm_symb_merge_opt,
                                  bool post_branching, bool fair_loop_approx)
{
    double start_time, build_time, check_time;

    const std::string* s;

    spot::timer_map timers;
    timers.start("construction");
    spot::tgba* a = spot::ltl_to_tgba_fm(f, model.get_dict(), fm_exprop_opt,
                         fm_symb_merge_opt, post_branching, fair_loop_approx);

    spot::tgba_product prod(&model, a);

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

            spot::tgba *pa = spot::tgba_run_to_tgba(&prod, run);
            std::ofstream dottyfile;
            dottyfile.open ("run.gv");
            spot::dotty_reachable(dottyfile, pa) << endl;
            dottyfile.close();
            delete run;
        }
        delete res;
    }
    else 
    {
        std::cout << "no accepting run found" << std::endl;
    }
  
    delete ec;
    delete a;
  
    return;
}





