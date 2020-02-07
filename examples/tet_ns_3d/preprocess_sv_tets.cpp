// ==================================================================
// preprocess_sv_tets.cpp
//
// This is a preprocessor code for handling Navier-Stokes equations 
// discretized by tetradedral elements.
//
// Date Created: Jan 01 2020
// ==================================================================
#include "Math_Tools.hpp"
#include "Mesh_Tet4.hpp"
#include "Mesh_Tet10.hpp"
#include "IEN_Tetra_P1.hpp"
#include "IEN_Tetra_P2.hpp"
#include "Global_Part_METIS.hpp"
#include "Global_Part_Serial.hpp"
#include "Part_Tet4.hpp"
#include "NodalBC_3D_vtp.hpp"
#include "NodalBC_3D_vtu.hpp"
#include "NodalBC_3D_inflow.hpp"
#include "ElemBC_3D_tet_outflow.hpp"
#include "NBC_Partition_3D.hpp"
#include "NBC_Partition_3D_inflow.hpp"
#include "EBC_Partition_vtp_outflow.hpp"

int main( int argc, char * argv[] )
{
  // Clean the potentially pre-existing hdf5 files in the job folder
  int sysret = system("rm -rf part_p*.h5");
  SYS_T::print_fatal_if(sysret != 0, "Error: system call failed. \n");
  sysret = system("rm -rf preprocessor_cmd.h5");
  SYS_T::print_fatal_if(sysret != 0, "Error: system call failed. \n");
  sysret = system("rm -rf NumLocalNode.h5");
  SYS_T::print_fatal_if(sysret != 0, "Error: system call failed. \n");

  // Define basic problem settins
  const int dofNum = 4; // degree-of-freedom for the physical problem
  const int dofMat = 4; // degree-of-freedom in the matrix problem
  const std::string part_file("part");
  
  // Element options: 501 linear tets, 502 quadratic tets
  int elemType = 501;
  int num_outlet = 1;
  
  // Default names for input geometry files
  std::string geo_file("./whole_vol.vtu");
  std::string sur_file_in("./inflow_vol.vtp");
  std::string sur_file_wall("./wall_vol.vtp");
  std::string sur_file_out_base("./outflow_vol_");

  // Mesh partition setting
  int cpu_size = 1;
  int in_ncommon = 2;
  bool isDualGraph = true;

  PetscMPIInt size;

  PetscInitialize(&argc, &argv, (char *)0, PETSC_NULL);

  MPI_Comm_size(PETSC_COMM_WORLD, &size);

  SYS_T::print_fatal_if(size!=1, "ERROR: preprocessor needs to be run in serial.\n");

  // Get the command line arguments
  SYS_T::GetOptionInt("-cpu_size", cpu_size);
  SYS_T::GetOptionInt("-in_ncommon", in_ncommon);
  SYS_T::GetOptionInt("-num_outlet", num_outlet);
  SYS_T::GetOptionInt("-elem_type", elemType);
  SYS_T::GetOptionString("-geo_file", geo_file);
  SYS_T::GetOptionString("-sur_file_in", sur_file_in);
  SYS_T::GetOptionString("-sur_file_wall", sur_file_wall);
  SYS_T::GetOptionString("-sur_file_out_base", sur_file_out_base);

  if( elemType != 501 && elemType !=502 ) SYS_T::print_fatal("Error: unknown element type.\n");

  // Print the command line arguments
  std::cout<<"==== Command Line Arguments ===="<<std::endl;
  std::cout<<" -elem_type: "<<elemType<<std::endl;
  std::cout<<" -num_outlet: "<<num_outlet<<std::endl;
  std::cout<<" -geo_file: "<<geo_file<<std::endl;
  std::cout<<" -sur_file_in: "<<sur_file_in<<std::endl;
  std::cout<<" -sur_file_wall: "<<sur_file_wall<<std::endl;
  std::cout<<" -sur_file_out_base: "<<sur_file_out_base<<std::endl;
  std::cout<<" -part_file: "<<part_file<<std::endl;
  std::cout<<" -cpu_size: "<<cpu_size<<std::endl;
  std::cout<<" -in_ncommon: "<<in_ncommon<<std::endl;
  std::cout<<" -isDualGraph: true \n";
  std::cout<<"---- Problem definition ----\n";
  std::cout<<" dofNum: "<<dofNum<<std::endl;
  std::cout<<" dofMat: "<<dofMat<<std::endl;
  std::cout<<"====  Command Line Arguments/ ===="<<std::endl;

  // Check if the vtu geometry files exist on disk
  SYS_T::file_check(geo_file); std::cout<<geo_file<<" found. \n";

  // If it is quadratic mesh, the mesh file will be all in vtu format
  if(elemType == 502)
  {
    sur_file_in.erase( sur_file_in.end()-4, sur_file_in.end() );
    sur_file_in += ".vtu";
    sur_file_wall.erase( sur_file_wall.end()-4, sur_file_wall.end() );
    sur_file_wall += ".vtu";
  }

  SYS_T::file_check(sur_file_in); std::cout<<sur_file_in<<" found. \n";

  SYS_T::file_check(sur_file_wall); std::cout<<sur_file_wall<<" found. \n";

  // Generate the outlet file names and check existance
  std::vector< std::string > sur_file_out;
  sur_file_out.resize( num_outlet );

  for(int ii=0; ii<num_outlet; ++ii)
  {
    std::ostringstream ss;
    ss<<sur_file_out_base;
    if( ii/10 == 0 ) ss<<"00";
    else if( ii/100 == 0 ) ss<<"0";

    if(elemType == 501 ) ss<<ii<<".vtp";
    else ss<<ii<<".vtu";
      
    sur_file_out[ii] = ss.str(); // generate the outlet face file name
    
    SYS_T::file_check(sur_file_out[ii]);
    std::cout<<sur_file_out[ii]<<" found. \n";
  }

  // Record the problem setting into a HDF5 file: preprocessor_cmd.h5
  hid_t cmd_file_id = H5Fcreate("preprocessor_cmd.h5",
      H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  HDF5_Writer * cmdh5w = new HDF5_Writer(cmd_file_id);

  cmdh5w->write_intScalar("num_outlet", num_outlet);
  cmdh5w->write_intScalar("cpu_size", cpu_size);
  cmdh5w->write_intScalar("in_ncommon", in_ncommon);
  cmdh5w->write_intScalar("dofNum", dofNum);
  cmdh5w->write_intScalar("dofMat", dofMat);
  cmdh5w->write_intScalar("elemType", elemType);
  cmdh5w->write_string("geo_file", geo_file);
  cmdh5w->write_string("sur_file_in", sur_file_in);
  cmdh5w->write_string("sur_file_out_base", sur_file_out_base);
  cmdh5w->write_string("sur_file_wall", sur_file_wall);
  cmdh5w->write_string("part_file", part_file);

  delete cmdh5w; H5Fclose(cmd_file_id);

  // Read the volumetric mesh file from the vtu file: geo_file
  int nFunc, nElem;
  std::vector<int> vecIEN;
  std::vector<double> ctrlPts;
  
  TET_T::read_vtu_grid(geo_file.c_str(), nFunc, nElem, ctrlPts, vecIEN);
  
  IIEN * IEN = nullptr;
  IMesh * mesh = nullptr;

  if(elemType == 501)
  {
    SYS_T::print_fatal_if(vecIEN.size() / nElem != 4, "Error: the mesh connectivity array size does not match with the element type 501. \n");
    
    IEN = new IEN_Tetra_P1(nElem, vecIEN);
    mesh = new Mesh_Tet4(nFunc, nElem);
  }
  else
  {
    SYS_T::print_fatal_if(vecIEN.size() / nElem != 10, "Error: the mesh connectivity array size does not match with the element type 502. \n");
    
    IEN = new IEN_Tetra_P2(nElem, vecIEN);
    mesh = new Mesh_Tet10(nFunc, nElem);
  }

  VEC_T::clean( vecIEN );
  
  mesh -> print_mesh_info();
  
  // Call METIS to partition the mesh 
  IGlobal_Part * global_part = nullptr;
  if(cpu_size > 1)
    global_part = new Global_Part_METIS( cpu_size, in_ncommon,
        isDualGraph, mesh, IEN, "epart", "npart" );
  else if(cpu_size == 1)
    global_part = new Global_Part_Serial( mesh, "epart", "npart" );
  else
  {
    std::cerr<<"ERROR: wrong cpu_size: "<<cpu_size<<std::endl;
    exit(EXIT_FAILURE);
  }

  // Generate the new nodal numbering
  Map_Node_Index * mnindex = new Map_Node_Index(global_part, cpu_size, mesh->get_nFunc());
  mnindex->write_hdf5("node_mapping");

  // Setup Nodal Boundary Conditions
  std::vector<INodalBC *> NBC_list;
  NBC_list.clear(); NBC_list.resize( dofMat );

  std::vector<std::string> dir_list;
  dir_list.push_back( sur_file_in );
  dir_list.push_back( sur_file_wall );

  if(elemType == 501)
  {
    NBC_list[0] = new NodalBC_3D_vtp( nFunc );
    NBC_list[1] = new NodalBC_3D_vtp( dir_list, nFunc );
    NBC_list[2] = new NodalBC_3D_vtp( dir_list, nFunc );
    NBC_list[3] = new NodalBC_3D_vtp( dir_list, nFunc );
  }
  else
  {
    NBC_list[0] = new NodalBC_3D_vtu( nFunc );
    NBC_list[1] = new NodalBC_3D_vtu( dir_list, nFunc );
    NBC_list[2] = new NodalBC_3D_vtu( dir_list, nFunc );
    NBC_list[3] = new NodalBC_3D_vtu( dir_list, nFunc );
  }

  // Inflow BC info
  std::vector<double> inflow_outward_vec;
  TET_T::get_out_normal( sur_file_in, ctrlPts, IEN, inflow_outward_vec );
  INodalBC * InFBC = new NodalBC_3D_inflow( sur_file_in, sur_file_wall,
      nFunc, inflow_outward_vec, elemType );

  // Setup Elemental Boundary Conditions
  std::vector< std::vector<double> > outflow_outward_vec;
  outflow_outward_vec.resize( sur_file_out.size() );
  for(unsigned int ii=0; ii<sur_file_out.size(); ++ii)
    TET_T::get_out_normal( sur_file_out[ii], ctrlPts, IEN, outflow_outward_vec[ii] );

  ElemBC * ebc = new ElemBC_3D_tet_outflow( sur_file_out, outflow_outward_vec, elemType );

  ebc -> resetTriIEN_outwardnormal( IEN );

  // debug
  std::vector<double> intNA;

  std::cout<<"\n outlet 0 \n";
  ebc -> get_intNA(0, intNA);
  VEC_T::print(intNA);

  std::cout<<"\n outlet 1 \n";
  ebc -> get_intNA(1, intNA);
  VEC_T::print(intNA);

  // end of debug


  // Finalize the code and exit
  delete InFBC; delete ebc;

  for(auto it_nbc=NBC_list.begin(); it_nbc != NBC_list.end(); ++it_nbc)
    delete *it_nbc;

  delete mnindex; delete global_part; delete mesh; delete IEN;
  PetscFinalize();
  return EXIT_SUCCESS;
}

// EOF
