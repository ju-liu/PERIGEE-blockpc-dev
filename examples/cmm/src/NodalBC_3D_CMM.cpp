#include "NodalBC_3D_CMM.hpp"

NodalBC_3D_CMM::NodalBC_3D_CMM( const int &nFunc, const bool &is_all_node )
{
  if( is_all_node )
  {
    num_nbc = 1;
    const int nbc_id = 0;

    per_slave_nodes.resize(num_nbc);
    per_master_nodes.resize(num_nbc);
    num_per_nodes.resize(num_nbc);

    per_slave_nodes[nbc_id].clear();
    per_master_nodes[nbc_id].clear();
    num_per_nodes[nbc_id] = 0;

    dir_nodes.resize(     num_nbc );
    num_dir_nodes.resize( num_nbc );

    dir_nodes[nbc_id].clear();

    for(unsigned int ii=0; ii<static_cast<unsigned int>(nFunc); ++ii)
      dir_nodes[nbc_id].push_back( ii );

    VEC_T::sort_unique_resize( dir_nodes[nbc_id] );

    num_dir_nodes[nbc_id] = dir_nodes[nbc_id].size();
  }
  else
  {
    num_nbc = 0;
    per_slave_nodes.clear();
    per_master_nodes.clear();
    num_per_nodes.clear();

    dir_nodes.clear();
    num_dir_nodes.clear();
  }

  Create_ID( nFunc );

  if( is_all_node )
    std::cout<<"===> NodalBC_3D_CMM specified by ALL nodes\n     is generated.\n";
  else
    std::cout<<"===> NodalBC_3D_CMM: NO nodal BC is generated.\n";
}


NodalBC_3D_CMM::NodalBC_3D_CMM(
    const INodalBC * const &nbc_inflow,
    const INodalBC * const &nbc_ring,
    const INodalBC * const &nbc_wall,
    const int &comp, const int &nFunc,
    const int &cmm_bc_type )
{
  num_nbc = 1;
  const int nbc_id = 0;

  per_slave_nodes.resize(num_nbc);
  per_master_nodes.resize(num_nbc);
  num_per_nodes.resize(num_nbc);

  per_slave_nodes[nbc_id].clear();
  per_master_nodes[nbc_id].clear();
  num_per_nodes[nbc_id] = 0;

  dir_nodes.resize(     num_nbc );
  num_dir_nodes.resize( num_nbc );

  dir_nodes[nbc_id].clear();

  const int ringbc_type = nbc_ring -> get_ring_bc_type();

  switch( cmm_bc_type ) 
  {
    // ======================== Deformable wall ========================
    case 0:
    {
      // regardless of comp, assign all interior inlet nodes as nodal/essential bc
      const int num_nbc_inflow = nbc_inflow -> get_num_nbc(); 
      for(int nbc_id_inflow = 0; nbc_id_inflow < num_nbc_inflow; ++nbc_id_inflow)
      {
        for(unsigned int ii=0; ii<nbc_inflow->get_num_dir_nodes(nbc_id_inflow); ++ii)
          dir_nodes[nbc_id].push_back( nbc_inflow->get_dir_nodes(nbc_id_inflow, ii) );
      }

      // obtain the type of ring nodes' specification
      const std::vector<int> cap_id = nbc_ring -> get_cap_id();

      // Record the number of nodes assigned as essential bc per dof
      std::vector<int> cap_num_dir_nodes( nbc_ring->get_num_caps(), 0 );

      const int nbc_id_ring = 0; 

      if( ringbc_type == 0 )
      {
        // regardless of comp, all ring nodes are added as essential bc
        for(unsigned int ii = 0; ii < nbc_ring->get_num_dir_nodes(nbc_id_ring); ++ii)
        {
          dir_nodes[nbc_id].push_back( nbc_ring->get_dir_nodes(nbc_id_ring, ii) );
          cap_num_dir_nodes[ cap_id[ii] ] += 1;
        }
      }
      else if( ringbc_type == 1 )
      {
        // if comp is 0 (rotated x-comp, corresponding to the normal comp),
        // ring node is added to dir_nodes
        for(unsigned int ii = 0; ii < nbc_ring->get_num_dir_nodes(nbc_id_ring); ++ii)
        {
          if( comp == 0 )
          {
            dir_nodes[nbc_id].push_back( nbc_ring->get_dir_nodes(nbc_id_ring, ii) );
            cap_num_dir_nodes[ cap_id[ii] ] += 1;
          }
        }
      }
      else SYS_T::print_fatal( "NodalBC_3D_CMM Error: No such type of essential bc for ring nodes.\n" );

      // Clean up the dir_nodes and generate ID array
      VEC_T::sort_unique_resize( dir_nodes[nbc_id] );

      num_dir_nodes[nbc_id] = dir_nodes[nbc_id].size();

      Create_ID( nFunc );

      std::cout<<"===> NodalBC_3D_CMM for deformable wall (cmmbc_type = 0) specified by \n";
      std::cout<<"     interior of inlet surface(s)"<<std::endl;

      for(int ii=0; ii<nbc_ring -> get_num_caps(); ++ii)
        std::cout<<"     outline of cap surface " << ii << ": " << cap_num_dir_nodes[ii] << " nodes" << std::endl;
      std::cout<<"     is generated. \n";

      break;
    }

    // ======================== Rigid wall ========================
    case 1:
    {
      // regardless of comp, assign all interior inlet nodes as nodal/essential bc
      const int num_nbc_inflow = nbc_inflow -> get_num_nbc(); 
      for(int nbc_id_inflow = 0; nbc_id_inflow < num_nbc_inflow; ++nbc_id_inflow)
      {
        for(unsigned int ii=0; ii<nbc_inflow->get_num_dir_nodes(nbc_id_inflow); ++ii)
          dir_nodes[nbc_id].push_back( nbc_inflow->get_dir_nodes(nbc_id_inflow, ii) );
      }

      const int nbc_id_ring = 0; 

      // Assign the ring nodes for this type of nodal/essential bc
      for(unsigned int ii=0; ii<nbc_ring->get_num_dir_nodes(nbc_id_ring); ++ii)
        dir_nodes[nbc_id].push_back( nbc_ring->get_dir_nodes(nbc_id_ring, ii) );

      const int nbc_id_wall = 0; 

      // Assign the wall nodes for this type of nodal/essential bc
      for(unsigned int ii=0; ii<nbc_wall->get_num_dir_nodes(nbc_id_wall); ++ii)
        dir_nodes[nbc_id].push_back( nbc_wall->get_dir_nodes(nbc_id_wall, ii) );
      
      // Clean up the dir_nodes and generate ID array
      VEC_T::sort_unique_resize( dir_nodes[nbc_id] );

      num_dir_nodes[nbc_id] = dir_nodes[nbc_id].size();

      Create_ID( nFunc );

      std::cout<<"===> NodalBC_3D_CMM for rigid wall (cmmbc_type = 1) specified by \n";
      std::cout<<"     interior of inlet surface, ring nodes, as well as the wall nodes\n";
      std::cout<<"     is generated. \n";

      break;
    }

    // ======================== Wall prestress solver ========================
    case 2:
    {
      // prepare ring node indices
      std::vector<unsigned int> ring_gnode;
      ring_gnode.clear();

      const int nbc_id_ring = 0; 

      for(unsigned int ii=0; ii<nbc_ring->get_num_dir_nodes(nbc_id_ring); ++ii)
        ring_gnode.push_back( nbc_ring->get_dir_nodes(nbc_id_ring, ii) );
   
      // prepare wall node indices
      std::vector<unsigned int> wall_gnode;
      wall_gnode.clear();

      const int nbc_id_wall = 0; 

      for(unsigned int ii=0; ii<nbc_wall->get_num_dir_nodes(nbc_id_wall); ++ii)
        wall_gnode.push_back( nbc_wall->get_dir_nodes(nbc_id_wall, ii) );
   
      for(unsigned int ii=0; ii<static_cast<unsigned int>(nFunc); ++ii)
      {
        if( VEC_T::is_invec(ring_gnode, ii) )
        {
          if( ringbc_type == 0 ) dir_nodes[nbc_id].push_back( ii );
          else if( ringbc_type == 1 )
          {
            // if comp is 0 (rotated x-comp, corresponding to the normal comp),
            // ring node is added to dir_nodes
            if( comp == 0 ) dir_nodes[nbc_id].push_back( ii ); 
          }
          else SYS_T::print_fatal( "NodalBC_3D_CMM Error: No such type of essential bc for ring nodes.\n" );
   
        }
        else if( !VEC_T::is_invec( wall_gnode, ii) )
          dir_nodes[nbc_id].push_back( ii );
      }

      // Clean up the dir_nodes and generate ID array
      VEC_T::sort_unique_resize( dir_nodes[nbc_id] );

      num_dir_nodes[nbc_id] = dir_nodes[nbc_id].size();

      Create_ID( nFunc );

      std::cout<<"===> NodalBC_3D_CMM for prestress generation (cmmbc_type = 2) specified by \n";
      std::cout<<"     the whole domain, with the exception of ring and wall nodes,\n";
      std::cout<<"     is generated. \n";

      break;
    }

    default:

      SYS_T::print_fatal( "NodalBC_3D_CMM Error: No such type of CMM BC.\n" );
      break;

  } // end switch
}

// EOF
