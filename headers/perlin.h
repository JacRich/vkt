#ifndef PERLIN_H
#define PERLIN_H

double perlin_3d(double, double, double);

double perlin_sampleOctave(double x, double y, double z, double persistence, int samples);

#endif

