#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <CGAL/Cartesian.h>
#include <CGAL/global_functions_spherical_kernel_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Sphere_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Min_sphere_of_points_d_traits_3.h>
#include <CGAL/Min_sphere_d.h>
#include <CGAL/Exact_spherical_kernel_3.h>

#include "consts.h"

typedef unsigned int uint;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_spherical_kernel_3						SK;

//2D definitions
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Line_2 Line_2;
typedef CGAL::Polygon_2<K> Polygon;

//3D definitions
typedef K::Point_3 Point_3;
typedef K::Segment_3 Segment_3;
typedef K::Line_3 Line_3;
typedef K::Plane_3 Plane_3;
typedef K::Sphere_3 Sphere;
typedef K::Circle_3 Circle_3;
typedef K::Vector_3 Vector_3;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef CGAL::Min_sphere_of_points_d_traits_3<K, double> Min_Sphere_Traits;

using namespace std;