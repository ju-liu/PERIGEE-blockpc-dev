#ifndef IIEN_HPP
#define IIEN_HPP
// ==================================================================
// IIEN.hpp
// This is an interface file for the IEN array classes.
//
// Author: Ju Liu
// Date: Sept. 24th 2013
// ==================================================================
#include <cstdlib>
#include <iostream>

class IIEN
{
  public:
    IIEN(){};

    virtual ~IIEN(){};

    // get the IEN arrray for element e at local node l_node
    virtual int get_IEN( const int &ee, const int &l_node ) const = 0;

    // get the number of local basis functions (per element)
    virtual int get_nLocBas( const int &ee = 0 ) const = 0;

    // print IEN array
    virtual void print_IEN() const = 0;

    // print info
    virtual void print_info() const
    {std::cerr<<"Error: print_info is not implemented. \n"; exit(EXIT_FAILURE);}
};

#endif
