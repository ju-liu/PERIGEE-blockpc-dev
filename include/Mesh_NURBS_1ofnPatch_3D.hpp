#ifndef MESH_NURBS_1OFNPATCH_3D_HPP
#define MESH_NURBS_1OFNPATCH_3D_HPP

#include "IMesh.hpp"
#include "Vec_Tools.hpp"

class Mesh_NURBS_1ofnPatch_3D : public IMesh
{
  public:
    Mesh_NURBS_1ofnPatch_3D( int in_s_degree, int in_t_degree, int in_u_degree,
        double input_hx_max, double input_hy_max, double input_hz_max,
        double input_hx_min, double input_hy_min, double input_hz_min,
        const std::vector<double> &in_sknot, const std::vector<double> &in_tknot,
        const std::vector<double> &in_uknot, const int &pindex, 
        const int &in_elem_start, const int &in_func_start );

    // ------------------------------------------------------------------------
    // This constructor generates a Mesh object what gives an incomplete mesh
    // object for code testing purpose. This constructor does not rely on the
    // knot vector input.
    // ------------------------------------------------------------------------
    Mesh_NURBS_1ofnPatch_3D( const int &in_s_degree, const int &in_t_degree,
        const int &in_u_degree, const double &input_hx_max, const double &input_hy_max,
        const double &input_hz_max, const double &input_hx_min,
        const double &input_hy_min, const double &input_hz_min,
        const int &inelemx, const int &inelemy, const int &inelemz,
        const int &in_pindex, const int &in_elem_start, const int &in_func_start );

    virtual ~Mesh_NURBS_1ofnPatch_3D();

    virtual void print_mesh_info() const;

    virtual int get_s_degree() const {return s_degree;}
    virtual int get_t_degree() const {return t_degree;}
    virtual int get_u_degree() const {return u_degree;}

    virtual int get_nFunc_x() const {return nFunc_x;}
    virtual int get_nFunc_y() const {return nFunc_y;}
    virtual int get_nFunc_z() const {return nFunc_z;}
    virtual int get_nFunc() const {return nFunc;}

    virtual int get_nElem_x() const {return nElem_x;}
    virtual int get_nElem_y() const {return nElem_y;}
    virtual int get_nElem_z() const {return nElem_z;}
    virtual int get_nElem() const {return nElem;}

    virtual int get_nLocBas() const {return nLocBas;}

    virtual double get_hx_max() const {return hx_max;}
    virtual double get_hy_max() const {return hy_max;}
    virtual double get_hz_max() const {return hz_max;}

    virtual double get_hx_min() const {return hx_min;}
    virtual double get_hy_min() const {return hy_min;}
    virtual double get_hz_min() const {return hz_min;}

    virtual double get_hx( int ee ) const;
    virtual double get_hy( int ee ) const;
    virtual double get_hz( int ee ) const;

    virtual int get_patch_index() const {return patch_index;}
    virtual int get_nElem_start() const {return elem_start;}
    virtual int get_nFunc_start() const {return func_start;}

    virtual void get_elem_index( const int &ee, 
        int &ex, int &ey, int &ez) const;

  private:
    double hx_max, hy_max, hz_max;
    double hx_min, hy_min, hz_min;
    int s_degree, t_degree, u_degree;
    int nFunc_x, nFunc_y, nFunc_z, nFunc;
    int nElem_x, nElem_y, nElem_z, nElem;
    int nLocBas;
    std::vector<double> hx, hy, hz;
    
    const int patch_index;
    const int elem_start;
    const int func_start;
};


#endif
