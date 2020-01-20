#include "IEN_Tetra_P2.hpp"
#include "Tet_Tools.hpp"

int main( int argc, char * argv[] )
{
  PetscInitialize(&argc, &argv, (char *)0, PETSC_NULL);

  std::string geo_file("./mesh-complete.mesh.vtu");

  int nFunc, nElem;
  std::vector<double> ptcoor;
  std::vector<int> vien;

  TET_T::read_vtu_grid(geo_file, nFunc, nElem, ptcoor, vien );
  
  IIEN * IEN = new IEN_Tetra_P2(nElem, vien);

  std::vector<double> outnormal;

  std::string sur_file("inflow.vtu");

  TET_T::get_out_normal( sur_file, ptcoor, IEN, outnormal );
  VEC_T::print(outnormal);

  /*
  std::vector<double> ptout;
  std::vector<int> ienout, ptidx, elemidx;
  for(int ii=0; ii<20; ++ii) 
  {
    ptout.push_back(ptcoor[3*vien[ii]+0]);
    ptout.push_back(ptcoor[3*vien[ii]+1]);
    ptout.push_back(ptcoor[3*vien[ii]+2]);
    ienout.push_back(ii);
    ptidx.push_back(ii*10);
  }

  elemidx.push_back(-1);
  elemidx.push_back(231);

  std::string out_name("./test-output");

  std::vector<int> ptag;
  ptag.push_back(2);
  ptag.push_back(4);

  TET_T::write_tet_grid( out_name, false, 20, 2, ptout, ienout,
     ptag, ptidx, elemidx );
  */
  PetscFinalize();
  return 0;
}
