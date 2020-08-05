#include "EBC_Partition_vtp_wall.hpp"

EBC_Partition_vtp_wall::EBC_Partition_vtp_wall( 
    const IPart * const &part,
    const Map_Node_Index * const &mnindex,
    const ElemBC * const &ebc )
: EBC_Partition_vtp(part, mnindex, ebc),
  fluid_density( ebc-> get_fluid_density() )
{
  part_thickness.clear();
  part_youngsmod.clear();

  // wall has only one surface as the assumption in wall ebc  
  const int ebc_id = 0;
  if( num_local_node[ebc_id] > 0 )
  {
    // re-generate local_node index list
    std::vector<int> local_node; local_node.clear();
    for(int jj=0; jj<ebc->get_num_cell(ebc_id); ++jj)
    {
      const int elem_index = ebc -> get_global_cell(ebc_id, jj);
      if( part -> get_elemLocIndex( elem_index ) != -1 )
      {
        for(int kk=0; kk<ebc->get_cell_nLocBas(ebc_id); ++kk) 
          local_node.push_back( ebc->get_ien(ebc_id, jj, kk) );
      }
    }
    VEC_T::sort_unique_resize( local_node );

    // Make sure local_node is compatible with existing data
    SYS_T::print_fatal_if( num_local_node[ebc_id] != static_cast<int>(local_node.size()),
        "Error: there is an internal error.\n");

    // obtain the wall properties of the whole surface
    std::vector<double> temp_th, temp_E;
    ebc -> get_wall_thickness( temp_th );
    ebc -> get_wall_youngsmod( temp_E );

    // save the wall properties belong to the local node, local to the cpu.
    for( int ii=0; ii<num_local_node[ebc_id]; ++ii )
    {
      part_thickness.push_back( temp_th[ local_node[ii] ] );
      part_youngsmod.push_back( temp_E[ local_node[ii] ] );
    }
  }
}


EBC_Partition_vtp_wall::~EBC_Partition_vtp_wall()
{
  VEC_T::clean( part_thickness );
  VEC_T::clean( part_youngsmod );
}


void EBC_Partition_vtp_wall::write_hdf5( const char * FileName ) const
{
  // Call base class writer to write base class data at ebc_wall folder
  EBC_Partition_vtp::write_hdf5( FileName, "ebc_wall" );

  const std::string input_fName(FileName);
  const std::string fName = SYS_T::gen_partfile_name( input_fName, cpu_rank );

  hid_t file_id = H5Fopen(fName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

  // open the folder at fName/ebc_wall again to append additional data 
  hid_t g_id = H5Gopen( file_id, "ebc_wall", H5P_DEFAULT );

  HDF5_Writer * h5w = new HDF5_Writer( file_id );

  h5w -> write_doubleScalar( g_id, "fluid_density", fluid_density );

  // num_ebc = 1 for wall elem bc
  const int ebc_id = 0;
  if( num_local_cell[ebc_id] > 0 )
  {
    h5w->write_doubleVector( g_id, "thickness", part_thickness );
    h5w->write_doubleVector( g_id, "youngsmod", part_youngsmod );
  }

  delete h5w; H5Gclose( g_id ); H5Fclose( file_id );
}


void EBC_Partition_vtp_wall::write_hdf5( const char * FileName, 
    const char * GroupName ) const
{
  // This function is NOT allowed. If the user uses the same groupname for the
  // wall and outlets, the wall data and the 0-th outlet data will be mixed.
  // We enforce the users to call the default write_hdf5, where the groupname
  // for the wall is fixed to be ebc_wall.
  SYS_T::print_fatal("Error: EBC_Partition_vtp_wall, write_hdf5 with groupname is not allowed.\n");
}

// EOF
