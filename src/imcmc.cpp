#include "imcmc.hpp"
#include "mpi.h"

namespace imcmc{

    imcmc_likelihood_state::imcmc_likelihood_state(){
        this_like_is_ok = true;
        stop_on_error   = false;
        prompt_warning  = false;
        prompt_error    = true;
    }

    void imcmc_likelihood_state::init(){

        this_like_is_ok = true;
        stop_on_error   = false;
        prompt_warning  = false;
        prompt_error    = true;

        if( MPI::COMM_WORLD.Get_rank() == ROOT_RANK ){
            std::cout << "\n #    ================= Likelihood State Initialization ================\n"
                      << " ---> imcmc_likelihood_state.this_like_is_ok  initialized to \'true\' \n"
                      << " ---> imcmc_likelihood_state.stop_on_error    initialized to \'false\'\n"
                      << " ---> imcmc_likelihood_state.prompt_warning   initialized to \'false\'\n";
	    }
    }

    void imcmc_likelihood_state::store_mesg( std::string& why ){
        strcpy(errmesg, why.c_str());
    }

    //  ISO C++ forbids converting a string constant to ‘char*’
    //  so I change older declaration "char *why" to "const char *why"
    void imcmc_likelihood_state::store_mesg( const char *why ){
        strcpy(errmesg, why);
    }

    void imcmc_likelihood_state::what_happened(){

        int rank = MPI::COMM_WORLD.Get_rank();

        if( stop_on_error ){
            std::cout << "\n #  ===================  Likelihood Error  ================== #\n"
                      << " >  we captured the following error @ rank " << rank << ":\n"
                      << " " << errmesg << "\n"
                      << " #  ========================================================= #\n";

            throw std::runtime_error(" #  ---> Likelihood Error Ecountered\n");
        }
        else if( prompt_warning ){
            std::cout << "\n #  ===================  Likelihood Warning  ================== #\n"
                      << " >  we captured the following Warning @ rank " << rank << ":\n"
                      << " " << errmesg << "\n"
                      << " #  =========================================================== #\n";
        }
    }

    void imcmc_runtime_error( std::string err_info ){

        int rank = MPI::COMM_WORLD.Get_rank();

        std::cout << "\n #    ====================================================== #\n";
        std::cout << " #    +++++++++++++++++++  Error Message  ++++++++++++++++++ #\n";
        std::cout << " ---@ Rank Num: " << rank << "\n";
        std::cout << " ---@ FileName: " << __FILE__ << "\n";
        std::cout << " ---@ Line Num: " << __LINE__ << "\n";
        std::cout << " ---@ Fun Name: " << __FUNCTION__ << "\n";
        std::cout << " +++> ";

        throw std::runtime_error(err_info);
    }

    void imcmc_runtime_warning( std::string warn_info ){

        int rank = MPI::COMM_WORLD.Get_rank();

        std::cout << "\n #    ====================================================== #\n";
        std::cout << " #    ------------------  Warning Message  ----------------- #\n";
        std::cout << " ---@ Rank Num: " << rank << "\n";
        std::cout << " ---@ FileName: " << __FILE__ << "\n";
        std::cout << " ---@ Line Num: " << __LINE__ << "\n";
        std::cout << " ---@ Fun Name: " << __FUNCTION__ << "\n";
        std::cout << " ---> ";
        std::cout << warn_info << std::endl;
        std::cout << " #    ------------------------------------------------------ #\n";
    }

//  new added verbose information displayer (@ Jan-28-2016)
	void imcmc_verbose( int rank, std::string& verbose_info ){
		if( rank == ROOT_RANK ){
			std::cout << " --> " << verbose_info << "\n";
		}
	}

}
