/* 
 * File:   SectionDataType.h
 * Author: zack
 *
 * Created on 16 septembre 2015, 10:58
 */

#ifndef SECTIONDATATYPE_H
#define	SECTIONDATATYPE_H

#include <mpi.h>

#define SECTION_SIZE 3

typedef struct{
    int temperature[SECTION_SIZE][SECTION_SIZE];
}t_section_datatype;

#endif	/* SECTIONDATATYPE_H */
