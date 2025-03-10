#include "AInt_Weight.hpp"

AInt_Weight::AInt_Weight( const IQuadPts * const &qua_s,
    const IQuadPts * const &qua_t,
    const IQuadPts * const &qua_u )
: num( qua_s->get_num_quadPts() * qua_t->get_num_quadPts() * qua_u->get_num_quadPts() )
{
  // Allocate the weights
  Weight = new double [num];

  const int n_s = qua_s->get_num_quadPts();
  const int n_t = qua_t->get_num_quadPts();
  const int n_u = qua_u->get_num_quadPts();

  for(int ii=0; ii<n_s; ++ii)
  {
    const double wei_s = qua_s->get_qw(ii);
    for(int jj=0; jj<n_t; ++jj)
    {
      const double wei_t = qua_t->get_qw(jj);
      for(int kk=0; kk<n_u; ++kk)
      {
        const double wei_u = qua_u->get_qw(kk);
        Weight[ ii + jj * n_s + kk * n_s * n_t ] = wei_s * wei_t * wei_u; 
      }
    }
  }
}

AInt_Weight::AInt_Weight( const IQuadPts * const &qua_s,
    const IQuadPts * const &qua_t  )
: num( qua_s->get_num_quadPts() * qua_t->get_num_quadPts() )
{
  // Allocate the weights
  Weight = new double [num];

  const int n_s = qua_s->get_num_quadPts();
  const int n_t = qua_t->get_num_quadPts();

  for(int ii=0; ii<n_s; ++ii)
  {
    const double wei_s = qua_s->get_qw(ii);
    for(int jj=0; jj<n_t; ++jj)
    {
      const double wei_t = qua_t->get_qw(jj);
      Weight[ ii + jj * n_s ] = wei_s * wei_t; 
    }
  }
}

AInt_Weight::AInt_Weight( const IQuadPts * const &qua ) 
: num( qua->get_num_quadPts() )
{
  // Allocate the weights
  Weight = new double [num];

  for(int ii=0; ii<num; ++ii) Weight[ii] = qua -> get_qw(ii);
}

AInt_Weight::~AInt_Weight()
{
  delete [] Weight; Weight = nullptr;
}

void AInt_Weight::print_info() const
{
  std::cout<<'\n';
  std::cout<<"===== AInt_Weight ===== \n";
  std::cout<<"num = "<<num<<std::endl;
  for(int ii=0; ii<num; ++ii) std::cout<<Weight[ii]<<'\n';
  std::cout<<"======================= \n";
}

// EOF
