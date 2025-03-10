// ==================================================================
// gmsh_process.cpp
//
// Code that handles the gmsh file.
//
// Date Created: July 1 2017
// Author: Ju Liu
// ==================================================================
#include "Gmsh_FileIO.hpp"

int main( int argc, char * argv[] )
{
  std::string gmshFile = "fsi_cylinder.msh";

  PetscInitialize(&argc, &argv, (char *)0, PETSC_NULL);
  SYS_T::GetOptionString("-gmsh_file", gmshFile);
  std::cout<<" -gmsh_file: "<<gmshFile<<std::endl;

  Gmsh_FileIO * GIO = new Gmsh_FileIO( gmshFile );

  GIO -> check_FSI_ordering();

  GIO -> print_info();

  GIO -> write_vtp(0,0,true);
  GIO -> write_vtp(1,0,true);
  GIO -> write_vtp(2,0,true);

  GIO -> write_vtp(2,1,true);
  GIO -> write_vtp(3,1);
  GIO -> write_vtp(4,1);
  GIO -> write_vtp(5,1);

  GIO -> write_each_vtu();

  const std::string wmname("whole_vol");
  const bool isXML = true;
  GIO -> write_vtu( wmname, isXML );

  delete GIO; 
  PetscFinalize();
  return 0;
}

// EOF
