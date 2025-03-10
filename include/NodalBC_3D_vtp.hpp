#ifndef NODALBC_3D_VTP_HPP
#define NODALBC_3D_VTP_HPP
// ==================================================================
// NodalBC_3D_vtp.hpp
//
// This is an instantiation of INodalbc for 3D problems by reading the
// dirichlet nodes from the vtp file.
//
// This class is designed to handle the mesh for unstructural tetrahedral
// mesh generated by automatic mesher like tetgen.
// 
// The data contained in this class include:
// dir_nodes : the nodal indices for the Dirichlet nodes;
// num_dir_nodes : the number of the Dirichlet nodes, i.e., the length
//                 of the dir_nodes array;
// per_slave_nodes : the nodal indices for the slave nodes;
// per_master_nodes : the nodal indices for the master nodes;
// num_per_nodes : the number of periodic-type nodes, i.e., the length 
//                 of the per_slave_nodes / per_master_nodes.
//
// ID : the vector for the ID array, which is generated based on the 
//      nFunc, the number of total basis functions, and the dir_nodes
//      and per_slave_nodes/per_master_nodes. Once the dir_nodes, per_
//      xxx_nodes, and nFunc are given, the ID array will be generated
//      by the function create_ID.
//
// Date: Jan. 6 2017
// Author: Ju Liu
// ==================================================================
#include "INodalBC.hpp"
#include "Tet_Tools.hpp"

class NodalBC_3D_vtp : public INodalBC
{
  public:
    // --------------------------------------------------------------
    // Default constructor: clear the dir_nodes, per_slave_nodes,
    // per_master_nodes; set num_dir_nodes, num_per_nodes to be zero;
    // set ID based on the above "no-nodal bc" setting.
    // --------------------------------------------------------------
    NodalBC_3D_vtp( const int &nFunc );
    
    // --------------------------------------------------------------
    // The vtp file specifies the Dirichlet nodes. No periodical BC.
    // --------------------------------------------------------------
    NodalBC_3D_vtp( const std::string &vtpfileName, const int &nFunc );

    // --------------------------------------------------------------
    // The list of vtp files specifies the Dirichlet nodes. 
    // No periodical type BC nodes.
    // --------------------------------------------------------------
    NodalBC_3D_vtp( const std::vector<std::string> &vtpfileList, 
        const int &nFunc );

    // --------------------------------------------------------------
    // Set a master-slave constraint relation, nodes in the given
    // file will follow the given master index. In each vtp file, the
    // master_idx[ii]-th node will be set as master and the rest nodes
    // are slave nodes. 
    // This BC type is useful in the constrained motion on the boundary.
    // I use this in my tensile test for a adventitial strp to disallow
    // deformation on the boundary of the loaded surface.
    // NOTE: Make sure that master_idx[ii] is smaller than the number 
    // of nodes in the vtpfileList[ii].
    // --------------------------------------------------------------
    NodalBC_3D_vtp( const std::vector<std::string> &vtpfileList, 
        const int &nFunc, const std::vector<int> &master_idx );

    // --------------------------------------------------------------
    // General NodalBC constructor. The specific implementation of the 
    // Nodal bc is given by the private funcitons and are chosen by 
    // the type flag.
    // --------------------------------------------------------------
    NodalBC_3D_vtp( const std::vector<std::string> &vtpfileList,
        const int &nFunc, const int &type );

    virtual ~NodalBC_3D_vtp();

    virtual unsigned int get_dir_nodes(const unsigned int &ii) const
    {return dir_nodes[ii];}

    virtual unsigned int get_per_slave_nodes(const unsigned int &ii) const
    {return per_slave_nodes[ii];}

    virtual unsigned int get_per_master_nodes(const unsigned int &ii) const
    {return per_master_nodes[ii];}

    virtual unsigned int get_num_dir_nodes() const {return num_dir_nodes;}

    virtual unsigned int get_num_per_nodes() const {return num_per_nodes;}

  private:
    std::vector<unsigned int> dir_nodes;
    unsigned int num_dir_nodes;

    std::vector<unsigned int> per_slave_nodes, per_master_nodes;
    unsigned int num_per_nodes;
    
    NodalBC_3D_vtp() {};

    // --------------------------------------------------------------
    // BC_type_1 is a master-slave driven implementation. It is 
    // identical to the NodalBC_3D_vtp(vtpfilelist, nFunc, master_idx),
    // with master_idx = [0,0,...,0].
    // --------------------------------------------------------------
    void BC_type_1( const std::vector<std::string> &vtpfileList,
        const int &nFunc  );

    // --------------------------------------------------------------
    // BC_type_2 is a bc that sets Dirichlet on the first vtp surface
    // and set master-slave on the second vtp surface with master
    // node the node 0 in that vtp file.
    // This is used for the tensile test for arterial strips,
    // the first vtp file specifies the surface where the strip
    // is fixed; the second file specifies the surface where the
    // pulling force is applied.
    // --------------------------------------------------------------
    void BC_type_2( const std::vector<std::string> &vtpfileList,
        const int &nFunc  );

    // --------------------------------------------------------------
    // BC_type_3 is a bc that sets Dirichlet on the given vtp file's
    // first node only. This is useful for Stokes/Navier-Stokes 
    // pressure boundary condition.
    // Note: the input vtifileList should have size 1, which means
    // only one vtp file is provided.
    // --------------------------------------------------------------
    void BC_type_3( const std::vector<std::string> &vtpfileList,
        const int &nFunc  );
};

#endif
