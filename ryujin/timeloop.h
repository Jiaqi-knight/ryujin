#ifndef TIMELOOP_H
#define TIMELOOP_H

#include <discretization.h>
#include <offline_data.h>
#include <initial_values.h>
#include <schlieren_postprocessor.h>
#include <time_step.h>

#include <deal.II/base/parameter_acceptor.h>
#include <deal.II/base/timer.h>

#include <sstream>

namespace ryujin
{

  template <int dim>
  class TimeLoop : public dealii::ParameterAcceptor
  {
  public:
    using vector_type = typename grendel::TimeStep<dim>::vector_type;

    TimeLoop(const MPI_Comm &mpi_comm);
    virtual ~TimeLoop() final = default;

    void run();

  private:
    /* Private methods for run(): */

    void initialize();

    vector_type interpolate_initial_values(double t = 0);

    void compute_error(const vector_type &U, double t);

    void output(const vector_type &U,
                const std::string &name,
                double t,
                unsigned int cycle);

    /* Data: */

    const MPI_Comm &mpi_communicator;
    std::ostringstream timer_output;
    dealii::TimerOutput computing_timer;

    std::string base_name;
    double t_final;
    double output_granularity;
    bool enable_detailed_output;
    bool enable_compute_error;

    grendel::Discretization<dim> discretization;
    grendel::OfflineData<dim> offline_data;
    grendel::InitialValues<dim> initial_values;
    grendel::TimeStep<dim> time_step;
    grendel::SchlierenPostprocessor<dim> schlieren_postprocessor;

    std::unique_ptr<std::ofstream> filestream;

    /* Data for output management: */
    std::thread output_thread;
    vector_type output_vector;
    dealii::LinearAlgebra::distributed::Vector<double> output_alpha;
  };

} // namespace ryujin

#endif /* TIMELOOP_H */
