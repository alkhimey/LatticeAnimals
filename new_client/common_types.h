/**
 * @author  Artium Nihamkin <artium@nihamkin.com>
 * @date February 2015 
 *
 * @section DESCRIPTION
 * 
 * Basic type definition that are common throughout the whole project.
 *
 */



#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

typedef unsigned short PolyDim;
typedef unsigned short PolySize;

typedef long long int  PolyCount;


struct JobProperty {
  PolySize n;
  PolySize split_n;
  PolyCount low;
  PolyCount high;
};



#endif
