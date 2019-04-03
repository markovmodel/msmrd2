//
// Created by maojrs on 4/1/19.
//

#pragma once
#include <vector>
#include <tuple>
#include <memory>
#include "quaternion.hpp"
#include "quaternionPartition.hpp"
#include "tools.hpp"
#include "vec3.hpp"


namespace msmrd {
    /*
     * This class discretizes the relative position and relative orientation (given by a quaternion) of
     * two rigid bodies. In order to do so, it uses the spherePartition and the quaternion Partition. The first one
     * to discretize the relative position vector (mainly its direction), and the latter one to discretize the relative
     * orientation. Overall the space spanned by the relative position and orientation is effectively six dimensional,
     * so this provides effectively a discretization of 6 dimensional space (5 if only the direction of the relative
     * position is discretized).
     *
     * Note the surface sphere partition to discretize the relative position should be fixed to one of the particles,
     * say to particle 1, i.e. they particle rotates the partition. As it is simpler to leave the partition in place,
     * we simply rotate the relative position back to the initial orientation of particle 1.
     */
    class positionOrientationPartition {
    public:
        int numSphericalSectionsPos;
        double relativeDistanceCutOff;
        int numRadialSectionsQuat;
        int numSphericalSectionsQuat;
        int numTotalSections = 0;
        std::unique_ptr<spherePartition> sphericalPartition;
        std::unique_ptr<quaternionPartition> quatPartition;
        /**
         * @param numSphericalSectionsPos number of sections in the surface partition of the unit sphere to discretize
         * the direction of the relative position.
         * @param reltiveDistanceCutOff determines the cutoff for the relative position, after this value is exceeded,
         * the partition is no longer effective and the getSectionNumber function returns 0 (unbound state)
         * @param numRadialSectionsQuat number of radial sections in volumetric 3D sphere partition used to
         * discretize the relative orientation quaternion
         * @param numSphericalSectionsQuat number of spherical sections in each shell of the quaternion discretization.
         * Each radial shell in the quaternion partition has numSphericalShells section in it.
         * @param numTotalSections total number of sections used to discretize relative position and orientation
         * @param sphericalPartition pointer to spherical partition that discretizes the direction
         * of the relative position vector in the surface of a unit 3D sphere.
         * @param quatPartition pointer to quaternion partition that discretizes the relative orientation (quaternion)
         * in the volume of a unit sphere.
         */

        positionOrientationPartition(double relativeDistanceCutOff, int numSphericalSectionsPos,
                                     int numRadialSectionsQuat, int numSphericalSectionsQuat);

        /* Gets the section number in this partition given a relative position and relative orientation.
         * Note in order to get consistent results relativePosition should be rotated by particle1.quat.conjugated(),
         * which is passed in as quaternionReference. This is equivalent to fixing the spherical surface partition
         * to particle1, so they rotate together. */
        int getSectionNumber(vec3<double> relativePosition, quaternion<double> relativeQuaternion,
                             quaternion<double> quaternionReference = {1,0,0,0});

        /* Gets interval delimiters of the phase space section corresponding to a given section number.
         * The function returns a tuple (secNumRelativePos, secNumRelativeQuat): the section number corresponding
         * to the surface spherical partition, and the section number corresponding to the quaternion partition,
         * respectively. The exact intervals can then be extracted by calling
         * spherePartition->getAngles(secNumRelativePos) and quaternion->getSectionintervals(secNumRelativeQuat)*/
        std::tuple<int, int> getSection(int secNumber);


        /* Other not so important functions (mostly for PyBindings)*/

        // Returns total number of sections, num radial sections and number of spherical sections (for pybind)
        std::vector<int> getNumSections(){
            return std::vector<int>{numTotalSections, numSphericalSectionsPos,
                                    numRadialSectionsQuat, numSphericalSectionsQuat};
        }

        int getSectionNumberPyBind(std::vector<double> relpos, std::vector<double> relquat, std::vector<double> qref );

    };

}