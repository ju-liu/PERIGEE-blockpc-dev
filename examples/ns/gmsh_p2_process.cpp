// ==================================================================
// gmsh_p2_process.cpp
//
// Code that handles the gmsh file generated with quadratic tet elem.
//
// Date Created: March 3 2020
// Author: Ju Liu
// ==================================================================
#include "Gmsh_FileIO.hpp"

int main( int argc, char * argv[] )
{
  std::string gmshFile = "cylinder_p2.msh";
  int num_outlet = 1;

  PetscInitialize(&argc, &argv, (char *)0, PETSC_NULL);
  
  SYS_T::GetOptionString("-gmsh_file", gmshFile);
  SYS_T::GetOptionInt("-num_outlet", num_outlet);
  
  std::cout<<" -gmsh_file: "<<gmshFile<<std::endl;
  std::cout<<" -num_outlet: "<<num_outlet<<std::endl;

  Gmsh_FileIO * GIO = new Gmsh_FileIO( gmshFile );

  GIO -> print_info();
 
  // Gmsh file should contain only one 3D domain 
  const int vol_id = 0;

  // Gmsh IEN needs to be updated by swapping local node 8 and 9 to be
  // compatible with the VTK IEN node ordering
  GIO -> update_quadratic_tet_IEN( vol_id );

  // Write the surface mesh into vtu files
  GIO -> write_quadratic_sur_vtu( 0, vol_id, true); // assumed to be wall vtp
  GIO -> write_quadratic_sur_vtu( 1, vol_id, true); // assumed to be wall vtp
  
  for(int ii=0; ii<num_outlet; ++ii)
    GIO -> write_quadratic_sur_vtu( 2+ii, vol_id, true );

  const std::string wmname("whole_vol");
  const bool isXML = true;
  GIO -> write_vtu( wmname, isXML );

  delete GIO; 
  PetscFinalize();
  return 0;
}

// EOF
